/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/cache/p9_hcd_cache_chiplet_reset.C $  */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* EKB Project                                                            */
/*                                                                        */
/* COPYRIGHT 2015,2016                                                    */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
///
/// @file  p9_hcd_cache_chiplet_reset.C
/// @brief Cache Chiplet Reset
///
/// Procedure Summary:
///   Reset quad chiplet logic
///   Clocking:
///    - setup cache sector buffer strength,
///      pulse mode and pulsed mode enable values
///      (attribute dependency Nimbus/Cumulus)
///    - Drop glsmux async reset
///   Scan0 flush entire cache chiplet

// *HWP HWP Owner          : David Du       <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still     <stillgs@us.ibm.com>
// *HWP FW Owner           : Sangeetha T S  <sangeet2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:SGPE
// *HWP Level              : 2

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <p9_misc_scom_addresses.H>
#include <p9_quad_scom_addresses.H>
#include <p9_perv_sbe_cmn.H>
#include <p9_hcd_common.H>
#include "p9_hcd_cache_chiplet_reset.H"

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Procedure: Cache Chiplet Reset
//------------------------------------------------------------------------------

fapi2::ReturnCode
p9_hcd_cache_chiplet_reset(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
{
    FAPI_INF(">>p9_hcd_cache_chiplet_reset");
    fapi2::buffer<uint64_t>                     l_data64;
    uint16_t                                    l_region_scan0;
    uint64_t                                    l_l2gmux_input       = 0;
    uint64_t                                    l_l2gmux_reset       = 0;
    uint8_t                                     l_attr_chip_unit_pos = 0;
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_chip =
        i_target.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();
    fapi2::Target<fapi2::TARGET_TYPE_PERV>      l_perv =
        i_target.getParent<fapi2::TARGET_TYPE_PERV>();
    auto l_core_functional_vector =
        i_target.getChildren<fapi2::TARGET_TYPE_CORE>
        (fapi2::TARGET_STATE_FUNCTIONAL);

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_perv,
                           l_attr_chip_unit_pos));
    l_attr_chip_unit_pos = l_attr_chip_unit_pos - p9hcd::PERV_TO_EQ_POS_OFFSET;

    FAPI_TRY(getScom(l_chip, PU_OCB_OCI_QCSR_SCOM, l_data64));
    FAPI_DBG("Working on cache[%d], good EXs in QCSR[%016llX]",
             l_attr_chip_unit_pos, l_data64);

    l_region_scan0 = p9hcd::SCAN0_REGION_ALL_BUT_EX;

    if (l_data64 & BIT64(l_attr_chip_unit_pos << 1))
    {
        l_region_scan0 |= p9hcd::SCAN0_REGION_EX0_L2_L3_REFR;
        l_l2gmux_reset |= BIT64(32);
        l_l2gmux_input |= BIT64(34);
    }

    if (l_data64 & BIT64((l_attr_chip_unit_pos << 1) + 1))
    {
        l_region_scan0 |= p9hcd::SCAN0_REGION_EX1_L2_L3_REFR;
        l_l2gmux_reset |= BIT64(33);
        l_l2gmux_input |= BIT64(35);
    }

    //--------------------------
    // Reset cache chiplet logic
    //--------------------------
    // If there is an unused, powered-off cache chiplet which needs to be
    // configured in the following steps to setup the PCB endpoint.

    for(auto it : l_core_functional_vector)
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               it.getParent<fapi2::TARGET_TYPE_PERV>(),
                               l_attr_chip_unit_pos));
        FAPI_DBG("Assert core[%d] DCC reset via NET_CTRL0[2]",
                 (l_attr_chip_unit_pos - p9hcd::PERV_TO_CORE_POS_OFFSET));
        FAPI_TRY(putScom(l_chip, (C_NET_CTRL0_WOR + (0x1000000 *
                                  (l_attr_chip_unit_pos - p9hcd::PERV_TO_CORE_POS_OFFSET))),
                         MASK_SET(2)));
    }

    FAPI_DBG("Init heartbeat hang counter");
    l_data64.flush<0>().setBit<2>();
    FAPI_TRY(putScom(i_target, EQ_HANG_PULSE_6_REG, l_data64));

    FAPI_DBG("Init NET_CTRL0[1-5,11-14,18,22,26],step needed for hotplug");
    l_data64 = p9hcd::Q_NET_CTRL0_INIT_VECTOR;
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0, l_data64));

    FAPI_DBG("Assert progdly/DCC bypass,L2 DCC reset via NET_CTRL1[1,2,23,24]");
    l_data64.flush<0>().insertFromRight<1, 2>(0x3).insertFromRight<23, 2>(0x3);
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL1_WOR, l_data64));

    FAPI_DBG("Flip cache glsmux to DPLL input via PPM_CGCR[3]");
    FAPI_TRY(putScom(i_target, EQ_PPM_CGCR, MASK_OR(0, 4, 0x9)));

    FAPI_DBG("Flip L2 glsmux to DPLL input via QPPM_EXCGCR[34:35]");
    FAPI_TRY(putScom(i_target, EQ_QPPM_EXCGCR_OR, l_l2gmux_input));

    FAPI_DBG("Assert DPLL ff_bypass via QPPM_DPLL_CTRL[2]");
    FAPI_TRY(putScom(i_target, EQ_QPPM_DPLL_CTRL_OR, MASK_SET(2)));

    FAPI_DBG("Drop vital thold via NET_CTRL0[16]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(16)));

    FAPI_DBG("Drop cache glsmux reset via PPM_CGCR[0]");
    FAPI_TRY(putScom(i_target, EQ_PPM_CGCR, MASK_SET(3)));

    FAPI_DBG("Drop L2 glsmux reset via QPPM_EXCGCR[32:33]");
    FAPI_TRY(putScom(i_target, EQ_QPPM_EXCGCR_CLEAR, l_l2gmux_reset));

    FAPI_DBG("Assert chiplet enable via NET_CTRL0[0]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WOR, MASK_SET(0)));

    FAPI_DBG("Drop PCB endpoint reset via NET_CTRL0[1]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(1)));

    FAPI_DBG("Drop chiplet electrical fence via NET_CTRL0[26]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(26)));

    FAPI_DBG("Drop PCB fence via NET_CTRL0[25]");
    FAPI_TRY(putScom(i_target, EQ_NET_CTRL0_WAND, MASK_UNSET(25)));

    FAPI_DBG("Set scan ratio to 1:1 in bypass mode via OPCG_ALIGN[47-51]");
    FAPI_TRY(getScom(i_target, EQ_OPCG_ALIGN, l_data64));
    l_data64.insertFromRight<47, 5>(0x0);
    FAPI_TRY(putScom(i_target, EQ_OPCG_ALIGN, l_data64));

#ifndef P9_HCD_STOP_SKIP_FLUSH
    //--------------------------------------------
    // perform scan0 module for pervasive chiplet
    //--------------------------------------------
    // Each scan0 will rotate the ring 8191 latches (2**13 - 1) and the longest
    // ring is defined by P9_HCD_SCAN_FUNC_REPEAT. When the design ALWAYS has
    // all stumps less than 8191, the loop can be removed.

    // Putting in block to avoid c++ crosses initialization compile error
    {
        uint32_t l_loop;

        FAPI_DBG("Scan0 region:all_but_vital type:gptr_repr_time rings");

        for(l_loop = 0; l_loop < P9_HCD_SCAN_GPTR_REPEAT; l_loop++)
            FAPI_TRY(p9_perv_sbe_cmn_scan0_module(l_perv,
                                                  l_region_scan0,
                                                  p9hcd::SCAN0_TYPE_GPTR_REPR_TIME));

        FAPI_DBG("Scan0 region:all_but_vital type:all_but_gptr_repr_time rings");

        for(l_loop = 0; l_loop < P9_HCD_SCAN_FUNC_REPEAT; l_loop++)
            FAPI_TRY(p9_perv_sbe_cmn_scan0_module(l_perv,
                                                  l_region_scan0,
                                                  p9hcd::SCAN0_TYPE_ALL_BUT_GPTR_REPR_TIME));
    }
#endif

    /// @todo scan_with_setpulse_module(L3 DCC)
    ///FAPI_DBG("Drop L3 DCC bypass via NET_CTRL1[1]");
    ///FAPI_TRY(putScom(i_target, EQ_NET_CTRL1_WAND, MASK_UNSET(1)));

fapi_try_exit:

    FAPI_INF("<<p9_hcd_cache_chiplet_reset");
    return fapi2::current_err;
}

