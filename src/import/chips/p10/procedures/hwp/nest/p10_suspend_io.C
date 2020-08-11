/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/nest/p10_suspend_io.C $   */
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
///
/// @file p10_suspend_io.C
/// @brief Procedure to suspend PCIe traffic (FAPI2)
///
/// *HWP HWP Maintainer: Manish Chowdhary  <manichow@in.ibm.com>
/// *HWP FW Maintainer : Raja Das <rajadas2@in.ibm.com>
/// *HWP Consumed by   : SBE

//-----------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------
#include <p10_suspend_io.H>
// Cronus only
#if !defined(__PPE__) && !defined(__HOSTBOOT_MODULE)
    #include <p10_pcie_utils.H>
#endif
#include <p10_scom_phb_3.H>
#include <p10_scom_phb_4.H>
#include <p10_scom_phb_a.H>
#include <p10_scom_phb_d.H>
#include <p10_scom_phb_e.H>
#include <p10_scom_phb_f.H>
#include <p10_scom_phb_2.H>

///
/// @brief p10_suspend_io procedure entry point
/// See doxygen in p10_suspend_io.H
///
fapi2::ReturnCode p10_suspend_io(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target, const bool i_enable_mpipl)
{
    FAPI_DBG("Start MPIPL Procedure");
    using namespace scomt::phb;

    bool found_freeze_err;   // true: found err that freezes PBCQ; otherwise false
    uint32_t l_poll_counter; // # of iterations while polling for inbound_active and outbound_active
    uint32_t l_cur_fir_bit = 0; // Counter use to track freeze bits read.
    uint64_t act0_bits;
    uint64_t act1_bits;

    fapi2::buffer<uint64_t> l_buf = 0;
    fapi2::buffer<uint64_t> l_buf2 = 0;
    uint8_t l_phb_id = 0;

    auto l_phb_chiplets_vec = i_target.getChildren<fapi2::TARGET_TYPE_PHB>();
    FAPI_DBG("PHB target vec size: %#x", l_phb_chiplets_vec.size());

    //Loop through all PHBs configured
    for (auto l_phb_chiplets : l_phb_chiplets_vec)
    {
        // Get the PHB id
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_phb_chiplets, l_phb_id),
                 "Error from FAPI_ATTR_GET (ATTR_CHIP_UNIT_POS");

// Cronus only
#if !defined(__PPE__) && !defined(__HOSTBOOT_MODULE)
        // Skip if PHB target is not enabled
        bool l_phbEnabled = false;
        FAPI_TRY(isPHBEnabled(l_phb_chiplets, l_phbEnabled),
                 "Error returned from isPHBEnabled()");

        if (!l_phbEnabled)
        {
            FAPI_DBG("PHB ID %i is disabled, skip suspend_io.", l_phb_id);
            continue;
        }

#endif

        //****************************************************************************************************************
        // STEP #1: nop
        //****************************************************************************************************************

        //******************************************************************************************************************
        // Recov_2: Force PEC into Freeze mode by writing into SW Freeze bit in PCI NEST FIR(26), if PEC not already frozen.
        //******************************************************************************************************************

        //Initially, start with no error found that freezes PEC
        found_freeze_err = false;

        FAPI_DBG("PHB%i: Recov_2 - Force freeze the PEC if not already frozen.", l_phb_id);

        //Read PCI Nest FIR Action0 register and put contents into l_buf
        FAPI_TRY(GET_REGS_NFIRACTION0_REG(l_phb_chiplets, l_buf));
        FAPI_DBG("PHB%i: PCI Nest FIR Action0 Register %#lx", l_phb_id, l_buf());
        act0_bits = l_buf;

        //Read PCI Nest FIR Action1 register and put contents into l_buf
        FAPI_TRY(GET_REGS_NFIRACTION1_REG(l_phb_chiplets, l_buf));
        FAPI_DBG("PHB%i: PCI Nest FIR Action1 Register %#lx", l_phb_id, l_buf());
        act1_bits = l_buf;

        //AND Action0 and Action1 bits to determine which FIR bits are set up for Freeze = 0b11
        l_buf2 = act0_bits & act1_bits;
        FAPI_DBG("PHB%i: PCI Nest FIR Freeze Bits %#lx", l_phb_id, l_buf2());

        //Read PCI Nest FIR register and put contents into l_buf
        FAPI_TRY(GET_REGS_NFIR_REG_RW(l_phb_chiplets, l_buf));
        FAPI_DBG("PHB%i: PCI Nest FIR Register %#lx", l_phb_id, l_buf());

        //Inspect which FIR bits are set AND if action is set to Freeze
        while (l_cur_fir_bit < REGS_NFIR_REG_NFIRNFIR_LEN)
        {
            if(l_buf.getBit(l_cur_fir_bit) && l_buf2.getBit(l_cur_fir_bit))
            {
                FAPI_DBG("PHB%i: PEC is already frozen. PCI Nest FIR[%u] is asserted.",
                         l_phb_id, l_cur_fir_bit);
                found_freeze_err = true;
                break;
            }

            l_cur_fir_bit++;
        }

        //Found PEC to not be frozen
        if(!found_freeze_err)
        {
            FAPI_DBG("PHB%i: PEC not frozen. Forcing PEC into freeze mode.", l_phb_id);

            //Force PEC freeze by setting SW Freeze bit in PCI Nest FIR Register
            l_buf = 0;
            l_buf.setBit<PHB_NFIR_REG_SW_DEFINED_FREEZE>();
            FAPI_DBG("PHB%i: PCI Nest FIR Register %#lx", l_phb_id, l_buf());
            FAPI_TRY(PREP_REGS_NFIR_REG_WO_OR(l_phb_chiplets));
            FAPI_TRY(PUT_REGS_NFIR_REG_WO_OR(l_phb_chiplets, l_buf));
        }

        FAPI_DBG("PHB%i: Successfully put PEC into freeze state", l_phb_id);

        //****************************************************************************************************************
        //Recov_3: Put the PHB in reset state
        //****************************************************************************************************************
        FAPI_DBG("PHB%i: Recov_3 - Put PHB into reset state.", l_phb_id);

        //Set bit0 of the PHB Reset Register to drive reset active to the phb
        l_buf = 0;
        l_buf.setBit<REGS_PHBRESET_REG_PE_ETU_RESET>();
        FAPI_DBG("PHB%i: PHB Reset Register %#lx", l_phb_id, l_buf());
        FAPI_TRY(PREP_REGS_PHBRESET_REG(l_phb_chiplets));
        FAPI_TRY(PUT_REGS_PHBRESET_REG(l_phb_chiplets, l_buf));
        FAPI_DBG("PHB%i: Successfully put PHB into reset", l_phb_id);

        //****************************************************************************************************************
        //Recov_4: Poll the CQ Status until all actively working DMA/CI operations are done by waiting for bits[0:1] = 00
        //****************************************************************************************************************
        FAPI_DBG("PHB%i: Recov_4 - Start polling for inbound_active and outbound_active state machines to become idle.",
                 l_phb_id);

        l_poll_counter = 0; //Reset poll counter
        FAPI_TRY(PREP_REGS_CQSTAT_REG(l_phb_chiplets));

        while (l_poll_counter < MAX_NUM_POLLS)
        {
            l_poll_counter++;
            FAPI_TRY(fapi2::delay(PBCQ_NANO_SEC_DELAY, PBCQ_SIM_CYC_DELAY),
                     "fapiDelay error.");

            //Read PBCQ General Status Register and put contents into l_buf
            FAPI_TRY(GET_REGS_CQSTAT_REG(l_phb_chiplets, l_buf));

            FAPI_DBG("PHB%i: PBCQ General Status Register %#lx", l_phb_id, l_buf());

            //Check for bits 0 (inbound_active) and 1 (outbound_active) to become deasserted
            if (!(l_buf.getBit(REGS_CQSTAT_REG_INBOUND_ACTIVE) || l_buf.getBit(REGS_CQSTAT_REG_OUTBOUND_ACTIVE)))
            {
                FAPI_DBG("PHB%i: PBCQ CQ status is idle.", l_phb_id);
                FAPI_DBG("PHB%i: End polling for inbound_active and outbound_active state machines to become idle.", l_phb_id);
                break;
            }
        }

        FAPI_DBG("PHB%i: inbound_active and outbound_active status (poll counter = %d).", l_phb_id, l_poll_counter);
        fapi2::buffer<uint64_t> l_buf3 = 0;

        FAPI_TRY(GET_REGS_NFIR_REG_RW(l_phb_chiplets, l_buf2));

        FAPI_DBG("PHB%i: PCI Nest FIR Register %#lx", l_phb_id, l_buf2());

        FAPI_TRY(GET_REGS_PHBRESET_REG(l_phb_chiplets, l_buf3));

        FAPI_DBG("PHB%i: PHB Reset Register %#lx", l_phb_id, l_buf3());

        FAPI_ASSERT(l_poll_counter < MAX_NUM_POLLS,
                    fapi2::P10_PBCQ_CQ_NOT_IDLE()
                    .set_TARGET(l_phb_chiplets)
                    .set_NFIR_ADDR(REGS_NFIR_REG_RW)
                    .set_NFIR_DATA(l_buf2)
                    .set_PHB_RESET_ADDR(REGS_PHBRESET_REG)
                    .set_PHB_RESET_DATA(l_buf3)
                    .set_CQ_STAT_ADDR(REGS_CQSTAT_REG)
                    .set_CQ_STAT_DATA(l_buf),
                    "PHB%i: PBCQ CQ Status did not clear.", l_phb_id);


        //****************************************************************************************************************
        //Recov_5: Clear the BAR and IRSN enables to avoid addressing conflict post IPL.
        //****************************************************************************************************************
        FAPI_DBG("PHB%i: Recov_5 - Clearing Base Address Enabled register.", l_phb_id);

        //Clear BARE register
        FAPI_TRY(GET_REGS_BARE_REG(l_phb_chiplets, l_buf));

        l_buf.clearBit<REGS_BARE_REG_MMIO_BAR0_EN>()
        .clearBit<REGS_BARE_REG_MMIO_BAR1_EN>()
        .clearBit<REGS_BARE_REG_PHB_BAR_EN>()
        .clearBit<REGS_BARE_REG_INT_BAR_EN>();

        FAPI_DBG("PHB%i: Base Address Enable Register %#lx", l_phb_id, l_buf());
        FAPI_TRY(PUT_REGS_BARE_REG(l_phb_chiplets, l_buf));
        FAPI_DBG("PHB%i: Successfully cleared BARE", l_phb_id);

        //****************************************************************************************************************
        //Recov_6: Clear PCI FIR (PFIR) Freeze Errors.
        //****************************************************************************************************************
        FAPI_DBG("PHB%i: Recov_6 - Clearing PCI FIR.", l_phb_id);

        //Clear FIR bits of PCI FIR register
        l_buf = (uint64_t)0x0;
        FAPI_DBG("PHB%i: PCI FIR Register Clear %#lx", l_phb_id, l_buf());

        FAPI_TRY(PREP_REGS_PFIR_REG_WO_AND(l_phb_chiplets));
        FAPI_TRY(PUT_REGS_PFIR_REG_WO_AND(l_phb_chiplets, l_buf));

        //Confirm FIR bits have been cleared
        FAPI_TRY(GET_REGS_PFIR_REG_RW(l_phb_chiplets, l_buf));
        FAPI_DBG("PHB%i: PCI FIR Register %#lx", l_phb_id, l_buf());
        FAPI_DBG("PHB%i: Successfully cleared PCI FIR.", l_phb_id);

        //****************************************************************************************************************
        //Recov_7: Clear CERR Report 0/1 bits
        //****************************************************************************************************************
        FAPI_DBG("PHB%i: Recov_7 - Clearing CERR Report 0/1 bits.", l_phb_id);

        // Clear any spurious cerr_rpt0/1 bits by writing
        // 0xFFFFFFFF_FFFFFFFF
        l_buf.flush<1>();
        FAPI_TRY(PREP_REGS_CERR_RPT0_REG(l_phb_chiplets));
        FAPI_TRY(PUT_REGS_CERR_RPT0_REG(l_phb_chiplets, l_buf));

        FAPI_TRY(PREP_REGS_CERR_RPT1_REG(l_phb_chiplets));
        FAPI_TRY(PUT_REGS_CERR_RPT1_REG(l_phb_chiplets, l_buf));

        FAPI_DBG("PHB%i: Succesfully cleared CERR Report 0/1 bits.", l_phb_id);

        //****************************************************************************************************************
        //Recov_8: Clear Nest FIR (NFIR) Freeze Errors.
        //****************************************************************************************************************
        FAPI_DBG("PHB%i: Recov_8 - Clearing PCI Nest FIR.", l_phb_id);

        //Clear FIR bits of PCI Nest FIR register
        l_buf.flush<0>();
        FAPI_TRY(PREP_REGS_NFIR_REG_WO_AND(l_phb_chiplets));
        FAPI_TRY(PUT_REGS_NFIR_REG_WO_AND(l_phb_chiplets, l_buf));

        //Confirm FIR bits have been cleared
        FAPI_TRY(GET_REGS_NFIR_REG_RW(l_phb_chiplets, l_buf));
        FAPI_DBG("PHB%i: PCI Nest FIR Register %#lx", l_phb_id, l_buf());

        if (l_buf.getBit<REGS_NFIR_REG_NFIRNFIR, REGS_NFIR_REG_NFIRNFIR_LEN>())
        {
            FAPI_ASSERT(false,
                        fapi2::P10_PHB_NFIR_NOT_CLEARED()
                        .set_TARGET(l_phb_chiplets)
                        .set_NFIR_ADDR(REGS_NFIR_REG_RW)
                        .set_NFIR_DATA(l_buf)
                        .set_PFIR_ADDR(REGS_PFIR_REG_RW)
                        .set_PFIR_DATA(l_buf2),
                        "PHB%i: PCI Nest FIR Register did not clear.", l_phb_id);
        }


        FAPI_DBG("PHB%i: Succesfully cleared PCI Nest FIR.", l_phb_id);
        //****************************************************************************************************************
        //Recov_9: PEC is ready for firmware to take PHB out of reset and start initialization.
        //****************************************************************************************************************
    }

fapi_try_exit:
    FAPI_DBG("End MPIPL Procedure");
    return fapi2::current_err;
}
