/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/utils/contained/p10_contained_ipl_istep.H $ */
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
#ifndef _P10_CONTAINED_IPL_ISTEP_H_
#define _P10_CONTAINED_IPL_ISTEP_H_

#include <p10_contained.H>
#include <p10_contained_sim.H>
#include <multicast_group_defs.H>

#include <p10_perv_sbe_cmn.H>
#include <p10_sbe_chiplet_reset.H>
#include <p10_sbe_gptr_time_initf.H>
#include <p10_sbe_repr_initf.H>
#include <p10_sbe_arrayinit.H>
#include <p10_sbe_initf.H>
#include <p10_sbe_startclocks.H>
#include <p10_hcd_cache_reset.H>
#include <p10_hcd_cache_gptr_time_initf.H>
#include <p10_hcd_cache_repair_initf.H>
#include <p10_hcd_cache_arrayinit.H>
#include <p10_hcd_cache_initf.H>
#include <p10_hcd_cache_startclocks.H>
#include <p10_hcd_core_reset.H>
#include <p10_hcd_core_gptr_time_initf.H>
#include <p10_hcd_core_repair_initf.H>
#include <p10_hcd_core_arrayinit.H>
#include <p10_hcd_core_initf.H>
#include <p10_hcd_core_startclocks.H>

///
/// @brief Convenience wrapper around FAPI_EXEC_HWP(...) which allows calling
///        procedures with FAPI_TRY(EXEC_IPL_PROC(...)) instead of manually
///        checking the returncode reference as with "plain" FAPI_EXEC_HWP(...).
///
/// @param[io] eip_rc Variable of type fapi2::ReturnCode (clobbered)
/// @param[in] eip_proc Procedure function pointer to call
/// @param[in] eip_tgt Variable of type fapi2::Target<...> passed to procedure
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
#define EXEC_IPL_PROC(eip_rc, eip_proc, eip_tgt) \
    ({ \
        FAPI_EXEC_HWP(eip_rc, eip_proc, eip_tgt); \
        eip_rc; \
    })

///
/// @brief Perform "istep3" re-initialization for chip-contained IPL
///
/// @param[in] i_target Reference to chip target
/// @param[in] i_runn Indicate if IPL'ing in RUNN mode
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
static inline fapi2::ReturnCode p10_contained_ipl_istep3(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const bool i_runn)
{
    fapi2::ReturnCode rc;
    auto const all = i_target.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_NO_TP);
    auto const tmp = CHC_MC_REGIONS_NO_PERV;
    uint16_t const regions =  static_cast<uint16_t>(tmp.getBit<4, 15>());

    // Scan-zero
    FAPI_TRY(p10_perv_sbe_cmn_scan0_module(all, regions,
                                           p10SbeChipletReset::SCAN_TYPES_TIME_GPTR_REPR));
    FAPI_TRY(p10_perv_sbe_cmn_scan0_module(all, regions,
                                           p10SbeChipletReset::SCAN_TYPES_EXCEPT_TIME_GPTR_REPR));

    FAPI_TRY(EXEC_IPL_PROC(rc, p10_sbe_gptr_time_initf, i_target));
    FAPI_TRY(EXEC_IPL_PROC(rc, p10_sbe_repr_initf, i_target));
    FAPI_TRY(EXEC_IPL_PROC(rc, p10_sbe_arrayinit, i_target));
    FAPI_TRY(EXEC_IPL_PROC(rc, p10_sbe_initf, i_target));

    FAPI_TRY(sim::init_via_dials(sim::ISTEP3, true));

    if (!i_runn)
    {
        FAPI_TRY(EXEC_IPL_PROC(rc, p10_sbe_startclocks, i_target));
    }

fapi_try_exit:
    return fapi2::current_err;
}

///
/// @brief Perform "istep4" re-initialization for cache- and chip-contained IPL
///
/// @param[in] i_cores Reference to all-good cores multicast target
/// @param[in] i_chc Specify chip-contained IPL (false=cache-contained IPL)
/// @param[in] i_runn Indicate if IPL'ing in RUNN mode
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
static inline fapi2::ReturnCode p10_contained_ipl_istep4(const fapi2::Target < fapi2::TARGET_TYPE_CORE
        | fapi2::TARGET_TYPE_MULTICAST,
        fapi2::MULTICAST_OR > & i_cores,
        const bool i_chc, const bool i_runn, const l3_config& i_l3_config,
        const fapi2::ATTR_ACTIVE_CORES_VEC_Type i_active_bvec)
{
    fapi2::ReturnCode rc;

    FAPI_TRY(EXEC_IPL_PROC(rc, p10_hcd_cache_reset, i_cores));
    FAPI_TRY(EXEC_IPL_PROC(rc, p10_hcd_cache_gptr_time_initf, i_cores));
    FAPI_TRY(EXEC_IPL_PROC(rc, p10_hcd_cache_repair_initf, i_cores));

    FAPI_TRY(sim::apply_selective_skip_arrayinit(sim::L3ARY, true));
    FAPI_TRY(EXEC_IPL_PROC(rc, p10_hcd_cache_arrayinit, i_cores));
    FAPI_TRY(sim::apply_selective_skip_arrayinit(sim::L3ARY, false));

    FAPI_TRY(EXEC_IPL_PROC(rc, p10_hcd_cache_initf, i_cores));

    FAPI_TRY(sim::init_via_dials(sim::ISTEP4_CACHE, i_chc));

    if (!i_runn)
    {
        FAPI_TRY(EXEC_IPL_PROC(rc, p10_hcd_cache_startclocks, i_cores));
    }

    FAPI_TRY(restore_l3_config(i_cores.getParent<fapi2::TARGET_TYPE_PROC_CHIP>(),
                               i_active_bvec, i_l3_config, i_runn));

    FAPI_TRY(EXEC_IPL_PROC(rc, p10_hcd_core_reset, i_cores));
    FAPI_TRY(EXEC_IPL_PROC(rc, p10_hcd_core_gptr_time_initf, i_cores));
    FAPI_TRY(EXEC_IPL_PROC(rc, p10_hcd_core_repair_initf, i_cores));
    FAPI_TRY(EXEC_IPL_PROC(rc, p10_hcd_core_arrayinit, i_cores));
    FAPI_TRY(EXEC_IPL_PROC(rc, p10_hcd_core_initf, i_cores));

    FAPI_TRY(sim::init_via_dials(sim::ISTEP4_CORE, i_chc));

    if (!i_runn)
    {
        FAPI_TRY(EXEC_IPL_PROC(rc, p10_hcd_core_startclocks, i_cores));
    }

fapi_try_exit:
    return fapi2::current_err;
}

#endif
