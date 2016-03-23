/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: chips/p9/procedures/hwp/nest/p9_pba_coherent_utils.C $        */
/*                                                                        */
/* IBM CONFIDENTIAL                                                       */
/*                                                                        */
/* EKB Project                                                            */
/*                                                                        */
/* COPYRIGHT 2015,2016                                                    */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* The source code for this program is not published or otherwise         */
/* divested of its trade secrets, irrespective of what has been           */
/* deposited with the U.S. Copyright Office.                              */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
//-----------------------------------------------------------------------------------
//
/// @file p9_pba_coherent_utils.C
/// @brief PBA alter/display library functions (FAPI)
///
// *HWP HWP Owner: Christina Graves clgraves@us.ibm.com
// *HWP FW Owner: Thi Tran thi@us.ibm.com
// *HWP Team: Nest
// *HWP Level: 2
// *HWP Consumed by: SBE
//
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------
#include <p9_pba_coherent_utils.H>
#include <p9_misc_scom_addresses.H>
#include <p9_quad_scom_addresses.H>
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
    const uint32_t PBA_SLVCTL_ENABLE_BIT = 0;
    const uint32_t PBA_SLVCTL_MASTER_ID_MATCH_START_BIT = 1;
    const uint32_t PBA_SLVCTL_MASTER_ID_MATCH_END_BIT = 3;
    const uint32_t PBA_SLVCTL_MASTER_ID_CARE_MASK_START_BIT = 5;
    const uint32_t PBA_SLVCTL_MASTER_ID_CARE_MASK_END_BIT = 7;
    const uint32_t PBA_SLVCTL_WRITE_TTYPE_START_BIT = 8;
    const uint32_t PBA_SLVCTL_WRITE_TTYPE_END_BIT = 10;
    const uint32_t PBA_SLVCTL_READ_TTYPE_BIT = 15;
    const uint32_t PBA_SLVCTL_READ_PREFETCH_CTL_START_BIT = 16;
    const uint32_t PBA_SLVCTL_READ_PREFETCH_CTL_END_BIT = 17;
    const uint32_t PBA_SLVCTL_READ_BUF_INVALIDATE_CTL_BIT = 18;
    const uint32_t PBA_SLVCTL_WRITE_BUF_PAIR_ALLOCATION_BIT = 19;
    const uint32_t PBA_SLVCTL_READ_BUF_PAIR_A_ALLOCATION_BIT = 20;
    const uint32_t PBA_SLVCTL_READ_BUF_PAIR_B_ALLOCATION_BIT = 21;
    const uint32_t PBA_SLVCTL_READ_BUF_PAIR_C_ALLOCATION_BIT = 22;
    const uint32_t PBA_SLVCTL_DISABLE_WRITE_GATHER_BIT = 24;
    const uint32_t PBA_SLVCTL_WRITE_GATHER_TIMEOUT_START_BIT = 25;
    const uint32_t PBA_SLVCTL_WRITE_GATHER_TIMEOUT_END_BIT = 27;
    const uint32_t PBA_SLVCTL_WRITE_TSIZE_START_BIT = 28;
    const uint32_t PBA_SLVCTL_WRITE_TSIZE_END_BIT = 35;
    const uint32_t PBA_SLVCTL_EXT_ADDR_START_BIT = 36;
    const uint32_t PBA_SLVCTL_EXT_ADDR_END_BIT = 49;

    const uint32_t PBA_SLVCTL_EXTADDR_SHIFT = 27;
    const uint32_t PBA_SLVCTL_EXTADDR_MASK = 0x3fff;

//PBA Slave Reset register field/bit definitions
    const uint32_t PBA_SLVRST_SET_START_BIT = 0;
    const uint32_t PBA_SLVRST_SET_END_BIT = 2;
    const uint32_t PBA_SLVRST_SLVCTL0_IN_PROG = 4;
    const uint32_t PBA_SLVRST_SLVCTL1_IN_PROG = 5;
    const uint32_t PBA_SLVRST_SLVCTL2_IN_PROG = 6;
    const uint32_t PBA_SLVRST_SLVCTL3_IN_PROG = 7;
    const uint32_t PBA_SLVRST_IN_PROG_START_BIT = 4;
    const uint32_t PBA_SLVRST_IN_PROG_END_BIT = 7;
    const uint32_t PBA_SLVRST_BUSY_START_BIT = 8;
    const uint32_t PBA_SLVRST_BUSY_END_BIT = 11;
    //mask to check if there is a PBA slave rest in progress and if the PBA Slave Control is busy
    //if it is not all these bits 4:11 should be set to 0
    const uint64_t PBA_SLVRST_BUSY_IN_PROG_MASK = 0xFF0000000000000ull;

//PBA Read Buffer Valid Status field/bit definitions
    const uint32_t PBA_RD_BUF_VALID_START_BIT = 33;
    const uint32_t PBA_RD_BUF_VALID_END_BIT = 39;
    const uint64_t PBA_RD_BUF_VALID_MASK = 0x7F000000ull;
    const uint64_t PBA_RD_BUF_EMPTY =  0x1000000ull;
    const uint64_t PBA_RD_BUF_VALID =  0x4000000ull;
    const uint64_t PBA_RD_BUF_VALIDWFP = 0x8000000ull;

//PBA Write Buffer Valid Status field/bit definitions
    const uint32_t PBA_WR_BUF_VALID_START_BIT = 35;
    const uint32_t PBA_WR_BUF_VALID_END_BIT = 39;
    const uint64_t PBA_WR_BUF_VALID_MASK = 0x1F000000ull;
    const uint64_t PBA_WR_BUF_EMPTY = 0x1000000ull;

//PBA BAR register field/bit definitions
    const uint32_t PBA_BAR_SCOPE_START_BIT = 0;
    const uint32_t PBA_BAR_SCOPE_END_BIT = 2;
    const uint32_t PBA_BAR_BASE_ADDRESS_START_BIT = 8;
    const uint32_t PBA_BAR_BASE_ADDRESS_END_BIT = 43;
    const uint32_t PBA_BAR_BASE_ADDRESS_SHIFT = 20;
    const uint64_t PBA_BAR_BASE_ADDRESS_MASK = 0xFFFFFFFFFull;

//PBA BAR Mask register field/bit definitions
    const uint32_t PBA_BAR_MASK_START_BIT = 23;
    const uint32_t PBA_BAR_MASK_END_BIT = 43;

//OCB3_ADDRESS field/bit definitions
    const uint32_t OCB3_ADDRESS_REG_ADDR_SHIFT = 32;

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

        //subtract the oci part of the address from this maximum number and divide by 8 to get the number of bytes
        //then divide by 128 to get the number of 128 bye granules that can be sent
        o_numGranules = ((maximumAddress - (i_address & oci_address_mask)) / 8) / 128;
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
        //set bit 4 and unset bit 5 of OCB3 Status Control Register
        ocb_status_ctl_data.flush<1>().clearBit<5>();
        FAPI_TRY(fapi2::putScom(i_target, PU_OCB_PIB_OCBCSR3_CLEAR,
                                ocb_status_ctl_data),
                 "Error writing to the OCB3 Status Control Register with and mask");
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
        pba_slave_ctl_data.setBit<PBA_SLVCTL_ENABLE_BIT>();
        //set bits 1:3 to 110 for setting MasterID Match = OCB
        pba_slave_ctl_data.insertFromRight < PBA_SLVCTL_MASTER_ID_MATCH_START_BIT,
                                           (PBA_SLVCTL_MASTER_ID_MATCH_END_BIT - PBA_SLVCTL_MASTER_ID_MATCH_START_BIT) + 1 >
                                           (6);
        //set bits 5:7 to 111 so that MasterID Care Match limits to ONLY the OCB
        pba_slave_ctl_data.insertFromRight < PBA_SLVCTL_MASTER_ID_CARE_MASK_START_BIT,
                                           (PBA_SLVCTL_MASTER_ID_CARE_MASK_END_BIT -
                                            PBA_SLVCTL_MASTER_ID_CARE_MASK_START_BIT) + 1 > (7);

        //set the write ttype bits 8:10 to whatever is in the flags
        pba_slave_ctl_data.insertFromRight < PBA_SLVCTL_WRITE_TTYPE_START_BIT,
                                           (PBA_SLVCTL_WRITE_TTYPE_END_BIT - PBA_SLVCTL_WRITE_TTYPE_START_BIT) + 1 > (l_operType);

        //it's not cache-inhibited so set bit 15 to cl_rd_nc (0)
        pba_slave_ctl_data.clearBit<PBA_SLVCTL_READ_TTYPE_BIT>();
        //set bits 16:17 to No prefetch 01 TODO May need to change this later if we want to use prefetch
        pba_slave_ctl_data.insertFromRight < PBA_SLVCTL_READ_PREFETCH_CTL_START_BIT,
                                           (PBA_SLVCTL_READ_PREFETCH_CTL_END_BIT - PBA_SLVCTL_READ_PREFETCH_CTL_START_BIT)
                                           + 1 > (1);
        //unset bit 18 - no auto-invalidate
        pba_slave_ctl_data.clearBit<PBA_SLVCTL_READ_BUF_INVALIDATE_CTL_BIT>();
        //set bit 19 - write buffer pair allocation bit to 1
        pba_slave_ctl_data.setBit<PBA_SLVCTL_WRITE_BUF_PAIR_ALLOCATION_BIT>();
        //set bit 21 - read buffer pair b allocation bit to 1
        pba_slave_ctl_data.setBit<PBA_SLVCTL_READ_BUF_PAIR_B_ALLOCATION_BIT>();
        //unset bits 20, 22, and 23
        pba_slave_ctl_data.clearBit<PBA_SLVCTL_READ_BUF_PAIR_A_ALLOCATION_BIT>().clearBit<PBA_SLVCTL_READ_BUF_PAIR_C_ALLOCATION_BIT>().clearBit<PBA_SLVCTL_READ_BUF_PAIR_C_ALLOCATION_BIT>();
        //unset bit 24 to allow write gather
        pba_slave_ctl_data.clearBit<PBA_SLVCTL_DISABLE_WRITE_GATHER_BIT>();
        //set bits 25:27 to 000 for write gather timeout NA
        pba_slave_ctl_data.insertFromRight < PBA_SLVCTL_WRITE_GATHER_TIMEOUT_START_BIT,
                                           (PBA_SLVCTL_WRITE_GATHER_TIMEOUT_END_BIT -
                                            PBA_SLVCTL_WRITE_GATHER_TIMEOUT_START_BIT) + 1 > (0);

        //set bits 28:35 for the tsize to 0 - TODO when this is a write need to do the chiplet ID of the L3 cache in the form of 00cc_ccc0 if it's an lco_m
        //pass in an extra quad target argument
        if (l_operType == p9_PBA_oper_flag::LCO)
        {
            FAPI_TRY(fapi2::getScom(i_ex_target, EX_L3_MODE_REG1, l3_mode_reg1), "Error reading from the L3 Mode Register");
            l3_mode_reg1.extractToRight(chiplet_number, 1, 5);
        }

        pba_slave_ctl_data.insertFromRight < PBA_SLVCTL_WRITE_TSIZE_START_BIT,
                                           (PBA_SLVCTL_WRITE_TSIZE_END_BIT - PBA_SLVCTL_WRITE_TSIZE_START_BIT) + 1 > (chiplet_number);
        //set bits 36:49 to the ext addr
        extaddr = ((uint32_t) (i_address >> PBA_SLVCTL_EXTADDR_SHIFT)) &
                  PBA_SLVCTL_EXTADDR_MASK;

        pba_slave_ctl_data.insertFromRight < PBA_SLVCTL_EXT_ADDR_START_BIT,
                                           (PBA_SLVCTL_EXT_ADDR_END_BIT - PBA_SLVCTL_EXT_ADDR_START_BIT) + 1 > (extaddr);

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
        pba_bar_data.insertFromRight < PBA_BAR_SCOPE_START_BIT,
                                     (PBA_BAR_SCOPE_END_BIT - PBA_BAR_SCOPE_START_BIT) + 1 >
                                     (PBA_BAR_SCOPE_LOCAL_NODE);

        //set base address bits 8:43
        pba_bar_data.insertFromRight < PBA_BAR_BASE_ADDRESS_START_BIT,
                                     (PBA_BAR_BASE_ADDRESS_END_BIT - PBA_BAR_BASE_ADDRESS_START_BIT) + 1 > ((
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
            FAPI_TRY(fapi2::putScom(i_target, PU_OCB_PIB_OCBDR3, data),
                     "Error writing to the PBA via the OCB");
        }

    fapi_try_exit:
        FAPI_DBG("End");
        return fapi2::current_err;
    }

    fapi2::ReturnCode p9_pba_coherent_pba_read(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
        const uint64_t i_address,
        uint8_t o_read_data[])
    {
        fapi2::buffer<uint64_t> data;

        FAPI_DBG("Start");

        //Perform a 128B read -- need to do 16 8B reads since it's in linear mode which can only do 8B...
        for (int i = 0; i < 16; i++)
        {
            FAPI_TRY(fapi2::getScom(i_target, PU_OCB_PIB_OCBDR3, data),
                     "Error reading from the PBA via the OCB");

            for (int j = 0; j < 8; j++)
            {
                o_read_data[(i * 8) + j] = (data >> (56 - (j * 8))) & 0xFFull;;
            }
        }

    fapi_try_exit:
        FAPI_DBG("End");
        return fapi2::current_err;
    }

    fapi2::ReturnCode p9_pba_coherent_cleanup_pba(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_DBG("Start");

        fapi2::buffer<uint64_t> data;

        //Clean up the PBA register by resetting PBASLVCTL3 by writing to the PBASLVRST
        data.insertFromRight < PBA_SLVRST_SET_START_BIT,
                             (PBA_SLVRST_SET_END_BIT - PBA_SLVRST_SET_START_BIT) + 1 > (7);
        FAPI_TRY(fapi2::putScom(i_target, PU_PBASLVRST_SCOM, data),
                 "Error writing to the PBA Slave Reset register");

        //Wait a little bit and make sure that the reset is no longer in progress
        FAPI_TRY(fapi2::delay(PBA_SLVRST_DELAY_HW_NS, PBA_SLVRST_DELAY_SIM_CYCLES),
                 "Error from PBA Slave Reset delay");

        FAPI_TRY(fapi2::getScom(i_target, PU_PBASLVRST_SCOM, data),
                 "Error reading from the PBA Slave Reset register");

        FAPI_ASSERT(!data.getBit<PBA_SLVRST_SLVCTL3_IN_PROG>(),
                    fapi2::P9_PBA_COHERENT_UTILS_RESET_ERR().set_TARGET(i_target).set_RDDATA(
                        data),
                    "Error in resetting the PBA Slave Reset register");

    fapi_try_exit:
        FAPI_DBG("End");
        return fapi2::current_err;
    }

    fapi2::ReturnCode p9_pba_coherent_status_check(
        const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
    {
        FAPI_DBG("Start");

        fapi2::buffer<uint64_t> rd_buf2_valid;
        fapi2::buffer<uint64_t> rd_buf3_valid;
        fapi2::buffer<uint64_t> wr_buf0_valid;
        fapi2::buffer<uint64_t> wr_buf1_valid;
        fapi2::buffer<uint64_t> reset_buf;

        //Check the 2 PBA Read Buffer Valid Status (2 and 3 since we set Buffer pair "B")by reading the read buffer status (bits 33:39) and making sure it's 1
        FAPI_TRY(fapi2::getScom(i_target, PU_PBARBUFVAL2, rd_buf2_valid),
                 "Error reading from the PBA Read Buffer Valid 2 Status Register");
        FAPI_TRY(fapi2::getScom(i_target, PU_PBARBUFVAL3, rd_buf3_valid),
                 "Error reading from the PBA Read Buffer Valid 3 Status Register");

        //Check the 2 PBA Write Buffer Valid Status by reading the write buffer status (bits 35:39) and making sure it's 1
        FAPI_TRY(fapi2::getScom(i_target, PU_PBAWBUFVAL0, wr_buf0_valid),
                 "Error reading from the PBA Write Buffer Valid 0 Status Register");
        FAPI_TRY(fapi2::getScom(i_target, PU_PBAWBUFVAL1, wr_buf1_valid),
                 "Error reading from the PBA Write Buffer Valid 1 Status Register");

        //Check the PBA Slave Reset Register for if things are still in progress
        FAPI_TRY(fapi2::getScom(i_target, PU_PBASLVRST_SCOM, reset_buf),
                 "Error reading from the PBA Slave Reset Register");

        //If there are any errors in the Status registers that we got above, collect all of the data and send an error
        FAPI_ASSERT((((((rd_buf2_valid & PBA_RD_BUF_VALID_MASK) == PBA_RD_BUF_EMPTY)
                       || ((rd_buf2_valid & PBA_RD_BUF_VALID_MASK) == PBA_RD_BUF_VALID)
                       || ((rd_buf2_valid & PBA_RD_BUF_VALID_MASK) == PBA_RD_BUF_VALIDWFP)) )
                     && (((rd_buf3_valid & PBA_RD_BUF_VALID_MASK) == PBA_RD_BUF_EMPTY)
                         || ((rd_buf3_valid & PBA_RD_BUF_VALID_MASK) == PBA_RD_BUF_VALID)
                         || ((rd_buf3_valid & PBA_RD_BUF_VALID_MASK) == PBA_RD_BUF_VALIDWFP)  )
                     && ((wr_buf0_valid & PBA_WR_BUF_VALID_MASK) == PBA_WR_BUF_EMPTY)
                     && ((wr_buf1_valid & PBA_WR_BUF_VALID_MASK) == PBA_WR_BUF_EMPTY)
                     && ((reset_buf & PBA_SLVRST_BUSY_IN_PROG_MASK) == 0)),
                    fapi2::P9_PBA_STATUS_ERR().set_TARGET(i_target).set_RDBUF2(
                        rd_buf2_valid).set_RDBUF3(rd_buf3_valid).set_WRBUF0(
                        wr_buf0_valid).set_WRBUF1(wr_buf1_valid).set_SLVRSTDATA(reset_buf),
                    "Error in checking the PBA Reset, PBA Read Buffer, or PBA Write Buffer Registers");

    fapi_try_exit:
        FAPI_DBG("End");
        return fapi2::current_err;
    }

} //extern "C"
