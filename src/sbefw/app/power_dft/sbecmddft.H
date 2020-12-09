/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power_dft/sbecmddft.H $                         */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020,2021                        */
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
enum dftAvpCommands
{
    DFT_CMD_RANGE_IPL= 1,
    DFT_CMD_LOAD_SEEPROM_FROM_SBE= 2,
    DFT_CMD_START_DDS_FROM_GPE = 3,
};

namespace SBEAPP_DFT
{
    uint32_t sbeHandleRangeIpl (uint8_t *i_pArg);

    ReturnCode sbeDoRangeIpl (const size_t i_startMajor,
                             const size_t i_startMinor,
                             const size_t i_endMajor,
                             const size_t i_endMinor);

    uint32_t sbeSeepromLoad (uint8_t *i_pArg);

    uint32_t gpeStartDDS (uint8_t *i_pArg);
}

