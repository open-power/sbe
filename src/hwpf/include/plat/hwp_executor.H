/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/include/plat/hwp_executor.H $                        */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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
/**
 *  @file fapi2PlatHwpExecutor.H
 *
 *  @brief Defines the FAPI HWP Executor Macro.
 *
 *  The HWP Executor macro is called when a PLAT invoker function or a HWP wants
 *  to execute a HWP. Each platform can modify the macro to do any platform
 *  specific work to execute the HWP (e.g. dlopening a shared library)
 */

#ifndef FAPI2PLATHWPEXECUTOR_H_
#define FAPI2PLATHWPEXECUTOR_H_

/**
 * @brief HWP Executor macro
 *
 * By default, this macro just calls the HWP directly. If this cannot be done
 * then the platform needs to modify
 */

#include <return_code.H>
#include <target.H>



// Macro to execute an arbitrary function with an arbitray number of arguments. The
// function is in a shared lib that must be dlopened. All that is required is that
// there is a typedef for the function pointer available that is called <FUNC>_FP_t
// i.e. if the function is called foo, then the typedef is called foo_FP_t
#define FAPI_PLAT_EXEC_HWP(RC, FUNC, _args_...) \
{\
    RC = FUNC(_args_); \
}

#define FAPI_PLAT_EXEC_HWP_LAMBDA(FUNC, _args_...) \
[&]()->fapi2::ReturnCode \
{\
    FUNC(_args_); \
}()

#endif // FAPI2PLATHWPEXECUTOR_H_
