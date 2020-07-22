/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/nest/p9_sbe_check_quiesce.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2020                        */
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
// *HWP HWP Owner: Joe McGill <jmcgill@us.ibm.com>
// *HWP FW Owner: Thi Tran thi@us.ibm.com
// *HWP Team: Nest
// *HWP Level: 3
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
#include <p9_suspend_io.H>
#include <p9n2_misc_scom_addresses.H>
#include <p9n2_misc_scom_addresses_fld.H>

//Needed for SW reset of XIVE unit
#include <p9_thread_control.H>
#include <p9_fbc_utils.H>
#include <p9_adu_setup.H>
#include <p9_adu_access.H>
#include <p9_adu_coherent_utils.H>

extern "C" {

//-----------------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------------
    //This is how many times each unit will try to do the quiesce if it has a wait for some cycles
    const uint32_t C_NUM_TRIES_QUIESCE_STATE = 10000;
    //These are the delays for the units that need them
    const uint32_t C_CAPP_DELAY_NS = 168000000 / C_NUM_TRIES_QUIESCE_STATE; //168 ms
    const uint32_t C_CAPP_DELAY_CYCLES = 336000000 / C_NUM_TRIES_QUIESCE_STATE; //2GHz * 168 ms
    const uint32_t C_NPU_DELAY_NS = 150000 / C_NUM_TRIES_QUIESCE_STATE; //150 microseconds
    const uint32_t C_NPU_DELAY_CYCLES = 300000 / C_NUM_TRIES_QUIESCE_STATE; //2GHz * 150 microseconds
    const uint32_t C_DELAY_NS_396      = 396000000 / C_NUM_TRIES_QUIESCE_STATE; //396 ms
    const uint32_t C_DELAY_CYCLES_396 = 792000000 / C_NUM_TRIES_QUIESCE_STATE; //2GHz * 396 ms
    const uint32_t C_INTP_DELAY_NS = 10000 / C_NUM_TRIES_QUIESCE_STATE; //10 microseconds
    const uint32_t C_INTP_DELAY_CYCLES = 20000 / C_NUM_TRIES_QUIESCE_STATE; //2GHz * 10 microseconds

    const uint32_t PHB_HV_IND_ADDR_VALID_BIT = 0;
    const uint32_t PHB_HV_IND_ADDR_START_BIT = 52;
    const uint32_t PHB_HV_IND_ADDR_LEN = 12;

//--------------------------------------------------------------------------
//  HWP entry point
//--------------------------------------------------------------------------
    fapi2::ReturnCode p9_sbe_check_quiesce(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_DBG("p9_sbe_check_quiesce: Entering..");

        // SBE will check quiesce state for all units on the powerbus on its chip
        FAPI_TRY(p9_capp_check_quiesce(i_target), "Error from p9_capp_check_quiesce");
        FAPI_TRY(p9_phb_check_quiesce(i_target), "Error from p9_phb_check_quiesce");
        FAPI_TRY(p9_vas_check_quiesce(i_target), "Error from p9_vas_check_quiesce");
        FAPI_TRY(p9_nx_check_quiesce(i_target), "Error from p9_nx_check_quiesce");
        FAPI_TRY(p9_psihb_check_quiesce(i_target), "Error from p9_psihb_check_quiesce");
        FAPI_TRY(p9_npu_check_quiesce(i_target), "Error from p9_npu_check_quiesce");
        FAPI_TRY(p9_openCapi_check_quiesce(i_target), "Error from p9_openCapi_check_quiesce");
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
                FAPI_INF("ERROR: There was an error doing the checkstop, "
                         "it may not have gone through");
            }
        }

        FAPI_DBG("p9_sbe_check_quiesce: Exiting..");
        return saveError;
    }

//---------------------------------------------------------------------------
//  Helper Functions
//---------------------------------------------------------------------------

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_capp_check_quiesce(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_DBG("p9_capp_check_quiesce: Entering ....");

        fapi2::buffer<uint64_t> l_data(0);

        FAPI_TRY(fapi2::getScom(i_target, CAPP_FLUSHSHUE, l_data));

        if (l_data != 0x0ull)
        {

            // read the value of CAPP Error status and control register so we don't
            // write over something
            FAPI_TRY(fapi2::getScom(i_target, CAPP_CAPP_ERR_STATUS_CONTROL, l_data));

            // Write the Force Quiesce bit
            l_data.setBit<CAPP_CAPP_ERR_STATUS_CONTROL_FORCE_QUIESCE>();
            FAPI_TRY(fapi2::putScom(i_target, CAPP_CAPP_ERR_STATUS_CONTROL, l_data));

            // Poll the Quiesce done bit
            for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
            {
                FAPI_TRY(fapi2::getScom(i_target, CAPP_CAPP_ERR_STATUS_CONTROL, l_data));

                if (!l_data.getBit<CAPP_CAPP_ERR_STATUS_CONTROL_QUIESCE_DONE>())
                {
                    break;
                }

                FAPI_TRY(fapi2::delay(C_CAPP_DELAY_NS, C_CAPP_DELAY_CYCLES));
            }

            FAPI_ASSERT(!l_data.getBit<CAPP_CAPP_ERR_STATUS_CONTROL_QUIESCE_DONE>(),
                        fapi2::P9_CAPP_QUIESCE_TIMEOUT()
                        .set_TARGET(i_target)
                        .set_DATA(l_data),
                        "CAPP quiesce timed out");
        }

    fapi_try_exit:
        FAPI_DBG("p9_capp_check_quiesce: Exiting ....");
        return fapi2::current_err;
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_phb_check_quiesce(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_DBG("p9_phb_check_quiesce: Entering ...");
        fapi2::buffer<uint64_t> l_data(0);

        // If ETU is not already in reset, use indirect register to quiesce DMAs
        // The address of the PHB Quiesce DMA Register is 0x0888 (found in PHB spec)
        auto l_phb_chiplets_vec = i_target.getChildren<fapi2::TARGET_TYPE_PHB>();

        for (auto& l_phb_chiplet : l_phb_chiplets_vec)
        {
            FAPI_TRY(fapi2::getScom(l_phb_chiplet, PHB_PHBRESET_REG, l_data));

            if (!l_data.getBit<PHB_PHBRESET_REG_PE_ETU_RESET>())
            {
                //Clear contents of PHB HV Indirect Address Register
                l_data.flush<0>();
                FAPI_TRY(fapi2::putScom(l_phb_chiplet , PHB_PHB4_SCOM_HVIAR, l_data));
                //Setup the PHB HV registers for the write
                l_data.insertFromRight<PHB_HV_IND_ADDR_START_BIT, PHB_HV_IND_ADDR_LEN>(0x888);
                l_data.setBit<PHB_HV_IND_ADDR_VALID_BIT>();
                FAPI_TRY(fapi2::putScom(l_phb_chiplet, PHB_PHB4_SCOM_HVIAR, l_data));
                //Setup PHB HV Indirect for write access
                l_data.flush<0>().insertFromRight<0, 63>(0x8000000000000000);
                FAPI_TRY(fapi2::putScom(l_phb_chiplet, PHB_PHB4_SCOM_HVIDR, l_data));
                //Clear contents of PHB HV Indirect Address Register
                l_data.flush<0>();
                FAPI_TRY(fapi2::putScom(l_phb_chiplet, PHB_PHB4_SCOM_HVIAR, l_data));
            }
        }

        FAPI_TRY(p9_suspend_io(i_target, true), "ERROR suspending IO");

    fapi_try_exit:
        FAPI_DBG("p9_phb_check_quiesce: Exiting ...");
        return fapi2::current_err;
    }


    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_npu_check_quiesce(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_DBG("p9_npu_check_quiesce: Entering...");
        const uint32_t l_numNTLs = 6;

        fapi2::buffer<uint64_t> l_data(0);
        const uint32_t CQ_fence_status_regs[l_numNTLs] =
        {
            0x00090500, 0x000B0500, 0x00190500,
            0x001B0500, 0x00290500, 0x002B0500
        };
#ifndef DD2
        const uint32_t C_GPU_MEMORY_BARS_SIZE = 12;
        const uint32_t C_MEMORY_BARS_SIZE = 36;
        const uint64_t l_GPU_Memory_BARs[C_GPU_MEMORY_BARS_SIZE] =
        {
            PU_NPU0_SM0_GPU_BAR, PU_NPU0_SM1_GPU_BAR, PU_NPU0_SM2_GPU_BAR, PU_NPU0_SM3_GPU_BAR,
            PU_NPU1_SM0_GPU_BAR, PU_NPU1_SM1_GPU_BAR, PU_NPU1_SM2_GPU_BAR, PU_NPU1_SM3_GPU_BAR,
            PU_NPU2_SM0_GPU_BAR, PU_NPU2_SM1_GPU_BAR, PU_NPU2_SM2_GPU_BAR, PU_NPU2_SM3_GPU_BAR
        };
        const uint64_t l_memory_bars[C_MEMORY_BARS_SIZE] =
        {
            PU_NPU0_SM0_NDT0_BAR, PU_NPU0_SM1_NDT0_BAR, PU_NPU0_SM2_NDT0_BAR, PU_NPU0_SM3_NDT0_BAR,
            PU_NPU1_SM0_NDT0_BAR, PU_NPU1_SM1_NDT0_BAR, PU_NPU1_SM2_NDT0_BAR, PU_NPU1_SM3_NDT0_BAR,
            PU_NPU2_SM0_NDT0_BAR, PU_NPU2_SM1_NDT0_BAR, PU_NPU2_SM2_NDT0_BAR, PU_NPU2_SM3_NDT0_BAR,
            PU_NPU0_SM0_NDT1_BAR, PU_NPU0_SM1_NDT1_BAR, PU_NPU0_SM2_NDT1_BAR, PU_NPU0_SM3_NDT1_BAR,
            PU_NPU1_SM0_NDT1_BAR, PU_NPU1_SM1_NDT1_BAR, PU_NPU1_SM2_NDT1_BAR, PU_NPU1_SM3_NDT1_BAR,
            PU_NPU2_SM0_NDT1_BAR, PU_NPU2_SM1_NDT1_BAR, PU_NPU2_SM2_NDT1_BAR, PU_NPU2_SM3_NDT1_BAR,
            PU_NPU0_SM0_PHY_BAR, PU_NPU0_SM1_PHY_BAR, PU_NPU0_SM2_PHY_BAR, PU_NPU0_SM3_PHY_BAR,
            PU_NPU1_SM0_PHY_BAR, PU_NPU1_SM1_PHY_BAR, PU_NPU1_SM2_PHY_BAR, PU_NPU1_SM3_PHY_BAR,
            PU_NPU2_SM0_PHY_BAR, PU_NPU2_SM1_PHY_BAR, PU_NPU2_SM2_PHY_BAR, PU_NPU2_SM3_PHY_BAR
        };
        const uint64_t l_NTL_config1_addrs[l_numNTLs] =
        {
            NV_0_CONFIG1, NV_1_CONFIG1, NV_2_CONFIG1, NV_3_CONFIG1,
            PU_NPU2_NTL0_CONFIG1, PU_NPU2_NTL1_CONFIG1
        };
        const uint64_t l_NTL_config2_addrs[l_numNTLs] =
        {
            NV_0_CONFIG2, NV_1_CONFIG2, NV_2_CONFIG2, NV_3_CONFIG2,
            PU_NPU2_NTL0_CONFIG2, PU_NPU2_NTL1_CONFIG2
        };

        const uint64_t PU_NPU_CTL_DA_ADDR_VAL = PU_NPU_CTL_DA_ADDR;
        const uint64_t PU_NPU_CTL_DA_DATA_VAL = PU_NPU_CTL_DA_DATA;
        const uint64_t PU_NPU_CTL_FENCE_STATE_VAL = PU_NPU_CTL_FENCE_STATE;

#else
        const uint32_t C_GPU_MEMORY_BARS_SIZE = 24;
        const uint32_t C_MEMORY_BARS_SIZE = 48;

        const uint32_t l_GPU_Memory_BARs[C_GPU_MEMORY_BARS_SIZE] =
        {
            P9N2_PU_NPU0_SM0_GPU0_BAR,    P9N2_PU_NPU0_SM1_GPU0_BAR,    P9N2_PU_NPU0_SM3_GPU0_BAR,    P9N2_PU_NPU0_CTL_GPU0_BAR,
            P9N2_PU_NPU2_SM0_GPU0_BAR,    P9N2_PU_NPU2_SM1_GPU0_BAR,    P9N2_PU_NPU2_SM3_GPU0_BAR,    P9N2_PU_NPU2_CTL_GPU0_BAR,
            P9N2_PU_NPU_MSC_SM0_GPU0_BAR, P9N2_PU_NPU_MSC_SM1_GPU0_BAR, P9N2_PU_NPU_MSC_SM3_GPU0_BAR, P9N2_PU_NPU_MSC_CTL_GPU0_BAR,
            P9N2_PU_NPU0_SM0_GPU1_BAR,    P9N2_PU_NPU0_SM1_GPU1_BAR,    P9N2_PU_NPU0_SM3_GPU1_BAR,    P9N2_PU_NPU0_CTL_GPU1_BAR,
            P9N2_PU_NPU2_SM0_GPU1_BAR,    P9N2_PU_NPU2_SM1_GPU1_BAR,    P9N2_PU_NPU2_SM3_GPU1_BAR,    P9N2_PU_NPU2_CTL_GPU1_BAR,
            P9N2_PU_NPU_MSC_SM0_GPU1_BAR, P9N2_PU_NPU_MSC_SM1_GPU1_BAR, P9N2_PU_NPU_MSC_SM3_GPU1_BAR, P9N2_PU_NPU_MSC_CTL_GPU1_BAR
        };

        const uint32_t l_memory_bars[C_MEMORY_BARS_SIZE] =
        {
            P9N2_PU_NPU0_SM0_NDT0_BAR,     P9N2_PU_NPU0_SM1_NDT0_BAR,     P9N2_PU_NPU0_SM3_NDT0_BAR,     P9N2_PU_NPU0_CTL_NDT0_BAR,
            P9N2_PU_NPU0_SM0_NDT1_BAR,     P9N2_PU_NPU0_SM1_NDT1_BAR,     P9N2_PU_NPU0_SM3_NDT1_BAR,     P9N2_PU_NPU0_CTL_NDT1_BAR,
            P9N2_PU_NPU2_SM0_NDT0_BAR,     P9N2_PU_NPU2_SM1_NDT0_BAR,     P9N2_PU_NPU2_SM3_NDT0_BAR,     P9N2_PU_NPU2_CTL_NDT0_BAR,
            P9N2_PU_NPU2_SM0_NDT1_BAR,     P9N2_PU_NPU2_SM1_NDT1_BAR,     P9N2_PU_NPU2_SM3_NDT1_BAR,     P9N2_PU_NPU2_CTL_NDT1_BAR,
            P9N2_PU_NPU_MSC_SM0_NDT0_BAR,  P9N2_PU_NPU_MSC_SM1_NDT0_BAR,  P9N2_PU_NPU_MSC_SM3_NDT0_BAR,  P9N2_PU_NPU_MSC_CTL_NDT0_BAR,
            P9N2_PU_NPU_MSC_SM0_NDT1_BAR,  P9N2_PU_NPU_MSC_SM1_NDT1_BAR,  P9N2_PU_NPU_MSC_SM3_NDT1_BAR,  P9N2_PU_NPU_MSC_CTL_NDT1_BAR,
            P9N2_PU_NPU_MSC_SM0_PHY_BAR,   P9N2_PU_NPU_MSC_SM1_PHY_BAR,   P9N2_PU_NPU_MSC_SM3_PHY_BAR,   P9N2_PU_NPU_MSC_CTL_PHY_BAR,
            P9N2_PU_NPU2_SM0_PHY_BAR,      P9N2_PU_NPU2_SM1_PHY_BAR,      P9N2_PU_NPU2_SM3_PHY_BAR,      P9N2_PU_NPU2_CTL_PHY_BAR,
            P9N2_PU_NPU0_SM0_GENID_BAR,    P9N2_PU_NPU0_SM1_GENID_BAR,    P9N2_PU_NPU0_SM3_GENID_BAR,    P9N2_PU_NPU0_CTL_GENID_BAR,
            P9N2_PU_NPU2_SM0_GENID_BAR,    P9N2_PU_NPU2_SM1_GENID_BAR,    P9N2_PU_NPU2_SM3_GENID_BAR,    P9N2_PU_NPU2_CTL_GENID_BAR,
            P9N2_PU_NPU_MSC_SM0_GENID_BAR, P9N2_PU_NPU_MSC_SM1_GENID_BAR, P9N2_PU_NPU_MSC_SM3_GENID_BAR, P9N2_PU_NPU_MSC_CTL_GENID_BAR,
            P9N2_PU_NPU0_SM0_PHY_BAR, P9N2_PU_NPU0_SM1_PHY_BAR, P9N2_PU_NPU_SM3_C_ERR_RPT_HOLD1, P9N2_PU_NPU_CTL_C_ERR_RPT_HOLD1
        };

        const uint32_t l_NTL_config1_addrs[l_numNTLs] =
        {
            P9N2_PU_NPU1_SM1_CONFIG1, P9N2_PU_NPU1_SM2_CONFIG1,
            P9N2_PU_NPU_SM1_CONFIG1,  P9N2_PU_NPU_SM2_CONFIG1,
            P9N2__SM1_CONFIG1,        P9N2__SM2_CONFIG1
        };

        const uint32_t l_NTL_config2_addrs[l_numNTLs] =
        {
            P9N2_PU_NPU1_SM0_CONFIG2, P9N2_PU_NPU1_SM1_CONFIG2,
            P9N2_PU_NPU_SM0_CONFIG2, P9N2_PU_NPU_SM1_CONFIG2,
            P9N2__SM0_CONFIG2, P9N2__SM1_CONFIG2
        };

        const uint64_t PU_NPU_CTL_DA_ADDR_VAL = P9N2__CTL_DA_ADDR;
        const uint64_t PU_NPU_CTL_DA_DATA_VAL = P9N2__CTL_DA_DATA;
        const uint64_t PU_NPU_CTL_FENCE_STATE_VAL = P9N2__CTL_FENCE_STATE;
#endif


        // 1) Place all six of the NTLs into Reset State
        // Set bits 8:9 in the NTL Misc Config 1 registers to place NTLs in Reset state
        for (uint32_t i = 0; i < l_numNTLs; i++)
        {
            //First check if the NTL was initialized before attempting to reset
            FAPI_TRY(fapi2::getScom(i_target, l_NTL_config2_addrs[i], l_data));

            if(l_data.getBit<0>())
            {
                FAPI_TRY(fapi2::getScom(i_target, l_NTL_config1_addrs[i], l_data));
                l_data.insertFromRight<NV_CONFIG1_NTL_RESET, NV_CONFIG1_NTL_RESET_LEN>(0x3);
                FAPI_TRY(fapi2::putScom(i_target, l_NTL_config1_addrs[i], l_data));
            }
        }

        //Poll the CQ Fence Status Registers util "Value" is detected to verify
        //that NTLs are in Reset State
        for (uint32_t i = 0; i < l_numNTLs; i++)
        {
            //First check if NTL was initialized before polling for reset
            FAPI_TRY(fapi2::getScom(i_target, l_NTL_config2_addrs[i], l_data));

            if(l_data.getBit<0>())
            {
                for (uint32_t j = 0; j < CAPP_CAPP_ERR_STATUS_CONTROL_QUIESCE_DONE; j++)
                {
                    //Set address of CQ fence status reg in the indirect address "sddr" reg
                    //also set bits 24 and 25 to indicate in the request we want 8 bytes  back
                    l_data.flush<0>()
                    .insertFromRight<PU_NPU_CTL_DA_ADDR_MISC,
                                     PU_NPU_CTL_DA_ADDR_MISC_LEN>
                                     (CQ_fence_status_regs[i]).setBit<24>().setBit<25>();
                    FAPI_TRY(fapi2::putScom(i_target, PU_NPU_CTL_DA_ADDR_VAL, l_data));
                    FAPI_TRY(fapi2::getScom(i_target, PU_NPU_CTL_DA_DATA_VAL, l_data));

                    //If bit 0 and 1 are set on the CQ fence status that indicates
                    //the reset was successful
                    if (l_data.getBit<0>() && l_data.getBit<1>())
                    {
                        break;
                    }

                    FAPI_TRY(fapi2::delay(C_NPU_DELAY_NS, C_NPU_DELAY_CYCLES));
                }

                FAPI_ASSERT((l_data.getBit<0>() && l_data.getBit<1>()),
                            fapi2::P9_NTL_NOT_IN_RESET()
                            .set_TARGET(i_target)
                            .set_NTL_ADDR(CQ_fence_status_regs[i])
                            .set_NTL_DATA(l_data),
                            "One of the NTLS are not in the reset state");
            }
        }

        // 2) Place all NV-Link Bricks into Fence State
        // Set bits 0:5 in the NPU Fence State register to place all bricks into Fence State
        l_data.setBit<PU_NPU_CTL_FENCE_STATE_BRK0>()
        .setBit<PU_NPU_CTL_FENCE_STATE_BRK1>()
        .setBit<PU_NPU_CTL_FENCE_STATE_BRK2>()
        .setBit<PU_NPU_CTL_FENCE_STATE_BRK3>()
        .setBit<PU_NPU_CTL_FENCE_STATE_BRK4>()
        .setBit<PU_NPU_CTL_FENCE_STATE_BRK5>();
        FAPI_TRY(fapi2::putScom(i_target, PU_NPU_CTL_FENCE_STATE_VAL, l_data));

#ifdef DD2
        l_data.flush<0>();
        //Write bits 0:22 of the NPU Interrupt Request register to eliminate
        //any interrupt requests that occurred between steps 1 and 2.
        FAPI_TRY(fapi2::putScom(i_target, P9N2__CTL_INT_REQ, l_data));
#endif

        // 3) Disable all NPU BAR registers
        // Reset bits 0 and 32 in the GPU-Memory BARs to stop NPU from responding
        // to accesses to GPU memory

        for (uint32_t i = 0; i < C_GPU_MEMORY_BARS_SIZE; i++)
        {
            l_data.flush<0>().clearBit<PU_NPU0_SM0_GPU_BAR_CONFIG_GPU0_ENABLE>().clearBit<PU_NPU0_SM0_GPU_BAR_CONFIG_GPU1_ENABLE>();
            FAPI_TRY(fapi2::putScom(i_target, l_GPU_Memory_BARs[i], l_data));
        }

        // Reset bit 0 in the NTL0/NDL0 Memory BARs and NTL1/NDL1 Memory BARs to
        //   stop NPU from responding to accesses to NTL/NDL registers
        // Reset bit 0 in PHY0/PHY1/NPU MMIO BAR for stack 0 and stack 1 to stop
        //   NPU from responding to PHY register accesses
        // Reset bit 0 in PHY0/PHY1/NPU MMIO BARs in stack 2 to stop
        //   NPU from responding to NPU MMIO register accesses
        for (uint32_t i = 0; i < C_MEMORY_BARS_SIZE; i++)
        {
            l_data.flush<0>().clearBit<PU_NPU0_SM0_NDT0_BAR_CONFIG_ENABLE>();
            FAPI_TRY(fapi2::putScom(i_target, l_memory_bars[i], l_data));

        }

    fapi_try_exit:
        FAPI_DBG("p9_npu_check_quiesce: Exiting...");
        return fapi2::current_err;
    }


    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_openCapi_check_quiesce(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_DBG("p9_openCapi_check_quiesce: Entering...");

        //P9 OCAPI procedssing unit (NPU), has  three stacks, out of which two are used for
        //OCAPI communication, each of these stacks have two OTLs.
        const uint32_t C_NUMOCAPISTACKS = 2;
        const uint32_t C_NUMOTLS = 2;

        //Bit position of ODL config register for resetting ODL
        const uint32_t C_ODL_RESET_BIT = 0;

        //Fence control registers to put OTLs in reset state
        const uint64_t c_Fence_cntl_regs[C_NUMOCAPISTACKS][C_NUMOTLS] =
        {
            {P9N2_PU_NPU2_NTL1_FENCE_CONTROL0, P9N2_PU_NPU2_NTL1_FENCE_CONTROL1},
            {P9N2_NV_5_FENCE_CONTROL0, P9N2_NV_5_FENCE_CONTROL1}
        };

        //Config registers for resetting ODL
        const uint64_t c_powerbus_odl_config_regs[C_NUMOCAPISTACKS][C_NUMOTLS] =
        {
            {0x901082A, 0x901082B},
            {0xC01082B, 0xC01082A}
        };

        //Registers to check whether OTL is enabled or not
        const uint64_t c_otl_config_regs[C_NUMOCAPISTACKS][C_NUMOTLS] =
        {
            {P9N2_PU_NPU_SM2_CONFIG0, P9N2_PU_NPU_CTL_CONFIG0},
            {P9N2__SM2_CONFIG0, P9N2__CTL_CONFIG0}
        };

        //Register to check the reset state of OTL
        const uint64_t c_cq_cntl_status_regs[C_NUMOCAPISTACKS] =
        {
            P9N2_PU_NPU2_NTL0_CTL_STATUS, P9N2_NV_4_CTL_STATUS
        };

        const uint32_t C_GPU_MEMORY_BARS_SIZE = 16;
        const uint32_t C_MMIO_BARS_SIZE = 28;

        //Register to disable NPU BARS
        const uint64_t c_GPU_MemoryBARs[C_GPU_MEMORY_BARS_SIZE] =
        {
            P9N2_PU_NPU2_SM0_GPU0_BAR, P9N2_PU_NPU2_SM1_GPU0_BAR, P9N2_PU_NPU2_SM3_GPU0_BAR, P9N2_PU_NPU2_CTL_GPU0_BAR,
            P9N2_PU_NPU2_SM0_GPU1_BAR, P9N2_PU_NPU2_SM1_GPU1_BAR, P9N2_PU_NPU2_SM3_GPU1_BAR, P9N2_PU_NPU2_CTL_GPU1_BAR,
            P9N2_PU_NPU_MSC_SM0_GPU0_BAR, P9N2_PU_NPU_MSC_SM1_GPU0_BAR, P9N2_PU_NPU_MSC_SM3_GPU0_BAR, P9N2_PU_NPU_MSC_CTL_GPU0_BAR,
            P9N2_PU_NPU_MSC_SM0_GPU1_BAR, P9N2_PU_NPU_MSC_SM1_GPU1_BAR, P9N2_PU_NPU_MSC_SM3_GPU1_BAR, P9N2_PU_NPU_MSC_CTL_GPU1_BAR
        };

        //MMIO BAR registers
        const uint64_t c_OTL_MMIO_BARs[C_MMIO_BARS_SIZE] =
        {
            P9N2_PU_NPU2_SM0_NDT0_BAR, P9N2_PU_NPU2_SM1_NDT0_BAR, P9N2_PU_NPU2_SM3_NDT0_BAR, P9N2_PU_NPU2_CTL_NDT0_BAR,
            P9N2_PU_NPU2_SM0_NDT1_BAR, P9N2_PU_NPU2_SM1_NDT1_BAR, P9N2_PU_NPU2_SM3_NDT1_BAR, P9N2_PU_NPU2_CTL_NDT1_BAR,
            P9N2_PU_NPU_MSC_SM0_NDT0_BAR, P9N2_PU_NPU_MSC_SM1_NDT0_BAR, P9N2_PU_NPU_MSC_SM3_NDT0_BAR, P9N2_PU_NPU_MSC_CTL_NDT0_BAR,
            P9N2_PU_NPU_MSC_SM0_NDT1_BAR, P9N2_PU_NPU_MSC_SM1_NDT1_BAR, P9N2_PU_NPU_MSC_SM3_NDT1_BAR, P9N2_PU_NPU_MSC_CTL_NDT1_BAR,

            P9N2_PU_NPU2_SM0_GENID_BAR, P9N2_PU_NPU2_SM1_GENID_BAR, P9N2_PU_NPU2_SM3_GENID_BAR, P9N2_PU_NPU2_CTL_GENID_BAR,
            P9N2_PU_NPU_MSC_SM0_GENID_BAR, P9N2_PU_NPU_MSC_SM1_GENID_BAR, P9N2_PU_NPU_MSC_SM3_GENID_BAR, P9N2_PU_NPU_MSC_CTL_GENID_BAR,

            P9N2_PU_NPU0_SM0_PHY_BAR, P9N2_PU_NPU0_SM1_PHY_BAR, P9N2_PU_NPU0_SM3_PHY_BAR, P9N2_PU_NPU0_CTL_PHY_BAR
        };


        const uint32_t  c_otl_reset_cmd = 0x3ull;  //Value to be written for OTL reset
        const uint32_t  c_otl_reset_status = 0x3ull; //Value indicating successful OTL reset
        const uint32_t  c_disable_gpu_bar = 0x0ull;

        fapi2::buffer<uint64_t> l_data(0);
        fapi2::buffer<uint64_t> l_brickData(0);

        //Step 1: Set all the OTLs to reset state, set bit 0 t0 1 to 0b11, in the status fence control register 0 and
        //fence control register 1 for the OTL
        for ( uint32_t l_stackIndex = 0; l_stackIndex < C_NUMOCAPISTACKS ; ++l_stackIndex )
        {

            for ( uint32_t l_OTLIndex = 0; l_OTLIndex < C_NUMOTLS ; ++l_OTLIndex )
            {

                FAPI_DBG(" going to read register c_otl_config_regs[%d][%d]: 0x%08X",
                         l_stackIndex, l_OTLIndex, c_otl_config_regs[l_stackIndex][l_OTLIndex]);

                FAPI_TRY(fapi2::getScom(i_target, c_otl_config_regs[l_stackIndex][l_OTLIndex], l_data));

                //Make sure that OTL is enabled
                if( l_data.getBit(0) )
                {
                    //Step 1.1: Place the OTL in reset state
                    FAPI_TRY(fapi2::getScom(i_target, c_Fence_cntl_regs[l_stackIndex][l_OTLIndex], l_data));
                    l_data.insertFromRight<P9N2_PU_NPU2_NTL1_FENCE_CONTROL0_FENCE0_REQUEST, P9N2_PU_NPU2_NTL1_FENCE_CONTROL0_FENCE0_REQUEST_LEN>
                    (c_otl_reset_cmd);
                    FAPI_TRY(fapi2::putScom(i_target, c_Fence_cntl_regs[l_stackIndex][l_OTLIndex], l_data));

                    //Step 1.2: Check fo the reset of OTL
                    //Poll the CQ_CTL status register until the value detected indicates that
                    //OTLs are in Reset State
                    for ( uint32_t l_tryIndex = 0; l_tryIndex < C_NUM_TRIES_QUIESCE_STATE ; ++l_tryIndex)
                    {
                        FAPI_TRY(fapi2::getScom(i_target, c_cq_cntl_status_regs[l_stackIndex], l_data));

                        if ( 0 == l_OTLIndex )
                        {
                            //For OTL 0 bits 48 and 49 needs to be validated
                            l_data.extractToRight<P9N2_PU_NPU2_NTL0_CTL_STATUS_BRK0_AM_FENCED, P9N2_PU_NPU2_NTL0_CTL_STATUS_BRK0_AM_FENCED_LEN>
                            (l_brickData);
                        }
                        else
                        {
                            //For
                            //OTL 1 bits 50 and 51 needs to be validated
                            l_data.extractToRight<P9N2_PU_NPU2_NTL0_CTL_STATUS_BRK1_AM_FENCED, P9N2_PU_NPU2_NTL0_CTL_STATUS_BRK1_AM_FENCED_LEN>
                            (l_brickData);
                        }

                        if (c_otl_reset_status == l_brickData )
                        {
                            FAPI_DBG("PAU Number:%d, OTL  Index:%d,  Brick fenced_data:0x%08x", l_stackIndex , l_OTLIndex, l_brickData);
                            break;
                        }
                    }

                    FAPI_ASSERT((l_brickData  == c_otl_reset_status),
                                fapi2::P9_OTL_NOT_IN_RESET()
                                .set_PROC_TARGET(i_target)
                                .set_STATUS_ADDR(c_cq_cntl_status_regs[l_stackIndex])
                                .set_STATUS_DATA(l_brickData),
                                " OTL did  not enter the reset state");

                    FAPI_TRY(fapi2::getScom(i_target, c_powerbus_odl_config_regs[l_stackIndex][l_OTLIndex], l_data));
                    l_data.setBit<C_ODL_RESET_BIT>();
                    FAPI_TRY(fapi2::putScom(i_target, c_powerbus_odl_config_regs[l_stackIndex][l_OTLIndex], l_data));
                }
            }//for OTL Index
        }//for stackIndex

        //Step 2: Put all the bricks in fence state
        //Put all the bricks in fence state
        //brick 0 and brick 1 are not used for openCAPI link
        l_data.flush<0>();
        l_data.setBit<P9N2__CTL_FENCE_STATE_BRK2>().
        setBit<P9N2__CTL_FENCE_STATE_BRK3>().
        setBit<P9N2__CTL_FENCE_STATE_BRK4>().
        setBit<P9N2__CTL_FENCE_STATE_BRK5>();

        FAPI_TRY(fapi2::putScom(i_target, P9N2__CTL_FENCE_STATE, l_data));

        //Clear interrupts
        l_data.flush<0>();
        FAPI_TRY(fapi2::putScom(i_target, P9N2__CTL_INT_REQ, l_data));


        //Step 3:
        //Diable all NPU BARs
        //Reset bits 0:2 in GPU0-Memory BAR and GPU1-Memory BAR to stop NPU from responding to accesses to AFU memory.
        for ( uint32_t i = 0; i < C_GPU_MEMORY_BARS_SIZE; i++)
        {
            FAPI_TRY(fapi2::getScom(i_target, c_GPU_MemoryBARs[i], l_data));
            l_data.insertFromRight<P9N2_PU_NPU2_SM0_GPU0_BAR_CONFIG_MEMSELMATCH, P9N2_PU_NPU2_SM0_GPU0_BAR_CONFIG_MEMSELMATCH_LEN>
            (c_disable_gpu_bar);
            FAPI_TRY(fapi2::putScom(i_target, c_GPU_MemoryBARs[i], l_data));
        }

        //Reset bit 0 in NTL0/NDL0 MMIO BAR and NTL1/NDL1 MMIO BAR to stop NPU from responding to accesses to AFU MMIO registers.
        //Reset bit 0 in Generation-ID Registers MMIO BAR to stop NPU from responding to AFU Config accesses.
        //Reset bit 0 in PHY0/PHY1/NPU MMIO BAR in stack-0 to stop NPU from responding to NPU MMIO register accesses.
        for ( uint32_t i = 0; i < C_MMIO_BARS_SIZE; i++)
        {
            FAPI_TRY(fapi2::getScom(i_target, c_OTL_MMIO_BARs[i], l_data));
            l_data.clearBit<0>();
            FAPI_TRY(fapi2::putScom(i_target, c_OTL_MMIO_BARs[i], l_data));
        }

    fapi_try_exit:
        FAPI_DBG("p9_openCapi_check_quiesce: Exiting...");
        return fapi2::current_err;

    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_vas_check_quiesce(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_DBG("p9_vas_check_quiesce: Entering...");
        fapi2::buffer<uint64_t> l_vas_north_misc_ctl_data(0);
        fapi2::buffer<uint64_t> l_vas_south_misc_ctl_data(0);

        // VAS needs to be quiesced before NX

        // Read the VAS Misc status and North control register so we don't write over anything
        FAPI_TRY(fapi2::getScom(i_target, PU_VAS_MISCCTL, l_vas_north_misc_ctl_data));

        // Set the 'Quiesce Requested' bit in the VAS Miscellaneous Status and
        // North Control Register to a 1. This will prevent VAS from
        // accepting new paste or write monitor operations
        l_vas_north_misc_ctl_data.setBit<PU_VAS_MISCCTL_MISC_CTL_QUIESCE_REQUEST>();
        FAPI_TRY(fapi2::putScom(i_target, PU_VAS_MISCCTL, l_vas_north_misc_ctl_data));

        // Check that VAS has quiesced. This is accomplished by reading two
        // status registers. The "RG is Idle' bit in the VAS Miscellaneous
        // Status and North Control Register as well as the 'EG is Idle',
        // 'CQ is Idle' and 'WC is Idle' bit sin the VAS Miscellaneous Status
        // and South Control Register must all be set to one to indicate that
        // VAS has gone idle.
        for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
        {
            // Read VAS Misc status and North control register to ensure
            // 'RG is idle' and 'EG is idle' bits are both set -
            FAPI_TRY(fapi2::getScom(i_target, PU_VAS_MISCCTL,
                                    l_vas_north_misc_ctl_data));

            // Read VAS Misc status and South control register to ensure
            // 'WC is idle' and 'CQ is idle' bits are both set -
            FAPI_TRY(fapi2::getScom(i_target, PU_VAS_SOUTHCTL,
                                    l_vas_south_misc_ctl_data));

            if (l_vas_north_misc_ctl_data.getBit<PU_VAS_MISCCTL_MISC_CTL_RG_IS_IDLE>()
                && l_vas_south_misc_ctl_data.getBit<PU_VAS_SOUTHCTL_SOUTH_CTL_WC_IDLE_BIT>()
                && l_vas_south_misc_ctl_data.getBit<PU_VAS_SOUTHCTL_SOUTH_CTL_CQ_IDLE_BIT>()
                && l_vas_south_misc_ctl_data.getBit<PU_VAS_SOUTHCTL_SOUTH_CTL_EG_IDLE_BIT>())
            {
                break;
            }

            FAPI_TRY(fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396));
        }

        // In order to prevent additional FIFO entries from getting posted
        // to the NX receive FIFOs while trying to quiesce NX, software may
        // wish to close all windows to prevent users from continuing to try
        // to access the accelerators. Software may close all windows by
        // writing the Open/Enable bit to a zero in the Window Control
        // Register<n>. This step is optional, but should be done as part of
        // an orderly shut down of a user's access.

        // Software may also choose to deallocate any pages that partitions
        // (or users) have mapped to VAS' MMIO space. In a general use case,
        // a partition (or user) will have pages that map to VAS' MMIO space
        // to allow the partition to return credits via the Local Receive
        // Window Credit Adder Register <m>. In order to stop MMIO traffic,
        // these pages should be unmapped. In a NX only usage model, this step
        // can be ignored as long as the Quiesce NX procedures are followed.

        FAPI_ASSERT((l_vas_north_misc_ctl_data.getBit<PU_VAS_MISCCTL_MISC_CTL_RG_IS_IDLE>()
                     && l_vas_south_misc_ctl_data.getBit<PU_VAS_SOUTHCTL_SOUTH_CTL_WC_IDLE_BIT>()
                     && l_vas_south_misc_ctl_data.getBit<PU_VAS_SOUTHCTL_SOUTH_CTL_CQ_IDLE_BIT>()
                     && l_vas_south_misc_ctl_data.getBit<PU_VAS_SOUTHCTL_SOUTH_CTL_EG_IDLE_BIT>()),
                    fapi2::P9_VAS_QUIESCE_TIMEOUT()
                    .set_TARGET(i_target)
                    .set_NORTHDATA(l_vas_north_misc_ctl_data)
                    .set_SOUTHDATA(l_vas_south_misc_ctl_data),
                    "VAS quiesce timed out");

        // Write Invalidate CAM location field of North register (optional)
        l_vas_north_misc_ctl_data.setBit<PU_VAS_MISCCTL_MISC_CTL_INVALIDATE_CAM_ALL>();
        FAPI_TRY(fapi2::putScom(i_target, PU_VAS_MISCCTL, l_vas_north_misc_ctl_data));

    fapi_try_exit:
        FAPI_DBG("p9_vas_check_quiesce: Exiting...");
        return fapi2::current_err;
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_nx_check_quiesce(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_DBG("p9_nx_check_quiesce: Entering....");
        fapi2::buffer<uint64_t> l_dma_status_reg_data(0);
        fapi2::buffer<uint64_t> l_data(0);

        //If (DMA Status Register[HMI Ative])
        FAPI_TRY(fapi2::getScom(i_target, PU_SU_STATUS, l_dma_status_reg_data));

        // If HMI has een signaled, NX PBI is frozen and read machines may be
        // stuck active. The quiesce may either be abandoned or write machines
        // idle status can be polled in lieu of the quiesce procedure.
        // The latter is selected here.
        if (l_dma_status_reg_data.getBit<PU_SU_STATUS_HMI_ACTIVE>())
        {
            //then while (!PowerBus Interface Error Report 0 Register[PBI Write Idle])
            for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
            {
                FAPI_TRY(fapi2::getScom(i_target, PU_NX_PB_ERR_RPT_0, l_data));

                if (l_data.getBit<PU_NX_PB_ERR_RPT_0_PBI_WRITE_IDLE>())
                {
                    break;
                }

                FAPI_TRY(fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396));
            }

            FAPI_ASSERT((l_data.getBit<PU_NX_PB_ERR_RPT_0_PBI_WRITE_IDLE>()),
                        fapi2::P9_NX_PBI_WRITE_IDLE_TIMEOUT()
                        .set_TARGET(i_target)
                        .set_DATA(l_data), "PBI Write Idle never happened");
        }

        if(!l_dma_status_reg_data.getBit<PU_SU_STATUS_HMI_ACTIVE>() ||
           l_data.getBit<PU_NX_PB_ERR_RPT_0_PBI_WRITE_IDLE>())
        {
            //Any CRB kill must be complete before issuing the following sequence
            FAPI_TRY(fapi2::getScom(i_target, PU_SU_CRB_KILL_REQ, l_data));

            //if (CRB Kill Request Configuration Register[Kill Enable])
            if (l_data.getBit<PU_SU_CRB_KILL_REQ_ENABLE>())
            {
                //while(!CRB Kill Request Configuration Register[Kill Done]){}
                for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
                {
                    FAPI_TRY(fapi2::getScom(i_target, PU_SU_CRB_KILL_REQ, l_data));

                    if (l_data.getBit<PU_SU_CRB_KILL_REQ_DONE>())
                    {
                        break;
                    }

                    FAPI_TRY(fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396));
                }

                FAPI_ASSERT(l_data.getBit<PU_SU_CRB_KILL_REQ_DONE>(),
                            fapi2::P9_NX_CRB_KILL_DONE_TIMEOUT()
                            .set_TARGET(i_target)
                            .set_DATA(l_data), "CRB kills were not complete");
            }

            //Stop UMAC from fetching new CRBs
            FAPI_TRY(fapi2::getScom(i_target, PU_UMAC_STATUS_CONTROL, l_data));
            //clear_reg(UMAC Status and Control Register[CRB Read Enable])
            l_data.clearBit<PU_UMAC_STATUS_CONTROL_CRB_READS_ENBL>();
            FAPI_TRY(fapi2::putScom(i_target, PU_UMAC_STATUS_CONTROL, l_data));

            //while(!UMAC Status and Control Register[CRB Read Halted]){}
            for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
            {
                FAPI_TRY(fapi2::getScom(i_target, PU_UMAC_STATUS_CONTROL, l_data));

                if (l_data.getBit<PU_UMAC_STATUS_CONTROL_CRB_READS_HALTED>())
                {
                    break;
                }

                FAPI_TRY(fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396));
            }

            FAPI_ASSERT(l_data.getBit<PU_UMAC_STATUS_CONTROL_CRB_READS_HALTED>(),
                        fapi2::P9_NX_STOP_UMAC_FETCHING_NEW_CRBS_TIMEOUT()
                        .set_TARGET(i_target)
                        .set_DATA(l_data),
                        "UMAC was not stopped from fetching new CRBs");

            //Wait for UMAC dispatch slots to drain of CRBs
            //UMAC can still have CRBs queued in receive FIFOs
            //while(!UMAC Status and Control Register[UMAC Quiesced]){}
            for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
            {
                FAPI_TRY(fapi2::getScom(i_target, PU_UMAC_STATUS_CONTROL, l_data));

                if (l_data.getBit<PU_UMAC_STATUS_CONTROL_QUIESCED>())
                {
                    break;
                }

                FAPI_TRY(fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396));
            }


            FAPI_ASSERT(l_data.getBit<PU_UMAC_STATUS_CONTROL_QUIESCED>(),
                        fapi2::P9_NX_UMAC_DISPATCH_SLOTS_TO_DRAIN_CRBS_TIMEOUT()
                        .set_TARGET(i_target)
                        .set_DATA(l_data),
                        "UMAC was not done dispatching slots to drain of CRBs");

            //Wait for DMA channels to drain
            //while(DMA Status Register[DMA Channel 0:4 Idle] != 5b1_1111){}
            for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
            {
                FAPI_TRY(fapi2::getScom(i_target, PU_SU_STATUS, l_dma_status_reg_data));

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
                        fapi2::P9_WAIT_FOR_DMA_CHANNELS_TO_DRAIN_TIMEOUT()
                        .set_TARGET(i_target)
                        .set_DATA(l_dma_status_reg_data),
                        "DMA channels were not drained");

            //Wait for ERAT to be idle. Should be trivially idle because of the above.
            //while(!ERAT Status and Control Register[ERAT idle]){}
            for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
            {
                FAPI_TRY(fapi2::getScom(i_target, PU_ERAT_STATUS_CONTROL, l_data));

                if (l_data.getBit<PU_ERAT_STATUS_CONTROL_IDLE>())
                {
                    break;
                }

                FAPI_TRY(fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396));
            }

            FAPI_ASSERT(l_data.getBit<PU_ERAT_STATUS_CONTROL_IDLE>(),
                        fapi2::P9_WAIT_FOR_ERAT_IDLE()
                        .set_TARGET(i_target).set_DATA(l_data),
                        "ERAT was not idle");

            //Wait for PBI master machines to be idle
            //while(!DMA Status Register[PBI Idle]){}
            for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
            {
                FAPI_TRY(fapi2::getScom(i_target, PU_SU_STATUS, l_dma_status_reg_data));

                if (l_dma_status_reg_data.getBit<PU_SU_STATUS_PBI_IDLE>())
                {
                    break;
                }

                FAPI_TRY(fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396));
            }

            FAPI_ASSERT(l_dma_status_reg_data.getBit<PU_SU_STATUS_PBI_IDLE>(),
                        fapi2::P9_PBI_MASTER_MACHINES_IDLE_TIMEOUT()
                        .set_TARGET(i_target)
                        .set_DATA(l_dma_status_reg_data),
                        "PBI Master Machines are not idle");
        }

        // If this procedure is followed, then usually if UMAC Status and
        // Control Register[Quiesce Request] is written to 1 then hardware will
        // trivially respond UMAC Status and Control Register[Quiesce Achieved] = 1

        // Write to UMAC Control register(bits 4:6) with '100'
        //TODO RTC 160710 for DD1 this is broken, will readd when we are on DD2
#ifdef DD2
        FAPI_TRY(fapi2::getScom(i_target, PU_UMAC_STATUS_CONTROL, l_data));
        l_data.setBit<PU_UMAC_STATUS_CONTROL_QUIESCE_REQUEST>()
        .clearBit<PU_UMAC_STATUS_CONTROL_QUIESCE_ACHEIVED>()
        .clearBit<PU_UMAC_STATUS_CONTROL_QUIESCE_FAILED>();
        FAPI_TRY(fapi2::putScom(i_target, PU_UMAC_STATUS_CONTROL, l_data));

        // Poll UMAC Control register status register (bit 5, bit 6 indicates fail)
        for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
        {
            FAPI_TRY(fapi2::getScom(i_target, PU_UMAC_STATUS_CONTROL, l_data));

            if (!l_data.getBit<PU_UMAC_STATUS_CONTROL_QUIESCE_FAILED>())
            {
                break;
            }

            FAPI_TRY(fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396));
        }


        FAPI_ASSERT(!l_data.getBit<PU_UMAC_STATUS_CONTROL_QUIESCE_FAILED>(),
                    fapi2::P9_UMAC_QUIESCE_FAILED()
                    .set_TARGET(i_target)
                    .set_DATA(l_data),
                    "UMAC status control quiesce failed");
#endif

    fapi_try_exit:
        FAPI_DBG("p9_nx_check_quiesce: Exiting....");
        return fapi2::current_err;
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_psihb_check_quiesce(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_DBG("p9_psihb_check_quiesce: Entering...");
        fapi2::buffer<uint64_t> l_psihb_data(0);
        const uint32_t c_error_mask_disable_all = 0xFFFull;

        // Need mechanism to quiesce PSI DMAs
        // There are bits on the PSIHB to force the DMAs to be rejected

        // Disable FSP Command Enable bit in PSIHB Command/Status register
        l_psihb_data.setBit<PU_PSIHB_STATUS_CTL_REG_FSP_CMD_ENABLE>()
        .setBit<PU_PSIHB_STATUS_CTL_REG_FSP_INT_ENABLE>();
        FAPI_TRY(fapi2::putScom(i_target, PU_PSIHB_STATUS_CTL_REG_SCOM2, l_psihb_data));

        //mask all interrupts to quiesce
        FAPI_TRY(fapi2::getScom(i_target, PU_PSIHB_ERROR_MASK_REG, l_psihb_data));
        l_psihb_data.insertFromRight<PU_PSIHB_ERROR_MASK_REG_INTERRUPT_DISABLE,
                                     PU_PSIHB_ERROR_MASK_REG_INTERRUPT_DISABLE_LEN>
                                     (c_error_mask_disable_all);
        FAPI_TRY(fapi2::putScom(i_target, PU_PSIHB_ERROR_MASK_REG, l_psihb_data));

        //Poll PSIHBCR bit 20 - inbound queue empty to be 0b0 for quiesce state
        for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
        {
            FAPI_TRY(fapi2::getScom(i_target, PU_PSIHB_STATUS_CTL_REG_SCOM, l_psihb_data));

            if (!l_psihb_data.getBit<PU_PSIHB_STATUS_CTL_REG_FSP_INBOUND_ACTIVE>())
            {
                break;
            }

            FAPI_TRY(fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396));
        }

        FAPI_ASSERT(!l_psihb_data.getBit<PU_PSIHB_STATUS_CTL_REG_FSP_INBOUND_ACTIVE>(),
                    fapi2::P9_PSIHBCR_INBOUND_QUEUE_NOT_EMPTY()
                    .set_TARGET(i_target)
                    .set_DATA(l_psihb_data), "PSIHBCR inbound queue not empty");

        // Disable TCE access by clearing the secure boot register - can't be done
        // later because it's a security hole
        l_psihb_data.flush<0>();
        FAPI_TRY(fapi2::putScom(i_target, PU_TRUST_CONTROL, l_psihb_data));

    fapi_try_exit:
        FAPI_DBG("p9_psihb_check_quiesce: Exiting...");
        return fapi2::current_err;
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_intp_check_quiesce(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_DBG("p9_intp_check_quiesce: Entering...");
        fapi2::buffer<uint64_t> l_data(0);

        //Start the scrub operation in all caches andPoll for completion
        FAPI_TRY(p9_int_scrub_caches(i_target), "Error scrubbing the caches");

        // Read INT_CQ_RST_CTL so that we don't override anything
        FAPI_TRY(fapi2::getScom(i_target, PU_INT_CQ_RST_CTL, l_data));

        // Set bit in INT_CQ_RST_CTL to request quiesce
        l_data.setBit<PU_INT_CQ_RST_CTL_QUIESCE_PB>();
        FAPI_TRY(fapi2::putScom(i_target, PU_INT_CQ_RST_CTL, l_data));

        // Poll master and slave quiesced via bits in RST_CTL
        for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
        {
            FAPI_TRY(fapi2::getScom(i_target, PU_INT_CQ_RST_CTL, l_data));

            if (l_data.getBit<PU_INT_CQ_RST_CTL_MASTER_IDLE>() &&
                l_data.getBit<PU_INT_CQ_RST_CTL_SLAVE_IDLE>())
            {
                break;
            }

            FAPI_TRY(fapi2::delay(C_INTP_DELAY_NS, C_INTP_DELAY_CYCLES));
        }

        FAPI_ASSERT((l_data.getBit<PU_INT_CQ_RST_CTL_MASTER_IDLE>()
                     && l_data.getBit<PU_INT_CQ_RST_CTL_SLAVE_IDLE>()),
                    fapi2::P9_INTP_QUIESCE_TIMEOUT()
                    .set_TARGET(i_target)
                    .set_DATA(l_data), "INTP master or slave is not IDLE");

        //Set sync_reset in RST_CTL
        l_data.setBit<PU_INT_CQ_RST_CTL_SYNC_RESET>();
        FAPI_TRY(fapi2::putScom(i_target, PU_INT_CQ_RST_CTL, l_data));

    fapi_try_exit:
        FAPI_DBG("p9_intp_check_quiesce: Exiting...");
        return fapi2::current_err;
    }

    //---------------------------------------------------------------------------------
    // NOTE: description in header
    //---------------------------------------------------------------------------------
    fapi2::ReturnCode p9_int_scrub_caches(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        fapi2::buffer<uint64_t> l_scrub_trig_data(0);
        const uint64_t l_scrub_addrs[4] =
        {
            PU_INT_PC_VPC_SCRUB_TRIG, PU_INT_VC_SBC_SCRUB_TRIG,
            PU_INT_VC_EQC_SCRUB_TRIG, PU_INT_VC_IVC_SCRUB_TRIG
        };

        //Start the scrub operation in all caches
        for (uint32_t i = 0; i < 4; i++)
        {
            fapi2::putScom(i_target, l_scrub_addrs[i], 0xA000000000000000);

            for (uint32_t j = 0; j < C_NUM_TRIES_QUIESCE_STATE; j++)
            {
                FAPI_TRY(fapi2::delay(C_INTP_DELAY_NS, C_INTP_DELAY_CYCLES));
                FAPI_TRY(fapi2::getScom(i_target, l_scrub_addrs[i], l_scrub_trig_data));

                if (!l_scrub_trig_data.getBit<0>())
                {
                    break;
                }
            }

            FAPI_ASSERT(!l_scrub_trig_data.getBit<0>(),
                        fapi2::P9_INT_SCRUB_NOT_FINISHED_ERR()
                        .set_TARGET(i_target)
                        .set_ADDRESS(l_scrub_addrs[i])
                        .set_DATA(l_scrub_trig_data),
                        "INT scrub operation still busy");
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
        static const uint64_t  SPWKUP_SRC_REGS[4] =
        {
            C_PPM_SPWKUP_OTR, C_PPM_SPWKUP_FSP,
            C_PPM_SPWKUP_HYP, C_PPM_SPWKUP_OCC
        };

        FAPI_DBG("p9_pm_check_quiesce: Entering...");

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

        }

    fapi_try_exit:
        FAPI_DBG("p9_pm_check_quiece: Exiting...");
        return fapi2::current_err;
    }

} // extern "C"
