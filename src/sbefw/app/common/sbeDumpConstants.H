/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/common/sbeDumpConstants.H $                     */
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

#ifndef __SBEFW_SBEDUMPCONSTANTS_H
#define __SBEFW_SBEDUMPCONSTANTS_H

#include<stdint.h>
#include "sbe_sp_intf.H"

//Dump types(Dump types encoded as in HDCT.bin)
enum dumpTypes
{
    DUMP_TYPE_SCS = 0x00,
    DUMP_TYPE_CCS = 0x01,
    DUMP_TYPE_CSCX = 0x02,
    DUMP_TYPE_CCR = 0x03,
    DUMP_TYPE_CHCR = 0x04,
    DUMP_TYPE_MPIPL = 0x05,
    DUMP_TYPE_HB = 0x06,
    DUMP_TYPE_PM = 0x07,
    DUMP_TYPE_PERF = 0x08,
    DUMP_TYPE_CCRCLK = 0x09,
    DUMP_TYPE_CHCRCLK = 0x0A,

};

//Chip Unit types(Chip unit types encoded as in HDCT.bin)
enum chipUnitTypes
{
    CHIP_UNIT_TYPE_UNKNOWN = 0x00,
    CHIP_UNIT_TYPE_CHIP = 0x01,
    CHIP_UNIT_TYPE_MCS = 0x02,
    CHIP_UNIT_TYPE_C = 0x03,
    CHIP_UNIT_TYPE_EQ = 0x04,
    CHIP_UNIT_TYPE_EX = 0x05,
    CHIP_UNIT_TYPE_PERV = 0x06,
    CHIP_UNIT_TYPE_MCBIST = 0x07,
    CHIP_UNIT_TYPE_SYSTEM = 0x08,
    CHIP_UNIT_TYPE_PHB = 0x09,
    CHIP_UNIT_TYPE_MI = 0x0A,
    CHIP_UNIT_TYPE_MC = 0x0B,
    CHIP_UNIT_TYPE_PAUC = 0x0C,
    CHIP_UNIT_TYPE_IOHS = 0x0D,
    CHIP_UNIT_TYPE_NMMU = 0x0E,
    CHIP_UNIT_TYPE_CAPP = 0x0F,
    CHIP_UNIT_TYPE_XBUS = 0x10,
    CHIP_UNIT_TYPE_MCA = 0x11,
    CHIP_UNIT_TYPE_CBUS = 0x12,
    CHIP_UNIT_TYPE_PEC = 0x13,
    CHIP_UNIT_TYPE_MBA = 0x14,
    CHIP_UNIT_TYPE_DMI = 0x15,
    CHIP_UNIT_TYPE_THREAD = 0x16,
    CHIP_UNIT_TYPE_MCC = 0x17,
    CHIP_UNIT_TYPE_CMIC = 0x18,
    CHIP_UNIT_TYPE_PAU = 0x19,
    CHIP_UNIT_TYPE_OMI = 0x20,
};

//Command Types(Command types encoded as in HDCT.bin)
enum commandTypes
{
    CMD_GETSCOM = 0x01,
    CMD_PUTSCOM = 0x02,
    CMD_GETCFAM = 0x03,
    CMD_PUTCFAM = 0x04,
    CMD_GETRING = 0x05,
    CMD_GETBLOCK = 0x06,
    CMD_STOPCLOCKS = 0x07,
    CMD_GETGPR = 0x08,
    CMD_GETSPR = 0x09,
    CMD_GETFPR = 0x0A,
    CMD_GETTRACEARRAY = 0x0B,
    CMD_GETFASTARRAY = 0x0C,
    CMD_GETMEMPBA = 0x0D,
    CMD_GETSRAM = 0x0E,
};

//Bit Modifiers incase of putscom
enum bitModifier
{
    B_NONE = 0x00,
    B_OR  = 0x01,
    B_AND = 0x02,
};


//Struct to map requested/sbe_Supported dump type(e-bmc/fsp) enum with HDCT dump types enum
struct _dumpTypeMap_t
{
    sbeSupportedDumpTypes dumpRequestReceived;
    dumpTypes hdctDumpType;

};
typedef struct _dumpTypeMap_t DUMPTYPEMAP ;


/*
 * @brief Map the requested dump type with equivalent HDCT dump type.
 *        eg Checkstop, HB, MPIPL.
 *
 * @return void
 */
uint16_t getEquivDumpType(uint8_t reqDumpType);

#endif /* __SBEFW_SBEDUMPCONSTANTS_H */
