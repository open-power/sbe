/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/measurement/sbemPcrStates.C $                       */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2021                             */
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
*  @file: ppe/sbe/sbefw/measurement/sbemPcrStates.C
*
*  @brief This file contains basic structure definition and updates
*
*/

#include "sbemPcrStates.H"
#include "sbemtrace.H"
#include "sbeglobals.H"
#include "sbeXipUtils.H"
#include "p10_scom_perv.H"
#include "p10_scom_proc_f.H"
#include "sha512.H"
#include "sbestates.H"

void securityState_PCR6::update(uint32_t sbe_role)
{
    #define SBEM_FUNC "securityState_PCR6::update"
    SBEM_ENTER(SBEM_FUNC);

    fapi2::buffer<uint64_t> cbs_cs_reg;

    //Fetch Jumper state
    getscom_abs(scomt::perv::FSXCOMP_FSXLOG_CBS_CS, &cbs_cs_reg());
    jumperState = cbs_cs_reg.getBit<5>();

    //Fetch Minimum Secure Version
    minimumSecureVersion = *(uint8_t*)(getXipOffsetAbs(P9_XIP_SECTION_SBE_SB_SETTINGS) + SHA512_DIGEST_LENGTH);

    SBEM_INFO("MinimumSecureVersion:[%d] Jumper State:[%d]", minimumSecureVersion, jumperState);

    SBEM_EXIT(SBEM_FUNC);
    #undef SBEM_FUNC
}

void securityState_PCR1::update(uint32_t sbe_role)
{
    #define SBEM_FUNC "securityState_PCR1::update"
    SBEM_ENTER(SBEM_FUNC);

    fapi2::buffer<uint64_t> MSMLockReg;
    fapi2::buffer<uint64_t> cbs_cs_reg;
    sbe_local_LFR lfrReg;

    //Fetch the Boot Seeprom Selection and Mpipl from LFR
    PPE_LVD(0xc0002040, lfrReg);

    //Fetch Jumper state
    getscom_abs(scomt::perv::FSXCOMP_FSXLOG_CBS_CS, &cbs_cs_reg());
    jumperState = cbs_cs_reg.getBit<5>();

    if (lfrReg.mpipl)
    {
        isMpipl = 0x1;
    }

    // Boot Seeprom Selection
    bootSeepromSelection = lfrReg.sec_boot_seeprom;

    //Check if primary proc
    if(sbe_role == SBE_ROLE_MASTER)
    {
        isPrimary = 0x1;
    }

    //Fetch Minimum Secure Version
    minimumSecureVersion = *(uint8_t*)(getXipOffsetAbs(P9_XIP_SECTION_SBE_SB_SETTINGS) + SHA512_DIGEST_LENGTH);

    //Fetch Measurement Seeprom Lock
    getscom_abs(scomt::proc::TP_TPCHIP_PIB_OTP_OTPC_M_EXPORT_REGL_STATUS, &MSMLockReg());
    mSeepromLock = MSMLockReg.getBit<12>();

    //Fetch Measurement Image version
    mSeepromVersion = *(uint32_t*)(getXipOffsetAbsMeasurement(P9_XIP_SECTION_SBE_SB_SETTINGS));

    SBEM_INFO("Jumper State: [%d] Primary Proc: [%d]  MeasurementSeepromLock: [%d] "
        "Minimum Secure Version: [%d]", jumperState, isPrimary, mSeepromLock, minimumSecureVersion);

    SBEM_INFO("Measurement Image Version [0x%08X], isMpipl[%d], bootSeepromSelection[%d]",
        mSeepromVersion, isMpipl, bootSeepromSelection);

    SBEM_EXIT(SBEM_FUNC);
    #undef SBEM_FUNC
}
