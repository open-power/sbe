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
    LOCK_MAX_POLLS = 6,
    LOCK_DELAY_NS = 5000000,
    LOCK_DELAY_CYCLES = 500000,
    BANDSEL_MIN = 0,
    BANDSEL_MAX = 15,
};

enum hw540133_wa_type
{
    NO_WORKAROUND,             // accept HW calibration
    CALIBRATED_BANDSEL_M1,     // force calibration to calibrated bandsel-1 (+1 if zero)
    LAST_LOCKING_BANDSEL_M1,   // force calibration to last locking bandsel-1
};

// Structure for reading/writing calibration status/settings
struct pll_cal
{
    bool calibrated;
    bool load;
    uint8_t bandsel;
    bool locked;
};

// Sort of a "scandef lite" which tells the procedure how many PLLs to check
// and where their calibration bits are on the ring
struct pll_ring_settings
{
    hw540133_wa_type wa_type;            ///< Workaround algorithm to apply
    uint64_t scan_region_type;           ///< Scan region & type of affected ring
    uint16_t nplls;                      ///< Number of PLLs to check on that ring
    uint16_t result_offsets[MAX_PLLS];   ///< Rotate counts for the scanning functions below;
    uint16_t override_offsets[MAX_PLLS]; ///< one per PLL to get to the first/next PLL, plus one to finish the ring
    uint8_t  lock_bit[MAX_PLLS];         ///< PCB slave lock indicator bits (last entry unused)
};

const pll_ring_settings perv_plls =
{
    CALIBRATED_BANDSEL_M1,
    0x0002000000000080ULL,
    4,
    {  97, 194, 192, 192, 587 },         ///< TOD, NEST, IOSS, IO filter PLLs
    { 249, 194, 192, 192, 435 },
    {   2,   3,   5,   4,   8 },
};

const pll_ring_settings pci_plls =
{
    CALIBRATED_BANDSEL_M1,
    0x0002000000000080ULL,
    1,
    { 100, 208 },                        ///< PCI filter PLL
    { 252,  56 },
    {   0,   8 },
};

const pll_ring_settings iohs_plls =
{
    CALIBRATED_BANDSEL_M1,
    0x0002000000000080ULL,
    1,
    {  98, 104 },                        ///< IOHS tank PLL
    { 146,  56 },
    {   0,   8 },
};

const pll_ring_settings mc_plls =
{
    LAST_LOCKING_BANDSEL_M1,
    0x0002000000000080ULL,
    2,
    {  98,  97, 106 },                   ///< MC tank PLLs (odd, even)
    { 146,  97,  58 },
    {   1,   0,   8 },
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

/// @brief Process all PLLs in ring to confirm calibration (CCALCOMP) and sample resolved band (BANDSEL)
fapi2::ReturnCode confirm_calibration(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target,
    const pll_ring_settings& i_settings,
    pll_cal* o_bands)
{
    using namespace scomt::perv;

    const uint64_t l_set_pulse_sl  = (i_settings.scan_region_type & 0x0FFFF00000000000ULL) | 0xC000000000008000ULL;

    uint8_t l_uncalibrated = 0xFF;
    uint8_t l_calibration_poll_count = 0;

    FAPI_DBG("Start");

    while (l_uncalibrated)
    {
        // move PLL feedback into boundary ring (via SL clock) where it's observable
        FAPI_DBG("Issuing SL setpulse to fill bndy ring");
        FAPI_TRY(fapi2::putScom(i_target, CLK_REGION, l_set_pulse_sl));

        // delay for PLL to resolve
        FAPI_TRY(fapi2::delay(CCALCOMP_DELAY_NS, CCALCOMP_DELAY_CYCLES));

        // sample CCALCOMP to confirm PLL has resolved
        FAPI_DBG("Read bndy ring to sample CCALCOMP, BANDSEL");
        FAPI_TRY(read_pll_cal_results(i_target,
                                      i_settings.scan_region_type,
                                      i_settings.nplls,
                                      i_settings.result_offsets,
                                      o_bands));

        l_uncalibrated = 0;

        for (int i = 0; i < i_settings.nplls; i++)
        {
            l_uncalibrated |= o_bands[i].calibrated ? 0 : (0x80 >> i);
        }

        FAPI_DBG("Poll: %d, NO CCALCOMP -> perv 0x%02X: 0x%02x",
                 l_calibration_poll_count,
                 i_target.getChipletNumber(),
                 l_uncalibrated);

        // all calibrated
        if (l_uncalibrated == 0)
        {
            break;
        }
        else
        {
            // bump poll count
            l_calibration_poll_count++;

            // check we haven't exceeded maximum poll limit
            FAPI_ASSERT(l_calibration_poll_count < CCALCOMP_MAX_POLLS,
                        fapi2::P10_HW540133_CCALCOMP_TIMEOUT()
                        .set_TARGET(i_target)
                        .set_UNCALIBRATED(l_uncalibrated),
                        "Polling timeout reached waiting for CCALCOMP on PLL!");
        }
    }

fapi_try_exit:
    FAPI_DBG("End");
    return fapi2::current_err;
}

fapi2::ReturnCode force_band(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target,
    const pll_ring_settings& i_settings,
    pll_cal* io_bands)
{
    using namespace scomt::perv;

    const uint64_t l_set_pulse_nsl = (i_settings.scan_region_type & 0x0FFFF00000000000ULL) | 0xC000000000004000ULL;

    uint8_t l_unlocked = 0xFF;
    uint8_t l_lock_poll_count = 0;

    FAPI_DBG("Start");

    FAPI_DBG("Scan updates into bndy ring");
    FAPI_TRY(write_pll_cal_overrides(i_target,
                                     i_settings.scan_region_type,
                                     i_settings.nplls,
                                     i_settings.override_offsets,
                                     io_bands));

    FAPI_DBG("Issuing NSL setpulse to update PLL");
    FAPI_TRY(fapi2::putScom(i_target, CLK_REGION, l_set_pulse_nsl));

    while (l_unlocked)
    {
        l_unlocked = 0;

        // delay for PLLs to lock
        FAPI_TRY(fapi2::delay(LOCK_DELAY_NS, LOCK_DELAY_CYCLES));

        fapi2::buffer<uint64_t> l_pll_lock_reg = 0;

        // read HW, update unlocked vector
        FAPI_TRY(fapi2::getScom(i_target, PLL_LOCK_REG, l_pll_lock_reg));

        for (int i = 0; i < i_settings.nplls; i++)
        {
            io_bands[i].locked = l_pll_lock_reg.getBit(i_settings.lock_bit[i]) ? true : false;
            l_unlocked |= (!io_bands[i].locked ? (0x80 >> i) : 0);
        }

        FAPI_DBG("Poll: %d, NO LOCK -> perv 0x%02X: 0x%02x",
                 l_lock_poll_count,
                 i_target.getChipletNumber(),
                 l_unlocked);

        if (l_unlocked == 0)
        {
            break;
        }
        else
        {
            // bump poll count
            l_lock_poll_count++;

            // return fapi2::FAPI2_RC_FALSE if we have exceeded polling limit
            // caller must handle
            if (l_lock_poll_count == LOCK_MAX_POLLS)
            {
                FAPI_DBG("Polling timeout reached waiting for LOCK on PLL!");
                return fapi2::FAPI2_RC_FALSE;
            }
        }
    }

fapi_try_exit:
    FAPI_DBG("End");
    return fapi2::current_err;
}

fapi2::ReturnCode apply_workaround(
    const fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > & i_mcast_target,
    const pll_ring_settings& i_settings)
{
    using namespace scomt::perv;

    FAPI_INF("Begin");

    fapi2::ATTR_IS_SIMULATION_Type l_attr_is_simulation = 0;
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;

#ifdef __PPE__

    if (SBE::isSimicsRunning())
    {
        FAPI_INF("Skipping workaround because Simics does not support scanning");
        goto fapi_try_exit;
    }

#endif

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_IS_SIMULATION, FAPI_SYSTEM, l_attr_is_simulation));

    if (l_attr_is_simulation)
    {
        FAPI_INF("Skipping workaround in simulation");
        goto fapi_try_exit;
    }

    for (auto l_chiplet_tgt : i_mcast_target.getChildren<fapi2::TARGET_TYPE_PERV>())
    {
        fapi2::ReturnCode l_rc;
        pll_cal l_bands[MAX_PLLS];

        for (int i = 0; i < i_settings.nplls; i++)
        {
            l_bands[i].calibrated = false;
            l_bands[i].load = false;
            l_bands[i].locked = false;
            l_bands[i].bandsel = 0;
        }

        // confirm that PLL calibrates, return band selected by HW calibration
        FAPI_TRY(confirm_calibration(l_chiplet_tgt, i_settings, l_bands),
                 "Error from confirm_calibration");

        FAPI_DBG("Base calibration (BANDSEL):");

        for (int i = 0; i < i_settings.nplls; i++)
        {
            FAPI_DBG("PLL %d: %d", i, l_bands[i].bandsel);
        }

        switch (i_settings.wa_type)
        {
            case NO_WORKAROUND:
                FAPI_DBG("Skipping all workarounds");
                break;

            case CALIBRATED_BANDSEL_M1:
                FAPI_DBG("Executing CALIBRATED_BANDSEL_M1 workaround");

                // set CCBANDSEL based on PLL BANDSEL feedback
                for (int i = 0; i < i_settings.nplls; i++)
                {
                    l_bands[i].load = true;
                    l_bands[i].bandsel = ((l_bands[i].bandsel == BANDSEL_MIN) ? 1 : (l_bands[i].bandsel - 1));
                }

                l_rc = force_band(l_chiplet_tgt, i_settings, l_bands);

                FAPI_ASSERT(l_rc == fapi2::FAPI2_RC_SUCCESS,
                            fapi2::P10_HW540133_CBM1_WA_ERR()
                            .set_TARGET(l_chiplet_tgt)
                            .set_LOCK_ERR(l_rc == fapi2::FAPI2_RC_FALSE),
                            "Error from force_band (CALIBRATED_BANDSEL_M1, chiplet: %02d)",
                            l_chiplet_tgt.getChipletNumber());

                break;

            case LAST_LOCKING_BANDSEL_M1:
                FAPI_DBG("Executing LAST_LOCKING_BANDSEL_M1 workaround");

                for (int i = 0; i < i_settings.nplls; i++)
                {
                    // execute workaround sequence on only one PLL in each ring at a time
                    // keep all previously adjusted PLLs in the ring with 'load' set to
                    // maintain resolved setpoint
                    FAPI_DBG("Adjusting PLL: %d", i);

                    for (int j = 0; j <= i; j++)
                    {
                        l_bands[j].load = true;
                    }

                    // confirm that PLL locks just forcing calibrated band
                    l_rc = force_band(l_chiplet_tgt, i_settings, l_bands);
                    FAPI_ASSERT(l_rc == fapi2::FAPI2_RC_SUCCESS,
                                fapi2::P10_HW540133_LLBM1_WA_ERR()
                                .set_TARGET(l_chiplet_tgt)
                                .set_LOCK_ERR(l_rc == fapi2::FAPI2_RC_FALSE)
                                .set_FIRST_ATTEMPT(true)
                                .set_INTERMEDIATE_STEP(false)
                                .set_LAST_ATTEMPT(false),
                                "Error from force_band (LAST_LOCKING_BANDSEL_M1, chiplet: %02d), first lock attempt",
                                l_chiplet_tgt.getChipletNumber());

                    FAPI_DBG("Locked when forced to calibrated band (%d)", l_bands[i].bandsel);

                    // step bandsel up until we fail to lock
                    while (l_bands[i].bandsel < BANDSEL_MAX)
                    {
                        l_bands[i].bandsel++;
                        FAPI_DBG("Attempting lock at bandsel %d...", l_bands[i].bandsel);
                        l_rc = force_band(l_chiplet_tgt, i_settings, l_bands);

                        if (l_rc == fapi2::FAPI2_RC_SUCCESS)
                        {
                            FAPI_DBG("Lock successful at bandsel: %d, stepping up", l_bands[i].bandsel);
                            continue;
                        }
                        else if (l_rc == fapi2::FAPI2_RC_FALSE)
                        {
                            FAPI_DBG("Lock failed at at bandsel: %d, backing off", l_bands[i].bandsel);
                            break;
                        }

                        FAPI_ASSERT(false,
                                    fapi2::P10_HW540133_LLBM1_WA_ERR()
                                    .set_TARGET(l_chiplet_tgt)
                                    .set_LOCK_ERR(l_rc == fapi2::FAPI2_RC_FALSE)
                                    .set_FIRST_ATTEMPT(false)
                                    .set_INTERMEDIATE_STEP(true)
                                    .set_LAST_ATTEMPT(false),
                                    "Error from force_band (LAST_LOCKING_BANDSEL_M1, chiplet: %02d), step sequence",
                                    l_chiplet_tgt.getChipletNumber());
                    }

                    // if last adjustment failed to lock, back off by two
                    if (l_rc == fapi2::FAPI2_RC_FALSE)
                    {
                        if (l_bands[i].bandsel < 2)
                        {
                            l_bands[i].bandsel = 0;
                        }
                        else
                        {
                            l_bands[i].bandsel -= 2;
                        }
                    }

                    FAPI_DBG("Confirming final band selection (%d)", l_bands[i].bandsel);
                    l_rc = force_band(l_chiplet_tgt, i_settings, l_bands);
                    FAPI_ASSERT(l_rc == fapi2::FAPI2_RC_SUCCESS,
                                fapi2::P10_HW540133_LLBM1_WA_ERR()
                                .set_TARGET(l_chiplet_tgt)
                                .set_LOCK_ERR(l_rc == fapi2::FAPI2_RC_FALSE)
                                .set_FIRST_ATTEMPT(false)
                                .set_INTERMEDIATE_STEP(false)
                                .set_LAST_ATTEMPT(true),
                                "Error from force_band (LAST_LOCKING_BANDSEL_M1, chiplet: %02d), last lock attempt",
                                l_chiplet_tgt.getChipletNumber());
                }

                break;

            default:
                FAPI_ASSERT(false,
                            fapi2::P10_HW540133_WORKAROUND_ERR()
                            .set_TARGET(l_chiplet_tgt)
                            .set_WORKAROUND_TYPE(i_settings.wa_type),
                            "Unsupported HW540133 workaround selection!");
        }
    }

fapi_try_exit:
    FAPI_INF("End");
    return fapi2::current_err;
}

}
