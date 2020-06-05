/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_tp_repr_initf.C $ */
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
//------------------------------------------------------------------------------
/// @file  p10_sbe_tp_repr_initf.C
///
/// @brief Scan initialize REPR for PERV chiplet
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------

#include "p10_sbe_tp_repr_initf.H"
#include "p10_perv_sbe_cmn.H"

static const ring_setup_t ISTEP2_REPR_RINGS[] =
{
    // perv & occ regions are merged for all ring types other than func
    {perv_occ_repr,  IGNORE_PG, TARGET_CHIP, 0x1, 0x1, 1},
};

fapi2::ReturnCode p10_sbe_tp_repr_initf(const
                                        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_INF("p10_sbe_tp_repr_initf: Entering ...");

    FAPI_TRY(p10_perv_sbe_cmn_setup_putring(i_target_chip, ISTEP2_REPR_RINGS, true));

    FAPI_INF("p10_sbe_tp_repr_initf: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}
