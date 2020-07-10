/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_hcd_core_startclocks.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2018,2020                        */
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
/// @file  p10_hcd_core_startclocks.C
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

#include "p10_hcd_core_startclocks.H"
#include "p10_hcd_mma_startclocks.H"
#include "p10_hcd_corecache_clock_control.H"
#include "p10_hcd_common.H"

#ifdef __PPE_QME
    #include "p10_scom_eq.H"
    #include "p10_ppe_c.H"
    using namespace scomt::eq;
    using namespace scomt::ppe_c;
#else
    #include "p10_scom_eq.H"
    #include "p10_scom_c.H"
    using namespace scomt::eq;
    using namespace scomt::c;
#endif

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

enum P10_HCD_CORE_STARTCLOCKS_CONSTANTS
{
    HCD_ECL2_CLK_SYNC_DONE_POLL_TIMEOUT_HW_NS        = 100000, // 10^5ns = 100us timeout
    HCD_ECL2_CLK_SYNC_DONE_POLL_DELAY_HW_NS          = 1000,   // 1us poll loop delay
    HCD_ECL2_CLK_SYNC_DONE_POLL_DELAY_SIM_CYCLE      = 32000,  // 32k sim cycle delay
};

//------------------------------------------------------------------------------
// Procedure: p10_hcd_core_startclocks
//------------------------------------------------------------------------------

fapi2::ReturnCode
p10_hcd_core_startclocks(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > & i_target)
{
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST > l_target =
        i_target;//getChildren w/o and/or
    fapi2::Target < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > eq_target =
        i_target.getParent < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST > ();
    uint32_t                l_regions  = i_target.getCoreSelect() << SHIFT32(8);
    uint32_t                l_scsr     = 0;
    fapi2::buffer<uint64_t> l_scomData = 0;
    fapi2::buffer<buffer_t> l_mmioData = 0;
    uint32_t                       l_eq_num             = 0;
    uint32_t                       l_core_num           = 0;
    fapi2::ATTR_CHIP_UNIT_POS_Type l_attr_chip_unit_pos = 0;

#ifndef EQ_SKEW_ADJUST_DISABLE
    uint32_t                l_timeout = 0;
    fapi2::Target < fapi2::TARGET_TYPE_SYSTEM > l_sys;
    fapi2::ATTR_RUNN_MODE_Type                  l_attr_runn_mode;
    FAPI_TRY( FAPI_ATTR_GET( fapi2::ATTR_RUNN_MODE, l_sys, l_attr_runn_mode ) );
#endif

    FAPI_INF(">>p10_hcd_core_startclocks");

    FAPI_DBG("Enable ECL2 Skewadjust via CPMS_CGCSR[1:CL2_CLK_SYNC_ENABLE]");
    FAPI_TRY( HCD_PUTMMIO_C( i_target, CPMS_CGCSR_WO_OR, MMIO_1BIT(1) ) );

#ifndef EQ_SKEW_ADJUST_DISABLE

    FAPI_DBG("Check ECL2 Skewadjust Sync Done via CPMS_CGCSR[33:CL2_CLK_SYNC_DONE]");
    l_timeout = HCD_ECL2_CLK_SYNC_DONE_POLL_TIMEOUT_HW_NS /
                HCD_ECL2_CLK_SYNC_DONE_POLL_DELAY_HW_NS;

    do
    {
        FAPI_TRY( HCD_GETMMIO_C( i_target, MMIO_LOWADDR(CPMS_CGCSR), l_mmioData ) );

        // use multicastAND to check 1
        if( ( !l_attr_runn_mode ) &&
            ( MMIO_GET(MMIO_LOWBIT(33)) == 1 ) )
        {
            break;
        }

        fapi2::delay(HCD_ECL2_CLK_SYNC_DONE_POLL_DELAY_HW_NS,
                     HCD_ECL2_CLK_SYNC_DONE_POLL_DELAY_SIM_CYCLE);
    }
    while( (--l_timeout) != 0 );

    FAPI_ASSERT( ( l_attr_runn_mode ? ( MMIO_GET(MMIO_LOWBIT(33)) == 1 ) : (l_timeout != 0) ),
                 fapi2::ECL2_CLK_SYNC_DONE_TIMEOUT()
                 .set_ECL2_CLK_SYNC_DONE_POLL_TIMEOUT_HW_NS(HCD_ECL2_CLK_SYNC_DONE_POLL_TIMEOUT_HW_NS)
                 .set_CPMS_CGCSR(l_mmioData)
                 .set_CORE_TARGET(i_target),
                 "ERROR: ECL2 Clock Sync Done Timeout");

#endif

    FAPI_TRY( p10_hcd_corecache_clock_control(eq_target, l_regions, HCD_CLK_START ) );

    FAPI_DBG("Disable ECL2 Regional Fences via CPLT_CTRL1[5-8:ECL2_FENCES]");
    FAPI_TRY( HCD_PUTSCOM_Q( eq_target, CPLT_CTRL1_WO_CLEAR, SCOM_LOAD32H(l_regions) ) );

    for (auto const& l_core : l_target.getChildren<fapi2::TARGET_TYPE_CORE>())
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
        FAPI_TRY(fapi2::getScom(l_eq, CPLT_CTRL2_RW, l_scomData));

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               l_core,
                               l_attr_chip_unit_pos));
        l_core_num = (uint32_t)l_attr_chip_unit_pos % 4;

        FAPI_DBG("Checking the good setting matches for EQ %d Core %d",
                 l_eq_num, l_core_num);

        if( l_scomData.getBit(5 + l_core_num) == 0)
        {
            FAPI_DBG("Partial Bad detected for EQ %d Core %d, Skip",
                     l_eq_num, l_core_num);
            continue;
        }

        l_regions = BIT32((5 + l_core_num));

        FAPI_DBG("Enable ECL2 Regional PSCOMs via CPLT_CTRL3[5-8:ECL2_REGIONS]");
        FAPI_TRY( HCD_PUTSCOM_Q( eq_target, CPLT_CTRL3_WO_OR,  SCOM_LOAD32H(l_regions) ) );

    }

    // Start MMA clocks along/after core clocks
    // shared for both stop11 and stop3 path
    FAPI_TRY( p10_hcd_mma_startclocks( i_target ) );

    // Undo potential quiesces before last clock off, no-op for IPL
    l_scsr = ( BIT32(4) | BITS32(7, 2) | BIT32(22) );

#ifndef __PPE_QME
    // Only Drop QME_SCSR_AUTO_SPECIAL_WAKEUP_DISABLE for Istep4
    // due to auto mode bug, also assert pm_exit
    l_scsr |= BIT32(20);
    FAPI_TRY( HCD_PUTMMIO_C( i_target, QME_SCSR_WO_OR, MMIO_LOAD32H( BIT32(1) ) ) );
#endif

    FAPI_DBG("Drop HBUS_DISABLE/L2RCMD_INTF_QUIESCE/NCU_TLBIE_QUIESCE/AUTO_PMSR_SHIFT_DIS via PCR_SCSR[4,7,8,22]");
    FAPI_TRY( HCD_PUTMMIO_C( i_target, QME_SCSR_WO_CLEAR, MMIO_LOAD32H( l_scsr ) ) );

#ifndef __PPE_QME
    FAPI_DBG("Assert special wakeup on hostboot core via SPWKUP_OTR[0]");
    FAPI_TRY( HCD_PUTSCOM_C( i_target, QME_SPWU_OTR, SCOM_1BIT(0) ) );

    FAPI_DBG("Set core as ready to run in STOP history register");
    FAPI_TRY( HCD_PUTSCOM_C( i_target, QME_SSH_SRC, 0 ) );
#endif

fapi_try_exit:

    FAPI_INF("<<p10_hcd_core_startclocks");

    return fapi2::current_err;
}
