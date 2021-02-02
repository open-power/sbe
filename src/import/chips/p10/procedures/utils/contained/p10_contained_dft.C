/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/utils/contained/p10_contained_dft.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020,2021                        */
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
/* EKB-Mirror-To: hw/ppe                                                  */
#include <p10_contained.H>
#include <p10_contained_ipl.H>
#include <p10_contained_ipl.C>

#include <fapi2.H>
#include <p10_perv_sbe_cmn.H>
#include <multicast_group_defs.H>
#include <multicast_defs.H>

extern "C" {
    fapi2::ReturnCode p10_contained_save_config(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_DBG("Entering p10_contained_save_config.");
        const auto l_perv_eqs_w_cores = i_target.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_EQ);
        fapi2::ATTR_ACTIVE_CORES_VEC_Type l_active_bvec = 0;
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_ACTIVE_CORES_VEC, i_target, l_active_bvec));

        FAPI_DBG("ATTR_ACTIVE_CORES_EC: %x", l_active_bvec);

        FAPI_DBG("Entering save_l3_config");
        FAPI_TRY(save_l3_config(i_target, l_active_bvec));
        FAPI_DBG("Entering save_eq_pgoods");
        FAPI_TRY(save_eq_pgoods(l_perv_eqs_w_cores));

    fapi_try_exit:
        return fapi2::current_err;
    }


    fapi2::ReturnCode p10_contained_stopclocks(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_DBG("Entering p10_contained_stopclocks");
        FAPI_TRY(stopclocks(i_target));

    fapi_try_exit:
        return fapi2::current_err;
    }

    fapi2::ReturnCode p10_contained_setup(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_DBG("Entering p10_contained_setup");
        bool l_chc;

        FAPI_TRY(is_chc_ipl(l_chc));
        FAPI_DBG("Chip_contained mode is %d", l_chc);

        FAPI_TRY(contained_setup(i_target, l_chc));

    fapi_try_exit:
        return fapi2::current_err;
    }


    fapi2::ReturnCode p10_contained_restore(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        const auto perv_eqs_w_cores = i_target.getMulticast<fapi2::TARGET_TYPE_PERV>(fapi2::MCGROUP_GOOD_EQ);

        FAPI_DBG("Setting multicast groups to ISTEP3_MC_GROUPS");
        FAPI_TRY(p10_perv_sbe_cmn_setup_multicast_groups(i_target,
                 ISTEP3_MC_GROUPS));

        FAPI_DBG("Setting multicast groups to SELECT_EX_MC_GROUPS");
        FAPI_TRY(p10_perv_sbe_cmn_setup_multicast_groups(i_target,
                 SELECT_EX_MC_GROUPS));

        FAPI_DBG("Calling restore_eq_pgoods");
        FAPI_TRY(restore_eq_pgoods(perv_eqs_w_cores));
    fapi_try_exit:
        return fapi2::current_err;
    }


    fapi2::ReturnCode p10_contained_restore_l3_config(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_DBG("Entering p10_contained_restore_l3_config");
        fapi2::ATTR_ACTIVE_CORES_VEC_Type l_active_bvec = 0;
        bool l_runn;
        bool l_chc;

        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_ACTIVE_CORES_VEC, i_target, l_active_bvec));
        FAPI_TRY(is_runn_ipl(l_runn));
        FAPI_TRY(is_chc_ipl(l_chc));
        FAPI_DBG("l_runn is %d; l_chc is %d", l_runn, l_chc);

        FAPI_TRY(restore_l3_config(i_target, l_active_bvec, l_runn, l_chc));

    fapi_try_exit:
        return fapi2::current_err;
    }
} // extern "C"
