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
const int EXCEPTION_VECTOR_NUM_CACHELINES = 96;
const uint32_t SBE_BOOTLOADER_VERSION = 0x901;
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
    //the branching instruction for 12KB past where it currently is (1024 * 12 = 12288 = 0x3000)
    //The branch instruction is 0100 10_address to branch to_ 0  0
    //                      0      6                   29 30 31
    //bit 30 is for absolute address (since it is not set this is relative)
    const uint32_t l_branch_to_12 = 0x4800C000ull;
    uint64_t l_bootloader_offset;
    uint64_t l_hostboot_hrmor_offset;
    uint64_t l_chip_base_address_nm;
    uint64_t l_chip_base_address_m;
    uint64_t l_target_address;
    uint32_t l_exception_instruction;
    bool l_firstAccess = true;
    uint32_t l_num_cachelines_to_roll;
    uint8_t l_data_to_pass_to_pba_array[FABRIC_CACHELINE_SIZE];
    uint32_t l_exception_vector_size = 0;
    int l_cacheline_num = 0;

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

    //Check to see if we need to populate the exception vectors
    //Check the SBE_HBBL_EXCEPTION_INSTRUCT attribute
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SBE_HBBL_EXCEPTION_INSTRUCT, FAPI_SYSTEM, l_exception_instruction),
             "fapiGetAttribute of ATTR_SBE_HBBL_EXCEPTION_INSTRUCT failed!");

    l_target_address = l_chip_base_address_nm;

    BootloaderConfigData_t l_bootloader_config_data;

    l_bootloader_config_data.version = SBE_BOOTLOADER_VERSION;

    //At address X + 0x8 put whatever is in ATTR_SBE_BOOT_SIDE
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SBE_BOOT_SIDE, FAPI_SYSTEM, l_bootloader_config_data.sbeBootSide),
             "fapiGetAttribute of ATTR_SBE_BOOT_SIDE failed!");

    //At address X + 0x9 put whatever is in ATTR_PNOR_BOOT_SIDE
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PNOR_BOOT_SIDE, FAPI_SYSTEM, l_bootloader_config_data.pnorBootSide),
             "fapiGetAttribute of ATTR_PNOR_BOOT_SIDE failed!");

    //At address X + 0xA put whatever is in ATTR_PNOR_SIZE
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PNOR_SIZE, FAPI_SYSTEM, l_bootloader_config_data.pnorSizeMB),
             "fapiGetAttribute of ATTR_PNOR_SIZE failed!");

    // check that the payload size is non-zero and evenly divisible into cachelines
    FAPI_ASSERT(i_payload_size && !(i_payload_size % FABRIC_CACHELINE_SIZE),
                fapi2::P9_SBE_LOAD_BOOTLOADER_INVALID_PAYLOAD_SIZE().
                set_CHIP_TARGET(i_master_chip_target).
                set_EX_TARGET(i_master_ex_target).
                set_PAYLOAD_SIZE(i_payload_size),
                "Payload size is invalid!");

    // move data using PBA setup/access HWPs

    if (l_exception_instruction != 0x0)
    {
        l_exception_vector_size = EXCEPTION_VECTOR_NUM_CACHELINES * FABRIC_CACHELINE_SIZE;
    }

    while (l_target_address < (l_chip_base_address_nm + i_payload_size + l_exception_vector_size))
    {
        // invoke PBA setup HWP to prep stream
        FAPI_TRY(p9_pba_setup( i_master_chip_target,
                               i_master_ex_target,
                               l_target_address,
                               PBA_HWP_WRITE_OP,
                               PBA_HWP_FLAGS,
                               l_num_cachelines_to_roll), "Error from p9_pba_setup");

        l_firstAccess = true;

        // call PBA access HWP per cacheline to move payload data
        while (l_num_cachelines_to_roll &&
               (l_target_address < (l_chip_base_address_nm + i_payload_size + l_exception_vector_size)))
        {
            if ((l_cacheline_num == 0) && (l_exception_instruction != 0))
            {
                //This is for the first cacheline of data that has the branch, pnor_size, and pnor_boot_side in it
                //The rest of the exception vector is what was in SBE_HBBL_EXCEPTION_INSTRUCT replicated multiple times (until the end of 12KB of exception vector data)
                for (uint32_t i = 0; i < FABRIC_CACHELINE_SIZE; i++)
                {
                    //At address X put whatever is in l_branch_to_12
                    if (i < 4)
                    {
                        l_data_to_pass_to_pba_array[i] = (l_branch_to_12 >> (24 - 8 * i )) & 0xFF;
                    }
                    //At address X + 0x4 put the HBBL_STRUCT_VERSION
                    else if (i < 8)
                    {
                        l_data_to_pass_to_pba_array[i] = (l_bootloader_config_data.version >> (24 - 8 * ((i - 4) % 4))) & 0xFF;
                    }
                    //At address X + 0x8 put the SBE_BOOT_SIDE
                    else if (i == 8)
                    {
                        l_data_to_pass_to_pba_array[i] = l_bootloader_config_data.sbeBootSide;
                    }
                    //At address X + 0x9 put the PNOR_BOOT_SIDE
                    else if (i  == 9)
                    {
                        l_data_to_pass_to_pba_array[i] = l_bootloader_config_data.pnorBootSide;
                    }
                    //At address X + 0xA pu the PNOR_SIZE
                    else if (i == 10)
                    {
                        l_data_to_pass_to_pba_array[i] = l_bootloader_config_data.pnorSizeMB >> 8 & 0xFF;
                    }
                    else if (i == 11)
                    {
                        l_data_to_pass_to_pba_array[i] = l_bootloader_config_data.pnorSizeMB & 0xFF;
                    }
                    //Fill the rest with the exception vector instruction
                    else
                    {
                        l_data_to_pass_to_pba_array[i] = (l_exception_instruction >> (24 - 8 * (i % 4))) & 0xFF;
                    }
                }
            }
            else if ((l_cacheline_num  == 1) && (l_exception_instruction != 0))
            {
                //This is for the other 95 cachelines that we are sending
                for (uint32_t i = 0; i < FABRIC_CACHELINE_SIZE; i++)
                {
                    l_data_to_pass_to_pba_array[i] = (l_exception_instruction >> (24 - 8 * (i % 4))) & 0xFF;
                }
            }
            else if ((l_cacheline_num >= EXCEPTION_VECTOR_NUM_CACHELINES) || (l_exception_instruction == 0))
            {
                //This is for the data after the exception vector
                for (uint32_t i = 0; i < FABRIC_CACHELINE_SIZE; i++)
                {
                    l_data_to_pass_to_pba_array[i] = i_payload_data[((l_cacheline_num - (l_exception_vector_size / FABRIC_CACHELINE_SIZE)) *
                                                     FABRIC_CACHELINE_SIZE)
                                                     + i];
                }
            }

            FAPI_TRY(p9_pba_access(i_master_chip_target,
                                   l_target_address,
                                   PBA_HWP_WRITE_OP,
                                   PBA_HWP_FLAGS,
                                   l_firstAccess,
                                   (l_num_cachelines_to_roll == 1) ||
                                   ((l_target_address + FABRIC_CACHELINE_SIZE) >
                                    (l_chip_base_address_nm + i_payload_size + l_exception_vector_size)),
                                   l_data_to_pass_to_pba_array), "Error from p9_pba_access");
            l_firstAccess = false;
            // decrement count of cachelines remaining in current stream
            l_num_cachelines_to_roll--;

            // stride address/payload data pointer offset to next cacheline
            l_target_address += FABRIC_CACHELINE_SIZE;
            l_cacheline_num++;
        }
    }

fapi_try_exit:
    FAPI_DBG("End");
    return fapi2::current_err;
}

