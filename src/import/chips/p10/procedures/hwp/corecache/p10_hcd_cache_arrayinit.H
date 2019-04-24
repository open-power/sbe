/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_hcd_cache_arrayinit.H $ */
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



#ifndef _P10_HCD_CACHE_ARRAYINIT_H_
#define _P10_HCD_CACHE_ARRAYINIT_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <fapi2.H>

//------------------------------------------------------------------------------
// Structure definitions
//------------------------------------------------------------------------------

/// function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode (*p10_hcd_cache_arrayinit_FP_t) (
    const fapi2::Target < fapi2::TARGET_TYPE_CORE |
    fapi2::TARGET_TYPE_MULTICAST,
    fapi2::MULTICAST_AND > &);

//------------------------------------------------------------------------------
// Function prototypes
//------------------------------------------------------------------------------

extern "C"
{

///
/// @brief
///
/// @param[in] i_target       Reference to fapi2::TARGET_TYPE_CORE|fapi2::TARGET_TYPE_MULTICAST
/// @param[in] i_regions      Mask of the regions to be affected by the arrayinit
///                           Bit   Region
///                           4:    #0  : perv
///                           5:    #1  : EC/L2  0
///                           6:    #2  : EC/L2  1
///                           7:    #3  : EC/L2  2
///                           8:    #4  : EC/L2  3
///                           9:    #5  : L3 0
///                           10:   #6  : L3 1
///                           11:   #7  : L3 2
///                           12:   #8  : L3 3
///                           13:   #9  : QME
///                           14:   #10 : CLKADJ
///                           15:   #11 : MMA 0
///                           16:   #12 : MMA 1
///                           17:   #13 : MMA 2
///                           18:   #14 : MMA 3
/// @return fapi::ReturnCode  FAPI2_RC_SUCCESS if success, else error code.
///
    fapi2::ReturnCode p10_hcd_cache_arrayinit(
        const fapi2::Target < fapi2::TARGET_TYPE_CORE |
        fapi2::TARGET_TYPE_MULTICAST,
        fapi2::MULTICAST_AND > & i_target);

} // extern "C"

#endif // _P10_HCD_CACHE_ARRAYINIT_H_
