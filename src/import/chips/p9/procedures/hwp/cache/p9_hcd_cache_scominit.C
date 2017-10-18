/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/cache/p9_hcd_cache_scominit.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2017                        */
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
/// @file  p9_hcd_cache_scominit.C
/// @brief  Cache Customization SCOMs
///
/// Procedure Summary:
///   Apply any SCOM initialization to the cache
///   Stop L3 configuration mode
///   Configure Trace Stop on Xstop
///   DTS Initialization sequense
///

// *HWP HWP Owner          : David Du         <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still       <stillgs@us.ibm.com>
// *HWP FW Owner           : Prem Shanker Jha <premjha2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:SGPE
// *HWP Level              : 3

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <p9_misc_scom_addresses.H>
#include <p9_quad_scom_addresses.H>
#include <p9_hcd_common.H>
#include <p9_l2_scom.H>
#include <p9_l3_scom.H>
#include <p9_ncu_scom.H>
#include "p9_hcd_cache_scominit.H"

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Procedure: Cache Customization SCOMs
//------------------------------------------------------------------------------


fapi2::ReturnCode
p9_hcd_cache_scominit(
    const fapi2::Target<fapi2::TARGET_TYPE_EQ>& i_target)
{
    FAPI_INF(">>p9_hcd_cache_scominit");
    fapi2::ReturnCode                           l_rc;
    fapi2::buffer<uint64_t>                     l_qcsr;
    fapi2::buffer<uint64_t>                     l_data64;
    uint16_t                                    l_exlist;
    uint8_t                                     l_exid;
    uint8_t                                     l_exloop;
    uint8_t                                     l_excount = 0;
    uint8_t                                     l_attr_chip_unit_pos = 0;
    uint8_t                                     l_attr_sys_force_all_cores = 0;
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM>    l_sys;
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_chip =
        i_target.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();
    fapi2::Target<fapi2::TARGET_TYPE_PERV>      l_eq_perv =
        i_target.getParent<fapi2::TARGET_TYPE_PERV>();
    auto l_ex_targets = i_target.getChildren<fapi2::TARGET_TYPE_EX>();

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_eq_perv,
                           l_attr_chip_unit_pos));
    l_attr_chip_unit_pos = l_attr_chip_unit_pos - p9hcd::PERV_TO_EQ_POS_OFFSET;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_SYS_FORCE_ALL_CORES, l_sys,
                           l_attr_sys_force_all_cores));

    FAPI_TRY(getScom(l_chip, PU_OCB_OCI_QCSR_SCOM, l_qcsr));
    l_exlist = ((l_qcsr & BITS64(0, 12)) >> 48);
    FAPI_DBG("List of L3 Victims[%16X] from QCSR", l_exlist);

    for (l_exloop = 0; l_exloop < 12; l_exloop++)
    {
        if (l_exlist & BIT16(l_exloop))
        {
            l_excount++;
        }
    }

    for (l_exloop = 0; l_exloop < 2; l_exloop++)
    {
        if (!(l_exlist & BIT16((l_exloop + (l_attr_chip_unit_pos << 1)))))
        {
            FAPI_DBG("Apply EQ_FIR_MASK to Deconfigured EQ[%x]EX[%x]",
                     l_attr_chip_unit_pos, l_exloop);
            FAPI_TRY(getScom(i_target, EQ_FIR_MASK, l_data64));
            l_data64 |= ((BIT64(4) | BIT64(6) | BIT64(8) | BIT64(11)) >> l_exloop);
            FAPI_TRY(putScom(i_target, EQ_FIR_MASK, l_data64));
        }
    }

    // EX0 or only one configured EX target given: l_exloop = 0;
    // possible EX1 when two EX targets are given: l_exloop = 1;
    // (If you had to ask why not put l_exloop = 0 in the for loop syntax,
    //  well, the syntax "auto" doesnt allows it.)
    l_exloop = 0;

    for (auto l_iter = l_ex_targets.begin(); l_iter != l_ex_targets.end(); l_iter++, l_exloop++)
    {
        // Hostboot mode(not force_all_cores) is the default IPL mode
        // That is, one mater core is bought up through istep4 by SBE
        // and then being powered off along with its cache in istep15
        // via stop11 function provided by SGPE.
        //
        // 1)The defined target pattern being processed by stop11 is
        //   Core[X]
        //   L2 of the EX where Core[X] is on
        //   L3 of EX0 if configured, L3 of EX1 if configured
        //   Quad where Core[X] is on
        //   (by defination, the EX with core has to be configured)
        //
        // Therefore, hostboot through istep4 has to bring up the
        // same elements above that will be shutdown by istep15.
        //
        // 2)In another word, the targets for istep4 needs to be
        //   Core[X]
        //   Both EXs if both configured
        //     Require following special handling in istep4:
        //       a) skip the L2 that Core[X] isnt on, but
        //       b) bring up L3 that Core[X] isnt on(or both L3)
        //   If only one EX is configured, then it has to be the EX
        //     that Core[X] is on, bring up both L2+L3 on that EX
        //   Quad where Core[X] is on
        //
        // 3)Master core is defined to be chip's 1st configured core
        //   Now select_ex in istep3 will provide such targets above
        //   a) Master core is in the first EX of a quad,
        //        AND second EX is also configured.
        //      Set master core in CCSR, set both EXes in QCSR
        //      (This is the case requires istep4 special handling)
        //   b) Master core is in the first EX of a quad,
        //        AND second EX is deconfigured
        //      Set master core in CCSR, set master EX in QCSR
        //   c) Master core is in the second EX, by defination,
        //        the first EX must have been deconfigured
        //      Set master core in CCSR, set master EX in QCSR
        //   Note: There wont be any problems on targets or require
        //   special handling in procedure for case b) and c) above
        //
        // Now we only skip L2 if we are in HB mode(not force_all_cores)
        // AND we are give second configured EX to process(l_exloop==1)
        if (l_attr_sys_force_all_cores || (!l_exloop))
        {
            FAPI_EXEC_HWP(l_rc, p9_l2_scom, *l_iter, l_sys, l_chip);

            if (l_rc)
            {
                FAPI_ERR("Error from p9_l2_scom (p9.l2.scom.initfile)");
                fapi2::current_err = l_rc;
                goto fapi_try_exit;
            }
        }

        FAPI_EXEC_HWP(l_rc, p9_l3_scom, *l_iter, l_sys, l_chip);

        if (l_rc)
        {
            FAPI_ERR("Error from p9_l3_scom (p9.l3.scom.initfile)");
            fapi2::current_err = l_rc;
            goto fapi_try_exit;
        }

        FAPI_EXEC_HWP(l_rc, p9_ncu_scom, *l_iter, l_sys, l_chip);

        if (l_rc)
        {
            FAPI_ERR("Error from p9_ncu_scom (p9.ncu.scom.initfile)");
            fapi2::current_err = l_rc;
            goto fapi_try_exit;
        }

        if (l_attr_sys_force_all_cores)
        {
            fapi2::Target<fapi2::TARGET_TYPE_EX> l_ex = *l_iter;
            auto l_core_targets = l_ex.getChildren<fapi2::TARGET_TYPE_CORE>();

            FAPI_ASSERT((l_core_targets.size() != 0),
                        fapi2::CACHE_SCOMINIT_NO_GOOD_CORE_IN_EX()
                        .set_QCSR(l_qcsr)
                        .set_CACHE_TARGET(i_target),
                        "NO Good Children Cores under this So-Called Good EX!");

            auto l_core = l_core_targets.begin();
            fapi2::Target<fapi2::TARGET_TYPE_CORE> l_ec   = *l_core;
            fapi2::Target<fapi2::TARGET_TYPE_PERV> l_perv =
                l_ec.getParent<fapi2::TARGET_TYPE_PERV>();

            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_perv,
                                   l_attr_chip_unit_pos));
            l_attr_chip_unit_pos = l_attr_chip_unit_pos - p9hcd::PERV_TO_CORE_POS_OFFSET;
            l_exid               = (l_attr_chip_unit_pos >> 1);

        }
        else
        {
            l_exid = l_exloop;
        }

        FAPI_DBG("Setup L3-LCO on TARGET_ID[%d] via EX_L3_MODE_REG1[0,2-5,6-21]", l_exid);
        FAPI_TRY(getScom(*l_iter, EX_L3_MODE_REG1, l_data64));
        l_data64.insertFromRight<2, 4>(l_exid).insertFromRight<6, 16>(l_exlist);

        if (l_excount > 1)
        {
            l_data64.setBit<0>();
        }

        FAPI_TRY(putScom(*l_iter, EX_L3_MODE_REG1, l_data64));

        FAPI_TRY(getScom(*l_iter, EX_L3_MODE_REG0, l_data64));

        if (l_excount == 2)
        {
            FAPI_DBG("Assert L3_DYN_LCO_BLK_DIS_CFG on TARGET_ID[%d] via EX_L3_MODE_REG0[9]", l_exid);
            FAPI_TRY(putScom(*l_iter, EX_L3_MODE_REG0, DATA_SET(9)));
        }
        else
        {
            FAPI_DBG("Drop L3_DYN_LCO_BLK_DIS_CFG on TARGET_ID[%d] via EX_L3_MODE_REG0[9]", l_exid);
            FAPI_TRY(putScom(*l_iter, EX_L3_MODE_REG0, DATA_UNSET(9)));
        }
    }

    FAPI_DBG("Enable DTS via THERM_MODE_REG[5,6-9,20-21]");
    FAPI_TRY(getScom(i_target, EQ_THERM_MODE_REG, l_data64));
    // DTS sampling enable | sample pulse count | DTS loop1 enable
    l_data64.setBit<5>().insertFromRight<6, 4>(0xF).insertFromRight<20, 2>(0x3);
    FAPI_TRY(putScom(i_target, EQ_THERM_MODE_REG, l_data64));

fapi_try_exit:

    FAPI_INF("<<p9_hcd_cache_scominit");
    return fapi2::current_err;
}




