/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/pm/p10_query_corecachemma_access_state.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2022                        */
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
// @file  p10_query_corecachemma_access_state.C
/// @brief Check the power state and clock status to set boolean scanable,
//  scomable parameters
///
/// HWP Owner       : Manish Chowdhary <manichow@in.ibm.com>
/// Backup HWP Owner: Rajees Rehman <stillgs@us.ibm.com>
/// FW Owner        :
/// Team            :
/// Level           : 2
/// Consumed by     : FSP:SBE
///
/// @endverbatim
///
//------------------------------------------------------------------------------

// ----------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------
#include "p10_query_corecachemma_access_state.H"
#include "p10_hcd_common.H"
#include "p10_scom_eq_a.H"
#include "p10_scom_eq_6.H"
#include "p10_scom_eq_c.H"
#include "p10_scom_c.H"

using namespace scomt::c;
using namespace scomt::eq;

enum
{
    CL2 = 0,
    L3 = 1,
    MMA = 2,
    QME = 3,
    MAX_UNITS = 4,
};
void update_scanState (fapi2::buffer<uint32_t>& o_scanState, uint32_t i_scomGrpState, uint32_t i_clockState,
                       uint32_t i_fenceState, uint32_t i_corePowerState);
//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------
#define CORE_START_POSITION 5
#define CACHE_START_POSITION 9
#define MMA_START_POSITION 15
#define QME_BIT_POS 13

// ----------------------------------------------------------------------
// Procedure Function
// ----------------------------------------------------------------------
fapi2::ReturnCode p10_query_corecachemma_access_state(
    const fapi2::Target < fapi2::TARGET_TYPE_EQ >& i_target,
    scomStatus_t& o_scomStateData,
    scanStatus_t& o_scanStateData,
    bool i_scanClockState
)
{
    //Initialize the output variables
    o_scomStateData.scomState = 0;
    o_scanStateData.scanState = 0;

    fapi2::buffer<uint64_t> l_scomData = 0;
    fapi2::buffer<uint32_t> l_scanStateData = 0;
    uint32_t l_clockState = 0;
    uint32_t l_fenceState = 0;
    uint32_t l_pgState = 0;
    uint32_t l_pscomState = 0;
    uint8_t l_attr_chip_unit_pos = 0;
    uint8_t l_core_num = 0;
    uint32_t l_pfet_senses = 0;
    uint32_t l_corePowerState = BIT32(QME_BIT_POS);
    fapi2::ReturnCode l_rc = fapi2::FAPI2_RC_SUCCESS;

    FAPI_INF("> p10_query_access_state..");

    //Get all the child cores of input EQ target
    const auto& v_Cores = i_target.getChildren<fapi2::TARGET_TYPE_CORE>(fapi2::TARGET_STATE_FUNCTIONAL);

    for (auto const& l_core : v_Cores)
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               l_core,
                               l_attr_chip_unit_pos));
        l_core_num = (uint8_t)(l_attr_chip_unit_pos % 4);

        //Read the power state of core/L2, L3 and MMA. If an unit is powered
        //off then it cannot be scaned or scomed. However, we would read the
        //clock state register available on quad region immaterial of whether
        //unit is having power or not, quad is always on.

        //Read the power state of core/l2
        l_rc = fapi2::getScom(l_core, CPMS_CL2_PFETSTAT, l_scomData);

        if (l_rc)
        {
            FAPI_ERR("getScom error: Core %d, addr 0x%.16llX, skip this core.", l_core_num, CPMS_CL2_PFETSTAT);
            continue;
        }

        l_scomData.extractToRight(l_pfet_senses, 0, 32);

        if( l_pfet_senses & BIT32(CPMS_CL2_PFETSTAT_VDD_PFETS_ENABLED_SENSE))
        {
            l_scanStateData.setBit(CORE_START_POSITION + l_core_num);
            l_corePowerState |= BIT32(CORE_START_POSITION + l_core_num);
        }

        //Read the power state of l3
        l_rc = fapi2::getScom(l_core, CPMS_L3_PFETSTAT, l_scomData);

        if (l_rc)
        {
            FAPI_ERR("getScom error: Core %d, addr 0x%.16llX, skip this core.", l_core_num, CPMS_L3_PFETSTAT);
            continue;
        }

        l_scomData.extractToRight(l_pfet_senses, 0, 32);

        if( l_pfet_senses & BIT32(CPMS_L3_PFETSTAT_VDD_PFETS_ENABLED_SENSE))
        {
            l_scanStateData.setBit(CACHE_START_POSITION + l_core_num);
            l_corePowerState |= BIT32(CACHE_START_POSITION + l_core_num);
        }

        //Read the power state of mma
        l_rc = fapi2::getScom(l_core, CPMS_MMA_PFETSTAT, l_scomData);

        if (l_rc)
        {
            FAPI_ERR("getScom error: Core %d, addr 0x%.16llX, skip this core.", l_core_num, CPMS_MMA_PFETSTAT);
            continue;
        }

        l_scomData.extractToRight(l_pfet_senses, 0, 32);

        if( l_pfet_senses & BIT32(CPMS_MMA_PFETSTAT_S_ENABLED_SENSE))
        {
            l_scanStateData.setBit(MMA_START_POSITION + l_core_num);
            l_corePowerState |= BIT32(MMA_START_POSITION + l_core_num);
        }

        //Read Clock grid control status data and if Bit 11 is not set, then
        //core rings can't be read
        l_rc = fapi2::getScom(l_core, CPMS_CGCSR, l_scomData);

        if (l_rc)
        {
            FAPI_ERR("getScom error: Core %d, addr 0x%.16llX, skip this core.", l_core_num, CPMS_CGCSR);
            continue;
        }

        if (!l_scomData.getBit<CPMS_CGCSR_CL2_CLKGLM_SEL>())
        {
            l_scanStateData.clearBit(CORE_START_POSITION + l_core_num);
        }
    }


    //Save the data, here so that any scom fails above will not skip saving the
    //data that was captured for the previous cores
    o_scanStateData.scanState = l_scanStateData;
    FAPI_INF("QUAD Status : scan access state(0x%08X)",
             o_scanStateData.scanState);

    // A region is scomable if following four factors are fulfilled
    // 1) Clock should be on for that region
    // 2) The region must not be fenced
    // 3) Region should be partial good
    // 4) PSCOM logic should be enabled
    //
    FAPI_TRY(getScom(i_target, CLOCK_STAT_SL, l_scomData));
    l_scomData.extractToRight(l_clockState, 0, 32);
    //Clock status bit off indicates clock is running

    FAPI_TRY(getScom(i_target, CPLT_CTRL1_RW, l_scomData));
    l_scomData.extractToRight(l_fenceState, 0, 32);
    //Fence bits on means region is fenced

    FAPI_TRY(getScom(i_target, CPLT_CTRL2_RW, l_scomData));
    l_scomData.extractToRight(l_pgState, 0, 32);
    //PG good bit on means region is marked as partial good

    FAPI_TRY(getScom(i_target, CPLT_CTRL3_RW, l_scomData));
    l_scomData.extractToRight(l_pscomState, 0, 32);
    //pscom bit is on means pscom is enabled

    o_scomStateData.scomState = (~l_clockState) & (~l_fenceState) & (l_pgState) & (l_pscomState);

    //The clock state needs to be checked only for the dump case
    if (!i_scanClockState)
    {
        update_scanState(l_scanStateData, o_scomStateData.scomState, l_clockState, l_fenceState, l_corePowerState);

        o_scanStateData.scanState = l_scanStateData;
        FAPI_INF("QUAD Status : scan access state(0x%08X)",
                 o_scanStateData.scanState);
    }

#ifndef __PPE__
    FAPI_INF("QUAD Status : clock state(0x%08X), fence state(0x%08X), "
             "pgood state(0x%08X), pscom state(0x%08X),scom state(0x%08X)",
             l_clockState,
             l_fenceState,
             l_pgState,
             l_pscomState,
             o_scomStateData.scomState);
#endif

fapi_try_exit:
    FAPI_INF("< p10_query_access_state...");
    fapi2::current_err = fapi2::FAPI2_RC_SUCCESS;
    return fapi2::current_err;
}

void update_scanState (fapi2::buffer<uint32_t>& o_scanState, uint32_t i_scomGrpState, uint32_t i_clockState,
                       uint32_t i_fenceState, uint32_t i_corePowerState)
{
    uint32_t pos = 0;
    uint32_t max_units = 0;

    for ( uint32_t x = 0; x < MAX_UNITS; x++)
    {
        switch (x)
        {
            case CL2:
                pos = CORE_START_POSITION;
                max_units = 4;
                break;

            case L3:
                pos = CACHE_START_POSITION;
                max_units = 4;
                break;

            case MMA:
                pos = MMA_START_POSITION;
                max_units = 4;
                break;

            case QME:
                pos = QME_BIT_POS;
                max_units = 1;
                break;
        }

        for (auto i = pos; i < (pos + max_units); i++)
        {
            if ( i_corePowerState & BIT32(i))
            {
                if ( i_scomGrpState & BIT32(i))
                {
                    o_scanState.clearBit(i);
                }
                else
                {
                    if ( (i_clockState & i_fenceState) & BIT32(i) )
                    {
                        o_scanState.setBit(i);
                    }
                    else
                    {
                        o_scanState.clearBit(i);
                    }
                }
            }
        }
    }
}
