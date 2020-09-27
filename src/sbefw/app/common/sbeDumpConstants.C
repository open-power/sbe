/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/common/sbeDumpConstants.C $                     */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2020                        */
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

#include "sbeDumpConstants.H"

DUMPTYPEMAP dumpTypeMap[5] = {
    {SBE_DUMP_TYPE_SCS , DUMP_TYPE_SCS},
    {SBE_DUMP_TYPE_MPIPL , DUMP_TYPE_MPIPL},
    {SBE_DUMP_TYPE_PERF , DUMP_TYPE_PERF},
    {SBE_DUMP_TYPE_CCS , DUMP_TYPE_CCS},
    {SBE_DUMP_TYPE_HB , DUMP_TYPE_HB}
};

//Index mapped table wrt commandTypes enum for storing the size of the genericHdctRow_t
//for a particular command type.
//Write NULL for commands that are not used in HDCT.txt
size_t genericHdctRowSize_table[NO_OF_CMD_TYPES] = {
    CMD_TYPE_NOT_USED,
    genericHdctRowSize(genericHdctRow_t,cmdGetScom),
    genericHdctRowSize(genericHdctRow_t,cmdPutScom),
    CMD_TYPE_NOT_USED,
    CMD_TYPE_NOT_USED,
    genericHdctRowSize(genericHdctRow_t,cmdGetRing),
    CMD_TYPE_NOT_USED,
    genericHdctRowSize(genericHdctRow_t,cmdStopClocks),
    CMD_TYPE_NOT_USED,
    CMD_TYPE_NOT_USED,
    CMD_TYPE_NOT_USED,
    genericHdctRowSize(genericHdctRow_t,cmdTraceArray),
    genericHdctRowSize(genericHdctRow_t,cmdFastArray),
    genericHdctRowSize(genericHdctRow_t,cmdGetMemPba),
    genericHdctRowSize(genericHdctRow_t,cmdGetSram)
};

uint16_t getEquivDumpType(uint8_t reqDumpType)
{
    #define SBE_FUNC " getEquivDumpType "

    uint8_t count = (sizeof(dumpTypeMap)/sizeof(dumpTypeMap[0]));
    uint16_t hdctDumpTypeMap = 0;
    for(uint8_t i = 0; i < count; i++)
    {
       if( dumpTypeMap[i].dumpRequestReceived == reqDumpType)
       {
            //Return as per HDCT.bin dump type format.
            hdctDumpTypeMap = (1 << dumpTypeMap[i].hdctDumpType);
       }
    }

    return hdctDumpTypeMap;
    #undef SBE_FUNC
}
