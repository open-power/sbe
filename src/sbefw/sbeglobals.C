/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbeglobals.C $                                      */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017                             */
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
