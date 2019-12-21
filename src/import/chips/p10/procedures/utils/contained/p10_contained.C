/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/utils/contained/p10_contained.C $ */
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

#include <fapi2.H>
#include <p10_contained.H>
#include <p10_scom_c.H>
#include <p10_scom_perv.H>
#include <p10_scan_via_scom.H>
#include <p10_contained_active_cache_ringspin.H>
#include <p10_contained_backing_cache_ringspin.H>
#include <p10_contained_chc_inactive_cores_ringspin.H>

fapi2::ReturnCode is_runn_ipl(bool& o_yes)
{
    fapi2::ATTR_RUNN_MODE_Type tmp;
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RUNN_MODE, SYS, tmp));

    o_yes = tmp == fapi2::ENUM_ATTR_RUNN_MODE_ON;

fapi_try_exit:
    return fapi2::current_err;
}

fapi2::ReturnCode is_cac_ipl(bool& o_yes)
{
    fapi2::ATTR_CONTAINED_IPL_TYPE_Type tmp;
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CONTAINED_IPL_TYPE, SYS, tmp));

    o_yes = tmp == fapi2::ENUM_ATTR_CONTAINED_IPL_TYPE_CACHE;

fapi_try_exit:
    return fapi2::current_err;
}

fapi2::ReturnCode is_chc_ipl(bool& o_yes)
{
    fapi2::ATTR_CONTAINED_IPL_TYPE_Type tmp;
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CONTAINED_IPL_TYPE, SYS, tmp));

    o_yes = tmp == fapi2::ENUM_ATTR_CONTAINED_IPL_TYPE_CHIP;

fapi_try_exit:
    return fapi2::current_err;
}

const size_t L3_MODE_REG0_LEN = 37;
const size_t L3_MODE_REG0_RSHIFT = 64 - L3_MODE_REG0_LEN;
const size_t L3_MODE_REG1_LEN = 39;
const size_t L3_MODE_REG1_RSHIFT = 64 - L3_MODE_REG1_LEN;
const size_t L3_BACKING_CTL_REG_LEN = 4;
const size_t L3_BACKING_CTL_REG_RSHIFT = 64 - L3_BACKING_CTL_REG_LEN;

fapi2::ReturnCode save_l3_config(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip,
                                 const uint32_t i_active_bvec, l3_config& io_l3_config)
{
    FAPI_INF(">> %s", __func__);

    using namespace scomt::c;

    fapi2::ATTR_CHIP_UNIT_POS_Type corenum;
    fapi2::buffer<uint64_t> tmp = 0;

    for (auto const& core : i_chip.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, core, corenum));

        if (is_active_core(corenum, i_active_bvec))
        {
            FAPI_TRY(GET_L3_MISC_L3CERRS_MODE_REG1(core, tmp));
            io_l3_config[corenum][L3_MISC_L3CERRS_MODE_REG1] = tmp;
            FAPI_TRY(GET_L3_MISC_L3CERRS_BACKING_CTL_REG(core, tmp));
            io_l3_config[corenum][L3_MISC_L3CERRS_BACKING_CTL_REG] = tmp;
        }
        else
        {
            FAPI_TRY(GET_L3_MISC_L3CERRS_MODE_REG1(core, tmp));
            io_l3_config[corenum][L3_MISC_L3CERRS_MODE_REG1] = tmp;
            FAPI_TRY(GET_L3_MISC_L3CERRS_MODE_REG0(core, tmp));
            io_l3_config[corenum][L3_MISC_L3CERRS_MODE_REG0] = tmp;
        }
    }

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}

fapi2::ReturnCode restore_l3_config(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip,
                                    const uint32_t i_active_bvec, const l3_config& i_l3_config,
                                    const bool i_runn)
{
    FAPI_INF(">> %s", __func__);

    using namespace scomt::c;
    using namespace scomt::perv;

    uint64_t mode_reg1, mode_reg0, backing_ctl_reg;
    fapi2::ATTR_CHIP_UNIT_POS_Type corenum;
    fapi2::Target<fapi2::TARGET_TYPE_PERV> perv;
    // ec_l3_fure
    const fapi2::buffer<uint64_t> scan_type = (fapi2::buffer<uint64_t>(0)
            .setBit<SCAN_REGION_TYPE_SCAN_TYPE_FUNC>()
            .setBit<SCAN_REGION_TYPE_SCAN_TYPE_REGF>());
    fapi2::buffer<uint64_t> scan_region_type;

    // TODO Eventually we should change this to use the parallel-scan feature
    //      of the clock-controller to save scoms maybe.
    for (auto const& core : i_chip.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, core, corenum));

        if (i_runn)
        {
            perv = core.getParent<fapi2::TARGET_TYPE_PERV>();
            scan_region_type = scan_type;
            scan_region_type.setBit(SCAN_REGION_TYPE_SCAN_REGION_UNIT5 +
                                    (corenum % 4));
        }

        // We need to set mode_reg1 for both active and backing caches
        mode_reg1 = i_l3_config.at(corenum).at(L3_MISC_L3CERRS_MODE_REG1);

        if (is_active_core(corenum, i_active_bvec))
        {
            backing_ctl_reg = i_l3_config.at(corenum).at(L3_MISC_L3CERRS_BACKING_CTL_REG);

            if (i_runn)
            {
                // The scan-via-scom procedures expect right-aligned input data
                FAPI_TRY(active_cache::ec_l3_fure(perv, scan_region_type,
                                                  mode_reg1 >> L3_MODE_REG1_RSHIFT,
                                                  backing_ctl_reg >> L3_BACKING_CTL_REG_RSHIFT));
            }
            else
            {
                PREP_L3_MISC_L3CERRS_MODE_REG1(core);
                FAPI_TRY(PUT_L3_MISC_L3CERRS_MODE_REG1(core, mode_reg1));
                PREP_L3_MISC_L3CERRS_BACKING_CTL_REG(core);
                FAPI_TRY(PUT_L3_MISC_L3CERRS_BACKING_CTL_REG(core, backing_ctl_reg));
            }
        }
        else
        {
            mode_reg0 = i_l3_config.at(corenum).at(L3_MISC_L3CERRS_MODE_REG0);

            if (i_runn)
            {
                // The scan-via-scom procedures expect right-aligned input data
                FAPI_TRY(backing_cache::ec_l3_fure(perv, scan_region_type,
                                                   mode_reg1 >> L3_MODE_REG1_RSHIFT,
                                                   mode_reg0 >> L3_MODE_REG0_RSHIFT));
            }
            else
            {
                PREP_L3_MISC_L3CERRS_MODE_REG1(core);
                FAPI_TRY(PUT_L3_MISC_L3CERRS_MODE_REG1(core, mode_reg1));
                PREP_L3_MISC_L3CERRS_MODE_REG0(core);
                FAPI_TRY(PUT_L3_MISC_L3CERRS_MODE_REG0(core, mode_reg0));
            }
        }
    }

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}

fapi2::ReturnCode disable_sreset_on_decr(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip,
        const uint32_t i_active_bvec)
{
    FAPI_INF(">> %s", __func__);

    using namespace scomt::c;
    using namespace scomt::perv;

    fapi2::ATTR_CHIP_UNIT_POS_Type corenum;
    fapi2::Target<fapi2::TARGET_TYPE_PERV> perv;
    // ec_l2_mode
    const fapi2::buffer<uint64_t> scan_type = (fapi2::buffer<uint64_t>(0)
            .setBit<SCAN_REGION_TYPE_SCAN_TYPE_CFG>());
    fapi2::buffer<uint64_t> scan_region_type;


    for (auto const& core : i_chip.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, core, corenum));

        if (!is_active_core(corenum, i_active_bvec))
        {
            perv = core.getParent<fapi2::TARGET_TYPE_PERV>();
            scan_region_type = scan_type;
            scan_region_type.setBit(SCAN_REGION_TYPE_SCAN_REGION_UNIT1 +
                                    (corenum % 4));

            FAPI_TRY(chc_inactive_cores::ec_cl2_mode(perv, scan_region_type, 0, 0));
        }
    }

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}
