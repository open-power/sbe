/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: sbe/sbefw/sbeFFDCType.H $                                     */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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
#ifndef __SBE_FFDC_TYPE_H
#define __SBE_FFDC_TYPE_H

//Bit mapped identifiers
#define SBE_FFDC_ATTR_DUMP      0x0001
#define SBE_FFDC_TRACE_DUMP     0x0002
#define SBE_FFDC_ALL_DUMP       0xFFFF

/* Structure indicating the contents of FFDC package
 * value 'true' - field present;value 'false' - field not present
 * bit_0 - attribute dump
 * bit_1 - trace buffer dump
 * bit 2-31 - reserved
 */
typedef struct
{
    uint32_t attrField:1;
    uint32_t traceField:1;
    uint32_t reserved:30;
    /* @breif - set dump fields
     *
     * @param[in] - uint32_t value to be updated
     */
    void set(uint32_t val)
    {
        if(val & SBE_FFDC_ATTR_DUMP)
        {
            attrField = true;
        }
        if(val & SBE_FFDC_TRACE_DUMP)
        {
            traceField = true;
        }
    }
} sbeFFDCDumpFields_t;

//Sturcture indicating the type of ffdc user data blob
//and its length in bytes
typedef struct
{
    uint32_t fieldId:16;
    uint32_t fieldLen:16;
} sbeFFDCUserDataIdentifier_t;

//Structure of ffdc user data blob
typedef struct
{
    sbeFFDCUserDataIdentifier_t userDataId;
    const void *userDataPtr;
} sbeFFDCUserData_t;

//keep it packed to 4byte boundary to avoid packing bytes
typedef struct
{
    uint32_t primaryStatus:16;//Chip Op Primary status
    uint32_t secondaryStatus:16;//Chip Op Secondary status
    sbeFFDCDumpFields_t dumpFields;//bitmapped dumpFields
} sbeFFDCDataHeader_t;

#endif //__SBE_FFDC_TYPE_H
