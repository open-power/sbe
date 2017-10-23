/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/io/p9_sbe_io_eol_toggle.H $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017,2018                        */
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
/// @file  p9_sbe_io_eol_toggle.H
///
/// @brief Toggles a pervasive bit on a coarse basis to preserve I/O end of life
///------------------------------------------------------------------------------
/// *HWP HW Owner        : Chris Steffen <cwsteffen@us.ibm.com>
/// *HWP HW Backup Owner : Gary Peterson <garyp@us.ibm.com>
/// *HWP FW Owner        : Jamie Knight <rjknight@us.ibm.com>
/// *HWP Team            : IO
/// *HWP Level           : 3
/// *HWP Consumed by     : SBE
///------------------------------------------------------------------------------


#ifndef _P9_SBE_IO_EOL_TOGGLE_H_
#define _P9_SBE_IO_EOL_TOGGLE_H_


#include <fapi2.H>


typedef fapi2::ReturnCode (*p9_sbe_io_eol_toggle_FP_t)(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&);

/// @brief Toggles a pervasive bit on a coarse basis to preserve I/O end of life
///
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PROC_CHIP target
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p9_sbe_io_eol_toggle(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target);
}

#endif
