/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_dts_init.C $ */
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
///
/// @file p10_sbe_dts_init.C
/// @brief Run the initization sequence on the DTS macros on the chip
///
/// *HWP HWP Owner: Greg Still <stillgs@us.ibm.com>
/// *HWP FW Owner: Prem Jha <premjha1@in.ibm.com>
/// *HWP Team: PM
/// *HWP Consumed by: SBE
///
/// High-level procedure flow:
///
/// @endverbatim

// -----------------------------------------------------------------------------
//  Includes
// -----------------------------------------------------------------------------
#include "p10_sbe_dts_init.H"
#include <multicast_group_defs.H>

#include "p10_scom_proc.H"
#include "p10_scom_perv.H"

// -----------------------------------------------------------------------------
//  Definitions
// -----------------------------------------------------------------------------

// The following are the sample times targed for DTS updates based on a 16MHz
// input reference.
enum SAMPLE_TIMES
{
    S16MS   = 0b0000,
    S8MS    = 0b0001,
    S4MS    = 0b0010,
    S2MS    = 0b0011,
    S1MS    = 0b0100,
    S0P5MS  = 0b0101,
    S250US  = 0b0110,
    S125US  = 0b0111,
    S62P5US = 0b1000,
    S31P3US = 0b1001,
    S15P6US = 0b1010,
    S7P8US  = 0b1011,
    S3P9US  = 0b1100,
    S2US    = 0b1101,
    S1US    = 0b1110,
    S0P5US  = 0b1111
};

static const uint32_t l_sample_time = S15P6US;

// -----------------------------------------------------------------------------
//  Function prototypes
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
//  Function definitions]");
// -----------------------------------------------------------------------------

// See .H for documentation
fapi2::ReturnCode p10_sbe_dts_init(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{

    FAPI_INF("> p10_sbe_dts_init");

    fapi2::buffer<uint64_t> l_data64;

    auto l_eq_mc =
        i_target.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_ALL_EQ);
    auto l_pau_mc =
        i_target.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_PAU);

    // Enable Thermal Macros in chiplets that have DTSs via the Therm Mode Reg.
    // Setup the macros up to sample the DTSs on a defined time scale and enable
    // the sensors on each loop that have DTSs attached.

    {
        using namespace scomt::perv;

        FAPI_DBG("Enable Core/Cache/Racetrack DTSs in EQs");

        // Core 0 on Loop1 (3 sensors), Core 1 on Loop2 (3 sensors), Racetrack on Loop3 (1 sensor)
        l_data64 = 0;
        FAPI_TRY(PREP_EPS_THERM_WSUB_MODE_REG(l_eq_mc));
        SET_EPS_THERM_WSUB_MODE_REG_THERM_DTS_SAMPLE_ENA(l_data64);
        SET_EPS_THERM_WSUB_MODE_REG_THERM_SAMPLE_PULSE_CNT(l_sample_time, l_data64);
        SET_EPS_THERM_WSUB_MODE_REG_THERM_DTS_ENABLE_L1(0b111, l_data64);
        SET_EPS_THERM_WSUB_MODE_REG_THERM_DTS_ENABLE_L2(0b111, l_data64);
        SET_EPS_THERM_WSUB_MODE_REG_THERM_DTS_ENABLE_L3(l_data64);
        FAPI_TRY(fapi2::putScom(l_eq_mc, EPS_THERM_WSUB_MODE_REG,  l_data64));

        // Core 2 on Loop1 (3 sensors), Core 3 on Loop2 (3 sensors), Loop 3 not used
        l_data64 = 0;
        FAPI_TRY(PREP_EPS_THERM_WSUB2_MODE_REG(l_eq_mc));
        SET_EPS_THERM_WSUB2_MODE_REG_THERM_DTS_SAMPLE_ENA(l_data64);
        SET_EPS_THERM_WSUB2_MODE_REG_THERM_SAMPLE_PULSE_CNT(l_sample_time, l_data64);
        SET_EPS_THERM_WSUB2_MODE_REG_THERM_DTS_ENABLE_L1(0b111, l_data64);
        SET_EPS_THERM_WSUB2_MODE_REG_THERM_DTS_ENABLE_L2(0b111, l_data64);
        FAPI_TRY(fapi2::putScom(l_eq_mc, EPS_THERM_WSUB2_MODE_REG,  l_data64));

    } // namespace

    {
        using namespace scomt::perv;

        FAPI_DBG("Enable Endcap DTSs in the PAUs");

        l_data64 = 0;
        FAPI_TRY(PREP_EPS_THERM_WSUB_MODE_REG(l_pau_mc));
        SET_EPS_THERM_WSUB_MODE_REG_THERM_DTS_SAMPLE_ENA(l_data64);
        SET_EPS_THERM_WSUB_MODE_REG_THERM_SAMPLE_PULSE_CNT(l_sample_time, l_data64);
        SET_EPS_THERM_WSUB_MODE_REG_THERM_DTS_ENABLE_L1(0b100, l_data64);
        FAPI_TRY(fapi2::putScom(l_pau_mc, EPS_THERM_WSUB_MODE_REG,  l_data64));

    } // namespace

    {
        using namespace scomt::proc;

        FAPI_DBG("Enable Endcap DTSs in the NO and N1");

        // N0 on Loop 1 (1 sensor)
        l_data64 = 0;
        FAPI_TRY(PREP_TP_TCN0_N0_EPS_THERM_WSUB_MODE_REG(i_target));
        SET_TP_TCN0_N0_EPS_THERM_WSUB_MODE_REG_THERM_DTS_SAMPLE_ENA(l_data64);
        SET_TP_TCN0_N0_EPS_THERM_WSUB_MODE_REG_THERM_SAMPLE_PULSE_CNT(l_sample_time, l_data64);
        SET_TP_TCN0_N0_EPS_THERM_WSUB_MODE_REG_THERM_DTS_ENABLE_L1(0b100, l_data64);
        FAPI_TRY(fapi2::putScom(i_target, TP_TCN0_N0_EPS_THERM_WSUB_MODE_REG, l_data64));

        // N1 on Loop 1 (1 sensor)
        l_data64 = 0;
        FAPI_TRY(PREP_TP_TCN1_N1_EPS_THERM_WSUB_MODE_REG(i_target));
        SET_TP_TCN1_N1_EPS_THERM_WSUB_MODE_REG_THERM_DTS_SAMPLE_ENA(l_data64);
        SET_TP_TCN1_N1_EPS_THERM_WSUB_MODE_REG_THERM_SAMPLE_PULSE_CNT(l_sample_time, l_data64);
        SET_TP_TCN1_N1_EPS_THERM_WSUB_MODE_REG_THERM_DTS_ENABLE_L1(0b100, l_data64);
        FAPI_TRY(fapi2::putScom(i_target, TP_TCN1_N1_EPS_THERM_WSUB_MODE_REG, l_data64));

    } // namespace

fapi_try_exit:
    FAPI_INF("< p10_sbe_dts_init");
    return fapi2::current_err;
} // END p10_sbe_dts_init
