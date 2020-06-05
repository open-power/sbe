/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_hcd_core_initf.C $ */
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
/// @file  p10_hcd_core_initf.C
/// @brief
///


// *HWP HWP Owner          : David Du         <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still       <stillgs@us.ibm.com>
// *HWP FW Owner           : Prem Shanker Jha <premjha2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:QME
// *HWP Level              : 2


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "p10_ring_id.H"
#include "p10_hcd_core_initf.H"
#include <p10_scom_eq_c.H>


//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Procedure: p10_hcd_core_initf
//------------------------------------------------------------------------------


fapi2::ReturnCode
p10_hcd_core_initf(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST > & i_target)
{
    FAPI_INF(">>p10_hcd_core_initf");

#ifndef P10_HCD_CORECACHE_SKIP_INITF

    fapi2::buffer<uint64_t>        l_data64             = 0;
    uint32_t                       l_eq_num             = 0;
    uint32_t                       l_core_num           = 0;
    fapi2::ATTR_CHIP_UNIT_POS_Type l_attr_chip_unit_pos = 0;

    FAPI_DBG("Scan ec_cl2_func ring");
    FAPI_TRY(fapi2::putRing(i_target, ec_cl2_func,
                            fapi2::RING_MODE_HEADER_CHECK),
             "Error from putRing (ec_cl2_func)");

    FAPI_DBG("Scan ec_cl2_mode ring");
    FAPI_TRY(fapi2::putRing(i_target, ec_cl2_mode,
                            fapi2::RING_MODE_HEADER_CHECK),
             "Error from putRing (ec_cl2_mode)");

    for (auto const& l_core : i_target.getChildren<fapi2::TARGET_TYPE_CORE>(fapi2::TARGET_STATE_FUNCTIONAL))
    {
        fapi2::Target<fapi2::TARGET_TYPE_EQ> l_eq = l_core.getParent<fapi2::TARGET_TYPE_EQ>();

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               l_eq,
                               l_attr_chip_unit_pos));
        l_eq_num = (uint32_t)l_attr_chip_unit_pos;

        // do this to avoid unused variable warning
        do
        {
            (void)( l_eq_num );
        }
        while (0);

        // Read partial good value from Chiplet Control 2
        FAPI_TRY(fapi2::getScom(l_eq, scomt::eq::CPLT_CTRL2_RW, l_data64));

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               l_core,
                               l_attr_chip_unit_pos));
        l_core_num = (uint32_t)l_attr_chip_unit_pos % 4;

        FAPI_DBG("Checking the good setting matches for EQ %d Core %d",
                 l_eq_num, l_core_num);

        if( l_data64.getBit(5 + l_core_num) == 0)
        {
            FAPI_DBG("Partial Bad detected for EQ %d Core %d, Skip",
                     l_eq_num, l_core_num);
            continue;
        }

        FAPI_DBG("Scan ec_mma_func ring");
        FAPI_TRY(fapi2::putRing(l_core, ec_mma_func,
                                fapi2::RING_MODE_HEADER_CHECK),
                 "Error from putRing (ec_mma_func)");

    }

fapi_try_exit:

#endif

    FAPI_INF("<<p10_hcd_core_initf");
    return fapi2::current_err;
}
