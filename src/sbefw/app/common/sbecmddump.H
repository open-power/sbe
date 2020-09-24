/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/common/sbecmddump.H $                           */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020                             */
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
#pragma once
#include <stdint.h>
#include "sbe_sp_intf.H"

/**
 * @brief Structure for GetDump Chip-op (0xAA01)
 */
typedef struct
{
    uint32_t reserved:16;
    uint32_t clockState:8;
    uint32_t dumpType:8;

    /*
     * @brief Validate the dumpType with the sbe supported dump type.
     *        eg Checkstop, HB, MPIPL.
     * @return bool true  if the dumpType is sbe supported dump type.
     *              false if the dumpType is not sbe supported.
     */
    bool validateDumpType()
    {
        SBE_DEBUG("Validating Dump Type: %X", (uint8_t)dumpType);
        bool status = false;
        switch(dumpType)
        {
            case SBE_DUMP_TYPE_SCS:
            case SBE_DUMP_TYPE_MPIPL:
            case SBE_DUMP_TYPE_PERF:
            case SBE_DUMP_TYPE_CCS:
            case SBE_DUMP_TYPE_HB:
                status = true;
        };
        return status;
    }

    /*
     * @brief Validate the clock state.
     *        eg Clock on and clock off.
     * @return bool true  if right clock state.
     *              false if unsupported clock state.
     */
    bool validateClockState()
    {
        SBE_DEBUG("Validating Clock State: %X", (uint8_t)clockState);
        bool status = false;
        switch(clockState)
        {
            case SBE_DUMP_CLOCK_ON:
            case SBE_DUMP_CLOCK_OFF:
                status = true;
        };
        return status;
    }

}sbeGetDumpReq_t;

/**
 * @brief collect Hw Dump(0xAA01)
 *
 * @param[in] i_pArg Buffer to be passed to the function (not used as of now)
 *
 * @return    Rc from the FIFO access utility.
 */
uint32_t sbeGetDump(uint8_t *i_pArg);
