/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/securebootcommon/sbemtrace.H $                      */
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
#ifndef __SBEFW_SBEM_TRACE_H
#define __SBEFW_SBEM_TRACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "pk_api.h"
#include "trac_interface.h"

#ifdef __cplusplus
}
#endif

#define  SBEM_ENTER_MRK ">>"
#define  SBEM_EXIT_MRK "<<"
#define  SBEM_ERR_MRK "E>"
#define  SBEM_INF_MRK "I>"
#define  SBEM_DEBUG_MRK "D>"

#define SBEM_ERROR(args...)
#define SBEM_INFO(args...)
#define SBEM_ENTER(args...)
#define SBEM_EXIT(args...)
#define SBEM_DEBUG(args...)

// Levels of logging
// 0 - No tracing
// 1 - Error
// 2 - Error, info
// 3 - Error, info, entry/exit
// 4 - Error, info, entry/exit, debug
#if (SBEM_TRACE_LEVEL >= 1)
#undef SBEM_ERROR
#define SBEM_ERROR(args...) PK_TRACE(SBEM_ERR_MRK"" args)
#endif
#if (SBEM_TRACE_LEVEL >= 2)
#undef SBEM_INFO
#define SBEM_INFO(args...) PK_TRACE(SBEM_INF_MRK"" args)
#endif
#if (SBEM_TRACE_LEVEL >= 3)
#undef SBEM_ENTER
#undef SBEM_EXIT
#define SBEM_ENTER(args...) PK_TRACE(SBEM_ENTER_MRK"" args)
#define SBEM_EXIT(args...) PK_TRACE(SBEM_EXIT_MRK"" args)
#endif
#if (SBEM_TRACE_LEVEL >= 4)
#undef SBEM_DEBUG
#define SBEM_DEBUG(args...) PK_TRACE(SBEM_DEBUG_MRK"" args)
#endif
#define SBEM_TRACE(args...) PK_TRACE(args)
#endif // __SBEFW_SBEM_TRACE_H
