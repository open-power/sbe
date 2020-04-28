/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbefapiutil.C $                                */
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
/*
 * @file: ppe/src/sbefw/sbefapiutil.C
 *
 * @brief This file contains the SBE FAPI Utility
 *
 */

#include "sbefapiutil.H"

using namespace fapi2;

fapi2::TargetType sbeGetFapiTargetType(const uint16_t i_sbeTargetType,
        const uint16_t i_chipletId)
{
    TargetType l_fapiTargetType = TARGET_TYPE_NONE;
    switch(i_sbeTargetType)
    {
        case TARGET_PERV:
            if((i_chipletId >= EQ_ID_0) && (i_chipletId <= EQ_ID_LAST))
            {
                l_fapiTargetType = fapi2::TARGET_TYPE_EQ;
            }
            else
            {
                l_fapiTargetType = fapi2::TARGET_TYPE_PERV;
            }
            break;
        case TARGET_EQ:
            l_fapiTargetType = fapi2::TARGET_TYPE_EQ;
            break;
        case TARGET_CORE:
            l_fapiTargetType = fapi2::TARGET_TYPE_CORE;
            break;
        case TARGET_PROC_CHIP:
            l_fapiTargetType = fapi2::TARGET_TYPE_PROC_CHIP;
            break;
    }
    return l_fapiTargetType;
}

bool sbeGetFapiTargetHandle(const uint16_t i_targetType,
                            const uint16_t i_chipletId,
                            fapi2::plat_target_handle_t &o_tgtHndl,
                            const fapi2::TargetType i_fapiTargetMask)
{
    bool l_rc = true;

    do
    {
        if(((i_targetType == TARGET_TYPE_CORE) &&
            (i_chipletId == SMT4_ALL_CORES)) ||
           ((i_targetType == TARGET_TYPE_EQ) &&
            (i_chipletId == EQ_ALL_CHIPLETS)))
        {
            // It's a valid combination for all cores/all EQs
            break;
        }
        fapi2::TargetType l_fapiTargetType = static_cast<TargetType>(
                                 sbeGetFapiTargetType(i_targetType, i_chipletId)
                                 & i_fapiTargetMask);
        switch(l_fapiTargetType)
        {
            case fapi2::TARGET_TYPE_PERV:
                o_tgtHndl = plat_getTargetHandleByChipletNumber
                                <fapi2::TARGET_TYPE_PERV>(i_chipletId);
                break;
            case fapi2::TARGET_TYPE_EQ:
                o_tgtHndl = plat_getTargetHandleByChipletNumber
                                <fapi2::TARGET_TYPE_EQ>(i_chipletId);
                break;
            case fapi2::TARGET_TYPE_CORE:
                o_tgtHndl = plat_getTargetHandleByInstance
                                <fapi2::TARGET_TYPE_CORE>(i_chipletId);
                break;
            case fapi2::TARGET_TYPE_PROC_CHIP:
                o_tgtHndl = plat_getChipTarget().get();
                break;
            default:
                l_rc = false;
                break;
        }
    } while(false);
    return l_rc;
}

uint16_t sbeToFapiRingMode(uint16_t i_ringMode)
{
    uint16_t l_fapiRingMode = RING_MODE_HEADER_CHECK;

    if(i_ringMode & SBE_RING_MODE_SET_PULSE_NO_OPCG_COND)
    {
        l_fapiRingMode |= RING_MODE_SET_PULSE_NO_OPCG_COND;
    }
    if(i_ringMode & SBE_RING_MODE_NO_HEADER_CHECK)
    {
        l_fapiRingMode |= RING_MODE_NO_HEADER_CHECK;
    }
    if(i_ringMode & SBE_RING_MODE_SET_PULSE_NSL)
    {
        l_fapiRingMode |= RING_MODE_SET_PULSE_NSL;
    }
    if(i_ringMode & SBE_RING_MODE_SET_PULSE_SL)
    {
        l_fapiRingMode |= RING_MODE_SET_PULSE_SL;
    }
    if(i_ringMode & SBE_RING_MODE_SET_PULSE_ALL)
    {
        l_fapiRingMode |= RING_MODE_SET_PULSE_ALL;
    }
    if(i_ringMode & SBE_RING_MODE_FASTARRAY)
    {
        l_fapiRingMode |= RING_MODE_FASTARRAY;
    }
    return l_fapiRingMode;
}


