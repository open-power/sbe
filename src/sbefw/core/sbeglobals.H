/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeglobals.H $                                 */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017,2020                        */
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

//This is a struct defintion for the local register 0xC0002040.
//This local register will be used to save the SPI clock divider setting
//MPIPL in progress setting
//What kind of reset is triggered in 0x50008, since otprom clears the reset bit
// ipl_reset which is the cbs_start flow
// hreset which is the runtime reset flow
// hreset_done bit to indicate SBE finished Hreset flow and back up to receive
// commands over fifo.
typedef struct sbe_local_LFR {
    uint32_t spi_clock_divider: 12; // bit0-11
    uint32_t sec_boot_seeprom : 1; //Bit 12 Set if sec boot sprm is in use
    uint32_t sec_meas_seeprom : 1; //Bit 13 Set if sec measure sprm is in use
    uint32_t mpipl : 1;            //Bit 14 Set if mpipl is triggered
    uint32_t ipl_reset : 1;        //Bit 15 Set if SBE is reset via 50008:bit12
    uint32_t runtime_reset : 1;    //Bit 16 Set if SBE is reset via 50008:bit13
    uint32_t hreset_done : 1;      //Bit 17 Set if HReset flow is done in FW
    uint32_t mpipl_reset_done : 1; //Bit 18 Set if Mpipl flow is done in FW
    uint32_t bitsreserved : 13;
    uint32_t reserved;

    sbe_local_LFR()
    {
        spi_clock_divider = 0;
        sec_boot_seeprom = 0;
        sec_meas_seeprom = 0;
        mpipl = 0;
        ipl_reset = 0;
        runtime_reset = 0;
        hreset_done = 0;
        mpipl_reset_done = 0;
    }
}sbe_local_LFR;

#endif //__SBE_GLOBALS_H

