/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_hcd_core_scominit.C $ */
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
/// @file  p10_hcd_core_scominit.C
/// @brief Applies core/L2/NCU scom inits
///
/// *HWP HW Maintainer: David Du         <daviddu@us.ibm.com>
/// *HWP FW Maintainer: Prem Shanker Jha <premjha2@in.ibm.com>
/// *HWP Consumed by  : SBE,QME
///

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "p10_hcd_core_scominit.H"
#include <p10_fbc_utils.H>
#include <p10_scom_c.H>
#include <p10_scom_eq.H>
#include <p10_core_scom.H>

#ifdef __PPE_QME
    #include <p10_l2_scom.H>
    #include <p10_ncu_scom.H>
#endif

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Procedure: p10_hcd_core_scominit_qme
//------------------------------------------------------------------------------

#ifdef __PPE_QME
static inline fapi2::ReturnCode p10_hcd_core_scominit_qme(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST > & i_target)
{
    using namespace scomt::c;

    FAPI_INF(">>p10_hcd_core_scominit_qme");

    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    auto l_chip = i_target.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();
    fapi2::ReturnCode l_rc;

    for (const auto& l_core : i_target.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        fapi2::ReturnCode l_rc;

        FAPI_DBG("Invoking p10.core.scom.initfile");
        FAPI_EXEC_HWP(l_rc, p10_core_scom, l_core, FAPI_SYSTEM, l_chip);

        if (l_rc)
        {
            FAPI_ERR("Error from p10_core_scom");
            fapi2::current_err = l_rc;
            goto fapi_try_exit;
        }

        FAPI_DBG("Invoking p10.l2.scom.initfile");
        FAPI_EXEC_HWP(l_rc, p10_l2_scom, l_core, FAPI_SYSTEM, l_chip);

        if (l_rc)
        {
            FAPI_ERR("Error from p10_l2_scom");
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

            l_darn_bar = l_base_address_mmio + l_darn_offset;

            if(l_darn_en == fapi2::ENUM_ATTR_PROC_NX_RNG_BAR_ENABLE_ENABLE)
            {
                l_darn_bar.setBit<NC_NCMISC_NCSCOMS_NCU_DARN_BAR_REG_EN>();
            }

            FAPI_TRY(fapi2::putScom(l_core, NC_NCMISC_NCSCOMS_NCU_DARN_BAR_REG, l_darn_bar),
                     "Error from putScom (NCU_DARN_BAR_REG)");
        }
    }

fapi_try_exit:
    FAPI_INF("<<p10_hcd_core_scominit_qme");
    return fapi2::current_err;
}

//------------------------------------------------------------------------------
// Procedure: p10_hcd_core_scominit_sbe
//------------------------------------------------------------------------------

#else

namespace
{
///
/// @brief Initialize the L2 topology id table entries
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

    PREP_L2_L2MISC_L2CERRS_TOPOTABLE0(c);
    FAPI_TRY(PUT_L2_L2MISC_L2CERRS_TOPOTABLE0(c, topo_scoms[0]));
    PREP_L2_L2MISC_L2CERRS_TOPOTABLE1(c);
    FAPI_TRY(PUT_L2_L2MISC_L2CERRS_TOPOTABLE1(c, topo_scoms[1]));
    PREP_L2_L2MISC_L2CERRS_TOPOTABLE2(c);
    FAPI_TRY(PUT_L2_L2MISC_L2CERRS_TOPOTABLE2(c, topo_scoms[2]));
    PREP_L2_L2MISC_L2CERRS_TOPOTABLE3(c);
    FAPI_TRY(PUT_L2_L2MISC_L2CERRS_TOPOTABLE3(c, topo_scoms[3]));

fapi_try_exit:
    return fapi2::current_err;
}
};

static inline fapi2::ReturnCode p10_hcd_core_scominit_sbe(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST > & i_target)
{
    using namespace scomt::eq;

    // Placeholder for core/L2 scom inits needed during HBI
    std::vector<uint64_t> l_topo_scoms;
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_chip = i_target.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();
    fapi2::Target<fapi2::TARGET_TYPE_SYSTEM> FAPI_SYSTEM;
    fapi2::ReturnCode l_rc;

    // Get the register values for the SCOMs to setup the topology id table
    FAPI_TRY(topo::get_topology_table_scoms(l_chip, l_topo_scoms));
    // Setup the topology id tables for L2 via multicast
    FAPI_TRY(init_topo_id_tables(i_target, l_topo_scoms));

    for (const auto& l_core : i_target.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        fapi2::buffer<uint64_t> l_data64 = 0;
        uint32_t l_eq_num = 0;
        uint32_t l_core_num = 0;
        fapi2::ATTR_CHIP_UNIT_POS_Type l_attr_chip_unit_pos = 0;
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
        FAPI_TRY(fapi2::getScom(l_eq, CPLT_CTRL2_RW, l_data64));

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS,
                               l_core,
                               l_attr_chip_unit_pos));
        l_core_num = (uint32_t)l_attr_chip_unit_pos % 4;

        FAPI_DBG("Checking the good setting matches for EQ %d Core %d",
                 l_eq_num, l_core_num);

        if( l_data64.getBit(9 + l_core_num) == 0)
        {
            FAPI_DBG("Partial Bad detected for EQ %d Core %d, Skip",
                     l_eq_num, l_core_num);
            continue;
        }

        FAPI_EXEC_HWP(fapi2::current_err, p10_core_scom, l_core, FAPI_SYSTEM, l_chip);

        if (l_rc)
        {
            FAPI_ERR("Error from p10_core_scom");
            fapi2::current_err = l_rc;
            goto fapi_try_exit;
        }
    }

fapi_try_exit:
    return fapi2::current_err;
}
#endif

//------------------------------------------------------------------------------
// Procedure: p10_hcd_core_scominit
//------------------------------------------------------------------------------

fapi2::ReturnCode p10_hcd_core_scominit(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST > & i_target)
{
    FAPI_INF(">>p10_hcd_core_scominit");

#ifdef __PPE_QME
    FAPI_TRY(p10_hcd_core_scominit_qme(i_target));
#else
    FAPI_TRY(p10_hcd_core_scominit_sbe(i_target));
#endif

fapi_try_exit:
    FAPI_INF("<<p10_hcd_core_scominit");
    return fapi2::current_err;
}
