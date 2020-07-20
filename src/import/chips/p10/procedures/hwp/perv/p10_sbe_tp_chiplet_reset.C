/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_tp_chiplet_reset.C $ */
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
/// @file  p10_sbe_tp_chiplet_reset.C
///
/// @brief Initial steps of PIB AND PCB
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------
//
#include "p10_sbe_tp_chiplet_reset.H"
#include "p10_scom_perv_0.H"
#include "p10_scom_perv_2.H"
#include "p10_scom_perv_4.H"
#include "p10_scom_perv_6.H"
#include "p10_scom_perv_7.H"
#include "p10_scom_perv_b.H"
#include "p10_scom_perv_c.H"
#include "p10_scom_perv_e.H"
#include "p10_scom_perv_f.H"
#include <p10_perv_sbe_cmn.H>
#include <target_filters.H>
#include <p10_hang_pulse_mc_setup_tables.H>

enum P10_SBE_TP_CHIPLET_RESET_Private_Constants
{
    PGOOD_REGIONS_STARTBIT = 4,
    PGOOD_REGIONS_LENGTH = 15,
    PGOOD_REGIONS_OFFSET = 12,
    REGIONS_NET = 0x0400,
    REGIONS_NET_PLL = 0x0410,
    SCAN_TYPES_EXCEPT_TIME_GPTR_REPR = 0xDCF,
    SCAN_TYPES_TIME_GPTR_REPR = 0x230,
    START_CMD = 0x1,
    CLOCK_TYPES_ALL = 0x7,
};

fapi2::ReturnCode p10_sbe_tp_chiplet_reset(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt;
    using namespace scomt::perv;

    const uint32_t BASE_ADDRESS = 0x000F0020;
    const uint8_t PRE_DIVIDER = 0x1;
    fapi2::buffer<uint16_t> l_regions;
    fapi2::buffer<uint64_t> l_data64, l_data64_root_ctrl0;
    fapi2::buffer<uint32_t> l_read_attr_pg;

    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_tpchiplet =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP, fapi2::TARGET_STATE_FUNCTIONAL)[0];

    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_n1 =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_NEST_SOUTH, fapi2::TARGET_STATE_FUNCTIONAL)[0];

    auto l_perv_mc_pci_pau = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(
                                 static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_MC |
                                         fapi2::TARGET_FILTER_ALL_PCI | fapi2::TARGET_FILTER_ALL_PAU ),
                                 fapi2::TARGET_STATE_FUNCTIONAL);

    FAPI_INF("p10_sbe_tp_chiplet_reset: Entering ...");

    FAPI_DBG("Release Nest/Cache clock DIV2 reset");
    l_data64.flush<0>().setBit<26>();
    FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL4_CLEAR_WO_CLEAR, l_data64));

    FAPI_DBG("Set up static power gating based on partial good info");
    l_data64.flush<0>();
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, l_n1, l_read_attr_pg));
    l_data64.writeBit<0>(!(l_read_attr_pg.getBit<17>())); // bit 0 = not (ATTR_PG(N1) region NMMU)

    for (auto& targ : l_perv_mc_pci_pau)
    {
        uint32_t l_chipletID = targ.getChipletNumber();

        // bit4..7 = MC3..0 cplts
        if(l_chipletID == 0xC)
        {
            l_data64.setBit<7>();
        }
        else  if(l_chipletID == 0xD)
        {
            l_data64.setBit<6>();
        }
        else  if(l_chipletID == 0xE)
        {
            l_data64.setBit<5>();
        }
        else  if(l_chipletID == 0xF)
        {
            l_data64.setBit<4>();
        }
        // bit16..17 = PCI1..0 cplts
        else if(l_chipletID == 0x8)
        {
            l_data64.setBit<17>();
        }
        else if(l_chipletID == 0x9)
        {
            l_data64.setBit<16>();
        }
        else // PAU
        {
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, targ, l_read_attr_pg));

            // bits 9,10 are unused in cplt_ctrl5
            if(l_chipletID == 0x10)
            {
                l_data64.writeBit<8>(!(l_read_attr_pg.getBit<13>())); // pau0 region
            }
            else if(l_chipletID == 0x11)
            {
                l_data64.writeBit<11>(!(l_read_attr_pg.getBit<13>())); // pau3 region
            }
            else if(l_chipletID == 0x12)
            {
                l_data64.writeBit<12>(!(l_read_attr_pg.getBit<13>())); // pau4 region
                l_data64.writeBit<13>(!(l_read_attr_pg.getBit<14>())); // pau5 region
            }
            else if(l_chipletID == 0x13)
            {
                l_data64.writeBit<14>(!(l_read_attr_pg.getBit<13>())); // pau6 region
                l_data64.writeBit<15>(!(l_read_attr_pg.getBit<14>())); // pau7 region
            }
        }
    }

    FAPI_TRY(fapi2::putScom(l_tpchiplet, CPLT_CTRL5_RW, l_data64));

    FAPI_DBG("Release Nest Async Reset to enable Nest clock mesh");
    l_data64.flush<0>().setBit<FSXCOMP_FSXLOG_ROOT_CTRL4_TP_AN_CLKGLM_NEST_ASYNC_RESET_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL4_CLEAR_WO_CLEAR, l_data64));

    // NET,PLL regions works with nest_gckn
    FAPI_DBG("Run scan0 module for NET and PLL regions, scan types GPTR, TIME, REPR");
    FAPI_TRY(p10_perv_sbe_cmn_scan0_module(l_tpchiplet, REGIONS_NET_PLL, SCAN_TYPES_TIME_GPTR_REPR));

    FAPI_DBG("Run scan0 module for NET and PLL regions, scan types except GPTR, TIME, REPR");
    FAPI_TRY(p10_perv_sbe_cmn_scan0_module(l_tpchiplet, REGIONS_NET_PLL, SCAN_TYPES_EXCEPT_TIME_GPTR_REPR));

    FAPI_DBG("Drop clock region fence for NET");
    l_data64.flush<0>()
    .setBit<CPLT_CTRL1_REGION4_FENCE_DC>();
    FAPI_TRY(fapi2::putScom(l_tpchiplet, CPLT_CTRL1_WO_CLEAR, l_data64));

    FAPI_DBG("Starting clock for NET");
    FAPI_TRY(p10_perv_sbe_cmn_clock_start_stop(l_tpchiplet, START_CMD, 0, 0, REGIONS_NET, CLOCK_TYPES_ALL));

    FAPI_DBG("Set PCB Reset");
    l_data64_root_ctrl0.flush<0>().setBit<FSXCOMP_FSXLOG_ROOT_CTRL0_PCB_RESET_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL0_SET_WO_OR,
                            l_data64_root_ctrl0));

    FAPI_DBG("switch pcb mux settings to intermediate state");
    l_data64_root_ctrl0.flush<0>().setBit<FSXCOMP_FSXLOG_ROOT_CTRL0_PCB2PCB_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL0_SET_WO_OR,
                            l_data64_root_ctrl0));

    FAPI_DBG("switch pcb mux settings to pcb2pcb path");
    l_data64_root_ctrl0.flush<0>()
    .setBit<FSXCOMP_FSXLOG_ROOT_CTRL0_FSI2PCB_DC>()
    .setBit<FSXCOMP_FSXLOG_ROOT_CTRL0_PIB2PCB_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL0_CLEAR_WO_CLEAR,
                            l_data64_root_ctrl0));

    FAPI_DBG("Release PCB Reset");
    l_data64_root_ctrl0.flush<0>().setBit<FSXCOMP_FSXLOG_ROOT_CTRL0_PCB_RESET_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL0_CLEAR_WO_CLEAR,
                            l_data64_root_ctrl0));

    FAPI_DBG("Drop Global Endpoint reset");
    l_data64_root_ctrl0.flush<0>().setBit<FSXCOMP_FSXLOG_ROOT_CTRL0_GLOBAL_EP_RESET_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL0_CLEAR_WO_CLEAR,
                            l_data64_root_ctrl0));

    FAPI_DBG("Drop OOB Mux");
    l_data64_root_ctrl0.flush<0>().setBit<FSXCOMP_FSXLOG_ROOT_CTRL0_OOB_MUX>();
    FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_ROOT_CTRL0_CLEAR_WO_CLEAR,
                            l_data64_root_ctrl0));

    FAPI_DBG("Transfer Perv pgood attribute into region good register(cplt_ctrl2 reg)");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, l_tpchiplet, l_read_attr_pg));
    l_data64.flush<0>();
    l_read_attr_pg.invert();
    l_data64.insert< PGOOD_REGIONS_STARTBIT, PGOOD_REGIONS_LENGTH, PGOOD_REGIONS_OFFSET >(l_read_attr_pg);
    FAPI_TRY(fapi2::putScom(l_tpchiplet, CPLT_CTRL2_RW, l_data64));

    FAPI_DBG("Enable PERV vital clock gating");
    l_data64.flush<0>().setBit<FSXCOMP_FSXLOG_PERV_CTRL0_TP_TCPERV_VITL_CG_DIS>();
    FAPI_TRY(fapi2::putScom(i_target_chip, FSXCOMP_FSXLOG_PERV_CTRL0_CLEAR_WO_CLEAR, l_data64));

    FAPI_DBG("Disable PERV align pulses");
    l_data64.flush<0>().setBit<CPLT_CTRL0_CTRL_CC_FORCE_ALIGN>();
    FAPI_TRY(fapi2::putScom(l_tpchiplet, CPLT_CTRL0_WO_CLEAR, l_data64));

    FAPI_DBG("Setup hang counters for Perv chiplet");
    FAPI_TRY(p10_perv_sbe_cmn_setup_hangpulse_counters(l_tpchiplet, false, BASE_ADDRESS, PRE_DIVIDER,
             SETUP_HANG_COUNTERS_PERV));

    FAPI_INF("p10_sbe_tp_chiplet_reset: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
