/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: import/chips/p9/procedures/hwp/core/p9_hcd_core_pcb_arb.C $   */
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
///
/// @file  p9_hcd_core_pcb_arb.C
/// @brief Core Chiplet PCB Arbitration
///
/// Procedure Summary:
///   If CME,
///     1.Request PCB Mux, via write to PCB_MUX_REQ_C0 @ CCSCR_OR
///       - setBit(5) @ CME_LOCAL_CORE_STOP_CONTROL_REGISTER_OR_0510
///     2.Poll for PCB Mux grant, via read from
///       Polled Timeout: ns
///       - getBit() @
///   Else (SBE),
///     Nop (as the CME is not running in bringing up the first Core)
///

// *HWP HWP Owner          : David Du      <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still    <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:CME
// *HWP Level              : 2

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <p9_quad_scom_addresses.H>
#include <p9_hcd_common.H>
#include "p9_hcd_core_pcb_arb.H"

//-----------------------------------------------------------------------------
// Constant Definitions: Core Chiplet PCB Arbitration
//-----------------------------------------------------------------------------


fapi2::ReturnCode
p9_hcd_core_pcb_arb(
    const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target,
    const p9hcd::P9_HCD_CME_CORE_MASKS i_core_mask,
    const p9hcd::P9_HCD_PCB_ARBITER_CTRL i_request)
{
    FAPI_INF(">>p9_hcd_core_pcb_arb: Core[%d] Req[%d]", i_core_mask, i_request);

#ifdef P9_HCD_CME_BUILD

    FAPI_DBG("Request or Release the PCB Arbiter");
    out32((i_request ? CME_LCL_SICR_OR : CME_LCL_SICR_CLR),
          (i_core_mask << SHIFT32(11)));

    FAPI_DBG("Poll for PCB Arbiter Granted");
    uint32_t l_sisr;

    do
    {
        l_sisr = (in32(CME_LCL_SISR) >> SHIFT32(11));

        if((  i_request  && ((i_core_mask &   l_sisr)  == i_core_mask)) ||
           ((!i_request) && ((i_core_mask & (~l_sisr)) == i_core_mask)))
        {
            break;
        }
    }
    while(1);

#else

    FAPI_DBG("Check for PCB Arbiter Granted to Core");

    /// @todo require core to cme target conversion
    /*
    fapi2::buffer<uint64_t> l_data64;
    FAPI_TRY(getScom(i_target, EX_0_CME_LCL_SISR_SCOM, l_data64));

    FAPI_ASSERT(((l_data64 & (i_core_mask << SHIFT64(11))) !=
                             (i_core_mask << SHIFT64(11))),
                fapi2::PMPROC_COREPCBARB_GRANTCME().set_CMESISR(l_data64),
                "PCB Arbiter is Granted to CME");
    */
    FAPI_DBG("PCB Arbiter is Granted to Core");

    /// @todo MPIPL: if check grant to cme, consider to overide it back to core

//fapi_try_exit:

#endif

    FAPI_INF("<<p9_hcd_core_pcb_arb");

    return fapi2::current_err;
}


