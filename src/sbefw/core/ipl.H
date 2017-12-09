/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/ipl.H $                                        */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017,2018                        */
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
#pragma once

#include "fapi2.H"
#include "sbestates.H"

extern sbeRole g_sbeRole;
extern fapi2::ReturnCode g_iplFailRc;
/**
 * @brief Support function to execute specific istep
 *
 * @param[in] i_major Major Istep Number
 * @param[in] i_minor Minor Istep Number
 *
 * @return FAPI2_RC_SUCCESS if success, else error code.
 */
fapi2::ReturnCode sbeExecuteIstep (uint8_t i_major, uint8_t i_minor);

/**
 * @brief Executes IPL steps in continuous mode.
 *
 * @par On the master SBE, this will run
 *      all steps from 2.2 to 5.2. On the slave SBE, it runs all steps from 2.2
 *      to 3.18.
 *      In case an error is encountered, the execution is aborted.
 */
void sbeDoContinuousIpl();

using voidfuncptr_t = void (*)(void);
using iStep_t = fapi2::ReturnCode (*)( voidfuncptr_t );

struct istepMap_t
{
    iStep_t         istepWrapper;
    voidfuncptr_t   istepHwp;
};

struct istepTableEntry_t
{
    uint8_t         istepMajorNum;
    istepMap_t*     istepMinorArr;
    size_t          len;
};

struct istepTable_t
{
    istepTableEntry_t*  istepMajorArr;
    size_t              len;
};

#define ISTEP_MAP(wrapper, hwp) \
    {wrapper, reinterpret_cast<voidfuncptr_t>(hwp)}

#define ISTEP_ENTRY(majornum, entrytable) \
    {majornum, entrytable, sizeof(entrytable)/sizeof(entrytable[0])}

#define REGISTER_ISTEP_TABLE(entries) \
    istepTable_t istepTable = { \
        entries, \
        sizeof(entries)/sizeof(entries[0]) \
    };

extern istepTable_t istepTable;
