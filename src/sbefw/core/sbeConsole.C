/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeConsole.C $                                 */
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
#include "sbetrace.H"
#include "fapi2.H"

#include "sbeConsole.H"

#include "p9_perv_scom_addresses.H"
#include "p9_perv_scom_addresses_fld.H"
#include "p9_misc_scom_addresses.H"
#include "p9_misc_scom_addresses_fld.H"

#include "sberegaccess.H"
#include "sbeglobals.H"
#include "p9_lpc_utils.H"

using namespace fapi2;

static uint32_t writeReg(uint8_t i_addr,
                  uint8_t i_data)
{
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;

    do {
        Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();

        buffer<uint32_t> data = 0;
        data.insert(i_data, 0, 8);

        ReturnCode fapiRc = lpc_rw(proc,
                                   LPC_IO_SPACE + uartBase + i_addr,
                                   sizeof(uint8_t),
                                   false,
                                   false,
                                   data);
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            rc = SBE_SEC_LPC_ACCESS_FAILED;
            break;
        }
    } while(0);

    return rc;
}

static uint32_t readReg(uint8_t i_addr,
                 uint8_t &o_data)
{
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;

    do
    {
        Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();

        buffer<uint32_t> data = 0;
        ReturnCode fapiRc = lpc_rw(proc,
                                   LPC_IO_SPACE + uartBase + i_addr,
                                   sizeof(uint8_t),
                                   true,
                                   false,
                                   data);
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            rc = SBE_SEC_LPC_ACCESS_FAILED;
            break;
        }
        data.extract(o_data, 0, 8);
    } while(0);

    return rc;
}

void uartInit(void)
{
    #define SBE_FUNC "uartInit"
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;

    uint8_t lpcConsoleCfg = 0;
    FAPI_ATTR_GET(fapi2::ATTR_LPC_CONSOLE_CNFG,
                  plat_getChipTarget(),
                  lpcConsoleCfg);
    if(!lpcConsoleCfg)
    {
        SBE_INFO(SBE_FUNC " ATTR_LPC_CONSOLE_CNFG not set");
        return;
    }
    do {
        // Check for existence of scratch register (and thus UART device).
        rc = writeReg(SCR, 'h');
        if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            SBE_ERROR(SBE_FUNC " failure to write scratch for uart detection");
            break;
        }
        uint8_t data = 0;
        rc = readReg(SCR, data);
        if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            SBE_ERROR(SBE_FUNC " failure to read scratch for uart detection");
            break;
        }
        if(data != 'h')
        {
            SBE_ERROR(SBE_FUNC " uart device not found!");
            break;
        }

        struct i2c_config {
            uint8_t reg;
            uint8_t data;
        };

        i2c_config configTable[] = {
            {LSR, 0x00}, // clear line control register
            {IER, 0x00}, // clear interrupt register
            {LCR, LCR_DLAB}, // set baud rate
            {DLL, uartDivisor & 0xff},
            {DLM, uartDivisor >> 8},
            {LCR, LCR_DWL8 | LCR_NOP | LCR_STP1}, // set 8N1 mode
            {MCR, MCR_RTS | MCR_DTR}, // enable Request-to-send/Data-terminal-ready
            {FCR, FCR_ENF | FCR_CLFR | FCR_CLFT}, // clear and enable FIFOs
        };

        uint8_t step = 0;
        for(auto config:configTable) {
            rc = writeReg(config.reg, config.data);
            if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
            {
                SBE_ERROR(SBE_FUNC " failure step [%d]", step);
                return;
            }
            step++;
        }

        SBE_INFO(SBE_FUNC " UART device initialized.");
        SBE_GLOBAL->sbeUartActive = true;
    } while(0);

    #undef SBE_FUNC
}

static void uartPutChar(char c)
{
    #define SBE_FUNC "uartPutChar"
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    static unsigned char tx_room = 16;
    if (tx_room < 1)
    {
        static const uint64_t DELAY_NS = 100;
        static const uint64_t DELAY_LOOPS = 100000000;

        uint64_t loops = 0;
        uint8_t data = 0;
        do {
            rc = readReg(LSR, data);
            if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
            {
                SBE_ERROR(SBE_FUNC " failure to read LSR");
                break;
            }
            if(data == LSR_BAD || (data & LSR_THRE))
            {
                break;
            }
            delay(DELAY_NS, 1000000);
        } while(++loops < DELAY_LOOPS);

        if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
        {
            SBE_ERROR(SBE_FUNC " LSR read error.");
        } else if(data == LSR_BAD) {
            SBE_ERROR(SBE_FUNC " LSR_BAD data error.");
        } else if(loops >= DELAY_LOOPS) {
            SBE_ERROR(SBE_FUNC " FIFO timeout.");
        } else {
	    tx_room = 16;
	}
    }

    rc = writeReg(THR, c);
    if(rc != SBE_SEC_OPERATION_SUCCESSFUL) {
        SBE_ERROR(SBE_FUNC " failure to write THR");
    } else {
        tx_room--;
    }

    #undef SBE_FUNC
}

void uartDisable(void)
{
    SBE_INFO("uart disabled");
    SBE_GLOBAL->sbeUartActive = false;
}

void uartLock(void)
{
    int rcPk = PK_OK;
    rcPk = pk_semaphore_pend (&SBE_GLOBAL->sbeUartBinSem, PK_WAIT_FOREVER);
    // PK API failure
    if (rcPk != PK_OK)
    {
        SBE_ERROR(SBE_FUNC"pk_semaphore_pend failed, "
                  "rcPk=%d, SBE_GLOBAL->sbeUartBinSem.count=%d",
                   rcPk, SBE_GLOBAL->sbeUartBinSem.count);
        pk_halt();
    }
}

void uartUnLock(void)
{
    int rcPk = PK_OK;
    rcPk = pk_semaphore_post(&SBE_GLOBAL->sbeUartBinSem);
    // PK API failure
    if (rcPk != PK_OK)
    {
        SBE_ERROR(SBE_FUNC"pk_semaphore_post failed, "
                  "rcPk=%d, SBE_GLOBAL->sbeUartBinSem.count=%d",
                   rcPk, SBE_GLOBAL->sbeUartBinSem.count);
        pk_halt();
    }
}

void sbeMsgConsole(uint32_t num)
{
    // 8 chars for max unit32_t and a null terminator
    char num_str[9] = {};

    int i = 0;
    if(num == 0)
        num_str[0] = '0';
    while(num)
    {
        num_str[i++] = (num % 10) + '0';
        num /= 10;
    }

    // reverse string
    char *start = num_str, *end = num_str + i-1;
    while(start < end)
    {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }

    sbeMsgConsole((char*)(num_str));
}

void sbeMsgConsole(char const *msg)
{
    size_t c = 0;
    while(msg[c] != '\0')
    {
        uartPutChar(msg[c++]);
    }
}
