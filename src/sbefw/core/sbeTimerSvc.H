/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeTimerSvc.H $                                */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2018                        */
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
 * @file: src/sbefw/sbeTimerSvc.H
 *
 * @brief This file contains the SBE Timer Service
 *
 */

#ifndef __SBEFW_TIMER_SVC_H
#define __SBEFW_TIMER_SVC_H
#include <stdint.h>
#include "pk_api.h"

// structure for timer service
struct timerService
{
    bool init;                /* Timer initialised */
    PkTimer fixedTimer;       /* pk timer */

    /**
      * @brief start timer
      *
      * @param[in] i_time       time in us
      * @param[in] i_callBack   callback function on timer expiry
      *
      * @return   error code as per sbe secondary errors
      */
    uint32_t startTimer(uint32_t i_time, PkTimerCallback i_callBack );

    /**
      * @brief stop timer
      *
      * @return   error code as per sbe secondary errors
      */
    uint32_t stopTimer();

    /**
      * @brief Returns timer status
      */
    inline bool isActive()
    {
        return ( init && pk_deque_is_queued( &(fixedTimer.deque) ));
    }
};

#endif //__SBEFW_TIMER_SVC_H
