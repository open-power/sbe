/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/nest/p9_sbe_check_quiesce.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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
//
/// @file p9_sbe_check_quiesce.C
/// @brief Check quiesce state for all units on the powerbus
///
// *HWP HWP Owner Christina Graves clgraves@us.ibm.com
// *HWP FW Owner: Thi Tran thi@us.ibm.com
// *HWP Team: Nest
// *HWP Level: 2
// *HWP Consumed by: SBE
//
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <p9_sbe_check_quiesce.H>
#include <p9_quad_scom_addresses.H>
#include <p9_misc_scom_addresses_fld.H>
#include <p9_misc_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p9_perv_scom_addresses.H>
#include <p9_inject_local_xstop.H>
#include <p9_thread_control.H>

extern "C" {

    //This is how many times each unit will try to do the quiesce if it has a wait for some condition to be set
    const uint32_t C_NUM_TRIES_QUIESCE_STATE = 5;
    //TODO RTC 160710
    const bool C_NX_DD2_READY = false;

//--------------------------------------------------------------------------
//  HWP entry point
//--------------------------------------------------------------------------
    fapi2::ReturnCode p9_sbe_check_quiesce(const
                                           fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        // SBE will check quiesce state for all units on the powerbus on its chip
        FAPI_TRY(p9_ec_eq_check_quiesce(i_target), "Error from p9_ec_eq_check_quiesce");
        FAPI_TRY(p9_capp_check_quiesce(i_target), "Error from p9_capp_check_quiesce");
        FAPI_TRY(p9_phb_check_quiesce(i_target), "Error from p9_phb_check_quiesce");
        FAPI_TRY(p9_npu_check_quiesce(i_target), "Error from p9_npu_check_quiesce");
        FAPI_TRY(p9_vas_check_quiesce(i_target), "Error from p9_vas_check_quiesce");
        FAPI_TRY(p9_nx_check_quiesce(i_target), "Error from p9_nx_check_quiesce");
        //TODO it looks like the hca will not be in the plan RTC 160709
        //FAPI_TRY(p9_hca_check_quiesce(i_target), "Error from p9_hca_check_quiesce");
        FAPI_TRY(p9_psihb_check_quiesce(i_target), "Error from p9_psihb_check_quiesce");
        FAPI_TRY(p9_intp_check_quiesce(i_target), "Error from p9_intp_check_quiesce");

    fapi_try_exit:
        fapi2::ReturnCode saveError = fapi2::current_err;
        fapi2::buffer<uint64_t> l_data(0);

        //If the quiesce fails then checkstop the system
        if (fapi2::current_err)
        {
            //Checkstop the system
            l_data.setBit<34>();
            fapi2::ReturnCode rc = fapi2::putScom(i_target, PERV_N3_LOCAL_FIR_OR, l_data);

            if (rc)
            {
                FAPI_INF("ERROR: There was an error doing the checkstop, it may not have gone through");
            }
        }

        FAPI_DBG("Exiting...");
        return saveError;
    }

//---------------------------------------------------------------------------
//  Helper Functions
//---------------------------------------------------------------------------

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_ec_eq_check_quiesce(const
            fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        fapi2::ReturnCode l_rc;
        const uint8_t C_ALL_THREADS = 0b1111;

        const uint64_t C_BLOCK_ALL_WKUP_EVENTS = 7;
        const uint64_t C_BLOCK_REG_WKUP_EVENTS = 6;

        fapi2::buffer<uint64_t> l_ras_status(0);
        fapi2::buffer<uint64_t> l_gpmmr_data(0);
        uint64_t l_state = 0x0ull;
        bool l_ras_is_in_stop = false;
        bool l_thread_warncheck = true;

        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        //Get all the cores on this proc chip
        auto l_core_targets = i_target.getChildren<fapi2::TARGET_TYPE_CORE>(fapi2::TARGET_STATE_FUNCTIONAL);

        //Block wake up on all cores
        l_gpmmr_data.setBit<C_BLOCK_ALL_WKUP_EVENTS>().setBit<C_BLOCK_REG_WKUP_EVENTS>();

        for (auto current_core : l_core_targets)
        {
            //Stop the threads on each core
            FAPI_EXEC_HWP(l_rc, p9_thread_control, current_core, C_ALL_THREADS, PTC_CMD_STOP, l_thread_warncheck, l_ras_status,
                          l_state);

            if (l_rc)
            {
                FAPI_ERR("Error from p9_thread_control in p9_sbe_check_quiesce");
                fapi2::current_err = l_rc;
                goto fapi_try_exit;
            }

            //Block wake up on each core
            FAPI_TRY(fapi2::putScom(current_core, C_PPM_GPMMR_SCOM1, l_gpmmr_data), "Error writing to GPMMR");
        }

        for (auto current_core : l_core_targets)
        {
            // check for maint mode or STOP state in core RAS Status register (try 5 times), if not reached fail
            // All ECs should already be there per initial instruction stop
            for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
            {
                FAPI_EXEC_HWP(l_rc, p9_thread_control, current_core, C_ALL_THREADS, PTC_CMD_QUERY, l_thread_warncheck, l_ras_status,
                              l_state);

                if (l_rc)
                {
                    FAPI_ERR("Error from p9_thread_control in p9_sbe_check_quiesce");
                    fapi2::current_err = l_rc;
                    goto fapi_try_exit;
                }

                l_ras_is_in_stop = (l_state && THREAD_STATE_MAINT) || (l_state && THREAD_STATE_QUIESCED);

                if (l_ras_is_in_stop)
                {
                    break;
                }
            }

            FAPI_ASSERT(l_ras_is_in_stop, fapi2::P9_RAS_STATUS_ERR().set_TARGET(current_core).set_RASSTATUS(l_ras_status),
                        "Error with the RAS Status not being set as expected");
        }

        for (auto current_core : l_core_targets)
        {
            // p9_inject_local_xstop.C
            // inject a local core checkstop on each core to prevent them from restarting
            FAPI_TRY(p9_inject_local_xstop(current_core), "Error in p9_inject_local_xstop");
        }

    fapi_try_exit:
        FAPI_DBG("Exiting...");
        return fapi2::current_err;
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_capp_check_quiesce(const
                                            fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        fapi2::buffer<uint64_t> l_capp_err_status_control_data(0);
        bool l_quiesce_achieved = false;

        // read the value of CAPP Error status and control register so we don't write over something
        FAPI_TRY(fapi2::getScom(i_target, CAPP_CAPP_ERR_STATUS_CONTROL, l_capp_err_status_control_data),
                 "Error reading CAPP_ERR_STATUS_CONTROL register");

        // Write the Force Quiesce bit
        l_capp_err_status_control_data.setBit<CAPP_CAPP_ERR_STATUS_CONTROL_FORCE_QUIESCE>();
        FAPI_TRY(fapi2::putScom(i_target, CAPP_CAPP_ERR_STATUS_CONTROL, l_capp_err_status_control_data),
                 "Error writing CAPP_ERR_STATUS_CONTROL register");

        // Poll the Quiesce done bit
        for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
        {
            FAPI_TRY(fapi2::getScom(i_target, CAPP_CAPP_ERR_STATUS_CONTROL, l_capp_err_status_control_data),
                     "Error reading from VAS_MISC_NORTH_CTL register");

            if (!l_capp_err_status_control_data.getBit<CAPP_CAPP_ERR_STATUS_CONTROL_QUIESCE_DONE>())
            {
                l_quiesce_achieved = true;
                break;
            }
        }

        FAPI_ASSERT(l_quiesce_achieved, fapi2::P9_CAPP_QUIESCE_TIMEOUT().set_TARGET(i_target).set_DATA(
                        l_capp_err_status_control_data),
                    "CAPP quiesce timed out");

    fapi_try_exit:
        FAPI_DBG("Exiting...");
        return fapi2::current_err;
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_phb_check_quiesce(const
                                           fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // mark HWP entry
        FAPI_DBG("Entering ...\n");
        fapi2::buffer<uint64_t> phb_hv_addr_reg_data(0x0888);
        //We want to set bit 0 (the Quiesce DMA bit)
        //This is the data that will be passed in to set the PHB Quiesce DMA register
        fapi2::buffer<uint64_t> phb_hv_data_reg_data(0x8000000000000000);

        //The address of the PHB Quiesce DMA Register is 0x0888 (found in PHB spec)
        uint8_t l_pci_id = 0;
        uint64_t phb_absolute_address_array[3];
        uint8_t num_phbs = 0;
        uint64_t phb_addr_reg = 0;
        uint64_t phb_data_reg = 0;

        auto l_pci_chiplets_vec = i_target.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_ALL_PCI,
                                  fapi2::TARGET_STATE_FUNCTIONAL);

        for (auto l_pci_chiplet : l_pci_chiplets_vec)
        {
            //Get the PCI ID
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_pci_chiplet, l_pci_id), "Error getting the CHIP_UNIT_POS");

            //There are a different number and different PHBs associated with each PCI ID
            //PHB 0 is attached to PCI0
            //PHB 1 and PHB 2 are attached to PCI1
            //PHB 3 and PHB 4 and PHB 5 are attached to PCI2
            if (l_pci_id == 0xd)
            {
                num_phbs = 1;
                phb_absolute_address_array[0] = PHB_0_PHB4_SCOM_HVIAR;
            }
            else if (l_pci_id == 0xe)
            {
                num_phbs = 2;
                phb_absolute_address_array[0] = PHB_1_PHB4_SCOM_HVIAR;
                phb_absolute_address_array[1] = PHB_2_PHB4_SCOM_HVIAR;
            }
            else if (l_pci_id == 0xf)
            {
                num_phbs = 3;
                phb_absolute_address_array[0] = PHB_3_PHB4_SCOM_HVIAR;
                phb_absolute_address_array[1] = PHB_4_PHB4_SCOM_HVIAR;
                phb_absolute_address_array[2] = PHB_5_PHB4_SCOM_HVIAR;
            }

            for (uint8_t i = 0; i < num_phbs; i++)
            {
                phb_addr_reg = phb_absolute_address_array[i];
                phb_data_reg = phb_absolute_address_array[i] + 1;
                //Clear contents of PHB HV Indirect Address Register
                phb_hv_addr_reg_data.flush<0>();
                FAPI_TRY(fapi2::putScom(i_target, phb_addr_reg, phb_hv_addr_reg_data),
                         "Error clearing PHB HV Indirect Address Register");
                //Setup the PHB HV registers for the write
                phb_hv_addr_reg_data.insertFromRight<PHB_HV_IND_ADDR_START_BIT, PHB_HV_IND_ADDR_LEN>(0x888);
                phb_hv_addr_reg_data.setBit<PHB_HV_IND_ADDR_VALID_BIT>();
                FAPI_TRY(fapi2::putScom(i_target, phb_addr_reg, phb_hv_addr_reg_data),
                         "Error writing PHB HV Indirect Address Register");
                //Setup PHB HV Indirect for write access
                FAPI_TRY(fapi2::putScom(i_target, phb_data_reg, phb_hv_data_reg_data),
                         "Error writing PHB HV Indirect Data Register");
                //Clear contents of PHB HV Indirect Address Register
                phb_hv_addr_reg_data.flush<0>();
                FAPI_TRY(fapi2::putScom(i_target, phb_addr_reg, phb_hv_addr_reg_data),
                         "Error clearing PHB HV Indirect Address Register");
            }
        }

    fapi_try_exit:
        FAPI_DBG("Exiting...");
        return fapi2::current_err;
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_npu_check_quiesce(const
                                           fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        const uint32_t c_fence_status_reg_size = 6;
        const uint32_t c_GPU_Memory_BARs_size = 12;
        const uint32_t c_memory_bars_size = 36;

        fapi2::buffer<uint64_t> l_ntl_misc_config_1_reg_data(0);
        fapi2::buffer<uint64_t> l_npu_indirect_address_reg_data(0);
        fapi2::buffer<uint64_t> l_npu_indirect_data_reg_data[6];
        fapi2::buffer<uint64_t> l_npu_fence_state_reg_data(0);
        fapi2::buffer<uint64_t> l_memory_bar_data(0);
        uint64_t CQ_fence_status_regs[c_fence_status_reg_size] = {0x00090500, 0x000B0500, 0x00190500, 0x001B0500, 0x00290500, 0x002B0500};
        uint64_t l_GPU_Memory_BARs[c_GPU_Memory_BARs_size] = {PU_NPU0_SM0_GPU_BAR, PU_NPU0_SM1_GPU_BAR, PU_NPU0_SM2_GPU_BAR, PU_NPU0_SM3_GPU_BAR, PU_NPU1_SM0_GPU_BAR, PU_NPU1_SM1_GPU_BAR, PU_NPU1_SM2_GPU_BAR, PU_NPU1_SM3_GPU_BAR, PU_NPU2_SM0_GPU_BAR, PU_NPU2_SM1_GPU_BAR, PU_NPU2_SM2_GPU_BAR, PU_NPU2_SM3_GPU_BAR};
        uint64_t l_memory_bars[c_memory_bars_size] = {PU_NPU0_SM0_NDT0_BAR, PU_NPU0_SM1_NDT0_BAR, PU_NPU0_SM2_NDT0_BAR, PU_NPU0_SM3_NDT0_BAR, PU_NPU1_SM0_NDT0_BAR, PU_NPU1_SM1_NDT0_BAR, PU_NPU1_SM2_NDT0_BAR, PU_NPU1_SM3_NDT0_BAR, PU_NPU2_SM0_NDT0_BAR, PU_NPU2_SM1_NDT0_BAR, PU_NPU2_SM2_NDT0_BAR, PU_NPU2_SM3_NDT0_BAR, PU_NPU0_SM0_NDT1_BAR, PU_NPU0_SM1_NDT1_BAR, PU_NPU0_SM2_NDT1_BAR, PU_NPU0_SM3_NDT1_BAR, PU_NPU1_SM0_NDT1_BAR, PU_NPU1_SM1_NDT1_BAR, PU_NPU1_SM2_NDT1_BAR, PU_NPU1_SM3_NDT1_BAR, PU_NPU2_SM0_NDT1_BAR, PU_NPU2_SM1_NDT1_BAR, PU_NPU2_SM2_NDT1_BAR, PU_NPU2_SM3_NDT1_BAR, PU_NPU0_SM0_PHY_BAR, PU_NPU0_SM1_PHY_BAR, PU_NPU0_SM2_PHY_BAR, PU_NPU0_SM3_PHY_BAR, PU_NPU1_SM0_PHY_BAR, PU_NPU1_SM1_PHY_BAR, PU_NPU1_SM2_PHY_BAR, PU_NPU1_SM3_PHY_BAR, PU_NPU2_SM0_PHY_BAR, PU_NPU2_SM1_PHY_BAR, PU_NPU2_SM2_PHY_BAR, PU_NPU2_SM3_PHY_BAR };
        bool l_NTL_in_reset_state[c_fence_status_reg_size] = {false, false, false, false, false, false};
        bool l_all_NTL_in_reset_state = false;

        // 1) Place all six of the NTLs into Reset State
        // Set bits 8:9 in the NTL Misc Config 1 registers to place NTLs in Reset state
        l_ntl_misc_config_1_reg_data.insertFromRight<NV_CONFIG1_NTL_RESET, NV_CONFIG1_NTL_RESET_LEN>(0x3);
        FAPI_TRY(fapi2::putScom(i_target, NV_0_CONFIG1, l_ntl_misc_config_1_reg_data),
                 "Error writing to NV 0 Config1 register");
        FAPI_TRY(fapi2::putScom(i_target, NV_1_CONFIG1, l_ntl_misc_config_1_reg_data),
                 "Error writing to NV 1 Config1 register");
        FAPI_TRY(fapi2::putScom(i_target, NV_2_CONFIG1, l_ntl_misc_config_1_reg_data),
                 "Error writing to NV 2 Config1 register");
        FAPI_TRY(fapi2::putScom(i_target, NV_3_CONFIG1, l_ntl_misc_config_1_reg_data),
                 "Error writing to NV 3 Config1 register");
        FAPI_TRY(fapi2::putScom(i_target, PU_NPU2_NTL0_CONFIG1, l_ntl_misc_config_1_reg_data),
                 "Error writing to NPU2 NTL0 Config1 register");
        FAPI_TRY(fapi2::putScom(i_target, PU_NPU2_NTL1_CONFIG1, l_ntl_misc_config_1_reg_data),
                 "Error writing to NPU2 NTL1 Config1 register");

        //Poll the CQ Fence Status Registers util "Value" is detected to verify that NTLs are in Reset State
        for (uint32_t i = 0; i < c_fence_status_reg_size; i++)
        {
            for (uint32_t j = 0; j < C_NUM_TRIES_QUIESCE_STATE; j ++)
            {
                l_npu_indirect_address_reg_data.flush<0>().insertFromRight<PU_NPU_CTL_DA_ADDR_MISC, PU_NPU_CTL_DA_ADDR_MISC_LEN>
                (CQ_fence_status_regs[i]);
                FAPI_TRY(fapi2::putScom(i_target, PU_NPU_CTL_DA_ADDR, l_npu_indirect_address_reg_data),
                         "Error writing to NPU Indirect Address register");
                FAPI_TRY(fapi2::getScom(i_target, PU_NPU_CTL_DA_DATA, l_npu_indirect_data_reg_data[i]),
                         "Error reading from NPU Indirect Data register");

                if (l_npu_indirect_data_reg_data[i].getBit<0>() && l_npu_indirect_data_reg_data[i].getBit<1>())
                {
                    l_NTL_in_reset_state[i] = true;
                    break;
                }
            }
        }

        l_all_NTL_in_reset_state = l_NTL_in_reset_state[0] && l_NTL_in_reset_state[1] && l_NTL_in_reset_state[2]
                                   && l_NTL_in_reset_state[3] && l_NTL_in_reset_state[4] && l_NTL_in_reset_state[5];
        FAPI_ASSERT(l_all_NTL_in_reset_state,
                    fapi2::P9_NTL_NOT_IN_RESET().set_TARGET(i_target).set_NTL_DATA0(l_npu_indirect_data_reg_data[0]).set_NTL_DATA1(
                        l_npu_indirect_data_reg_data[1]).set_NTL_DATA2(l_npu_indirect_data_reg_data[2]).set_NTL_DATA3(
                        l_npu_indirect_data_reg_data[3]).set_NTL_DATA4(l_npu_indirect_data_reg_data[4]).set_NTL_DATA5(
                        l_npu_indirect_data_reg_data[5]), "One or more of the NTLS are not in the reset state");

        // 2) Place allNV-Link Bricks into Fence State
        // Set bits 0:5 in the NPU Fence State register to place all bricks into Fence State
        l_npu_fence_state_reg_data.setBit<PU_NPU_CTL_FENCE_STATE_BRK0>().setBit<PU_NPU_CTL_FENCE_STATE_BRK1>().setBit<PU_NPU_CTL_FENCE_STATE_BRK2>().setBit<PU_NPU_CTL_FENCE_STATE_BRK3>().setBit<PU_NPU_CTL_FENCE_STATE_BRK4>().setBit<PU_NPU_CTL_FENCE_STATE_BRK5>();
        FAPI_TRY(fapi2::putScom(i_target, PU_NPU_CTL_FENCE_STATE, l_npu_fence_state_reg_data),
                 "Error writing to the NPU Fence State Register");

        // 3) Disable all NPU BAR registers
        // Reset bits 0 and 32 in the GPU-Memory BARs to stop NPU from responding to accesses to GPU memory
        l_memory_bar_data.clearBit<PU_NPU0_SM0_GPU_BAR_CONFIG_GPU0_ENABLE>().clearBit<PU_NPU0_SM0_GPU_BAR_CONFIG_GPU1_ENABLE>();

        for (uint32_t i = 0; i < c_GPU_Memory_BARs_size; i++)
        {
            FAPI_TRY(fapi2::putScom(i_target, l_GPU_Memory_BARs[i], l_memory_bar_data),
                     "Error writing to NPU0 SM0 GPU BAR register");
        }

        // Reset bit 0 in the NTL0/NDL0 Memory BARs and NTL1/NDL1 Memory BARs to stop NPU from responding to accesses to NTL/NDL registers
        // Reset bit 0 in PHY0/PHY1/NPU MMIO BAR for stack 0 and stack 1 to stop NPU from responding to PHY register accesses
        // Reset bit 0 in PHY0/PHY1/NPU MMIO BARs in stack 2 to stop NPU from responding to NPU MMIO register accesses
        l_memory_bar_data.flush<0>().clearBit<PU_NPU0_SM0_NDT0_BAR_CONFIG_ENABLE>();

        for (uint32_t i = 0; i < c_memory_bars_size; i++)
        {
            FAPI_TRY(fapi2::putScom(i_target, l_memory_bars[i], l_memory_bar_data), "Error writing to NPU0 SM0 NDT0 BAR register");
        }

    fapi_try_exit:
        FAPI_DBG("Exiting...");
        return fapi2::current_err;
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_vas_check_quiesce(const
                                           fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        fapi2::buffer<uint64_t> l_vas_north_misc_ctl_data(0);
        fapi2::buffer<uint64_t> l_vas_south_misc_ctl_data(0);
        bool l_quiesce_achieved = false;

        // VAS needs to be quiesced before NX

        // Read the VAS Misc status and North control register so we don't write over anything
        FAPI_TRY(fapi2::getScom(i_target, PU_VAS_MISCCTL, l_vas_north_misc_ctl_data),
                 "Error reading from VAS_MISC_NORTH_CTL register");
        // Set the 'Quiesce Requested' bit in the VAS Miscellaneous Status and North Control Register to a 1. This will prevent VAS from accepting new paste or write monitor operations
        l_vas_north_misc_ctl_data.setBit<PU_VAS_MISCCTL_MISC_CTL_QUIESCE_REQUEST>();
        FAPI_TRY(fapi2::putScom(i_target, PU_VAS_MISCCTL, l_vas_north_misc_ctl_data),
                 "Error writing to VAS_MISC_NORTH_CTL register");

        //Check that VAS has quiesced. This is accomplished by reading two status registers. The "RG is Idle' bit in the VAS Miscellaneous Status and North Control Register as well as the 'EG is Idle', 'CQ is Idle' and 'WC is Idle' bit sin the VAS Miscellaneous Status and South Control Register must all be set to one to indicate that VAS has gone idle.
        for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
        {
            // Read VAS Misc status and North control register to ensure 'RG is idle' and 'EG is idle' bits are both set -
            FAPI_TRY(fapi2::getScom(i_target, PU_VAS_MISCCTL, l_vas_north_misc_ctl_data),
                     "Error reading from VAS_MISC_NORTH_CTL register");
            // Read VAS Misc status and South control register to ensure 'WC is idle' and 'CQ is idle' bits are both set -
            FAPI_TRY(fapi2::getScom(i_target, PU_VAS_SOUTHCTL, l_vas_south_misc_ctl_data),
                     "Error reading from VAS_MISC_SOUTH_CTL register");

            if (l_vas_north_misc_ctl_data.getBit<PU_VAS_MISCCTL_MISC_CTL_RG_IS_IDLE>()
                && l_vas_south_misc_ctl_data.getBit<PU_VAS_SOUTHCTL_SOUTH_CTL_WC_IDLE_BIT>()
                && l_vas_south_misc_ctl_data.getBit<PU_VAS_SOUTHCTL_SOUTH_CTL_CQ_IDLE_BIT>()
                && l_vas_south_misc_ctl_data.getBit<PU_VAS_SOUTHCTL_SOUTH_CTL_EG_IDLE_BIT>())
            {
                l_quiesce_achieved = true;
                break;
            }
        }

        //In order to prevent additional FIFO entries from getting posted to the NX receive FIFOs while trying to quiesce NX, software may wish to close all windows to prevent users from continuing to try to access the accelerators. Software may close all windows by writing the Open/Enable bit to a zero in the Window Control Register<n>. This step is optional, but should be done as part of an orderly shut down of a user's access.

        //Software may also choose to deallocate any pages that partitions (or users) have mapped to VAS' MMIO space. In a general use case, a partition (or user) will have pages that map to VAS' MMIO space to allow the partition to return credits via the Local Receive Window Credit Adder Register <m>. In order to stop MMIO traffic, these pages should be unmapped. In a NX only usage model, this step can be ignored as long as the Quiesce NX procedures are followed.

        FAPI_ASSERT(l_quiesce_achieved, fapi2::P9_VAS_QUIESCE_TIMEOUT().set_TARGET(i_target).set_NORTHDATA(
                        l_vas_north_misc_ctl_data).set_SOUTHDATA(l_vas_south_misc_ctl_data),
                    "VAS quiesce timed out");

        // Write Invalidate CAM location field of North register (optional)
        l_vas_north_misc_ctl_data.setBit<PU_VAS_MISCCTL_MISC_CTL_INVALIDATE_CAM_ALL>();
        FAPI_TRY(fapi2::putScom(i_target, PU_VAS_MISCCTL, l_vas_north_misc_ctl_data),
                 "Error writing to VAS_MISC_NORTH_CTL register");

    fapi_try_exit:
        FAPI_DBG("Exiting...");
        return fapi2::current_err;
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_nx_check_quiesce(const
                                          fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        fapi2::buffer<uint64_t> l_dma_status_reg_data(0);
        fapi2::buffer<uint64_t> l_pb_err_rpt0_data(0);
        fapi2::buffer<uint64_t> l_umac_status_ctrl_data(0);
        fapi2::buffer<uint64_t> l_crb_kill_req_data(0);
        fapi2::buffer<uint64_t> l_erat_status_control_data(0);
        bool l_quiesce_achieved = false;
        bool l_go_to_next_nx_step = false;

        //If (DMA Status Register[HMI Ative])
        FAPI_TRY(fapi2::getScom(i_target, PU_SU_STATUS, l_dma_status_reg_data), "Error reading from DMA Status Register");

        if(l_dma_status_reg_data.getBit<PU_SU_STATUS_HMI_ACTIVE>())
        {
            //then while (!PowerBus Interface Error Report 0 Register[PBI Write Idle])
            for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
            {
                FAPI_TRY(fapi2::getScom(i_target, PU_NX_PB_ERR_RPT_0, l_pb_err_rpt0_data),
                         "Error reading from PowerBus Interface Error Report");

                if (l_pb_err_rpt0_data.getBit<PU_NX_PB_ERR_RPT_0_PBI_WRITE_IDLE>())
                {
                    l_go_to_next_nx_step = true;
                    break;
                }
            }

            FAPI_ASSERT(l_go_to_next_nx_step, fapi2::P9_NX_PBI_WRITE_IDLE_TIMEOUT().set_TARGET(i_target).set_DATA(
                            l_pb_err_rpt0_data), "PBI Write Idle never happened");
            l_go_to_next_nx_step = false;

            //Any CRB kill must be complete before issuing the following sequence
            FAPI_TRY(fapi2::getScom(i_target, PU_SU_CRB_KILL_REQ, l_crb_kill_req_data), "Error reading CRB Kill Request Register");

            //if (CRB Kill Request Configuration Register[Kill Enable])
            if (l_crb_kill_req_data.getBit<PU_SU_CRB_KILL_REQ_ENABLE>())
            {
                //while(!CRB Kill Request Configuration Register[Kill Done]){}
                for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
                {
                    FAPI_TRY(fapi2::getScom(i_target, PU_SU_CRB_KILL_REQ, l_crb_kill_req_data), "Error reading CRB Kill Request Register");

                    if (l_crb_kill_req_data.getBit<PU_SU_CRB_KILL_REQ_DONE>())
                    {
                        l_go_to_next_nx_step = true;
                        break;
                    }
                }

                FAPI_ASSERT(l_go_to_next_nx_step, fapi2::P9_NX_CRB_KILL_DONE_TIMEOUT().set_TARGET(i_target).set_DATA(
                                l_crb_kill_req_data), "CRB kills were not complete");
            }

            l_go_to_next_nx_step = false;

            //Stop UMAC from fetching new CRBs
            FAPI_TRY(fapi2::getScom(i_target, PU_UMAC_STATUS_CONTROL, l_umac_status_ctrl_data),
                     "Error reading from UMAC_STATUS_CONTROL_REG");
            //clear_reg(UMAC Status and Control Register[CRB Read Enable])
            l_umac_status_ctrl_data.clearBit<PU_UMAC_STATUS_CONTROL_CRB_READS_ENBL>();
            FAPI_TRY(fapi2::putScom(i_target, PU_UMAC_STATUS_CONTROL, l_umac_status_ctrl_data),
                     "Error writing to UMAC_STATUS_CONTROL_REG");

            //while(!UMAC Status and Control Register[CRB Read Halted]){}
            for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
            {
                FAPI_TRY(fapi2::getScom(i_target, PU_UMAC_STATUS_CONTROL, l_umac_status_ctrl_data),
                         "Error reading from UMAC_STATUS_CONTROL_REG");

                if (l_umac_status_ctrl_data.getBit<PU_UMAC_STATUS_CONTROL_CRB_READS_HALTED>())
                {
                    l_go_to_next_nx_step = true;
                    break;
                }
            }

            FAPI_ASSERT(l_go_to_next_nx_step, fapi2::P9_NX_STOP_UMAC_FETCHING_NEW_CRBS_TIMEOUT().set_TARGET(i_target).set_DATA(
                            l_umac_status_ctrl_data), "UMAC was not stopped from fetching new CRBs");
            l_go_to_next_nx_step = false;

            //Wait for UMAC dispatch slots to drain of CRBs
            //UMAC can still have CRBs queued in receive FIFOs
            //while(!UMAC Status and Control Register[UMAC Quiesced]){}
            for(uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
            {
                FAPI_TRY(fapi2::getScom(i_target, PU_UMAC_STATUS_CONTROL, l_umac_status_ctrl_data),
                         "Error reading from UMAC_STATUS_CONTROL_REG");

                if (l_umac_status_ctrl_data.getBit<PU_UMAC_STATUS_CONTROL_QUIESCED>())
                {
                    l_go_to_next_nx_step = true;
                    break;
                }
            }

            FAPI_ASSERT(l_go_to_next_nx_step, fapi2::P9_NX_UMAC_DISPATCH_SLOTS_TO_DRAIN_CRBS_TIMEOUT().set_TARGET(
                            i_target).set_DATA(
                            l_umac_status_ctrl_data), "UMAC was not done dispatching slots to drain of CRBs");
            l_go_to_next_nx_step = false;

            //Wait for DMA channels to drain
            //while(DMA Status Register[DMA Channel 0:4 Idle] != 5b1_1111){}
            for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
            {
                FAPI_TRY(fapi2::getScom(i_target, PU_SU_STATUS, l_dma_status_reg_data), "Error reading from DMA Status Register");

                if (l_dma_status_reg_data.getBit<PU_SU_STATUS_DMA_CH0_IDLE>()
                    && l_dma_status_reg_data.getBit<PU_SU_STATUS_DMA_CH1_IDLE>()
                    && l_dma_status_reg_data.getBit<PU_SU_STATUS_DMA_CH2_IDLE>()
                    && l_dma_status_reg_data.getBit<PU_SU_STATUS_DMA_CH3_IDLE>()
                    && l_dma_status_reg_data.getBit<PU_SU_STATUS_DMA_CH4_IDLE>())
                {
                    l_go_to_next_nx_step = true;
                    break;
                }
            }

            FAPI_ASSERT(l_go_to_next_nx_step, fapi2::P9_WAIT_FOR_DMA_CHANNELS_TO_DRAIN_TIMEOUT().set_TARGET(i_target).set_DATA(
                            l_dma_status_reg_data), "DMA channels were not drained");
            l_go_to_next_nx_step = false;

            //Wait for ERAT to be idle. Should be trivially idle because of the above.
            //while(!ERAT Status and Control Register[ERAT idle]){}
            for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
            {
                FAPI_TRY(fapi2::getScom(i_target, PU_ERAT_STATUS_CONTROL, l_erat_status_control_data),
                         "Error reading from the ERAT Status and Control Register");

                if (l_erat_status_control_data.getBit<PU_ERAT_STATUS_CONTROL_IDLE>())
                {
                    l_go_to_next_nx_step = true;
                    break;
                }
            }

            FAPI_ASSERT(l_go_to_next_nx_step, fapi2::P9_WAIT_FOR_ERAT_IDLE().set_TARGET(i_target).set_DATA(
                            l_erat_status_control_data),
                        "ERAT was not idle");
            l_go_to_next_nx_step = false;

            //Wait for PBI master machines to be idle
            //while(!DMA Status Register[PBI Idle]){}
            for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
            {
                FAPI_TRY(fapi2::getScom(i_target, PU_SU_STATUS, l_dma_status_reg_data), "Error reading from DMA Status Register");

                if (l_dma_status_reg_data.getBit<PU_SU_STATUS_PBI_IDLE>())
                {
                    l_go_to_next_nx_step = true;
                    break;
                }
            }

            FAPI_ASSERT(l_go_to_next_nx_step, fapi2::P9_PBI_MASTER_MACHINES_IDLE_TIMEOUT().set_TARGET(i_target).set_DATA(
                            l_dma_status_reg_data), "PBI Master Machines are not idle");
            l_go_to_next_nx_step = false;
        }

        //If this procedure is followed, then usually if UMAC Status and Control Register[Quiesce Request] is written to 1 then hardware will trivially respond UMAC Status and Control Register[Quiesce Achieved] = 1

        // Write to UMAC Control register(bits 4:6) with '100'
        //TODO RTC 160710 for DD1 this is broken, will readd when we are on DD2
        if (C_NX_DD2_READY)
        {
            FAPI_TRY(fapi2::getScom(i_target, PU_UMAC_STATUS_CONTROL, l_umac_status_ctrl_data),
                     "Error reading from UMAC_STATUS_CONTROL_REG");
            l_umac_status_ctrl_data.setBit<PU_UMAC_STATUS_CONTROL_QUIESCE_REQUEST>().clearBit<PU_UMAC_STATUS_CONTROL_QUIESCE_ACHEIVED>().clearBit<PU_UMAC_STATUS_CONTROL_QUIESCE_FAILED>();
            FAPI_TRY(fapi2::putScom(i_target, PU_UMAC_STATUS_CONTROL, l_umac_status_ctrl_data),
                     "Error writing to the UMAC_STATUS_CONTROL_REG");

            // Poll UMAC Control register status register (bit 5, bit 6 indicates fail)
            for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
            {
                FAPI_TRY(fapi2::getScom(i_target, PU_UMAC_STATUS_CONTROL, l_umac_status_ctrl_data),
                         "Error reading from UMAC_STATUS_CONTROL_REG");

                if (l_umac_status_ctrl_data.getBit<PU_UMAC_STATUS_CONTROL_QUIESCE_ACHEIVED>())
                {
                    l_quiesce_achieved = true;
                    break;
                }

                FAPI_ASSERT(!l_umac_status_ctrl_data.getBit<PU_UMAC_STATUS_CONTROL_QUIESCE_FAILED>(),
                            fapi2::P9_UMAC_QUIESCE_FAILED().set_TARGET(i_target).set_DATA(l_umac_status_ctrl_data),
                            "UMAC status control quiesce failed");
            }

            FAPI_ASSERT(l_quiesce_achieved, fapi2::P9_UMAC_QUIESCE_TIMEOUT().set_TARGET(i_target).set_DATA(l_umac_status_ctrl_data),
                        "UMAC status control quiesce timed out");
        }

    fapi_try_exit:
        FAPI_DBG("Exiting...");
        return fapi2::current_err;
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_hca_check_quiesce(const
                                           fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // Need to flush

        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        //TODO RTC 160709 John said that at the point HCA looks to be out of plan. There are updates required to make functional for DD2. I am supposed to check back in a couple of months (October 2016) to confirm that it's not in plan.

        return fapi2::FAPI2_RC_SUCCESS;

        /*fapi_try_exit:
          FAPI_DBG("Exiting...");
          return fapi2::current_err;*/
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_psihb_check_quiesce(const
            fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        fapi2::buffer<uint64_t> l_psihb_data(0);
        const uint64_t c_error_mask_disable_all = 0xFFFull;
        bool l_inbound_queue_empty = false;

        // Need mechanism to quiesce PSI DMAs
        // There are bits on the PSIHB to force the DMAs to be rejected

        // Disable FSP Command Enable bit in PSIHB Command/Status register
        l_psihb_data.setBit<PU_PSIHB_STATUS_CTL_REG_FSP_CMD_ENABLE>().setBit<PU_PSIHB_STATUS_CTL_REG_FSP_INT_ENABLE>();
        FAPI_TRY(fapi2::putScom(i_target, PU_PSIHB_STATUS_CTL_REG_SCOM2, l_psihb_data),
                 "Error writing to PSIHB_STATUS_CTL_REG");

        //mask all interrupts to quiesce
        l_psihb_data.flush<0>().insertFromRight<PU_PSIHB_ERROR_MASK_REG_INTERRUPT_DISABLE, PU_PSIHB_ERROR_MASK_REG_INTERRUPT_DISABLE_LEN>
        (c_error_mask_disable_all);
        FAPI_TRY(fapi2::putScom(i_target, PU_PSIHB_ERROR_MASK_REG, l_psihb_data), "Error writing to PSIHB_ERROR_MASK_REG");

        //Poll PSIHBCR bit 20 - inbound queue empty to be 0b0 for quiesce state
        for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
        {
            FAPI_TRY(fapi2::getScom(i_target, PU_PSIHB_STATUS_CTL_REG_SCOM, l_psihb_data),
                     "Error reading from PSIHB_STATUS_CTL_REG");

            if (!l_psihb_data.getBit<PU_PSIHB_STATUS_CTL_REG_FSP_INBOUND_ACTIVE>())
            {
                l_inbound_queue_empty = true;
                break;
            }
        }

        FAPI_ASSERT(l_inbound_queue_empty, fapi2::P9_PSIHBCR_INBOUND_QUEUE_NOT_EMPTY().set_TARGET(i_target).set_DATA(
                        l_psihb_data), "PSIHBCR inbound queue not empty");

    fapi_try_exit:
        FAPI_DBG("Exiting...");
        return fapi2::current_err;
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_intp_check_quiesce(const
                                            fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // mark HWP entry
        FAPI_DBG("Entering ...\n");

        fapi2::buffer<uint64_t> l_int_cq_rst_ctl_data(0);
        bool l_quiesce_achieved = false;

        // Read INT_CQ_RST_CTL so that we don't override anything
        FAPI_TRY(fapi2::getScom(i_target, PU_INT_CQ_RST_CTL, l_int_cq_rst_ctl_data),
                 "Error reading INT_CQ_RST_CTL register to get initial value");

        // Set bit in INT_CQ_RST_CTL to request quiesce
        l_int_cq_rst_ctl_data.setBit<PU_INT_CQ_RST_CTL_QUIESCE_PB>();
        FAPI_TRY(fapi2::putScom(i_target, PU_INT_CQ_RST_CTL, l_int_cq_rst_ctl_data), "Error writing INT_CQ_RST_CTL register");

        // Poll master and slave quiesced via bits in RST_CTL
        for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
        {
            FAPI_TRY(fapi2::getScom(i_target, PU_INT_CQ_RST_CTL, l_int_cq_rst_ctl_data),
                     "Error reading INT_CQ_RST_CTL register to poll quiesce bits");

            if (l_int_cq_rst_ctl_data.getBit<PU_INT_CQ_RST_CTL_MASTER_IDLE>()
                && l_int_cq_rst_ctl_data.getBit<PU_INT_CQ_RST_CTL_SLAVE_IDLE>())
            {
                l_quiesce_achieved = true;
                break;
            }
        }

        FAPI_ASSERT(l_quiesce_achieved, fapi2::P9_INTP_QUIESCE_TIMEOUT().set_TARGET(i_target).set_DATA(
                        l_int_cq_rst_ctl_data), "INTP master or slave is not IDLE");

        //Set sync_reset in RST_CTL
        l_int_cq_rst_ctl_data.setBit<PU_INT_CQ_RST_CTL_SYNC_RESET>();
        FAPI_TRY(fapi2::putScom(i_target, PU_INT_CQ_RST_CTL, l_int_cq_rst_ctl_data),
                 "Error writing INT_CQ_RST_CTL register to set reset");


    fapi_try_exit:
        FAPI_DBG("Exiting...");
        return fapi2::current_err;
    }

} // extern "C"

