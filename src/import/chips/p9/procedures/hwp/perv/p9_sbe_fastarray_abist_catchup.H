/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_fastarray_abist_catchup.H $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2017                        */
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
/// @file  p9_sbe_fastarray_abist_catchup.H
///
/// @brief Abist catchup sub-procedure for p9_fastarray procedure
//         Used to catch up Abist cycles for the skipped array rows
//------------------------------------------------------------------------------
// *HWP HW Owner        : Joachim Fenkes <fenkes@de.ibm.com>
// *HWP HW Backup Owner : Nick Klazynski <jklazyns@us.ibm.com>
// *HWP FW Owner        : Nagendra Gurram <nagendra.g@in.ibm.com>
// *HWP Team            : Chip
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------

#ifndef _P9_SBE_FASTARRAY_ABIST_CATCHUP
#define _P9_SBE_FASTARRAY_ABIST_CATCHUP

#include <fapi2.H>

typedef fapi2::ReturnCode (*p9_sbe_fastarray_abist_catchup_FP_t)(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>&,
    const uint32_t);

/**
 * @brief Abist catchup chip-op for p9_fastarray procedure
 * @param[in] i_target_chiplet The chiplet target on where we need to skip cycles
 * @param[in] i_clockCycles  Number of cycles that need to be skipped
 * @return FAPI2_RC_SUCCESS if success, else error code.
 */

fapi2::ReturnCode p9_sbe_fastarray_abist_catchup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const uint32_t i_clockCycles);

#endif //_P9_SBE_FASTARRAY_ABIST_CATCHUP
