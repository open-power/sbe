/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeConsole.H $                                 */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2018                             */
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
#pragma once

#include "sbe_sp_intf.H"
#include "sbe_build_info.H"

#define VARG_COUNT_HELPER(_0, _1, _2, _3, _4, _5, _6, _7, N, ...) N
#define VARG_COUNT(...) VARG_COUNT_HELPER(, ##__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1, 0)

#define SBE_MSG_CONSOLE_1(msg) \
    SBE_UART_LOCK; \
    _SBE_MSG_CONSOLE(msg); \
    _SBE_MSG_CONSOLE("\n\r"); \
    SBE_UART_UNLOCK;

#define SBE_MSG_CONSOLE_2(msg1, msg2) \
    SBE_UART_LOCK; \
    _SBE_MSG_CONSOLE(msg1); \
    _SBE_MSG_CONSOLE(msg2); \
    _SBE_MSG_CONSOLE("\n\r"); \
    SBE_UART_UNLOCK;

#define SBE_MSG_CONSOLE_3(msg1, msg2, msg3) \
    SBE_UART_LOCK; \
    _SBE_MSG_CONSOLE(msg1); \
    _SBE_MSG_CONSOLE(msg2); \
    _SBE_MSG_CONSOLE(msg3); \
    _SBE_MSG_CONSOLE("\n\r"); \
    SBE_UART_UNLOCK;

#define SBE_MSG_CONSOLE_4(msg1, msg2, msg3, msg4) \
    SBE_UART_LOCK; \
    _SBE_MSG_CONSOLE(msg1); \
    _SBE_MSG_CONSOLE(msg2); \
    _SBE_MSG_CONSOLE(msg3); \
    _SBE_MSG_CONSOLE(msg4); \
    _SBE_MSG_CONSOLE("\n\r"); \
    SBE_UART_UNLOCK;

#define SBE_MSG_CONSOLE_HELPER_CALL(count, ...) SBE_MSG_CONSOLE_ ## count(__VA_ARGS__)
#define SBE_MSG_CONSOLE_HELPER(count, ...) SBE_MSG_CONSOLE_HELPER_CALL(count, __VA_ARGS__)
#define SBE_MSG_CONSOLE(...) SBE_MSG_CONSOLE_CHECK(__VA_ARGS__)

#ifndef SBE_CONSOLE_SUPPORT

#define SBE_UART_INIT
#define SBE_UART_DISABLE
#define SBE_UART_LOCK
#define SBE_UART_UNLOCK
#define _SBE_MSG_CONSOLE(msg)
#define SBE_MSG_CONSOLE_CHECK(...)

#else

#define SBE_UART_INIT uartInit()
#define SBE_UART_DISABLE uartDisable()
#define SBE_UART_LOCK uartLock()
#define SBE_UART_UNLOCK uartUnLock()

#define SBE_MSG_CONSOLE_CHECK(...) \
    if(SBE_GLOBAL->sbeUartActive) \
    { \
        SBE_MSG_CONSOLE_HELPER(VARG_COUNT(__VA_ARGS__), __VA_ARGS__) \
    }
// SBE messages
#define SBE_CONSOLE_WELCOME_MSG ("\n\r--== Welcome to SBE - CommitId[" STRINGIFY(SBE_COMMIT_ID) "] ==--")

#define _SBE_MSG_CONSOLE(msg) sbeMsgConsole(msg)

#define LPC_IO_SPACE        0xD0010000
#define LPC_MAX_IO_SPACE    (64*1024)

void uartInit(void);
void uartDisable(void);
void uartLock(void);
void uartUnLock(void);
void sbeMsgConsole(char const *msg);
void sbeMsgConsole(uint32_t num);

extern "C" void _putchar(char c);

//
/** UART Register Offsets */
enum
{
    RBR = 0,    ///< Recv Buffer
    THR = 0,    ///< Tran Holding
    DLL = 0,    ///< Divisor Latch LSB
    IER = 1,    ///< Interrupt Enable
    DLM = 1,    ///< Divisor Latch MSB
    FCR = 2,    ///< FIFO Control
    IIR = 2,    ///< Interrupt Identification
    LCR = 3,    ///< Line Control
    MCR = 4,    ///< Modem Control
    LSR = 5,    ///< Line Status
    MSR = 6,    ///< Modem Status
    SCR = 7,    ///< Scratch
};

/** Line Status Register (LSR) bit definitions */
enum
{
    LSR_DR = 0x01,    ///<  Data ready
    LSR_OE = 0x02,    ///<  Overrun
    LSR_PE = 0x04,    ///<  Parity error
    LSR_FE = 0x08,    ///<  Framing error
    LSR_BI = 0x10,    ///<  Break
    LSR_THRE = 0x20,  ///<  Xmit holding register empty
    LSR_TEMT = 0x40,  ///<  Xmitter empty
    LSR_ERR = 0x80,   ///<  Error
    LSR_BAD = 0xff,   ///<  Invalid value for LSR
};

/** Line Control Register (LCR) bit definitions */
enum
{
    LCR_DWL5 = 0x00,  ///<  Data word length: 5 bits
    LCR_DWL6 = 0x01,  ///<  Data word length: 6 bits
    LCR_DWL7 = 0x02,  ///<  Data word length: 7 bits
    LCR_DWL8 = 0x03,  ///<  Data word length: 8 bits

    LCR_STP1 = 0x00,  ///<  1 stop bits
    LCR_STP2 = 0x04,  ///<  1.5(5) or 2(6,7,8) stop bits

    LCR_NOP  = 0x00,  ///<  No Parity
    LCR_ODDP = 0x08,  ///<  Odd Parity
    LCR_EVEP = 0x18,  ///<  Even Parity
    LCR_HIP  = 0x28,  ///<  High Parity
    LCR_LOP  = 0x38,  ///<  Low Parity

    LCR_DLAB = 0x80,  ///<  DLL access
};

/** Modem Control Register (MCR) bit definitions */
enum
{
    MCR_DTR = 0x01,   ///<  Data terminal ready
    MCR_RTS = 0x02,   ///<  Request to send
};

/** FIFO Control Register (FCR) bit definitions */
enum
{
    FCR_ENF  = 0x01,   ///<  Enable FIFOs.
    FCR_CLFR = 0x02,   ///<  Clear Receive FIFO.
    FCR_CLFT = 0x04,   ///<  Clear Transmit FIFO
};

// uart config
static const uint64_t uartBase  = 0x3f8;
static const uint64_t uartBaud  = 115200;
static const uint64_t uartClock = 1843200;
static const uint64_t uartDivisor = ((uartClock / 16) / uartBaud);

#endif // SBE_CONSOLE_SUPPORT
