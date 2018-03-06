/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdgeneric.C $                         */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2018                        */
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
/*
 * @file: ppe/sbe/sbefw/sbecmdgeneric.C
 *
 * @brief This file contains the SBE generic Commands
 *
 */

#include "sbecmdgeneric.H"
#include "sbefifo.H"
#include "sbetrace.H"
#include "sbe_sp_intf.H"
#include "sbe_build_info.H"
#include "sbeFifoMsgUtils.H"
#include "sbeFFDC.H"
#include "sberegaccess.H"
#include "sbestates.H"
#include "sbeHostMsg.H"
#include "sbeHostUtils.H"
#include "sbeglobals.H"
#include "sbeXipUtils.H"

#include "fapi2.H"
//#include "p9_xip_image.h"

using namespace fapi2;

#ifdef __SBEFW_SEEPROM__

void updateFifoCapabilities(uint32_t * capability)
{
    // @TODO via RTC : 160602
    // Update Capability flags based on lastes spec.
    capability[IPL_CAPABILITY_START_IDX] =
                                EXECUTE_ISTEP_SUPPPORTED |
                                SUSPEND_IO_SUPPPORTED;

    capability[SCOM_CAPABILITY_START_IDX] =
                                GET_SCOM_SUPPPORTED |
                                PUT_SCOM_SUPPPORTED |
                                MODIFY_SCOM_SUPPPORTED |
                                PUT_SCOM_UNDER_MASK_SUPPPORTED ;

    capability[GENERIC_CHIPOP_CAPABILITY_START_IDX] =
                                GET_SBE_FFDC_SUPPPORTED |
                                GET_SBE_CAPABILITIES_SUPPPORTED|
                                SBE_QUIESCE;

    capability[MEMORY_CAPABILITY_START_IDX] =
                                GET_MEMORY_SUPPPORTED |
                                PUT_MEMORY_SUPPPORTED |
                                GET_SRAM_OCC_SUPPPORTED |
                                PUT_SRAM_OCC_SUPPPORTED;

    capability[INSTRUCTION_CTRL_CAPABILITY_START_IDX] =
                                CONTROL_INSTRUCTIONS_SUPPPORTED;

    capability[REGISTER_CAPABILITY_START_IDX] =
                                GET_REGISTER_SUPPPORTED |
                                PUT_REGISTER_SUPPPORTED ;

    capability[RING_CAPABILITY_START_IDX] =
                                GET_RING_SUPPPORTED |
                                PUT_RING_SUPPPORTED;
}
#endif //__SBEFW_SEEPROM__

#ifndef __SBEFW_SEEPROM__
#endif //not __SBEFW_SEEPROM__

