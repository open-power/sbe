/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_stopclocks.C $   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2020                        */
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
/// @file  p10_stopclocks.C
///
/// @brief The purpose of this procedure is to stop the clocks of the P10 processor chip
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : FSP:SBE:HB
//------------------------------------------------------------------------------

#include "p10_stopclocks.H"
#include "p10_scom_perv_2.H"
#include "p10_scom_perv_3.H"
#include "p10_scom_perv_4.H"
#include "p10_scom_perv_7.H"
#include "p10_scom_perv_a.H"
#include "p10_scom_perv_f.H"
#include <p10_cplt_stopclocks.H>
#include <p10_tp_stopclocks.H>
#include <p10_hcd_core_stopclocks.H>
#include <p10_hcd_cache_stopclocks.H>
#include <p10_hcd_eq_stopclocks.H>
#include <p10_common_stopclocks.H>
#include <p10_hcd_common.H>
#include <multicast_group_defs.H>
#include <target_filters.H>

//------------------------------------------------------------------------------
// Function definition:  p10_stopclocks
// parameters:  i_target                      =>   chip target
//          i_flags.stop_nest_clks        True if NEST chiplet clocks should be stopped, else false
//              i_flags.stop_pcie_clks        True if PCIE chiplet clocks should be stopped, else false
//              i_flags.stop_mc_clks          True if MC   chiplet clocks should be stopped, else false
//              i_flags.stop_pau_clks         True if PAU chiplet clocks should be stopped, else false
//              i_flags.stop_axon_clks        True if AXON chiplet clocks should be stopped, else false
//              i_flags.stop_tp_clks          True if PERV (TP) chiplet clocks all except SBE should be stopped, else false
//              i_flags.stop_sbe_clks         True if PERV (TP) chiplet SBE clocks should be stopped, else false
//              i_flags.stop_vitl_clks        True if PERV VITL clocks should be stopped, else false
//              i_flags.stop_cache_clks       True if CACHE chiplet clocks should be stopped, else false
//              i_flags.stop_core_clks        True if CORE chiplet clocks should be stopped, else false
//              i_flags.stop_eq_clks          True if EQ chiplet clocks should be stopped, else false
// returns: FAPI_RC_SUCCESS if operation was successful, else error
//------------------------------------------------------------------------------

static fapi2::ReturnCode p10_vital_stopclocks(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip);

fapi2::ReturnCode p10_stopclocks(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip,
                                 const p10_stopclocks_flags& i_flags)
{
    using namespace scomt;
    using namespace scomt::perv;

    fapi2::ReturnCode l_rc_core, l_rc_cache, l_rc_vitl, l_rc_eq;
    fapi2::Target<fapi2::TARGET_TYPE_PERV>      l_target_tp = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
            (fapi2::TARGET_FILTER_TP, fapi2::TARGET_STATE_FUNCTIONAL)[0];

    fapi2::buffer<uint32_t> l_cfam_data;
    fapi2::buffer<uint64_t> l_data64;
    fapi2::buffer<uint64_t> l_sl_clock_status;
    fapi2::buffer<uint64_t> l_nsl_clock_status;
    fapi2::buffer<uint64_t> l_ary_clock_status;

    bool pcb_is_bypassed      = false;
    bool pcb_clks_are_off     = false;

    bool tp_cplt_en           = false;
    bool tp_ep_rst            = true;
    bool tp_vitl_clk_off      = true;

    FAPI_INF("p10_stopclocks : Entering ...");

    auto core_mc_target = i_target_chip.getMulticast(fapi2::MCGROUP_GOOD_EQ, fapi2::MCCORE_ALL);
    //auto l_target_core = i_target_chip.getChildren<fapi2::TARGET_TYPE_CORE>(fapi2::TARGET_STATE_FUNCTIONAL);

    FAPI_DBG("p10_stopclocks : Input arguments received are \n\t"
             "i_stop_nest = %d\n\ti_stop_pcie = %d\n\ti_stop_mc = %d\n\t"
             "i_stop_pau = %d\n\ti_stop_axon = %d\n", i_flags.stop_nest_clks, i_flags.stop_pcie_clks,
             i_flags.stop_mc_clks, i_flags.stop_pau_clks, i_flags.stop_axon_clks);
    FAPI_DBG("\n\ti_stop_tp = %d\n\ti_stop_sbe = %d\n\ti_stop_vitl =  %d\n",
             i_flags.stop_tp_clks, i_flags.stop_sbe_clks, i_flags.stop_vitl_clks);
    FAPI_DBG("p10_stopclocks : Input QUAD arguments received are \n\t"
             "i_stop_cache = %d\n\ti_stop_core = %d\n\ti_stop_eq = %d ",
             i_flags.stop_cache_clks, i_flags.stop_core_clks, i_flags.stop_eq_clks);

    FAPI_DBG("p10_stopclocks : Check to see if the Perv Vital clocks are OFF");

    if (fapi2::is_platform<fapi2::PLAT_SBE>())
    {
        FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_PERV_CTRL0_RW, l_data64));
        tp_cplt_en         = l_data64.getBit<FSXCOMP_FSXLOG_PERV_CTRL0_TP_TCPERV_CHIPLET_EN_DC>();
        tp_ep_rst          = l_data64.getBit<FSXCOMP_FSXLOG_PERV_CTRL0_TP_TCPERV_PCB_EP_RESET_DC>();
        tp_vitl_clk_off    = l_data64.getBit<FSXCOMP_FSXLOG_PERV_CTRL0_TP_VITL_CLKOFF_DC>();
    }
    else
    {
        FAPI_TRY(fapi2::getCfamRegister(i_target_chip, FSXCOMP_FSXLOG_PERV_CTRL0_FSI, l_cfam_data));
        tp_cplt_en      = l_cfam_data.getBit<FSXCOMP_FSXLOG_PERV_CTRL0_TP_TCPERV_CHIPLET_EN_DC>();
        tp_ep_rst       = l_cfam_data.getBit<FSXCOMP_FSXLOG_PERV_CTRL0_TP_TCPERV_PCB_EP_RESET_DC>();
        tp_vitl_clk_off = l_cfam_data.getBit<FSXCOMP_FSXLOG_PERV_CTRL0_TP_VITL_CLKOFF_DC>();
    }

    FAPI_DBG("Read PERV_CTRL0 Reg Value and observe CPLT_EN = %d, EP_RST = %d, VITL_CLKOFF = %d", tp_cplt_en,
             tp_ep_rst, tp_vitl_clk_off);

    if(tp_vitl_clk_off)
    {
        FAPI_ERR("p10_stopclocks : Perv Vital clocks are off, so stopclocks cant go ahead");
        FAPI_ASSERT(false,
                    fapi2::TP_VITL_CLK_OFF_ERR()
                    .set_TARGET_CHIP(i_target_chip),
                    "Perv Vital clocks are off");
    }

    if(tp_cplt_en && !(tp_ep_rst) && !(tp_vitl_clk_off))
    {
        const bool l_stop_any_chiplet = i_flags.stop_nest_clks || i_flags.stop_pcie_clks || i_flags.stop_mc_clks
                                        || i_flags.stop_pau_clks || i_flags.stop_axon_clks || i_flags.stop_cache_clks
                                        || i_flags.stop_core_clks || i_flags.stop_eq_clks;

        FAPI_DBG("p10_stopclocks : Check to see if the PIB/PCB network is being bypassed");

        if (fapi2::is_platform<fapi2::PLAT_SBE>())
        {
            FAPI_TRY(fapi2::getScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL0_RW, l_data64));
            // PIB2PCB path : bit 16:0 bit18:1 bit19:0
            pcb_is_bypassed = l_data64.getBit<FSXCOMP_FSXLOG_ROOT_CTRL0_PIB2PCB_DC>()
                              || l_data64.getBit<FSXCOMP_FSXLOG_ROOT_CTRL0_FSI2PCB_DC>();
        }
        else
        {
            FAPI_TRY(fapi2::getCfamRegister(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL0_FSI, l_cfam_data));
            // PIB2PCB path : bit 16:0 bit18:1 bit19:0
            pcb_is_bypassed = l_cfam_data.getBit<FSXCOMP_FSXLOG_ROOT_CTRL0_PIB2PCB_DC>()
                              || l_cfam_data.getBit<FSXCOMP_FSXLOG_ROOT_CTRL0_FSI2PCB_DC>();
        }

        if(pcb_is_bypassed)
        {
            if(l_stop_any_chiplet)
            {
                FAPI_ERR("p10_stopclocks : The PIB/PCB is being bypassed, so only the TP chiplet is accessible. Stopping TP only.");
            }
            else
            {
                FAPI_IMP("p10_stopclocks : The PIB/PCB is being bypassed, so only the TP chiplet is accessible.");
            }
        }
        else
        {
            FAPI_INF("p10_stopclocks : Reading Clock Status Register in the TP chiplet to see if SBE,PIB and NET clocks are running. Bits 5, 6 & 8 should be zero.");

            FAPI_DBG("Read Perv Clock Stat SL");
            FAPI_TRY(fapi2::getScom(l_target_tp, CLOCK_STAT_SL, l_sl_clock_status));
            FAPI_DBG("Perv CLOCK_STAT_SL Value : %#018lX", l_sl_clock_status);

            FAPI_DBG("Read Perv Clock Stat NSL");
            FAPI_TRY(fapi2::getScom(l_target_tp, CLOCK_STAT_NSL, l_nsl_clock_status));
            FAPI_DBG("Perv CLOCK_STAT_NSL Value : %#018lX", l_nsl_clock_status);

            FAPI_DBG("Read Perv Clock Stat ARY");
            FAPI_TRY(fapi2::getScom(l_target_tp, CLOCK_STAT_ARY, l_ary_clock_status));
            FAPI_DBG("Perv CLOCK_STAT_ARY Value : %#018lX", l_ary_clock_status);

            if(l_sl_clock_status.getBit<5>() || l_sl_clock_status.getBit<6>() || l_sl_clock_status.getBit<8>() ||
               l_nsl_clock_status.getBit<5>() || l_nsl_clock_status.getBit<6>() || l_nsl_clock_status.getBit<8>() ||
               l_ary_clock_status.getBit<5>() || l_ary_clock_status.getBit<6>() || l_ary_clock_status.getBit<8>())
            {
                FAPI_ERR("p10_stopclocks : At least one of the SBE or PIB or NET clocks is NOT running.  May not be able to use the PCB fabric to access chiplets.");
                pcb_clks_are_off      = true;
            }
            else
            {
                pcb_clks_are_off      = false;
            }
        }
    }
    else
    {
        FAPI_ERR("p10_stopclocks : TP chiplet dont have favourable conditions to access Clock Controller registers.");
        pcb_clks_are_off      = true;
    }

    FAPI_DBG("\n\tpcb_is_bypassed = %d \n\tpcb_clks_are_off = %d \t\n ", pcb_is_bypassed, pcb_clks_are_off);

    if ( (pcb_is_bypassed == false) && (pcb_clks_are_off == false))
    {
        // TODO: Should both p10_hcd_core_stopclocks, p10_hcd_cache_stopclocks takes core target in unicast??
        // Core stopclocks
        if(i_flags.stop_core_clks)
        {
            FAPI_INF("p10_stopclocks : Calling p10_hcd_core_stopclocks");
            l_rc_core = p10_hcd_core_stopclocks(core_mc_target);

            FAPI_ASSERT_NOEXIT(l_rc_core == fapi2::FAPI2_RC_SUCCESS,
                               fapi2::HCD_CORE_STOPCLOCKS_ERR(),
                               "p10_hcd_core_stopclocks returned error");
        }

        // Cache stopclocks
        if(i_flags.stop_cache_clks)
        {
            // unicast target looping
            FAPI_INF("p10_stopclocks : Calling p10_hcd_cache_stopclocks");
            l_rc_cache = p10_hcd_cache_stopclocks(core_mc_target);

            FAPI_ASSERT_NOEXIT(l_rc_cache == fapi2::FAPI2_RC_SUCCESS,
                               fapi2::HCD_CACHE_STOPCLOCKS_ERR(),
                               "p10_hcd_cache_stopclocks returned error");
        }

        // EQ stopclocks
        if(i_flags.stop_eq_clks)
        {
            FAPI_INF("p10_stopclocks : Calling p10_hcd_eq_stopclocks.");

            for (const auto l_eq_target : i_target_chip.getChildren<fapi2::TARGET_TYPE_EQ>(fapi2::TARGET_STATE_FUNCTIONAL))
            {
                l_rc_eq = p10_hcd_eq_stopclocks(l_eq_target);
                FAPI_ASSERT_NOEXIT(l_rc_eq == fapi2::FAPI2_RC_SUCCESS,
                                   fapi2::HCD_EQ_STOPCLOCKS_ERR(),
                                   "p10_hcd_eq_stopclocks returned error");
            }
        }

        // Chiplet stopclocks : Nest, Pcie, Mc, Pau, Axon
        if(i_flags.stop_nest_clks || i_flags.stop_pcie_clks || i_flags.stop_mc_clks || i_flags.stop_pau_clks
           || i_flags.stop_axon_clks )
        {
            // p10_cplt_stopclocks will log errors if any during stopclocks sequence into FFDC
            FAPI_INF("p10_stopclocks : Call p10_cplt_stopclocks function");
            p10_cplt_stopclocks(i_target_chip, i_flags.stop_nest_clks, i_flags.stop_pcie_clks, i_flags.stop_mc_clks,
                                i_flags.stop_pau_clks, i_flags.stop_axon_clks);
        }

    }

    // TP chiplet stopclocks
    if(i_flags.stop_tp_clks || i_flags.stop_sbe_clks)
    {
        // p10_tp_stopclocks will log errors if any during stopclocks sequence into FFDC
        FAPI_INF("p10_stopclocks : Call p10_tp_stopclocks function");
        p10_tp_stopclocks(i_target_chip, i_flags.stop_tp_clks, i_flags.stop_sbe_clks);
    }

    // Vital stopclocks
    if(i_flags.stop_vitl_clks)
    {

        l_rc_vitl = p10_vital_stopclocks(i_target_chip);

        FAPI_ASSERT_NOEXIT(l_rc_vitl == fapi2::FAPI2_RC_SUCCESS,
                           fapi2::VITL_STOPCLOCKS_ERR(),
                           "p10_vital_stopclocks returned error");
    }

    FAPI_INF("p10_stopclocks : Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Stop vital clocks
///
/// @param[in]     i_target_chip    Reference to TARGET_TYPE_PROC_CHIP
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p10_vital_stopclocks(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{

    using namespace scomt;
    using namespace scomt::perv;

    fapi2::buffer<uint32_t> l_cfam_data;

    FAPI_INF("p10_vital_stopclocks : Entering ...");

    if (fapi2::is_platform<fapi2::PLAT_SBE>())
    {
        FAPI_ERR("p10_stopclocks : WARNING::VITAL clocks can't be stopped in SBE mode\n\t --> Skipping VITAL Stopclocks..! <--");
    }
    else
    {
        FAPI_INF("p10_stopclocks : Stopping Pervasive VITAL clocks");

        l_cfam_data.flush<0>().setBit<FSXCOMP_FSXLOG_PERV_CTRL0_TP_VITL_CLKOFF_DC>();

        FAPI_DBG("Writing the PERV_CTRL0 register TP_VITL_CLKOFF_DC=1");
        FAPI_TRY(fapi2::putCfamRegister(i_target_chip, FSXCOMP_FSXLOG_PERV_CTRL0_SET_FSI, l_cfam_data));

        //Raising Fence RC0(8:14) PC0(18,15)
        FAPI_INF("p10_stopclocks : Raising fences");

        l_cfam_data.flush<0>().setBit<8, 7>();

        FAPI_DBG("Writing the ROOT_CTRL0 register to raise FSI fences");
        FAPI_TRY(fapi2::putCfamRegister(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL0_SET_FSI, l_cfam_data));

        l_cfam_data.flush<0>()
        .setBit<FSXCOMP_FSXLOG_PERV_CTRL0_TP_FENCE_EN_DC>()
        .setBit<FSXCOMP_FSXLOG_PERV_CTRL0_TP_FENCE_PCB_DC>();

        FAPI_DBG("Writing the PERV_CTRL0 register to raise fences");
        FAPI_TRY(fapi2::putCfamRegister(i_target_chip, FSXCOMP_FSXLOG_PERV_CTRL0_SET_FSI, l_cfam_data));
    }

    FAPI_INF("p10_vital_stopclocks : Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}
