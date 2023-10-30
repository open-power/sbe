/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeglobals.C $                                 */
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
#include "sbetrace.H"
#include "sbeglobals.H"
#include "sbe_build_info.H"
////////////////////////////////////////////////////////////////
//// @brief Stacks for Non-critical Interrupts and Threads
//////////////////////////////////////////////////////////////////
// Moved it out-side the scope of Global Class for symbol generation in syms
uint8_t sbe_Kernel_NCInt_stack[SBE_NONCRITICAL_STACK_SIZE];
uint8_t sbeCommandReceiver_stack[SBE_THREAD_CMD_RECV_STACK_SIZE];
uint8_t sbeSyncCommandProcessor_stack[SBE_THREAD_SYNC_CMD_PROC_STACK_SIZE];
uint8_t sbeAsyncCommandProcessor_stack[SBE_THREAD_ASYNC_CMD_PROC_STACK_SIZE];

SBEGlobalsSingleton* sbeGlobal = &SBEGlobalsSingleton::getInstance();
SBEGlobalsSingleton& SBEGlobalsSingleton::getInstance()
{
    static SBEGlobalsSingleton iv_instance;
    return iv_instance;
}
// SBE commit id
uint32_t SBEGlobalsSingleton::fwCommitId = SBE_COMMIT_ID;

secureMemRegion_t SBEGlobalsSingleton::mainMemRegions[MAX_MAIN_STORE_REGIONS] = {};
secureMemRegion_t SBEGlobalsSingleton::occSramRegions[MAX_OCC_SRAM_REGIONS] = {};

uint64_t SBEGlobalsSingleton::i2cModeRegister = 0x004D000000000000ull;

uint16_t SBEGlobalsSingleton::failedPrimStatus = SBE_PRI_OPERATION_SUCCESSFUL;
uint16_t SBEGlobalsSingleton::failedSecStatus  = SBE_SEC_OPERATION_SUCCESSFUL;
uint16_t SBEGlobalsSingleton::failedSeqId      = 0;
uint8_t  SBEGlobalsSingleton::failedCmdClass   = 0;
uint8_t  SBEGlobalsSingleton::failedCmd        = 0;

bool  SBEGlobalsSingleton::isHreset = false;

#ifdef SBE_IPL_STATUS_LPC_SUPPORT
bool SBEGlobalsSingleton::sbeLPCActive = false;
#endif

#ifdef SBE_CONSOLE_SUPPORT
bool SBEGlobalsSingleton::sbeUartActive = false;
#endif
