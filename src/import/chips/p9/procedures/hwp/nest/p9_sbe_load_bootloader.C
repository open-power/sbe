/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/nest/p9_sbe_load_bootloader.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2017                        */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */

///
/// @file p9_sbe_load_bootloader.C
/// @brief Shift HB bootloader payload from SEEPROM to L3 cache of master core
///        via PBA
///

// *HWP HWP Owner: Joe McGill jmcgill@us.ibm.com
// *HWP FW Owner: Thi Tran thi@us.ibm.com
// *HWP Team: Nest
// *HWP Level: 3
// *HWP Consumed by: SBE

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <p9_sbe_load_bootloader.H>
#include <p9_sbe_hb_structures.H>

#include <p9_quad_scom_addresses.H>
#include <p9_quad_scom_addresses_fld.H>
#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>

#include <p9_pba_setup.H>
#include <p9_pba_access.H>
#include <p9_pba_coherent_utils.H>
#include <p9_fbc_utils.H>
#include <p9_ram_core.H>

//------------------------------------------------------------------------------
// Macro definitions
//------------------------------------------------------------------------------
#define PACK_1B(ARR,INDEX,VAL) \
    { \
        ARR[INDEX++] =  VAL        & 0xFFULL; \
    }
#define PACK_2B(ARR,INDEX,VAL) \
    { \
        ARR[INDEX++] = (VAL >> 8)  & 0xFFULL; \
        ARR[INDEX++] =  VAL        & 0xFFULL; \
    }
#define PACK_4B(ARR,INDEX,VAL) \
    { \
        ARR[INDEX++] = (VAL >> 24) & 0xFFULL; \
        ARR[INDEX++] = (VAL >> 16) & 0xFFULL; \
        ARR[INDEX++] = (VAL >> 8)  & 0xFFULL; \
        ARR[INDEX++] =  VAL        & 0xFFULL; \
    }
#define PACK_8B(ARR,INDEX,VAL) \
    { \
        ARR[INDEX++] = (VAL >> 56) & 0xFFULL; \
        ARR[INDEX++] = (VAL >> 48) & 0xFFULL; \
        ARR[INDEX++] = (VAL >> 40) & 0xFFULL; \
        ARR[INDEX++] = (VAL >> 32) & 0xFFULL; \
        ARR[INDEX++] = (VAL >> 24) & 0xFFULL; \
        ARR[INDEX++] = (VAL >> 16) & 0xFFULL; \
        ARR[INDEX++] = (VAL >> 8)  & 0xFFULL; \
        ARR[INDEX++] =  VAL        & 0xFFULL; \
    }

//------------------------------------------------------------------------------
// Constant definitions
//------------------------------------------------------------------------------

// Execption vector constant definitions
const uint32_t EXCEPTION_VECTOR_SIZE = 12 * 1024; // 12KB
const uint32_t EXCEPTION_VECTOR_NUM_CACHELINES = EXCEPTION_VECTOR_SIZE /
        FABRIC_CACHELINE_SIZE;
// PPC branch instruction inserted into first address of exception vector,
// used to jump over entirety of exception vector storage
const uint32_t EXCEPTION_VECTOR_BRANCH = 0x48000000 |
        EXCEPTION_VECTOR_SIZE;

// PBA setup/access HWP call constants
const bool PBA_HWP_WRITE_OP = false;

// static offset between:
//   Core unit target positions 0..23 and their associated
//   Perv unit target positions 32..55
const uint8_t PERV_TO_CORE_POS_OFFSET = 0x20;

// RAM constants
const uint8_t THREAD_1 = 1;
const uint8_t THREAD_2 = 2;
const uint8_t THREAD_3 = 3;
const uint32_t HRMOR_SPR_NUMBER = 313;
const uint32_t PSSCR_SPR_NUMBER = 855;
const uint64_t HOSTBOOT_PSSCR_VALUE = 0x00000000003F00FFULL;

//------------------------------------------------------------------------------
// Function definitions
//------------------------------------------------------------------------------

///
/// @brief Program SPRs via RAM prior to instructions tart
///
/// @param[in] i_master_chip_target Reference to processor chip target
/// @param[in] i_master_ex_target Reference to EX unit target which is the
///            parent of the master core(s)
/// @param[in] i_master_hrmor HRMOR value for master normal core
/// @return fapi::ReturnCode. FAPI2_RC_SUCCESS if success, else error code.
///
fapi2::ReturnCode
ram_sprs(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_master_chip_target,
    const fapi2::Target<fapi2::TARGET_TYPE_EX>& i_master_ex_target,
    const uint64_t i_master_hrmor)
{
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    std::vector<fapi2::Target<fapi2::TARGET_TYPE_CORE>> l_master_core_targets;
    uint8_t l_fused_core_mode;
    uint64_t l_bootloader_offset;
    bool l_master_core_found = false;
    uint8_t l_master_core_unit_pos = 0;

    FAPI_DBG("Start");

    // determine fused core status
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FUSED_CORE_MODE,
                           FAPI_SYSTEM,
                           l_fused_core_mode),
             "Error from FAPI_ATTR_GET (ATTR_FUSED_CORE_MODE)");

    // retrieve master core target position (0..23) index via attribute query
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MASTER_CORE,
                           i_master_chip_target,
                           l_master_core_unit_pos),
             "Error from FAPI_ATTR_GET (ATTR_MASTER_CORE)!");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SBE_BOOTLOADER_OFFSET,
                           FAPI_SYSTEM,
                           l_bootloader_offset),
             "Error from FAPI_ATTR_GET (ATTR_SBE_BOOTLOADER_OFFSET)");

    for (auto& l_core : i_master_ex_target.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        if (l_fused_core_mode)
        {
            l_master_core_targets.push_back(l_core);
            l_master_core_found = true;
        }
        else
        {
            // use this to find the master core target, needed for RAM operation
            // - traverse the children of the master EX target we are passed, and look
            //   for a matching position (via its PERV target parent)
            // - without modifications to the HWP prototype this is the most efficient
            //   solution possible given SBE FAPI platform support
            {
                auto l_perv = l_core.getParent<fapi2::TARGET_TYPE_PERV>();
                uint8_t l_perv_unit_pos = 0;

                FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                                       l_perv,
                                       l_perv_unit_pos),
                         "Error from FAPI_ATTR_GET (ATTR_CHIP_UNIT_POS)!");

                // PERV target positions for cores 0..23 are 32:57, compare using
                // static offset
                if (l_master_core_unit_pos ==
                    (l_perv_unit_pos - PERV_TO_CORE_POS_OFFSET))
                {
                    l_master_core_targets.push_back(l_core);
                    l_master_core_found = true;
                    break;
                }
            }
        }
    }

    // if target is not found, raise an error
    FAPI_ASSERT(l_master_core_found,
                fapi2::P9_MASTER_CORE_NOT_FOUND()
                .set_CHIP_TARGET(i_master_chip_target)
                .set_EX_TARGET(i_master_ex_target)
                .set_MASTER_CORE_UNIT_POS(l_master_core_unit_pos)
                .set_FUSED_MODE(l_fused_core_mode),
                "Error finding the master core target!");

    // set the HRMOR (core) and PSSCR (threads 2/3).  The PSSCR must be
    // set on these threads as in fused mode HB will never execute on them,
    // but CME checks bits in them to perform STOP11 request in step 16
    for (auto& l_master_core_target : l_master_core_targets)
    {
        RamCore l_ram_t1(l_master_core_target, THREAD_1);
        RamCore l_ram_t2(l_master_core_target, THREAD_2);
        RamCore l_ram_t3(l_master_core_target, THREAD_3);
        fapi2::buffer<uint64_t> l_sicr = 0;
        fapi2::buffer<uint64_t> l_thread_info = 0;
        fapi2::buffer<uint64_t> l_ram_data = i_master_hrmor;
        uint8_t l_core_unit_pos = 0;

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               l_master_core_target,
                               l_core_unit_pos),
                 "Error from FAPI_ATTR_GET (ATTR_CHIP_UNIT_POS)!");

        // override to PM_EXIT is required to RAM given current core
        // state -- set appropriate bit in parent EX SICR register
        if (!(l_core_unit_pos % 2))
        {
            l_sicr.setBit<EQ_CME_SCOM_SICR_PM_EXIT_C0>();
        }
        else
        {
            l_sicr.setBit<EQ_CME_SCOM_SICR_PM_EXIT_C1>();
        }

        FAPI_TRY(fapi2::putScom(i_master_ex_target,
                                EX_0_CME_SCOM_SICR_SCOM2,
                                l_sicr),
                 "Error from putScom (EX_0_CME_SCOM_SICR_SCOM2)!");

        // override to PC state is required to RAM given current
        // core state -- set RAM_THREAD_ACTIVE
        l_thread_info.setBit < C_THREAD_INFO_RAM_THREAD_ACTIVE + THREAD_1 > ();
        l_thread_info.setBit < C_THREAD_INFO_RAM_THREAD_ACTIVE + THREAD_2 > ();
        l_thread_info.setBit < C_THREAD_INFO_RAM_THREAD_ACTIVE + THREAD_3 > ();
        FAPI_TRY(fapi2::putScom(l_master_core_target,
                                C_0_THREAD_INFO,
                                l_thread_info),
                 "Error from putScom (C_0_THREAD_INFO)!");

        // call put_reg method to perform RAM operation
        if (l_core_unit_pos != l_master_core_unit_pos)
        {
            l_ram_data -= l_bootloader_offset;
        }

        // set HRMOR via thread 2 instance -- core level SPR
        FAPI_TRY(l_ram_t2.put_reg(REG_SPR, HRMOR_SPR_NUMBER, &l_ram_data),
                 "Error ramming HRMOR (T2)!");
        // set PSSCR via thread specific instances
        l_ram_data = HOSTBOOT_PSSCR_VALUE;
        FAPI_TRY(l_ram_t1.put_reg(REG_SPR, PSSCR_SPR_NUMBER, &l_ram_data),
                 "Error ramming PSSCR (T1)!");
        FAPI_TRY(l_ram_t2.put_reg(REG_SPR, PSSCR_SPR_NUMBER, &l_ram_data),
                 "Error ramming PSSCR (T2)!");
        FAPI_TRY(l_ram_t3.put_reg(REG_SPR, PSSCR_SPR_NUMBER, &l_ram_data),
                 "Error ramming PSSCR (T3)!");
    }

fapi_try_exit:
    FAPI_DBG("End");
    return fapi2::current_err;
}


///
/// @brief Read attributes, determine final image footprint (base address/size)
///
/// @param[in] i_master_chip_target Reference to processor chip target
/// @param[in] i_master_ex_target Reference to EX unit target which is the
///            parent of the master core
/// @param[in] i_payload_size Size of image payload, in B
/// @param[out] o_load_base_address Base real address for bootloader load
/// @param[out] o_load_size Size of complete bootloader payload
/// @param[out] o_load_exception_vector Bootloader payload includes
///             exception vector table?
/// @return fapi::ReturnCode. FAPI2_RC_SUCCESS if success, else error code.
///
fapi2::ReturnCode
calc_image_footprint(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_master_chip_target,
    const fapi2::Target<fapi2::TARGET_TYPE_EX>& i_master_ex_target,
    const uint64_t i_payload_size,
    uint64_t& o_load_base_address,
    uint64_t& o_load_size,
    bool& o_load_exception_vector)
{
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    uint64_t l_drawer_base_address_nm0;
    uint64_t l_drawer_base_address_nm1;
    uint64_t l_drawer_base_address_m;
    uint64_t l_drawer_base_address_mmio;
    uint64_t l_bootloader_offset;
    uint64_t l_hostboot_hrmor_offset;
    uint32_t l_exception_instruction;

    FAPI_DBG("Start");

    // read platform initialized attributes needed to determine target
    // base address for bootloader image load
    //
    // target base address = (drawer non-mirrored base address) +
    //                       (hostboot HRMOR offset) +
    //                       (bootloader offset)
    FAPI_TRY(p9_fbc_utils_get_chip_base_address_no_aliases(
                 i_master_chip_target,
                 ABS_FBC_GRP_ID_ONLY,
                 l_drawer_base_address_nm0,
                 l_drawer_base_address_nm1,
                 l_drawer_base_address_m,
                 l_drawer_base_address_mmio),
             "Error from p9_fbc_utils_get_chip_base_address (drawer)");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_HOSTBOOT_HRMOR_OFFSET,
                           FAPI_SYSTEM,
                           l_hostboot_hrmor_offset),
             "Error from FAPI_ATTR_GET (ATTR_HOSTBOOT_HRMOR_OFFSET)");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SBE_BOOTLOADER_OFFSET,
                           FAPI_SYSTEM,
                           l_bootloader_offset),
             "Error from FAPI_ATTR_GET (ATTR_SBE_BOOTLOADER_OFFSET)");

    // add hostboot HRMOR offset and bootloader offset contributions
    l_drawer_base_address_nm0 += l_hostboot_hrmor_offset;
    l_drawer_base_address_nm0 += l_bootloader_offset;

    // check that base address is cacheline aligned
    FAPI_ASSERT(!(l_drawer_base_address_nm0 % FABRIC_CACHELINE_SIZE),
                fapi2::P9_SBE_LOAD_BOOTLOADER_INVALID_TARGET_ADDRESS()
                .set_CHIP_TARGET(i_master_chip_target)
                .set_EX_TARGET(i_master_ex_target)
                .set_TARGET_BASE_ADDRESS(l_drawer_base_address_nm0)
                .set_HRMOR_OFFSET(l_hostboot_hrmor_offset)
                .set_BOOTLOADER_OFFSET(l_bootloader_offset),
                "Target base address is not cacheline aligned!");

    // read attribute to determine if we should populate the exception vectors
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SBE_HBBL_EXCEPTION_INSTRUCT,
                           FAPI_SYSTEM,
                           l_exception_instruction),
             "Error from FAPI_ATTR_GET (ATTR_SBE_HBBL_EXCEPTION_INSTRUCT)");

    o_load_base_address = l_drawer_base_address_nm0;
    o_load_size = i_payload_size;
    o_load_exception_vector = false;

    if (l_exception_instruction != 0)
    {
        o_load_exception_vector = true;
        o_load_size += EXCEPTION_VECTOR_SIZE;
    }

    // check that the final load size is non-zero and evenly divisible
    // into cachelines
    FAPI_ASSERT(o_load_size && !(o_load_size % FABRIC_CACHELINE_SIZE),
                fapi2::P9_SBE_LOAD_BOOTLOADER_INVALID_PAYLOAD_SIZE()
                .set_CHIP_TARGET(i_master_chip_target)
                .set_EX_TARGET(i_master_ex_target)
                .set_PAYLOAD_SIZE(o_load_size),
                "Payload size is invalid!");

fapi_try_exit:
    FAPI_DBG("End");
    return fapi2::current_err;
}


///
/// @brief Determine data to load for cacheline containing bootloader
///        configuration structure data
///
/// @param[in] i_master_chip_target Reference to processor chip target
/// @param[in] i_load_size Size of complete bootloader payload
/// @param[inout] io_data Pointer to cacheline buffer to fill
/// @return fapi::ReturnCode. FAPI2_RC_SUCCESS if success, else error code.
///
fapi2::ReturnCode
get_bootloader_config_data(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_master_chip_target,
    const uint64_t i_load_size,
    uint8_t* io_data)
{
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    uint64_t l_chip_base_address_nm0;
    uint64_t l_chip_base_address_nm1;
    uint64_t l_chip_base_address_m;
    uint64_t l_chip_base_address_mmio;
    uint64_t l_index = 0;
    // Variable to fetch the Key-Addr Stash Pair
    uint64_t l_stashAddrAttr = 0;
    uint8_t* l_stashDataPtr = NULL;

    fapi2::buffer<uint64_t> l_cbs_cs;
    BootloaderConfigData_t l_bootloader_config_data;

    FAPI_DBG("Start");

    // read platform initialized attributes to determine struct content
    FAPI_TRY(p9_fbc_utils_get_chip_base_address_no_aliases(i_master_chip_target,
             EFF_FBC_GRP_CHIP_IDS,
             l_chip_base_address_nm0,
             l_chip_base_address_nm1,
             l_chip_base_address_m,
             l_chip_base_address_mmio),
             "Error from p9_fbc_utils_get_chip_base_address (chip)");

    l_bootloader_config_data.version = ADDR_STASH_SUPPORT_ADDED;

    // XSCOM BAR offset
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_XSCOM_BAR_BASE_ADDR_OFFSET,
                           FAPI_SYSTEM,
                           l_bootloader_config_data.xscomBAR),
             "Error from FAPI_ATTR_GET (ATTR_PROC_XSCOM_BAR_BASE_ADDR_OFFSET)");
    l_bootloader_config_data.xscomBAR += l_chip_base_address_mmio;

    // LPC BAR offset
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_LPC_BAR_BASE_ADDR_OFFSET,
                           FAPI_SYSTEM,
                           l_bootloader_config_data.lpcBAR),
             "Error from FAPI_ATTR_GET (ATTR_PROC_LPC_BAR_BASE_ADDR_OFFSET)");
    l_bootloader_config_data.lpcBAR += l_chip_base_address_mmio;

    // SBE boot side
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SBE_BOOT_SIDE,
                           FAPI_SYSTEM,
                           l_bootloader_config_data.sbeBootSide),
             "Error from FAPI_ATTR_GET (ATTR_SBE_BOOT_SIDE)");

    // PNOR boot side
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PNOR_BOOT_SIDE,
                           FAPI_SYSTEM,
                           l_bootloader_config_data.pnorBootSide),
             "Error from FAPI_ATTR_GET (ATTR_PNOR_BOOT_SIDE)");

    // PNOR size
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PNOR_SIZE,
                           FAPI_SYSTEM,
                           l_bootloader_config_data.pnorSizeMB),
             "Error from FAPI_ATTR_GET (ATTR_PNOR_SIZE)");

    // pass size of load including exception vectors and bootloader
    l_bootloader_config_data.blLoadSize = i_load_size;

    // Set Secure Settings Byte
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SECURE_SETTINGS,
                           FAPI_SYSTEM,
                           l_bootloader_config_data.secureSettings.data8),
             "Error from FAPI_ATTR_GET (ATTR_SECURE_SETTINGS)");

    // re-read Secure Access Bit in case it's changed
    FAPI_TRY(fapi2::getScom(i_master_chip_target,
                            PERV_CBS_CS_SCOM,
                            l_cbs_cs),
             "Error from getScom (PERV_CBS_CS_SCOM)");
    l_bootloader_config_data.secureSettings.secureAccessBit =
        (l_cbs_cs.getBit<PERV_CBS_CS_SECURE_ACCESS_BIT>()) ? (1) : (0);

    // initialize cacheline storage
    PACK_4B(io_data, l_index, EXCEPTION_VECTOR_BRANCH);
    PACK_4B(io_data, l_index, l_bootloader_config_data.version);
    PACK_1B(io_data, l_index, l_bootloader_config_data.sbeBootSide);
    PACK_1B(io_data, l_index, l_bootloader_config_data.pnorBootSide);
    PACK_2B(io_data, l_index, l_bootloader_config_data.pnorSizeMB);
    PACK_8B(io_data, l_index, l_bootloader_config_data.blLoadSize);
    PACK_1B(io_data, l_index, l_bootloader_config_data.secureSettings.data8);
    PACK_1B(io_data, l_index, 0x0);
    PACK_2B(io_data, l_index, 0x0);
    PACK_4B(io_data, l_index, 0x0);
    PACK_8B(io_data, l_index, l_bootloader_config_data.xscomBAR);
    PACK_8B(io_data, l_index, l_bootloader_config_data.lpcBAR);

    // Fetch the address for the stash storage
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SBE_ADDR_KEY_STASH_ADDR,
                           FAPI_SYSTEM,
                           l_stashAddrAttr),
             "fapiGetAttribute of ATTR_SBE_ADDR_KEY_STASH_ADDR failed!");

    if(l_stashAddrAttr) // If not 0, use this as addr to point to the data
    {
        l_stashDataPtr = reinterpret_cast<uint8_t*>(l_stashAddrAttr);

        for(uint8_t l_idx = 0; l_idx < sizeof(keyAddrPair_t); l_idx++)
        {
            // Total of 72Bytes will be stashed, First 8Bytes are the keys
            // Next 8 x 8Bytes are the addresses
            PACK_1B(io_data, l_index, l_stashDataPtr[l_idx]);
        }
    }

fapi_try_exit:
    FAPI_DBG("End");
    return fapi2::current_err;
}


///
/// @brief Determine data to load for cacheline containing exception
///        vector content
///
/// @param[inout] io_data Pointer to cacheline buffer to fill
/// @return fapi::ReturnCode. FAPI2_RC_SUCCESS if success, else error code.
///
fapi2::ReturnCode
get_exception_vector_data(
    uint8_t* io_data)
{
    uint8_t l_index = 0;
    uint32_t l_exception_instruction;

    FAPI_DBG("Start");

    // read attribute to determine if we should populate the exception vectors
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SBE_HBBL_EXCEPTION_INSTRUCT,
                           fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(),
                           l_exception_instruction),
             "Error from FAPI_ATTR_GET (ATTR_SBE_HBBL_EXCEPTION_INSTRUCT)");

    while (l_index < FABRIC_CACHELINE_SIZE)
    {
        PACK_4B(io_data, l_index, l_exception_instruction);
    }

fapi_try_exit:
    FAPI_DBG("End");
    return fapi2::current_err;
}


// HWP entry point, description in header
fapi2::ReturnCode p9_sbe_load_bootloader(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_master_chip_target,
    const fapi2::Target<fapi2::TARGET_TYPE_EX>& i_master_ex_target,
    const uint64_t i_payload_size,
    uint8_t* i_payload_data)
{
    bool l_first_access = true;
    uint32_t l_num_cachelines_to_roll;
    uint32_t l_cacheline_num = 0;
    uint8_t l_is_mpipl = 0x0;
    p9_PBA_oper_flag l_pba_flags;
    uint8_t l_data[FABRIC_CACHELINE_SIZE];
    uint64_t l_target_address;
    uint64_t l_end_address;
    uint64_t l_load_base_address;
    uint64_t l_load_size;
    bool l_load_exception_vector;

    FAPI_DBG("Start");

    // set PBA flags which control load operation
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_IS_MPIPL,
                           fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>(),
                           l_is_mpipl),
             "Error from FAPI_ATTR_GET (ATTR_IS_MPIPL)!");

    // set the PBA operation type (ttype) based on the IPL type:
    // - MPIPL: install the payload into memory via DMA ttype
    // - non-MPIPL: install the payload directly into the target L3 cache, via
    //              LCO (lateral castout)
    if (l_is_mpipl)
    {
        l_pba_flags.setOperationType(p9_PBA_oper_flag::DMA);
    }
    else
    {
        l_pba_flags.setOperationType(p9_PBA_oper_flag::LCO);
    }

    l_pba_flags.setFastMode(true);

    // calculate base address/size for image load
    // also, determine if FW specific exception vector will be installed
    FAPI_TRY(calc_image_footprint(i_master_chip_target,
                                  i_master_ex_target,
                                  i_payload_size,
                                  l_load_base_address,
                                  l_load_size,
                                  l_load_exception_vector),
             "Error from calc_image_footprint");

    l_target_address = l_load_base_address;
    l_end_address = l_load_base_address + l_load_size;

    while (l_target_address < l_end_address)
    {
        // invoke PBA setup HWP to prepare current stream of contiguous
        // cachelines
        FAPI_TRY(p9_pba_setup(i_master_chip_target,
                              i_master_ex_target,
                              l_target_address,
                              PBA_HWP_WRITE_OP,
                              l_pba_flags.setFlag(),
                              l_num_cachelines_to_roll),
                 "Error from p9_pba_setup");

        l_first_access = true;

        // call PBA access HWP per cacheline to move payload data
        while (l_num_cachelines_to_roll && (l_target_address < l_end_address))
        {
            // set content to move into next cacheline based on current
            // position in load sequence
            if (l_load_exception_vector && (l_cacheline_num == 0))
            {
                // write bootloader configuration data in first cacheline
                FAPI_TRY(get_exception_vector_data(l_data),
                         "Error from get_exception_vector_data");
                FAPI_TRY(get_bootloader_config_data(i_master_chip_target,
                                                    l_load_size,
                                                    l_data),
                         "Error from get_bootloader_config_data");
            }
            // write payload image content past end of optional exception vector
            else if (!l_load_exception_vector ||
                     (l_cacheline_num >= EXCEPTION_VECTOR_NUM_CACHELINES))
            {
                uint64_t l_payload_cacheline_index = l_cacheline_num;

                if (l_load_exception_vector)
                {
                    l_payload_cacheline_index -= EXCEPTION_VECTOR_NUM_CACHELINES;
                }

                l_payload_cacheline_index *= FABRIC_CACHELINE_SIZE;

                for (uint8_t ii = 0; ii < FABRIC_CACHELINE_SIZE; ii++)
                {
                    l_data[ii] = i_payload_data[l_payload_cacheline_index + ii];
                }
            }

            FAPI_TRY(p9_pba_access(i_master_chip_target,
                                   l_target_address,
                                   PBA_HWP_WRITE_OP,
                                   l_pba_flags.setFlag(),
                                   l_first_access,
                                   (l_num_cachelines_to_roll == 1) ||
                                   ((l_target_address + FABRIC_CACHELINE_SIZE) >=
                                    l_end_address),
                                   l_data),
                     "Error from p9_pba_access");

            // set data to fill pattern for remainder of exception vector
            if (l_load_exception_vector && (l_cacheline_num == 0))
            {
                FAPI_TRY(get_exception_vector_data(l_data),
                         "Error from get_exception_vector_data");
            }

            // reset flag
            l_first_access = false;
            // decrement count of cachelines remaining in current stream
            l_num_cachelines_to_roll--;
            // stride address/payload data pointer offset to next cacheline
            l_target_address += FABRIC_CACHELINE_SIZE;
            l_cacheline_num++;
        }
    }

    // RAM SPRs prior to instruction start
    FAPI_TRY(ram_sprs(i_master_chip_target,
                      i_master_ex_target,
                      l_load_base_address),
             "Error from ram_sprs");

fapi_try_exit:
    FAPI_DBG("End");
    return fapi2::current_err;
}
