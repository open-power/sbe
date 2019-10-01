/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_initf.C $    */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019                             */
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
//------------------------------------------------------------------------------
/// @file  p10_sbe_initf.C
///
/// @brief Initialize necessary latches in available chiplets
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : FSP
//------------------------------------------------------------------------------

#include "p10_sbe_initf.H"
#include "p10_perv_sbe_cmn.H"

static const ring_setup_t ISTEP3_FURE_RINGS[] =
{
    { n0_fure,        IGNORE_PG, TARGET_CHIP,    0x2,  0x2,  0},
    { n1_fure,        IGNORE_PG, TARGET_CHIP,    0x3,  0x3,  0},
    { n1_nmmu1_fure,  0x9,       TARGET_CHIP,    0x3,  0x3,  0},
    { pci_fure,       IGNORE_PG, TARGET_CHIPLET, 0x8,  0x9,  0},
    { mc_fure,        IGNORE_PG, TARGET_CHIPLET, 0xC,  0xF,  0},
    { iohs0_fure,     IGNORE_PG, TARGET_CHIPLET, 0x18, 0x18, 0},
    { iohs0_pdl_fure, IGNORE_PG, TARGET_CHIPLET, 0x18, 0x18, 0},
    { iohs0_ndl_fure, 0x8,       TARGET_CHIPLET, 0x18, 0x18, 0},
    { iohs1_fure,     IGNORE_PG, TARGET_CHIPLET, 0x19, 0x19, 0},
    { iohs1_pdl_fure, IGNORE_PG, TARGET_CHIPLET, 0x19, 0x19, 0},
    { iohs1_ndl_fure, 0x8,       TARGET_CHIPLET, 0x19, 0x19, 0},
    { iohs2_fure,     IGNORE_PG, TARGET_CHIPLET, 0x1A, 0x1A, 0},
    { iohs2_pdl_fure, IGNORE_PG, TARGET_CHIPLET, 0x1A, 0x1A, 0},
    { iohs2_ndl_fure, 0x8,       TARGET_CHIPLET, 0x1A, 0x1A, 0},
    { iohs3_fure,     IGNORE_PG, TARGET_CHIPLET, 0x1B, 0x1B, 0},
    { iohs3_pdl_fure, IGNORE_PG, TARGET_CHIPLET, 0x1B, 0x1B, 0},
    { iohs3_ndl_fure, 0x8,       TARGET_CHIPLET, 0x1B, 0x1B, 0},
    { iohs4_fure,     IGNORE_PG, TARGET_CHIPLET, 0x1C, 0x1C, 0},
    { iohs4_pdl_fure, IGNORE_PG, TARGET_CHIPLET, 0x1C, 0x1C, 0},
    { iohs4_ndl_fure, 0x8,       TARGET_CHIPLET, 0x1C, 0x1C, 0},
    { iohs5_fure,     IGNORE_PG, TARGET_CHIPLET, 0x1D, 0x1D, 0},
    { iohs5_pdl_fure, IGNORE_PG, TARGET_CHIPLET, 0x1D, 0x1D, 0},
    { iohs5_ndl_fure, 0x8,       TARGET_CHIPLET, 0x1D, 0x1D, 0},
    { iohs6_fure,     IGNORE_PG, TARGET_CHIPLET, 0x1E, 0x1E, 0},
    { iohs6_pdl_fure, IGNORE_PG, TARGET_CHIPLET, 0x1E, 0x1E, 0},
    { iohs6_ndl_fure, 0x8,       TARGET_CHIPLET, 0x1E, 0x1E, 0},
    { iohs7_fure,     IGNORE_PG, TARGET_CHIPLET, 0x1F, 0x1F, 0},
    { iohs7_pdl_fure, IGNORE_PG, TARGET_CHIPLET, 0x1F, 0x1F, 0},
    { iohs7_ndl_fure, 0x8,       TARGET_CHIPLET, 0x1F, 0x1F, 0},
    { pau0_fure,      IGNORE_PG, TARGET_CHIP,    0x10, 0x10, 0},
    { pau1_fure,      IGNORE_PG, TARGET_CHIP,    0x11, 0x11, 0},
    { pau2_fure,      IGNORE_PG, TARGET_CHIP,    0x12, 0x12, 0},
    { pau3_fure,      IGNORE_PG, TARGET_CHIP,    0x13, 0x13, 0},
    { pau0_pau0_fure, 0x5,       TARGET_CHIP,    0x10, 0x10, 0},
    { pau1_pau3_fure, 0x5,       TARGET_CHIP,    0x11, 0x11, 0},
    { pau2_pau4_fure, 0x5,       TARGET_CHIP,    0x12, 0x12, 0},
    { pau2_pau5_fure, 0x6,       TARGET_CHIP,    0x12, 0x12, 0},
    { pau3_pau6_fure, 0x5,       TARGET_CHIP,    0x13, 0x13, 0},
    { pau3_pau7_fure, 0x6,       TARGET_CHIP,    0x13, 0x13, 0},
    { eq_fure,        IGNORE_PG, TARGET_CHIPLET, 0x20, 0x27, 1},
};

fapi2::ReturnCode p10_sbe_initf(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    FAPI_INF("p10_sbe_initf: Entering ...");

    FAPI_TRY(p10_perv_sbe_cmn_setup_putring(i_target_chip, ISTEP3_FURE_RINGS));

    FAPI_INF("p10_sbe_initf: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}
