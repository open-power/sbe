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
#include <p10_contained_ringspin.gen.H>

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

fapi2::ReturnCode save_l3_config(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip,
                                 const uint32_t i_active_bvec)
{
    FAPI_INF(">> %s", __func__);

    using namespace scomt::c;

    fapi2::ATTR_CHIP_UNIT_POS_Type corenum;
    fapi2::ATTR_CONTAINED_L3_CONFIG_Type l3_config;
    fapi2::buffer<uint64_t> tmp = 0;

    for (auto const& core : i_chip.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, core, corenum));

        if (is_active_core(corenum, i_active_bvec))
        {
            FAPI_TRY(GET_L3_MISC_L3CERRS_MODE_REG1(core, tmp));
            l3_config[corenum * 2 + 0] = tmp;
            FAPI_TRY(GET_L3_MISC_L3CERRS_BACKING_CTL_REG(core, tmp));
            l3_config[corenum * 2 + 1] = tmp;
        }
        else
        {
            FAPI_TRY(GET_L3_MISC_L3CERRS_MODE_REG1(core, tmp));
            l3_config[corenum * 2 + 0] = tmp;
            FAPI_TRY(GET_L3_MISC_L3CERRS_MODE_REG0(core, tmp));
            l3_config[corenum * 2 + 1] = tmp;
        }
    }

    FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_CONTAINED_L3_CONFIG, i_chip, l3_config));

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}

#define RINGSPIN_EC_SWITCH_PRCD(chip_ec, prcd) ec ## chip_ec::prcd
#define RINGSPIN_EC_SWITCH_CASE(chip_ec, prcd, args...)         \
    case 0x ## chip_ec:                                         \
    {                                                           \
        FAPI_TRY(RINGSPIN_EC_SWITCH_PRCD(chip_ec, prcd)(args)); \
        break;                                                  \
    }

fapi2::ReturnCode restore_l3_config(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip,
                                    const uint32_t i_active_bvec, const bool i_runn,
                                    const bool i_chc)
{
    FAPI_INF(">> %s", __func__);

    using namespace scomt::c;
    using namespace scomt::perv;

    uint64_t mode_reg1, mode_reg0, backing_ctl_reg;
    fapi2::ATTR_CHIP_UNIT_POS_Type corenum;
    fapi2::ATTR_EC_Type ec;
    fapi2::Target<fapi2::TARGET_TYPE_PERV> perv;
    // ec_l3_func
    const fapi2::buffer<uint64_t> scan_type = (fapi2::buffer<uint64_t>(0)
            .setBit<SCAN_REGION_TYPE_SCAN_TYPE_FUNC>());
    fapi2::buffer<uint64_t> scan_region_type;
    fapi2::ATTR_CONTAINED_L3_CONFIG_Type l3_config;

    FAPI_TRY(FAPI_ATTR_GET_PRIVILEGED(fapi2::ATTR_EC, i_chip, ec));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CONTAINED_L3_CONFIG, i_chip, l3_config));

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

        if (!i_chc && i_runn)
        {
            mode_reg1 = 0;
        }
        else
        {
            mode_reg1 = l3_config[corenum * 2 + 0];
        }

        if (is_active_core(corenum, i_active_bvec))
        {
            backing_ctl_reg = l3_config[corenum * 2 + 1];

            if (i_runn)
            {
                switch (ec)
                {
                // *INDENT-OFF*
                RINGSPIN_EC_SWITCH_CASE(10, ec_l3_func_restore_active_l3,
                                        perv, scan_region_type, backing_ctl_reg,
                                        mode_reg1)
                default:
                    FAPI_ERR("No generated ringspin procedure for ATTR_EC=%02x"
                             " PROCEDURE=ec_l3_func_restore_backing_l3", ec);
                    return fapi2::FAPI2_RC_FALSE;
                }
                // *INDENT-ON*
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
            mode_reg0 = l3_config[corenum * 2 + 1];

            if (i_runn)
            {
                switch (ec)
                {
                // *INDENT-OFF*
                RINGSPIN_EC_SWITCH_CASE(10, ec_l3_func_restore_backing_l3,
                                        perv, scan_region_type, mode_reg0,
                                        mode_reg1)
                default:
                    FAPI_ERR("No generated ringspin procedure for ATTR_EC=%02x"
                             " PROCEDURE=ec_l3_func_restore_backing_l3", ec);
                    return fapi2::FAPI2_RC_FALSE;
                // *INDENT-ON*
                }
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
    fapi2::ATTR_EC_Type ec;
    fapi2::Target<fapi2::TARGET_TYPE_PERV> perv;
    // ec_l2_mode
    const fapi2::buffer<uint64_t> scan_type = (fapi2::buffer<uint64_t>(0)
            .setBit<SCAN_REGION_TYPE_SCAN_TYPE_CFG>());
    fapi2::buffer<uint64_t> scan_region_type;

    FAPI_TRY(FAPI_ATTR_GET_PRIVILEGED(fapi2::ATTR_EC, i_chip, ec));

    for (auto const& core : i_chip.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, core, corenum));

        if (!is_active_core(corenum, i_active_bvec))
        {
            perv = core.getParent<fapi2::TARGET_TYPE_PERV>();
            scan_region_type = scan_type;
            scan_region_type.setBit(SCAN_REGION_TYPE_SCAN_REGION_UNIT1 +
                                    (corenum % 4));

            switch (ec)
            {
            // *INDENT-OFF*
            RINGSPIN_EC_SWITCH_CASE(10, ec_cl2_mode_disable_core_sreset,
                                    perv, scan_region_type, 0, 0)
            default:
                FAPI_ERR("No generated ringspin procedure for ATTR_EC=%02x"
                         " PROCEDURE=ec_cl2_mode_disable_core_sreset", ec);
                return fapi2::FAPI2_RC_FALSE;
            // *INDENT-ON*
            }
        }
    }

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}

#ifdef P10_CONTAINED_ENABLE_SEEDING

#include <random>

static std::mt19937_64 seed_rng;

fapi2::ReturnCode seed_exer_threads(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_chip)
{
    FAPI_INF(">> %s", __func__);

    using namespace scomt::perv;

    fapi2::ATTR_EC_Type ec;
    fapi2::ATTR_CHIP_UNIT_POS_Type corenum;
    fapi2::ATTR_RUNN_MASTER_SEED_Type master_seed;
    fapi2::ATTR_RUNN_CORE_SEED_SELECT_Type core_seed_sel;
    fapi2::ATTR_RUNN_THREAD_SEEDS_Type thread_seeds;
    fapi2::ATTR_ACTIVE_CORES_VEC_Type active_bvec;
    fapi2::Target<fapi2::TARGET_TYPE_PERV> perv;
    // ec_cl2_func
    const fapi2::buffer<uint64_t> scan_type = (fapi2::buffer<uint64_t>(0)
            .setBit<SCAN_REGION_TYPE_SCAN_TYPE_FUNC>());
    fapi2::buffer<uint64_t> scan_region_type;

    FAPI_TRY(FAPI_ATTR_GET_PRIVILEGED(fapi2::ATTR_EC, i_chip, ec));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_ACTIVE_CORES_VEC, i_chip, active_bvec));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RUNN_MASTER_SEED, i_chip, master_seed));
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RUNN_CORE_SEED_SELECT, i_chip,
                           core_seed_sel));

    for (auto const& core : i_chip.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, core, corenum));

        if (!is_active_core(corenum, active_bvec))
        {
            continue;
        }

        perv = core.getParent<fapi2::TARGET_TYPE_PERV>();

        scan_region_type = scan_type;
        scan_region_type.setBit(SCAN_REGION_TYPE_SCAN_REGION_UNIT1 +
                                (corenum % 4));

        // Initialize the RNG and *then* advance the RNG state to a core.
        seed_rng.seed(master_seed);

        if (core_seed_sel != fapi2::ENUM_ATTR_RUNN_CORE_SEED_SELECT_RANDOM)
        {
            // Advance the RNG to the core specified
            seed_rng.discard(core_seed_sel * 4);
        }
        else
        {
            // Advance the RNG to this core
            seed_rng.discard(corenum * 4);
        }

        thread_seeds[0] = seed_rng();
        thread_seeds[1] = seed_rng();
        thread_seeds[2] = seed_rng();
        thread_seeds[3] = seed_rng();

        // Attempt to write the attribute - if the user has an override set via
        // CONST in a seedfile then the write will silently turn into a NOP and
        // we pick up the override value instead.
        FAPI_TRY(FAPI_ATTR_SET(fapi2::ATTR_RUNN_THREAD_SEEDS, core,
                               thread_seeds));
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RUNN_THREAD_SEEDS, core,
                               thread_seeds));

        switch (ec)
        {
        // *INDENT-OFF*
        RINGSPIN_EC_SWITCH_CASE(10, ec_cl2_func_set_runn_seed,
                                perv, scan_region_type,
                                thread_seeds[0], thread_seeds[1],
                                thread_seeds[2], thread_seeds[3])
        default:
            FAPI_ERR("No generated ringspin procedure for ATTR_EC=%02x"
                     " PROCEDURE=ec_cl2_func_set_runn_seed", ec);
            return fapi2::FAPI2_RC_FALSE;
        // *INDENT-ON*
        }
    }

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}

#endif // P10_CONTAINED_ENABLE_SEEDING
