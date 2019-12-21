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
#include <p10_contained.H>
#include <p10_contained_sim.H>

#ifdef P10_CONTAINED_SIM

#define ECMD_TRY(ecmd_f, ...) \
    do { \
        auto rc = ecmd_f(__VA_ARGS__); \
        if (rc != ECMD_SUCCESS) \
            goto fapi_try_exit; \
    } while (0)

///
/// @brief Executes a simulator subcommand
///
/// @param[in] i_cmd Simulator subcommand to execute
///
/// @return FAPI2_RC_SUCCESS if success else error code
///
static fapi2::ReturnCode subcmd(const std::string& i_cmd)
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
static fapi2::ReturnCode __attribute__((unused)) batch(const std::string& i_batchfile)
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
static bool getenvvar(const char* i_envvar, std::string& o_val)
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

fapi2::ReturnCode apply_selective_skip_arrayinit(const array i_array, const bool i_set)
{
    FAPI_INF(">> %s", __func__);

    std::string tmp;
    ecmdDataBuffer bit = ecmdDataBuffer(1);

    if (i_set)
    {
        bit.setBit(0);
    }

    if (!getenvvar("P10_CONTAINED_SIM_APPLY_SELECTIVE_SKIP_ARRAYINIT", tmp))
    {
        FAPI_ERR("P10_CONTAINED_SIM_APPLY_SELECTIVE_SKIP_ARRAYINIT not set");
        FAPI_INF(">> %s", __func__);
        return fapi2::FAPI2_RC_SUCCESS;
    }

    if (i_array == QMEARY)
    {
        FAPI_ERR("QME selective skip arrayinit not implemented");
    }
    else
    {
        ECMD_TRY(simSTKFAC,
                 "B0.C0.S0.P0.E10.EX00.L3.L3_DF.L3CAENG.ENG.ADDR.L3CAC_ADDRCONFIG.L3CAC_BLOCK_WRITE_ABIST_AT_SPEED.LATC.L2", 1, bit);
        ECMD_TRY(simSTKFAC,
                 "B0.C0.S0.P0.E10.EX01.L3.L3_DF.L3CAENG.ENG.ADDR.L3CAC_ADDRCONFIG.L3CAC_BLOCK_WRITE_ABIST_AT_SPEED.LATC.L2", 1, bit);
        ECMD_TRY(simSTKFAC,
                 "B0.C0.S0.P0.E10.EX06.L3.L3_DF.L3CAENG.ENG.ADDR.L3CAC_ADDRCONFIG.L3CAC_BLOCK_WRITE_ABIST_AT_SPEED.LATC.L2", 1, bit);
        ECMD_TRY(simSTKFAC,
                 "B0.C0.S0.P0.E10.EX00.L3.L3_MISC.L3DAENG.ENG.ADDR.L3DIR_ADDRCONFIG.L3DIR_BLOCK_WRITE_ABIST_AT_SPEED.LATC.L2", 1, bit);
        ECMD_TRY(simSTKFAC,
                 "B0.C0.S0.P0.E10.EX01.L3.L3_MISC.L3DAENG.ENG.ADDR.L3DIR_ADDRCONFIG.L3DIR_BLOCK_WRITE_ABIST_AT_SPEED.LATC.L2", 1, bit);
        ECMD_TRY(simSTKFAC,
                 "B0.C0.S0.P0.E10.EX06.L3.L3_MISC.L3DAENG.ENG.ADDR.L3DIR_ADDRCONFIG.L3DIR_BLOCK_WRITE_ABIST_AT_SPEED.LATC.L2", 1, bit);
    }

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}

fapi2::ReturnCode set_mcd_topoid_parity()
{
    FAPI_INF(">> %s", __func__);

    std::string tmp;
    ecmdDataBuffer bit = ecmdDataBuffer(1);
    bit.setBit(0);

    if (!getenvvar("P10_CONTAINED_SIM_SET_MCD_TOPOID_PARITY", tmp))
    {
        FAPI_ERR("P10_CONTAINED_SIM_SET_MCD_TOPOID_PARITY not set");
        FAPI_INF(">> %s", __func__);
        return fapi2::FAPI2_RC_SUCCESS;
    }

    ECMD_TRY(simSTKFAC,
             "B0.C0.S0.P0.E10.ES.MCD.MCD_BANK0.MCD_BANK_DF.CFG_REGS.CFG_TOPOID_PARITY_Q_INST.LATC.L2", 1, bit);

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}

fapi2::ReturnCode set_pc_decrementer()
{
    FAPI_INF(">> %s", __func__);

    std::string tmp;
    fapi2::ATTR_RUNN_SRESET_THREADS_BVEC_Type threads;
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RUNN_SRESET_THREADS_BVEC, SYS, threads));

    if (!getenvvar("P10_CONTAINED_SIM_SET_PC_DECREMENTER", tmp))
    {
        FAPI_ERR("P10_CONTAINED_SIM_SET_PC_DECREMENTER not set");
        FAPI_INF(">> %s", __func__);
        return fapi2::FAPI2_RC_SUCCESS;
    }

    if (threads & fapi2::ENUM_ATTR_RUNN_SRESET_THREADS_BVEC_T0)
    {
        // Set decrementer to 64 cycles and bit[33]=1 (invert bit)
        ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.EC.PC.V0_DEC",
                 "0b0000000000000000000000000000000001000000000000000000000001000000");
        ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.EC.PC.V0_DEC",
                 "0b0000000000000000000000000000000001000000000000000000000001000000");
        ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.EC.PC.V0_DEC",
                 "0b0000000000000000000000000000000001000000000000000000000001000000");
    }

    if (threads & fapi2::ENUM_ATTR_RUNN_SRESET_THREADS_BVEC_T1)
    {
        // Set decrementer to 64 cycles and bit[33]=1 (invert bit)
        ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.EC.PC.V1_DEC",
                 "0b0000000000000000000000000000000001000000000000000000000001000000");
        ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.EC.PC.V1_DEC",
                 "0b0000000000000000000000000000000001000000000000000000000001000000");
        ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.EC.PC.V1_DEC",
                 "0b0000000000000000000000000000000001000000000000000000000001000000");
    }

    if (threads & fapi2::ENUM_ATTR_RUNN_SRESET_THREADS_BVEC_T2)
    {
        // Set decrementer to 64 cycles and bit[33]=1 (invert bit)
        ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.EC.PC.V2_DEC",
                 "0b0000000000000000000000000000000001000000000000000000000001000000");
        ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.EC.PC.V2_DEC",
                 "0b0000000000000000000000000000000001000000000000000000000001000000");
        ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.EC.PC.V2_DEC",
                 "0b0000000000000000000000000000000001000000000000000000000001000000");
    }

    if (threads & fapi2::ENUM_ATTR_RUNN_SRESET_THREADS_BVEC_T3)
    {
        // Set decrementer to 64 cycles and bit[33]=1 (invert bit)
        ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.EC.PC.V3_DEC",
                 "0b0000000000000000000000000000000001000000000000000000000001000000");
        ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.EC.PC.V3_DEC",
                 "0b0000000000000000000000000000000001000000000000000000000001000000");
        ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.EC.PC.V3_DEC",
                 "0b0000000000000000000000000000000001000000000000000000000001000000");
    }

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}

fapi2::ReturnCode init_via_dials(const stage i_stage, const bool i_chc)
{
    FAPI_INF(">> %s", __func__);

    std::string tmp;

    if (!getenvvar("P10_CONTAINED_SIM_SCAN_VIA_DIALS", tmp))
    {
        FAPI_ERR("P10_CONTAINED_SIM_SCAN_VIA_DIALS not set");
        FAPI_INF(">> %s", __func__);
        return fapi2::FAPI2_RC_SUCCESS;
    }

    switch (i_stage)
    {
        case ISTEP4_CACHE:
            {
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.L3.L3_MISC.L3CERRS.L3_TOPOLOGY_TBL_ENTRY_00_VALID", "0b1");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.L3.L3_MISC.L3CERRS.L3_TOPOLOGY_TBL_ENTRY_00_VALID", "0b1");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.L3.L3_MISC.L3CERRS.L3_TOPOLOGY_TBL_ENTRY_00_VALID", "0b1");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.NC.NCMISC.NCSCOMS.TOPOTABLE_ENTRY00_VAL", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.NC.NCMISC.NCSCOMS.TOPOTABLE_ENTRY00_VAL", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.NC.NCMISC.NCSCOMS.TOPOTABLE_ENTRY00_VAL", "ON");
            }

            if (!i_chc)
            {
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.L3.L3_MISC.L3CERRS.L3_CACHE_CONTAINED_MODE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.L3.L3_MISC.L3CERRS.L3_CACHE_CONTAINED_MODE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.L3.L3_MISC.L3CERRS.L3_CACHE_CONTAINED_MODE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.L3.L3_MISC.L3CERRS.L3_NO_ALLOCATE_MODE_EN", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.L3.L3_MISC.L3CERRS.L3_NO_ALLOCATE_MODE_EN", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.L3.L3_MISC.L3CERRS.L3_NO_ALLOCATE_MODE_EN", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.NC.NC_RUN_MODE", "TRASH_MODE");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.NC.NC_RUN_MODE", "TRASH_MODE");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.NC.NC_RUN_MODE", "TRASH_MODE");
            }
            else
            {
                // XXX
                // Usually, the scan-via-scom restore would do the right thing,
                // since that commit isn't part of the model-tag, we can't use
                // it. For now, just set the number of backing caches here for
                // the active L3 (normally everyone inits to CHIP_CONTAINED_1_BACKING).
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.L3.L3_ACTIVE_RUN_MODE", "CHIP_CONTAINED_2_BACKING");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.L3.L3_ACTIVE_RUN_MODE", "CHIP_CONTAINED_1_BACKING");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.L3.L3_ACTIVE_RUN_MODE", "CHIP_CONTAINED_1_BACKING");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.L3.L3_MISC.L3CERRS.L3CERRS_LINE_DEL_DISABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.L3.L3_MISC.L3CERRS.L3CERRS_LINE_DEL_DISABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.L3.L3_MISC.L3CERRS.L3CERRS_LINE_DEL_DISABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.NC.NC_RUN_MODE", "CHIPCONTAINED_MODE");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.NC.NC_RUN_MODE", "CHIPCONTAINED_MODE");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.NC.NC_RUN_MODE", "CHIPCONTAINED_MODE");
            }

            break;

        case ISTEP4_CORE:
            {
                fapi2::ATTR_RUNN_SRESET_THREADS_BVEC_Type threads;
                FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_RUNN_SRESET_THREADS_BVEC,
                                       SYS, threads));

                if (threads & fapi2::ENUM_ATTR_RUNN_SRESET_THREADS_BVEC_T0)
                {
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.EC.PC.PMC.VT0_SRESET_ON_1ST_DEC_EN", "ON");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.EC.PC.PMC.VT0_SRESET_ON_1ST_DEC_EN", "ON");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.EC.PC.PMC.VT0_SRESET_ON_1ST_DEC_EN", "ON");

                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.EC.PC.THRCTL.TCTLCOM.VT0_SRESET_ON_1ST_DEC_EN", "ON");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.EC.PC.THRCTL.TCTLCOM.VT0_SRESET_ON_1ST_DEC_EN", "ON");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.EC.PC.THRCTL.TCTLCOM.VT0_SRESET_ON_1ST_DEC_EN", "ON");

                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.EC.PC.PMC.DECR_EXTENDED_FUNC", "ON");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.EC.PC.PMC.DECR_EXTENDED_FUNC", "ON");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.EC.PC.PMC.DECR_EXTENDED_FUNC", "ON");

                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.EC.PC.THRCTL.TCTLCOM.DECR_EXTENDED_FUNC", "ON");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.EC.PC.THRCTL.TCTLCOM.DECR_EXTENDED_FUNC", "ON");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.EC.PC.THRCTL.TCTLCOM.DECR_EXTENDED_FUNC", "ON");

                    // Set decrementer to 64 cycles and bit[33]=1 (invert bit)
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.EC.PC.V0_DEC",
                             "0b0000000000000000000000000000000001000000000000000000000001000000");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.EC.PC.V0_DEC",
                             "0b0000000000000000000000000000000001000000000000000000000001000000");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.EC.PC.V0_DEC",
                             "0b0000000000000000000000000000000001000000000000000000000001000000");

                    // Set PSSCR[60:63] = 0b0000
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.EC.PC.PMC.V0_PSSCR",
                             "0b1011000000000000000000000000000000000000011111110000001111110000");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.EC.PC.PMC.V0_PSSCR",
                             "0b1011000000000000000000000000000000000000011111110000001111110000");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.EC.PC.PMC.V0_PSSCR",
                             "0b1011000000000000000000000000000000000000011111110000001111110000");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.EC.PC.PMC.V1_PSSCR",
                             "0b1011000000000000000000000000000000000000011111110000001111110000");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.EC.PC.PMC.V1_PSSCR",
                             "0b1011000000000000000000000000000000000000011111110000001111110000");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.EC.PC.PMC.V1_PSSCR",
                             "0b1011000000000000000000000000000000000000011111110000001111110000");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.EC.PC.PMC.V2_PSSCR",
                             "0b1011000000000000000000000000000000000000011111110000001111110000");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.EC.PC.PMC.V2_PSSCR",
                             "0b1011000000000000000000000000000000000000011111110000001111110000");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.EC.PC.PMC.V2_PSSCR",
                             "0b1011000000000000000000000000000000000000011111110000001111110000");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.EC.PC.PMC.V3_PSSCR",
                             "0b1011000000000000000000000000000000000000011111110000001111110000");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.EC.PC.PMC.V3_PSSCR",
                             "0b1011000000000000000000000000000000000000011111110000001111110000");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.EC.PC.PMC.V3_PSSCR",
                             "0b1011000000000000000000000000000000000000011111110000001111110000");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.EC.PC.PMC.MIN_REQ_POW_LEVEL", "0b0000");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.EC.PC.PMC.MIN_REQ_POW_LEVEL", "0b0000");
                    ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.EC.PC.PMC.MIN_REQ_POW_LEVEL", "0b0000");
                }

                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.L2.L2MISC.L2CERRS.TOPOTABLE_ENTRY00_VAL", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.L2.L2MISC.L2CERRS.TOPOTABLE_ENTRY00_VAL", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.L2.L2MISC.L2CERRS.TOPOTABLE_ENTRY00_VAL", "ON");

                // Set pagesize to 4KiB
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.EC.MU.MU_CFG_CMMU_PIPE_HVR_ERAT_RLD_PGSZ", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.EC.MU.MU_CFG_CMMU_PIPE_HVR_ERAT_RLD_PGSZ", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.EC.MU.MU_CFG_CMMU_PIPE_HVR_ERAT_RLD_PGSZ", "ON");
            }

            if (!i_chc)
            {
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.L2.L2_RUN_MODE", "TRASH_MODE");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.L2.L2_RUN_MODE", "TRASH_MODE");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.L2.L2_RUN_MODE", "TRASH_MODE");
            }
            else
            {
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX00.L2.L2_RUN_MODE", "CHIP_CONTAINED");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX01.L2.L2_RUN_MODE", "CHIP_CONTAINED");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.EX06.L2.L2_RUN_MODE", "CHIP_CONTAINED");
            }

            break;

        case ISTEP3:
            {
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.VAS.VA_EG.EG_SCF.EG_TOPOID_XLAT_TBL_VALIDS", "0b1");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.NX.PBI.CQ_WRAP.NXCQ_SCOM.TOPOID_XLAT_TBL_VALIDS", "0b1");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.MM0.MM_FBC.CQ_WRAP.NXCQ_SCOM.TOP_ID_XLAT_TBL0", "0b1");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.MM1.MM_FBC.CQ_WRAP.NXCQ_SCOM.TOP_ID_XLAT_TBL0", "0b1");
                // XXX
                // Parity bullshit, so do this manually via scom after IPL for
                // now (not RUNN compatible).
//                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.MCD.BANK0_MCD_MUON_VALID", "ON");
//                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.MCD.BANK0_MCD_MUON_CHIP_CONTAINED_MODE", "ON");
//                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.MCD.BANK0_MCD_TOPID_ACTIVE0_ENABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_CMD.PB_CMD_RTES3_4U_PBI_ES.PB_CMD_SNOOPER.CMD_ARB_ENABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_CMD.PB_CMD_RTES3_4U_PBI_ES.PB_CMD_SNOOPER.DATA_ARB_ENABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_CMD.PB_CMD_RTES3_4U_PBI_ES.PB_CMD_SNOOPER.FP_ENABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_COM.PB_CFG_REPRO_MODE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_COM.PB_CFG_PAU_STEP_OVERRIDE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_COM.PB_CFG_MCA_RATIO_OVERRIDE", "0b010");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_COM.PB_CFG_PBIASY_MC0_DISABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_COM.PB_CFG_PBIASY_MC1_DISABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_COM.PB_CFG_PBIASY_MC2_DISABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_COM.PB_CFG_PBIASY_MC3_DISABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_COM.PB_CFG_PBIASY_PA0_DISABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_COM.PB_CFG_PBIASY_PA3_DISABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_COM.PB_CFG_PBIASY_PA4_DISABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_COM.PB_CFG_PBIASY_PA5_DISABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_COM.PB_CFG_PBIASY_PA6_DISABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_COM.PB_CFG_PBIASY_PA7_DISABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_COM.PB_CFG_PBIASY_PTL0_DISABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_COM.PB_CFG_PBIASY_PTL1_DISABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_COM.PB_CFG_PBIASY_PTL2_DISABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_COM.PB_CFG_PBIASY_PTL3_DISABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_COM.PB_CFG_PBIASY_PTL4_DISABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_COM.PB_CFG_PBIASY_PTL5_DISABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_COM.PB_CFG_PBIASY_PTL6_DISABLE", "ON");
                ECMD_TRY(simPutDial, "B0.C0.S0.P0.E10.PB.PB_COM.PB_CFG_PBIASY_PTL7_DISABLE", "ON");
            }
            break;
    }

fapi_try_exit:
    FAPI_INF("<< %s", __func__);
    return fapi2::current_err;
}

fapi2::ReturnCode checkpoint(const bool i_chc, const bool i_runn,
                             const std::string& i_suffix)
{
    std::string name;

    if (!getenvvar("USER", name))
    {
        FAPI_ERR("Could not read USER env variable");
        return fapi2::FAPI2_RC_INVALID_PARAMETER;
    }

    name += (i_chc) ? "_chc" : "_cc";
    name += (i_runn) ? "r_" : "_";
    name += i_suffix;

    FAPI_INF("Taking simulation checkpoint '%s'", name.c_str());
    ECMD_TRY(simcheckpoint, name.c_str());

fapi_try_exit:
    return fapi2::current_err;
}

#endif // P10_CONTAINED_SIM

} // namespace simtools
