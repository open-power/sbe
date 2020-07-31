/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/hw540133.C $         */
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
//------------------------------------------------------------------------------
/// @file  hw540133.C
/// @brief Implementation of workaround for HW540133
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Joe McGill <jmcgill@us.ibm.com>
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : SBE, HB
//------------------------------------------------------------------------------

#include "hw540133.H"
#include "p10_scan_via_scom.H"

namespace hw540133
{

enum hw540133_PRIVATE_CONSTANTS
{
    MAX_PLLS = 5,
    CCALCOMP_MAX_POLLS = 6,
    CCALCOMP_DELAY_NS = 1000000,
    CCALCOMP_DELAY_CYCLES = 500000,
};

// Structure for reading/writing calibration status/settings
struct pll_cal
{
    union
    {
        bool calibrated;
        bool load;
    };
    uint8_t bandsel;
};

// Sort of a "scandef lite" which tells the procedure how many PLLs to check
// and where their calibration bits are on the ring
struct pll_ring_settings
{
    uint64_t scan_region_type;           ///< Scan region & type of affected ring
    uint16_t nplls;                      ///< Number of PLLs to check on that ring
    uint16_t result_offsets[MAX_PLLS];   ///< Rotate counts for the scanning functions below;
    uint16_t override_offsets[MAX_PLLS]; ///< one per PLL to get to the first/next PLL, plus one to finish the ring
};

const pll_ring_settings perv_plls =
{
    0x0002000000000080ULL,
    4,
    {  97, 194, 192, 192, 587 },
    { 249, 194, 192, 192, 435 }
};

const pll_ring_settings pci_plls =
{
    0x0002000000000080ULL,
    1,
    { 100, 208 },
    { 252,  56 }
};

const pll_ring_settings iohs_plls =
{
    0x0002000000000080ULL,
    1,
    {  98, 104 },
    { 146,  56 }
};

const pll_ring_settings mc_plls =
{
    0x0002000000000080ULL,
    2,
    {  98,  97, 106 },
    { 146,  97,  58 }
};

/// @brief Rotate across the PLL ring and scan out the BANDSEL and CCALCOMP bits
static inline fapi2::ReturnCode read_pll_cal_results(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_cplt,
    const uint64_t i_scan_region_type,
    uint32_t i_nplls,
    const uint16_t* i_ring_offsets,
    pll_cal* o_results)
{
    fapi2::buffer<uint64_t> tmp = 0;

    // Set up clock controller and write the header
    FAPI_TRY(fapi2::putScom(i_cplt, scomt::perv::SCAN_REGION_TYPE, i_scan_region_type));
    FAPI_TRY(svs::scan64_put(i_cplt, 0x00BAD666BAD66600, 0));

    // For each PLL, rotate to the next CCALCOMP and BANDSEL bits and read them
    while (i_nplls--)
    {
        FAPI_TRY(svs::rotate(i_cplt, *i_ring_offsets++, tmp));
        o_results->calibrated = !!(tmp & 0x8000000000000000);
        {
            uint64_t x = ((tmp & 0x7800000000000000) >> 59);
            o_results->bandsel = ((x & 0x8) >> 3) | ((x & 0x4) >> 1) | ((x & 0x2) << 1) | ((x & 0x1) << 3);
        }

        o_results++;
    }

    // Rotate once more to close the ring
    FAPI_TRY(svs::rotate(i_cplt, *i_ring_offsets, tmp));

    // Header check
    if (tmp != 0x00BAD666BAD66600)
    {
        FAPI_ERR("Header mismatch: %016llx != %016llx", 0x00BAD666BAD66600, tmp);
        return fapi2::FAPI2_RC_FALSE;
    }

    FAPI_TRY(fapi2::putScom(i_cplt, scomt::perv::SCAN_REGION_TYPE, 0));

fapi_try_exit:
    return fapi2::current_err;
}

/// @brief Poke CCALLOAD and CCALBANDSEL overrides into the PLL ring
static inline fapi2::ReturnCode write_pll_cal_overrides(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_cplt,
    const uint64_t i_scan_region_type,
    uint32_t i_nplls,
    const uint16_t* i_ring_offsets,
    const pll_cal* i_overrides)
{
    fapi2::buffer<uint64_t> tmp = 0;
    FAPI_TRY(fapi2::putScom(i_cplt, scomt::perv::SCAN_REGION_TYPE, i_scan_region_type));
    FAPI_TRY(svs::scan64_put(i_cplt, 0x00BAD666BAD66600, 0));

    while (i_nplls--)
    {
        FAPI_TRY(svs::rotate(i_cplt, *i_ring_offsets++, tmp));
        tmp &= 0x781fffffffffffffull;
        tmp |= i_overrides->load ? 0x8000000000000000 : 0;
        {
            uint64_t x = i_overrides->bandsel;
            tmp |= (((x & 0x0000000000000008ull) >> 3) << 55) | (((x & 0x0000000000000004ull) >> 2) << 56) |
                   (((x & 0x0000000000000002ull) >> 1) << 57) | (((x & 0x0000000000000001ull) >> 0) << 58);
        }
        FAPI_TRY(svs::scan64_put(i_cplt, tmp, 0));

        i_overrides++;
    }

    FAPI_TRY(svs::rotate(i_cplt, *i_ring_offsets, tmp));

    if (tmp != 0x00BAD666BAD66600)
    {
        FAPI_ERR("Header mismatch: %016llx != %016llx", 0x00BAD666BAD66600, tmp);
        return fapi2::FAPI2_RC_FALSE;
    }

    FAPI_TRY(fapi2::putScom(i_cplt, scomt::perv::SCAN_REGION_TYPE, 0));

fapi_try_exit:
    return fapi2::current_err;
}

fapi2::ReturnCode apply_workaround(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > & i_mcast_target,
    const pll_ring_settings& i_settings)
{
    using namespace scomt::perv;
    const uint64_t l_set_pulse_sl  = (i_settings.scan_region_type & 0x0FFFF00000000000ULL) | 0xC000000000008000ULL;
    const uint64_t l_set_pulse_nsl = (i_settings.scan_region_type & 0x0FFFF00000000000ULL) | 0xC000000000004000ULL;
    pll_cal l_bands[MAX_PLLS];

#ifdef __PPE__

    if (SBE::isSimicsRunning())
    {
        FAPI_INF("Skipping workaround because Simics does not support scanning");
        return fapi2::FAPI2_RC_SUCCESS;
    }

#endif

    FAPI_INF("Begin");

    for (auto l_chiplet_tgt : i_mcast_target.getChildren<fapi2::TARGET_TYPE_PERV>())
    {
        uint8_t l_uncalibrated = 0xFF;
        uint8_t l_calibration_poll_count = 0;

        while (l_uncalibrated)
        {
            // move PLL feedback into boundary ring (via SL clock) where it's observable
            FAPI_DBG("Isusing SL setpulse to fill bndy ring");
            FAPI_TRY(fapi2::putScom(l_chiplet_tgt, CLK_REGION, l_set_pulse_sl));

            // sample CCALCOMP to confirm PLL has resolved
            FAPI_DBG("Read bndy ring to sample CCALCOMP, BANDSEL");
            FAPI_TRY(read_pll_cal_results(l_chiplet_tgt,
                                          i_settings.scan_region_type,
                                          i_settings.nplls,
                                          i_settings.result_offsets,
                                          l_bands));

            l_uncalibrated = 0;

            for (int i = 0; i < i_settings.nplls; i++)
            {
                l_uncalibrated |= l_bands[i].calibrated ? 0 : (0x80 >> i);
            }

            FAPI_DBG("Poll: %d, NO CCALCOMP -> perv%d: 0x%02x",
                     l_calibration_poll_count,
                     l_chiplet_tgt.getChipletNumber(),
                     l_uncalibrated);

            // all calibrated, set CCBANDSEL based on PLL BANDSEL feedback
            if (l_uncalibrated == 0)
            {
                for (int i = 0; i < i_settings.nplls; i++)
                {
                    l_bands[i].load = true;
                    l_bands[i].bandsel = l_bands[i].bandsel ? (l_bands[i].bandsel - 1) : 1;
                }

                FAPI_DBG("Scan updates into bndy ring");

                FAPI_TRY(write_pll_cal_overrides(l_chiplet_tgt,
                                                 i_settings.scan_region_type,
                                                 i_settings.nplls,
                                                 i_settings.override_offsets,
                                                 l_bands));

                FAPI_DBG("Issuing NSL setpulse to update PLL");
                FAPI_TRY(fapi2::putScom(l_chiplet_tgt, CLK_REGION, l_set_pulse_nsl));

                break;
            }
            else
            {
                // bump poll count
                l_calibration_poll_count++;

                // check we haven't exceeded maximum poll limit
                FAPI_ASSERT(l_calibration_poll_count < CCALCOMP_MAX_POLLS,
                            fapi2::P10_SBE_CHIPLET_PLL_SETUP_CCALCOMP_TIMEOUT()
                            .set_TARGET(l_chiplet_tgt)
                            .set_UNCALIBRATED(l_uncalibrated),
                            "Polling timeout reached waiting for CCALCOMP on PLL!");

                // delay for PLL to resolve
                FAPI_TRY(fapi2::delay(CCALCOMP_DELAY_NS, CCALCOMP_DELAY_CYCLES));
            }
        }
    }

fapi_try_exit:
    FAPI_INF("End");
    return fapi2::current_err;
}
}
