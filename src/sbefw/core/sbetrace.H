/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbetrace.H $                                   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2017                        */
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
#ifndef __SBEFW_SBE_TRACE_H
#define __SBEFW_SBE_TRACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "pk_api.h"
#include "trac_interface.h"

#ifdef SBE_CONSOLE_SUPPORT
#include "printf.H"
#endif

#ifdef __cplusplus
}
#endif

#define  SBE_ENTER_MRK ">>"
#define  SBE_EXIT_MRK "<<"
#define  SBE_ERR_MRK "E>"
#define  SBE_INF_MRK "I>"
#define  SBE_DEBUG_MRK "D>"

#define SBE_ERROR(args...)
#define SBE_INFO(args...)
#define SBE_ENTER(args...)
#define SBE_EXIT(args...)
#define SBE_DEBUG(args...)

#if defined(SBE_CONSOLE_SUPPORT) && defined(ENABLE_CONSOLE_TRACE_IN_FILE)
#define PK_TRACE_INTERNAL(args...) {	\
	printf(args);			\
	printf("\n\r");			\
}
#else
#define PK_TRACE_INTERNAL(args...) PK_TRACE(args)
#endif

// Levels of logging
// 0 - No tracing
// 1 - Error
// 2 - Error, info
// 3 - Error, info, entry/exit
// 4 - Error, info, entry/exit, debug
#if (SBE_TRACE_LEVEL >= 1)
#undef SBE_ERROR
#define SBE_ERROR(args...) PK_TRACE_INTERNAL(SBE_ERR_MRK"" args)
#endif
#if (SBE_TRACE_LEVEL >= 2)
#undef SBE_INFO
#define SBE_INFO(args...) PK_TRACE_INTERNAL(SBE_INF_MRK"" args)
#endif
#if (SBE_TRACE_LEVEL >= 3)
#undef SBE_ENTER
#undef SBE_EXIT
#define SBE_ENTER(args...) PK_TRACE_INTERNAL(SBE_ENTER_MRK"" args)
#define SBE_EXIT(args...) PK_TRACE_INTERNAL(SBE_EXIT_MRK"" args)
#endif
#if (SBE_TRACE_LEVEL >= 4)
#undef SBE_DEBUG
#define SBE_DEBUG(args...) PK_TRACE_INTERNAL(SBE_DEBUG_MRK"" args)
#endif
#define SBE_TRACE(args...) PK_TRACE_INTERNAL(args)
#endif // __SBEFW_SBE_TRACE_H
