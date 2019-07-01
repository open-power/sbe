/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/ppe/pk/test/ipc_func_tables.c $   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019                             */
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
#include "ipc_api.h"
#include "ipc_ping.h"
//extern ipc_msgq_t G_gpe0_test_msgq0;

// Function table for multi target (common) functions
IPC_MT_FUNC_TABLE_START

#ifdef IPC_ENABLE_PING
    IPC_HANDLER(ipc_ping_handler, 0)             // 0
#else
    IPC_HANDLER_DEFAULT                          // 0
#endif

IPC_HANDLER_DEFAULT                          // 1
IPC_HANDLER_DEFAULT                          // 2
IPC_HANDLER_DEFAULT                          // 3
IPC_HANDLER_DEFAULT                          // 4
IPC_HANDLER_DEFAULT                          // 5
IPC_HANDLER_DEFAULT                          // 6
IPC_HANDLER_DEFAULT                          // 7
IPC_MT_FUNC_TABLE_END

// Function table for single target (processor-specific) functions
IPC_ST_FUNC_TABLE_START
IPC_HANDLER_DEFAULT                          // 0
//IPC_MSGQ_HANDLER(&G_gpe0_test_msgq0)         // 0
IPC_HANDLER_DEFAULT                          // 1
IPC_HANDLER_DEFAULT                          // 2
IPC_HANDLER_DEFAULT                          // 3
IPC_HANDLER_DEFAULT                          // 4
IPC_HANDLER_DEFAULT                          // 5
IPC_HANDLER_DEFAULT                          // 6
IPC_HANDLER_DEFAULT                          // 7
IPC_HANDLER_DEFAULT                          // 8
IPC_HANDLER_DEFAULT                          // 9
IPC_HANDLER_DEFAULT                          // 10
IPC_HANDLER_DEFAULT                          // 11
IPC_HANDLER_DEFAULT                          // 12
IPC_HANDLER_DEFAULT                          // 13
IPC_HANDLER_DEFAULT                          // 14
IPC_HANDLER_DEFAULT                          // 15
IPC_ST_FUNC_TABLE_END
