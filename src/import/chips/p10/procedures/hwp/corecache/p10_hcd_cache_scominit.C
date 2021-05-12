/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_hcd_cache_scominit.C $ */
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
    #include <p10_ncu_scom.H>
    #include "p10_scom_c_c.H"
    #include "p10_scom_c_e.H"
    #include "p10_ppe_c_7.H"
#else
    #include <p10_scom_c.H>
    #include "p10_fbc_core_topo.H"
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
    using namespace scomt::c;
    FAPI_INF(">>p10_hcd_cache_scominit_qme");

    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    auto l_chip = i_target.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();

    for (const auto& l_core : i_target.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        fapi2::ReturnCode l_rc;

        FAPI_DBG("Invoking p10.l3.scom.initfile on target");
        FAPI_EXEC_HWP(l_rc, p10_l3_scom, l_core, FAPI_SYSTEM, l_chip);

        if (l_rc)
        {
            FAPI_ERR("Error from p10_l3_scom");
            fapi2::current_err = l_rc;
            goto fapi_try_exit;
        }

        FAPI_DBG("Invoking p10.ncu.scom.initfile");
        FAPI_EXEC_HWP(l_rc, p10_ncu_scom, l_core, FAPI_SYSTEM, l_chip);

        if (l_rc)
        {
            FAPI_ERR("Error from p10_ncu_scom");
            fapi2::current_err = l_rc;
            goto fapi_try_exit;
        }

        FAPI_DBG("Configuring NCU darn bar");
        {
            fapi2::ATTR_PROC_NX_RNG_BAR_ENABLE_Type l_darn_en;
            fapi2::ATTR_PROC_NX_RNG_BAR_BASE_ADDR_OFFSET_Type l_darn_offset;
            fapi2::buffer<uint64_t> l_darn_bar;
            uint64_t l_base_address_nm0_unused;
            uint64_t l_base_address_nm1_unused;
            uint64_t l_base_address_m_unused;
            uint64_t l_base_address_mmio;

            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_NX_RNG_BAR_ENABLE, l_chip, l_darn_en),
                     "Error from FAPI_ATTR_GET (ATTR_PROC_NX_RNG_BAR_ENABLE)");

            FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_PROC_NX_RNG_BAR_BASE_ADDR_OFFSET, FAPI_SYSTEM, l_darn_offset),
                     "Error from FAPI_ATTR_GET (ATTR_PROC_NX_RNG_BAR_BASE_ADDR_OFFSET)");

            FAPI_TRY(p10_fbc_utils_get_chip_base_address(
                         l_chip,
                         EFF_TOPOLOGY_ID,
                         l_base_address_nm0_unused,
                         l_base_address_nm1_unused,
                         l_base_address_m_unused,
                         l_base_address_mmio),
                     "Error from p10_fbc_utils_get_chip_base_address");

#ifndef EPM_TUNING

            l_darn_bar = l_base_address_mmio + l_darn_offset;

            if(l_darn_en == fapi2::ENUM_ATTR_PROC_NX_RNG_BAR_ENABLE_ENABLE)
            {

#else

            l_darn_bar = 0x60302031D0000;

#endif

                l_darn_bar.setBit<NC_NCMISC_NCSCOMS_NCU_DARN_BAR_REG_EN>();

#ifndef EPM_TUNING

            }

#endif

            FAPI_TRY(fapi2::putScom(l_core, NC_NCMISC_NCSCOMS_NCU_DARN_BAR_REG, l_darn_bar),
                     "Error from putScom (NCU_DARN_BAR_REG)");
        }

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
        FAPI_TRY(p10_fbc_core_topo( c,
                                    l_topo_scoms,
                                    nullptr,   // HOMER pointer not needed for HW mode
                                    stopImageSection::PROC_STOP_SECTION_CACHE,
                                    rt_topo::RT_TOPO_MODE_HW));

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
        FAPI_TRY(p10_fbc_core_topo( c,
                                    l_topo_scoms,
                                    nullptr,   // HOMER pointer not needed for HW mode
                                    stopImageSection::PROC_STOP_SECTION_CACHE,
                                    rt_topo::RT_TOPO_MODE_HW));


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
