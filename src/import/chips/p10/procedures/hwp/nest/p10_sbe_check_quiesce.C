/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/nest/p10_sbe_check_quiesce.C $ */
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
//--------------------------------------------------------------------------
//
//
/// @file p10_sbe_check_quiesce.C
/// @brief Check quiesce state for all units on the powerbus
///
/// *HWP HWP Maintainer: Rajees Rahman  <rajerpp1@in.ibm.com>
/// *HWP FW Maintainer: Raja Das <rajadas2@in.ibm.com>
/// *HWP Consumed by: SBE
///
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <p10_sbe_check_quiesce.H>
#include <p10_sbe_scominit.H>
#include <p10_suspend_io.H>
// Cronus only
#if !defined(__PPE__) && !defined(__HOSTBOOT_MODULE)
    #include <p10_pcie_utils.H>
#endif
#include <p10_scom_proc.H>
#include <p10_scom_nmmu.H>
#include <p10_scom_pau.H>
#include <p10_scom_phb_e.H>
#include <p10_scom_phb_d.H>
#include <p10_scom_c.H>

//-----------------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------------
//This is how many times each unit will try to do the quiesce if it has a wait for some cycles
const uint32_t C_NUM_TRIES_QUIESCE_STATE = 10000;
//These are the delays for the units that need them
const uint32_t C_DELAY_NS_396      = 396000000 / C_NUM_TRIES_QUIESCE_STATE; //396 ms
const uint32_t C_DELAY_CYCLES_396 = 792000000 / C_NUM_TRIES_QUIESCE_STATE; //2GHz * 396 ms
const uint32_t C_PAU_DELAY_NS = 150000 / C_NUM_TRIES_QUIESCE_STATE; //150 microseconds
const uint32_t C_PAU_DELAY_CYCLES = 300000 / C_NUM_TRIES_QUIESCE_STATE; //2GHz * 150 microseconds
const uint32_t C_INTP_DELAY_NS = 10000 / C_NUM_TRIES_QUIESCE_STATE; //10 microseconds
const uint32_t C_INTP_DELAY_CYCLES = 20000 / C_NUM_TRIES_QUIESCE_STATE; //2GHz * 10 microseconds

using namespace scomt;
using namespace scomt::proc;
using namespace scomt::nmmu;
using namespace scomt::phb;

const uint32_t PHB_HV_IND_ADDR_VALID_BIT = 0;
const uint32_t PHB_HV_IND_ADDR_START_BIT = 52;
const uint32_t PHB_HV_IND_ADDR_LEN = 12;


//---------------------------------------------------------------------------------
// NOTE: description in header
//---------------------------------------------------------------------------------
fapi2::ReturnCode p10_phb_check_quiesce(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    FAPI_DBG("p10_phb_check_quiesce: Entering ...");
    fapi2::buffer<uint64_t> l_data(0);

    // If ETU is not already in reset, use indirect register to quiesce DMAs
    // The address of the PHB Quiesce DMA Register is 0x0888 (found in PHB spec)
    auto l_phb_chiplets_vec = i_target.getChildren<fapi2::TARGET_TYPE_PHB>();

    for (auto& l_phb_chiplet : l_phb_chiplets_vec)
    {

// Cronus only
#if !defined(__PPE__) && !defined(__HOSTBOOT_MODULE)
        // Skip if PHB target is not enabled
        bool l_phbEnabled = false;
        FAPI_TRY(isPHBEnabled(l_phb_chiplet, l_phbEnabled),
                 "Error returned from isPHBEnabled()");

        if (!l_phbEnabled)
        {
            FAPI_DBG("PHB is disabled, skip checking quiesce.");
            continue;
        }

#endif

        FAPI_TRY(PREP_REGS_PHBRESET_REG(l_phb_chiplet));
        FAPI_TRY(GET_REGS_PHBRESET_REG(l_phb_chiplet, l_data));

        if (!GET_REGS_PHBRESET_REG_PE_ETU_RESET(l_data))
        {
            //Clear contents of PHB HV Indirect Address Register
            l_data.flush<0>();

            FAPI_TRY(PREP_RSB_SCOM_SSR_PHB5_HVIAR(l_phb_chiplet));
            FAPI_TRY(GET_RSB_SCOM_SSR_PHB5_HVIAR(l_phb_chiplet, l_data));
            //Setup the PHB HV registers for the write
            l_data.insertFromRight<PHB_HV_IND_ADDR_START_BIT, PHB_HV_IND_ADDR_LEN>(0x888);
            l_data.setBit<PHB_HV_IND_ADDR_VALID_BIT>();

            FAPI_TRY(PUT_RSB_SCOM_SSR_PHB5_HVIAR(l_phb_chiplet, l_data));

            //Setup PHB HV Indirect for write access
            l_data.flush<0>().insertFromRight<0, 63>(0x8000000000000000);
            FAPI_TRY(PUT_RSB_SCOM_SSR_PHB5_HVIAR(l_phb_chiplet, l_data));

            //Clear contents of PHB HV Indirect Address Register
            l_data.flush<0>();
            FAPI_TRY(PUT_RSB_SCOM_SSR_PHB5_HVIAR(l_phb_chiplet, l_data));
        }
    }

    FAPI_TRY(p10_suspend_io(i_target, true), "ERROR suspending IO");

fapi_try_exit:
    FAPI_DBG("p10_phb_check_quiesce: Exiting ...");
    return fapi2::current_err;
}

typedef fapi2::ReturnCode (*getScomFuncType)(const fapi2::Target<fapi2::TARGET_TYPE_PAU>& i_target,
        fapi2::buffer<uint64_t>& o_data);

typedef fapi2::ReturnCode (*putScomFuncType)(const fapi2::Target<fapi2::TARGET_TYPE_PAU>& i_target,
        const uint64_t i_data);

typedef fapi2::ReturnCode (*prepScomFuncType)(const fapi2::Target<fapi2::TARGET_TYPE_PAU>& i_target);

typedef fapi2::buffer<uint64_t>& (*setRegFldFuncType)(const uint64_t i_data,
        fapi2::buffer<uint64_t>& o_data);

typedef fapi2::buffer<uint64_t>& (*getRegFldFuncType)(fapi2::buffer<uint64_t>& i_data,
        uint64_t& o_data);

typedef fapi2::buffer<uint64_t>& (*clearRegFldFuncType)(fapi2::buffer<uint64_t>& o_data);

//---------------------------------------------------------------------------------
// NOTE: description in header
//---------------------------------------------------------------------------------
fapi2::ReturnCode p10_pau_check_quiesce(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    using namespace scomt::pau;

    FAPI_DBG("p10_pau_check_quiesce: Entering...");

    fapi2::buffer<uint64_t> l_data(0);
    fapi2::buffer<uint64_t> l_misc_config2(0);
    uint64_t l_fenced_data = 0;
    uint64_t l_ocapi_enabled = 0;
    uint64_t l_nvlink_enabled = 0;

    const uint32_t c_otl_reset_cmd = 0x3ull;
    // reset status value will be 0b01, if brick is not configured
    // as neither OCAPI nor NVLINK. Otherwise reset status will be 0b11.
    const uint32_t c_otl_reset_status_not_ocapi_nvlink = 0x01ul;
    const uint32_t c_otl_reset_status_ocapi_nvlink = 0x3ul;
    uint32_t l_otl_reset_status = c_otl_reset_status_ocapi_nvlink;

    const uint32_t c_fence_all_brick_cmd = 0x3ul;
    const uint32_t c_otl_disabled_status = 0x1ul;

    const uint32_t c_misc_fence_state_clear = 12;

    const uint32_t c_num_brick_per_pau = 2;
    const uint32_t c_num_gpu_bar_reg_per_brick = 6;
    const uint32_t c_num_ndt_bar_reg_per_brick = 4;
    const uint32_t c_num_gen_id_bar_reg_per_pau = 4;
    const uint32_t c_num_mmio_bar_reg_per_pau = 4;

    //CS_CTL_MISC_FENCE_CONTROL functions
    prepScomFuncType c_pau_0_fence_ctrl_prep_functions[c_num_brick_per_pau] =
    {
        PREP_CS_CTL_MISC_FENCE_CONTROL0,
        PREP_CS_CTL_MISC_FENCE_CONTROL1
    };
    putScomFuncType c_pau_0_fence_ctrl_put_functions[c_num_brick_per_pau] =
    {
        PUT_CS_CTL_MISC_FENCE_CONTROL0,
        PUT_CS_CTL_MISC_FENCE_CONTROL1
    };
    setRegFldFuncType c_pau_0_fence_ctrl_set_request_fence_functions[c_num_brick_per_pau] =
    {
        SET_CS_CTL_MISC_FENCE_CONTROL0_0_REQUEST_FENCE,
        SET_CS_CTL_MISC_FENCE_CONTROL1_1_REQUEST_FENCE
    };

    //CS_CTL_MISC_STATUS functions
    getScomFuncType c_pau_0_cq_ctrl_status_get_functions[c_num_brick_per_pau] =
    {
        GET_CS_CTL_MISC_STATUS1,
        GET_CS_CTL_MISC_STATUS2
    };
    getRegFldFuncType c_pau_0_cq_ctrl_status_get_am_fenced_functions[c_num_brick_per_pau] =
    {
        GET_CS_CTL_MISC_STATUS1_BRK0_AM_FENCED,
        GET_CS_CTL_MISC_STATUS2_BRK1_AM_FENCED
    };
    uint64_t c_pau_0_cq_ctrl_status_address[c_num_brick_per_pau] =
    {
        CS_CTL_MISC_STATUS1,
        CS_CTL_MISC_STATUS2
    };

    //CS_CTL_MISC_CONFIG2 functions
    getRegFldFuncType c_pau_0_cq_ctrl_misc_config2_get_ocapi_mode[c_num_brick_per_pau] =
    {
        GET_CS_CTL_MISC_CONFIG2_BRK0_OCAPI_MODE,
        GET_CS_CTL_MISC_CONFIG2_BRK1_OCAPI_MODE
    };
    getRegFldFuncType c_pau_0_cq_ctrl_misc_config2_get_nvlink_mode[c_num_brick_per_pau] =
    {
        GET_CS_CTL_MISC_CONFIG2_BRK0_NVLINK_MODE,
        GET_CS_CTL_MISC_CONFIG2_BRK1_NVLINK_MODE
    };

    //CPU_BAR functions
    getScomFuncType c_pau_0_gpu_bar_get_functions[c_num_brick_per_pau][c_num_gpu_bar_reg_per_brick] =
    {
        {
            GET_CS_SM0_SNP_MISC_GPU0_BAR, GET_CS_SM1_SNP_MISC_GPU0_BAR,
            GET_CS_SM2_SNP_MISC_GPU0_BAR, GET_CS_SM3_SNP_MISC_GPU0_BAR,
            GET_CS_CTL_MISC_GPU0_BAR, GET_XSL_MAIN_GPU0_BAR
        },
        {
            GET_CS_SM0_SNP_MISC_GPU1_BAR, GET_CS_SM1_SNP_MISC_GPU1_BAR,
            GET_CS_SM2_SNP_MISC_GPU1_BAR, GET_CS_SM3_SNP_MISC_GPU1_BAR,
            GET_CS_CTL_MISC_GPU1_BAR, GET_XSL_MAIN_GPU1_BAR
        }
    };
    clearRegFldFuncType c_pau_0_gpu_bar_clear_enable_functions[c_num_brick_per_pau][c_num_gpu_bar_reg_per_brick] =
    {
        {
            CLEAR_CS_SM0_SNP_MISC_GPU0_BAR_ENABLE, CLEAR_CS_SM1_SNP_MISC_GPU0_BAR_ENABLE,
            CLEAR_CS_SM2_SNP_MISC_GPU0_BAR_ENABLE, CLEAR_CS_SM3_SNP_MISC_GPU0_BAR_ENABLE,
            CLEAR_CS_CTL_MISC_GPU0_BAR_ENABLE, CLEAR_XSL_MAIN_GPU0_BAR_ENABLE
        },
        {
            CLEAR_CS_SM0_SNP_MISC_GPU1_BAR_ENABLE, CLEAR_CS_SM1_SNP_MISC_GPU1_BAR_ENABLE,
            CLEAR_CS_SM2_SNP_MISC_GPU1_BAR_ENABLE, CLEAR_CS_SM3_SNP_MISC_GPU1_BAR_ENABLE,
            CLEAR_CS_CTL_MISC_GPU1_BAR_ENABLE, CLEAR_XSL_MAIN_GPU1_BAR_ENABLE
        }
    };
    putScomFuncType c_pau_0_gpu_bar_put_functions[c_num_brick_per_pau][c_num_gpu_bar_reg_per_brick] =
    {
        {
            PUT_CS_SM0_SNP_MISC_GPU0_BAR, PUT_CS_SM1_SNP_MISC_GPU0_BAR,
            PUT_CS_SM2_SNP_MISC_GPU0_BAR, PUT_CS_SM3_SNP_MISC_GPU0_BAR,
            PUT_CS_CTL_MISC_GPU0_BAR, PUT_XSL_MAIN_GPU0_BAR
        },
        {
            PUT_CS_SM0_SNP_MISC_GPU1_BAR, PUT_CS_SM1_SNP_MISC_GPU1_BAR,
            PUT_CS_SM2_SNP_MISC_GPU1_BAR, PUT_CS_SM3_SNP_MISC_GPU1_BAR,
            PUT_CS_CTL_MISC_GPU1_BAR, PUT_XSL_MAIN_GPU1_BAR
        }
    };

    //NDT functions
    getScomFuncType c_pau_0_ndt_bar_get_functions[c_num_brick_per_pau][c_num_ndt_bar_reg_per_brick] =
    {
        {
            GET_CS_SM0_SNP_MISC_NDT0_BAR, GET_CS_SM1_SNP_MISC_NDT0_BAR,
            GET_CS_SM2_SNP_MISC_NDT0_BAR, GET_CS_SM3_SNP_MISC_NDT0_BAR
        },
        {
            GET_CS_SM0_SNP_MISC_NDT1_BAR, GET_CS_SM1_SNP_MISC_NDT1_BAR,
            GET_CS_SM2_SNP_MISC_NDT1_BAR, GET_CS_SM3_SNP_MISC_NDT1_BAR
        }
    };
    clearRegFldFuncType c_pau_0_ndt_bar_clear_enable_functions[c_num_brick_per_pau][c_num_ndt_bar_reg_per_brick] =
    {
        {
            CLEAR_CS_SM0_SNP_MISC_NDT0_BAR_CONFIG_NDT0_BAR_ENABLE, CLEAR_CS_SM1_SNP_MISC_NDT0_BAR_CONFIG_NDT0_BAR_ENABLE,
            CLEAR_CS_SM2_SNP_MISC_NDT0_BAR_CONFIG_NDT0_BAR_ENABLE, CLEAR_CS_SM3_SNP_MISC_NDT0_BAR_CONFIG_NDT0_BAR_ENABLE
        },
        {
            CLEAR_CS_SM0_SNP_MISC_NDT1_BAR_CONFIG_NDT1_BAR_ENABLE, CLEAR_CS_SM1_SNP_MISC_NDT1_BAR_CONFIG_NDT1_BAR_ENABLE,
            CLEAR_CS_SM2_SNP_MISC_NDT1_BAR_CONFIG_NDT1_BAR_ENABLE, CLEAR_CS_SM3_SNP_MISC_NDT1_BAR_CONFIG_NDT1_BAR_ENABLE
        }
    };
    putScomFuncType c_pau_0_ndt_bar_put_functions[c_num_brick_per_pau][c_num_ndt_bar_reg_per_brick] =
    {
        {
            PUT_CS_SM0_SNP_MISC_NDT0_BAR, PUT_CS_SM1_SNP_MISC_NDT0_BAR,
            PUT_CS_SM2_SNP_MISC_NDT0_BAR, PUT_CS_SM3_SNP_MISC_NDT0_BAR
        },
        {
            PUT_CS_SM0_SNP_MISC_NDT1_BAR, PUT_CS_SM1_SNP_MISC_NDT1_BAR,
            PUT_CS_SM2_SNP_MISC_NDT1_BAR, PUT_CS_SM3_SNP_MISC_NDT1_BAR
        }
    };

    //GENID_BAR functions
    getScomFuncType c_pau_0_gen_id_bar_get_functions[c_num_gen_id_bar_reg_per_pau] =
    {
        GET_CS_SM0_SNP_MISC_GENID_BAR,
        GET_CS_SM1_SNP_MISC_GENID_BAR,
        GET_CS_SM2_SNP_MISC_GENID_BAR,
        GET_CS_SM3_SNP_MISC_GENID_BAR
    };
    clearRegFldFuncType c_pau_0_gen_id_bar_clear_enable_functions[c_num_gen_id_bar_reg_per_pau] =
    {
        CLEAR_CS_SM0_SNP_MISC_GENID_BAR_CONFIG_GENID_BAR_ENABLE,
        CLEAR_CS_SM1_SNP_MISC_GENID_BAR_CONFIG_GENID_BAR_ENABLE,
        CLEAR_CS_SM2_SNP_MISC_GENID_BAR_CONFIG_GENID_BAR_ENABLE,
        CLEAR_CS_SM3_SNP_MISC_GENID_BAR_CONFIG_GENID_BAR_ENABLE
    };
    putScomFuncType c_pau_0_gen_id_bar_put_functions[c_num_gen_id_bar_reg_per_pau] =
    {
        PUT_CS_SM0_SNP_MISC_GENID_BAR,
        PUT_CS_SM1_SNP_MISC_GENID_BAR,
        PUT_CS_SM2_SNP_MISC_GENID_BAR,
        PUT_CS_SM3_SNP_MISC_GENID_BAR
    };

    //MMIO_BAR functions
    getScomFuncType c_pau_0_mmio_bar_get_functions[c_num_mmio_bar_reg_per_pau] =
    {
        GET_CS_SM0_SNP_MISC_PAUMMIO_BAR,
        GET_CS_SM1_SNP_MISC_PAUMMIO_BAR,
        GET_CS_SM2_SNP_MISC_PAUMMIO_BAR,
        GET_CS_SM3_SNP_MISC_PAUMMIO_BAR
    };
    clearRegFldFuncType c_pau_0_mmio_bar_clear_enable_functions[c_num_mmio_bar_reg_per_pau] =
    {
        CLEAR_CS_SM0_SNP_MISC_PAUMMIO_BAR_CONFIG_PAUMMIO_BAR_ENABLE,
        CLEAR_CS_SM1_SNP_MISC_PAUMMIO_BAR_CONFIG_PAUMMIO_BAR_ENABLE,
        CLEAR_CS_SM2_SNP_MISC_PAUMMIO_BAR_CONFIG_PAUMMIO_BAR_ENABLE,
        CLEAR_CS_SM3_SNP_MISC_PAUMMIO_BAR_CONFIG_PAUMMIO_BAR_ENABLE
    };
    putScomFuncType c_pau_0_mmio_bar_put_functions[c_num_mmio_bar_reg_per_pau] =
    {
        PUT_CS_SM0_SNP_MISC_PAUMMIO_BAR,
        PUT_CS_SM1_SNP_MISC_PAUMMIO_BAR,
        PUT_CS_SM2_SNP_MISC_PAUMMIO_BAR,
        PUT_CS_SM3_SNP_MISC_PAUMMIO_BAR
    };

    // Looping over all powered PAUs
    for(const auto& l_pau_target :
        i_target.getChildren<fapi2::TARGET_TYPE_PAU>(fapi2::TARGET_STATE_FUNCTIONAL))
    {
        FAPI_INF("Quiescing PAU: 0x%lX", l_pau_target.get());

        // Place both OTLs into reset state
        for(uint32_t i = 0; i < c_num_brick_per_pau; i++)
        {
            // Check whether OTL is enabled or not
            FAPI_TRY(c_pau_0_cq_ctrl_status_get_functions[i](l_pau_target, l_data));
            c_pau_0_cq_ctrl_status_get_am_fenced_functions[i](l_data, l_fenced_data);

            if(l_fenced_data == c_otl_disabled_status)
            {
                FAPI_INF("The OTL:%d is already disabled, l_fenced_data = %u",
                         i, l_fenced_data);
                continue;
            }

            l_data.flush<0>();
            FAPI_TRY(c_pau_0_fence_ctrl_prep_functions[i](l_pau_target));
            c_pau_0_fence_ctrl_set_request_fence_functions[i](c_otl_reset_cmd, l_data);
            FAPI_TRY(c_pau_0_fence_ctrl_put_functions[i](l_pau_target, l_data));

            // Check the configuration to find the expected reset status
            l_misc_config2.flush<0>();
            FAPI_TRY(GET_CS_CTL_MISC_CONFIG2(l_pau_target, l_misc_config2));
            c_pau_0_cq_ctrl_misc_config2_get_ocapi_mode[i](l_misc_config2, l_ocapi_enabled);
            c_pau_0_cq_ctrl_misc_config2_get_nvlink_mode[i](l_misc_config2, l_nvlink_enabled);

            if(l_ocapi_enabled || l_nvlink_enabled)
            {
                l_otl_reset_status = c_otl_reset_status_ocapi_nvlink;
            }
            else
            {
                l_otl_reset_status = c_otl_reset_status_not_ocapi_nvlink;
            }

            for (uint32_t j = 0; j < C_NUM_TRIES_QUIESCE_STATE; j++)
            {
                FAPI_TRY(c_pau_0_cq_ctrl_status_get_functions[i](l_pau_target, l_data));
                c_pau_0_cq_ctrl_status_get_am_fenced_functions[i](l_data, l_fenced_data);

                if(l_fenced_data == l_otl_reset_status)
                {
                    break;
                }

                FAPI_TRY(fapi2::delay(C_PAU_DELAY_NS, C_PAU_DELAY_CYCLES));
            }

            FAPI_DBG("Brick number:%u, l_fenced_data:%u", i, l_fenced_data);

            FAPI_ASSERT(
                (l_fenced_data == l_otl_reset_status),
                fapi2::P10_OTL_NOT_IN_RESET()
                .set_PROC_TARGET(i_target)
                .set_PAU_TARGET(l_pau_target)
                .set_STATUS_ADDR(c_pau_0_cq_ctrl_status_address[i])
                .set_STATUS_DATA(l_data)
                .set_MISC_CONFIG2_DATA(l_misc_config2),
                "One of the OTLs are not in the reset state");
        }

        l_data.flush<0>();
        // Place all bricks into fence state
        FAPI_TRY(PREP_MISC_FENCE_STATE(l_pau_target));
        // No scom filed accessors are avialable for these fields
        l_data.insertFromRight<c_misc_fence_state_clear,
                               c_num_brick_per_pau>(c_fence_all_brick_cmd);
        FAPI_TRY(PUT_MISC_FENCE_STATE(l_pau_target, l_data));

        // Clear all interrupt requests
        l_data.flush<0>();
        FAPI_TRY(PREP_MISC_REGS_INT_REQ(l_pau_target));
        // Writing zeros to all bits, since it will not affect reserved fields
        FAPI_TRY(PUT_MISC_REGS_INT_REQ(l_pau_target, l_data));

        // Disable all PAU BARs
        for(uint32_t i = 0; i < c_num_brick_per_pau; i++)
        {
            for(uint8_t j = 0; j < c_num_gpu_bar_reg_per_brick; j++)
            {
                FAPI_TRY(c_pau_0_gpu_bar_get_functions[i][j](l_pau_target, l_data));
                c_pau_0_gpu_bar_clear_enable_functions[i][j](l_data);
                FAPI_TRY(c_pau_0_gpu_bar_put_functions[i][j](l_pau_target, l_data));
            }
        }

        for(uint32_t i = 0; i < c_num_brick_per_pau; i++)
        {
            for(uint8_t j = 0; j < c_num_ndt_bar_reg_per_brick; j++)
            {
                FAPI_TRY(c_pau_0_ndt_bar_get_functions[i][j](l_pau_target, l_data));
                c_pau_0_ndt_bar_clear_enable_functions[i][j](l_data);
                FAPI_TRY(c_pau_0_ndt_bar_put_functions[i][j](l_pau_target, l_data));
            }
        }

        for(uint8_t i = 0; i < c_num_gen_id_bar_reg_per_pau; i++)
        {
            FAPI_TRY(c_pau_0_gen_id_bar_get_functions[i](l_pau_target, l_data));
            c_pau_0_gen_id_bar_clear_enable_functions[i](l_data);
            FAPI_TRY(c_pau_0_gen_id_bar_put_functions[i](l_pau_target, l_data));
        }

        for(uint8_t i = 0; i < c_num_mmio_bar_reg_per_pau; i++)
        {
            FAPI_TRY(c_pau_0_mmio_bar_get_functions[i](l_pau_target, l_data));
            c_pau_0_mmio_bar_clear_enable_functions[i](l_data);
            FAPI_TRY(c_pau_0_mmio_bar_put_functions[i](l_pau_target, l_data));
        }
    }

fapi_try_exit:
    FAPI_DBG("p10_pau_check_quiesce: Exiting....");
    return fapi2::current_err;
}

//---------------------------------------------------------------------------------
// NOTE: description in header
//---------------------------------------------------------------------------------
fapi2::ReturnCode p10_nx_check_quiesce(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    FAPI_DBG("p10_nx_check_quiesce: Entering....");
    fapi2::buffer<uint64_t> l_dma_status_reg_data(0);
    fapi2::buffer<uint64_t> l_data(0);

    //If (DMA Status Register[HMI Ative])
    FAPI_TRY(GET_NX_DMA_SU_STATUS(i_target, l_dma_status_reg_data));

    // If HMI has been signaled, wait for power bus interface to go idle.
    if (GET_NX_DMA_SU_STATUS_HMI_ACTIVE(l_dma_status_reg_data))
    {
        for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
        {
            FAPI_TRY(GET_FBC_CQ_WRAP_NXCQ_SCOM_NX_PB_ERR_RPT_0(i_target, l_data));

            if (GET_FBC_CQ_WRAP_NXCQ_SCOM_NX_PB_ERR_RPT_0_NX_PBI_WRITE_IDLE(l_data))
            {
                break;
            }

            FAPI_TRY(fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396));
        }

        FAPI_ASSERT(GET_FBC_CQ_WRAP_NXCQ_SCOM_NX_PB_ERR_RPT_0_NX_PBI_WRITE_IDLE(l_data),
                    fapi2::P10_NX_PBI_WRITE_IDLE_TIMEOUT()
                    .set_TARGET(i_target)
                    .set_NX_PB_ERR_RPT_0(l_data), "PBI Write Idle never happened");
    }

    //Any CRB kill must be complete before issuing the following sequence
    FAPI_TRY(GET_NX_DMA_SU_CRB_KILL_REQ(i_target, l_data));

    if (GET_NX_DMA_SU_CRB_KILL_REQ_ENABLE(l_data))
    {
        //Wait for completing the kill request.
        for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
        {
            FAPI_TRY(GET_NX_DMA_SU_CRB_KILL_REQ(i_target, l_data));

            if (GET_NX_DMA_SU_CRB_KILL_REQ_DONE(l_data))
            {
                break;
            }

            FAPI_TRY(fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396));
        }

        FAPI_ASSERT(GET_NX_DMA_SU_CRB_KILL_REQ_DONE(l_data),
                    fapi2::P10_NX_CRB_KILL_DONE_TIMEOUT()
                    .set_TARGET(i_target)
                    .set_NX_DMA_SU_CRB_KILL_REQ(l_data), "CRB kills were not complete");
    }

    //Stop UMAC from fetching new CRBs
    FAPI_TRY(GET_NX_PBI_UMAC_STATUS_CONTROL(i_target, l_data));
    CLEAR_NX_PBI_UMAC_STATUS_CONTROL_CRB_READS_ENBL(l_data);
    FAPI_TRY(PUT_NX_PBI_UMAC_STATUS_CONTROL(i_target, l_data));

    for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
    {
        FAPI_TRY(GET_NX_PBI_UMAC_STATUS_CONTROL(i_target, l_data));

        if(GET_NX_PBI_UMAC_STATUS_CONTROL_CRB_READS_HALTED(l_data))
        {
            break;
        }

        FAPI_TRY(fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396));
    }

    FAPI_ASSERT(GET_NX_PBI_UMAC_STATUS_CONTROL_CRB_READS_HALTED(l_data),
                fapi2::P10_NX_STOP_UMAC_FETCHING_NEW_CRBS_TIMEOUT()
                .set_TARGET(i_target)
                .set_NX_PBI_UMAC_STATUS_CONTROL(l_data),
                "UMAC was not stopped from fetching new CRBs");

    //Wait for UMAC dispatch slots to drain of CRBs
    for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
    {
        FAPI_TRY(GET_NX_PBI_UMAC_STATUS_CONTROL(i_target, l_data));

        if(GET_NX_PBI_UMAC_STATUS_CONTROL_UMAC_QUIESCED(l_data))
        {
            break;
        }

        FAPI_TRY(fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396));
    }

    FAPI_ASSERT(GET_NX_PBI_UMAC_STATUS_CONTROL_UMAC_QUIESCED(l_data),
                fapi2::P10_NX_UMAC_DISPATCH_SLOTS_TO_DRAIN_CRBS_TIMEOUT()
                .set_TARGET(i_target)
                .set_NX_PBI_UMAC_STATUS_CONTROL(l_data),
                "UMAC was not done dispatching slots to drain of CRBs");

    //Wait for DMA channels to drain
    for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
    {
        FAPI_TRY(GET_NX_DMA_SU_STATUS(i_target, l_dma_status_reg_data));

        if(GET_NX_DMA_SU_STATUS_DMA_CH0_IDLE(l_dma_status_reg_data)
           && GET_NX_DMA_SU_STATUS_DMA_CH1_IDLE(l_dma_status_reg_data)
           && GET_NX_DMA_SU_STATUS_DMA_CH2_IDLE(l_dma_status_reg_data)
           && GET_NX_DMA_SU_STATUS_DMA_CH3_IDLE(l_dma_status_reg_data)
           && GET_NX_DMA_SU_STATUS_DMA_CH4_IDLE(l_dma_status_reg_data))
        {
            break;
        }

        fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396);
    }

    FAPI_ASSERT((GET_NX_DMA_SU_STATUS_DMA_CH0_IDLE(l_dma_status_reg_data)
                 && GET_NX_DMA_SU_STATUS_DMA_CH1_IDLE(l_dma_status_reg_data)
                 && GET_NX_DMA_SU_STATUS_DMA_CH2_IDLE(l_dma_status_reg_data)
                 && GET_NX_DMA_SU_STATUS_DMA_CH3_IDLE(l_dma_status_reg_data)
                 && GET_NX_DMA_SU_STATUS_DMA_CH4_IDLE(l_dma_status_reg_data)),
                fapi2::P10_WAIT_FOR_DMA_CHANNELS_TO_DRAIN_TIMEOUT()
                .set_TARGET(i_target)
                .set_NX_DMA_SU_STATUS(l_dma_status_reg_data),
                "DMA channels were not drained");

    //Wait for ERAT to be idle. Should be trivially idle because of the above.
    for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
    {
        FAPI_TRY(GET_NX_PBI_SHIM_ERAT_STATUS_CONTROL(i_target, l_data));

        if (GET_NX_PBI_SHIM_ERAT_STATUS_CONTROL_ERAT_IDLE(l_data))
        {
            break;
        }

        FAPI_TRY(fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396));
    }

    FAPI_ASSERT(GET_NX_PBI_SHIM_ERAT_STATUS_CONTROL_ERAT_IDLE(l_data),
                fapi2::P10_WAIT_FOR_ERAT_IDLE()
                .set_TARGET(i_target)
                .set_NX_PBI_SHIM_ERAT_STATUS_CONTROL(l_data),
                "ERAT was not idle");

    //Wait for PBI master machines to be idle
    for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
    {
        FAPI_TRY(GET_NX_DMA_SU_STATUS(i_target, l_dma_status_reg_data));

        if (GET_NX_DMA_SU_STATUS_PBI_IDLE(l_dma_status_reg_data))
        {
            break;
        }

        FAPI_TRY(fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396));
    }

    FAPI_ASSERT(GET_NX_DMA_SU_STATUS_PBI_IDLE(l_dma_status_reg_data),
                fapi2::P10_PBI_MASTER_MACHINES_IDLE_TIMEOUT()
                .set_TARGET(i_target)
                .set_NX_DMA_SU_STATUS(l_dma_status_reg_data),
                "PBI Master Machines are not idle");

    // If this procedure is followed, then usually if UMAC Status and
    // Control Register[Quiesce Request] is written to 1 then hardware will
    // trivially respond UMAC Status and Control Register[Quiesce Achieved] = 1
    FAPI_TRY(GET_NX_PBI_UMAC_STATUS_CONTROL(i_target, l_data));
    SET_NX_PBI_UMAC_STATUS_CONTROL_QUIESCE_REQUEST(l_data);
    CLEAR_NX_PBI_UMAC_STATUS_CONTROL_QUIESCE_ACHEIVED(l_data);
    CLEAR_NX_PBI_UMAC_STATUS_CONTROL_QUIESCE_FAILED(l_data);
    FAPI_TRY(PUT_NX_PBI_UMAC_STATUS_CONTROL(i_target, l_data));

    // Poll UMAC Control register status register
    for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
    {
        FAPI_TRY(GET_NX_PBI_UMAC_STATUS_CONTROL(i_target, l_data));

        if (!GET_NX_PBI_UMAC_STATUS_CONTROL_QUIESCE_FAILED(l_data))
        {
            break;
        }

        FAPI_TRY(fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396));
    }


    FAPI_ASSERT(!GET_NX_PBI_UMAC_STATUS_CONTROL_QUIESCE_FAILED(l_data),
                fapi2::P10_UMAC_QUIESCE_FAILED()
                .set_TARGET(i_target)
                .set_NX_PBI_UMAC_STATUS_CONTROL(l_data),
                "UMAC status control quiesce failed");

fapi_try_exit:
    FAPI_DBG("p10_nx_check_quiesce: Exiting....");
    return fapi2::current_err;
}

//---------------------------------------------------------------------------------
// NOTE: description in header
//---------------------------------------------------------------------------------
fapi2::ReturnCode p10_psihb_check_quiesce(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    FAPI_DBG("p10_psihb_check_quiesce: Entering...");
    fapi2::buffer<uint64_t> l_psihb_data(0);
    const uint32_t c_error_mask_disable_all = 0xFFFul;

    // Disable FSP Command Enable bit in PSIHB Command/Status register

    FAPI_TRY(PREP_TP_TPBR_PSIHB_STATUS_CTL_REG_WO_CLEAR(i_target));

    SET_TP_TPBR_PSIHB_STATUS_CTL_REG_FSP_CMD_ENABLE(l_psihb_data);
    SET_TP_TPBR_PSIHB_STATUS_CTL_REG_FSP_INT_ENABLE(l_psihb_data);
    FAPI_TRY(PUT_TP_TPBR_PSIHB_STATUS_CTL_REG_WO_CLEAR(i_target, l_psihb_data));

    //mask all interrupts to quiesce
    FAPI_TRY(GET_TP_TPBR_PSIHB_ERROR_MASK_REG(i_target, l_psihb_data));
    SET_TP_TPBR_PSIHB_ERROR_MASK_REG_INTERRUPT_DISABLE(c_error_mask_disable_all, l_psihb_data);
    FAPI_TRY(PUT_TP_TPBR_PSIHB_ERROR_MASK_REG(i_target, l_psihb_data));

    //Poll PSIHBCR bit 20 - inbound queue empty to be 0b0 for quiesce state
    for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
    {
        FAPI_TRY(GET_TP_TPBR_PSIHB_STATUS_CTL_REG(i_target, l_psihb_data));

        if (!GET_TP_TPBR_PSIHB_STATUS_CTL_REG_FSP_INBOUND_ACTIVE(l_psihb_data))
        {
            break;
        }

        FAPI_TRY(fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396));
    }

    FAPI_ASSERT(!GET_TP_TPBR_PSIHB_STATUS_CTL_REG_FSP_INBOUND_ACTIVE(l_psihb_data),
                fapi2::P10_PSIHBCR_INBOUND_QUEUE_NOT_EMPTY()
                .set_TARGET(i_target)
                .set_TPBR_PSIHB_STATUS_CTL_REG(l_psihb_data), "PSIHBCR inbound queue not empty");

    // Disable TCE access by clearing the secure boot register - can't be done
    // later because it's a security hole
    l_psihb_data.flush<0>();
    FAPI_TRY(PREP_TP_TPBR_PSIHB_TRUST_CONTROL(i_target));
    FAPI_TRY(PUT_TP_TPBR_PSIHB_TRUST_CONTROL(i_target, l_psihb_data));

    // Reset interrupt state machine & set mode to LSI
    FAPI_TRY(GET_TP_TPBR_PSIHB_INTERRUPT_CONTROL(i_target, l_psihb_data));
    SET_TP_TPBR_PSIHB_INTERRUPT_CONTROL_INTERRUPT_SM_RESET(l_psihb_data);
    SET_TP_TPBR_PSIHB_INTERRUPT_CONTROL_ESB_OR_LSI_INTERRUPTS(l_psihb_data);
    FAPI_TRY(PUT_TP_TPBR_PSIHB_INTERRUPT_CONTROL(i_target, l_psihb_data));

    // Reset notify address valid bit
    FAPI_TRY(GET_TP_TPBR_PSIHB_ESB_NOTIFY(i_target, l_psihb_data));
    SET_TP_TPBR_PSIHB_ESB_NOTIFY_VALID(l_psihb_data);
    FAPI_TRY(PUT_TP_TPBR_PSIHB_ESB_NOTIFY(i_target, l_psihb_data));

fapi_try_exit:
    FAPI_DBG("p10_psihb_check_quiesce: Exiting...");
    return fapi2::current_err;
}


//---------------------------------------------------------------------------------
// NOTE: description in header
//---------------------------------------------------------------------------------
fapi2::ReturnCode p10_vas_check_quiesce(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    FAPI_DBG("p10_vas_check_quiesce: Entering...");
    fapi2::buffer<uint64_t> l_vas_north_misc_ctl_data(0);
    fapi2::buffer<uint64_t> l_vas_south_misc_ctl_data(0);

    // VAS needs to be quiesced before NX

    // Read the VAS Misc status and North control register so we don't write over anything
    FAPI_TRY(GET_VAS_VA_RG_SCF_MISCCTL(i_target, l_vas_north_misc_ctl_data));

    // Set the 'Quiesce Requested' bit in the VAS Miscellaneous Status and
    // North Control Register to a 1. This will prevent VAS from
    // accepting new paste or write monitor operations
    SET_VAS_VA_RG_SCF_MISCCTL_QUIESCE_REQUEST(l_vas_north_misc_ctl_data);
    FAPI_TRY(PUT_VAS_VA_RG_SCF_MISCCTL(i_target, l_vas_north_misc_ctl_data));

    // Check that VAS has quiesced. This is accomplished by reading two
    // status registers. The "RG is Idle' bit in the VAS Miscellaneous
    // Status and North Control Register as well as the 'EG is Idle',
    // 'CQ is Idle' and 'WC is Idle' bits in the VAS Miscellaneous Status
    // and South Control Register must all be set to one to indicate that
    // VAS has gone idle.
    bool l_misc_ctl_rg_is_idle;

    for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
    {
        // Read VAS Misc status and North control register to ensure
        // 'RG is idle' is set -
        FAPI_TRY(GET_VAS_VA_RG_SCF_MISCCTL(i_target, l_vas_north_misc_ctl_data));
        l_misc_ctl_rg_is_idle =
            GET_VAS_VA_RG_SCF_MISCCTL_RG_IS_IDLE(l_vas_north_misc_ctl_data);

        // Read VAS Misc status and South control register to ensure
        // 'WC is idle', 'CQ is idle' and 'EG is idle' bits are set -
        FAPI_TRY(GET_VAS_VA_EG_SCF_SOUTHCTL(i_target, l_vas_south_misc_ctl_data));

        if(l_misc_ctl_rg_is_idle
           && GET_VAS_VA_EG_SCF_SOUTHCTL_WC_IDLE_BIT(l_vas_south_misc_ctl_data)
           && GET_VAS_VA_EG_SCF_SOUTHCTL_CQ_IDLE_BIT(l_vas_south_misc_ctl_data)
           && GET_VAS_VA_EG_SCF_SOUTHCTL_EG_IDLE_BIT(l_vas_south_misc_ctl_data))
        {
            break;
        }

        FAPI_TRY(fapi2::delay(C_DELAY_NS_396, C_DELAY_CYCLES_396));
    }

    // In order to prevent additional FIFO entries from getting posted
    // to the NX receive FIFOs while trying to quiesce NX, software may
    // wish to close all windows to prevent users from continuing to try
    // to access the accelerators. Software may close all windows by
    // writing the Open/Enable bit to a zero in the Window Control
    // Register<n>. This step is optional, but should be done as part of
    // an orderly shut down of a user's access.

    // Software may also choose to deallocate any pages that partitions
    // (or users) have mapped to VAS' MMIO space. In a general use case,
    // a partition (or user) will have pages that map to VAS' MMIO space
    // to allow the partition to return credits via the Local Receive
    // Window Credit Adder Register <m>. In order to stop MMIO traffic,
    // these pages should be unmapped. In a NX only usage model, this step
    // can be ignored as long as the Quiesce NX procedures are followed.

    FAPI_ASSERT((l_misc_ctl_rg_is_idle
                 && GET_VAS_VA_EG_SCF_SOUTHCTL_WC_IDLE_BIT(l_vas_south_misc_ctl_data)
                 && GET_VAS_VA_EG_SCF_SOUTHCTL_CQ_IDLE_BIT(l_vas_south_misc_ctl_data)
                 && GET_VAS_VA_EG_SCF_SOUTHCTL_EG_IDLE_BIT(l_vas_south_misc_ctl_data)),
                fapi2::P10_VAS_QUIESCE_TIMEOUT()
                .set_TARGET(i_target)
                .set_NORTHDATA(l_vas_north_misc_ctl_data)
                .set_SOUTHDATA(l_vas_south_misc_ctl_data),
                "VAS quiesce timed out");

    // Write Invalidate CAM location field of North register (optional)
    FAPI_TRY(PREP_VAS_VA_RG_SCF_MISCCTL(i_target));
    SET_VAS_VA_RG_SCF_MISCCTL_INVALIDATE_CAM_ALL(l_vas_north_misc_ctl_data);
    FAPI_TRY(PUT_VAS_VA_RG_SCF_MISCCTL(i_target, l_vas_north_misc_ctl_data));

fapi_try_exit:
    FAPI_DBG("p10_vas_check_quiesce: Exiting...");
    return fapi2::current_err;
}

//---------------------------------------------------------------------------------
// NOTE: description in header
//---------------------------------------------------------------------------------
fapi2::ReturnCode p10_int_scrub_caches(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    const uint64_t c_flush_poll_cmd = 0ull;
    fapi2::buffer<uint64_t> l_scrub_trig_data(0);
    const uint64_t l_cache_ctrl_addrs[] =
    {
        INT_VC_ESBC_FLUSH_CTRL,
        INT_VC_EASC_FLUSH_CTRL,
        INT_VC_ENDC_FLUSH_CTRL,
        INT_PC_NXC_REGS_FLUSH_CTRL
    };

    //Start the scrub operation in all caches
    for (uint32_t i = 0; i < 4; i++)
    {
        // (flush_poll address) = (flush_ctrl address) + 1
        fapi2::putScom(i_target, l_cache_ctrl_addrs[i] + 1, c_flush_poll_cmd);

        for (uint32_t j = 0; j < C_NUM_TRIES_QUIESCE_STATE; j++)
        {
            FAPI_TRY(fapi2::delay(C_INTP_DELAY_NS, C_INTP_DELAY_CYCLES));
            FAPI_TRY(fapi2::getScom(i_target, l_cache_ctrl_addrs[i], l_scrub_trig_data));

            if (!l_scrub_trig_data.getBit<0>())
            {
                break;
            }
        }

        FAPI_ASSERT(!l_scrub_trig_data.getBit<0>(),
                    fapi2::P10_INT_SCRUB_NOT_FINISHED_ERR()
                    .set_TARGET(i_target)
                    .set_FLUSH_CTRL_ADDRESS(l_cache_ctrl_addrs[i])
                    .set_FLUSH_CTRL_DATA(l_scrub_trig_data),
                    "INT scrub operation still busy");
    }

fapi_try_exit:
    return fapi2::current_err;
}

//---------------------------------------------------------------------------------
// NOTE: description in header
//---------------------------------------------------------------------------------
fapi2::ReturnCode p10_intp_check_quiesce(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    FAPI_DBG("p10_intp_check_quiesce: Entering...");
    fapi2::buffer<uint64_t> l_data(0);

    //Start the scrub operation in all caches
    FAPI_TRY(p10_int_scrub_caches(i_target), "Error scrubbing the caches");

    // Read INT_CQ_RST_CTL so that we don't override anything
    FAPI_TRY(GET_INT_CQ_RST_CTL(i_target, l_data));

    // Set bit in INT_CQ_RST_CTL to request quiesce
    SET_INT_CQ_RST_CTL_QUIESCE_PB(l_data);

    FAPI_TRY(PUT_INT_CQ_RST_CTL(i_target, l_data));

    // Poll master and slave quiesced via bits in RST_CTL
    for (uint32_t i = 0; i < C_NUM_TRIES_QUIESCE_STATE; i++)
    {
        FAPI_TRY(GET_INT_CQ_RST_CTL(i_target, l_data));

        if (GET_INT_CQ_RST_CTL_MASTER_IDLE(l_data) &&
            GET_INT_CQ_RST_CTL_SLAVE_IDLE(l_data))
        {
            break;
        }

        FAPI_TRY(fapi2::delay(C_INTP_DELAY_NS, C_INTP_DELAY_CYCLES));
    }

    FAPI_ASSERT((GET_INT_CQ_RST_CTL_MASTER_IDLE(l_data) &&
                 GET_INT_CQ_RST_CTL_SLAVE_IDLE(l_data)),
                fapi2::P10_INTP_QUIESCE_TIMEOUT()
                .set_TARGET(i_target)
                .set_RST_CTL_DATA(l_data),
                "INTP master or slave is not IDLE");

    //Set sync_reset in RST_CTL
    SET_INT_CQ_RST_CTL_SYNC_RESET(l_data);
    FAPI_TRY(PUT_INT_CQ_RST_CTL(i_target, l_data));

    //Resetting of interrupt unit clears out the thread context
    //configuration, call p10_sbe_scominit_int to set the thread
    //context again
    FAPI_TRY(p10_sbe_scominit_int(i_target), "ERROR calling p10_sbe_scominit_int");

fapi_try_exit:
    FAPI_DBG("p10_intp_check_quiesce: Exiting...");
    return fapi2::current_err;
}

//--------------------------------------------------------------------------
//  HWP entry point
//--------------------------------------------------------------------------
fapi2::ReturnCode p10_pm_clear_special_wakeup(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    FAPI_DBG("p10_pm_clear_special_wakeup: Entering..");

    using namespace scomt::c;

    typedef fapi2::ReturnCode (*prepCoreScomFuncType)(
        const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target);
    typedef fapi2::ReturnCode (*putCoreScomFuncType)(
        const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_target,
        const uint64_t i_data);

    const uint8_t c_qme_spwu_count = 4;
    prepCoreScomFuncType c_prep_qme_spwu[c_qme_spwu_count] =
    {
        PREP_QME_SPWU_OTR,
        PREP_QME_SPWU_FSP,
        PREP_QME_SPWU_HYP,
        PREP_QME_SPWU_OCC
    };

    putCoreScomFuncType c_put_qme_spwu[c_qme_spwu_count] =
    {
        PUT_QME_SPWU_OTR,
        PUT_QME_SPWU_FSP,
        PUT_QME_SPWU_HYP,
        PUT_QME_SPWU_OCC
    };

    for(auto& l_childCore :
        i_target.getChildren<fapi2::TARGET_TYPE_CORE>())
    {
        for (uint8_t i = 0; i < c_qme_spwu_count; i++)
        {
            FAPI_TRY(c_prep_qme_spwu[i](l_childCore));

            uint64_t l_data = 0;
            FAPI_TRY(c_put_qme_spwu[i](l_childCore, l_data));
        }
    }

fapi_try_exit:
    FAPI_DBG("p10_pm_clear_special_wakeup: Exiting...");
    return fapi2::current_err;
}

//--------------------------------------------------------------------------
//  HWP entry point
//--------------------------------------------------------------------------
fapi2::ReturnCode p10_sbe_check_quiesce(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    FAPI_DBG("p10_sbe_check_quiesce: Entering..");

    FAPI_TRY(p10_phb_check_quiesce(i_target), "Error from p10_phb_check_quiesce");
    FAPI_TRY(p10_pau_check_quiesce(i_target), "Error from p10_pau_check_quiesce");
    FAPI_TRY(p10_vas_check_quiesce(i_target), "Error from p10_vas_check_quiesce");
    FAPI_TRY(p10_nx_check_quiesce(i_target), "Error from p10_nx_check_quiesce");
    FAPI_TRY(p10_psihb_check_quiesce(i_target), "Error from p10_psihb_check_quiesce");
    FAPI_TRY(p10_intp_check_quiesce(i_target), "Error from p10_intp_check_quiesce");

    //We also need to clean up any active special wakeups
    FAPI_TRY(p10_pm_clear_special_wakeup(i_target), "Error from p10_pm_clear_special_wakeup");

fapi_try_exit:
    fapi2::ReturnCode saveError = fapi2::current_err;
    fapi2::buffer<uint64_t> l_data(0);

    //If the quiesce fails then checkstop the system
    if (fapi2::current_err)
    {
        //Checkstop the system
        l_data.setBit<56>();
        fapi2::ReturnCode rc = fapi2::putScom(i_target, TP_TCN1_N1_LOCAL_FIR_WO_OR, l_data);

        if (rc)
        {
            FAPI_ERR("ERROR: There was an error doing the checkstop, "
                     "it may not have gone through");
        }
    }

    FAPI_DBG("p10_sbe_check_quiesce: Exiting..");
    return saveError;
}
