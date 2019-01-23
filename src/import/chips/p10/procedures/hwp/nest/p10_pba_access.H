/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/nest/p10_pba_access.H $   */
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
//-----------------------------------------------------------------------------------
//
/// @file p10_pba_access.H
/// @brief Read coherent state of memory via the PBA (FAPI)
///
// *HWP HW Maintainer : Chris Riedl (cmr@ibm.com)
// *HWP FW Maintainer : Raja Das    (rajadas2@in.ibm.com)
// *HWP Consumed by   : SBE
// ----------------------------------------------------------------------------------
//
// *! ADDITIONAL COMMENTS :
// *!
// *! The purpose of this procedure is to perform a coherent read or write from system
// *! memory via fabric commands issued from the PBA.
// *!
// *! Succcessful operation assumes that:
// *!   o System clocks are running
// *!   o Fabric is initalized
// *!
// *!
//-----------------------------------------------------------------------------------

#ifndef _P10_PBA_ACCESS_H_
#define _P10_PBA_ACCESS_H_

//-----------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------

#include <fapi2.H>
#include <p10_pba_constants.H>
//-----------------------------------------------------------------------------------
// Structure definitions
//-----------------------------------------------------------------------------------

//function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode
(*p10_pba_access_FP_t) (const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&,
                        const uint64_t,
                        const bool,
                        const uint32_t,
                        const bool,
                        const bool,
                        uint8_t[]);

//-----------------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------------

extern "C" {

//-----------------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------------

/// @brief setup for reads/writes from the PBA
/// @param[in]  i_target       => P9 chip target
/// @param[in]  i_address      => base real address for read/write operation (expected to be 128B aligned)
/// @param[in]  i_rnw          => if the operation is read not write (1 for read, 0 for write)
/// @param[in]  i_flags        => other information that is needed - see the p10_pba_constants pba_flags enums for bit definitions
///                               Note: to construct the flag you can use the p10_PBA_oper_flag class
/// @param[in]  i_lastGranule  => is this the last granule that is to be read/written
/// @param[in]  i_firstGranule => first granule that is to be read/written NOT USED FOR PBA
/// @param[in, out] io_data    => The data that is read/written
/// @return FAPI_RC_SUCCESS if the setup completes successfully,

    fapi2::ReturnCode p10_pba_access(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        const bool i_rnw,
        const uint32_t i_flags,
        const bool i_firstGranule,
        const bool i_lastGranule,
        uint8_t io_data[]);
} //extern "C"

#endif //_P10_PBA_ACCESS_H_
