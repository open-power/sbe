/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/verification/sbevtrace.H $                          */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2021                        */
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
#ifndef __SBEFW_SBEV_TRACE_H
#define __SBEFW_SBEV_TRACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "pk_api.h"
#include "trac_interface.h"

#ifdef __cplusplus
}
#endif

#define  SBEV_ENTER_MRK ">>"
#define  SBEV_EXIT_MRK "<<"
#define  SBEV_ERR_MRK "E>"
#define  SBEV_INF_MRK "I>"
#define  SBEV_DEBUG_MRK "D>"

#define SBEV_ERROR(args...)
#define SBEV_INFO(args...)
#define SBEV_ENTER(args...)
#define SBEV_EXIT(args...)
#define SBEV_DEBUG(args...)

// Levels of logging
// 0 - No tracing
// 1 - Error
// 2 - Error, info
// 3 - Error, info, entry/exit
// 4 - Error, info, entry/exit, debug
#if (SBEV_TRACE_LEVEL >= 1)
#undef SBEV_ERROR
#define SBEV_ERROR(args...) PK_TRACE(SBEV_ERR_MRK"" args)
#endif
#if (SBEV_TRACE_LEVEL >= 2)
#undef SBEV_INFO
#define SBEV_INFO(args...) PK_TRACE(SBEV_INF_MRK"" args)
#endif
#if (SBEV_TRACE_LEVEL >= 3)
#undef SBEV_ENTER
#undef SBEV_EXIT
#define SBEV_ENTER(args...) PK_TRACE(SBEV_ENTER_MRK"" args)
#define SBEV_EXIT(args...) PK_TRACE(SBEV_EXIT_MRK"" args)
#endif
#if (SBEV_TRACE_LEVEL >= 4)
#undef SBEV_DEBUG
#define SBEV_DEBUG(args...) PK_TRACE(SBEV_DEBUG_MRK"" args)
#endif
#define SBEV_TRACE(args...) PK_TRACE(args)
#endif // __SBEFW_SBEV_TRACE_H
