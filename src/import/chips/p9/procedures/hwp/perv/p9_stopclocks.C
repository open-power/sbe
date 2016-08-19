/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_stopclocks.C $     */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2019                        */
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
// *HWP Level           : 2
// *HWP Consumed by     : FSP:HB
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
#include <p9_check_chiplet_states.H>
#include <p9_hcd_common.H>

//------------------------------------------------------------------------------
// Function definition:  p9_stopclocks
// parameters: i_target                =>   chip target
//             i_stop_nest_clks        =>   True to stop NEST chiplet clocks                              (should default TRUE)
//             i_stop_mc_clks          =>   True to stop MC chiplet clocks                                (should default TRUE)
//             i_stop_xbus_clks        =>   True to stop XBUS chiplet clocks                              (should default TRUE)
//             i_stop_abus_clks        =>   True to stop OBUS chiplet clocks                              (should default TRUE)
//             i_stop_pcie_clks        =>   True to stop PCIE chiplet clocks                              (should default TRUE)
//             i_stop_tp_clks          =>   True to stop PERVASIVE (TP) chiplet clocks                    (should default FALSE)
//             i_stop_vitl_clks        =>   True to stop PERVASIVE VITL clocks                            (should default FALSE)
//             i_eq_clk_regions        =>   EQ chiplet clock regions of which clocks should be stopped    (default ALL_BUT_PLL_REFR)
//             i_ex_select             =>   EX chiplet selected for clocks stop                           (default BOTH_EX)
// returns: FAPI_RC_SUCCESS if operation was successful, else error
//------------------------------------------------------------------------------

fapi2::ReturnCode p9_stopclocks(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip,
                                const bool i_stop_nest_clks,
                                const bool i_stop_mc_clks,
                                const bool i_stop_xbus_clks,
                                const bool i_stop_obus_clks,
                                const bool i_stop_pcie_clks,
                                const bool i_stop_tp_clks,
                                const bool i_stop_vitl_clks,
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

    uint8_t l_cplt_scomable;

    bool pcb_is_bypassed      = false;
    bool pcb_clks_are_off     = false;
    bool perv_vitl_clks_off   = false;

    bool tp_cplt_scomable     = true;
    bool nest_cplt_scomable   = true;
    bool xbus_cplt_scomable   = true;
    bool mc_cplt_scomable     = true;
    bool obus_cplt_scomable   = true;
    bool pcie_cplt_scomable   = true;

    FAPI_INF("p9_stopclocks : Entering ...");

    FAPI_DBG("p9_stopclocks : Input arguments recieved are \n\t i_stop_nest_clks = %s\n\t i_stop_mc_clks = %s\n\t i_stop_xbus_clks = %s\n\t i_stop_obus_clks = %s\n\t i_stop_pcie_clks = %s\n\t i_stop_tp_clks = %s\n\t i_stop_vitl_clks =  %s\n",
             btos(i_stop_nest_clks), btos(i_stop_mc_clks), btos(i_stop_xbus_clks), btos(i_stop_obus_clks), btos(i_stop_pcie_clks),
             btos(i_stop_tp_clks), btos(i_stop_vitl_clks));
    FAPI_DBG("p9_stopclocks : Input CACHE arguments recieved are \n\t i_eq_clk_regions = %#018lx \n\t i_ex_select = %#018lx\n",
             (uint64_t)i_eq_clk_regions, (uint64_t)i_ex_select);

    FAPI_DBG("p9_stopclocks : Check to see if the Perv Vital clocks are OFF");
    FAPI_TRY(fapi2::getCfamRegister(i_target_chip, PERV_PERV_CTRL0_FSI, l_cfam_data));

    if(l_cfam_data.getBit<PERV_PERV_CTRL0_TP_VITL_CLKOFF_DC>())
    {
        FAPI_INF("p9_stopclocks : Perv Vital clocks are off, so stopclocks cant go ahead");
        perv_vitl_clks_off   = true;
    }
    else
    {
        perv_vitl_clks_off   = false;
    }

    if(!(perv_vitl_clks_off))
    {
        FAPI_DBG("p9_stopclocks : Check to see if the PIB/PCB network is being bypassed");
        FAPI_TRY(fapi2::getCfamRegister(i_target_chip, PERV_ROOT_CTRL0_FSI, l_cfam_data));

        if(l_cfam_data.getBit<PERV_ROOT_CTRL0_PIB2PCB_DC>())
        {
            FAPI_INF("p9_stopclocks : The PIB/PCB is being bypassed, so only the TP chiplet is accessible.");
            pcb_is_bypassed      = true;
        }
        else
        {
            pcb_is_bypassed      = false;
        }


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
            FAPI_INF("p9_stopclocks : At least one of the NET or PIB clocks is NOT running.  May not be able to use the PCB fabric to access chiplets.");
            pcb_clks_are_off      = true;
        }
        else
        {
            pcb_clks_are_off      = false;
        }


        if ( (pcb_is_bypassed == false) && (pcb_clks_are_off == false) )
        {
            FAPI_DBG("p9_stopclocks : Call p9_check_chiplet_states to get the state of chip");
            FAPI_TRY(p9_check_chiplet_states(i_target_chip));

            //To get the chiplet state from Attributes
            for (auto l_target_cplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
                 (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_TP | fapi2::TARGET_FILTER_ALL_MC | fapi2::TARGET_FILTER_ALL_NEST
                                                   | fapi2::TARGET_FILTER_ALL_OBUS | fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_XBUS),
                  fapi2::TARGET_STATE_FUNCTIONAL))
            {
                FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_TARGET_IS_SCOMMABLE, l_target_cplt, l_cplt_scomable));

                if(!(l_cplt_scomable))
                {
                    uint8_t l_attr_unit_pos = 0;
                    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_target_cplt, l_attr_unit_pos));

                    if (l_attr_unit_pos == 0x01)
                    {
                        tp_cplt_scomable = false;
                    }
                    else if (l_attr_unit_pos == 0x02 || l_attr_unit_pos == 0x03 || l_attr_unit_pos == 0x04 || l_attr_unit_pos == 0x05)
                    {
                        nest_cplt_scomable = false;
                    }
                    else if (l_attr_unit_pos == 0x06)
                    {
                        xbus_cplt_scomable = false;
                    }
                    else if (l_attr_unit_pos == 0x07 || l_attr_unit_pos == 0x08 )
                    {
                        mc_cplt_scomable = false;
                    }
                    else if (l_attr_unit_pos == 0x09 || l_attr_unit_pos == 0x0A || l_attr_unit_pos == 0x0B || l_attr_unit_pos == 0x0C)
                    {
                        obus_cplt_scomable = false;
                    }
                    else if (l_attr_unit_pos == 0x0D || l_attr_unit_pos == 0x0E || l_attr_unit_pos == 0x0F )
                    {
                        pcie_cplt_scomable = false;
                    }
                }
            }

            FAPI_DBG("p9_stopclocks : Chiplet scomable states \n\t tp_cplt_scomable = %s\n\t nest_cplt_scomable = %s\n\t xbus_cplt_scomable = %s\n\t mc_cplt_scomable = %s\n\t obus_cplt_scomable = %s\n\t pcie_cplt_scomable = %s\n",
                     btos(tp_cplt_scomable), btos(nest_cplt_scomable), btos(xbus_cplt_scomable), btos(mc_cplt_scomable),
                     btos(obus_cplt_scomable), btos(pcie_cplt_scomable));

            // Core stopclocks
            for (auto l_target_core : i_target_chip.getChildren<fapi2::TARGET_TYPE_CORE>(fapi2::TARGET_STATE_FUNCTIONAL))
            {
                FAPI_INF("p9_stopclocks : Calling p9_hcd_core_stopclocks");
                FAPI_TRY(p9_hcd_core_stopclocks(l_target_core));
            }

            // L2 & Cache stopclocks
            for (auto l_target_eq : i_target_chip.getChildren<fapi2::TARGET_TYPE_EQ>(fapi2::TARGET_STATE_FUNCTIONAL))
            {
                FAPI_INF("p9_stopclocks : Calling p9_hcd_cache_stopclocks");
                FAPI_TRY(p9_hcd_cache_stopclocks(l_target_eq, i_eq_clk_regions, i_ex_select));
            }

            // Chiplet stopclocks
            FAPI_INF("p9_stopclocks : Call p9_cplt_stopclocks function");
            FAPI_TRY(p9_cplt_stopclocks(i_target_chip, (i_stop_xbus_clks && xbus_cplt_scomable), (i_stop_obus_clks
                                        && obus_cplt_scomable), (i_stop_pcie_clks && pcie_cplt_scomable), (i_stop_mc_clks && mc_cplt_scomable)));

            //Nest stopclocks
            if((i_stop_nest_clks && nest_cplt_scomable))
            {
                FAPI_INF("p9_stopclocks : Call p9_nest_stopclocks function");
                FAPI_TRY(p9_nest_stopclocks(i_target_chip));
            }
        }

        // TP chiplet stopclocks
        if((i_stop_tp_clks && tp_cplt_scomable) && !(pcb_is_bypassed == false && pcb_clks_are_off == true))
        {
            FAPI_INF("p9_stopclocks : Call p9_tp_stopclocks function");
            FAPI_TRY(p9_tp_stopclocks(i_target_chip));
        }

        // Vital stopclocks
        if(i_stop_vitl_clks)
        {
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
        }
    }

    FAPI_INF("p9_stopclocks : Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
