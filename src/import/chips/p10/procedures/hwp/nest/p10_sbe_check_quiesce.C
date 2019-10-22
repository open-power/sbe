/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/nest/p10_sbe_check_quiesce.C $ */
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
#include <p10_scom_proc.H>
#include <p10_scom_nmmu.H>

//-----------------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------------
//This is how many times each unit will try to do the quiesce if it has a wait for some cycles
const uint32_t C_NUM_TRIES_QUIESCE_STATE = 10000;
//These are the delays for the units that need them
const uint32_t C_DELAY_NS_396      = 396000000 / C_NUM_TRIES_QUIESCE_STATE; //396 ms
const uint32_t C_DELAY_CYCLES_396 = 792000000 / C_NUM_TRIES_QUIESCE_STATE; //2GHz * 396 ms

using namespace scomt;
using namespace scomt::proc;
using namespace scomt::nmmu;
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

    FAPI_TRY(PREP_TP_TPBR_PSIHB_STATUS_CTL_REG_SCOM2(i_target));

    SET_TP_TPBR_PSIHB_STATUS_CTL_REG_FSP_CMD_ENABLE(l_psihb_data);
    SET_TP_TPBR_PSIHB_STATUS_CTL_REG_FSP_INT_ENABLE(l_psihb_data);
    FAPI_TRY(PUT_TP_TPBR_PSIHB_STATUS_CTL_REG_SCOM2(i_target, l_psihb_data));

    //mask all interrupts to quiesce
    FAPI_TRY(GET_TP_TPBR_PSIHB_ERROR_MASK_REG(i_target, l_psihb_data));
    SET_TP_TPBR_PSIHB_ERROR_MASK_REG_INTERRUPT_DISABLE(c_error_mask_disable_all, l_psihb_data);
    FAPI_TRY(PUT_TP_TPBR_PSIHB_ERROR_MASK_REG(i_target, l_psihb_data));

    //Poll PSIHBCR bit 20 - inbound queue empty to be 0b0 for quiesce state
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

fapi_try_exit:
    FAPI_DBG("p10_psihb_check_quiesce: Exiting...");
    return fapi2::current_err;
}

//--------------------------------------------------------------------------
//  HWP entry point
//--------------------------------------------------------------------------
fapi2::ReturnCode p10_sbe_check_quiesce(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    FAPI_DBG("p10_sbe_check_quiesce: Entering..");

    // SBE will check quiesce state for all units on the powerbus on its chip
    // TODO (SW477416): Implement HW Procedure for checking the quiesce state of
    // CAPP, PHB, NPU and VAS.

    FAPI_TRY(p10_nx_check_quiesce(i_target), "Error from p10_nx_check_quiesce");
    FAPI_TRY(p10_psihb_check_quiesce(i_target), "Error from p10_psihb_check_quiesce");

    // TODO (SW477416): Implement HW Procedure for checking the quiesce state of INTP.

    //We also need to clean up any active special wakeups, and redirect
    //special wakeups to the SGPE
    // TODO (SW477416):

fapi_try_exit:
    // TODO (SW477416): If the quiesce fails then checkstop the system

    FAPI_DBG("p10_sbe_check_quiesce: Exiting..");
    return fapi2::current_err;
}
