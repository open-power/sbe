/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/nest/p10_suspend_io.H $   */
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
///
/// @file p10_suspend_io.H
/// @brief Procedure to suspend PCIe traffic.
///
/// *HWP HWP Maintainer: Manish Chowdhary  <manichow@in.ibm.com>
/// *HWP FW Maintainer : Raja Das <rajadas2@in.ibm.com>
/// *HWP Consumed by   : SBE


#ifndef _p10_SUSPEND_IO_H_
#define _p10_SUSPEND_IO_H_

//-----------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------
#include <fapi2.H>

//-----------------------------------------------------------------------------------
// Structure definitions
//-----------------------------------------------------------------------------------

//function pointer typedef definition for HWP call support
typedef fapi2::ReturnCode (*p10_suspend_io_FP_t) (const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&, const bool);

//-----------------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------------

// PCI Nest FIR Register bit26 for SW Freeze enablement
const uint8_t PHB_NFIR_REG_SW_DEFINED_FREEZE = 26;
//Maximum number of iterations (So, 20ms * 100 = 2s before timeout)
const uint32_t MAX_NUM_POLLS = 100;
//(40000000 ns = 20 ms) to wait for PBCQ inbound/outbound to become inactive
const uint64_t PBCQ_NANO_SEC_DELAY = 40000000;
//400,000 simulation cycles to wait for PBCQ inbound/outbound to become inactive
const uint64_t PBCQ_SIM_CYC_DELAY = 400000;


extern "C" {

//-----------------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------------
///
/// @brief Freeze PEC and drive reset to PHB to suspend all IO traffic
///
/// @param[in] i_target        Reference to chip proc target
/// @param[in] i_enable_mpipl  true = running during MPIPL; false = otherwise.
///
/// @return fapi2:ReturnCode. FAPI2_RC_SUCCESS if success, else error code.
///
    fapi2::ReturnCode p10_suspend_io(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const bool i_enable_mpipl);

} //extern"C"

#endif //_p10_SUSPEND_IO_H_
