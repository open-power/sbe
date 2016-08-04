/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/nest/p9_inject_local_xstop.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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
// @file p9_inject_local_xstop.C
// @brief  (eCMD) Sets FIR registers to trigger local checkstop in the core
//
// @author Josh Hannan <jlhannan@us.ibm.com>
//
//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "p9_inject_local_xstop.H"
#include <fapi2.H>
#include <p9_quad_scom_addresses.H>

const uint32_t CORE_LOCAL_XSTOP_BIT = 51;

fapi2::ReturnCode  p9_inject_local_xstop(fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target)
{
    fapi2::buffer<uint64_t> data;

    // Clear Bit CORE_LOCAL_XSTOP_BIT of FIRMASK register so FIR is visible
    data.flush<1>().clearBit<CORE_LOCAL_XSTOP_BIT>();
    FAPI_TRY(fapi2::putScom(i_target, C_CORE_FIRMASK_AND, data),
             "p9_inject_local_xstop: Could not write to FIR Mask Register");

    // Set bit CORE_LOCAL_XSTOP_BIT of Both Action Registers
    // This will perform a local checkstop when the fir bit is set
    FAPI_TRY(fapi2::getScom(i_target, C_CORE_ACTION0, data),
             "p9_inject_local_xstop: Error while performing getscom on FIR Action 0 Register");
    data.setBit<CORE_LOCAL_XSTOP_BIT>();
    FAPI_TRY(fapi2::putScom(i_target, C_CORE_ACTION0, data),
             "p9_inject_local_xstop: Error while performing putscom on FIR Action 0 Register");


    FAPI_TRY(fapi2::getScom(i_target, C_CORE_ACTION1, data),
             "p9_inject_local_xstop: Error while performing getscom on FIR Action 1 Register");
    data.setBit<CORE_LOCAL_XSTOP_BIT>();
    FAPI_TRY(fapi2::putScom(i_target, C_CORE_ACTION1, data),
             "p9_inject_local_xstop: Error while performing putscom on FIR Action 1 Register");

    // Set bit CORE_LOCAL_XSTOP_BIT of FIR register to perform local checkstop
    data.flush<0>().setBit<CORE_LOCAL_XSTOP_BIT>();
    FAPI_TRY(fapi2::putScom(i_target, C_CORE_FIR_OR, data),
             "p9_inject_local_xstop: Could not write to Fault Isolation Register");

fapi_try_exit:
    FAPI_DBG("Exiting...");
    return fapi2::current_err;
}




