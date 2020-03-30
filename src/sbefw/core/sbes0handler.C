/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbes0handler.C $                               */
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
/*
 * @file: ppe/src/sbefw/sbes0handler.C
 *
 * @brief This file contains the SBE S0 Handler
 *
 */

#include "sbefifo.H"
#include "sbeSpMsg.H"
#include "sbe_sp_intf.H"
#include "sbetrace.H"
#include "sbeFifoMsgUtils.H"
#include "sberegaccess.H"
#include "sbefapiutil.H"
#include "sbecmdiplcontrol.H"
#include "sbecmdmpipl.H"
#include "sbeFFDC.H"
#include "sbes0handler.H"
#include "sbearchregdump.H"

#include "fapi2.H"
#include "p9_perv_scom_addresses.H"

using namespace fapi2;

#ifdef __SBEFW_SEEPROM__

///////////////////////////////////////////////////////////////////////
// @brief sbeHandleS0
//
// @return  RC from the function
///////////////////////////////////////////////////////////////////////
uint32_t sbeHandleS0(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeHandleS0 "
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;

    do // Do while for both StartMpipl and ContinueMpipl
    {
        fapiRc = startMpiplIstepsExecute();
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            rc = SBE_SEC_S0_START_MPIPL_FAILED;
            SBE_ERROR(SBE_FUNC "Failed in Start Mpipl S0 Interface");
            break;
        }

        // Collect Architected Register Dump
        fapiRc = sbeDumpArchRegs();
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            rc = SBE_SEC_S0_ARCH_REG_DUMP_FAILED;
            SBE_ERROR(SBE_FUNC "Failed to collect ArchRegDump S0 Interface");
            break;
        }

        // TODO - RTC: 190585
        //Core and Cache stop Clock
        fapiRc = stopClockS0();
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            rc = SBE_SEC_S0_STOP_CLOCK_FAILED;
            SBE_ERROR(SBE_FUNC "Failed in Core/Cache StopClock S0 Interface");
            break;
        }

        sbeRole sbeRole = SbeRegAccess::theSbeRegAccess().isSbeMaster() ?
                   SBE_ROLE_MASTER : SBE_ROLE_SLAVE;

        if(sbeRole == SBE_ROLE_MASTER)
        {
            fapiRc = continueMpiplIstepsExecute(sbeRole);
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                rc = SBE_SEC_S0_CONTINUE_MPIPL_FAILED;
                SBE_ERROR(SBE_FUNC "Failed in Continue Mpipl S0 Interface, "
                        "SBE Role[%d]", sbeRole);
                break;
            }
        }
    }while(0);

    if(rc)
    {
        // Async Response to be stored
        captureAsyncFFDC(SBE_PRI_GENERIC_EXECUTION_FAILURE, rc);
    }
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

#endif // #ifdef __SBEFW_SEEPROM__

#ifndef __SBEFW_SEEPROM__

uint32_t clearS0interrupt()
{
    #define SBE_FUNC "clearS0interrupt"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = FAPI2_RC_SUCCESS;
    // Default Mask bit for S0 Innterrupt, bit 49
    uint64_t MASK_ALL_AND_MODIFY_BIT = 0xFFFDFFFFFFFFFFFFULL;
    
    do
    {
        Target<TARGET_TYPE_PROC_CHIP > procTgt = plat_getChipTarget();
        uint64_t data;
        rc = getscom_abs_wrap (&procTgt, PERV_SB_CS_SCOM, &data);
        if( rc )
        {
            break;
        }
        data = data & MASK_ALL_AND_MODIFY_BIT;
        rc = putscom_abs_wrap (&procTgt, PERV_SB_CS_SCOM, data);
        if( rc )
        {
            break;
        }
    }while(0);

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

#endif // #ifndef __SBEFW_SEEPROM__

