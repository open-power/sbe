/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeglobals.H $                                 */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017,2019                        */
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
#include "sbeSecureMemRegionManager.H"

#define SBE_GLOBAL sbeGlobal

constexpr size_t MAX_MAIN_STORE_REGIONS = 8;
constexpr size_t MAX_OCC_SRAM_REGIONS   = 1;

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

        /**
         * @brief Global semaphore for async task
         *
         *   This is used to send invoke commands on Async process
         *
         */
        PkSemaphore sbeSemAsyncProcess;

        #ifdef SBE_CONSOLE_SUPPORT
        // Binary semaphore to protect uart access
        PkSemaphore sbeUartBinSem;
        static bool sbeUartActive;
        #endif

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
        // Instance of Master EX core to be used for DMT FFDC collection
        uint8_t deadmanCore;
        // Cached HWP Return Code that hints at what HWP FFDC to collect
        uint32_t asyncFfdcRC;

        // SBE commit id
        static uint32_t fwCommitId;

        // hreset flow
        static bool isHreset;

        // Secure memory window arrays
        static secureMemRegion_t mainMemRegions[MAX_MAIN_STORE_REGIONS];
        static secureMemRegion_t occSramRegions[MAX_OCC_SRAM_REGIONS];

        // i2c mode register register
        static uint64_t i2cModeRegister;

        // Last failed rc's - for SBE internal ffdc
        static uint16_t failedPrimStatus;
        static uint16_t failedSecStatus;
        static uint16_t failedSeqId;
        static uint8_t  failedCmdClass;
        static uint8_t  failedCmd;

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
            sbeSemAsyncProcess(),
            SBERole(SBE_ROLE_MASTER),
            sbefreq(( 133 * 1000 * 1000)/SBE::SBE_TO_NEST_FREQ_FACTOR),
            sbeFWSecurityEnabled(1)
        {
        }
};
extern SBEGlobalsSingleton* sbeGlobal;

//This is a struct defintion for the local register. This local register will be
//used to save the SPI clock divider setting
//Size should be 16byte
//Note: Offset of these members are defined in sbe_link.H. Update them if you
//move the existing members
//TODO: Replace regX with the actual register number or give a meaning full name
struct sbe_local_regX {
    uint16_t spi_clock_divider: 12;
    uint16_t sec_boot_seeprom : 1; //Set if secondary boot seeprom is in use
    uint16_t sec_meas_seeprom : 1; //Set if secondary measurement sprm is in use
    uint16_t mpipl : 1;            //Set if mpipl
    uint32_t bitsreserved : 17;
    uint16_t reserved[2];
};

#endif //__SBE_GLOBALS_H

