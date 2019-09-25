/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_hcd_cache_scominit.C $ */
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



///
/// @file  p10_hcd_cache_scominit.C
/// @brief
///

///
/// *HWP HW Maintainer: David Du         <daviddu@us.ibm.com>
/// *HWP FW Maintainer: Prem Shanker Jha <premjha2@in.ibm.com>
/// *HWP Consumed by  : SBE,QME
///

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "p10_hcd_cache_scominit.H"
#include <p10_scom_c.H>
#include <p10_scom_proc.H>
#include <p10_fbc_utils.H>

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Procedure: p10_hcd_cache_scominit
//------------------------------------------------------------------------------

namespace
{
///
/// @brief Initialize the L3 and NCU topology id table entries
/// @param[in] c                Reference to core target
/// @param[in] topo_scoms       Vector where each element is the content to write
///                             into the topology id table SCOM register.
///                             topo_scoms[0] contains reg value for entries  0.. 7
///                             topo_scoms[1] contains reg value for entries  8..15
///                             topo_scoms[2] contains reg value for entries 16..23
///                             topo_scoms[3] contains reg value for entries 24..31
///                             assert(topo_scoms.size() == 4)
/// @return fapi::ReturnCode    FAPI2_RC_SUCCESS on success, error otherwise
///
fapi2::ReturnCode init_topo_id_tables(const fapi2::Target < fapi2::TARGET_TYPE_CORE
                                      | fapi2::TARGET_TYPE_MULTICAST > & c,
                                      const std::vector<uint64_t>& topo_scoms)
{
    using namespace scomt::c;

    PREP_L3_MISC_L3CERRS_TOPOLOGY_TBL0_SCOM_RD(c);
    FAPI_TRY(PUT_L3_MISC_L3CERRS_TOPOLOGY_TBL0_SCOM_RD(c, topo_scoms[0]));
    PREP_L3_MISC_L3CERRS_TOPOLOGY_TBL1_SCOM_RD(c);
    FAPI_TRY(PUT_L3_MISC_L3CERRS_TOPOLOGY_TBL1_SCOM_RD(c, topo_scoms[1]));
    PREP_L3_MISC_L3CERRS_TOPOLOGY_TBL2_SCOM_RD(c);
    FAPI_TRY(PUT_L3_MISC_L3CERRS_TOPOLOGY_TBL2_SCOM_RD(c, topo_scoms[2]));
    PREP_L3_MISC_L3CERRS_TOPOLOGY_TBL3_SCOM_RD(c);
    FAPI_TRY(PUT_L3_MISC_L3CERRS_TOPOLOGY_TBL3_SCOM_RD(c, topo_scoms[3]));

    PREP_NC_NCMISC_NCSCOMS_NCU_TOPOTABLE_REG0(c);
    FAPI_TRY(PUT_NC_NCMISC_NCSCOMS_NCU_TOPOTABLE_REG0(c, topo_scoms[0]));
    PREP_NC_NCMISC_NCSCOMS_NCU_TOPOTABLE_REG1(c);
    FAPI_TRY(PUT_NC_NCMISC_NCSCOMS_NCU_TOPOTABLE_REG1(c, topo_scoms[1]));
    PREP_NC_NCMISC_NCSCOMS_NCU_TOPOTABLE_REG2(c);
    FAPI_TRY(PUT_NC_NCMISC_NCSCOMS_NCU_TOPOTABLE_REG2(c, topo_scoms[2]));
    PREP_NC_NCMISC_NCSCOMS_NCU_TOPOTABLE_REG3(c);
    FAPI_TRY(PUT_NC_NCMISC_NCSCOMS_NCU_TOPOTABLE_REG3(c, topo_scoms[3]));

fapi_try_exit:
    return fapi2::current_err;
}
};

#ifdef __PPE_QME
static inline fapi2::ReturnCode
p10_hcd_cache_scominit_qme(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST > & i_target)
{
    using namespace scomt::c;
    using namespace scomt::proc;

    fapi2::buffer<uint64_t> l_data;
    fapi2::ATTR_PROC_FABRIC_SL_DOMAIN_Type l_attr_sl_domain;
    fapi2::ATTR_PROC_CHIP_LCO_TARGETS_Type l_attr_chip_lco_targets;
    fapi2::ATTR_CHIP_UNIT_POS_Type l_attr_chip_unit_pos;
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_chip = i_target.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();
    fapi2::Target < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST > l_eq =
        i_target.getParent < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST > ();
    fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST,
          fapi2::MULTICAST_OR > c_mc_or = i_target;

    std::vector<uint64_t> l_topo_scoms;
    // Get the register values for the SCOMs to setup the topology id table
    FAPI_TRY(topo::get_topology_table_scoms(l_chip, l_topo_scoms));
    // Setup the topology id tables for L3 and NCU via multicast
    FAPI_TRY(init_topo_id_tables(i_target, l_topo_scoms));

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_FABRIC_SL_DOMAIN, l_chip, l_attr_sl_domain));

    if (l_attr_sl_domain == fapi2::ENUM_ATTR_PROC_FABRIC_SL_DOMAIN_CHIP)
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_CHIP_LCO_TARGETS, l_chip, l_attr_chip_lco_targets));
    }
    else
    {
        // Read which EQ this QME is on
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_eq, l_attr_chip_unit_pos));

        // Chip is split into hemispheres by even/odd EQ
        size_t hemisphere = l_attr_chip_unit_pos % 2;

        // Read Core Configuration
        FAPI_TRY(GET_TP_TPCHIP_OCC_OCI_OCB_CCSR_RW(l_chip, l_data));

        // l_attr_chip_lco_targets = hemisphere caches
        l_attr_chip_lco_targets = (l_data >> 32) & (0xF0F0F0F0 >> (hemisphere << 2));
    }

    unsigned num_caches = __builtin_popcount(l_attr_chip_lco_targets);

    // No point in setting up LCO with only one or two caches
    // (note: for two caches LCO requires a mode bit which is scan-only)
    if (num_caches < 3)
    {
        goto fapi_try_exit;
    }

    for (const auto& c : i_target.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, c, l_attr_chip_unit_pos));

        FAPI_TRY(GET_L3_MISC_L3CERRS_MODE_REG1(c, l_data));
        // 1. core-num is LCO ID
        SET_L3_MISC_L3CERRS_MODE_REG1_MY_LCO_TARGET_ID_CFG(l_attr_chip_unit_pos, l_data);
        FAPI_TRY(PUT_L3_MISC_L3CERRS_MODE_REG1(c, l_data));
    }

    // can write the targets and enable via multicast :D
    l_data = 0;
    // 2. all-good cores makeup the LCO targets
    SET_L3_MISC_L3CERRS_MODE_REG1_LCO_TARGETS_CFG(l_attr_chip_lco_targets, l_data);
    // 3. LCO enable multicast
    SET_L3_MISC_L3CERRS_MODE_REG1_LCO_ENABLE_CFG(l_data);
    FAPI_TRY(PUT_L3_MISC_L3CERRS_MODE_REG1(c_mc_or, l_data));

fapi_try_exit:
    return fapi2::current_err;
}
#else
static inline fapi2::ReturnCode
p10_hcd_cache_scominit_sbe (
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST > & i_target)
{
    using namespace scomt::c;

    unsigned l_lco_id;
    uint32_t l_attr_active_vec;
    uint32_t l_attr_backing_vec;

    fapi2::buffer<uint64_t> l_data;
    std::vector<fapi2::Target<fapi2::TARGET_TYPE_CORE>> l_active;
    std::vector<fapi2::Target<fapi2::TARGET_TYPE_CORE>> l_backing;
    fapi2::ATTR_ACTIVE_CORES_NUM_Type l_attr_active_num;
    fapi2::ATTR_BACKING_CACHES_NUM_Type l_attr_backing_num;
    fapi2::ATTR_CHIP_UNIT_POS_Type l_attr_chip_unit_pos;
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_chip = i_target.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();

    std::vector<uint64_t> l_topo_scoms;
    // Get the register values for the SCOMs to setup the topology id table
    FAPI_TRY(topo::get_topology_table_scoms(l_chip, l_topo_scoms));

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_ACTIVE_CORES_VEC, l_chip, l_attr_active_vec));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_ACTIVE_CORES_NUM, l_chip, l_attr_active_num));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_BACKING_CACHES_VEC, l_chip, l_attr_backing_vec));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_BACKING_CACHES_NUM, l_chip, l_attr_backing_num));

    l_active.reserve(l_attr_active_num);
    l_backing.reserve(l_attr_backing_num);

    for (const auto& c : i_target.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, c, l_attr_chip_unit_pos));

        if ((1 << (31 - l_attr_chip_unit_pos)) & l_attr_active_vec)
        {
            l_active.push_back(c);
        }
        else if ((1 << (31 - l_attr_chip_unit_pos)) & l_attr_backing_vec)
        {
            l_backing.push_back(c);
        }
    }

    l_lco_id = l_attr_backing_num;

    for (const auto& c : l_active)
    {
        // 0. setup the the L3 and NCU topology id tables
        FAPI_TRY(init_topo_id_tables(c, l_topo_scoms));

        if (l_attr_backing_num)
        {
            FAPI_TRY(GET_L3_MISC_L3CERRS_BACKING_CTL_REG(c, l_data));
            // 1. enable castout to backing caches
            SET_L3_MISC_L3CERRS_BACKING_CTL_REG_CASTOUT_TO_BACKING_L3_EN_CFG((l_attr_backing_vec != 0), l_data);
            // 2. configure the number of backing caches
            SET_L3_MISC_L3CERRS_BACKING_CTL_REG_BACKING_CNT_CFG(l_attr_backing_num, l_data);
            FAPI_TRY(PUT_L3_MISC_L3CERRS_BACKING_CTL_REG(c, l_data));
        }

        FAPI_TRY(GET_L3_MISC_L3CERRS_MODE_REG1(c, l_data));

        // 3. clear the LCO targets
        SET_L3_MISC_L3CERRS_MODE_REG1_LCO_TARGETS_CFG(0, l_data);

        if (l_attr_backing_num)
        {
            // 4. assign sequential LCO IDs starting at nbacking
            SET_L3_MISC_L3CERRS_MODE_REG1_MY_LCO_TARGET_ID_CFG(l_lco_id++, l_data);
            // 5. enable LCOs
            SET_L3_MISC_L3CERRS_MODE_REG1_LCO_ENABLE_CFG(l_data);
        }
        else
        {
            // 4. assign LCO ID matching core number
            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, c, l_attr_chip_unit_pos));
            SET_L3_MISC_L3CERRS_MODE_REG1_MY_LCO_TARGET_ID_CFG(l_attr_chip_unit_pos, l_data);
        }

        FAPI_TRY(PUT_L3_MISC_L3CERRS_MODE_REG1(c, l_data));
    }

    l_lco_id = 0;

    for (const auto& c : l_backing)
    {
        // 0. setup the the L3 and NCU topology id tables
        FAPI_TRY(init_topo_id_tables(c, l_topo_scoms));

        FAPI_TRY(GET_L3_MISC_L3CERRS_MODE_REG0(c, l_data));
        // 1. enable direct-mapped mode
        SET_L3_MISC_L3CERRS_MODE_REG0_L3_DMAP_CI_EN_CFG(l_data);
        FAPI_TRY(PUT_L3_MISC_L3CERRS_MODE_REG0(c, l_data));

        FAPI_TRY(GET_L3_MISC_L3CERRS_MODE_REG1(c, l_data));
        // 2. assign sequential LCO IDs starting at 0
        SET_L3_MISC_L3CERRS_MODE_REG1_MY_LCO_TARGET_ID_CFG(l_lco_id++, l_data);
        // 3. (optional / don't care) clear the LCO targets
        SET_L3_MISC_L3CERRS_MODE_REG1_LCO_TARGETS_CFG(0, l_data);
        // 4. enable LCOs
        SET_L3_MISC_L3CERRS_MODE_REG1_LCO_ENABLE_CFG(l_data);
        FAPI_TRY(PUT_L3_MISC_L3CERRS_MODE_REG1(c, l_data));
    }

fapi_try_exit:
    return fapi2::current_err;
}
#endif


fapi2::ReturnCode
p10_hcd_cache_scominit(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST > & i_target)
{
    FAPI_INF(">>p10_hcd_cache_scominit");

#ifdef __PPE_QME
    FAPI_TRY(p10_hcd_cache_scominit_qme(i_target));
#else
    FAPI_TRY(p10_hcd_cache_scominit_sbe(i_target));
#endif

fapi_try_exit:
    FAPI_INF("<<p10_hcd_cache_scominit");
    return fapi2::current_err;
}
