/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/common/include/p10_ppe_eq_3.H $          */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2020                        */
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

#ifndef __PPE_HCODE__
    #include "proc_scomt.H"
#endif

#ifndef __p10_ppe_eq_3_H_
#define __p10_ppe_eq_3_H_


#ifndef __PPE_HCODE__
namespace scomt
{
namespace ppe_eq
{
#endif


//>> [QME_DB1]
//ERRCODE: // Some of the name targets not found in address targets: eq
// No targets could be determined from the address.

static const uint32_t QME_DB1 = 0xc0001040ull;

static const uint32_t QME_DB1_NUMBER = 0;
static const uint32_t QME_DB1_NUMBER_LEN = 8;
static const uint32_t QME_DB1_DATA = 8;
static const uint32_t QME_DB1_DATA_LEN = 56;
//<< [QME_DB1]
// ppe_eq/reg00003.H

//>> [QME_PCBSID]
//ERRCODE: // Some of the name targets not found in address targets: eq

static const uint32_t QME_PCBSID = 0xc0000340ull;

static const uint32_t QME_PCBSID_EQ_VLD = 0;
static const uint32_t QME_PCBSID_EQ_MERGING_SCHEME = 2;
static const uint32_t QME_PCBSID_EQ_MERGING_SCHEME_LEN = 3;
static const uint32_t QME_PCBSID_SP_ACK = 7;
static const uint32_t QME_PCBSID_EQ_RD_NOT_WR = 8;
static const uint32_t QME_PCBSID_EQ_P = 9;
static const uint32_t QME_PCBSID_EQ_LOCAL_ADDR_P = 10;
static const uint32_t QME_PCBSID_EQ_PORT = 12;
static const uint32_t QME_PCBSID_EQ_PORT_LEN = 4;
static const uint32_t QME_PCBSID_EQ_LOCAL_ADDR = 16;
static const uint32_t QME_PCBSID_EQ_LOCAL_ADDR_LEN = 16;
//<< [QME_PCBSID]
// ppe_eq/reg00003.H

//>>THE END<<

#ifndef __PPE_HCODE__
}
}
#include "ppe_eq/reg00003.H"
#endif
#endif
