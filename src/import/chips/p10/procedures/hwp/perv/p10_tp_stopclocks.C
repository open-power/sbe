/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_tp_stopclocks.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2023                        */
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
//------------------------------------------------------------------------------
/// @file  p10_tp_stopclocks.C
///
/// @brief Stop clocks for tp chiplet
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : FSP:HB
//------------------------------------------------------------------------------
#include "p10_tp_stopclocks.H"
#include "p10_scom_perv_2.H"
#include "p10_scom_perv_7.H"
#include "p10_scom_perv_f.H"
#include <p10_perv_sbe_cmn.H>
#include <p10_common_stopclocks.H>
#include <target_filters.H>

enum P10_TP_STOPCLOCKS_Private_Constants
{
    CLOCK_CMD = 0x2,
    CLOCK_TYPES = 0x7,
    REGIONS_ALL_EXCEPT_SBE = 0x5FFF,
    REGIONS_ALL_EXCEPT_SBE_PIB_NET_DPLLNEST_PPLL = 0x4BCF,
};

static fapi2::ReturnCode p10_tp_stopclocks_regions_all_except_sbe(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip, bool i_perv_bist_mode);
static fapi2::ReturnCode p10_sbe_region_stopclocks(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip);

fapi2::ReturnCode p10_tp_stopclocks(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip,
                                    const bool i_stop_tp_clks,
                                    const bool i_stop_sbe_clks,
                                    const bool i_perv_bist_mode)
{
    fapi2::ReturnCode l_rc;
    bool l_tp_chiplet_accessible = 0;

    FAPI_INF("p10_tp_stopclocks: Entering ...");

#ifndef __PPE__
    FAPI_INF("p10_tp_stopclocks: Input arguments \n\t i_stop_tp_clks  = %s \n\t i_stop_sbe_clks = %s",
             btos(i_stop_tp_clks), btos(i_stop_sbe_clks));
#endif

    // We are intentionally making the trade of dropping the FFDC
    // gathered by the sub-HWP so that we can avoid breaking from
    // the current HWP by using FAPI_ASSERT_NOEXIT on a custom RC
    // for stopclocks.
    l_rc = p10_common_stopclock_tp_chiplet_accessible(i_target_chip, l_tp_chiplet_accessible);
    FAPI_ASSERT_NOEXIT(l_rc == fapi2::FAPI2_RC_SUCCESS,
                       fapi2::TP_STOPCLOCKS_ERR()
                       .set_TARGET_CHIP(i_target_chip),
                       "Checking tp chiplet accessible returned error, can't call stopclocks fpr tp chiplet");

    if(l_tp_chiplet_accessible)
    {
        if(i_stop_tp_clks)
        {
            // We are intentionally making the trade of dropping the FFDC
            // gathered by the sub-HWP so that we can avoid breaking from
            // the current HWP by using FAPI_ASSERT_NOEXIT on a custom RC
            // for stopclocks.
            l_rc = p10_tp_stopclocks_regions_all_except_sbe(i_target_chip, i_perv_bist_mode);

            FAPI_ASSERT_NOEXIT(l_rc == fapi2::FAPI2_RC_SUCCESS,
                               fapi2::TP_STOPCLOCKS_ERR()
                               .set_TARGET_CHIP(i_target_chip),
                               "p10_tp_stopclocks returned error when stopping clocks for regions all except SBE [perv, sbe, pib, occ, net, pll regions]");
        }

        if(i_stop_sbe_clks)
        {
            // We are intentionally making the trade of dropping the FFDC
            // gathered by the sub-HWP so that we can avoid breaking from
            // the current HWP by using FAPI_ASSERT_NOEXIT on a custom RC
            // for stopclocks.
            l_rc = p10_sbe_region_stopclocks(i_target_chip);
            FAPI_ASSERT_NOEXIT(l_rc == fapi2::FAPI2_RC_SUCCESS,
                               fapi2::TP_STOPCLOCKS_ERR()
                               .set_TARGET_CHIP(i_target_chip),
                               "p10_tp_stopclocks returned error when stopping clocks for sbe region");
        }
    }

    FAPI_INF("p10_tp_stopclocks: Exiting ...");

    return fapi2::FAPI2_RC_SUCCESS;

}

/// @brief -- Stopclocks for all regions except sbe (perv, sbe, pib, occ, net, pll regions)
///        -- Check if chiplet is accessible,switch mux to PIB2PCB, Call clock_start_stop module,
///           Set vital fence and flushmode, raise pg fences
///
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
static fapi2::ReturnCode p10_tp_stopclocks_regions_all_except_sbe(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip, bool i_perv_bist_mode)
{
    fapi2::buffer<uint64_t> l_clock_regions;

    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_tpchiplet =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP, fapi2::TARGET_STATE_FUNCTIONAL)[0];

    FAPI_INF("p10_tp_stopclocks_regions_all_except_sbe: Entering ...");

    if (i_perv_bist_mode)
    {
        // Reqiured for DFT ABIST testing
        FAPI_DBG("p10_tp_stopclocks: TP regions selected is REGIONS_ALL_EXCEPT_SBE_PIB_NET_DPLLNEST_PPLL");
        l_clock_regions = REGIONS_ALL_EXCEPT_SBE_PIB_NET_DPLLNEST_PPLL;
    }
    else
    {
        FAPI_DBG("p10_tp_stopclocks: TP regions selected is REGIONS_ALL_EXCEPT_SBE");
        l_clock_regions = REGIONS_ALL_EXCEPT_SBE;

        FAPI_DBG("Switch mux to PIB2PCB path");
        FAPI_TRY(p10_perv_sbe_cmn_switch_mux_scom(i_target_chip, PIB2PCB));
    }

    FAPI_DBG("p10_tp_stopclocks: Regions value: %#018lX", l_clock_regions);
    FAPI_TRY(p10_perv_sbe_cmn_clock_start_stop(l_tpchiplet, CLOCK_CMD, 0, 0, l_clock_regions, CLOCK_TYPES));

    FAPI_DBG("p10_tp_stopclocks: Assert vital fence and set flush_inhibit");
    FAPI_TRY(p10_common_stopclocks_set_vitalfence_flushmode(l_tpchiplet));

    FAPI_DBG("p10_tp_stopclocks: Raise partial good fences and set abist_muxsel, syncclk_muxsel");
    FAPI_TRY(p10_common_stopclocks_cplt_ctrl_action_function(l_tpchiplet, l_clock_regions));

    FAPI_INF("p10_tp_stopclocks_regions_all_except_sbe: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}

/// @brief -- Stopclocks for sbe region
///        -- Check if chiplet is accessible,Raise chiplet fence, Switch mux to FSI2PCB path,
///           Send CBS cmd to stop sbe clocks, Raise cfam protection fences
///
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
static fapi2::ReturnCode p10_sbe_region_stopclocks(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt;
    using namespace scomt::perv;

    fapi2::buffer<uint32_t> l_data32, l_data32_root_ctrl0;

    FAPI_INF("p10_sbe_region_stopclocks: Entering ...");

    if (fapi2::is_platform<fapi2::PLAT_SBE>())
    {
        FAPI_ERR("p10_tp_stopclocks: Calling TP stopclocks for SBE regions in SBE mode is INVALID\n\t --> Skipping TP Stopclocks for SBE regions..! <--");

        fapi2::current_err = fapi2::FAPI2_RC_INVALID_PARAMETER;
        goto fapi_try_exit;
    }
    else
    {
        FAPI_DBG("p10_tp_stopclocks: Raise chiplet fence");
        l_data32.flush<0>().setBit<FSXCOMP_FSXLOG_PERV_CTRL0_TP_FENCE_EN_DC>();
        FAPI_TRY(fapi2::putCfamRegister(i_target_chip, FSXCOMP_FSXLOG_PERV_CTRL0_SET_FSI, l_data32));

        FAPI_DBG("Switch mux to FSI2PCB path");
        FAPI_TRY(p10_perv_sbe_cmn_switch_mux_cfam(i_target_chip, FSI2PCB));

        //Using CBS interface to stop clock
        FAPI_DBG("p10_tp_stopclocks: Call module clock start stop for SBE only");
        FAPI_TRY(p10_common_stopclocks_sbe_clkstop(i_target_chip));

        FAPI_DBG("p10_tp_stopclocks: Assert CFAM fences");
        // RC0 bit0 : cfam_protection0,  bit8 : cfam_protection1, bit9 : cfam_protection2
        // Not raising ROOT_CTRL0_CFAM_PROTECTION_1_DC because we need the SHIFT <- TPvitl path for toad mode
        // Not raising ROOT_CTRL0_CFAM_PROTECTION_2_DC because we need FSI2PIB <- PERV EPS connectivity
        l_data32_root_ctrl0.flush<0>()
        .setBit<FSXCOMP_FSXLOG_ROOT_CTRL0_CFAM_PROTECTION_0_DC>();
        FAPI_TRY(fapi2::putCfamRegister(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL0_SET_FSI, l_data32_root_ctrl0));

        FAPI_DBG("p10_tp_stopclocks: Raise OOB Mux");
        l_data32_root_ctrl0.flush<0>().setBit<FSXCOMP_FSXLOG_ROOT_CTRL0_OOB_MUX>();
        FAPI_TRY(fapi2::putCfamRegister(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL0_SET_FSI, l_data32_root_ctrl0));
    }

    FAPI_INF("p10_sbe_region_stopclocks: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}
