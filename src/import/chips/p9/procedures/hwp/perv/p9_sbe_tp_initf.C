/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_tp_initf.C $   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
/// @file  p9_sbe_tp_initf.C
///
/// @brief TP chiplet scaninits for the TP rings
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : Sunil Kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_tp_initf.H"
#include "p9_ring_id.h"

fapi2::ReturnCode p9_sbe_tp_initf(const
                                  fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_INF("p9_sbe_tp_initf: Entering ...");

    FAPI_DBG("Scan perv_fure ring");
    FAPI_TRY(fapi2::putRing(i_target_chip, perv_fure),
             "Error from putRing (perv_fure)");

    FAPI_DBG("Scan occ_fure ring");
    FAPI_TRY(fapi2::putRing(i_target_chip, occ_fure),
             "Error from putRing (occ_fure)");

    FAPI_DBG("Scan perv_ana_func ring");
    FAPI_TRY(fapi2::putRing(i_target_chip, perv_ana_func),
             "Error from putRing (perv_ana_func)");

fapi_try_exit:
    FAPI_INF("p9_sbe_tp_initf: Exiting ...");
    return fapi2::current_err;
}
