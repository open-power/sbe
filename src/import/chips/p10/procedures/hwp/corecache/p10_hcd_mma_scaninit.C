/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_hcd_mma_scaninit.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2021                        */
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
/// @file  p10_hcd_mma_scaninit.C
/// @brief
///

// *HWP HWP Owner          : David Du         <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still       <stillgs@us.ibm.com>
// *HWP FW Owner           : Prem Shanker Jha <premjha2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : QME
// *HWP Level              : 2


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "p10_hcd_mma_scaninit.H"
#include <p10_hcd_common.H>
#include <p10_pm_hcd_flags.h>
#include <p10_ring_id.H>
#include <p10_perv_sbe_cmn.H>


#ifdef __PPE_QME
    #include "p10_scom_eq_c.H"
    #include "p10_scom_eq_0.H"
    #include "p10_scom_eq_f.H"
    #include "p10_ppe_eq.H"
    #include "p10_ppe_c.H"
    using namespace scomt::ppe_eq;
    using namespace scomt::ppe_c;
    #include "qme.h"
    extern QmeRecord G_qme_record;
    #define QME_FLAGS_RUNNING_EPM QME_FLAGS_RUNNING_EPM
#else
    #include "p10_scom_eq.H"
    #include "p10_scom_c.H"
    using namespace scomt::eq;
    using namespace scomt::c;
    #define QME_FLAGS_RUNNING_EPM p10hcd::QME_FLAGS_RUNNING_EPM
#endif

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

enum P10_HCD_MMA_ARRAYINIT_CONSTANTS
{
    LOOP_COUNTER = 0x0000000000042FFF,
    START_ABIST_MATCH_VALUE = 0x0000000F00000000
};

//------------------------------------------------------------------------------
// Procedure: p10_hcd_mma_scaninit
//------------------------------------------------------------------------------

fapi2::ReturnCode
p10_hcd_mma_scaninit(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST > & i_target)
{

#if !defined P10_HCD_CORECACHE_SKIP_ARRAY || !defined P10_HCD_CORECACHE_SKIP_FLUSH
    fapi2::Target < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > eq_target =
        i_target.getParent < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST > ();
    fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > perv_target =
        eq_target.getParent < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > ();
    uint32_t                       l_regions            = i_target.getCoreSelect();
    uint32_t                       l_loop               = 0;
#endif
#ifndef P10_HCD_CORECACHE_SKIP_INITF
    fapi2::buffer<uint64_t>        l_data64             = 0;
    uint32_t                       l_eq_num             = 0;
    uint32_t                       l_core_num           = 0;
    fapi2::ATTR_CHIP_UNIT_POS_Type l_attr_chip_unit_pos = 0;
#endif
    fapi2::buffer<buffer_t> l_mmioData                  = 0;
    fapi2::buffer<uint64_t> l_scomData                  = 0;

    FAPI_INF(">>p10_hcd_mma_scaninit");
    FAPI_TRY(HCD_GETMMIO_Q( eq_target, MMIO_LOWADDR(QME_FLAGS_RW), l_mmioData ) );

#ifdef __PPE_QME

    if( ( G_qme_record.hcode_func_enabled & QME_EPM_BROADSIDE_ENABLE ) ||
        (  MMIO_GET ( MMIO_LOWBIT(QME_FLAGS_RUNNING_EPM) ) != 1 ) )
    {
#endif

#ifndef P10_HCD_CORECACHE_SKIP_FLUSH

        FAPI_DBG("Scan0 region:mma type:gptr_repr_time rings");

        for(l_loop = 0; l_loop < P10_HCD_SCAN0_GPTR_REPEAT; l_loop++)
            FAPI_TRY(p10_perv_sbe_cmn_scan0_module(perv_target,
                                                   (l_regions << SHIFT16(15)),
                                                   HCD_SCAN0_TYPE_GPTR_REPR_TIME));

        FAPI_DBG("Scan0 region:mma type:all_but_gptr_repr_time rings");

        for(l_loop = 0; l_loop < P10_HCD_SCAN0_FUNC_REPEAT; l_loop++)
            FAPI_TRY(p10_perv_sbe_cmn_scan0_module(perv_target,
                                                   (l_regions << SHIFT16(15)),
                                                   HCD_SCAN0_TYPE_ALL_BUT_GPTR_REPR_TIME));

#endif

#ifdef __PPE_QME
    }

#endif

#ifdef __PPE_QME

    if ( MMIO_GET ( MMIO_LOWBIT(QME_FLAGS_RUNNING_EPM) ) != 1 )
    {
#endif


#ifndef P10_HCD_CORECACHE_SKIP_INITF

        for (auto const& l_core : i_target.getChildren<fapi2::TARGET_TYPE_CORE>(fapi2::TARGET_STATE_FUNCTIONAL))
        {
            fapi2::Target<fapi2::TARGET_TYPE_EQ> l_eq = l_core.getParent<fapi2::TARGET_TYPE_EQ>();

            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                                   l_eq,
                                   l_attr_chip_unit_pos));
            l_eq_num = (uint32_t)l_attr_chip_unit_pos;
            static_cast<void>(l_eq_num);

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

            FAPI_DBG("Scan ec_mma_gptr ring");
            FAPI_TRY(fapi2::putRing(l_core, ec_mma_gptr,
                                    fapi2::RING_MODE_HEADER_CHECK),
                     "Error from putRing (ec_mma_gptr)");

            FAPI_DBG("Scan ec_mma_time ring");
            FAPI_TRY(fapi2::putRing(l_core, ec_mma_time,
                                    fapi2::RING_MODE_HEADER_CHECK),
                     "Error from putRing (ec_mma_time)");

            FAPI_DBG("Scan ec_mma_func ring");
            FAPI_TRY(fapi2::putRing(l_core, ec_mma_func,
                                    fapi2::RING_MODE_HEADER_CHECK),
                     "Error from putRing (ec_mma_func)");

        }

#endif

#ifdef __PPE_QME
    }

#endif

    /*it is confirmed that we do not need to arrayinit mma, keep the code here in case we need to do so
    #ifdef __PPE_QME

        if( ( G_qme_record.hcode_func_enabled & QME_EPM_BROADSIDE_ENABLE ) ||
            (  MMIO_GET ( MMIO_LOWBIT(QME_FLAGS_RUNNING_EPM) ) != 1 ) )
        {
    #endif

            FAPI_DBG("Assert sdis_n(flushing LCBES condition) via CPLT_CONF0[34]");
            FAPI_TRY( HCD_PUTSCOM_Q( eq_target, scomt::eq::CPLT_CONF0_WO_OR, SCOM_1BIT(34) ) );

    #ifndef P10_HCD_CORECACHE_SKIP_ARRAY
            FAPI_DBG("Arrayinit selected MMA regions");

            FAPI_TRY(p10_perv_sbe_cmn_array_init_module(perv_target,
                     (l_regions << SHIFT16(15)),
                     LOOP_COUNTER,
                     START_ABIST_MATCH_VALUE, false, false));

    #endif

    #ifndef P10_HCD_CORECACHE_SKIP_FLUSH
            FAPI_DBG("Scan0 region:mma type:all_but_gptr_repr_time rings");

            for(l_loop = 0; l_loop < P10_HCD_SCAN0_FUNC_REPEAT; l_loop++)
                FAPI_TRY(p10_perv_sbe_cmn_scan0_module(perv_target,
                                                       (l_regions << SHIFT16(15)),
                                                       HCD_SCAN0_TYPE_ALL_BUT_GPTR_REPR_TIME));

    #endif

            FAPI_DBG("Drop sdis_n(flushing LCBES condition) via CPLT_CONF0[34]");
            FAPI_TRY( HCD_PUTSCOM_Q( eq_target, scomt::eq::CPLT_CONF0_WO_CLEAR, SCOM_1BIT(34) ) );

    #ifdef __PPE_QME
        }

    #endif
    */

fapi_try_exit:

    FAPI_INF("<<p10_hcd_mma_scaninit");
    return fapi2::current_err;
}
