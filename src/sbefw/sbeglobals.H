/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbeglobals.H $                                      */
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
#ifndef __SBE_GLOBALS_H
#define __SBE_GLOBALS_H

#include "sbeutil.H"
#include "sbeSpMsg.H"
#include "sbeHostMsg.H"
#include "sbestates.H"
#include "sbeexeintf.H"
#include "sbecmdgeneric.H"

#define SBE_GLOBAL sbeGlobal

// Extern declartion, Defined in sbeglobal.C
extern uint8_t sbe_Kernel_NCInt_stack[SBE_NONCRITICAL_STACK_SIZE];
extern uint8_t sbeCommandReceiver_stack[SBE_THREAD_CMD_RECV_STACK_SIZE];
extern uint8_t sbeSyncCommandProcessor_stack[SBE_THREAD_SYNC_CMD_PROC_STACK_SIZE];
extern uint8_t sbeAsyncCommandProcessor_stack[SBE_THREAD_ASYNC_CMD_PROC_STACK_SIZE];

class SBEGlobalsSingleton
{
    public:
        // Disable copy contructor and assingment operator
        SBEGlobalsSingleton(const SBEGlobalsSingleton&) = delete;
        SBEGlobalsSingleton& operator=(const SBEGlobalsSingleton&) = delete;
        static SBEGlobalsSingleton& getInstance();

        sbeFifoCmdReqBuf_t sbeFifoCmdHdr;
        sbeCmdRespHdr_t sbeCmdRespHdr;
        sbePsu2SbeCmdReqHdr_t sbePsu2SbeCmdReqHdr;
        sbeSbe2PsuRespHdr_t sbeSbe2PsuRespHdr;
        sbeIntrHandle_t sbeIntrSource;
        ////////////////////////////////////////////////////////////////
        //// @brief Global semaphores
        /////////////////////////////////////////////////////////////////
        /**
         * @brief Global semaphore : SBE_GLOBAL->sbeSemCmdRecv
         *
         *   This is used to synchronize between the ISR and
         *   the command receiver thread.
         *
         */
        PkSemaphore sbeSemCmdRecv;
        /**
         * @brief Global semaphore : SBE_GLOBAL->sbeSemCmdProcess
         *
         *   This is used to synchronize between command receiver thread
         *   and synchronous command processor thread.
         *
         */
        PkSemaphore sbeSemCmdProcess;
        sbeRole SBERole;
        // SBE Frequency. Initially nest frequency is 133 MHZ
        uint32_t sbefreq;
        // Host specified memory allocations
        // passthrough command address
        sbeHostAddr_t hostPassThroughCmdAddr;
        // ffdc address
        sbeHostAddr_t hostFFDCAddr;
        // Key Addr Pair
        sbeStashMemoryPair_t sbeKeyAddrPair;
        // SBE FW security enabled; 0 - disabled; 1 - enabled
        uint8_t sbeFWSecurityEnabled;

        ////////////////////////////////////////////////////////////////
        //// @brief PkThread structure for SBE Command Receiver thread
        //////////////////////////////////////////////////////////////////
        PkThread sbeCommandReceiver_thread;
        ////////////////////////////////////////////////////////////////
        //// @brief PkThread structure for SBE Synchronous ChipOps
        ////        processing thread
        //////////////////////////////////////////////////////////////////
        PkThread sbeSyncCommandProcessor_thread;
        ////////////////////////////////////////////////////////////////
        ////// @brief PkThread structure for SBE Asynchronous ChipOps
        //////        processing thread
        //////////////////////////////////////////////////////////////////
        PkThread sbeAsyncCommandProcessor_thread;
    private:
        /* Constructor
         */
        SBEGlobalsSingleton():
            sbeFifoCmdHdr(),
            sbeCmdRespHdr(),
            sbePsu2SbeCmdReqHdr(),
            sbeSbe2PsuRespHdr(),
            sbeIntrSource(),
            sbeSemCmdRecv(),
            sbeSemCmdProcess(),
            SBERole(SBE_ROLE_MASTER),
            sbefreq(( 133 * 1000 * 1000)/SBE::SBE_TO_NEST_FREQ_FACTOR),
            sbeFWSecurityEnabled(1)
        {
        }
};
extern SBEGlobalsSingleton* sbeGlobal;
#endif //__SBE_GLOBALS_H

