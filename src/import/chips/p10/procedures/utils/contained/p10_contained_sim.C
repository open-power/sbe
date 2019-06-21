/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/utils/contained/p10_contained_sim.C $ */
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
#include <cstdlib>
#include <ecmdClientCapi.H>
#include <fapi2.H>
#include <p10_contained_sim.H>

#ifdef P10_CONTAINED_SIM

namespace
{
///
/// @brief Executes a simulator subcommand
///
/// @param[in] i_cmd Simulator subcommand to execute
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
fapi2::ReturnCode subcmd(const std::string& i_cmd)
{
    auto rc = simSUBCMD(i_cmd.c_str());
    return (rc == ECMD_SUCCESS) ? fapi2::FAPI2_RC_SUCCESS
           : fapi2::FAPI2_RC_INVALID_PARAMETER;
}

///
/// @brief Executes a batchfile in the simulator
///
/// @param[in] i_batchfile Path to batchfile to execute in simulator
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
fapi2::ReturnCode batch(const std::string& i_batchfile)
{
    FAPI_INF("Executing batchfile[%s] via simSUBCMD", i_batchfile.c_str());
    std::string cmd = "batch " + i_batchfile;
    FAPI_TRY(subcmd(cmd));

fapi_try_exit:
    return fapi2::current_err;
}

///
/// @brief Read an environment variable, indicate if the variable is set to a
///        value, and return that value when set.
///
/// @param[in] i_envvar Environment variable to read
/// @param[out] o_val Environment variable value
///
/// @return true if environment variable is set to some value else false
///
bool getenvvar(const char* i_envvar, std::string& o_val)
{
    const char* tmp = std::getenv(i_envvar);

    if (tmp != NULL)
    {
        o_val = std::string(tmp);
        return true;
    }
    else
    {
        return false;
    }
}

} // namespace

namespace sim
{
bool skip_stopclocks()
{
    std::string tmp;
    return getenvvar("P10_CONTAINED_SIM_SKIP_STOPCLOCKS", tmp);
}

bool skip_l3purge()
{
    std::string tmp;
    return getenvvar("P10_CONTAINED_SIM_SKIP_L3PURGE", tmp);
}

fapi2::ReturnCode scan_via_resetfile(stage i_stage, bool i_chc)
{
    std::string batchfile;
    std::string envvar;

    if (!getenvvar("P10_CONTAINED_SIM_SCAN_VIA_RESETFILE", batchfile))
    {
        return fapi2::FAPI2_RC_SUCCESS;
    }

    if (i_stage == ISTEP4_CACHE)
    {
        envvar = i_chc ? "P10_CONTAINED_SIM_ISTEP4_CHC_CACHE_RESETFILE"
                 : "P10_CONTAINED_SIM_ISTEP4_CAC_CACHE_RESETFILE";

    }
    else if (i_stage == ISTEP4_CORE)
    {
        envvar = i_chc ? "P10_CONTAINED_SIM_ISTEP4_CHC_CORE_RESETFILE"
                 : "P10_CONTAINED_SIM_ISTEP4_CAC_CORE_RESETFILE";
    }
    else if (i_stage == ISTEP3)
    {
        envvar = "P10_CONTAINED_SIM_ISTEP3_CHC_RESETFILE";
    }

    if (getenvvar(envvar.c_str(), batchfile))
    {
        FAPI_TRY(batch(batchfile));
    }
    else
    {
        FAPI_ERR("Missing environment variable: %s", envvar.c_str());
        return fapi2::FAPI2_RC_INVALID_PARAMETER;
    }

fapi_try_exit:
    return fapi2::current_err;
}

#endif // P10_CONTAINED_SIM

} // namespace simtools
