/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/pm/p10_query_corecachemma_access_state.H $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2021                        */
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
///----------------------------------------------------------------------------
///
/// @file p10_query_corecachemma_access_state.H
///
/// @brief Determine the scommable and scannable states of the various units
//  in a quad
///
/// HWP Owner       : Manish Chowdhary <manichow@in.ibm.com>
/// Backup HWP Owner: Rajees Rehman <stillgs@us.ibm.com>
/// FW Owner        :
/// Team            :
/// Level           : 2
/// Consumed by     : FSP:SBE
///----------------------------------------------------------------------------

#ifndef _QUERY_CORECACHEMMA_ACCESS_STATE_H_
#define _QUERY_CORECACHEMMA_ACCESS_STATE_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <fapi2.H>

typedef union scomStatus
{
    struct
    {
#ifdef _BIG_ENDIAN
        uint32_t reserved: 5;
        uint32_t ecl20   : 1;
        uint32_t ecl21   : 1;
        uint32_t ecl22   : 1;
        uint32_t ecl23   : 1;
        uint32_t l30     : 1;
        uint32_t l31     : 1;
        uint32_t l32     : 1;
        uint32_t l33     : 1;
        uint32_t qme     : 1;
        uint32_t clkadj  : 1;
        uint32_t mma0    : 1;
        uint32_t mma1    : 1;
        uint32_t mma2    : 1;
        uint32_t mma3    : 1;
        uint32_t reserved1: 13;
#else
        uint32_t reserved1: 13;
        uint32_t mma3    : 1;
        uint32_t mma2    : 1;
        uint32_t mma1    : 1;
        uint32_t mma0    : 1;
        uint32_t clkadj  : 1;
        uint32_t qme     : 1;
        uint32_t l33     : 1;
        uint32_t l32     : 1;
        uint32_t l31     : 1;
        uint32_t l30     : 1;
        uint32_t ecl23   : 1;
        uint32_t ecl22   : 1;
        uint32_t ecl21   : 1;
        uint32_t ecl20   : 1;
        uint32_t reserved: 5;
#endif
    };
    uint32_t scomState = 0;
} scomStatus_t;

typedef union scanStatus
{
    struct
    {
#ifdef _BIG_ENDIAN
        uint32_t reserved: 5;
        uint32_t ecl20   : 1;
        uint32_t ecl21   : 1;
        uint32_t ecl22   : 1;
        uint32_t ecl23   : 1;
        uint32_t l30     : 1;
        uint32_t l31     : 1;
        uint32_t l32     : 1;
        uint32_t l33     : 1;
        uint32_t qme     : 1;
        uint32_t reserved2: 1;
        uint32_t mma0    : 1;
        uint32_t mma1    : 1;
        uint32_t mma2    : 1;
        uint32_t mma3    : 1;
        uint32_t reserved3: 13;
#else
        uint32_t reserved3: 13;
        uint32_t mma3    : 1;
        uint32_t mma2    : 1;
        uint32_t mma1    : 1;
        uint32_t mma0    : 1;
        uint32_t reserved2: 1;
        uint32_t qme     : 1;
        uint32_t l33     : 1;
        uint32_t l32     : 1;
        uint32_t l31     : 1;
        uint32_t l30     : 1;
        uint32_t ecl23   : 1;
        uint32_t ecl22   : 1;
        uint32_t ecl21   : 1;
        uint32_t ecl20   : 1;
        uint32_t reserved: 5;
#endif
    };
    uint32_t scanState = 0;
} scanStatus_t;

typedef fapi2::ReturnCode
(*p10_query_corecachemma_access_state_FP_t) (const fapi2::Target <fapi2::TARGET_TYPE_EQ>&, scomStatus_t&,
        scanStatus_t&, bool);

//------------------------------------------------------------------------------
// Function prototypes
//------------------------------------------------------------------------------
extern "C"
{
    /// @brief Determines the scomable and scanable state of core/L2,L3 and MMA for the input
    //         EQ target.
    ///
    /// @param[in]   i_target        Reference to input EQ for which access state needs to be
    //                               determined
    //  @param[out]  o_scomStateData Output parameter to indicate scommable state of the core
    //  @param[out]  o_scanStateData Output parameter to indicate scanable state of the core
    ///
    /// @return fapi2::ReturnCode. FAPI2_RC_SUCCESS if success, else error code
    ///
    fapi2::ReturnCode p10_query_corecachemma_access_state(
        const fapi2::Target < fapi2::TARGET_TYPE_EQ >& i_target,
        scomStatus_t& o_scomStateData,
        scanStatus_t& o_scanStateData,
        bool i_scanClockState = false);
} // extern "C"

#endif // _PROC_SBE_INSTRUCT_START_H_
