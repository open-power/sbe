/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbefapiutil.C $                                     */
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
/*
 * @file: ppe/src/sbefw/sbefapiutil.C
 *
 * @brief This file contains the SBE FAPI Utility
 *
 */

#include "sbefapiutil.H"

using namespace fapi2;

bool sbeGetFapiTargetHandle(uint16_t i_targetType,
                            uint16_t i_chipletId,
                            fapi2::plat_target_handle_t &o_tgtHndl)
{
    bool l_rc = true;
    switch(i_targetType)
    {
        case TARGET_EX:
            o_tgtHndl = plat_getTargetHandleByChipletNumber
                            <fapi2::TARGET_TYPE_EX>(i_chipletId);
            break;
        case TARGET_PERV:
            o_tgtHndl = plat_getTargetHandleByChipletNumber
                            <fapi2::TARGET_TYPE_PERV>(i_chipletId);
            break;
        case TARGET_EQ:
            o_tgtHndl = plat_getTargetHandleByChipletNumber
                            <fapi2::TARGET_TYPE_EQ>(i_chipletId);
            break;
        case TARGET_CORE:
            o_tgtHndl = plat_getTargetHandleByChipletNumber
                            <fapi2::TARGET_TYPE_CORE>(i_chipletId);
            break;
        case TARGET_PROC_CHIP:
            o_tgtHndl = plat_getChipTarget().get();
            break;
        default:
            l_rc = false;
            break;
    }
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


