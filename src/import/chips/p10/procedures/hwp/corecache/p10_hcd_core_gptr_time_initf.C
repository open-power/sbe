/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_hcd_core_gptr_time_initf.C $ */
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



///
/// @file  p10_hcd_core_gptr_time_initf.C
/// @brief
///


// *HWP HWP Owner          : David Du         <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still       <stillgs@us.ibm.com>
// *HWP FW Owner           : Prem Shanker Jha <premjha2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:QME
// *HWP Level              : 1


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "p10_ring_id.H"
#include "p10_hcd_core_gptr_time_initf.H"
#include <p10_scom_eq_c.H>


//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Procedure: p10_hcd_core_gptr_time_initf
//------------------------------------------------------------------------------


fapi2::ReturnCode
p10_hcd_core_gptr_time_initf(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST > & i_target)
{
    FAPI_INF(">>p10_hcd_core_gptr_time_initf");

#ifndef P10_HCD_CORECACHE_SKIP_INITF

#ifndef __PPE_QME

    fapi2::buffer<uint64_t>        l_data64             = 0;
    uint32_t                       l_eq_num             = 0;
    uint32_t                       l_core_num           = 0;
    fapi2::ATTR_CHIP_UNIT_POS_Type l_attr_chip_unit_pos = 0;

#endif


    FAPI_DBG("Scan ec_cl2_gptr ring");
    FAPI_TRY(fapi2::putRing(i_target, ec_cl2_gptr,
                            fapi2::RING_MODE_HEADER_CHECK),
             "Error from putRing (ec_cl2_gptr)");

    FAPI_DBG("Scan ec_cl2_time ring");
    FAPI_TRY(fapi2::putRing(i_target, ec_cl2_time,
                            fapi2::RING_MODE_HEADER_CHECK),
             "Error from putRing (ec_cl2_time)");
#ifndef __PPE_QME

    for (auto const& l_core : i_target.getChildren<fapi2::TARGET_TYPE_CORE>(fapi2::TARGET_STATE_FUNCTIONAL))
    {
        fapi2::Target<fapi2::TARGET_TYPE_EQ> l_eq = l_core.getParent<fapi2::TARGET_TYPE_EQ>();

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               l_eq,
                               l_attr_chip_unit_pos));
        l_eq_num = (uint32_t)l_attr_chip_unit_pos;

        // Read partial good value from Chiplet Control 2
        FAPI_TRY(fapi2::getScom(l_eq, scomt::eq::CPLT_CTRL2_RW, l_data64));

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               l_core,
                               l_attr_chip_unit_pos));
        l_core_num = (uint32_t)l_attr_chip_unit_pos % 4;

        FAPI_DBG("Checking the good setting matches for EQ %d Core %d",
                 l_eq_num, l_core_num);

        // While one could assume that the the plaform target model matches the active partial good
        // settings in the hardware, let's not (at least until we get through some MPIPL and core
        // deconfiguration testing.

        FAPI_ASSERT((l_data64.getBit(5 + l_core_num)),
                    fapi2::CORE_REPAIR_FUNCTIONAL_TARGET_MISMATCH_PARTIAL_GOOD()
                    .set_CPLT_CTRL2(l_data64)
                    .set_CORE_NUM(l_core_num)
                    .set_EQ_NUM(l_eq_num),
                    "Configuration Mismatch: CPLT_CTRL2 partial good bit is not set.");

        FAPI_DBG("Scan ec_mma_gptr ring");
        FAPI_TRY(fapi2::putRing(l_core, ec_mma_gptr,
                                fapi2::RING_MODE_HEADER_CHECK),
                 "Error from putRing (ec_mma_gptr)");

        FAPI_DBG("Scan ec_mma_time ring");
        FAPI_TRY(fapi2::putRing(l_core, ec_mma_time,
                                fapi2::RING_MODE_HEADER_CHECK),
                 "Error from putRing (ec_mma_time)");
    }

#endif

fapi_try_exit:

#endif

    FAPI_INF("<<p10_hcd_core_gptr_time_initf");
    return fapi2::current_err;
}
