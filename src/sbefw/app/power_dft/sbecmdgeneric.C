/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power_dft/sbecmdgeneric.C $                     */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2021                        */
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

void updatePsuCapabilities(uint32_t * capability)
{
    capability[PSU_CORE_CONTROL_CAPABILITY_START_IDX] =
                                      PSU_CONTROL_DEADMAN_LOOP |
                                      PSU_EXIT_CACHE_CONTAINED_MODE;
    capability[PSU_CORE_CONTROL_CAPABILITY_START_IDX+1] =
                                      PSU_RESERVED_1;

    capability[PSU_SCOM_CAPABILITY_START_IDX] =
                                      PSU_DEFAULT_CAPABILITY_D2;
    capability[PSU_SCOM_CAPABILITY_START_IDX+1] =
                                      PSU_RESERVED_2;

    capability[PSU_RING_CAPABILITY_START_IDX] =
                                      PSU_PUT_RING_FROM_IMAGE_SUPPORTED;
    capability[PSU_RING_CAPABILITY_START_IDX+1] =
                                      PSU_RESERVED_3;

    capability[PSU_TIMER_CAPABILITY_START_IDX] =
                                      PSU_CONTROL_TIMER_SUPPORTED;
    capability[PSU_TIMER_CAPABILITY_START_IDX+1] =
                                      PSU_RESERVED_4;

    capability[PSU_MPIPL_CAPABILITY_START_IDX] =
                                      PSU_CMD_GETHWREG;
    capability[PSU_MPIPL_CAPABILITY_START_IDX+1] =
                                      PSU_RESERVED_5;

    capability[PSU_SECURITY_CONTROL_CAPABILITY_START_IDX] =
                                      PSU_UNSECURE_MEM_REGION_SUPPORTED;
    capability[PSU_SECURITY_CONTROL_CAPABILITY_START_IDX+1] =
                                      PSU_RESERVED_6;

    capability[PSU_GENERIC_CHIPOP_CAPABILITY_START_IDX] =
                                      PSU_GET_SBE_CAPABILITIES_SUPPPORTED |
                                      PSU_READ_SBE_SEEPROM_SUPPORTED |
                                      PSU_SET_FFDC_ADDRESS_SUPPORTED |
                                      PSU_QUISCE_SUPPORTED |
                                      PSU_SET_SYSTEM_FABRIC_ID_MAP_SUPPORTED |
                                      PSU_STASH_MPIPL_CONFIG_SUPPORTED |
                                      PSU_SECURITY_LIST_BIN_DUMP_SUPPORTED |
                                      PSU_GENERIC_MSG_UPDATE_OCMB_TARGET_SUPPORTED;
    capability[PSU_GENERIC_CHIPOP_CAPABILITY_START_IDX+1] =
                                      PSU_RESERVED_7;

}
void updateFifoCapabilities(uint32_t * capability)
{
    // @TODO via RTC : 160602
    // Update Capability flags based on lastes spec.
    capability[IPL_CAPABILITY_START_IDX] =
                                EXECUTE_ISTEP_SUPPPORTED |
                                SUSPEND_IO_SUPPPORTED |
                                FLUSH_NVDIMM_SUPPPORTED;
    capability[IPL_CAPABILITY_START_IDX+1] =
                                RESERVED_A1_CAPABILITIES;

    capability[SCOM_CAPABILITY_START_IDX] =
                                GET_SCOM_SUPPPORTED |
                                PUT_SCOM_SUPPPORTED |
                                MODIFY_SCOM_SUPPPORTED |
                                MULTI_SCOM_SUPPPORTED |
                                PUT_SCOM_UNDER_MASK_SUPPPORTED ;
    capability[SCOM_CAPABILITY_START_IDX+1] =
                                RESERVED_A2_CAPABILITIES;

    capability[RING_CAPABILITY_START_IDX] =
                                GET_RING_SUPPPORTED |
                                PUT_RING_SUPPPORTED ;
    capability[RING_CAPABILITY_START_IDX+1] =
                                RESERVED_A3_CAPABILITIES;

    capability[MEMORY_CAPABILITY_START_IDX] =
                                GET_MEMORY_SUPPPORTED |
                                PUT_MEMORY_SUPPPORTED |
                                GET_SRAM_SUPPPORTED |
                                PUT_SRAM_SUPPPORTED;
    capability[MEMORY_CAPABILITY_START_IDX+1] =
                                RESERVED_A4_CAPABILITIES;

    capability[REGISTER_CAPABILITY_START_IDX] =
                                GET_REGISTER_SUPPPORTED |
                                PUT_REGISTER_SUPPPORTED |
                                GETHWREG_SUPPPORTED |
                                PUTHWREG_SUPPPORTED ;
    capability[REGISTER_CAPABILITY_START_IDX+1] =
                                RESERVED_A5_CAPABILITIES;

    capability[ARRAY_CAPABILITY_START_IDX] =
                                CONTROL_FAST_ARRAY_SUPPPORTED |
                                CONTROL_TRACE_ARRAY_SUPPPORTED;
    capability[ARRAY_CAPABILITY_START_IDX+1] =
                                RESERVED_A6_CAPABILITIES;

    capability[INSTRUCTION_CTRL_CAPABILITY_START_IDX] =
                                CONTROL_INSTRUCTIONS_SUPPPORTED;
    capability[INSTRUCTION_CTRL_CAPABILITY_START_IDX+1] =
                                RESERVED_A7_CAPABILITIES;

    capability[GENERIC_CHIPOP_CAPABILITY_START_IDX] =
                                GET_SBE_CAPABILITIES_SUPPORTED |
                                GET_SBE_FFDC_SUPPPORTED |
                                SBE_QUIESCE_SUPPPORTED;
    capability[GENERIC_CHIPOP_CAPABILITY_START_IDX+1] =
                                RESERVED_A8_CAPABILITIES;

    capability[MPIPL_CAPABILITY_START_IDX] =
                                ENTER_MPIPL_SUPPORTED |
                                CONTINUE_MPIPL_SUPPORTED |
                                STOP_CLOCKS_MPIPL_SUPPORTED |
                                GET_TI_INFO_SUPPORTED;
    capability[MPIPL_CAPABILITY_START_IDX+1] =
                                RESERVED_A9_CAPABILITIES;

    capability[DUMP_CAPABILITY_START_IDX] =
                                GET_DUMP_SUPPORTED;
    capability[DUMP_CAPABILITY_START_IDX+1] =
                                RESERVED_AA_CAPABILITIES;

    capability[HOST_FIFO_CAPABILITY_START_IDX] =
                                SBE_CMD_HALT_SUPPORTED;
    capability[HOST_FIFO_CAPABILITY_START_IDX+1] =
                                RESERVED_AB_CAPABILITIES;

}
#endif //__SBEFW_SEEPROM__

#ifndef __SBEFW_SEEPROM__
#endif //not __SBEFW_SEEPROM__
