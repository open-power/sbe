/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/nest/p9_sbe_load_bootloader.C $       */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* EKB Project                                                            */
/*                                                                        */
/* COPYRIGHT 2015,2016                                                    */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
//--------------------------------------------------------------------------
//
// @file p9_sbe_load_bootloader.C
// @brief Shift HB bootloader payload from SEEPROM to L3 cache of master core via PBA
//
// *HWP HWP Owner: Christina Graves clgraves@us.ibm.com
// *HWP HWP Writer: Murey A Luna Torres malunato@us.ibm.com, Joseph McGill jmcgill@us.ibm.com
// *HWP FW Owner: Thi Tran thi@us.ibm.com
// *HWP Team: Nest
// *HWP Level: 2
// *HWP Consumed by: SBE
//
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <p9_sbe_load_bootloader.H>
#include <p9_pba_setup.H>
#include <p9_pba_access.H>
#include <p9_fbc_utils.H>

//-----------------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------------

// PBA setup/access HWP call constants
const bool PBA_HWP_WRITE_OP = false;
const uint32_t PBA_HWP_FLAGS = FLAG_FASTMODE | // fastmode
                               ((p9_pba_write_ttype::LCO << FLAG_TTYPE_SHIFT) & p9_pba_flags::FLAG_TTYPE);   // LCO_M

//-----------------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------------

fapi2::ReturnCode p9_sbe_load_bootloader(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_master_chip_target,
    const fapi2::Target<fapi2::TARGET_TYPE_EX>& i_master_ex_target,
    const uint64_t i_payload_size,
    uint8_t* i_payload_data)
{
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    uint64_t l_bootloader_offset;
    uint64_t l_hostboot_hrmor_offset;
    uint64_t l_chip_base_address_nm;
    uint64_t l_chip_base_address_m;
    uint64_t l_target_address;
    uint64_t l_payload_data_offset;
    bool firstAccess = true;

    FAPI_DBG("Start");

    // read platform initialized attributes needed to determine target base address
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SBE_BOOTLOADER_OFFSET, FAPI_SYSTEM, l_bootloader_offset),
             "Error from FAPI_ATTR_GET (ATTR_SBE_BOOTLOADER_OFFSET)");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_HOSTBOOT_HRMOR_OFFSET, FAPI_SYSTEM, l_hostboot_hrmor_offset),
             "Error from FAPI_ATTR_GET (ATTR_HOSTBOOT_HRMOR_OFFSET)");

    // target base address = (chip non-mirrored base address) +
    //                       (hostboot HRMOR offset) +
    //                       (bootloader offset)

    FAPI_TRY(p9_fbc_utils_get_chip_base_address(i_master_chip_target,
             l_chip_base_address_nm,
             l_chip_base_address_m),
             "Error from p9_fbc_utils_get_chip_base_address");

    // add hostboot HRMOR offset and bootloader offset contributions
    l_chip_base_address_nm += l_hostboot_hrmor_offset;
    l_chip_base_address_nm += l_bootloader_offset;

    // check that base address is cacheline aligned
    FAPI_ASSERT(!(l_chip_base_address_nm % FABRIC_CACHELINE_SIZE),
                fapi2::P9_SBE_LOAD_BOOTLOADER_INVALID_TARGET_ADDRESS().
                set_CHIP_TARGET(i_master_chip_target).
                set_EX_TARGET(i_master_ex_target).
                set_TARGET_BASE_ADDRESS(l_chip_base_address_nm).
                set_HRMOR_OFFSET(l_hostboot_hrmor_offset).
                set_BOOTLOADER_OFFSET(l_bootloader_offset),
                "Target base address is not cacheline aligned!");
    // check that the payload size is non-zero and evenly divisible into cachelines
    FAPI_ASSERT(i_payload_size && !(i_payload_size % FABRIC_CACHELINE_SIZE),
                fapi2::P9_SBE_LOAD_BOOTLOADER_INVALID_PAYLOAD_SIZE().
                set_CHIP_TARGET(i_master_chip_target).
                set_EX_TARGET(i_master_ex_target).
                set_PAYLOAD_SIZE(i_payload_size),
                "Payload size is invalid!");

    // move data using PBA setup/access HWPs
    l_target_address = l_chip_base_address_nm;
    l_payload_data_offset = 0;

    while (l_target_address < (l_chip_base_address_nm + i_payload_size))
    {
        // invoke PBA setup HWP to prep stream
        uint32_t l_num_cachelines_to_roll;
        FAPI_TRY(p9_pba_setup( i_master_chip_target,
                               i_master_ex_target,
                               l_target_address,
                               PBA_HWP_WRITE_OP,
                               PBA_HWP_FLAGS,
                               l_num_cachelines_to_roll), "Error from p9_pba_setup");

        firstAccess = true;

        // call PBA access HWP per cacheline to move payload data
        while (l_num_cachelines_to_roll &&
               (l_target_address < (l_chip_base_address_nm + i_payload_size)))
        {

            FAPI_TRY(p9_pba_access(i_master_chip_target,
                                   l_target_address,
                                   PBA_HWP_WRITE_OP,
                                   PBA_HWP_FLAGS,
                                   firstAccess,
                                   (l_num_cachelines_to_roll == 1) ||
                                   ((l_target_address + FABRIC_CACHELINE_SIZE) >
                                    (l_chip_base_address_nm + i_payload_size)),
                                   i_payload_data + l_payload_data_offset), "Error from p9_pba_access");
            firstAccess = false;
            // decrement count of cachelines remaining in current stream
            l_num_cachelines_to_roll--;

            // stride address/payload data pointer offset to next cacheline
            l_target_address += FABRIC_CACHELINE_SIZE;
            l_payload_data_offset += (FABRIC_CACHELINE_SIZE / sizeof(uint8_t));
        }

    }

fapi_try_exit:
    FAPI_DBG("End");
    return fapi2::current_err;
}

