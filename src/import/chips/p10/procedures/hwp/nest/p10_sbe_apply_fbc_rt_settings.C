/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/nest/p10_sbe_apply_fbc_rt_settings.C $ */
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
/// @file p10_sbe_apply_fbc_rt_settings.C
/// @brief Apply fabric runtime settings via XSCOM
///

//
// *HWP HW Maintainer: Joe McGill <jmcgill@us.ibm.com>
// *HWP FW Maintainer: Raja Das <rajadas2@in.ibm.com>
// *HWP Consumed by  : SBE
//

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <p10_sbe_apply_fbc_rt_settings.H>
#include <p10_sbe_apply_xscom_inits.H>
//------------------------------------------------------------------------------
// Function definitions
//------------------------------------------------------------------------------

// doxygen in header
fapi2::ReturnCode
p10_sbe_apply_fbc_rt_settings(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
    const size_t i_xscomPairSize,
    const void* i_pxscomInit)
{
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    fapi2::ATTR_SYSTEM_IPL_PHASE_Type l_ipl_type;
    fapi2::ATTR_PROC_SBE_MASTER_CHIP_Type l_is_master_sbe;
    fapi2::ATTR_IS_MPIPL_Type l_is_mpipl;

    FAPI_DBG("Start");

    // validate that HWP should be run -- only desire to run on the
    // master SBE in a non-MPIPL
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_SBE_MASTER_CHIP,
                           i_target,
                           l_is_master_sbe),
             "Error from FAPI_ATTR_GET (ATTR_PROC_SBE_MASTER_CHIP)");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_IS_MPIPL,
                           FAPI_SYSTEM,
                           l_is_mpipl),
             "Error from FAPI_ATTR_GET (ATTR_IS_MPIPL)");
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYSTEM_IPL_PHASE,
                           FAPI_SYSTEM,
                           l_ipl_type),
             "Error from FAPI_ATTR_GET (ATTR_SYSTEM_IPL_PHASE)");

    if ((l_ipl_type != fapi2::ENUM_ATTR_SYSTEM_IPL_PHASE_HB_IPL) ||
        (l_is_master_sbe != fapi2::ENUM_ATTR_PROC_SBE_MASTER_CHIP_TRUE) ||
        (l_is_mpipl != fapi2::ENUM_ATTR_IS_MPIPL_FALSE))
    {
        goto fapi_try_exit;
    }

    FAPI_EXEC_HWP(fapi2::current_err,
                  p10_sbe_apply_xscom_inits,
                  i_target,
                  i_xscomPairSize,
                  i_pxscomInit);

fapi_try_exit:
    FAPI_DBG("End");
    return fapi2::current_err;
}
