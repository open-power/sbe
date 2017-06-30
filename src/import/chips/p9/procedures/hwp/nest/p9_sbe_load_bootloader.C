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
#include <p9_pba_coherent_utils.H>
#include <p9_quad_scom_addresses.H>
#include <p9_quad_scom_addresses_fld.H>
#include <p9_ram_core.H>
#include <p9_perv_scom_addresses.H>

//-----------------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------------

// PBA setup/access HWP call constants
const bool PBA_HWP_WRITE_OP = false;
const int EXCEPTION_VECTOR_NUM_CACHELINES = 96;
const uint8_t PERV_TO_CORE_POS_OFFSET = 0x20;

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
    const uint32_t l_branch_to_12 = 0x48003000ull;
    const uint32_t C_0_THREAD_INFO_RAM_THREAD_ACTIVE_T0 = 18;
    uint64_t l_bootloader_offset;
    uint64_t l_hostboot_hrmor_offset;
    uint64_t l_drawer_base_address_nm0, l_drawer_base_address_nm1;
    uint64_t l_drawer_base_address_m;
    uint64_t l_drawer_base_address_mmio;
    uint64_t l_chip_base_address_nm0, l_chip_base_address_nm1;
    uint64_t l_chip_base_address_m;
    uint64_t l_chip_base_address_mmio;
    uint64_t l_target_address;
    uint32_t l_exception_instruction;
    bool l_firstAccess = true;
    uint32_t l_num_cachelines_to_roll;
    uint8_t l_data_to_pass_to_pba_array[FABRIC_CACHELINE_SIZE];
    uint32_t l_exception_vector_size = 0;
    uint8_t l_master_core = 0;
    int l_cacheline_num = 0;
    uint8_t l_is_mpipl = 0x0;
    p9_PBA_oper_flag l_myPbaFlag;
    fapi2::buffer<uint64_t> l_dataBuf;
    fapi2::Target<fapi2::TARGET_TYPE_CORE> l_coreTarget;
    bool l_coreFoundMatch = false;

    FAPI_DBG("Start");

    //Find the master core for writing the HRMOR later
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_MASTER_CORE, i_master_chip_target, l_master_core), "Error getting ATTR_MASTER_CORE");

    for ( auto& l_current_core : i_master_ex_target.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        uint8_t l_attr_chip_unit_pos = 0;
        fapi2::Target<fapi2::TARGET_TYPE_PERV> l_perv = l_current_core.getParent<fapi2::TARGET_TYPE_PERV>();
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_perv, l_attr_chip_unit_pos),
                 "Error getting ATTR_CHIP_UNIT_POS");
        l_attr_chip_unit_pos = l_attr_chip_unit_pos - PERV_TO_CORE_POS_OFFSET;
        FAPI_DBG("l_attr_chip_unit_pos = %d, l_attr_chip_unit_pos = %d, l_master_core = %d", l_attr_chip_unit_pos,
                 l_attr_chip_unit_pos, l_master_core);

        if (l_attr_chip_unit_pos == l_master_core)
        {
            l_coreTarget = l_current_core;
            l_coreFoundMatch = true;
            break;
        }
    }

    FAPI_ASSERT(l_coreFoundMatch, fapi2::P9_MASTER_CORE_NOT_FOUND().set_CHIP_TARGET(i_master_chip_target).set_EX_TARGET(
                    i_master_ex_target).set_MASTER_CORE(l_master_core) , "Error in finding the master core");

    // read platform initialized attributes needed to determine target base address
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SBE_BOOTLOADER_OFFSET, FAPI_SYSTEM, l_bootloader_offset),
             "Error from FAPI_ATTR_GET (ATTR_SBE_BOOTLOADER_OFFSET)");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_HOSTBOOT_HRMOR_OFFSET, FAPI_SYSTEM, l_hostboot_hrmor_offset),
             "Error from FAPI_ATTR_GET (ATTR_HOSTBOOT_HRMOR_OFFSET)");

    // target base address = (drawer non-mirrored base address) +
    //                       (hostboot HRMOR offset) +
    //                       (bootloader offset)
    FAPI_TRY(p9_fbc_utils_get_chip_base_address(i_master_chip_target,
             EFF_FBC_GRP_ID_ONLY,
             l_drawer_base_address_nm0,
             l_drawer_base_address_nm1,
             l_drawer_base_address_m,
             l_drawer_base_address_mmio),
             "Error from p9_fbc_utils_get_chip_base_address (drawer)");

    FAPI_TRY(p9_fbc_utils_get_chip_base_address(i_master_chip_target,
             EFF_FBC_GRP_CHIP_IDS,
             l_chip_base_address_nm0,
             l_chip_base_address_nm1,
             l_chip_base_address_m,
             l_chip_base_address_mmio),
             "Error from p9_fbc_utils_get_chip_base_address (chip)");

    // add hostboot HRMOR offset and bootloader offset contributions
    l_drawer_base_address_nm0 += l_hostboot_hrmor_offset;
    l_drawer_base_address_nm0 += l_bootloader_offset;

    // check that base address is cacheline aligned
    FAPI_ASSERT(!(l_drawer_base_address_nm0 % FABRIC_CACHELINE_SIZE),
                fapi2::P9_SBE_LOAD_BOOTLOADER_INVALID_TARGET_ADDRESS().
                set_CHIP_TARGET(i_master_chip_target).
                set_EX_TARGET(i_master_ex_target).
                set_TARGET_BASE_ADDRESS(l_drawer_base_address_nm0).
                set_HRMOR_OFFSET(l_hostboot_hrmor_offset).
                set_BOOTLOADER_OFFSET(l_bootloader_offset),
                "Target base address is not cacheline aligned!");

    //Check to see if we need to populate the exception vectors
    //Check the SBE_HBBL_EXCEPTION_INSTRUCT attribute
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SBE_HBBL_EXCEPTION_INSTRUCT, FAPI_SYSTEM, l_exception_instruction),
             "fapiGetAttribute of ATTR_SBE_HBBL_EXCEPTION_INSTRUCT failed!");

    l_target_address = l_drawer_base_address_nm0;

    BootloaderConfigData_t l_bootloader_config_data;

    l_bootloader_config_data.version = MMIO_BARS_ADDED;

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

    // adjust exception vector size
    if (l_exception_instruction != 0x0)
    {
        l_exception_vector_size = EXCEPTION_VECTOR_NUM_CACHELINES * FABRIC_CACHELINE_SIZE;
    }

    // Pass size of load including exception vectors and Bootloader
    l_bootloader_config_data.blLoadSize = l_exception_vector_size + i_payload_size;

    // Set Secure Settings Byte
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SECURE_SETTINGS, FAPI_SYSTEM, l_bootloader_config_data.secureSettings.data8));

    // -- re-read Secure Access Bit in case it's changed
    FAPI_TRY(fapi2::getScom(i_master_chip_target, PERV_CBS_CS_SCOM, l_dataBuf));

    l_bootloader_config_data.secureSettings.secureAccessBit = l_dataBuf.getBit<4>() ? 1 : 0;
    l_dataBuf.flush<0>();

    // fill in MMIO BARs
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_XSCOM_BAR_BASE_ADDR_OFFSET, FAPI_SYSTEM, l_bootloader_config_data.xscomBAR),
             "Error from FAPI_ATTR_GET (ATTR_PROC_XSCOM_BAR_BASE_ADDR_OFFSET)");
    l_bootloader_config_data.xscomBAR += l_chip_base_address_mmio;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_LPC_BAR_BASE_ADDR_OFFSET, FAPI_SYSTEM, l_bootloader_config_data.lpcBAR),
             "Error from FAPI_ATTR_GET (ATTR_PROC_LPC_BAR_BASE_ADDR_OFFSET)");
    l_bootloader_config_data.lpcBAR += l_chip_base_address_mmio;

    // move data using PBA setup/access HWPs
    l_myPbaFlag.setFastMode(true);  // FASTMODE

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_IS_MPIPL, FAPI_SYSTEM, l_is_mpipl), "fapiGetAttribute of ATTR_IS_MPIPL failed!");

    if (!l_is_mpipl)
    {
        l_myPbaFlag.setOperationType(p9_PBA_oper_flag::LCO); // LCO operation
    }
    else
    {
        l_myPbaFlag.setOperationType(p9_PBA_oper_flag::DMA); // DMA operation
    }

    while (l_target_address < (l_drawer_base_address_nm0 + i_payload_size + l_exception_vector_size))
    {
        // invoke PBA setup HWP to prep stream
        FAPI_TRY(p9_pba_setup( i_master_chip_target,
                               i_master_ex_target,
                               l_target_address,
                               PBA_HWP_WRITE_OP,
                               l_myPbaFlag.setFlag(),
                               l_num_cachelines_to_roll), "Error from p9_pba_setup");

        l_firstAccess = true;

        // call PBA access HWP per cacheline to move payload data
        while (l_num_cachelines_to_roll &&
               (l_target_address < (l_drawer_base_address_nm0 + i_payload_size + l_exception_vector_size)))
        {
            if ((l_cacheline_num == 0) && (l_exception_instruction != 0))
            {
                //This is for the first cacheline of data that has the branch, pnor_size, and pnor_boot_side in it
                //The rest of the exception vector is what was in SBE_HBBL_EXCEPTION_INSTRUCT replicated multiple times (until the end of 12KB of exception vector data)
                for (uint32_t i = 0; i < FABRIC_CACHELINE_SIZE; i++)
                {
                    //At address X (0-3) put whatever is in l_branch_to_12
                    if (i < 4)
                    {
                        l_data_to_pass_to_pba_array[i] = (l_branch_to_12 >> (24 - 8 * i )) & 0xFF;
                    }
                    //At address X + 0x4 (4-7) put the HBBL_STRUCT_VERSION
                    else if (i < 8)
                    {
                        l_data_to_pass_to_pba_array[i] = (l_bootloader_config_data.version >> (24 - 8 * ((i - 4) % 4))) & 0xFF;
                    }
                    //At address X + 0x8 (8) put the SBE_BOOT_SIDE
                    else if (i == 8)
                    {
                        l_data_to_pass_to_pba_array[i] = l_bootloader_config_data.sbeBootSide;
                    }
                    //At address X + 0x9 (9) put the PNOR_BOOT_SIDE
                    else if (i  == 9)
                    {
                        l_data_to_pass_to_pba_array[i] = l_bootloader_config_data.pnorBootSide;
                    }
                    //At address X + 0xA (10-11) pu the PNOR_SIZE
                    else if (i == 10)
                    {
                        l_data_to_pass_to_pba_array[i] = l_bootloader_config_data.pnorSizeMB >> 8 & 0xFF;
                    }
                    else if (i == 11)
                    {
                        l_data_to_pass_to_pba_array[i] = l_bootloader_config_data.pnorSizeMB & 0xFF;
                    }
                    //At address X + 0xC (12-19) put the total load size
                    else if (i < 20)
                    {
                        l_data_to_pass_to_pba_array[i] = (l_bootloader_config_data.blLoadSize >> (56 - 8 * ((i - 12) % 8))) & 0xFF;
                    }
                    //At address X + 0x14 (20) put the secure access byte
                    else if (i == 20)
                    {
                        l_data_to_pass_to_pba_array[i] = l_bootloader_config_data.secureSettings.data8;
                    }
                    //At address X + 0x15 (21-27) put a zero
                    else if (i < 28)
                    {
                        l_data_to_pass_to_pba_array[i] = 0;
                    }
                    //At address X + 0x1B (28-35) put the XSCOM BAR
                    else if (i < 36)
                    {
                        l_data_to_pass_to_pba_array[i] = (l_bootloader_config_data.xscomBAR >> (56 - 8 * ((i - 28) % 8))) & 0xFF;
                    }
                    //At address X + 0x1B (36-43) put the LPC BAR
                    else if (i < 44)
                    {
                        l_data_to_pass_to_pba_array[i] = (l_bootloader_config_data.lpcBAR >> (56 - 8 * ((i - 36) % 8))) & 0xFF;
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
                                   l_myPbaFlag.setFlag(),
                                   l_firstAccess,
                                   (l_num_cachelines_to_roll == 1) ||
                                   ((l_target_address + FABRIC_CACHELINE_SIZE) >=
                                    (l_drawer_base_address_nm0 + i_payload_size + l_exception_vector_size)),
                                   l_data_to_pass_to_pba_array), "Error from p9_pba_access");
            l_firstAccess = false;
            // decrement count of cachelines remaining in current stream
            l_num_cachelines_to_roll--;

            // stride address/payload data pointer offset to next cacheline
            l_target_address += FABRIC_CACHELINE_SIZE;
            l_cacheline_num++;
        }
    }

    {

        fapi2::ATTR_FUSED_CORE_MODE_Type l_attr_fused_mode;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FUSED_CORE_MODE,
                               FAPI_SYSTEM,
                               l_attr_fused_mode));
        l_dataBuf.flush<0>();

        //Override PM_EXIT on master core bit 4 is for core 0 bit 5 is for core 1
        if ( (l_master_core % 2 == 0) ||
             (l_attr_fused_mode == fapi2::ENUM_ATTR_FUSED_CORE_MODE_CORE_FUSED))
        {
            l_dataBuf.setBit<EQ_CME_SCOM_SICR_PM_EXIT_C0>();
        }

        if ( (l_master_core % 2 != 0) ||
             (l_attr_fused_mode == fapi2::ENUM_ATTR_FUSED_CORE_MODE_CORE_FUSED))
        {
            l_dataBuf.setBit<EQ_CME_SCOM_SICR_PM_EXIT_C1>();
        }

        FAPI_TRY(fapi2::putScom(i_master_ex_target, EX_0_CME_SCOM_SICR_SCOM2,
                                l_dataBuf) )

        for (auto& coreTgt : i_master_ex_target.getChildren<fapi2::TARGET_TYPE_CORE>())
        {

            // In non-fused mode, set HRMOR for master core only
            if ((l_attr_fused_mode != fapi2::ENUM_ATTR_FUSED_CORE_MODE_CORE_FUSED) &&
                (( coreTgt != l_coreTarget)))
            {
                continue;
            }

            //instantiate the basic RamCore class
            RamCore ram(coreTgt, 0);
            //Set the HRMOR

            //Set ram_thread_active for t0
            l_dataBuf.flush<0>().setBit<C_0_THREAD_INFO_RAM_THREAD_ACTIVE_T0>();
            FAPI_TRY(fapi2::putScom(coreTgt, C_0_THREAD_INFO, l_dataBuf),
                     "Error setting thread active for t0");

            if( coreTgt == l_coreTarget )
            {
                l_dataBuf.flush<0>().insertFromRight<0, 64>(l_drawer_base_address_nm0);
            }
            else
            {
                l_dataBuf.flush<0>().insertFromRight<0, 64>(
                    l_drawer_base_address_nm0 - l_bootloader_offset);
            }

            //call RamCore put_reg method
            FAPI_TRY(ram.put_reg(REG_SPR, 313, &l_dataBuf), "Error ramming HRMOR");
        }
    }

fapi_try_exit:
    FAPI_DBG("End");
    return fapi2::current_err;
}

