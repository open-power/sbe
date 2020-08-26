/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_cl2_l3_cleanup.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020                             */
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
/// @file  p10_cl2_l3_cleanup.C
/// @brief In mpipl down path verify cores chiplet state..if it is still running then
///        will put the core to stop 11 state
///
/// *HWP HW Owner        : Greg Still <stillgs@us.ibm.com>
/// *HWP FW Owner        : Prasad Bg Ranganath <prasadbgr@in.ibm.com>
/// *HWP Team            : PM
/// *HWP Level           : 2
/// *HWP Consumed by     : HB,PGPE,CME,OCC
///
// EKB-Mirror-To: hw/ppe

// -----------------------------------------------------------------------------
//  Includes
// -----------------------------------------------------------------------------
#include "p10_cl2_l3_cleanup.H"
#include "p10_hcd_cache_stopclocks.H"
#include "p10_hcd_core_poweroff.H"
#include "p10_hcd_l3_purge.H"
#include "p10_hcd_l2_purge.H"
#include "p10_hcd_powerbus_purge.H"
#include "p10_hcd_l2_tlbie_quiesce.H"
#include "p10_hcd_ncu_purge.H"
#include "p10_hcd_chtm_purge.H"
#include "p10_hcd_core_stopclocks.H"
#include "p10_hcd_cache_poweroff.H"
#include "p10_hcd_core_shadows_disable.H"
#include "p10_hcd_core_stopgrid.H"
#include <p10_scom_proc.H>
#include <p10_scom_c.H>
#include <p10_scom_eq.H>


using namespace scomt;
using namespace proc;
using namespace c;
using namespace eq;


#define CL2_START_POSITION 5
#define L3_START_POSITION 9
#define MMA_START_POSITION 15

fapi2::ReturnCode cleanup_cl2_l3_states(
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_core_target,
    uint8_t i_core_unit_pos);


fapi2::ReturnCode p10_cl2_l3_cleanup(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    FAPI_IMP (">>p10_cl2_l3_cleanup");
    uint8_t l_core_unit_pos;

    for (const auto& l_core : i_target.getChildren<fapi2::TARGET_TYPE_CORE>())
    {

        FAPI_TRY(FAPI_ATTR_GET( fapi2::ATTR_CHIP_UNIT_POS,
                                l_core,
                                l_core_unit_pos));
        FAPI_IMP("Core present but non functional %d",
                 l_core_unit_pos);

        FAPI_TRY(cleanup_cl2_l3_states(l_core, l_core_unit_pos));
    }

fapi_try_exit:
    FAPI_IMP("<< p10_cl2_l3_cleanup...");
    return fapi2::current_err;

}
fapi2::ReturnCode cleanup_cl2_l3_states(
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_core_target,
    uint8_t i_core_unit_pos)
{
    fapi2::buffer<uint64_t> l_data = 0;
    FAPI_IMP (">>cleanup_cl2_l3_states");

    uint64_t l_cl2_pfet_sense = 0;
    uint64_t l_l3_pfet_sense = 0;
    uint8_t l_core_relative_pos  = i_core_unit_pos % 4;
    uint8_t l_core_clock_State = 0;
    uint8_t l_l3_clock_State = 0;


    do
    {
        auto l_eq_target = i_core_target.getParent<fapi2::TARGET_TYPE_EQ>();

        //verify L3/core clocks are on
        FAPI_TRY(GET_CLOCK_STAT_SL(l_eq_target, l_data));

        l_core_clock_State = l_data.getBit(l_core_relative_pos + CL2_START_POSITION);
        l_l3_clock_State = l_data.getBit(l_core_relative_pos + L3_START_POSITION);
        FAPI_IMP("Core position %d CL2 state  %d L3 state %d", l_core_relative_pos,
                 l_core_clock_State, l_l3_clock_State);

        if (!l_core_clock_State)
        {
            //Read the power state of core/l2
            FAPI_TRY(GET_CPMS_CL2_PFETSTAT(i_core_target, l_data));
            GET_CPMS_CL2_PFETSTAT_VDD_PFETS_ENABLED_SENSE(l_data, l_cl2_pfet_sense);
        }

        if (!l_l3_clock_State)
        {
            //Read the power state of L3
            FAPI_TRY(GET_CPMS_L3_PFETSTAT(i_core_target, l_data));
            GET_CPMS_L3_PFETSTAT_VDD_PFETS_ENABLED_SENSE(l_data, l_l3_pfet_sense);
        }

        if ( l_core_clock_State && l_l3_clock_State)
        {
            FAPI_IMP("Both L3 and L2 units %d are powered off and clock off", i_core_unit_pos );
        }
        else
        {
            //Verify core is powered on
            //If core is powered on
            //  then if core(ECl2) clocks are on
            //      then need to purge the l2 and stop the core clocks
            //  if L3 clocks are on
            //      then purge L3 and stop the l3 clocks
            //  Power off the core and L3
            if( l_cl2_pfet_sense)
            {
                if (!l_core_clock_State)
                {
                    FAPI_TRY(p10_hcd_l2_purge(i_core_target));
                    FAPI_TRY(p10_hcd_l2_tlbie_quiesce(i_core_target));
                    FAPI_TRY(p10_hcd_ncu_purge(i_core_target));
                    FAPI_TRY(p10_hcd_core_shadows_disable(i_core_target));
                }
                else
                {
                    FAPI_IMP("Core clock %d is already stopped", i_core_unit_pos);
                }

            }

            if (l_l3_pfet_sense)
            {
                if (!l_l3_clock_State)
                {
                    FAPI_TRY(p10_hcd_chtm_purge(i_core_target));
                    FAPI_TRY(p10_hcd_l3_purge(i_core_target));
                    FAPI_TRY(p10_hcd_powerbus_purge(i_core_target));
                }
                else
                {
                    FAPI_IMP("Cache clock %d is already stopped", i_core_unit_pos);
                }

            }
        }

    }
    while(0);

fapi_try_exit:
    FAPI_IMP ("<<cleanup_cl2_l3_states");
    return fapi2::current_err;

}
