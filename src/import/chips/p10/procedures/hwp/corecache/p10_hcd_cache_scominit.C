/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_hcd_cache_scominit.C $ */
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
/// @file  p10_hcd_cache_scominit.C
/// @brief Applies L3 scom inits
///
/// *HWP HW Maintainer: David Du         <daviddu@us.ibm.com>
/// *HWP FW Maintainer: Prem Shanker Jha <premjha2@in.ibm.com>
/// *HWP Consumed by  : SBE,QME
///

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "p10_hcd_cache_scominit.H"
#include <p10_fbc_utils.H>
#include "p10_hcd_common.H"

#ifdef __PPE_QME
    #include <p10_l3_scom.H>
    #include "p10_scom_c_c.H"
    #include "p10_scom_c_e.H"
    #include "p10_ppe_c_7.H"
#else
    #include <p10_scom_c.H>
#endif

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Procedure: p10_hcd_cache_scominit_qme
//------------------------------------------------------------------------------

#ifdef __PPE_QME
static inline fapi2::ReturnCode p10_hcd_cache_scominit_qme(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST > & i_target)
{
    FAPI_INF(">>p10_hcd_cache_scominit_qme");

    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    auto l_chip = i_target.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();

    for (const auto& l_core : i_target.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        fapi2::ReturnCode l_rc;

        FAPI_DBG("Invoking p10.l3.scom.initfile on target");
        FAPI_EXEC_HWP(l_rc, p10_l3_scom, l_core, FAPI_SYSTEM, l_chip);
    }

fapi_try_exit:
    FAPI_INF("<<p10_hcd_cache_scominit_qme");
    return fapi2::current_err;
}

//------------------------------------------------------------------------------
// Procedure: p10_hcd_cache_scominit_sbe
//------------------------------------------------------------------------------

#else

namespace
{

inline int fastlog2(unsigned n)
{
    return (n == 0) ? 0 : __builtin_ctz(n);
}
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

static inline fapi2::ReturnCode p10_hcd_cache_scominit_sbe(
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

        FAPI_TRY(GET_L3_MISC_L3CERRS_BACKING_CTL_REG(c, l_data));
        // 1. enable castout to backing caches
        SET_L3_MISC_L3CERRS_BACKING_CTL_REG_CASTOUT_TO_BACKING_L3_EN_CFG((l_attr_backing_vec != 0), l_data);
        // 2. configure the number of backing caches
        SET_L3_MISC_L3CERRS_BACKING_CTL_REG_BACKING_CNT_CFG(fastlog2(l_attr_backing_num), l_data);
        FAPI_TRY(PUT_L3_MISC_L3CERRS_BACKING_CTL_REG(c, l_data));

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

//------------------------------------------------------------------------------
// Procedure: p10_hcd_cache_scominit
//------------------------------------------------------------------------------

fapi2::ReturnCode p10_hcd_cache_scominit(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST > & i_target)
{
    fapi2::buffer<buffer_t> l_mmioData = 0;

    FAPI_INF(">>p10_hcd_cache_scominit");

#ifdef __PPE_QME
    using namespace scomt::ppe_c;
    FAPI_TRY(p10_hcd_cache_scominit_qme(i_target));
#else
    using namespace scomt::c;
    FAPI_TRY(p10_hcd_cache_scominit_sbe(i_target));
#endif

    // Undo potential powerbus quiesce before last clock off, no-op for IPL
    FAPI_DBG("Drop PB_PURGE_REQ via PCR_SCSR[12]");
    FAPI_TRY( HCD_PUTMMIO_C( i_target, QME_SCSR_WO_CLEAR, MMIO_LOAD32H( BIT32(12) ) ) );

    FAPI_DBG("Drop L3_PM_RCMD_DIS_CFG and L3_PM_LCO_DIS_CFG via PM_LCO_DIS_REG[0,1]");
    FAPI_TRY( HCD_PUTSCOM_C( i_target, scomt::c::L3_MISC_L3CERRS_PM_LCO_DIS_REG, 0 ) );

    FAPI_DBG("Drop NCU_PM_RCMD_DIS_CFG via NCU_RCMD_QUIESCE_REG[0]");
    FAPI_TRY( HCD_PUTSCOM_C( i_target, scomt::c::NC_NCMISC_NCSCOMS_NCU_RCMD_QUIESCE_REG, 0 ) );

fapi_try_exit:
    FAPI_INF("<<p10_hcd_cache_scominit");
    return fapi2::current_err;
}
