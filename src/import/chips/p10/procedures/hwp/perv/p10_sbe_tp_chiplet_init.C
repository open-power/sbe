/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_tp_chiplet_init.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019                             */
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
/// @file  p10_sbe_tp_chiplet_init.C
///
/// @brief TP Chiplet Start Clocks
///        Starting clocks for all regions in perv chiplet other than pib and net
///        Clock test to check osc runnong
///        Configures pfet controls, tod errr regs , perv hang counters
///        pervasive LFIRS and set mask in IPOLL mask reg
///        Starts VREF calibration and checks for Calib Done
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------

#include "p10_sbe_tp_chiplet_init.H"

#include "p9_const_common.H"
#include <p9_misc_scom_addresses.H>
#include <p9_perv_scom_addresses.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p10_perv_sbe_cmn.H>
#include <target_filters.H>

enum P10_SBE_TP_CHIPLET_INIT_Private_Constants
{
    START_CMD = 0x1,
    REGIONS_ALL_EXCEPT_PIB_NET_PLL = 0x6B8F, // Exclude NEST_DPLL, PAU_DPLL
    CLOCK_TYPES_ALL = 0x7,
    REGIONS_PLL = 0x0010,
    CLOCK_TYPES_SL = 0x4,
    HW_NS_DELAY = 100000, // unit is nano seconds
    SIM_CYCLE_DELAY = 1000, // unit is sim cycles
    POLL_COUNT = 300,
    LFIR_ACTION0_VALUE = 0x0000000000000000,
    LFIR_ACTION1_VALUE = 0xFFFFBC2BFC7FFFFF,
    FIR_MASK_VALUE = 0x0000000000000000,
    IPOLL_MASK_VALUE = 0xFC00000000000000,
    TOD_ERROR_ROUTING = 0x9FC02000F0004000,
    TOD_ERROR_MASK = 0x0000000003F00002,
    TOD_ERROR_REG = 0xFFFFFFFFFFFFFFFF
};


static fapi2::ReturnCode p10_sbe_tp_chiplet_init_region_fence_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

fapi2::ReturnCode p10_sbe_tp_chiplet_init(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint64_t> l_data64;
    fapi2::buffer<uint64_t> l_regions;
    uint32_t base_address = 0x000D0070;
    uint8_t pre_divider;
    uint32_t l_attr_pau_freq_mhz;
    int l_timeout = 0;
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;

    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_tpchiplet = i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
            (fapi2::TARGET_FILTER_TP, fapi2::TARGET_STATE_FUNCTIONAL)[0];

    FAPI_INF("p10_sbe_tp_chiplet_init: Entering ...");

    // This step is redundant since tp_chiplet_reset already drops the OOB mux

    FAPI_DBG("Reset PCB Master Interrupt Register");
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_PIB_INTERRUPT_TYPE_REG, 0x0));

    FAPI_DBG("Clear pervasive chiplet region fence");
    FAPI_TRY(p10_sbe_tp_chiplet_init_region_fence_setup(l_tpchiplet));

    FAPI_DBG("Start clocks for all regions except Pib, Net and Pll ");
    FAPI_TRY(p10_perv_sbe_cmn_clock_start_stop(l_tpchiplet, START_CMD, 0, 0, REGIONS_ALL_EXCEPT_PIB_NET_PLL,
             CLOCK_TYPES_ALL));

    // startclocks for pll  - This is no longer necessary for P10

    FAPI_DBG("Drop FSI fence 5");
    l_data64.flush<0>().setBit<PERV_ROOT_CTRL0_SET_FENCE5_DC>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_ROOT_CTRL0_CLEAR_SCOM, l_data64));

    FAPI_DBG("Set TOD error routing register");
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TOD_ERROR_ROUTING_REG, TOD_ERROR_ROUTING));
    //config TOD error mask reg;
    FAPI_DBG("Configure TOD error mask register");
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TOD_ERROR_MASK_REG, TOD_ERROR_MASK));
    //clear TOD error reg;
    FAPI_DBG("Clear TOD error register");
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TOD_ERROR_REG, TOD_ERROR_REG));

    FAPI_DBG("Clear pervasive LFIR");
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_LOCAL_FIR_AND, 0));

    FAPI_DBG("Configure pervasive LFIR" );
    //Setting LOCAL_FIR_ACTION0 register value
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_LOCAL_FIR_ACTION0,
                            LFIR_ACTION0_VALUE));
    //Setting LOCAL_FIR_ACTION1 register value
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_LOCAL_FIR_ACTION1,
                            LFIR_ACTION1_VALUE));
    //Setting LOCAL_FIR_MASK register value
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_LOCAL_FIR_MASK, FIR_MASK_VALUE));

    // Enables any checkstop if set, to propogate to FSP and get notified
    FAPI_DBG("Unmask CFIR Mask");
    //Setting FIR_MASK register value
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_FIR_MASK, FIR_MASK_VALUE));

    FAPI_DBG("Setup IPOLL mask register");
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_HOST_MASK_REG, IPOLL_MASK_VALUE));

    // setup hang pulse freq
    FAPI_DBG("Setup constant freq hangpulses");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FREQ_PAU_MHZ, FAPI_SYSTEM, l_attr_pau_freq_mhz));
    pre_divider =  ((l_attr_pau_freq_mhz + 125) / 250);
    FAPI_TRY(p10_perv_sbe_cmn_setup_hangpulse_counters(l_tpchiplet, base_address, pre_divider));

    FAPI_DBG("Start  calibration");
    //Setting KVREF_AND_VMEAS_MODE_STATUS_REG register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TP_KVREF_AND_VMEAS_MODE_STATUS_REG, l_data64));
    l_data64.setBit<PERV_1_KVREF_AND_VMEAS_MODE_STATUS_REG_START_CAL>();
    FAPI_TRY(fapi2::putScom(i_target_chip, PERV_TP_KVREF_AND_VMEAS_MODE_STATUS_REG, l_data64));

    FAPI_DBG("Check for calibration done");
    l_timeout = POLL_COUNT;

    //UNTIL KVREF_AND_VMEAS_MODE_STATUS_REG.KVREF_CAL_DONE == 1
    while (l_timeout != 0)
    {
        //Getting KVREF_AND_VMEAS_MODE_STATUS_REG register value
        FAPI_TRY(fapi2::getScom(i_target_chip, PERV_TP_KVREF_AND_VMEAS_MODE_STATUS_REG, l_data64));
        bool l_poll_data = l_data64.getBit<PERV_1_KVREF_AND_VMEAS_MODE_STATUS_REG_CAL_DONE>();

        if (l_poll_data == 1)
        {
            break;
        }

        fapi2::delay(HW_NS_DELAY, SIM_CYCLE_DELAY);
        --l_timeout;
    }

    FAPI_DBG("Loop Count :%d", l_timeout);

    FAPI_ASSERT(l_timeout > 0,
                fapi2::KVREF_CAL_NOT_DONE_ERR()
                .set_PERV_TP_KVREF_AND_VMEAS_MODE_STATUS_REG(l_data64)
                .set_LOOP_COUNT(l_timeout)
                .set_HW_DELAY(HW_NS_DELAY),
                "ERROR: Calibration not done, bit16 not set");

    FAPI_INF("p10_sbe_tp_chiplet_init: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}


/// @brief region fence setup
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p10_sbe_tp_chiplet_init_region_fence_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    // Local variable and constant definition
    fapi2::buffer <uint32_t> l_attr_pg;
    fapi2::buffer <uint16_t> l_attr_pg_data;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p10_sbe_tp_chiplet_init_region_fence_setup: Entering ...");

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PG, i_target_chiplet, l_attr_pg));

    l_attr_pg.invert();
    l_attr_pg.extractToRight<12, 15>(l_attr_pg_data);

    FAPI_DBG("Drop partial good fences");
    //Setting CPLT_CTRL1 register value
    l_data64.flush<0>();
    l_data64.writeBit<PERV_1_CPLT_CTRL1_TC_VITL_REGION_FENCE>(l_attr_pg.getBit<11>());
    l_data64.insertFromRight<4, 15>(l_attr_pg_data);
    FAPI_TRY(fapi2::putScom(i_target_chiplet, PERV_CPLT_CTRL1_CLEAR, l_data64));

    FAPI_INF("p10_sbe_tp_chiplet_init_region_fence_setup: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
