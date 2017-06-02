/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/nest/p9_suspend_io.C $     */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2017                        */
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
/// @file p9_suspend_io.C
/// @brief Procedure to suspend PCIe traffic (FAPI2)
///
// *HWP HWP Owner: Ricardo Mata Jr. ricmata@us.ibm.com
// *HWP FW Owner: Thi Tran thi@us.ibm.com
// *HWP Team: Nest
// *HWP Level: 3
// *HWP Consumed by: HB

//-----------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------
#include <p9_suspend_io.H>
#include <p9_misc_scom_addresses.H>
#include <p9_misc_scom_addresses_fld.H>

///
/// @brief p9_suspend_io procedure entry point
/// See doxygen in p9_suspend_io.H
///
fapi2::ReturnCode p9_suspend_io(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target, const bool i_enable_mpipl)
{
    FAPI_DBG("Start MPIPL Procedure");

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

    fapi2::ATTR_CHIP_EC_FEATURE_HW363246_Type l_hw363246;
    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_EC_FEATURE_HW363246,
                           i_target,
                           l_hw363246),
             "Error from FAPI_ATTR_GET (ATTR_CHIP_EC_FEATURE_HW363246)");

    //Loop through all PHBs configured
    for (auto l_phb_chiplets : l_phb_chiplets_vec)
    {
        // Get the PHB id
        FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, l_phb_chiplets, l_phb_id),
                 "Error from FAPI_ATTR_GET (ATTR_CHIP_UNIT_POS");

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
        FAPI_TRY(fapi2::getScom(l_phb_chiplets, PHB_NFIRACTION0_REG, l_buf),
                 "Error from getScom (0x%.16llX)", PHB_NFIRACTION0_REG);
        FAPI_DBG("PHB%i: PCI Nest FIR Action0 Register %#lx", l_phb_id, l_buf());
        act0_bits = l_buf;

        //Read PCI Nest FIR Action1 register and put contents into l_buf
        FAPI_TRY(fapi2::getScom(l_phb_chiplets, PHB_NFIRACTION1_REG, l_buf),
                 "Error from getScom (0x%.16llX)", PHB_NFIRACTION1_REG);
        FAPI_DBG("PHB%i: PCI Nest FIR Action1 Register %#lx", l_phb_id, l_buf());
        act1_bits = l_buf;

        //AND Action0 and Action1 bits to determine which FIR bits are set up for Freeze = 0b11
        l_buf2 = act0_bits & act1_bits;
        FAPI_DBG("PHB%i: PCI Nest FIR Freeze Bits %#lx", l_phb_id, l_buf2());

        //Read PCI Nest FIR register and put contents into l_buf
        FAPI_TRY(fapi2::getScom(l_phb_chiplets, PHB_NFIR_REG, l_buf),
                 "Error from getScom (0x%.16llX)", PHB_NFIR_REG);
        FAPI_DBG("PHB%i: PCI Nest FIR Register %#lx", l_phb_id, l_buf());

        //Inspect which FIR bits are set AND if action is set to Freeze
        while (l_cur_fir_bit < PHB_NFIR_REG_NFIRNFIR_LEN)
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
            FAPI_TRY(fapi2::putScom(l_phb_chiplets, PHB_NFIR_REG_OR, l_buf),
                     "Error from putScom (0x%.16llX)", PHB_NFIR_REG_OR);
        }

        FAPI_DBG("PHB%i: Successfully put PEC into freeze state", l_phb_id);

        //****************************************************************************************************************
        //Recov_3: Put the PHB in reset state
        //****************************************************************************************************************
        FAPI_DBG("PHB%i: Recov_3 - Put PHB into reset state.", l_phb_id);

        //Set bit0 of the PHB Reset Register to drive reset active to the phb
        l_buf = 0;
        l_buf.setBit<PHB_PHBRESET_REG_PE_ETU_RESET>();
        FAPI_DBG("PHB%i: PHB Reset Register %#lx", l_phb_id, l_buf());
        FAPI_TRY(fapi2::putScom(l_phb_chiplets, PHB_PHBRESET_REG, l_buf),
                 "Error from putScom (0x%.16llX)", PHB_PHBRESET_REG);
        FAPI_DBG("PHB%i: Successfully put PHB into reset", l_phb_id);

        //****************************************************************************************************************
        //Recov_4: Poll the CQ Status until all actively working DMA/CI operations are done by waiting for bits[0:1] = 00
        //****************************************************************************************************************
        FAPI_DBG("PHB%i: Recov_4 - Start polling for inbound_active and outbound_active state machines to become idle.",
                 l_phb_id);

        l_poll_counter = 0; //Reset poll counter

        while (l_poll_counter < MAX_NUM_POLLS)
        {
            l_poll_counter++;
            FAPI_TRY(fapi2::delay(PBCQ_NANO_SEC_DELAY, PBCQ_SIM_CYC_DELAY),
                     "fapiDelay error.");

            //Read PBCQ General Status Register and put contents into l_buf
            FAPI_TRY(fapi2::getScom(l_phb_chiplets, PHB_CQSTAT_REG, l_buf),
                     "Error from getScom (0x%.16llX)", PHB_CQSTAT_REG);
            FAPI_DBG("PHB%i: PBCQ General Status Register %#lx", l_phb_id, l_buf());

            //Check for bits 0 (inbound_active) and 1 (outbound_active) to become deasserted
            if (!(l_buf.getBit(PEC_STACK0_CQSTAT_REG_PE_INBOUND_ACTIVE) || l_buf.getBit(PEC_STACK0_CQSTAT_REG_PE_OUTBOUND_ACTIVE)))
            {

                FAPI_DBG("PHB%i: PBCQ CQ status is idle.", l_phb_id);
                FAPI_DBG("PHB%i: End polling for inbound_active and outbound_active state machines to become idle.", l_phb_id);
                break;
            }
        }

        FAPI_DBG("PHB%i: inbound_active and outbound_active status (poll counter = %d).", l_phb_id, l_poll_counter);
        fapi2::buffer<uint64_t> l_buf3 = 0;

        FAPI_TRY(fapi2::getScom(l_phb_chiplets, PHB_NFIR_REG, l_buf2),
                 "Error from getScom (0x%.16llX)", PHB_NFIR_REG);

        FAPI_DBG("PHB%i: PCI Nest FIR Register %#lx", l_phb_id, l_buf2());

        FAPI_TRY(fapi2::getScom(l_phb_chiplets, PHB_PHBRESET_REG, l_buf3),
                 "Error from getScom (0x%.16llX)", PHB_PHBRESET_REG);

        FAPI_DBG("PHB%i: PHB Reset Register %#lx", l_phb_id, l_buf3());

        FAPI_ASSERT(l_poll_counter < MAX_NUM_POLLS,
                    fapi2::P9_PBCQ_CQ_NOT_IDLE()
                    .set_TARGET(l_phb_chiplets)
                    .set_NFIR_ADDR(PHB_NFIR_REG)
                    .set_NFIR_DATA(l_buf2)
                    .set_PHB_RESET_ADDR(PHB_PHBRESET_REG)
                    .set_PHB_RESET_DATA(l_buf3)
                    .set_CQ_STAT_ADDR(PHB_CQSTAT_REG)
                    .set_CQ_STAT_DATA(l_buf),
                    "PHB%i: PBCQ CQ Status did not clear.", l_phb_id);


        //****************************************************************************************************************
        //Recov_5: Clear the BAR and IRSN enables to avoid addressing conflict post IPL.
        //****************************************************************************************************************
        FAPI_DBG("PHB%i: Recov_5 - Clearing Base Address Enabled register.", l_phb_id);

        //Clear BARE register
        FAPI_TRY(fapi2::getScom(l_phb_chiplets, PHB_BARE_REG, l_buf),
                 "Error from getScom (0x%.16llX)", PHB_BARE_REG);
        l_buf.clearBit<PHB_BARE_REG_PE_MMIO_BAR0_EN>()
        .clearBit<PHB_BARE_REG_PE_MMIO_BAR1_EN>()
        .clearBit<PHB_BARE_REG_PE_PHB_BAR_EN>()
        .clearBit<PHB_BARE_REG_PE_INT_BAR_EN>();
        FAPI_DBG("PHB%i: Base Address Enable Register %#lx", l_phb_id, l_buf());
        FAPI_TRY(fapi2::putScom(l_phb_chiplets, PHB_BARE_REG, l_buf),
                 "Error from putScom (0x%.16llX)", PHB_BARE_REG);
        FAPI_DBG("PHB%i: Successfully cleared BARE", l_phb_id);

        //****************************************************************************************************************
        //Recov_6: Clear PCI FIR (PFIR) Freeze Errors.
        //****************************************************************************************************************
        FAPI_DBG("PHB%i: Recov_6 - Clearing PCI FIR.", l_phb_id);

        //Clear FIR bits of PCI FIR register
        l_buf = (uint64_t)0x0;
        FAPI_DBG("PHB%i: PCI FIR Register Clear %#lx", l_phb_id, l_buf());
        FAPI_TRY(fapi2::putScom(l_phb_chiplets, PHB_PFIR_REG_AND, l_buf),
                 "Error from putScom (0x%.16llX)", PHB_PFIR_REG_AND);

        //Confirm FIR bits have been cleared
        FAPI_TRY(fapi2::getScom(l_phb_chiplets, PHB_PFIR_REG, l_buf),
                 "Error from getScom (0x%.16llX)", PHB_PFIR_REG);
        FAPI_DBG("PHB%i: PCI FIR Register %#lx", l_phb_id, l_buf());
        FAPI_DBG("PHB%i: Successfully cleared PCI FIR.", l_phb_id);

        //****************************************************************************************************************
        //Recov_7: Clear CERR Report 0/1 bits
        //****************************************************************************************************************
        FAPI_DBG("PHB%i: Recov_7 - Clearing CERR Report 0/1 bits.", l_phb_id);

        if (!l_hw363246)
        {
            // Clear any spurious cerr_rpt0/1 bits by writing
            // 0xFFFFFFFF_FFFFFFFF
            l_buf.flush<1>();
            FAPI_TRY(fapi2::putScom(l_phb_chiplets, PHB_CERR_RPT0_REG, l_buf),
                     "Error from putScom PHB_CERR_RPT0_REG");
            FAPI_TRY(fapi2::putScom(l_phb_chiplets, PHB_CERR_RPT1_REG, l_buf),
                     "Error from putScom PHB_CERR_RPT1_REG");
        }

        FAPI_DBG("PHB%i: Succesfully cleared CERR Report 0/1 bits.", l_phb_id);

        //****************************************************************************************************************
        //Recov_8: Clear Nest FIR (NFIR) Freeze Errors.
        //****************************************************************************************************************
        FAPI_DBG("PHB%i: Recov_8 - Clearing PCI Nest FIR.", l_phb_id);

        //Clear FIR bits of PCI Nest FIR register
        FAPI_TRY(fapi2::getScom(l_phb_chiplets, PHB_NFIR_REG, l_buf),
                 "Error from getScom (0x%.16llX)", PHB_NFIR_REG);
        FAPI_DBG("PHB%i: PCI Nest FIR Register %#lx", l_phb_id, l_buf());
        l_buf.invert();
        FAPI_DBG("PHB%i: PCI Nest FIR Register Clear %#lx", l_phb_id, l_buf());

        FAPI_TRY(fapi2::putScom(l_phb_chiplets, PHB_NFIR_REG_AND, l_buf),
                 "Error from putScom (0x%.16llX)", PHB_NFIR_REG_AND);

        //Confirm FIR bits have been cleared
        FAPI_TRY(fapi2::getScom(l_phb_chiplets, PHB_NFIR_REG, l_buf),
                 "Error from getScom (0x%.16llX)", PHB_NFIR_REG);
        FAPI_DBG("PHB%i: PCI Nest FIR Register %#lx", l_phb_id, l_buf());

        FAPI_TRY(fapi2::getScom(l_phb_chiplets, PHB_PFIR_REG, l_buf2),
                 "Error from getScom (0x%.16llX)", PHB_PFIR_REG);

        FAPI_DBG("PHB%i: PCI FIR Register %#lx", l_phb_id, l_buf2());

        if (l_buf.getBit<PHB_NFIR_REG_NFIRNFIR, PHB_NFIR_REG_NFIRNFIR_LEN>())
        {
            FAPI_ASSERT(false,
                        fapi2::P9_PHB_NFIR_NOT_CLEARED()
                        .set_TARGET(l_phb_chiplets)
                        .set_NFIR_ADDR(PHB_NFIR_REG)
                        .set_NFIR_DATA(l_buf)
                        .set_PFIR_ADDR(PHB_PFIR_REG)
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
