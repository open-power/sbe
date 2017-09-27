/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_stopclocks.C $     */
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
//------------------------------------------------------------------------------
/// @file  p9_stopclocks.C
///
/// @brief The purpose of this procedure is to stop the clocks of the P9 processor chip
//------------------------------------------------------------------------------
// *HWP HW Owner        : Soma BhanuTej <soma.bhanu@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : Sunil Kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 3
// *HWP Consumed by     : FSP:SBE:HB
//------------------------------------------------------------------------------

//## auto_generated
#include "p9_stopclocks.H"
#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p9_cplt_stopclocks.H>
#include <p9_nest_stopclocks.H>
#include <p9_tp_stopclocks.H>
#include <p9_hcd_core_stopclocks.H>
#include <p9_hcd_cache_stopclocks.H>
#ifdef __PPE__
    #include <p9_common_stopclocks.H>
#else
    #include <p9_check_chiplet_states.H>
#endif
#include <p9_hcd_common.H>

//------------------------------------------------------------------------------
// Function definition:  p9_stopclocks
// parameters:  i_target                      =>   chip target
//              i_flags                       =>   flags as per the following definition
//              i_flags.stop_nest_clks        =>   True to stop NEST chiplet clocks                              (should default TRUE)
//              i_flags.stop_mc_clks          =>   True to stop MC chiplet clocks                                (should default TRUE)
//              i_flags.stop_xbus_clks        =>   True to stop XBUS chiplet clocks                              (should default TRUE)
//              i_flags.stop_obus_clks        =>   True to stop OBUS chiplet clocks                              (should default TRUE)
//              i_flags.stop_pcie_clks        =>   True to stop PCIE chiplet clocks                              (should default TRUE)
//              i_flags.stop_tp_clks          =>   True to stop PERVASIVE (TP) chiplet clocks all except PIB/NET (should default FALSE)
//              i_flags.stop_pib_clks         =>   True to stop PERVASIVE (TP) chiplet PIB/NET clocks            (should default FALSE)
//              i_flags.stop_vitl_clks        =>   True to stop PERVASIVE VITL clocks                            (should default FALSE)
//              i_flags.stop_cache_clks       =>   True to stop CACHE chiplet clocks                             (should default TRUE)
//              i_flags.stop_core_clks        =>   True to stop CORE chiplet clocks                              (should default TRUE)
//              i_flags.sync_stop_quad_clks   =>   True to stop CACHE & CORE chiplet clocks synchronously        (should default TRUE)
//              i_eq_clk_regions              =>   EQ chiplet clock regions of which clocks should be stopped    (default ALL_BUT_PLL_REFR)
//              i_ex_select                   =>   EX chiplet selected for clocks stop                           (default BOTH_EX)
// returns: FAPI_RC_SUCCESS if operation was successful, else error
//------------------------------------------------------------------------------

fapi2::ReturnCode p9_stopclocks(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip,
                                const p9_stopclocks_flags& i_flags,
                                const p9hcd::P9_HCD_CLK_CTRL_CONSTANTS i_eq_clk_regions,
                                const p9hcd::P9_HCD_EX_CTRL_CONSTANTS i_ex_select)
{

    fapi2::Target<fapi2::TARGET_TYPE_PERV>      l_target_tp = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
            (fapi2::TARGET_FILTER_TP, fapi2::TARGET_STATE_FUNCTIONAL)[0];

    fapi2::buffer<uint32_t> l_cfam_data;
    fapi2::buffer<uint64_t> l_data64;
    fapi2::buffer<uint64_t> l_sl_clock_status;
    fapi2::buffer<uint64_t> l_nsl_clock_status;
    fapi2::buffer<uint64_t> l_ary_clock_status;

    uint8_t l_cplt_scomable = 0;

    bool pcb_is_bypassed      = false;
    bool pcb_clks_are_off     = false;
    bool perv_vitl_clks_off   = false;

    bool tp_cplt_scomable     = true;
    bool nest_cplt_scomable   = true;
    bool xbus_cplt_scomable   = true;
    bool mc_cplt_scomable     = true;
    bool obus_cplt_scomable   = true;
    bool pcie_cplt_scomable   = true;

    bool tp_cplt_en           = false;
    bool tp_ep_rst            = true;
    bool tp_vitl_clk_off      = true;
    bool tp_mesh_clk_en       = false;

    bool sync_quad_stopclocks = i_flags.sync_stop_quad_clks;
#ifdef __PPE__
    uint8_t l_tp_chiplet_accesible = 0;
#endif

    FAPI_INF("p9_stopclocks : Entering ...");

    FAPI_DBG("p9_stopclocks : Input arguments received are \n\t"
             "i_stop_nest = %d\n\t i_stop_mc = %d\n\t i_stop_xbus = %d\n\t"
             "i_stop_obus = %d\n", i_flags.stop_nest_clks, i_flags.stop_mc_clks,
             i_flags.stop_xbus_clks, i_flags.stop_obus_clks);
    FAPI_DBG("i_stop_pcie = %d\n\t i_stop_tp = %d\n\t i_stop_pib = %d\n\t "
             "i_stop_vitl =  %d\n",
             i_flags.stop_pcie_clks,
             i_flags.stop_tp_clks, i_flags.stop_pib_clks,
             i_flags.stop_vitl_clks);
    FAPI_DBG("p9_stopclocks : Input QUAD arguments received are \n\t"
             "i_stop_cache = %d\n\t i_stop_core = %d\n\t i_sync_stop_quad = %d\n\t "
             "i_eq_clk_regions = %#018lx \n\t i_ex_select = %#018lx\n",
             i_flags.stop_cache_clks, i_flags.stop_core_clks, i_flags.sync_stop_quad_clks,
             (uint64_t)i_eq_clk_regions, (uint64_t)i_ex_select);

    FAPI_DBG("p9_stopclocks : Check to see if the Perv Vital clocks are OFF");
#ifdef __PPE__
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_PERV_CTRL0_SCOM, l_data64));
    perv_vitl_clks_off = l_data64.getBit<PERV_PERV_CTRL0_TP_VITL_CLKOFF_DC>();
#else
    FAPI_TRY(fapi2::getCfamRegister(i_target_chip, PERV_PERV_CTRL0_FSI, l_cfam_data));
    perv_vitl_clks_off = l_cfam_data.getBit<PERV_PERV_CTRL0_TP_VITL_CLKOFF_DC>();
#endif

    if(perv_vitl_clks_off)
    {
        FAPI_ERR("p9_stopclocks : Perv Vital clocks are off, so stopclocks cant go ahead");
    }

    if(!(perv_vitl_clks_off))
    {
        FAPI_DBG("p9_stopclocks : Check to see if the PIB/PCB network is being bypassed");
#ifdef __PPE__
        FAPI_TRY(fapi2::getScom(i_target_chip, PERV_ROOT_CTRL0_SCOM, l_data64));
        pcb_is_bypassed = l_data64.getBit<PERV_ROOT_CTRL0_PIB2PCB_DC>();
#else
        FAPI_TRY(fapi2::getCfamRegister(i_target_chip, PERV_ROOT_CTRL0_FSI, l_cfam_data));
        pcb_is_bypassed = l_cfam_data.getBit<PERV_ROOT_CTRL0_PIB2PCB_DC>();
#endif

        if(pcb_is_bypassed)
        {
            if(i_flags.stop_nest_clks || i_flags.stop_mc_clks || i_flags.stop_xbus_clks || i_flags.stop_obus_clks
               || i_flags.stop_pcie_clks || i_flags.stop_cache_clks
               || i_flags.stop_core_clks)
            {
                FAPI_ERR("p9_stopclocks : The PIB/PCB is being bypassed, so only the TP chiplet is accessible.");
            }
            else
            {
                FAPI_IMP("p9_stopclocks : The PIB/PCB is being bypassed, so only the TP chiplet is accessible.");
            }
        }

#ifdef __PPE__
        FAPI_TRY(fapi2::getScom(i_target_chip, PERV_PERV_CTRL0_SCOM, l_data64));
        tp_cplt_en      = l_data64.getBit<PERV_PERV_CTRL0_TP_CHIPLET_EN_DC>();
        tp_ep_rst       = l_data64.getBit<PERV_PERV_CTRL0_TP_PCB_EP_RESET_DC>();
        tp_vitl_clk_off = l_data64.getBit<PERV_PERV_CTRL0_CLEAR_TP_VITL_CLKOFF_DC>();
        tp_mesh_clk_en  = l_data64.getBit<PERV_PERV_CTRL0_TP_PLLCHIPLET_FORCE_OUT_EN_DC>();
#else
        FAPI_TRY(fapi2::getCfamRegister(i_target_chip, PERV_PERV_CTRL0_FSI, l_cfam_data));
        tp_cplt_en      = l_cfam_data.getBit<PERV_PERV_CTRL0_TP_CHIPLET_EN_DC>();
        tp_ep_rst       = l_cfam_data.getBit<PERV_PERV_CTRL0_TP_PCB_EP_RESET_DC>();
        tp_vitl_clk_off = l_cfam_data.getBit<PERV_PERV_CTRL0_CLEAR_TP_VITL_CLKOFF_DC>();
        tp_mesh_clk_en  = l_cfam_data.getBit<PERV_PERV_CTRL0_TP_PLLCHIPLET_FORCE_OUT_EN_DC>();
#endif
        FAPI_DBG("Read PERV_CTRL0 Reg Value and observe CPLT_EN = %d, EP_RST = %d, VITL_CLKOFF = %d", tp_cplt_en,
                 tp_ep_rst, tp_vitl_clk_off);

        if (tp_cplt_en && !(tp_ep_rst) && !(tp_vitl_clk_off))
        {
            FAPI_INF("p9_stopclocks : Reading Clock Status Register in the TP chiplet to see if PIB and NET clocks are running. Bits 5 & 6 should be zero.");

            FAPI_DBG("Read Perv Clock Stat SL");
            FAPI_TRY(fapi2::getScom(l_target_tp, PERV_CLOCK_STAT_SL, l_sl_clock_status));
            FAPI_DBG("Perv CLOCK_STAT_SL Value : %#018lX", l_sl_clock_status);

            FAPI_DBG("Read Perv Clock Stat NSL");
            FAPI_TRY(fapi2::getScom(l_target_tp, PERV_CLOCK_STAT_NSL, l_nsl_clock_status));
            FAPI_DBG("Perv CLOCK_STAT_NSL Value : %#018lX", l_nsl_clock_status);

            FAPI_DBG("Read Perv Clock Stat ARY");
            FAPI_TRY(fapi2::getScom(l_target_tp, PERV_CLOCK_STAT_ARY, l_ary_clock_status));
            FAPI_DBG("Perv CLOCK_STAT_ARY Value : %#018lX", l_ary_clock_status);

            if(l_sl_clock_status.getBit<PERV_1_CLOCK_STAT_SL_STATUS_UNIT1>() ||
               l_sl_clock_status.getBit<PERV_1_CLOCK_STAT_SL_STATUS_UNIT2>() ||
               l_nsl_clock_status.getBit<PERV_1_CLOCK_STAT_NSL_STATUS_UNIT1>() ||
               l_nsl_clock_status.getBit<PERV_1_CLOCK_STAT_NSL_STATUS_UNIT2>() ||
               l_ary_clock_status.getBit<PERV_1_CLOCK_STAT_ARY_STATUS_UNIT1>() ||
               l_ary_clock_status.getBit<PERV_1_CLOCK_STAT_ARY_STATUS_UNIT2>())
            {
                FAPI_ERR("p9_stopclocks : At least one of the NET or PIB clocks is NOT running.  May not be able to use the PCB fabric to access chiplets.");
                pcb_clks_are_off      = true;
            }
            else
            {
                pcb_clks_are_off      = false;
            }

#ifdef __PPE__
            l_tp_chiplet_accesible = true;
#endif
        }
        else
        {
            FAPI_ERR("p9_stopclocks : TP chiplet dont have favourable conditions to access Clock Controller registers.");
            pcb_clks_are_off      = true;
        }


        if ( (pcb_is_bypassed == false) && (pcb_clks_are_off == false) )
        {
            FAPI_DBG("p9_stopclocks : Call p9_check_chiplet_states to get the state of chip");
#ifndef __PPE__
            FAPI_TRY(p9_check_chiplet_states(i_target_chip));
#endif

            //To get the chiplet state from Attributes
            for (const auto& l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
                 (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_TP | fapi2::TARGET_FILTER_ALL_MC | fapi2::TARGET_FILTER_ALL_NEST
                                                   | fapi2::TARGET_FILTER_ALL_OBUS | fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_XBUS),
                  fapi2::TARGET_STATE_FUNCTIONAL))
            {
                uint8_t l_attr_unit_pos = 0;
                FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_cplt, l_attr_unit_pos));

                if (l_attr_unit_pos == PERV_CHIPLET_ID)
                {
#ifdef __PPE__

                    if(l_tp_chiplet_accesible)
                    {
                        FAPI_TRY(p9_common_stopclock_is_scommable(l_target_cplt, l_cplt_scomable));
                    }
                    else
                    {
                        l_cplt_scomable = false;
                    }

#else
                    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_TARGET_IS_SCOMMABLE, l_target_cplt, l_cplt_scomable));
#endif
                    tp_cplt_scomable = l_cplt_scomable;
                }
                else
                {
#ifdef __PPE__
                    uint8_t l_chiplet_accessible = 0;
                    FAPI_TRY(p9_common_stopclock_chiplet_accesible(l_target_cplt, l_chiplet_accessible));

                    if(l_chiplet_accessible)
                    {
                        FAPI_TRY(p9_common_stopclock_is_scommable(l_target_cplt, l_cplt_scomable));
                    }
                    else
                    {
                        l_cplt_scomable = false;
                    }

#else
                    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_TARGET_IS_SCOMMABLE, l_target_cplt, l_cplt_scomable));
#endif

                    if (l_attr_unit_pos == N0_CHIPLET_ID || l_attr_unit_pos == N1_CHIPLET_ID || l_attr_unit_pos == N2_CHIPLET_ID
                        || l_attr_unit_pos == N3_CHIPLET_ID)
                    {
                        nest_cplt_scomable = l_cplt_scomable;
                    }
                    else if (l_attr_unit_pos == XB_CHIPLET_ID)
                    {
                        xbus_cplt_scomable = l_cplt_scomable;
                    }
                    else if (l_attr_unit_pos == MC01_CHIPLET_ID || l_attr_unit_pos == MC23_CHIPLET_ID )
                    {
                        mc_cplt_scomable = l_cplt_scomable;
                    }
                    else if (l_attr_unit_pos == OB0_CHIPLET_ID || l_attr_unit_pos == OB1_CHIPLET_ID || l_attr_unit_pos == OB2_CHIPLET_ID
                             || l_attr_unit_pos == OB3_CHIPLET_ID)
                    {
                        obus_cplt_scomable = l_cplt_scomable;
                    }
                    else if (l_attr_unit_pos == PCI0_CHIPLET_ID || l_attr_unit_pos == PCI1_CHIPLET_ID
                             || l_attr_unit_pos == PCI2_CHIPLET_ID )
                    {
                        pcie_cplt_scomable = l_cplt_scomable;
                    }
                }
            }

            FAPI_DBG("p9_stopclocks : Chiplet scomable states \n\t tp_cplt_scomable = %d\n\t nest_cplt_scomable = %d\n\t xbus_cplt_scomable = %d\n\t mc_cplt_scomable = %d\n\t obus_cplt_scomable = %d\n\t pcie_cplt_scomable = %d\n",
                     tp_cplt_scomable, nest_cplt_scomable, xbus_cplt_scomable, mc_cplt_scomable,
                     obus_cplt_scomable, pcie_cplt_scomable);

            // Core stopclocks
            if(i_flags.stop_core_clks || sync_quad_stopclocks)
            {

                for (const auto& l_target_core : i_target_chip.getChildren<fapi2::TARGET_TYPE_CORE>(fapi2::TARGET_STATE_FUNCTIONAL))
                {
                    FAPI_INF("p9_stopclocks : Calling p9_hcd_core_stopclocks");
                    FAPI_TRY(p9_hcd_core_stopclocks(l_target_core, sync_quad_stopclocks));
                }
            }

            // L2 & Cache stopclocks
            if(i_flags.stop_cache_clks || sync_quad_stopclocks)
            {
                for (const auto& l_target_eq : i_target_chip.getChildren<fapi2::TARGET_TYPE_EQ>(fapi2::TARGET_STATE_FUNCTIONAL))
                {
                    FAPI_INF("p9_stopclocks : Calling p9_hcd_cache_stopclocks");
                    FAPI_TRY(p9_hcd_cache_stopclocks(l_target_eq, i_eq_clk_regions, i_ex_select, sync_quad_stopclocks));
                }
            }

            // Chiplet stopclocks
            FAPI_INF("p9_stopclocks : Call p9_cplt_stopclocks function");
            FAPI_TRY(p9_cplt_stopclocks(i_target_chip, (i_flags.stop_xbus_clks && xbus_cplt_scomable), (i_flags.stop_obus_clks
                                        && obus_cplt_scomable), (i_flags.stop_pcie_clks && pcie_cplt_scomable), (i_flags.stop_mc_clks && mc_cplt_scomable)));

            //Nest stopclocks
            if((i_flags.stop_nest_clks && nest_cplt_scomable))
            {
                FAPI_INF("p9_stopclocks : Call p9_nest_stopclocks function");
                FAPI_TRY(p9_nest_stopclocks(i_target_chip));
            }
        }

        // TP chiplet stopclocks
        if(((i_flags.stop_tp_clks && i_flags.stop_pib_clks) && tp_cplt_scomable) && !(pcb_is_bypassed == false
                && pcb_clks_are_off == true))
        {
            FAPI_INF("p9_stopclocks : Call p9_tp_stopclocks function");
            FAPI_TRY(p9_tp_stopclocks(i_target_chip, i_flags.stop_tp_clks, i_flags.stop_pib_clks));
        }
        else if((i_flags.stop_tp_clks && tp_cplt_scomable) && !(pcb_is_bypassed == false && pcb_clks_are_off == true))
        {
            FAPI_INF("p9_stopclocks : Call p9_tp_stopclocks function to stop only TP clocks");
            FAPI_TRY(p9_tp_stopclocks(i_target_chip, i_flags.stop_tp_clks, 0));
        }
        else if(i_flags.stop_pib_clks && !tp_vitl_clk_off && !tp_ep_rst && tp_mesh_clk_en)
        {
            FAPI_INF("p9_stopclocks : Call p9_tp_stopclocks function to stop only PIB clocks");
            FAPI_TRY(p9_tp_stopclocks(i_target_chip, 0, i_flags.stop_pib_clks));
        }
        else if(i_flags.stop_tp_clks || i_flags.stop_pib_clks)
        {
            FAPI_ERR("p9_stopclocks : Invalid condition to stop TP chiplet clocks\n\t TP_CPLT_SCOMABLE= %d, PCB_IS_BYPASSED=%d, PCB_CLKS_ARE_OFF=%d TP_MESH_CLK_EN=%d",
                     tp_cplt_scomable, pcb_is_bypassed, pcb_clks_are_off, tp_mesh_clk_en);
        }

        // Vital stopclocks
        if(i_flags.stop_vitl_clks)
        {
#ifdef __PPE__
            FAPI_ERR("p9_stopclocks : WARNING::VITAL clocks can't be stopped in SBE mode\n\t --> Skipping VITAL Stopclocks..! <--");
#else
            FAPI_INF("p9_stopclocks : Stopping Pervasive VITAL clocks");

            FAPI_DBG("Reading the current value of PERV_CTRL0 register");
            FAPI_TRY(fapi2::getCfamRegister(i_target_chip, PERV_PERV_CTRL0_FSI, l_cfam_data));

            l_cfam_data.setBit<PERV_PERV_CTRL0_TP_VITL_CLKOFF_DC>();

            FAPI_DBG("Writing the PERV_CTRL0 register TP_VITL_CLKOFF_DC=1");
            FAPI_TRY(fapi2::putCfamRegister(i_target_chip, PERV_PERV_CTRL0_FSI, l_cfam_data));

            //Raising Fence 4 & 5
            FAPI_INF("p9_stopclocks : Raising FSI fence 4 and 5");

            FAPI_DBG("Reading the current value of ROOT_CTRL0 register");
            FAPI_TRY(fapi2::getCfamRegister(i_target_chip, PERV_ROOT_CTRL0_FSI, l_cfam_data));

            l_cfam_data.setBit<PERV_ROOT_CTRL0_FENCE4_DC>();
            l_cfam_data.setBit<PERV_ROOT_CTRL0_FENCE5_DC>();

            FAPI_DBG("Writing the ROOT_CTRL0 register to raise FSI fence 4 and 5");
            FAPI_TRY(fapi2::putCfamRegister(i_target_chip, PERV_ROOT_CTRL0_FSI, l_cfam_data));
#endif
        }
    }

    FAPI_INF("p9_stopclocks : Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
