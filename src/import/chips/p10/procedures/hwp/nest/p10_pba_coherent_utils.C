/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/nest/p10_pba_coherent_utils.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019                             */
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
//-----------------------------------------------------------------------------------
//
/// @file p10_pba_coherent_utils.C
/// @brief PBA alter/display library functions (FAPI)
///
// *HWP HW Maintainer : Chris Riedl (cmr@ibm.com)
// *HWP FW Maintainer : Raja Das    (rajadas2@in.ibm.com)
// *HWP Consumed by   : SBE
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------
#include <p10_pba_coherent_utils.H>
#include <fapi2_mem_access.H>

#include <p10_scom_c.H>
#include <p10_scom_perv.H>
#include <p10_scom_proc.H>
#include <p10_fbc_utils.H>

extern "C"
{
    //---------------------------------------------------------------------------------
    // Constant definitions
    //---------------------------------------------------------------------------------

    //PBA Delay Constants
    const uint32_t PBA_SLVRST_DELAY_HW_NS = 1000;
    const uint32_t PBA_SLVRST_DELAY_SIM_CYCLES = 200;
    const uint32_t WRITE_DELAY_HW_NS = 100;
    const uint32_t WRITE_DELAY_SIM_CYCLES = 20;
    const uint32_t PBA_BAR_SCOPE_LOCAL_NODE = 0;

//PBA Slave Control register field/bit definitions
    const uint32_t PBA_SLVCTL_EXTADDR_SHIFT = 27;
    const uint32_t PBA_SLVCTL_EXTADDR_MASK = 0x3fff;

//PBA Slave Reset register field/bit definitions
    //mask to check if there is a PBA slave rest in progress and if the PBA Slave Control is busy for PBASLVCTL3
    //if it is not bits 7 and 11 should be set to 0
    const uint64_t PBA_SLVRST_BUSY_IN_PROG_MASK = 0x0110000000000000ull;

//PBA Read Buffer Valid Status field/bit definitions
    const uint64_t PBA_RD_BUF_VALID_MASK = 0x7F000000ull;
    const uint64_t PBA_RD_BUF_EMPTY =  0x1000000ull;
    const uint64_t PBA_RD_BUF_VALID =  0x4000000ull;
    const uint64_t PBA_RD_BUF_VALIDWFP = 0x8000000ull;
    const uint64_t PBA_RD_BUF_CRESPERR = 0x20000000ull;

//PBA Write Buffer Valid Status field/bit definitions
    const uint64_t PBA_WR_BUF_VALID_MASK = 0x1F000000ull;
    const uint64_t PBA_WR_BUF_EMPTY = 0x1000000ull;
    const uint64_t PBA_WR_BUF_CRESPERR = 0x10000000ull;

//PBA BAR register field/bit definitions
    const uint32_t PBA_BAR_BASE_ADDRESS_SHIFT = 20;
    const uint64_t PBA_BAR_BASE_ADDRESS_MASK = 0xFFFFFFFFFull;

//OCB3_ADDRESS field/bit definitions
    const uint32_t OCB3_ADDRESS_REG_ADDR_SHIFT = 32;

    const uint32_t FSI2PIB_RESET_PIB_RESET_BIT = 0;
//PBA CI ttype limited to 8B transfers via OCB indirect-access (others don't go through)
    const uint64_t PBA_CI_TTYPE_ADDR_MASK = 0x7ul;
    //---------------------------------------------------------------------------------
    // Function definitions
    //---------------------------------------------------------------------------------

    fapi2::ReturnCode p10_pba_coherent_utils_check_args(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address, const uint32_t i_flags)
    {
        p10_PBA_oper_flag l_myPbaFlag;
        p10_PBA_oper_flag::OperationType_t l_operType;

        FAPI_DBG("Start");

        // Process input flag
        l_myPbaFlag.getFlag(i_flags);
        l_operType = l_myPbaFlag.getOperationType();

        if (l_operType == p10_PBA_oper_flag::CI
            || l_operType == p10_PBA_oper_flag::ATOMIC)
        {
            //Check the address alignment
            FAPI_ASSERT(!(i_address & PBA_CI_TTYPE_ADDR_MASK),
                        fapi2::P10_PBA_COHERENT_UTILS_INVALID_ARGS().set_TARGET(i_target).set_ADDRESS(
                            i_address),
                        "Address is not 8B aligned for cache-inhibited or atomic access");
        }
        else
        {
            //Check the address alignment
            FAPI_ASSERT(!(i_address & P10_FBC_UTILS_CACHELINE_MASK),
                        fapi2::P10_PBA_COHERENT_UTILS_INVALID_ARGS().set_TARGET(i_target).set_ADDRESS(
                            i_address),
                        "Address is not cacheline aligned");
        }

        //Make sure the address is within the PBA bounds
        FAPI_ASSERT(i_address <= P10_FBC_UTILS_FBC_MAX_ADDRESS,
                    fapi2::P10_PBA_COHERENT_UTILS_INVALID_ARGS().set_TARGET(i_target).set_ADDRESS(
                        i_address),
                    "Address exceeds supported fabric real address range");

    fapi_try_exit:
        FAPI_DBG("End");
        return fapi2::current_err;
    }

    fapi2::ReturnCode p10_pba_coherent_utils_check_fbc_state(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        bool fbc_initialized = false;
        bool fbc_running = false;
        FAPI_DBG("Start");

        //Make sure the fabric is initialized and running
        FAPI_TRY(p10_fbc_utils_get_fbc_state(i_target, fbc_initialized, fbc_running),
                 "Error from p10_fbc_utils_get_fbc_state");
        FAPI_ASSERT(fbc_initialized
                    && fbc_running, fapi2::P10_PBA_FBC_NOT_INITIALIZED_ERR().set_TARGET(i_target).set_INITIALIZED(
                        fbc_initialized).set_RUNNING(
                        fbc_running), "Fabric is not initialized or running");

    fapi_try_exit:
        FAPI_DBG("End");
        return fapi2::current_err;
    }

    fapi2::ReturnCode p10_pba_coherent_utils_get_num_granules(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        uint32_t& o_numGranules)
    {
        using namespace scomt::proc;

        uint64_t oci_address_mask;
        uint64_t maximumAddress;
        //First set up the pba_bar_mask
        fapi2::buffer<uint64_t> pba_bar_mask_data;
        //Set the PBA BAR mask to allow as much of the OCI address to pass through directly as possible
        //by setting bits 23:43 to 0b1.
        uint64_t pba_bar_mask_attr = 0x1FFFFFull;

        FAPI_DBG("Start");

        FAPI_TRY(PREP_TP_TPBR_PBA_PBAO_PBABARMSK3(i_target));
        SET_TP_TPBR_PBA_PBAO_PBABARMSK3_PBABARMSK3_MSK(pba_bar_mask_attr, pba_bar_mask_data);
        FAPI_TRY(PUT_TP_TPBR_PBA_PBAO_PBABARMSK3(i_target, pba_bar_mask_data));

        //maximum size before we need to rerun setup - this is the number if the PBA Bar Mask is set with bits 23:43 to 0b1
        maximumAddress = 0x8000000ull;
        //mask to mask away bits 37:63 of the input address
        oci_address_mask = 0x7FFFFFFull;

        //subtract the oci part of the address from this maximum number
        //then divide by 128 to get the number of 128 bye granules that can be sent
        o_numGranules = (maximumAddress - (i_address & oci_address_mask)) / 128;
        FAPI_DBG("o_numGranules = %016x", o_numGranules);

    fapi_try_exit:
        FAPI_DBG("End");
        return fapi2::current_err;
    }

    fapi2::ReturnCode p10_pba_coherent_setup_pba(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const fapi2::Target<fapi2::TARGET_TYPE_CORE>& i_core_target,
        const uint64_t i_address,
        const bool i_rnw,
        const uint32_t i_flags)
    {
        using namespace scomt::c;
        using namespace scomt::proc;

        uint32_t extaddr;
        uint64_t ocb3_addr_data;
        uint64_t chiplet_number = 0x0ull;
        fapi2::buffer<uint64_t> ocb_status_ctl_data;
        fapi2::buffer<uint64_t> pba_slave_ctl_data;
        fapi2::buffer<uint64_t> l3_mode_reg1;

        p10_PBA_oper_flag l_myPbaFlag;
        p10_PBA_oper_flag::OperationType_t l_operType;
        p10_PBA_oper_flag::AtomicOpType_t l_atomicOp;

        FAPI_DBG("Start");

        // Process input flag
        l_myPbaFlag.getFlag(i_flags);

        // Verify flag is valid
        FAPI_ASSERT(l_myPbaFlag.isFlagValid(),
                    fapi2::P10_PBA_UTILS_INVALID_FLAG()
                    .set_FLAGS(i_flags),
                    "There was an invalid argument passed in when building PBA flag.  Check error trace!");

        l_operType = l_myPbaFlag.getOperationType();
        l_atomicOp = l_myPbaFlag.getAtomicOpType();

        //Write the OCB3 Status Control Register
        //Configure linear stream mode (auto-increment +8 with each data register read/write)
        // Set bits 0:1 to clear PULL_READ_UNDERFLOW and PUSH_WRITE_OVERFLOW
        // Set bit 5 to clear stream type (enables linear mode)
        // Set bits 6:15 to clear other errors
        FAPI_TRY(PREP_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBCSR3_WO_CLEAR(i_target));
        ocb_status_ctl_data.flush<0>();
        SET_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBCSR3_PULL_READ_UNDERFLOW(ocb_status_ctl_data);
        SET_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBCSR3_PUSH_WRITE_OVERFLOW(ocb_status_ctl_data);
        SET_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBCSR3_OCB_STREAM_TYPE(ocb_status_ctl_data);
        SET_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBCSR3_OCB_OCI_TIMEOUT(ocb_status_ctl_data);
        SET_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBCSR3_OCB_OCI_READ_DATA_PARITY(ocb_status_ctl_data);
        SET_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBCSR3_OCB_OCI_SLAVE_ERROR(ocb_status_ctl_data);
        SET_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBCSR3_OCB_PIB_ADDR_PARITY_ERR(ocb_status_ctl_data);
        SET_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBCSR3_OCB_PIB_DATA_PARITY_ERR(ocb_status_ctl_data);
        SET_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBCSR3_SPARE_3(ocb_status_ctl_data);
        FAPI_TRY(PUT_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBCSR3_WO_CLEAR(i_target, ocb_status_ctl_data));

        // Set bit 4 to enable stream mode
        FAPI_TRY(PREP_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBCSR3_WO_OR(i_target));
        ocb_status_ctl_data.flush<0>();
        SET_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBCSR3_OCB_STREAM_MODE(ocb_status_ctl_data);
        FAPI_TRY(PUT_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBCSR3_WO_OR(i_target, ocb_status_ctl_data));

        //Write the address to OCB3_ADDRESS Register
        FAPI_TRY(PREP_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBAR3(i_target));
        ocb3_addr_data = 0xB000000000000000 | ((i_address & 0x7FFFFFFull) << OCB3_ADDRESS_REG_ADDR_SHIFT);
        FAPI_TRY(PUT_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBAR3(i_target, ocb3_addr_data));

        //Write the PBA Slave Control Register that controls the tsize, fastmode, etc
        FAPI_TRY(PREP_TP_TPBR_PBA_PBAO_PBASLVCTL3(i_target));
        //set bit 0 to enable OCI Base Address Range Enabled
        SET_TP_TPBR_PBA_PBAO_PBASLVCTL3_ENABLE(pba_slave_ctl_data);
        //set bits 1:3 to 110 for setting MasterID Match = OCB
        SET_TP_TPBR_PBA_PBAO_PBASLVCTL3_MID_MATCH_VALUE(6, pba_slave_ctl_data);
        //set bits 5:7 to 111 so that MasterID Care Match limits to ONLY the OCB
        SET_TP_TPBR_PBA_PBAO_PBASLVCTL3_MID_CARE_MASK(7, pba_slave_ctl_data);

        //set the write ttype bits 8:10 to whatever is in the flags
        SET_TP_TPBR_PBA_PBAO_PBASLVCTL3_WRITE_TTYPE(l_operType, pba_slave_ctl_data);

        // PBA read_ttype: 0=CL_RD_NC, 1=CI_PR_RD
        if (l_operType == p10_PBA_oper_flag::CI && i_rnw)
        {
            SET_TP_TPBR_PBA_PBAO_PBASLVCTL3_READ_TTYPE(pba_slave_ctl_data);
        }
        else
        {
            CLEAR_TP_TPBR_PBA_PBAO_PBASLVCTL3_READ_TTYPE(pba_slave_ctl_data);
        }

        //set bits 16:17 to No prefetch 01
        SET_TP_TPBR_PBA_PBAO_PBASLVCTL3_READ_PREFETCH_CTL(1, pba_slave_ctl_data);
        //unset bit 18 - no auto-invalidate
        CLEAR_TP_TPBR_PBA_PBAO_PBASLVCTL3_BUF_INVALIDATE_CTL(pba_slave_ctl_data);
        //set bit 19 - write buffer pair allocation bit to 1
        SET_TP_TPBR_PBA_PBAO_PBASLVCTL3_BUF_ALLOC_W(pba_slave_ctl_data);
        //set bit 21 - read buffer pair b allocation bit to 1
        SET_TP_TPBR_PBA_PBAO_PBASLVCTL3_BUF_ALLOC_B(pba_slave_ctl_data);
        //unset bits 20, 22, and 23
        CLEAR_TP_TPBR_PBA_PBAO_PBASLVCTL3_BUF_ALLOC_A(pba_slave_ctl_data);
        CLEAR_TP_TPBR_PBA_PBAO_PBASLVCTL3_BUF_ALLOC_C(pba_slave_ctl_data);
        CLEAR_TP_TPBR_PBA_PBAO_PBASLVCTL3_RESERVED_23(pba_slave_ctl_data);
        //unset bit 24 to allow write gather
        CLEAR_TP_TPBR_PBA_PBAO_PBASLVCTL3_DIS_WRITE_GATHER(pba_slave_ctl_data);
        //set bits 25:27 to 000 for write gather timeout NA
        SET_TP_TPBR_PBA_PBAO_PBASLVCTL3_WR_GATHER_TIMEOUT(0, pba_slave_ctl_data);

        //set bits 28:35 for the tsize to 0 - when this is an lco_m write need
        //to do the chiplet ID of the L3 cache in the form of 00cc_ccc0
        if (l_operType == p10_PBA_oper_flag::LCO && !i_rnw)
        {
            FAPI_TRY(GET_L3_MISC_L3CERRS_MODE_REG1(i_core_target, l3_mode_reg1));
            GET_L3_MISC_L3CERRS_MODE_REG1_MY_LCO_TARGET_ID_CFG(l3_mode_reg1, chiplet_number);

            // Re-prep scomtools for manipulating the PBA register
            PREP_TP_TPBR_PBA_PBAO_PBASLVCTL3(i_target);

            SET_TP_TPBR_PBA_PBAO_PBASLVCTL3_WRITE_TSIZE(chiplet_number << 1, pba_slave_ctl_data);
        }
        else if (l_operType == p10_PBA_oper_flag::ATOMIC && !i_rnw)
        {
            // Use atomic operation specified in flags
            SET_TP_TPBR_PBA_PBAO_PBASLVCTL3_WRITE_TSIZE(l_atomicOp, pba_slave_ctl_data);
        }

        //set bits 36:49 to the ext addr
        extaddr = ((uint32_t) (i_address >> PBA_SLVCTL_EXTADDR_SHIFT)) &
                  PBA_SLVCTL_EXTADDR_MASK;

        SET_TP_TPBR_PBA_PBAO_PBASLVCTL3_EXTADDR(extaddr, pba_slave_ctl_data);

        FAPI_TRY(PUT_TP_TPBR_PBA_PBAO_PBASLVCTL3(i_target, pba_slave_ctl_data));

    fapi_try_exit:
        FAPI_DBG("End");
        return fapi2::current_err;
    }

    fapi2::ReturnCode p10_pba_coherent_setup_pba_bar(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_baseAddress)
    {
        using namespace scomt::proc;

        fapi2::buffer<uint64_t> pba_bar_data;

        FAPI_DBG("Start");

        FAPI_TRY(PREP_TP_TPBR_PBA_PBAO_PBABAR3(i_target));
        //set command scope to local node scope
        SET_TP_TPBR_PBA_PBAO_PBABAR3_CMD_SCOPE(PBA_BAR_SCOPE_LOCAL_NODE, pba_bar_data);

        //set base address bits 8:43
        SET_TP_TPBR_PBA_PBAO_PBABAR3_ADDR((i_baseAddress >> PBA_BAR_BASE_ADDRESS_SHIFT)
                                          & PBA_BAR_BASE_ADDRESS_MASK, pba_bar_data);

        //write the register
        FAPI_TRY(PUT_TP_TPBR_PBA_PBAO_PBABAR3(i_target, pba_bar_data));

    fapi_try_exit:
        FAPI_DBG("End");
        return fapi2::current_err;

    }

    fapi2::ReturnCode p10_pba_coherent_pba_write(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        const uint8_t i_write_data[],
        const p10_PBA_oper_flag::OperationType_t i_ttype)
    {
        using namespace scomt::proc;

        fapi2::ReturnCode rc;
        uint64_t write_data = 0x0ull;
        FAPI_DBG("Start");

        //Perform a 128B write -- need to do 16 8B writes since it's in linear mode which can only do 8B...
        for (int i = 0; i < 16; i++)
        {
            write_data = 0x0ull;

            for (int j = 0; j < 8; j++)
            {
                write_data = write_data + ((uint64_t)(i_write_data[(i * 8) + j]) << (56 - (8 * j)));
            }

            fapi2::buffer<uint64_t> data(write_data);
            rc = fapi2::putScom(i_target, TP_TPCHIP_OCC_OCI_OCB_PIB_OCBDR3, data);

            if (rc)
            {
                FAPI_ERR("Error writing to the PBA via the OCB");
                rc = p10_pba_coherent_error_handling(i_target, rc);
                break;
            }

            // PBA atomic and cache-inhibited operations are limited to 8B
            if (i_ttype == p10_PBA_oper_flag::CI
                || i_ttype == p10_PBA_oper_flag::ATOMIC)
            {
                break;
            }
        }

        FAPI_DBG("End");
        return rc;
    }

    fapi2::ReturnCode p10_pba_coherent_pba_read(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        const p10_PBA_oper_flag::OperationType_t i_ttype,
        uint8_t o_read_data[])
    {
        using namespace scomt::proc;

        fapi2::ReturnCode rc;
        fapi2::buffer<uint64_t> data;

        FAPI_DBG("Start");

        //Perform a 128B read -- need to do 16 8B reads since it's in linear mode which can only do 8B...
        for (int i = 0; i < 16; i++)
        {
            rc = fapi2::getScom(i_target, TP_TPCHIP_OCC_OCI_OCB_PIB_OCBDR3, data);

            if (rc)
            {
                FAPI_ERR("Error reading from the PBA via the OCB");
                rc = p10_pba_coherent_error_handling(i_target, rc);
                break;
            }

            for (int j = 0; j < 8; j++)
            {
                o_read_data[(i * 8) + j] = (data >> (56 - (j * 8))) & 0xFFull;;
            }

            // PBA atomic and cache-inhibited operations are limited to 8B
            if (i_ttype == p10_PBA_oper_flag::CI
                || i_ttype == p10_PBA_oper_flag::ATOMIC)
            {
                break;
            }
        }

        FAPI_DBG("End");
        return rc;
    }

    fapi2::ReturnCode p10_pba_coherent_cleanup_pba(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        using namespace scomt::proc;

        FAPI_DBG("Start");

        fapi2::buffer<uint64_t> data;

        //Clean up the PBA register by resetting PBASLVCTL3 by writing to the PBASLVRST
        FAPI_TRY(PREP_TP_TPBR_PBA_PBAO_PBASLVRST(i_target));

        SET_TP_TPBR_PBA_PBAO_PBASLVRST_SET(7, data);
        FAPI_TRY(PUT_TP_TPBR_PBA_PBAO_PBASLVRST(i_target, data));

        //Wait a little bit and make sure that the reset is no longer in progress
        FAPI_TRY(fapi2::delay(PBA_SLVRST_DELAY_HW_NS, PBA_SLVRST_DELAY_SIM_CYCLES),
                 "Error from PBA Slave Reset delay");

        FAPI_TRY(GET_TP_TPBR_PBA_PBAO_PBASLVRST(i_target, data));

        FAPI_ASSERT(!data.getBit < TP_TPBR_PBA_PBAO_PBASLVRST_IN_PROG + 3 > (),
                    fapi2::P10_PBA_COHERENT_UTILS_RESET_ERR().set_TARGET(i_target).set_RDDATA(
                        data),
                    "Error in resetting the PBA Slave Reset register");

    fapi_try_exit:
        FAPI_DBG("End");
        return fapi2::current_err;
    }

    fapi2::ReturnCode p10_pba_coherent_check_ocb_status(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        using namespace scomt::proc;

        fapi2::ReturnCode rc;
        fapi2::buffer<uint64_t> l_ocb_csr_data;
        bool l_expected_state;

        // read OCB3 Status/Control register
        FAPI_DBG("proc_pba_coherent_utils_check_ocb_status: Reading OCB3 Status/Control register");
        FAPI_TRY(GET_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBCSR3_RO(i_target, l_ocb_csr_data));

        // check for any bits set (outside of status/reserved fields)
        l_expected_state = (!GET_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBCSR3_PULL_READ_UNDERFLOW(l_ocb_csr_data)
                            && !GET_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBCSR3_PUSH_WRITE_OVERFLOW(l_ocb_csr_data)
                            && !GET_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBCSR3_OCB_OCI_TIMEOUT(l_ocb_csr_data)
                            && !GET_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBCSR3_OCB_OCI_READ_DATA_PARITY(l_ocb_csr_data)
                            && !GET_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBCSR3_OCB_OCI_SLAVE_ERROR(l_ocb_csr_data)
                            && !GET_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBCSR3_OCB_PIB_ADDR_PARITY_ERR(l_ocb_csr_data)
                            && !GET_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBCSR3_OCB_PIB_DATA_PARITY_ERR(l_ocb_csr_data)
                            && !GET_TP_TPCHIP_OCC_OCI_OCB_PIB_OCBCSR3_SPARE_3(l_ocb_csr_data));

        FAPI_ASSERT(l_expected_state,
                    fapi2::P10_PBA_COHERENT_UTILS_OCB_STATUS_MISMATCH()
                    .set_TARGET(i_target)
                    .set_DATA(l_ocb_csr_data),
                    "Error in OCB Status/Control Register State");

    fapi_try_exit:
        FAPI_DBG("End");
        return fapi2::current_err;
    }

    fapi2::ReturnCode p10_pba_coherent_check_pba_fir(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        using namespace scomt::proc;

        fapi2::buffer<uint64_t> l_pbaf_fir_data;
        fapi2::buffer<uint64_t> l_pbao_fir_data;
        bool l_expected_state;

        FAPI_TRY(GET_TP_TPBR_PBA_PBAO_PBAFIR_RW(i_target, l_pbao_fir_data));
        l_expected_state = (!GET_TP_TPBR_PBA_PBAO_PBAFIR_OCI_APAR_ERR(l_pbao_fir_data)
                            && !GET_TP_TPBR_PBA_PBAO_PBAFIR_OCI_SLAVE_INIT(l_pbao_fir_data)
                            && !GET_TP_TPBR_PBA_PBAO_PBAFIR_OCI_WRPAR_ERR(l_pbao_fir_data)
                            && !GET_TP_TPBR_PBA_PBAO_PBAFIR_OCI_BAD_REG_ADDR(l_pbao_fir_data));

        FAPI_TRY(GET_TP_TPBR_PBA_PBAF_PBAFIR_RW(i_target, l_pbaf_fir_data));
        l_expected_state = (l_expected_state
                            && !GET_TP_TPBR_PBA_PBAF_PBAFIR_PB_RDADRERR_FW(l_pbaf_fir_data)
                            && !GET_TP_TPBR_PBA_PBAF_PBAFIR_PB_RDDATATO_FW(l_pbaf_fir_data)
                            && !GET_TP_TPBR_PBA_PBAF_PBAFIR_PB_UNEXPCRESP(l_pbaf_fir_data)
                            && !GET_TP_TPBR_PBA_PBAF_PBAFIR_PB_UNEXPDATA(l_pbaf_fir_data)
                            && !GET_TP_TPBR_PBA_PBAF_PBAFIR_PB_PARITY_ERR(l_pbaf_fir_data)
                            && !GET_TP_TPBR_PBA_PBAF_PBAFIR_PB_WRADRERR_FW(l_pbaf_fir_data)
                            && !GET_TP_TPBR_PBA_PBAF_PBAFIR_PB_BADCRESP(l_pbaf_fir_data)
                            && !GET_TP_TPBR_PBA_PBAF_PBAFIR_PB_ACKDEAD_FW_RD(l_pbaf_fir_data)
                            && !GET_TP_TPBR_PBA_PBAF_PBAFIR_INTERNAL_ERR(l_pbaf_fir_data)
                            && !GET_TP_TPBR_PBA_PBAF_PBAFIR_ILLEGAL_CACHE_OP(l_pbaf_fir_data));

        if (GET_TP_TPBR_PBA_PBAF_PBAFIR_PB_RDADRERR_FW(l_pbaf_fir_data)
            || GET_TP_TPBR_PBA_PBAF_PBAFIR_PB_WRADRERR_FW(l_pbaf_fir_data))
        {
            FAPI_ASSERT(l_expected_state,
                        fapi2::P10_PBA_COHERENT_UTILS_PBA_FIR_ERR_ADDR_ERR()
                        .set_TARGET(i_target)
                        .set_DATA(l_pbaf_fir_data),
                        "Error in PBA FIR, with address error");
        }
        else
        {
            FAPI_ASSERT(l_expected_state,
                        fapi2::P10_PBA_COHERENT_UTILS_PBA_FIR_ERR_NO_ADDR_ERR()
                        .set_TARGET(i_target)
                        .set_DATA(l_pbaf_fir_data),
                        "Error in PBA FIR, without address error");
        }

    fapi_try_exit:
        FAPI_DBG("End");
        return fapi2::current_err;
    }

    fapi2::ReturnCode p10_pba_coherent_check_status(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&
            i_target)
    {
        FAPI_TRY(p10_pba_coherent_check_pba_fir(i_target), "Error from p10_pba_coherent_check_pba_fir");
        FAPI_TRY(p10_pba_coherent_check_ocb_status(i_target), "Error from p10_pba_coherent_check_ocb_status");

    fapi_try_exit:
        FAPI_DBG("End");
        return fapi2::current_err;
    }

    void p10_pba_coherent_append_input_data(const uint64_t i_address, const bool i_rnw, const uint32_t i_flags,
                                            fapi2::ReturnCode& o_rc)
    {
#ifndef __PPE__
        uint64_t l_address = i_address;
        bool l_rnw = i_rnw;
        uint32_t l_flags = i_flags;
        fapi2::ffdc_t ADDRESS;
        fapi2::ffdc_t RNW;
        fapi2::ffdc_t FLAGS;
        ADDRESS.ptr() = static_cast<void*>(&l_address);
        ADDRESS.size() = sizeof(l_address);
        RNW.ptr() = static_cast<void*>(&l_rnw);
        RNW.size() = sizeof(l_rnw);
        FLAGS.ptr() = static_cast<void*>(&l_flags);
        FLAGS.size() = sizeof(l_flags);

        if ((o_rc == (fapi2::ReturnCode) fapi2::RC_P10_PBA_COHERENT_UTILS_INVALID_ARGS)
            || (o_rc == (fapi2::ReturnCode) fapi2::RC_P10_PBA_COHERENT_UTILS_RESET_ERR)
            || (o_rc == (fapi2::ReturnCode) fapi2::RC_P10_PBA_FBC_NOT_INITIALIZED_ERR)
            || (o_rc == (fapi2::ReturnCode) fapi2::RC_P10_PBA_COHERENT_UTILS_OCB_STATUS_MISMATCH)
            || (o_rc == (fapi2::ReturnCode) fapi2::RC_P10_PBA_COHERENT_UTILS_PBA_FIR_ERR_ADDR_ERR)
            || (o_rc == (fapi2::ReturnCode) fapi2::RC_P10_PBA_COHERENT_UTILS_PBA_FIR_ERR_NO_ADDR_ERR))
        {
            FAPI_ADD_INFO_TO_HWP_ERROR(o_rc, RC_P10_PBA_COHERENT_UTILS_EXTRA_INPUT_DATA);
        }

#endif
    }

#ifndef __PPE__
    fapi2::ReturnCode p10_pba_utils_unlock_pib(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        using namespace scomt::perv;
        using namespace scomt::proc;

        fapi2::ReturnCode rc;
        fapi2::buffer<uint32_t> l_cfam_data;
        fapi2::buffer<uint64_t> l_pba_slv_rst_data;

        FAPI_DBG("Start");
        // unlock PIB in case of HW229314
        FAPI_DBG("Checking FSI2PIB Status Register");
        rc = fapi2::getCfamRegister(i_target, TP_TPVSB_FSI_W_FSI2PIB_STATUS_FSI, l_cfam_data);

        if (rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("getCfamRegister error");
            return rc;
        }

        if (l_cfam_data.getBit(TP_TPVSB_FSI_W_FSI2PIB_STATUS_PIB_ABORT))
        {
            FAPI_DBG("Performing PIB reset");

            // reset PIB/OCB
            l_cfam_data.flush<0>();
            l_cfam_data.setBit(FSI2PIB_RESET_PIB_RESET_BIT);
            const fapi2::buffer<uint32_t> l_const_cfam_data = l_cfam_data;
            rc = fapi2::putCfamRegister(i_target, TP_TPVSB_FSI_W_FSI2PIB_RESET_FSI, l_const_cfam_data);

            if (rc != fapi2::FAPI2_RC_SUCCESS)
            {
                FAPI_ERR("Error resetting PIB/OCB");
                return rc;
            }

            // ensure PBA region is unlocked, discard/ignore return code
            (void) fapi2::getScom(i_target, TP_TPBR_PBA_PBAO_PBASLVRST, l_pba_slv_rst_data);

            rc = fapi2::putCfamRegister(i_target, TP_TPVSB_FSI_W_FSI2PIB_RESET_FSI, l_const_cfam_data);

            if (rc != fapi2::FAPI2_RC_SUCCESS)
            {
                FAPI_ERR("Error ensuring PBA region is unlocked");
                return rc;
            }
        }
        else
        {
            // ensure PBA region is unlocked, discard/ignore return code
            (void) fapi2::getScom(i_target, TP_TPBR_PBA_PBAO_PBASLVRST, l_pba_slv_rst_data);
        }

        return rc;
    }
#endif

    fapi2::ReturnCode p10_pba_coherent_error_handling(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        fapi2::ReturnCode i_rc)
    {
        fapi2::ReturnCode l_return_rc;
        FAPI_DBG("Start");

        // analyze failure, attempt to differentiate between SCOM failure due to faulty HW
        // versus failure to return read data, which under some conditions can cause SCOM to fail as well
#ifndef __PPE__
        // ensure that PIB abort condition (which may have occurred as a result of SCOM read
        // failure) is cleared, so that analysis of HW state is possible
        l_return_rc = p10_pba_utils_unlock_pib(i_target);

        if (l_return_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            // if the chip is in a functional state, accesses to the cfam region
            // should always succeed -- if a fail occurs, return the original return code
            // (as this represents the first error encountered, and the analysis
            // indicates a HW issue which the PBA operation did not functionally cause)
            FAPI_ERR("Error from p10_pba_utils_unlock_pib, returning original SCOM fail rc");
            FAPI_DBG("End");
            return i_rc;
        }

#else

        // PPE platform expected to clear/handle PIB abort condition, check for
        // timeout return code to indicate that we should continue analysis
        if (i_rc != (fapi2::ReturnCode) fapi2::RC_SBE_PIB_TIMEOUT_ERROR)
        {
            FAPI_ERR("Returning original SCOM fail rc");
            FAPI_DBG("End");
            return i_rc;
        }

        // ensure PBA region is unlocked, discard/ignore return code
        fapi2::buffer<uint64_t> l_pba_slv_rst_data;
        (void) fapi2::getScom(i_target, scomt::proc::TP_TPBR_PBA_PBAO_PBASLVRST, l_pba_slv_rst_data);
#endif

        // PIB is unlocked, analyze state of FIRs/state machines to see if the SCOM
        // failure was a result of a read failure on the fabric launched by PBA
        l_return_rc = p10_pba_coherent_check_status(i_target);

        if (l_return_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            // check for return codes that this routine can emit which indicate
            // a functional failure in PBA that could result in the SCOM failure as a side-effect
            if ((l_return_rc == (fapi2::ReturnCode) fapi2::RC_P10_PBA_COHERENT_UTILS_PBA_FIR_ERR_ADDR_ERR) ||
                (l_return_rc == (fapi2::ReturnCode) fapi2::RC_P10_PBA_COHERENT_UTILS_PBA_FIR_ERR_NO_ADDR_ERR) ||
                (l_return_rc == (fapi2::ReturnCode) fapi2::RC_P10_PBA_COHERENT_UTILS_OCB_STATUS_MISMATCH))
            {
                FAPI_ERR("Error from p10_pba_coherent_check_status, returning PBA rc");
            }
            // none of these match, return the original return code as it was the first error
            else
            {
                FAPI_ERR("Error from p10_pba_coherent_check_status, returning original SCOM fail rc");
                l_return_rc = i_rc;
            }

            FAPI_DBG("End");
            return l_return_rc;
        }

        // no sign of an error in PBA/OCB logic, just return the original return code for the SCOM fail
        FAPI_ERR("No PBA error found, returning original SCOM fail rc");
        FAPI_DBG("End");
        return i_rc;
    }

} //extern "C"
