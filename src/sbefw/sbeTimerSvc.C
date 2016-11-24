/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbeTimerSvc.C $                                     */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2017                        */
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
 * @file: src/sbefw/sbeTimerSvc.C
 *
 * @brief This file contains the SBE timer service
 *
 */

#include "sbetrace.H"
#include "sbeTimerSvc.H"
#include "sbe_sp_intf.H"
#include "pk.h"

uint32_t timerService::startTimer(uint32_t i_time, PkTimerCallback i_callBack )
{
    #define SBE_FUNC "timerService::startTimer "
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    do
    {
        int l_pkRc = 0;
        if( !init )
        {
            l_pkRc = pk_timer_create(&fixedTimer, i_callBack, NULL);
            if(l_pkRc)
            {
                SBE_ERROR(SBE_FUNC" Pk Timer Create failed, RC=[%d]", l_pkRc);
                l_rc = SBE_SEC_OS_FAILURE;
                break;
            }
            init = true;
        }
        if( isActive() )
        {
            SBE_ERROR(SBE_FUNC" Timer already started");
            l_rc = SBE_SEC_TIMER_ALREADY_STARTED;
            break;
        }
        // Schedule the timer
        l_pkRc = pk_timer_schedule(&fixedTimer,
                        PK_MILLISECONDS((uint32_t)i_time));
        if(l_pkRc)
        {
            SBE_ERROR(SBE_FUNC" Pk Timer Schedule failed, RC=[%d]", l_pkRc);
            l_rc = SBE_SEC_OS_FAILURE;
            break;
        }
    } while(0);
    return l_rc;
    #undef SBE_FUNC
}

uint32_t timerService::stopTimer( )
{
    #define SBE_FUNC "timerService::stopTimer "
    uint32_t l_rc = SBE_SEC_OPERATION_SUCCESSFUL;
    PkMachineContext  ctx;
    do
    {
        // The critical section enter/exit set is done to ensure
        // pk_timer_cancel operations are non-interrupible.
        pk_critical_section_enter(&ctx);
        if( !isActive() )
        {
            SBE_INFO(SBE_FUNC" Timer is not running");
            break;
        }
        // Cancel the timer
        int l_pkRc = pk_timer_cancel(&fixedTimer );
        if(l_pkRc)
        {
            SBE_ERROR(SBE_FUNC" Pk Timer cancel failed, RC=[%d]", l_pkRc);
            l_rc = SBE_SEC_OS_FAILURE;
            break;
        }
    } while(0);
    pk_critical_section_exit(&ctx);
    return l_rc;
    #undef SBE_FUNC
}

