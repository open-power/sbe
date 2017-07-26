/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/nest/p9_sbe_check_quiesce.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2017                        */
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

//*******************************************************************************************************
//*******************************************************************************************************
//TODO RTC Story 169882 - I NEED TO ADD IN ERROR HANDLING THAT WAS TAKEN OUT TO ALLOW THIS TO FIT
//IN SBE!! THIS IS NOT READY FOR GA OR PRODUCTION YET BUT CAN BE USED FOR TESTING
//*******************************************************************************************************
//*******************************************************************************************************

//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <p9_sbe_check_quiesce.H>
#include <p9_quad_scom_addresses.H>
#include <p9_misc_scom_addresses_fld.H>
#include <p9_misc_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p9_perv_scom_addresses.H>
#include <p9_suspend_io.H>
#include <p9n2_misc_scom_addresses.H>

//Needed for SW reset of XIVE unit
#include <p9_thread_control.H>
#include <p9_fbc_utils.H>
#include <p9_adu_setup.H>
#include <p9_adu_access.H>
#include <p9_adu_coherent_utils.H>


extern "C" {

    //This is how many times each unit will try to do the quiesce if it has a wait for some cycles
    const uint32_t C_NUM_TRIES_QUIESCE_STATE = 10000;
    //These are the delays for the units that need them
    const uint32_t C_CAPP_DELAY_NS = 168000000 / C_NUM_TRIES_QUIESCE_STATE; //168 ms
    const uint32_t C_CAPP_DELAY_CYCLES = 336000000 / C_NUM_TRIES_QUIESCE_STATE; //2GHz * 168 ms
    const uint32_t C_NPU_DELAY_NS = 150000 / C_NUM_TRIES_QUIESCE_STATE; //150 microseconds
    const uint32_t C_NPU_DELAY_CYCLES = 300000 / C_NUM_TRIES_QUIESCE_STATE; //2GHz * 150 microseconds
    const uint32_t C_DELAY_NS_396 = 396000000 / C_NUM_TRIES_QUIESCE_STATE; //396 ms -- Scott said this is too hard to answer
    const uint32_t C_DELAY_CYCLES_396 = 792000000 / C_NUM_TRIES_QUIESCE_STATE; //2GHz * 396 ms
    const uint32_t C_INTP_DELAY_NS = 10000 / C_NUM_TRIES_QUIESCE_STATE; //10 microseconds
    const uint32_t C_INTP_DELAY_CYCLES = 20000 / C_NUM_TRIES_QUIESCE_STATE; //2GHz * 10 microseconds

//--------------------------------------------------------------------------
//  HWP entry point
//--------------------------------------------------------------------------
    fapi2::ReturnCode p9_sbe_check_quiesce(const
                                           fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_IMP("p9_sbe_check_quiesce: Entering..");
        // mark HWP entry

        // SBE will check quiesce state for all units on the powerbus on its chip
        FAPI_TRY(p9_capp_check_quiesce(i_target), "Error from p9_capp_check_quiesce");
        FAPI_TRY(p9_phb_check_quiesce(i_target), "Error from p9_phb_check_quiesce");
        FAPI_TRY(p9_npu_check_quiesce(i_target), "Error from p9_npu_check_quiesce");
        FAPI_TRY(p9_vas_check_quiesce(i_target), "Error from p9_vas_check_quiesce");
        FAPI_TRY(p9_nx_check_quiesce(i_target), "Error from p9_nx_check_quiesce");
        FAPI_TRY(p9_psihb_check_quiesce(i_target), "Error from p9_psihb_check_quiesce");
        FAPI_TRY(p9_intp_check_quiesce(i_target), "Error from p9_intp_check_quiesce");

        //We also need to clean up any active special wakeups, and redirect
        //special wakeups to the SGPE
        FAPI_TRY(p9_pm_check_quiesce(i_target), " Error from p9_pm_check_quiesce");

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

        FAPI_IMP("p9_sbe_check_quiesce: Exiting..");
        return saveError;
    }

//---------------------------------------------------------------------------
//  Helper Functions
//---------------------------------------------------------------------------

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_capp_check_quiesce(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_IMP("p9_capp_check_quiesce: Entering ....");
        // mark HWP entry

        fapi2::buffer<uint64_t> l_data(0);

        uint8_t l_useXiveHwReset;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_USE_XIVE_HW_RESET,
                               i_target,
                               l_useXiveHwReset));

        //TODO RTC:177741 HW Reset for XIVE isnt working , use this workaround until it does
        if(!l_useXiveHwReset)
        {
            //This part is actually used for the intp quiesce DD1 workaround but needs to be here because after this
            //the fabric is finished
            uint64_t l_notify_page_addr = 0x0ull;
            uint32_t l_numGranules;
            p9_ADU_oper_flag l_adu_flag;
            uint8_t l_write_data[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

            //Read the Interrupt Controller BAR to figure out the Notify Port page address
            //Notify Port Page is the second page off the IC BAR
            //bit 1 will tell whether we need to add a 4K offset (if bit 1 = '0') or 64K offset (if bit 1 = '1')
            //Add this to the address (bits 8:48 of the IC BAR)
            fapi2::getScom(i_target, PU_INT_CQ_IC_BAR, l_data);
            l_notify_page_addr = (l_data & 0x00FFFFFFFFFF8000ull) + 0x1000;

            if (l_data.getBit<1>() != 0)
            {
                l_notify_page_addr = l_notify_page_addr + 0xF000;
            }

            //---------------------
            // Use syncs to mak sure no more requests are pending on the queues
            //---------------------
            //Trigger VC syncs
            //write IPI trigger sync
            l_adu_flag.setAutoIncrement(false);
            l_adu_flag.setOperationType(p9_ADU_oper_flag::CACHE_INHIBIT);
            l_adu_flag.setLockControl(true);
            l_adu_flag.setOperFailCleanup(true);
            l_adu_flag.setFastMode(false);
            l_adu_flag.setItagMode(false);
            l_adu_flag.setEccMode(false);
            l_adu_flag.setEccItagOverrideMode(false);
            l_adu_flag.setTransactionSize(static_cast<p9_ADU_oper_flag::Transaction_size_t>(0x8));
            FAPI_TRY(p9_adu_setup(i_target, (l_notify_page_addr + 0xC00), false, l_adu_flag.setFlag(), l_numGranules));
            FAPI_TRY(p9_adu_access(i_target, (l_notify_page_addr + 0xC00), false, l_adu_flag.setFlag(), true, true, l_write_data));
            //write HW trigger sync
            FAPI_TRY(p9_adu_setup(i_target, (l_notify_page_addr + 0xC80), false, l_adu_flag.setFlag(), l_numGranules));
            FAPI_TRY(p9_adu_access(i_target, (l_notify_page_addr + 0xC80), false, l_adu_flag.setFlag(), true, true, l_write_data));
            //write OS trigger sync
            FAPI_TRY(p9_adu_setup(i_target, (l_notify_page_addr + 0xD00), false, l_adu_flag.setFlag(), l_numGranules));
            FAPI_TRY(p9_adu_access(i_target, (l_notify_page_addr + 0xD00), false, l_adu_flag.setFlag(), true, true, l_write_data));
            //write Hyp trigger sync
            FAPI_TRY(p9_adu_setup(i_target, (l_notify_page_addr + 0xD80), false, l_adu_flag.setFlag(), l_numGranules));
            FAPI_TRY(p9_adu_access(i_target, (l_notify_page_addr + 0xD80), false, l_adu_flag.setFlag(), true, true, l_write_data));
            //Write Redist trigger sync
            FAPI_TRY(p9_adu_setup(i_target, (l_notify_page_addr + 0xE00), false, l_adu_flag.setFlag(), l_numGranules));
            FAPI_TRY(p9_adu_access(i_target, (l_notify_page_addr + 0xE00), false, l_adu_flag.setFlag(), true, true, l_write_data));
        }

        fapi2::getScom(i_target, CAPP_FLUSHSHUE, l_data);

        if (l_data != 0x0ull)
        {

            // read the value of CAPP Error status and control register so we don't write over something
            fapi2::getScom(i_target, CAPP_CAPP_ERR_STATUS_CONTROL, l_data);

            // Write the Force Quiesce bit
            l_data.setBit<CAPP_CAPP_ERR_STATUS_CONTROL_FORCE_QUIESCE>();
            fapi2::putScom(i_target, CAPP_CAPP_ERR_STATUS_CONTROL, l_data);

            // Poll the Quiesce done bit
            for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
            {
                fapi2::getScom(i_target, CAPP_CAPP_ERR_STATUS_CONTROL, l_data);

                if (!l_data.getBit<CAPP_CAPP_ERR_STATUS_CONTROL_QUIESCE_DONE>())
                {
                    break;
                }

                fapi2::delay(C_CAPP_DELAY_NS, C_CAPP_DELAY_CYCLES);
            }

            FAPI_ASSERT(!l_data.getBit<CAPP_CAPP_ERR_STATUS_CONTROL_QUIESCE_DONE>(),
                        fapi2::P9_CAPP_QUIESCE_TIMEOUT().set_TARGET(i_target).set_DATA(l_data),
                        "CAPP quiesce timed out");
        }

    fapi_try_exit:
        FAPI_IMP("p9_capp_check_quiesce: Exiting ....");
        return fapi2::current_err;
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_phb_check_quiesce(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_IMP("p9_phb_check_quiesce: Entering ...");
        // mark HWP entry
        fapi2::buffer<uint64_t> l_data(0);
        //We want to set bit 0 (the Quiesce DMA bit)
        //This is the data that will be passed in to set the PHB Quiesce DMA register

        //The address of the PHB Quiesce DMA Register is 0x0888 (found in PHB spec)
        uint8_t l_pci_id = 0;
        uint64_t phb_absolute_address_array[3];
        uint32_t num_phbs = 0;

        auto l_pci_chiplets_vec = i_target.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_ALL_PCI,
                                  fapi2::TARGET_STATE_FUNCTIONAL);

        for (auto& l_pci_chiplet : l_pci_chiplets_vec)
        {
            //Get the PCI ID
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_pci_chiplet, l_pci_id));

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
                //Need to make sure that tc_pci1_iovalid(2) is 0b1 to access PHB2
                fapi2::buffer<uint64_t> l_perv_pci1_cplt_conf1_data(0);
                fapi2::getScom(i_target, PEC_1_CPLT_CONF1, l_perv_pci1_cplt_conf1_data);

                if(l_perv_pci1_cplt_conf1_data.getBit<PEC_CPLT_CONF1_IOVALID_5D>())
                {
                    // PHB 2 is enabled
                    num_phbs = 2;
                    phb_absolute_address_array[0] = PHB_1_PHB4_SCOM_HVIAR;
                    phb_absolute_address_array[1] = PHB_2_PHB4_SCOM_HVIAR;
                }
                else
                {
                    // PHB2 is disabled
                    num_phbs = 1;
                    phb_absolute_address_array[0] = PHB_1_PHB4_SCOM_HVIAR;
                }
            }
            else if (l_pci_id == 0xf)
            {
                //Need to make sure that tc_pci2_iovalid(2) is 0b1 to access PHB5
                fapi2::buffer<uint64_t> l_perv_pci2_cplt_conf1_data(0);
                fapi2::getScom(i_target, PEC_2_CPLT_CONF1, l_perv_pci2_cplt_conf1_data);

                if (l_perv_pci2_cplt_conf1_data.getBit<PEC_CPLT_CONF1_IOVALID_6D>())
                {
                    num_phbs = 3;
                    phb_absolute_address_array[2] = PHB_5_PHB4_SCOM_HVIAR;
                }
                else
                {
                    num_phbs = 2;
                }

                phb_absolute_address_array[0] = PHB_3_PHB4_SCOM_HVIAR;
                phb_absolute_address_array[1] = PHB_4_PHB4_SCOM_HVIAR;
            }

            for (uint32_t i = 0; i < num_phbs; i++)
            {
                //Clear contents of PHB HV Indirect Address Register
                l_data.flush<0>();
                fapi2::putScom(i_target, phb_absolute_address_array[i], l_data);
                //Setup the PHB HV registers for the write
                l_data.insertFromRight<PHB_HV_IND_ADDR_START_BIT, PHB_HV_IND_ADDR_LEN>(0x888);
                l_data.setBit<PHB_HV_IND_ADDR_VALID_BIT>();
                fapi2::putScom(i_target, phb_absolute_address_array[i], l_data);
                //Setup PHB HV Indirect for write access
                l_data.flush<0>().insertFromRight<0, 63>(0x8000000000000000);
                fapi2::putScom(i_target, (phb_absolute_address_array[i] + 1), l_data);
                //Clear contents of PHB HV Indirect Address Register
                l_data.flush<0>();
                fapi2::putScom(i_target, phb_absolute_address_array[i], l_data);
            }
        }

        FAPI_TRY(p9_suspend_io(i_target, true), "ERROR suspending IO");

    fapi_try_exit:
        FAPI_IMP("p9_phb_check_quiesce: Exiting ...");
        return fapi2::current_err;
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_npu_check_quiesce(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_IMP("p9_npu_check_quiesce: Entering...");
        // mark HWP entry

        const uint32_t c_fence_status_reg_size = 6;

        fapi2::buffer<uint64_t> l_data(0);
        const uint32_t CQ_fence_status_regs[c_fence_status_reg_size] = {0x00090500, 0x000B0500, 0x00190500, 0x001B0500, 0x00290500, 0x002B0500};
        const uint32_t c_config_size = 6;
#ifndef DD2
        const uint32_t c_GPU_Memory_BARs_size = 12;
        const uint32_t c_memory_bars_size = 36;
        const uint64_t l_GPU_Memory_BARs[c_GPU_Memory_BARs_size] = {PU_NPU0_SM0_GPU_BAR, PU_NPU0_SM1_GPU_BAR, PU_NPU0_SM2_GPU_BAR, PU_NPU0_SM3_GPU_BAR, PU_NPU1_SM0_GPU_BAR, PU_NPU1_SM1_GPU_BAR, PU_NPU1_SM2_GPU_BAR, PU_NPU1_SM3_GPU_BAR, PU_NPU2_SM0_GPU_BAR, PU_NPU2_SM1_GPU_BAR, PU_NPU2_SM2_GPU_BAR, PU_NPU2_SM3_GPU_BAR};
        const uint64_t l_memory_bars[c_memory_bars_size] = {PU_NPU0_SM0_NDT0_BAR, PU_NPU0_SM1_NDT0_BAR, PU_NPU0_SM2_NDT0_BAR, PU_NPU0_SM3_NDT0_BAR, PU_NPU1_SM0_NDT0_BAR, PU_NPU1_SM1_NDT0_BAR, PU_NPU1_SM2_NDT0_BAR, PU_NPU1_SM3_NDT0_BAR, PU_NPU2_SM0_NDT0_BAR, PU_NPU2_SM1_NDT0_BAR, PU_NPU2_SM2_NDT0_BAR, PU_NPU2_SM3_NDT0_BAR, PU_NPU0_SM0_NDT1_BAR, PU_NPU0_SM1_NDT1_BAR, PU_NPU0_SM2_NDT1_BAR, PU_NPU0_SM3_NDT1_BAR, PU_NPU1_SM0_NDT1_BAR, PU_NPU1_SM1_NDT1_BAR, PU_NPU1_SM2_NDT1_BAR, PU_NPU1_SM3_NDT1_BAR, PU_NPU2_SM0_NDT1_BAR, PU_NPU2_SM1_NDT1_BAR, PU_NPU2_SM2_NDT1_BAR, PU_NPU2_SM3_NDT1_BAR, PU_NPU0_SM0_PHY_BAR, PU_NPU0_SM1_PHY_BAR, PU_NPU0_SM2_PHY_BAR, PU_NPU0_SM3_PHY_BAR, PU_NPU1_SM0_PHY_BAR, PU_NPU1_SM1_PHY_BAR, PU_NPU1_SM2_PHY_BAR, PU_NPU1_SM3_PHY_BAR, PU_NPU2_SM0_PHY_BAR, PU_NPU2_SM1_PHY_BAR, PU_NPU2_SM2_PHY_BAR, PU_NPU2_SM3_PHY_BAR };
        const uint64_t l_NTL_config_addrs[c_config_size] = {NV_0_CONFIG1, NV_1_CONFIG1, NV_2_CONFIG1, NV_3_CONFIG1, PU_NPU2_NTL0_CONFIG1, PU_NPU2_NTL1_CONFIG1};
#else
        const uint32_t c_GPU_Memory_BARs_DD2_size = 24;
        const uint32_t c_memory_bars_size_dd2 = 48;
        const uint32_t l_GPU_Memory_BARs_DD2[c_GPU_Memory_BARs_DD2_size] = {0x5011004, 0x5011034, 0x5011064, 0x5011094, 0x5011204, 0x5011234, 0x5011264, 0x5011294, 0x5011404, 0x5011434, 0x5011464, 0x5011494, 0x5011005, 0x5011035, 0x5011065, 0x5011095, 0x5011205, 0x5011235, 0x5011265, 0x5011295, 0x5011405, 0x5011435, 0x5011465, 0x5011495};
        const uint32_t l_memory_bars_dd2[c_memory_bars_size_dd2] = {0x501100D, 0x501103D, 0x501106D, 0x501109D, 0x501100E, 0x501103E, 0x501106E, 0x501109E, 0x501120D, 0x501123D, 0x501126D, 0x501129D, 0x501120E, 0x501123E, 0x501126E, 0x501129E, 0x501140D, 0x501143D, 0x501146D, 0x501149D, 0x501140E, 0x501143E, 0x501146E, 0x501149E, 0x5011406, 0x5011436, 0x5011466, 0x5011496, 0x5011206, 0x5011236, 0x5011266, 0x5011296, 0x5011007, 0x5011037, 0x5011067, 0x5011097, 0x5011207, 0x5011237, 0x5011267, 0x5011297, 0x5011407, 0x5011437, 0x5011467, 0x5011497, 0x5011006, 0x5011036, 0x5011366, 0x5011396};
        const uint32_t l_NTL_config_addrs_DD2[c_config_size] = {0x5011128, 0x5011148, 0x5011328, 0x5011348, 0x5011528, 0x5011548};
#endif
        uint32_t l_GPU_Memory_BARs_size_for_loop = 0;
        uint32_t l_memory_bars_size_for_loop = 0;


#ifndef DD2
        l_GPU_Memory_BARs_size_for_loop = c_GPU_Memory_BARs_size;
        l_memory_bars_size_for_loop = c_memory_bars_size;
#else
        l_GPU_Memory_BARs_size_for_loop = c_GPU_Memory_BARs_DD2_size;
        l_memory_bars_size_for_loop = c_memory_bars_size_dd2;
#endif

        // 1) Place all six of the NTLs into Reset State
        // Set bits 8:9 in the NTL Misc Config 1 registers to place NTLs in Reset state
        for (uint32_t i = 0; i < c_config_size; i++)
        {
#ifndef DD2
            fapi2::getScom(i_target, l_NTL_config_addrs[i], l_data);
#else
            fapi2::getScom(i_target, l_NTL_config_addrs_DD2[i], l_data);
#endif
            l_data.insertFromRight<NV_CONFIG1_NTL_RESET, NV_CONFIG1_NTL_RESET_LEN>(0x3);
#ifndef DD2
            fapi2::putScom(i_target, l_NTL_config_addrs[i], l_data);
#else
            fapi2::putScom(i_target, l_NTL_config_addrs_DD2[i], l_data);
#endif
        }

        //Poll the CQ Fence Status Registers util "Value" is detected to verify that NTLs are in Reset State
        for (uint32_t i = 0; i < c_fence_status_reg_size; i++)
        {
            for (uint32_t j = 0; j < CAPP_CAPP_ERR_STATUS_CONTROL_QUIESCE_DONE; j++)
            {
                l_data.flush<0>().insertFromRight<PU_NPU_CTL_DA_ADDR_MISC, PU_NPU_CTL_DA_ADDR_MISC_LEN>
                (CQ_fence_status_regs[i]);

#ifndef DD2
                fapi2::putScom(i_target, PU_NPU_CTL_DA_ADDR, l_data);
                fapi2::getScom(i_target, PU_NPU_CTL_DA_DATA, l_data);
#else
                fapi2::putScom(i_target, 0x501168E, l_data);
                fapi2::getScom(i_target, 0x501168F, l_data);
#endif

                if (l_data.getBit<0>() && l_data.getBit<1>())
                {
                    break;
                }

                fapi2::delay(C_NPU_DELAY_NS, C_NPU_DELAY_CYCLES);
            }

            FAPI_ASSERT((l_data.getBit<0>()
                         && l_data.getBit<1>()), fapi2::P9_NTL_NOT_IN_RESET().set_TARGET(i_target).set_NTL_ADDR(
                            CQ_fence_status_regs[i]).set_NTL_DATA(l_data), "One of the NTLS are not in the reset state");
        }

        // 2) Place all NV-Link Bricks into Fence State
        // Set bits 0:5 in the NPU Fence State register to place all bricks into Fence State
        l_data.setBit<PU_NPU_CTL_FENCE_STATE_BRK0>().setBit<PU_NPU_CTL_FENCE_STATE_BRK1>().setBit<PU_NPU_CTL_FENCE_STATE_BRK2>().setBit<PU_NPU_CTL_FENCE_STATE_BRK3>().setBit<PU_NPU_CTL_FENCE_STATE_BRK4>().setBit<PU_NPU_CTL_FENCE_STATE_BRK5>();

#ifndef DD2
        fapi2::putScom(i_target, PU_NPU_CTL_FENCE_STATE, l_data);
#else
        {
            fapi2::buffer<uint64_t> l_npu_interrupt_reg_data(0);
            fapi2::putScom(i_target, 0x5011696, l_data);
            //Write bits 0:22 of the NPU Interrupt Request register to eliminate any interrupt requests that occurred between steps 1 and 2.
            fapi2::putScom(i_target, 0x5011697, l_npu_interrupt_reg_data);
        }
#endif

        // 3) Disable all NPU BAR registers
        // Reset bits 0 and 32 in the GPU-Memory BARs to stop NPU from responding to accesses to GPU memory

        for (uint32_t i = 0; i < l_GPU_Memory_BARs_size_for_loop; i++)
        {
            l_data.flush<0>().clearBit<PU_NPU0_SM0_GPU_BAR_CONFIG_GPU0_ENABLE>().clearBit<PU_NPU0_SM0_GPU_BAR_CONFIG_GPU1_ENABLE>();
#ifndef DD2
            fapi2::putScom(i_target, l_GPU_Memory_BARs[i], l_data);
#else
            fapi2::putScom(i_target, l_GPU_Memory_BARs_DD2[i], l_data);
#endif
        }

        // Reset bit 0 in the NTL0/NDL0 Memory BARs and NTL1/NDL1 Memory BARs to stop NPU from responding to accesses to NTL/NDL registers
        // Reset bit 0 in PHY0/PHY1/NPU MMIO BAR for stack 0 and stack 1 to stop NPU from responding to PHY register accesses
        // Reset bit 0 in PHY0/PHY1/NPU MMIO BARs in stack 2 to stop NPU from responding to NPU MMIO register accesses
        for (uint32_t i = 0; i < l_memory_bars_size_for_loop; i++)
        {
            l_data.flush<0>().clearBit<PU_NPU0_SM0_NDT0_BAR_CONFIG_ENABLE>();
#ifndef DD2
            fapi2::putScom(i_target, l_memory_bars[i], l_data);
#else
            fapi2::putScom(i_target, l_memory_bars_dd2[i], l_data);
#endif
        }

    fapi_try_exit:
        FAPI_IMP("p9_npu_check_quiesce: Exiting...");
        return fapi2::current_err;
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_vas_check_quiesce(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_IMP("p9_vas_check_quiesce: Entering...");
        // mark HWP entry

        fapi2::buffer<uint64_t> l_vas_north_misc_ctl_data(0);
        fapi2::buffer<uint64_t> l_vas_south_misc_ctl_data(0);

        // VAS needs to be quiesced before NX

        // Read the VAS Misc status and North control register so we don't write over anything
        fapi2::getScom(i_target, PU_VAS_MISCCTL, l_vas_north_misc_ctl_data);
        // Set the 'Quiesce Requested' bit in the VAS Miscellaneous Status and North Control Register to a 1. This will prevent VAS from accepting new paste or write monitor operations
        l_vas_north_misc_ctl_data.setBit<PU_VAS_MISCCTL_MISC_CTL_QUIESCE_REQUEST>();
        fapi2::putScom(i_target, PU_VAS_MISCCTL, l_vas_north_misc_ctl_data);

        //Check that VAS has quiesced. This is accomplished by reading two status registers. The "RG is Idle' bit in the VAS Miscellaneous Status and North Control Register as well as the 'EG is Idle', 'CQ is Idle' and 'WC is Idle' bit sin the VAS Miscellaneous Status and South Control Register must all be set to one to indicate that VAS has gone idle.
        for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
        {
            // Read VAS Misc status and North control register to ensure 'RG is idle' and 'EG is idle' bits are both set -
            fapi2::getScom(i_target, PU_VAS_MISCCTL, l_vas_north_misc_ctl_data);
            // Read VAS Misc status and South control register to ensure 'WC is idle' and 'CQ is idle' bits are both set -
            fapi2::getScom(i_target, PU_VAS_SOUTHCTL, l_vas_south_misc_ctl_data);

            if (l_vas_north_misc_ctl_data.getBit<PU_VAS_MISCCTL_MISC_CTL_RG_IS_IDLE>()
                && l_vas_south_misc_ctl_data.getBit<PU_VAS_SOUTHCTL_SOUTH_CTL_WC_IDLE_BIT>()
                && l_vas_south_misc_ctl_data.getBit<PU_VAS_SOUTHCTL_SOUTH_CTL_CQ_IDLE_BIT>()
                && l_vas_south_misc_ctl_data.getBit<PU_VAS_SOUTHCTL_SOUTH_CTL_EG_IDLE_BIT>())
            {
                break;
            }

            fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396);
        }

        //In order to prevent additional FIFO entries from getting posted to the NX receive FIFOs while trying to quiesce NX, software may wish to close all windows to prevent users from continuing to try to access the accelerators. Software may close all windows by writing the Open/Enable bit to a zero in the Window Control Register<n>. This step is optional, but should be done as part of an orderly shut down of a user's access.

        //Software may also choose to deallocate any pages that partitions (or users) have mapped to VAS' MMIO space. In a general use case, a partition (or user) will have pages that map to VAS' MMIO space to allow the partition to return credits via the Local Receive Window Credit Adder Register <m>. In order to stop MMIO traffic, these pages should be unmapped. In a NX only usage model, this step can be ignored as long as the Quiesce NX procedures are followed.

        FAPI_ASSERT((l_vas_north_misc_ctl_data.getBit<PU_VAS_MISCCTL_MISC_CTL_RG_IS_IDLE>()
                     && l_vas_south_misc_ctl_data.getBit<PU_VAS_SOUTHCTL_SOUTH_CTL_WC_IDLE_BIT>()
                     && l_vas_south_misc_ctl_data.getBit<PU_VAS_SOUTHCTL_SOUTH_CTL_CQ_IDLE_BIT>()
                     && l_vas_south_misc_ctl_data.getBit<PU_VAS_SOUTHCTL_SOUTH_CTL_EG_IDLE_BIT>()),
                    fapi2::P9_VAS_QUIESCE_TIMEOUT().set_TARGET(i_target).set_NORTHDATA(
                        l_vas_north_misc_ctl_data).set_SOUTHDATA(l_vas_south_misc_ctl_data),
                    "VAS quiesce timed out");

        // Write Invalidate CAM location field of North register (optional)
        l_vas_north_misc_ctl_data.setBit<PU_VAS_MISCCTL_MISC_CTL_INVALIDATE_CAM_ALL>();
        fapi2::putScom(i_target, PU_VAS_MISCCTL, l_vas_north_misc_ctl_data);

    fapi_try_exit:
        FAPI_IMP("p9_vas_check_quiesce: Exiting...");
        return fapi2::current_err;
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_nx_check_quiesce(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_IMP("p9_nx_check_quiesce: Entering....");
        // mark HWP entry

        fapi2::buffer<uint64_t> l_dma_status_reg_data(0);
        fapi2::buffer<uint64_t> l_data(0);

        //If (DMA Status Register[HMI Ative])
        fapi2::getScom(i_target, PU_SU_STATUS, l_dma_status_reg_data);

        //If HMI has een signaled, NX PBI is frozen and read machines may be stuck active. The quiesce may either be abandoned or write machines idle status can be polled in lieu of the quiesce procedure. The latter is selected here.
        if (l_dma_status_reg_data.getBit<PU_SU_STATUS_HMI_ACTIVE>())
        {
            //then while (!PowerBus Interface Error Report 0 Register[PBI Write Idle])
            for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
            {
                fapi2::getScom(i_target, PU_NX_PB_ERR_RPT_0, l_data);

                if (l_data.getBit<PU_NX_PB_ERR_RPT_0_PBI_WRITE_IDLE>())
                {
                    break;
                }

                fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396);
            }

            FAPI_ASSERT((l_data.getBit<PU_NX_PB_ERR_RPT_0_PBI_WRITE_IDLE>()),
                        fapi2::P9_NX_PBI_WRITE_IDLE_TIMEOUT().set_TARGET(i_target).set_DATA(
                            l_data), "PBI Write Idle never happened");
        }

        if(!l_dma_status_reg_data.getBit<PU_SU_STATUS_HMI_ACTIVE>() || l_data.getBit<PU_NX_PB_ERR_RPT_0_PBI_WRITE_IDLE>())
        {
            //Any CRB kill must be complete before issuing the following sequence
            fapi2::getScom(i_target, PU_SU_CRB_KILL_REQ, l_data);

            //if (CRB Kill Request Configuration Register[Kill Enable])
            if (l_data.getBit<PU_SU_CRB_KILL_REQ_ENABLE>())
            {
                //while(!CRB Kill Request Configuration Register[Kill Done]){}
                for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
                {
                    fapi2::getScom(i_target, PU_SU_CRB_KILL_REQ, l_data);

                    if (l_data.getBit<PU_SU_CRB_KILL_REQ_DONE>())
                    {
                        break;
                    }

                    fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396);
                }

                FAPI_ASSERT(l_data.getBit<PU_SU_CRB_KILL_REQ_DONE>(),
                            fapi2::P9_NX_CRB_KILL_DONE_TIMEOUT().set_TARGET(i_target).set_DATA(
                                l_data), "CRB kills were not complete");
            }

            //Stop UMAC from fetching new CRBs
            fapi2::getScom(i_target, PU_UMAC_STATUS_CONTROL, l_data);
            //clear_reg(UMAC Status and Control Register[CRB Read Enable])
            l_data.clearBit<PU_UMAC_STATUS_CONTROL_CRB_READS_ENBL>();
            fapi2::putScom(i_target, PU_UMAC_STATUS_CONTROL, l_data);

            //while(!UMAC Status and Control Register[CRB Read Halted]){}
            for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
            {
                fapi2::getScom(i_target, PU_UMAC_STATUS_CONTROL, l_data);

                if (l_data.getBit<PU_UMAC_STATUS_CONTROL_CRB_READS_HALTED>())
                {
                    break;
                }

                fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396);
            }

            FAPI_ASSERT(l_data.getBit<PU_UMAC_STATUS_CONTROL_CRB_READS_HALTED>(),
                        fapi2::P9_NX_STOP_UMAC_FETCHING_NEW_CRBS_TIMEOUT().set_TARGET(i_target).set_DATA(
                            l_data), "UMAC was not stopped from fetching new CRBs");

            //Wait for UMAC dispatch slots to drain of CRBs
            //UMAC can still have CRBs queued in receive FIFOs
            //while(!UMAC Status and Control Register[UMAC Quiesced]){}
            for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
            {
                fapi2::getScom(i_target, PU_UMAC_STATUS_CONTROL, l_data);

                if (l_data.getBit<PU_UMAC_STATUS_CONTROL_QUIESCED>())
                {
                    break;
                }

                fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396);
            }


            FAPI_ASSERT(l_data.getBit<PU_UMAC_STATUS_CONTROL_QUIESCED>(),
                        fapi2::P9_NX_UMAC_DISPATCH_SLOTS_TO_DRAIN_CRBS_TIMEOUT().set_TARGET(
                            i_target).set_DATA(
                            l_data), "UMAC was not done dispatching slots to drain of CRBs");

            //Wait for DMA channels to drain
            //while(DMA Status Register[DMA Channel 0:4 Idle] != 5b1_1111){}
            for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
            {
                fapi2::getScom(i_target, PU_SU_STATUS, l_dma_status_reg_data);

                if (l_dma_status_reg_data.getBit<PU_SU_STATUS_DMA_CH0_IDLE>()
                    && l_dma_status_reg_data.getBit<PU_SU_STATUS_DMA_CH1_IDLE>()
                    && l_dma_status_reg_data.getBit<PU_SU_STATUS_DMA_CH2_IDLE>()
                    && l_dma_status_reg_data.getBit<PU_SU_STATUS_DMA_CH3_IDLE>()
                    && l_dma_status_reg_data.getBit<PU_SU_STATUS_DMA_CH4_IDLE>())
                {
                    break;
                }

                fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396);
            }

            FAPI_ASSERT((l_dma_status_reg_data.getBit<PU_SU_STATUS_DMA_CH0_IDLE>()
                         && l_dma_status_reg_data.getBit<PU_SU_STATUS_DMA_CH1_IDLE>()
                         && l_dma_status_reg_data.getBit<PU_SU_STATUS_DMA_CH2_IDLE>()
                         && l_dma_status_reg_data.getBit<PU_SU_STATUS_DMA_CH3_IDLE>()
                         && l_dma_status_reg_data.getBit<PU_SU_STATUS_DMA_CH4_IDLE>()),
                        fapi2::P9_WAIT_FOR_DMA_CHANNELS_TO_DRAIN_TIMEOUT().set_TARGET(i_target).set_DATA(
                            l_dma_status_reg_data), "DMA channels were not drained");

            //Wait for ERAT to be idle. Should be trivially idle because of the above.
            //while(!ERAT Status and Control Register[ERAT idle]){}
            for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
            {
                fapi2::getScom(i_target, PU_ERAT_STATUS_CONTROL, l_data);

                if (l_data.getBit<PU_ERAT_STATUS_CONTROL_IDLE>())
                {
                    break;
                }

                fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396);
            }

            FAPI_ASSERT(l_data.getBit<PU_ERAT_STATUS_CONTROL_IDLE>(), fapi2::P9_WAIT_FOR_ERAT_IDLE().set_TARGET(i_target).set_DATA(
                            l_data),
                        "ERAT was not idle");

            //Wait for PBI master machines to be idle
            //while(!DMA Status Register[PBI Idle]){}
            for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
            {
                fapi2::getScom(i_target, PU_SU_STATUS, l_dma_status_reg_data);

                if (l_dma_status_reg_data.getBit<PU_SU_STATUS_PBI_IDLE>())
                {
                    break;
                }

                fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396);
            }

            FAPI_ASSERT(l_dma_status_reg_data.getBit<PU_SU_STATUS_PBI_IDLE>(),
                        fapi2::P9_PBI_MASTER_MACHINES_IDLE_TIMEOUT().set_TARGET(i_target).set_DATA(
                            l_dma_status_reg_data), "PBI Master Machines are not idle");
        }

        //If this procedure is followed, then usually if UMAC Status and Control Register[Quiesce Request] is written to 1 then hardware will trivially respond UMAC Status and Control Register[Quiesce Achieved] = 1

        // Write to UMAC Control register(bits 4:6) with '100'
        //TODO RTC 160710 for DD1 this is broken, will readd when we are on DD2
#ifdef DD2
        fapi2::getScom(i_target, PU_UMAC_STATUS_CONTROL, l_data);
        l_data.setBit<PU_UMAC_STATUS_CONTROL_QUIESCE_REQUEST>().clearBit<PU_UMAC_STATUS_CONTROL_QUIESCE_ACHEIVED>().clearBit<PU_UMAC_STATUS_CONTROL_QUIESCE_FAILED>();
        fapi2::putScom(i_target, PU_UMAC_STATUS_CONTROL, l_data);

        // Poll UMAC Control register status register (bit 5, bit 6 indicates fail)
        for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
        {
            fapi2::getScom(i_target, PU_UMAC_STATUS_CONTROL, l_data);

            if (!l_data.getBit<PU_UMAC_STATUS_CONTROL_QUIESCE_FAILED>())
            {
                break;
            }

            fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396);
        }


        FAPI_ASSERT(!l_data.getBit<PU_UMAC_STATUS_CONTROL_QUIESCE_FAILED>(),
                    fapi2::P9_UMAC_QUIESCE_FAILED().set_TARGET(i_target).set_DATA(l_data),
                    "UMAC status control quiesce failed");
#endif

    fapi_try_exit:
        FAPI_IMP("p9_nx_check_quiesce: Exiting....");
        return fapi2::current_err;
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_psihb_check_quiesce(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_IMP("p9_psihb_check_quiesce: Entering...");
        // mark HWP entry

        fapi2::buffer<uint64_t> l_psihb_data(0);
        const uint32_t c_error_mask_disable_all = 0xFFFull;

        // Need mechanism to quiesce PSI DMAs
        // There are bits on the PSIHB to force the DMAs to be rejected

        // Disable FSP Command Enable bit in PSIHB Command/Status register
        l_psihb_data.setBit<PU_PSIHB_STATUS_CTL_REG_FSP_CMD_ENABLE>().setBit<PU_PSIHB_STATUS_CTL_REG_FSP_INT_ENABLE>();
        fapi2::putScom(i_target, PU_PSIHB_STATUS_CTL_REG_SCOM2, l_psihb_data);

        //mask all interrupts to quiesce
        fapi2::getScom(i_target, PU_PSIHB_ERROR_MASK_REG, l_psihb_data);
        l_psihb_data.insertFromRight<PU_PSIHB_ERROR_MASK_REG_INTERRUPT_DISABLE, PU_PSIHB_ERROR_MASK_REG_INTERRUPT_DISABLE_LEN>
        (c_error_mask_disable_all);
        fapi2::putScom(i_target, PU_PSIHB_ERROR_MASK_REG, l_psihb_data);

        //Poll PSIHBCR bit 20 - inbound queue empty to be 0b0 for quiesce state
        for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
        {
            fapi2::getScom(i_target, PU_PSIHB_STATUS_CTL_REG_SCOM, l_psihb_data);

            if (!l_psihb_data.getBit<PU_PSIHB_STATUS_CTL_REG_FSP_INBOUND_ACTIVE>())
            {
                break;
            }

            fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396);
        }

        FAPI_ASSERT(!l_psihb_data.getBit<PU_PSIHB_STATUS_CTL_REG_FSP_INBOUND_ACTIVE>(),
                    fapi2::P9_PSIHBCR_INBOUND_QUEUE_NOT_EMPTY().set_TARGET(i_target).set_DATA(
                        l_psihb_data), "PSIHBCR inbound queue not empty");

        //Disable TCE access by clearing the secure boot register - can't be done later because it's a security hole
        l_psihb_data.flush<0>();
        fapi2::putScom(i_target, PU_TRUST_CONTROL, l_psihb_data);

    fapi_try_exit:
        FAPI_IMP("p9_psihb_check_quiesce: Exiting...");
        return fapi2::current_err;
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_intp_check_quiesce(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_IMP("p9_intp_check_quiesce: Entering...");
        // mark HWP entry

        fapi2::buffer<uint64_t> l_data(0);
        uint8_t l_useXiveHwReset;

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_USE_XIVE_HW_RESET,
                               i_target,
                               l_useXiveHwReset));

        //TODO RTC:177741 HW reset for INT is not working, need to retest this
        //if we ever think the hw is fixed
        if(l_useXiveHwReset)
        {
            // Read INT_CQ_RST_CTL so that we don't override anything
            fapi2::getScom(i_target, PU_INT_CQ_RST_CTL, l_data);

            // Set bit in INT_CQ_RST_CTL to request quiesce
            l_data.setBit<PU_INT_CQ_RST_CTL_QUIESCE_PB>();
            fapi2::putScom(i_target, PU_INT_CQ_RST_CTL, l_data);

            // Poll master and slave quiesced via bits in RST_CTL
            for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
            {
                fapi2::getScom(i_target, PU_INT_CQ_RST_CTL, l_data);

                if (l_data.getBit<PU_INT_CQ_RST_CTL_MASTER_IDLE>() && l_data.getBit<PU_INT_CQ_RST_CTL_SLAVE_IDLE>())
                {
                    break;
                }

                fapi2::delay(C_INTP_DELAY_NS, C_INTP_DELAY_CYCLES);
            }

            FAPI_ASSERT((l_data.getBit<PU_INT_CQ_RST_CTL_MASTER_IDLE>()
                         && l_data.getBit<PU_INT_CQ_RST_CTL_SLAVE_IDLE>()), fapi2::P9_INTP_QUIESCE_TIMEOUT().set_TARGET(i_target).set_DATA(
                            l_data), "INTP master or slave is not IDLE");

            //Set sync_reset in RST_CTL
            l_data.setBit<PU_INT_CQ_RST_CTL_SYNC_RESET>();
            fapi2::putScom(i_target, PU_INT_CQ_RST_CTL, l_data);
        }
        else
        {
            uint64_t l_int_vc_eqc_config_mask_verify_vc_syncs_complete = 0x00000000F8000000;
            const uint64_t l_intp_scrub_masks[4] = {PU_INT_VC_IVC_SCRUB_MASK, PU_INT_VC_SBC_SCRUB_MASK, PU_INT_VC_EQC_SCRUB_MASK, PU_INT_PC_VPC_SCRUB_MASK};
            //Workaround for the sync reset
            //------------------------------------------------------------------
            //Use syncs to make sure no more requests are pending on the queue
            //------------------------------------------------------------------
            //Trigger VC Syncs
            //This is done up in the CAPP unit because we need the fabric

            //Verify VC syncs complete and then reset sync done bits
            fapi2::getScom(i_target, PU_INT_VC_EQC_CONFIG, l_data);
            FAPI_ASSERT((l_data & l_int_vc_eqc_config_mask_verify_vc_syncs_complete) ==
                        l_int_vc_eqc_config_mask_verify_vc_syncs_complete,
                        fapi2::P9_INT_WORKAROUND_ERR().set_TARGET(i_target).set_ADDRESS(PU_INT_VC_EQC_CONFIG).set_DATA(l_data),
                        "Error with VC syncs not being set as expected");
            l_data.clearBit<32>().clearBit<33>().clearBit<34>().clearBit<35>().clearBit<36>();
            fapi2::putScom(i_target, PU_INT_VC_EQC_CONFIG, l_data);

            //---------------------------
            //Scrub all Int caches
            //---------------------------
            //Fill the scrub mask regs to 0
            for (uint32_t i = 0; i < 4; i++)
            {
                fapi2::putScom(i_target, l_intp_scrub_masks[i], 0x0000000000000000);
            }

            //Start the scrub operation in all caches andPoll for completion
            FAPI_TRY(p9_int_scrub_caches(i_target), "Error scrubbing the caches");

            //----------------------------
            //Change all VSDs to invalid
            //----------------------------
            //Change all VC VSDs
            //Do the IVE VC VSD
            fapi2::putScom(i_target, PU_INT_VC_VSD_TABLE_ADDR, 0x8000000000000000);

            for (uint32_t i = 0; i < 16; i++)
            {
                fapi2::putScom(i_target, PU_INT_VC_VSD_TABLE_DATA, 0x0000000000000000);
            }

            //Do the ESB VC VSD
            fapi2::putScom(i_target, PU_INT_VC_VSD_TABLE_ADDR, 0x8001000000000000);

            for (uint32_t i = 0; i < 16; i++)
            {
                fapi2::putScom(i_target, PU_INT_VC_VSD_TABLE_DATA, 0x0000000000000000);
            }

            //Do the EQD VC VSD
            fapi2::putScom(i_target, PU_INT_VC_VSD_TABLE_ADDR, 0x8002000000000000);

            for (uint32_t i = 0; i < 16; i++)
            {
                fapi2::putScom(i_target, PU_INT_VC_VSD_TABLE_DATA, 0x0000000000000000);
            }

            //Do the VPD VC VSD
            fapi2::putScom(i_target, PU_INT_VC_VSD_TABLE_ADDR, 0x8003000000000000);

            for (uint32_t i = 0; i < 32; i++)
            {
                fapi2::putScom(i_target, PU_INT_VC_VSD_TABLE_DATA, 0x0000000000000000);
            }

            //Do the IRQ VC VSD
            fapi2::putScom(i_target, PU_INT_VC_VSD_TABLE_ADDR, 0x8004000000000000);

            for (uint32_t i = 0; i < 6; i++)
            {
                fapi2::putScom(i_target, PU_INT_VC_VSD_TABLE_DATA, 0x0000000000000000);
            }

            //Change all PC VSDs
            //Do the IVE PC VSD
            fapi2::putScom(i_target, PU_INT_PC_VSD_TABLE_ADDR, 0x8000000000000000);

            for (uint32_t i = 0; i < 16; i++)
            {
                fapi2::putScom(i_target, PU_INT_PC_VSD_TABLE_DATA, 0x0000000000000000);
            }

            //Do the ESB PC VSD
            fapi2::putScom(i_target, PU_INT_PC_VSD_TABLE_ADDR, 0x8001000000000000);

            for (uint32_t i = 0; i < 16; i++)
            {
                fapi2::putScom(i_target, PU_INT_PC_VSD_TABLE_DATA, 0x0000000000000000);
            }

            //Do the EQD PC VSD
            fapi2::putScom(i_target, PU_INT_PC_VSD_TABLE_ADDR, 0x8002000000000000);

            for (uint32_t i = 0; i < 16; i++)
            {
                fapi2::putScom(i_target, PU_INT_PC_VSD_TABLE_DATA, 0x0000000000000000);
            }

            //Do the VPD PC VSD
            fapi2::putScom(i_target, PU_INT_PC_VSD_TABLE_ADDR, 0x8002000000000000);

            for (uint32_t i = 0; i < 32; i++)
            {
                fapi2::putScom(i_target, PU_INT_PC_VSD_TABLE_DATA, 0x0000000000000000);
            }

            //----------------------------
            //Re-scrub all Int caches
            //----------------------------
            //Start the scrub operation in all caches and Poll for completion
            FAPI_TRY(p9_int_scrub_caches(i_target), "Error re-scrubbing the caches");
            //----------------------------
            //Disable all thread contexts (this will also trigger an internal reset)
            //----------------------------
            fapi2::putScom(i_target, PU_INT_TCTXT_EN0, 0x0000000000000000);
            fapi2::putScom(i_target, PU_INT_TCTXT_EN1, 0x0000000000000000);

            //----------------------------
            //Reset Quiesce
            //----------------------------
            fapi2::putScom(i_target, PU_INT_CQ_RST_CTL, 0x0000000000000000);
        }

    fapi_try_exit:
        FAPI_IMP("p9_intp_check_quiesce: Exiting...");
        return fapi2::current_err;
    }

    //This is a helper function to scrub all the caches for Int
    fapi2::ReturnCode p9_int_scrub_caches(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        fapi2::buffer<uint64_t> l_scrub_trig_data(0);
        const uint64_t l_scrub_addrs[4] = {PU_INT_VC_IVC_SCRUB_TRIG, PU_INT_VC_SBC_SCRUB_TRIG, PU_INT_VC_EQC_SCRUB_TRIG, PU_INT_PC_VPC_SCRUB_TRIG};

        //Start the scrub operation in all caches
        for (uint32_t i = 0; i < 4; i++)
        {
            fapi2::putScom(i_target, l_scrub_addrs[i], 0xA000000000000000);

            for (uint32_t j = 0; j < C_NUM_TRIES_QUIESCE_STATE; j++)
            {
                fapi2::delay(C_INTP_DELAY_NS, C_INTP_DELAY_CYCLES);
                fapi2::getScom(i_target, l_scrub_addrs[i], l_scrub_trig_data);

                if (!l_scrub_trig_data.getBit<0>())
                {
                    break;
                }
            }

            FAPI_ASSERT(!l_scrub_trig_data.getBit<0>(),
                        fapi2::P9_INT_WORKAROUND_ERR().set_TARGET(i_target).set_ADDRESS(l_scrub_addrs[i]).set_DATA(l_scrub_trig_data),
                        "INT_VC_IVC_SCRUB_TRIG register is not complete");
        }

    fapi_try_exit:
        return fapi2::current_err;
    }

    fapi2::ReturnCode p9_pm_check_quiesce(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        // Bit-13 used to set WAKEUP_NOTIFY_SELECT bit on the Core Power Management Mode Reg
        static const uint64_t  CPPM_WKUP_NOTIFY_SELECT = 0x0004000000000000;
        // Bit 0 in each SPWKUP_PMM register controls if SPWKUP is asserted from the source
        static const uint64_t  CLEAR_SPWKUP            = 0x0000000000000000;
        // Special wakeup sources
        static const uint64_t  SPWKUP_SRC_REGS[4] = {C_PPM_SPWKUP_OTR, C_PPM_SPWKUP_FSP, C_PPM_SPWKUP_HYP, C_PPM_SPWKUP_OCC};

        //TODO: RTC 166888 Remove unneeded consts when p9_pm_suspend gets pulled into MPIPL flow
        // Bits 1:3 are XCR control of the PPE_XIXCR, we want to HALT which = 0x010
        static const uint64_t  PPE_XIXCR_XCR_HALT      = 0x6000000000000000;
        // Bit 0 in each C_CPPMR register controls the DISABLE_WRITE functionality
        static const uint64_t CLEAR_DISABLE_WRITE      = 0x8000000000000000;
        //XIXCR scom regs that control the OCC's GPEs
        static const uint64_t  GPE_XIXCR_REGS[4] = {P9N2_PU_GPE0_GPENXIXCR_SCOM, P9N2_PU_GPE1_GPENXIXCR_SCOM, P9N2_PU_GPE2_GPENXIXCR_SCOM, P9N2_PU_GPE3_GPENXIXCR_SCOM};

        FAPI_IMP("p9_pm_check_quiesce: Entering...");

        //Loop over cores and set the WKUP_NOTIFY_SELECT bit and clear out
        //SPWK request from all srcs
        for(auto& l_childCore :
            i_target.getChildren<fapi2::TARGET_TYPE_CORE>())
        {
            //This scom sets WKUP_NOTIFY_SELECT
            FAPI_TRY(fapi2::putScom(l_childCore, PERV_EC00_CPPM_CPMMR_OR, CPPM_WKUP_NOTIFY_SELECT ));


            //This loop clears spwkup asserts from all the possilble srcs
            for(uint8_t i = 0; i < 4; i++)
            {
                FAPI_TRY(fapi2::putScom(l_childCore, SPWKUP_SRC_REGS[i] , CLEAR_SPWKUP));
            }

            //TODO: RTC 166888 Remove clearing WRITE_DISABLE once p9_powman_suspend is added to MPIPL flow
            //This scom clears the WRITE_DISABLE bit, this step enables SBE writes
            FAPI_TRY(fapi2::putScom(l_childCore, C_CPPM_CPMMR_CLEAR, CLEAR_DISABLE_WRITE ));

        }

        //TODO: RTC 166888 Remove GPE halts once p9_powman_suspend is added to MPIPL flow
        //Need to halt the GPEs so wakeups are handled correctly later on
        //Loop over the differnt GPE XIXCR scom regs and send the halt command
        // to each by writing 0x6 to bits 1:3 of the XIXCR register
        for(uint8_t i = 0; i < 4; i++)
        {
            FAPI_TRY(fapi2::putScom(i_target, GPE_XIXCR_REGS[i], PPE_XIXCR_XCR_HALT));
        }

    fapi_try_exit:
        FAPI_IMP("p9_pm_check_quiece: Exiting...");
        return fapi2::current_err;
    }

} // extern "C"

