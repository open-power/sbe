/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_sbe_tp_arrayinit.C $ */
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
/// @file  p10_sbe_tp_arrayinit.C
///
/// @brief SBE PRV Array Init Procedure
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : SBE
//------------------------------------------------------------------------------

#include "p10_sbe_tp_arrayinit.H"
#include "p10_scom_perv_0.H"
#include <p10_perv_sbe_cmn.H>
#include <target_filters.H>

enum P10_SBE_TP_ARRAYINIT_Private_Constants
{
    REGIONS_EXCEPT_PIB_NET_SBE_OCC_PLL = 0x438F, // excluding PAU_DPLL, NEST_DPLL, PERV_PLL
    SCAN_TYPES = 0xDCF,
    LOOP_COUNTER = 0x0000000000042FFF,
    START_ABIST_MATCH_VALUE = 0x0000000F00000000,
    PIBMEM_EXCLUDE_ABIST = 0xC000000000000000,
    PIBMEM_INCLUDE_ABIST = 0x8000000000000000
};

static fapi2::ReturnCode p10_sbe_tp_arrayinit_sdisn_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet,
    const bool i_set);

fapi2::ReturnCode p10_sbe_tp_arrayinit(const
                                       fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip)
{
    fapi2::buffer<uint16_t> l_regions;
    fapi2::buffer<uint64_t> l_data64;

    fapi2::Target<fapi2::TARGET_TYPE_PERV> l_tpchiplet =
        i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>(fapi2::TARGET_FILTER_TP, fapi2::TARGET_STATE_FUNCTIONAL)[0];

    FAPI_INF("p10_sbe_tp_arrayinit: Entering ...");

    FAPI_DBG("set sdis_n");
    FAPI_TRY(p10_sbe_tp_arrayinit_sdisn_setup(l_tpchiplet , true));

    FAPI_DBG("Exclude PIBMEM from TP array init");
    FAPI_TRY(fapi2::getScom(i_target_chip, 0x0008FFF0, l_data64 ));
    l_data64.setBit<5>();
    FAPI_TRY(fapi2::putScom(i_target_chip, 0x0008FFF0, l_data64 ));


    FAPI_DBG("Call ARRAY INIT Module for Pervasive Chiplet");
    FAPI_TRY(p10_perv_sbe_cmn_array_init_module(l_tpchiplet, REGIONS_EXCEPT_PIB_NET_SBE_OCC_PLL,
             LOOP_COUNTER, START_ABIST_MATCH_VALUE));

    FAPI_DBG("Call SCAN0 Module for Pervasive Chiplet");
    FAPI_TRY(p10_perv_sbe_cmn_scan0_module(l_tpchiplet, REGIONS_EXCEPT_PIB_NET_SBE_OCC_PLL, SCAN_TYPES));

    FAPI_DBG("Add PIBMEM back to TP array init");
    l_data64.clearBit<5>();
    FAPI_TRY(fapi2::putScom(i_target_chip, 0x0008FFF0, l_data64));

    FAPI_DBG("reset sdis_n");
    FAPI_TRY(p10_sbe_tp_arrayinit_sdisn_setup(l_tpchiplet, false));

    FAPI_INF("p10_sbe_tp_arrayinit: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}

/// @brief Sdis_n set or clear
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PERV target
/// @param[in]     i_set           set or clear the LCBES condition
/// @return  FAPI2_RC_SUCCESS if success, else error code.
static fapi2::ReturnCode p10_sbe_tp_arrayinit_sdisn_setup(
    const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chip,
    const bool i_set)
{
    using namespace scomt::perv;

    fapi2::buffer<uint64_t> l_data64;
    FAPI_INF("p10_sbe_tp_arrayinit_sdisn_setup: Entering ...");

    if ( i_set )
    {
        //Setting CPLT_CONF0 register value
        l_data64.flush<0>();
        l_data64.setBit<CPLT_CONF0_CTRL_CC_SDIS_DC_N>();
        FAPI_TRY(fapi2::putScom(i_target_chip, CPLT_CONF0_WO_OR, l_data64));
    }
    else
    {
        //Setting CPLT_CONF0 register value
        l_data64.flush<0>();
        l_data64.setBit<CPLT_CONF0_CTRL_CC_SDIS_DC_N>();
        FAPI_TRY(fapi2::putScom(i_target_chip, CPLT_CONF0_WO_CLEAR, l_data64));
    }

    FAPI_INF("p10_sbe_tp_arrayinit_sdisn_setup: Exiting ...");

fapi_try_exit:
    return fapi2::current_err;

}
