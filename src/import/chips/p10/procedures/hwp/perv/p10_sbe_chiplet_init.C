/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_chiplet_init.C $ */
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
/// @file  p10_sbe_chiplet_init.C
///
/// @brief init procedure for all enabled chiplets
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------

#include "p10_sbe_chiplet_init.H"
#include "p10_scom_perv_0.H"
#include "p10_scom_proc_f.H"
#include <target_filters.H>
#include <multicast_group_defs.H>

enum P10_SBE_CHIPLET_INIT_Private_Constants
{
    HW_NS_DELAY = 100000, // unit is nano seconds
    SIM_CYCLE_DELAY = 1000, // unit is sim cycles
    POLL_COUNT = 300
};

fapi2::ReturnCode p10_sbe_chiplet_init(const
                                       fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    using namespace scomt;

    fapi2::buffer<uint64_t> l_data64;
    int l_timeout = 0;
    fapi2::buffer<uint8_t> l_attr_topology_id, l_attr_topology_mode;
    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;

    FAPI_INF("p10_sbe_chiplet_init: Entering..");

    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_tpchiplet =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP, fapi2::TARGET_STATE_FUNCTIONAL)[0];

    auto l_mc_all    = i_target_chip.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_NO_TP);

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_TOPOLOGY_ID, i_target_chip, l_attr_topology_id));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_TOPOLOGY_MODE, FAPI_SYSTEM, l_attr_topology_mode));

    //setup fabric topology into cplt_config reg for all good chiplets
    FAPI_DBG("Setup fabric topology for all chiplets");
    l_data64.flush<0>();
    l_data64.insertFromRight<perv::CPLT_CONF0_TC_TOPOLOGY_ID_DC, perv::CPLT_CONF0_TC_TOPOLOGY_ID_DC_LEN>
    (l_attr_topology_id);
    l_data64.writeBit<47>(l_attr_topology_mode);
    // All good but TP
    FAPI_TRY(fapi2::putScom(l_mc_all, perv::CPLT_CONF0_WO_OR, l_data64));
    // Only TP
    FAPI_TRY(fapi2::putScom(l_tpchiplet, perv::CPLT_CONF0_WO_OR, l_data64));

    FAPI_DBG("Start  calibration");
    //Setting KVREF_AND_VMEAS_MODE_STATUS_REG register value
    FAPI_TRY(fapi2::getScom(i_target_chip, proc::TP_TPCHIP_TPC_ITR_FMU_KVREF_AND_VMEAS_MODE_STATUS_REG, l_data64));
    l_data64.setBit<proc::TP_TPCHIP_TPC_ITR_FMU_KVREF_AND_VMEAS_MODE_STATUS_REG_KVREF_START_CAL>();
    FAPI_TRY(fapi2::putScom(i_target_chip, proc::TP_TPCHIP_TPC_ITR_FMU_KVREF_AND_VMEAS_MODE_STATUS_REG, l_data64));

    FAPI_DBG("Check for calibration done");
    l_timeout = POLL_COUNT;

    //UNTIL KVREF_AND_VMEAS_MODE_STATUS_REG.KVREF_CAL_DONE == 1
    while (l_timeout != 0)
    {
        //Getting KVREF_AND_VMEAS_MODE_STATUS_REG register value
        FAPI_TRY(fapi2::getScom(i_target_chip, proc::TP_TPCHIP_TPC_ITR_FMU_KVREF_AND_VMEAS_MODE_STATUS_REG, l_data64));
        bool l_poll_data = l_data64.getBit<proc::TP_TPCHIP_TPC_ITR_FMU_KVREF_AND_VMEAS_MODE_STATUS_REG_KVREF_CAL_DONE>();

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

    FAPI_INF("p10_sbe_chiplet_init: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
