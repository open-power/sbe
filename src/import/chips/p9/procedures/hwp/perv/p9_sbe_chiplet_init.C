/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_sbe_chiplet_init.C $ */
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
/// @file  p9_sbe_chiplet_init.C
///
/// @brief init procedure for all enabled chiplets
//------------------------------------------------------------------------------
// *HWP HW Owner        : Abhishek Agarwal <abagarw8@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : sunil kumar <skumar8j@in.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 2
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------


//## auto_generated
#include "p9_sbe_chiplet_init.H"

#include "p9_perv_scom_addresses.H"


fapi2::ReturnCode p9_sbe_chiplet_init(const
                                      fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    bool l_read_reg = false;
    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p9_sbe_chiplet_init: Entering..");

    FAPI_DBG("Check  for XSTOP Bit");
    //Getting INTERRUPT_TYPE_REG register value
    FAPI_TRY(fapi2::getScom(i_target_chip, PERV_PIB_INTERRUPT_TYPE_REG, l_data64));
    //l_read_reg = PIB.INTERRUPT_TYPE_REG.CHECKSTOP
    l_read_reg = l_data64.getBit<2>();

    FAPI_ASSERT(!(l_read_reg),
                fapi2::CHECKSTOP_ERR()
                .set_READ_CHECKSTOP(l_read_reg),
                "ERROR:CHECKSTOP BIT GET SET ");

    FAPI_INF("p9_sbe_chiplet_init: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}


