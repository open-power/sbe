/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_select_ex.C $ */
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
/// @file p10_sbe_select_ex.C
/// @brief Select the Hostboot core(s) from the available cores on the chip
///
/// *HWP HWP Owner: Greg Still <stillgs@us.ibm.com>
/// *HWP FW Owner: Prem Jha <premjha1@in.ibm.com>
/// *HWP Team: PM
/// *HWP Consumed by: SBE
///
/// High-level procedure flow:
/// @verbatim
///
///   Prerequisite:  MCGROUP_GOOD_EQ has been setup in all EQ chiplets
///
/// Two modes are supported:
/// - SINGLE: select 1 core based on type (normal or fused) and then 8MB of
///   backing cache (2 normal or fused)
/// - ALL: select all good cores
///
/// In both modes, the OCC registers are written with the valid configuration.
/// Additionally, default PFET controller delays are written into all good
/// cores so that future power-on operations will succeed.
///
/// Attributes control the procedure behavior
///     ATTR_ACTIVE_CORES_NUM:      required number of active NORMAL cores
///       for fused mode, this number / 2 is the number of fused cores required
///       for fused mode, this number must be even
///     ATTR_BACKING_CACHES_NUM:    required number of 4MB backing MB caches
///     ATTR_ACTIVE_CORES_VEC:      bit vector of active cores (overridable attribute)
///     ATTR_BACKING_CACHES_VEC:    bit vector of backing caches (overridable attribute)
///     ATTR_FUSED_MODE:            needed for error checking
///
/// num_active = 0
/// num_backing = 0
/// fused_first_half = false;
/// active_config = 0
/// backing_config = 0
/// cores_config = 0    // superset of active_configured and backing_configured
/// Loop over functional cores
///     if num_active < ATTR_ACTIVE_CORES_NUM
///         if fused_mode
///             if fused_first_half
///                 ASSERT(core_num == first_core_num+1)
///                 fused_first_half = false
///             else
///                 fused_first_half = true
///                 fused_first_core = core_num
///         set bit c in core_config buffer
///         set bit c in active_config buffer
///         configure partial good and multicast(core)
///         ++num_active
///         next
///     if num_backing < ATTR_BACKING_CACHES_NUM
///         set bit c in core_config buffer
///         set bit c in backing_config buffer
///         configure partial good and multicast(core)
///         ++num_backing
///         next
///
/// ATTR_SET(ATTR_ACTIVE_CORES_VEC, active_config)
/// ATTR_GET(ATTR_ACTIVE_CORES_VEC, active_config)     // May return override
///
/// ATTR_SET(ATTR_BACKING_CACHES_VEC, backing_config)
/// ATTR_GET(ATTR_BACKING_CACHES_VEC, backing_config)  // May return override
///
/// needed_config = active_config | backing_config
/// ASSERT(needed_config & config_cores == needed_config)
///
/// Write resultant scoreboard Core mask into OCC complex
///   - This is the "master record " of the enabled cores/quad in the system
///   - This is only for during the IPL (will be updated later in step 15)
/// Set default PFET controller delay values into Core
/// @endverbatim

// -----------------------------------------------------------------------------
//  Includes
// -----------------------------------------------------------------------------
#include "p10_sbe_select_ex.H"
#include "p10_hcd_common.H"
#include <multicast_group_defs.H>

#include "p10_scom_proc.H"
#include "p10_scom_eq.H"
#include "p10_scom_c.H"

// -----------------------------------------------------------------------------
//  Definitions
// -----------------------------------------------------------------------------

const static uint32_t NUM_OF_QUADS = 8;

// -----------------------------------------------------------------------------
//  Function prototypes
// -----------------------------------------------------------------------------

fapi2::ReturnCode select_ex_calc_active_backing_nums(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
    const std::vector<fapi2::Target<fapi2::TARGET_TYPE_CORE>>& i_core_functional_vector,
    fapi2::ATTR_ACTIVE_CORES_NUM_Type& o_active_cores_num,
    fapi2::ATTR_ACTIVE_CORES_NUM_Type& o_backing_caches_num);

fapi2::ReturnCode select_ex_pfet_delay(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);

fapi2::ReturnCode select_ex_config(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_eq_target,
    const uint32_t i_core_num);

// -----------------------------------------------------------------------------
//  Function definitions
// -----------------------------------------------------------------------------

// See .H for documentation
fapi2::ReturnCode p10_sbe_select_ex(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
    const selectex::MODE i_mode)
{
    using namespace scomt::proc;

    FAPI_INF("> p10_sbe_select_ex");

    fapi2::buffer<uint32_t> l_core_config_bvec = 0;
    fapi2::buffer<uint32_t> l_active_config_bvec = 0;
    fapi2::buffer<uint32_t> l_backing_config_bvec = 0;
    fapi2::buffer<uint32_t> l_needed_config_bvec = 0;
    fapi2::buffer<uint64_t> l_data64;

    fapi2::ATTR_CONTAINED_IPL_TYPE_Type l_attr_contained_ipl_type;
    fapi2::ATTR_ACTIVE_CORES_NUM_Type l_attr_num_active = 0;
    fapi2::ATTR_BACKING_CACHES_NUM_Type l_attr_num_backing = 0;
    uint32_t l_num_active = 0;
    uint32_t l_num_backing = 0;
    bool b_master_found = false;
    bool b_fused = false;
    bool b_fused_first_half = false;
    uint32_t l_fused_core_num_base = 0;

    fapi2::ATTR_ACTIVE_CORES_VEC_Type l_attr_active_cores_bvec;
    fapi2::ATTR_BACKING_CACHES_VEC_Type l_attr_backing_caches_bvec;

    fapi2::Target<fapi2::TARGET_TYPE_EQ> l_eq_target;

    const fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    auto l_core_functional_vector = i_target.getChildren<fapi2::TARGET_TYPE_CORE>
                                    (fapi2::TARGET_STATE_FUNCTIONAL);


    fapi2::ATTR_FUSED_CORE_MODE_Type l_attr_fused_mode;
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FUSED_CORE_MODE,
                           FAPI_SYSTEM,
                           l_attr_fused_mode));

    if (l_attr_fused_mode == fapi2::ENUM_ATTR_FUSED_CORE_MODE_CORE_FUSED)
    {
        b_fused = true;
        FAPI_DBG("Fused core mode detected");

        // Check that we're not trying to force fused cores on a chip where the
        // force mechanism is disabled via eFuses AND we are not already in
        // fused mode

        // @todo RTC 207903
        // Update the following P9 fields with the P10 variants
//         fapi2::buffer<uint64_t> l_perv_ctrl0;
//         fapi2::buffer<uint64_t> l_device_id_reg;
//
//         FAPI_TRY(fapi2::getScom(i_target, PERV_PERV_CTRL0_SCOM, l_perv_ctrl0));
//         FAPI_TRY(fapi2::getScom(i_target, PERV_DEVICE_ID_REG, l_device_id_reg));
//
//         FAPI_ASSERT(!(l_perv_ctrl0.getBit<P9N2_PERV_PERV_CTRL0_TP_OTP_SCOM_FUSED_CORE_MODE>()
//                       && l_device_id_reg.getBit<P9N2_PERV_DEVICE_ID_REG_HW_MODE_SEL>() &&
//                       !l_device_id_reg.getBit<P9N2_PERV_DEVICE_ID_REG_TP_EX_FUSE_SMT8_CTYPE_EN>()),
//                     fapi2::SBE_SELECT_EX_FORCE_FUSED_CORES_DISABLED(),
//                     "Failed to force fused core mode because external control has been disabled via eFuses");
    }

    {
        FAPI_TRY(select_ex_calc_active_backing_nums(i_target,
                 l_core_functional_vector,
                 l_attr_num_active,
                 l_attr_num_backing),
                 "Error from select_ex_calc_active_backing_nums");

        FAPI_DBG("ATTR_ACTIVE_CORES_NUM   = %d", l_attr_num_active);
        FAPI_DBG("ATTR_BACKING_CACHES_NUM = %d", l_attr_num_backing);

        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_ACTIVE_CORES_NUM, i_target, l_attr_num_active),
                 "Error from FAPI_ATTR_SET (ATTR_ACTIVE_CORES_NUM)");
        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_BACKING_CACHES_NUM, i_target, l_attr_num_backing),
                 "Error from FAPI_ATTR_SET (ATTR_BACKING_CACHES_NUM)");
    }

    // Loop through the core functional vector on the view that the FAPI
    // platform code is expected to return the vector elements in ascending
    // order.
    //
    // For non-fused mode, the 3 cores in the vector is going to be the
    // hostboot cores.
    //
    // For fused core mode, the first core must be even numbered and then
    // the companion odd numbered one must also be in the vector. This is repeated
    // for subsequent cores so that we have 2 fused cores to have the necessary
    // back cache.
    //
    // If the first or third core in the vector is odd or the
    // companion odd core to the first and second even one is not present, raise
    // errors.
    //
    for (auto const& core : l_core_functional_vector)
    {
        fapi2::ATTR_CHIP_UNIT_POS_Type l_attr_chip_unit_pos = 0;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               core,
                               l_attr_chip_unit_pos));

        uint32_t l_core_num  = (uint32_t)l_attr_chip_unit_pos;

        l_eq_target = core.getParent<fapi2::TARGET_TYPE_EQ>();

        if (b_fused)
        {
            if (b_fused_first_half)
            {
                // Check that the second normal core is functional.
                FAPI_DBG("Odd core check: base core == %d, core num = %d",
                         l_fused_core_num_base, l_core_num);
                FAPI_ASSERT(l_core_num == l_fused_core_num_base + 1,
                            fapi2::SBE_SELECT_EX_FUSED_ODD_ERROR()
                            .set_CHIP(i_target)
                            .set_CORE_NUM(l_core_num)
                            .set_FUSED_CORE_NUM_BASE(l_fused_core_num_base),
                            "Odd core within fused set is not functional");
                b_fused_first_half = false;
            }
            else
            {
                // Check that the first normal core is even.
                FAPI_ASSERT(l_core_num % 2 == 0,
                            fapi2::SBE_SELECT_EX_FUSED_NOT_EVEN_ERROR()
                            .set_CHIP(i_target)
                            .set_CORE_NUM(l_core_num),
                            "The first core found in fused mode was not an even core");
                b_fused_first_half = true;
                l_fused_core_num_base = l_core_num;
            }
        }

        if (!b_master_found)
        {
            b_master_found = true;
            FAPI_TRY(FAPI_ATTR_SET( fapi2::ATTR_MASTER_CORE,
                                    i_target,
                                    l_attr_chip_unit_pos));
            FAPI_DBG("Set ATTR_MASTER_CORE        => %d 0x%X",
                     l_attr_chip_unit_pos, l_attr_chip_unit_pos);
        }

        if (l_num_active < l_attr_num_active)
        {
            l_core_config_bvec.setBit(l_core_num);
            l_active_config_bvec.setBit(l_core_num);
            FAPI_TRY(select_ex_config(l_eq_target, l_core_num));
            ++l_num_active;
            continue;  // next core
        }

        if (l_num_backing < l_attr_num_backing)
        {
            l_core_config_bvec.setBit(l_core_num);
            l_backing_config_bvec.setBit(l_core_num);
            FAPI_TRY(select_ex_config(l_eq_target, l_core_num));
            ++l_num_backing;
            continue;  // next core
        }

        // Exit loop if the required numbers have been achieved
        if (l_num_active == l_attr_num_active &&
            l_num_backing == l_attr_num_backing)
        {
            FAPI_DBG("Found everything needed.  Breaking loop");
            break;
        }

        FAPI_DBG("Scoreboard values for OCC: Core 0x%08X", l_core_config_bvec);

    } // Core loop

    FAPI_DBG("Active core check: num_active %d attr_num_active %d", l_num_active, l_attr_num_active);
    FAPI_ASSERT(l_num_active == l_attr_num_active,
                fapi2::SBE_SELECT_EX_INSUFFICIENT_ACTIVE_CORES_ERROR()
                .set_CHIP(i_target)
                .set_CORE_CONFIG(l_core_config_bvec)
                .set_ATTR_ACTIVE_CORES_NUM(l_attr_num_active)
                .set_ACTIVE_CORES_NUM(l_num_active)
                .set_ACTIVE_CORES_VEC(l_active_config_bvec),
                "Insufficient active cores found");

    FAPI_DBG("Backing check: num_backing %d attr_num_backing %d", l_num_backing, l_attr_num_backing);
    FAPI_ASSERT(l_num_backing == l_attr_num_backing,
                fapi2::SBE_SELECT_EX_INSUFFICIENT_BACKING_CACHES_ERROR()
                .set_CHIP(i_target)
                .set_CORE_CONFIG(l_core_config_bvec)
                .set_ATTR_ACTIVE_CORES_NUM(l_attr_num_active)
                .set_ATTR_BACKING_CACHES_NUM(l_attr_num_backing)
                .set_ACTIVE_CORES_NUM(l_num_active)
                .set_BACKING_CACHES_NUM(l_num_backing)
                .set_ACTIVE_CORES_VEC(l_active_config_bvec)
                .set_BACKING_CACHES_VEC(l_backing_config_bvec),
                "Insufficient backing caches found");


    // get chip contained specific attribute overrides for active/backing setup
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CONTAINED_IPL_TYPE, FAPI_SYSTEM, l_attr_contained_ipl_type),
             "Error from FAPI_ATTR_GET (ATTR_CONTAINED_IPL_TYPE)");

    if (l_attr_contained_ipl_type == fapi2::ENUM_ATTR_CONTAINED_IPL_TYPE_CHIP)
    {
        FAPI_TRY(FAPI_ATTR_GET( fapi2::ATTR_CHIP_CONTAINED_ACTIVE_CORES_VEC,
                                i_target,
                                l_attr_active_cores_bvec));

        FAPI_TRY(FAPI_ATTR_GET( fapi2::ATTR_CHIP_CONTAINED_BACKING_CACHES_VEC,
                                i_target,
                                l_attr_backing_caches_bvec));
    }
    else
    {
        l_attr_active_cores_bvec = l_active_config_bvec;
        l_attr_backing_caches_bvec = l_backing_config_bvec;
    }

    // Set active cores attribute
    FAPI_TRY(FAPI_ATTR_SET( fapi2::ATTR_ACTIVE_CORES_VEC,
                            i_target,
                            l_attr_active_cores_bvec));
    FAPI_DBG("Set ATTR_ACTIVE_CORES_VEC   => 0x%08X", l_attr_active_cores_bvec);

    // Set backing caches attribute
    FAPI_TRY(FAPI_ATTR_SET( fapi2::ATTR_BACKING_CACHES_VEC,
                            i_target,
                            l_attr_backing_caches_bvec));
    FAPI_DBG("Set ATTR_BACKING_CACHES_VEC => 0x%08X", l_attr_backing_caches_bvec);

    l_needed_config_bvec = l_attr_active_cores_bvec | l_attr_backing_caches_bvec;

    FAPI_ASSERT((l_needed_config_bvec & l_core_config_bvec) == l_needed_config_bvec,
                fapi2::SBE_SELECT_EX_NEEDED_CONFIG_ERROR()
                .set_CHIP(i_target)
                .set_NEEDED_CONFIG(l_needed_config_bvec)
                .set_CORE_CONFIG(l_core_config_bvec)
                .set_ATTR_ACTIVE_CORES_NUM(l_attr_num_active)
                .set_ATTR_BACKING_CACHES_NUM(l_attr_num_backing)
                .set_ACTIVE_CORES_NUM(l_num_active)
                .set_BACKING_CACHES_NUM(l_num_backing)
                .set_ACTIVE_CORES_VEC(l_active_config_bvec)
                .set_BACKING_CACHES_VEC(l_backing_config_bvec),
                "Need configuration not satisfied");

    // Setup all Cores PFET delays
    FAPI_TRY(select_ex_pfet_delay(i_target));

    // Write to the OCC Core Configuration Status Register
    l_data64.flush<0>().insertFromRight<0, 32>(l_core_config_bvec);
    FAPI_TRY(fapi2::putScom(i_target, TP_TPCHIP_OCC_OCI_OCB_CCSR_RW, l_data64));
    FAPI_DBG("Write OCI CCSR to 0x%16llX", l_data64);

fapi_try_exit:
    FAPI_INF("< p10_sbe_select_ex");
    return fapi2::current_err;
} // END p10_sbe_select_ex


///-----------------------------------------------------------------------------
/// @brief Compute the number of active cores/backing caches to be configured
///        by SBE, based on IPL type/parameters
///
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode select_ex_calc_active_backing_nums(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
    const std::vector<fapi2::Target<fapi2::TARGET_TYPE_CORE>>& i_core_functional_vector,
    fapi2::ATTR_ACTIVE_CORES_NUM_Type& o_active_cores_num,
    fapi2::ATTR_ACTIVE_CORES_NUM_Type& o_backing_caches_num)
{
    FAPI_DBG("> select_ex_calc_active_backing_nums...");

    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    fapi2::ATTR_CONTAINED_IPL_TYPE_Type l_attr_contained_ipl_type;
    fapi2::ATTR_IS_MPIPL_Type l_attr_is_mpipl;
    fapi2::ATTR_FUSED_CORE_MODE_Type l_attr_fused_core_mode;
    fapi2::ATTR_CHIP_CONTAINED_ACTIVE_CORES_VEC_Type l_attr_chip_contained_active_cores_vec;
    fapi2::ATTR_CHIP_CONTAINED_BACKING_CACHES_VEC_Type l_attr_chip_contained_backing_caches_vec;
    fapi2::ATTR_SBE_SELECT_EX_POLICY_Type l_attr_sbe_select_ex_policy;

    uint8_t l_functional_cores_num = i_core_functional_vector.size();
    o_active_cores_num = 0;
    o_backing_caches_num = 0;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CONTAINED_IPL_TYPE, FAPI_SYSTEM, l_attr_contained_ipl_type),
             "Error from FAPI_ATTR_GET (ATTR_CONTAINED_IPL_TYPE)");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_IS_MPIPL, FAPI_SYSTEM, l_attr_is_mpipl),
             "Error from FAPI_ATTR_GET (ATTR_IS_MPIPL)");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_FUSED_CORE_MODE, FAPI_SYSTEM, l_attr_fused_core_mode),
             "Error from FAPI_ATTR_GET (ATTR_FUSED_CORE_MODE)");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SBE_SELECT_EX_POLICY, FAPI_SYSTEM, l_attr_sbe_select_ex_policy),
             "Error from FAPI_ATTR_GET (ATTR_SBE_SELECT_EX_POLICY)");

    if (l_attr_contained_ipl_type == fapi2::ENUM_ATTR_CONTAINED_IPL_TYPE_CACHE)
    {
        o_active_cores_num = l_functional_cores_num;
        o_backing_caches_num = 0;
    }
    else if (l_attr_contained_ipl_type == fapi2::ENUM_ATTR_CONTAINED_IPL_TYPE_CHIP)
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_CONTAINED_ACTIVE_CORES_VEC, i_target, l_attr_chip_contained_active_cores_vec),
                 "Error from FAPI_ATTR_GET (ATTR_CHIP_CONTAINED_ACTIVE_CORES_VEC)");
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_CONTAINED_BACKING_CACHES_VEC, i_target,
                               l_attr_chip_contained_backing_caches_vec),
                 "Error from FAPI_ATTR_GET (ATTR_CHIP_CONTAINED_BACKING_CACHES_VEC)");

        for (auto l_bit = 0; l_bit < 32; l_bit++)
        {
            o_active_cores_num += (l_attr_chip_contained_active_cores_vec & 0x1);
            o_backing_caches_num += (l_attr_chip_contained_backing_caches_vec & 0x1);
            l_attr_chip_contained_active_cores_vec = l_attr_chip_contained_active_cores_vec >> 1;
            l_attr_chip_contained_backing_caches_vec = l_attr_chip_contained_backing_caches_vec >> 1;
        }
    }
    else
    {
        if (l_attr_sbe_select_ex_policy == fapi2::ENUM_ATTR_SBE_SELECT_EX_POLICY_CRONUS_MAX_ACTIVE)
        {
            o_active_cores_num = l_functional_cores_num;
            o_backing_caches_num = 0;
        }
        else
        {
            // default to minset of active cores / backing caches
            o_active_cores_num = ((l_attr_fused_core_mode == fapi2::ENUM_ATTR_FUSED_CORE_MODE_CORE_FUSED) ? (2) : (1));
            o_backing_caches_num = 2;

            if ((l_attr_sbe_select_ex_policy == fapi2::ENUM_ATTR_SBE_SELECT_EX_POLICY_HB_MAX_FOOTPRINT) ||
                (l_attr_sbe_select_ex_policy == fapi2::ENUM_ATTR_SBE_SELECT_EX_POLICY_HB_MAX_FOOTPRINT_MAX_THREADS))
            {
                // maximal set of backing caches
                // backing cache configuration can only grow in powers of 2
                while (((o_backing_caches_num * 2) + o_active_cores_num) <=
                       l_functional_cores_num)
                {
                    o_backing_caches_num *= 2;
                }
            }

            if ((l_attr_sbe_select_ex_policy == fapi2::ENUM_ATTR_SBE_SELECT_EX_POLICY_HB_MAX_THREADS) ||
                (l_attr_sbe_select_ex_policy == fapi2::ENUM_ATTR_SBE_SELECT_EX_POLICY_HB_MAX_FOOTPRINT_MAX_THREADS))
            {
                // maximal set of active cores
                o_active_cores_num = (l_functional_cores_num - o_backing_caches_num);
            }
        }

        if (l_attr_is_mpipl)
        {
            o_backing_caches_num = 0;
        }
    }

fapi_try_exit:
    FAPI_DBG("< select_ex_calc_active_backing_nums...");
    return fapi2::current_err;
}


///-----------------------------------------------------------------------------
/// @brief Update the QMEs PFET delays
///
/// @return  FAPI2_RC_SUCCESS if success, else error code.
fapi2::ReturnCode select_ex_pfet_delay(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    using namespace scomt::c;

    FAPI_DBG("> select_ex_pfet_delay...");

    fapi2::buffer<uint64_t> l_data64;
    fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST > l_core_mc;

    l_core_mc = i_target.getMulticast(fapi2::MCGROUP_GOOD_EQ, fapi2::MCCORE_ALL);

    // *INDENT-OFF*
    l_data64.flush<0>()
            .insertFromRight<0,  4>(HCD_PFET_DELAY_POWERDOWN)
            .insertFromRight<4,  4>(HCD_PFET_DELAY_POWERUP_L3)
            .insertFromRight<8,  4>(HCD_PFET_DELAY_POWERUP_CL2)
            .insertFromRight<12, 4>(HCD_PFET_DELAY_POWERUP_MMA);
    // *INDENT-ON*

#ifdef UNICAST_ONLY
    auto l_core_functional_vector = i_target.getChildren<fapi2::TARGET_TYPE_CORE>
                                    (fapi2::TARGET_STATE_FUNCTIONAL);
    // UNICAST
    FAPI_DBG("Setting PFET Delays in each core with unicast");

    for (auto& core : l_core_functional_vector)
    {
        FAPI_TRY(fapi2::putScom(core, CPMS_PFETDLY, l_data64));
    }

#else
    // MULTICAST
    FAPI_DBG("Setting PFET Delays in all cores via multicast");
    FAPI_TRY(fapi2::putScom(l_core_mc, CPMS_PFETDLY, l_data64));
#endif

    // Clear QME Scratch 1[Runtime Wakeup Mode](3) to force SMF enabled systems
    // to start Hostboot in UV mode

    // @todo RTC 207903
    // This bit should come from p10_pm_hcd_flags.h but it is presently
    // not mirrored.
//    l_data64.flush<0>().setBit<31>();
//     FAPI_TRY(fapi2::putScom(i_target_core, C_CPPM_CPMMR_CLEAR, l_data64));
//     FAPI_DBG("Clearing CPPMR[Runtime Wakeup Mode] in core %d", i_core_num);

fapi_try_exit:
    FAPI_DBG("< select_ex_pfet_delay...");
    return fapi2::current_err;
}

///-----------------------------------------------------------------------------
/// @brief Configure a core/L3 cache set for use
///
/// @return  FAPI2_RC_SUCCESS if success, else error code.
inline
fapi2::ReturnCode select_ex_config(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_eq_target,
    const uint32_t i_core_num)
{
    using namespace scomt::eq;

    FAPI_DBG("> select_ex_config for core/L3 %d", i_core_num);

    fapi2::buffer<uint64_t> l_data64;

    l_data64.flush<0>();
    l_data64.setBit(5  + (i_core_num & 0x3)); // Core
    l_data64.setBit(9  + (i_core_num & 0x3)); // L3
    l_data64.setBit(15 + (i_core_num & 0x3)); // MMA

    // Partial Good
    FAPI_TRY(fapi2::putScom(i_eq_target, CPLT_CTRL2_WO_OR, l_data64));

    // PSCOM enable
    FAPI_TRY(fapi2::putScom(i_eq_target, CPLT_CTRL3_WO_OR, l_data64));

    // Power Gate enable
    FAPI_TRY(fapi2::putScom(i_eq_target, CPLT_CTRL5_WO_OR, l_data64));

fapi_try_exit:
    FAPI_DBG("< select_ex_config...");
    return fapi2::current_err;
}
