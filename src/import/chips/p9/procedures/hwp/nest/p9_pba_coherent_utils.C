/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/nest/p9_pba_coherent_utils.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2017                        */
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
/// @file p9_pba_coherent_utils.C
/// @brief PBA alter/display library functions (FAPI)
///
// *HWP HWP Owner: Joe McGill jmcgill@us.ibm.com
// *HWP FW Owner: Thi Tran thi@us.ibm.com
// *HWP Team: Nest
// *HWP Level: 3
// *HWP Consumed by: SBE
//
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------
#include <p9_pba_coherent_utils.H>
#include <p9_misc_scom_addresses.H>
#include <p9_quad_scom_addresses.H>
#include <p9_misc_scom_addresses_fld.H>
#include <p9_perv_scom_addresses_fld.H>
#include <p9_perv_scom_addresses.H>
#include <p9_fbc_utils.H>

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
    //---------------------------------------------------------------------------------
    // Function definitions
    //---------------------------------------------------------------------------------

    fapi2::ReturnCode p9_pba_coherent_utils_check_args(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address)
    {

        FAPI_DBG("Start");

        //Check the address alignment
        FAPI_ASSERT(!(i_address & P9_FBC_UTILS_CACHELINE_MASK),
                    fapi2::P9_PBA_COHERENT_UTILS_INVALID_ARGS().set_TARGET(i_target).set_ADDRESS(
                        i_address),
                    "Address is not cacheline aligned");

        //Make sure the address is within the PBA bounds
        FAPI_ASSERT(i_address <= P9_FBC_UTILS_FBC_MAX_ADDRESS,
                    fapi2::P9_PBA_COHERENT_UTILS_INVALID_ARGS().set_TARGET(i_target).set_ADDRESS(
                        i_address),
                    "Address exceeds supported fabric real address range");

    fapi_try_exit:
        FAPI_DBG("End");
        return fapi2::current_err;
    }

    fapi2::ReturnCode p9_pba_coherent_utils_check_fbc_state(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        bool fbc_initialized = false;
        bool fbc_running = false;
        FAPI_DBG("Start");

        //Make sure the fabric is initialized and running
        FAPI_TRY(p9_fbc_utils_get_fbc_state(i_target, fbc_initialized, fbc_running),
                 "Error from p9_fbc_utils_get_fbc_state");
        FAPI_ASSERT(fbc_initialized
                    && fbc_running, fapi2::P9_PBA_FBC_NOT_INITIALIZED_ERR().set_TARGET(i_target).set_INITIALIZED(
                        fbc_initialized).set_RUNNING(
                        fbc_running), "Fabric is not initialized or running");

    fapi_try_exit:
        FAPI_DBG("End");
        return fapi2::current_err;
    }

    fapi2::ReturnCode p9_pba_coherent_utils_get_num_granules(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        uint32_t& o_numGranules)
    {
        uint64_t oci_address_mask;
        uint64_t maximumAddress;
        //First set up the pba_bar_mask
        fapi2::buffer<uint64_t> pba_bar_mask_data;
        //Set the PBA BAR mask to allow as much of the OCI address to pass through directly as possible
        //by setting bits 23:43 to 0b1.
        uint64_t pba_bar_mask_attr = 0x1FFFFF00000ull;

        FAPI_DBG("Start");

        pba_bar_mask_data.insertFromRight<0, 64>(pba_bar_mask_attr);

        //write the PBA Bar Mask Register
        FAPI_TRY(fapi2::putScom(i_target, PU_PBABARMSK3, pba_bar_mask_data),
                 "Error writing to the PBA Bar Mask Attribute");


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

    fapi2::ReturnCode p9_pba_coherent_setup_pba(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const fapi2::Target<fapi2::TARGET_TYPE_EX>& i_ex_target,
        const uint64_t i_address,
        const bool i_rnw,
        const uint32_t i_flags)
    {
        uint32_t extaddr;
        uint64_t ocb3_addr_data;
        uint64_t chiplet_number = 0x0ull;
        fapi2::buffer<uint64_t> ocb_status_ctl_data;
        fapi2::buffer<uint64_t> ocb3_addr;
        fapi2::buffer<uint64_t> pba_slave_ctl_data;
        fapi2::buffer<uint64_t> l3_mode_reg1;

        p9_PBA_oper_flag l_myPbaFlag;
        p9_PBA_oper_flag::OperationType_t l_operType;

        FAPI_DBG("Start");

        // Process input flag
        l_myPbaFlag.getFlag(i_flags);
        l_operType = l_myPbaFlag.getOperationType();

        //Write the OCB3 Status Control Register
        //Configure linear stream mode (auto-increment +8 with each data register read/write)
        // Set bits 0:1 to clear PULL_READ_UNDERFLOW and PUSH_WRITE_OVERFLOW
        // Set bit 5 to clear stream type (enables linear mode)
        // Set bits 6:15 to clear other errors
        ocb_status_ctl_data.flush<0>().setBit<0, 2>().setBit<5, 11>();
        FAPI_TRY(fapi2::putScom(i_target, PU_OCB_PIB_OCBCSR3_CLEAR,
                                ocb_status_ctl_data),
                 "Error writing to the OCB3 Status Control Register with and mask");
        // Set bit 4 to enable stream mode
        ocb_status_ctl_data.flush<0>().setBit<4>();
        FAPI_TRY(fapi2::putScom(i_target, PU_OCB_PIB_OCBCSR3_OR,
                                ocb_status_ctl_data),
                 "Error writing to the OCB3 Status Control Register with or mask");

        //Write the address to OCB3_ADDRESS Register
        ocb3_addr_data = 0xB000000000000000 | ((i_address & 0x7FFFFFFull) << OCB3_ADDRESS_REG_ADDR_SHIFT);
        ocb3_addr.insertFromRight<0, 64>(ocb3_addr_data);

        FAPI_TRY(fapi2::putScom(i_target, PU_OCB_PIB_OCBAR3, ocb3_addr),
                 "Error writing the OCB3_ADDRESS Register");

        //Write the PBA Slave Control Register that controls the tsize, fastmode, etc
        //set bit 0 to enable OCI Base Address Range Enabled
        pba_slave_ctl_data.setBit<PU_PBASLVCTL3_ENABLE>();
        //set bits 1:3 to 110 for setting MasterID Match = OCB
        pba_slave_ctl_data.insertFromRight < PU_PBASLVCTL3_MID_MATCH_VALUE, PU_PBASLVCTL3_MID_MATCH_VALUE_LEN >(6);
        //set bits 5:7 to 111 so that MasterID Care Match limits to ONLY the OCB
        pba_slave_ctl_data.insertFromRight < PU_PBASLVCTL3_MID_CARE_MASK, PU_PBASLVCTL3_MID_CARE_MASK_LEN > (7);

        //set the write ttype bits 8:10 to whatever is in the flags
        pba_slave_ctl_data.insertFromRight < PU_PBASLVCTL3_WRITE_TTYPE, PU_PBASLVCTL3_WRITE_TTYPE_LEN > (l_operType);

        //it's not cache-inhibited so set bit 15 to cl_rd_nc (0)
        pba_slave_ctl_data.clearBit<PU_PBASLVCTL3_READ_TTYPE>();
        //set bits 16:17 to No prefetch 01 TODO May need to change this later if we want to use prefetch
        pba_slave_ctl_data.insertFromRight < PU_PBASLVCTL3_READ_PREFETCH_CTL, PU_PBASLVCTL3_READ_PREFETCH_CTL_LEN > (1);
        //unset bit 18 - no auto-invalidate
        pba_slave_ctl_data.clearBit<PU_PBASLVCTL3_BUF_INVALIDATE_CTL>();
        //set bit 19 - write buffer pair allocation bit to 1
        pba_slave_ctl_data.setBit<PU_PBASLVCTL3_BUF_ALLOC_W>();
        //set bit 21 - read buffer pair b allocation bit to 1
        pba_slave_ctl_data.setBit<PU_PBASLVCTL3_BUF_ALLOC_B>();
        //unset bits 20, 22, and 23
        pba_slave_ctl_data.clearBit<PU_PBASLVCTL3_BUF_ALLOC_A>().clearBit<PU_PBASLVCTL3_BUF_ALLOC_C>().clearBit<PU_PBASLVCTL3_RESERVED_23>();
        //unset bit 24 to allow write gather
        pba_slave_ctl_data.clearBit<PU_PBASLVCTL3_DIS_WRITE_GATHER>();
        //set bits 25:27 to 000 for write gather timeout NA
        pba_slave_ctl_data.insertFromRight < PU_PBASLVCTL3_WR_GATHER_TIMEOUT, PU_PBASLVCTL3_WR_GATHER_TIMEOUT_LEN > (0);

        //set bits 28:35 for the tsize to 0 - when this is an lco_m write need to do the chiplet ID of the L3 cache in the form of 00cc_ccc0
        if (l_operType == p9_PBA_oper_flag::LCO && !i_rnw)
        {
            FAPI_TRY(fapi2::getScom(i_ex_target, EX_L3_MODE_REG1, l3_mode_reg1), "Error reading from the L3 Mode Register");
            l3_mode_reg1.extractToRight(chiplet_number, 2, 4);
        }

        pba_slave_ctl_data.insertFromRight < PU_PBASLVCTL3_WRITE_TSIZE, PU_PBASLVCTL3_WRITE_TSIZE_LEN > (chiplet_number << 1);
        //set bits 36:49 to the ext addr
        extaddr = ((uint32_t) (i_address >> PBA_SLVCTL_EXTADDR_SHIFT)) &
                  PBA_SLVCTL_EXTADDR_MASK;

        pba_slave_ctl_data.insertFromRight < PU_PBASLVCTL3_EXTADDR, PU_PBASLVCTL3_EXTADDR_LEN > (extaddr);

        FAPI_TRY(fapi2::putScom(i_target, PU_PBASLVCTL3_SCOM, pba_slave_ctl_data),
                 "Error writing the PBA Slave Control Register");

    fapi_try_exit:
        FAPI_DBG("End");
        return fapi2::current_err;
    }

    fapi2::ReturnCode p9_pba_coherent_setup_pba_bar(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_baseAddress)
    {
        fapi2::buffer<uint64_t> pba_bar_data;

        FAPI_DBG("Start");

        //Validate the input parameters
        //Check the address alignment
        FAPI_ASSERT(!(i_baseAddress & P9_FBC_UTILS_CACHELINE_MASK),
                    fapi2::P9_PBA_COHERENT_UTILS_INVALID_ARGS().set_TARGET(i_target).set_ADDRESS(
                        i_baseAddress),
                    "Base Address is not cacheline aligned");
        //Make sure the address is within the PBA bounds
        FAPI_ASSERT(i_baseAddress <= P9_FBC_UTILS_FBC_MAX_ADDRESS,
                    fapi2::P9_PBA_COHERENT_UTILS_INVALID_ARGS().set_TARGET(i_target).set_ADDRESS(
                        i_baseAddress),
                    "Base Address exceeds supported fabric real address range");

        //set command scope to local node scope
        pba_bar_data.insertFromRight < PU_PBABAR0_CMD_SCOPE, PU_PBABAR0_CMD_SCOPE_LEN >
        (PBA_BAR_SCOPE_LOCAL_NODE);

        //set base address bits 8:43
        pba_bar_data.insertFromRight < PU_PBABAR0_ADDR, PU_PBABAR0_ADDR_LEN > ((
                    i_baseAddress >> PBA_BAR_BASE_ADDRESS_SHIFT) & PBA_BAR_BASE_ADDRESS_MASK);

        //write the register
        FAPI_TRY(fapi2::putScom(i_target, PU_PBABAR3, pba_bar_data),
                 "Error writing the PBA Bar Register");

    fapi_try_exit:
        FAPI_DBG("End");
        return fapi2::current_err;

    }

    fapi2::ReturnCode p9_pba_coherent_pba_write(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        const uint8_t i_write_data[])
    {
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
            rc = fapi2::putScom(i_target, PU_OCB_PIB_OCBDR3, data);

            if (rc)
            {
                FAPI_ERR("Error writing to the PBA via the OCB");
                rc = p9_pba_coherent_error_handling(i_target, rc);
                break;
            }
        }

        FAPI_DBG("End");
        return rc;
    }

    fapi2::ReturnCode p9_pba_coherent_pba_read(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        uint8_t o_read_data[])
    {
        fapi2::ReturnCode rc;
        fapi2::buffer<uint64_t> data;

        FAPI_DBG("Start");

        //Perform a 128B read -- need to do 16 8B reads since it's in linear mode which can only do 8B...
        for (int i = 0; i < 16; i++)
        {
            rc = fapi2::getScom(i_target, PU_OCB_PIB_OCBDR3, data);

            if (rc)
            {
                FAPI_ERR("Error reading from the PBA via the OCB");
                rc = p9_pba_coherent_error_handling(i_target, rc);
                break;
            }

            for (int j = 0; j < 8; j++)
            {
                o_read_data[(i * 8) + j] = (data >> (56 - (j * 8))) & 0xFFull;;
            }
        }

        FAPI_DBG("End");
        return rc;
    }

    fapi2::ReturnCode p9_pba_coherent_cleanup_pba(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_DBG("Start");

        fapi2::buffer<uint64_t> data;

        //Clean up the PBA register by resetting PBASLVCTL3 by writing to the PBASLVRST
        data.insertFromRight < PU_PBASLVRST_SET, PU_PBASLVRST_SET_LEN > (7);
        FAPI_TRY(fapi2::putScom(i_target, PU_PBASLVRST_SCOM, data),
                 "Error writing to the PBA Slave Reset register");

        //Wait a little bit and make sure that the reset is no longer in progress
        FAPI_TRY(fapi2::delay(PBA_SLVRST_DELAY_HW_NS, PBA_SLVRST_DELAY_SIM_CYCLES),
                 "Error from PBA Slave Reset delay");

        FAPI_TRY(fapi2::getScom(i_target, PU_PBASLVRST_SCOM, data),
                 "Error reading from the PBA Slave Reset register");

        FAPI_ASSERT(!data.getBit < PU_PBASLVRST_IN_PROG + 3 > (),
                    fapi2::P9_PBA_COHERENT_UTILS_RESET_ERR().set_TARGET(i_target).set_RDDATA(
                        data),
                    "Error in resetting the PBA Slave Reset register");

    fapi_try_exit:
        FAPI_DBG("End");
        return fapi2::current_err;
    }

    fapi2::ReturnCode p9_pba_coherent_check_ocb_status(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        fapi2::ReturnCode rc;
        fapi2::buffer<uint64_t> l_ocb_csr_data;
        bool l_expected_state;

        // read OCB3 Status/Control register
        FAPI_DBG("proc_pba_coherent_utils_check_ocb_status: Reading OCB3 Status/Control register");
        FAPI_TRY(fapi2::getScom(i_target, PU_OCB_PIB_OCBCSR3_RO, l_ocb_csr_data),
                 "Error reading from OCB Control/Status Register");

        // check for any bits set (outside of status/reserved fields)
        l_expected_state =
            !l_ocb_csr_data.getBit<PU_OCB_PIB_OCBCSR3_PULL_READ_UNDERFLOW>() &&
            !l_ocb_csr_data.getBit<PU_OCB_PIB_OCBCSR3_PUSH_WRITE_OVERFLOW>() &&
            !l_ocb_csr_data.getBit<PU_OCB_PIB_OCBCSR3_OCI_TIMEOUT>() &&
            !l_ocb_csr_data.getBit<PU_OCB_PIB_OCBCSR3_OCI_READ_DATA_PARITY>() &&
            !l_ocb_csr_data.getBit<PU_OCB_PIB_OCBCSR3_OCI_SLAVE_ERROR>() &&
            !l_ocb_csr_data.getBit<PU_OCB_PIB_OCBCSR3_ADDR_PARITY_ERR>() &&
            !l_ocb_csr_data.getBit<PU_OCB_PIB_OCBCSR3_DATA_PARITY_ERR>() &&
            !l_ocb_csr_data.getBit<PU_OCB_PIB_OCBCSR3_FSM_ERR>();

        FAPI_ASSERT(l_expected_state,
                    fapi2::P9_PBA_COHERENT_UTILS_OCB_STATUS_MISMATCH()
                    .set_TARGET(i_target)
                    .set_DATA(l_ocb_csr_data),
                    "Error in OCB Status/Control Register State");

    fapi_try_exit:
        FAPI_DBG("End");
        return fapi2::current_err;
    }

    fapi2::ReturnCode p9_pba_coherent_check_pba_fir(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        fapi2::buffer<uint64_t> l_pba_fir_data;
        bool l_expected_state;

        // read PBA FIR register
        FAPI_DBG("proc_pba_coherent_utils_check_pba_fir: Reading PBA FIR register");
        FAPI_TRY(fapi2::getScom(i_target, PU_PBAFIR, l_pba_fir_data), "Error reading PBA Fir register");

        // check for unexpected state
        l_expected_state =
            !l_pba_fir_data.getBit<PU_PBAFIR_OCI_APAR_ERR>() &&
            !l_pba_fir_data.getBit<PU_PBAFIR_PB_RDADRERR_FW>() &&
            !l_pba_fir_data.getBit<PU_PBAFIR_PB_RDDATATO_FW>() &&
            !l_pba_fir_data.getBit<PU_PBAFIR_OCI_SLAVE_INIT>() &&
            !l_pba_fir_data.getBit<PU_PBAFIR_OCI_WRPAR_ERR>() &&
            //l_pba_fir_data.getBit<PBA_FIR_OCI_REREQTO_BIT>() &&
            !l_pba_fir_data.getBit<PU_PBAFIR_PB_UNEXPCRESP>() &&
            !l_pba_fir_data.getBit<PU_PBAFIR_PB_UNEXPDATA>() &&
            !l_pba_fir_data.getBit<PU_PBAFIR_PB_PARITY_ERR>() &&
            !l_pba_fir_data.getBit<PU_PBAFIR_PB_WRADRERR_FW>() &&
            !l_pba_fir_data.getBit<PU_PBAFIR_PB_BADCRESP>() &&
            !l_pba_fir_data.getBit<PU_PBAFIR_PB_ACKDEAD_FW_RD>() &&
            !l_pba_fir_data.getBit<PU_PBAFIR_INTERNAL_ERR>() &&
            !l_pba_fir_data.getBit<PU_PBAFIR_ILLEGAL_CACHE_OP>() &&
            !l_pba_fir_data.getBit<PU_PBAFIR_OCI_BAD_REG_ADDR>();

        if (l_pba_fir_data.getBit<PU_PBAFIR_PB_RDADRERR_FW>() || l_pba_fir_data.getBit<PU_PBAFIR_PB_WRADRERR_FW>())
        {
            FAPI_ASSERT(l_expected_state,
                        fapi2::P9_PBA_COHERENT_UTILS_PBA_FIR_ERR_ADDR_ERR()
                        .set_TARGET(i_target)
                        .set_DATA(l_pba_fir_data),
                        "Error in PBA FIR, with address error");
        }
        else
        {
            FAPI_ASSERT(l_expected_state,
                        fapi2::P9_PBA_COHERENT_UTILS_PBA_FIR_ERR_NO_ADDR_ERR()
                        .set_TARGET(i_target)
                        .set_DATA(l_pba_fir_data),
                        "Error in PBA FIR, without address error");
        }

    fapi_try_exit:
        FAPI_DBG("End");
        return fapi2::current_err;

    }

    fapi2::ReturnCode p9_pba_coherent_check_status(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&
            i_target)
    {
        FAPI_TRY(p9_pba_coherent_check_pba_fir(i_target), "Error from p9_pba_coherent_check_pba_fir");
        FAPI_TRY(p9_pba_coherent_check_ocb_status(i_target), "Error from p9_pba_coherent_check_ocb_status");

    fapi_try_exit:
        FAPI_DBG("End");
        return fapi2::current_err;
    }

    void p9_pba_coherent_append_input_data(const uint64_t i_address, const bool i_rnw, const uint32_t i_flags,
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

        if ((o_rc == (fapi2::ReturnCode) fapi2::RC_P9_PBA_COHERENT_UTILS_INVALID_ARGS)
            || (o_rc == (fapi2::ReturnCode) fapi2::RC_P9_PBA_COHERENT_UTILS_RESET_ERR)
            || (o_rc == (fapi2::ReturnCode) fapi2::RC_P9_PBA_FBC_NOT_INITIALIZED_ERR)
            || (o_rc == (fapi2::ReturnCode) fapi2::RC_P9_PBA_COHERENT_UTILS_OCB_STATUS_MISMATCH)
            || (o_rc == (fapi2::ReturnCode) fapi2::RC_P9_PBA_COHERENT_UTILS_PBA_FIR_ERR_ADDR_ERR)
            || (o_rc == (fapi2::ReturnCode) fapi2::RC_P9_PBA_COHERENT_UTILS_PBA_FIR_ERR_NO_ADDR_ERR))
        {
            FAPI_ADD_INFO_TO_HWP_ERROR(o_rc, RC_P9_PBA_COHERENT_UTILS_EXTRA_INPUT_DATA);
        }

#endif
    }

#ifndef __PPE__
    fapi2::ReturnCode p9_pba_utils_unlock_pib(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        fapi2::ReturnCode rc;
        fapi2::buffer<uint32_t> l_cfam_data;
        fapi2::buffer<uint64_t> l_pba_slv_rst_data;

        FAPI_DBG("Start");
        // unlock PIB in case of HW229314
        FAPI_DBG("Checking FSI2PIB Status Register");
        rc = fapi2::getCfamRegister(i_target, PERV_FSI2PIB_STATUS_FSI, l_cfam_data);

        if (rc != fapi2::FAPI2_RC_SUCCESS)
        {
            FAPI_ERR("getCfamRegister error");
            return rc;
        }

        if (l_cfam_data.getBit(PERV_FSI2PIB_STATUS_PIB_ABORT))
        {
            FAPI_DBG("Performing PIB reset");

            // reset PIB/OCB
            l_cfam_data.flush<0>();
            l_cfam_data.setBit(FSI2PIB_RESET_PIB_RESET_BIT);
            const fapi2::buffer<uint32_t> l_const_cfam_data = l_cfam_data;
            rc = fapi2::putCfamRegister(i_target, PERV_FSI2PIB_RESET_FSI, l_const_cfam_data);

            if (rc != fapi2::FAPI2_RC_SUCCESS)
            {
                FAPI_ERR("Error resetting PIB/OCB");
                return rc;
            }

            // ensure PBA region is unlocked, discard/ignore return code
            (void) fapi2::getScom(i_target, PU_PBASLVRST_PIB, l_pba_slv_rst_data);

            rc = fapi2::putCfamRegister(i_target, PERV_FSI2PIB_RESET_FSI, l_const_cfam_data);

            if (rc != fapi2::FAPI2_RC_SUCCESS)
            {
                FAPI_ERR("Error ensuring PBA region is unlocked");
                return rc;
            }
        }
        else
        {
            // ensure PBA region is unlocked, discard/ignore return code
            (void) fapi2::getScom(i_target, PU_PBASLVRST_PIB, l_pba_slv_rst_data);
        }

        return rc;
    }
#endif

    fapi2::ReturnCode p9_pba_coherent_error_handling(
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
        l_return_rc = p9_pba_utils_unlock_pib(i_target);

        if (l_return_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            // if the chip is in a functional state, accesses to the cfam region
            // should always succeed -- if a fail occurs, return the original return code
            // (as this represents the first error encountered, and the analysis
            // indicates a HW issue which the PBA operation did not functionally cause)
            FAPI_ERR("Error from p9_pba_utils_unlock_pib, returning original SCOM fail rc");
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
        (void) fapi2::getScom(i_target, PU_PBASLVRST_PIB, l_pba_slv_rst_data);

#endif

        // PIB is unlocked, analyze state of FIRs/state machines to see if the SCOM
        // failure was a result of a read failure on the fabric launched by PBA
        l_return_rc = p9_pba_coherent_check_status(i_target);

        if (l_return_rc != fapi2::FAPI2_RC_SUCCESS)
        {
            // check for return codes that this routine can emit which indicate
            // a functional failure in PBA that could result in the SCOM failure as a side-effect
            if ((l_return_rc == (fapi2::ReturnCode) fapi2::RC_P9_PBA_COHERENT_UTILS_PBA_FIR_ERR_ADDR_ERR) ||
                (l_return_rc == (fapi2::ReturnCode) fapi2::RC_P9_PBA_COHERENT_UTILS_PBA_FIR_ERR_NO_ADDR_ERR) ||
                (l_return_rc == (fapi2::ReturnCode) fapi2::RC_P9_PBA_COHERENT_UTILS_OCB_STATUS_MISMATCH))
            {
                FAPI_ERR("Error from p9_pba_coherent_check_status, returning PBA rc");
            }
            // none of these match, return the original return code as it was the first error
            else
            {
                FAPI_ERR("Error from p9_pba_coherent_check_status, returning original SCOM fail rc");
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
