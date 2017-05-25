/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/lib/p9_sbe_ppe_ffdc.C $    */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2017                             */
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
/// @file  p9_sbe_ppe_ffdc.C
/// @brief Get a PPE minimal internal state for debug
///
/// *HWP HW Owner        : Greg Still <stillgs@us.ibm.com>
/// *HWP HW Backup Owner : Brian Vanderpool <vanderp@us.ibm.com>
/// *HWP FW Owner        : Amit Tendolkar <amit.tendolkar@in.ibm.com>
/// *HWP Team            : PM
/// *HWP Level           : 2
/// *HWP Consumed by     : SBE
///
/// @verbatim
///
/// Procedure Summary:
///   - Reads a PPE's minimalistic state in terms of select XIRs and SPRs
///
/// @endverbatim

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <fapi2.H>
#include <p9_sbe_ppe_ffdc.H>
#include <p9_sbe_ppe_utils.H>

// @note enum used to loop the SPRs to be rammed from the PPE
typedef enum
{
    SPR_IDX_LR = 0,
    SPR_IDX_SRR0,
    SPR_IDX_SRR1,
    SPR_IDX_MAX
}   SbePpeFfdcSPRIdx_t;

//-----------------------------------------------------------------------------
fapi2::ReturnCode
p9_sbe_ppe_ffdc (
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
    const uint64_t                                     i_base_address,
    std::vector<uint64_t>&                             o_v_ppe_reg_ffdc )
{
    fapi2::ReturnCode l_rc;
    FAPI_INF(">> p9_sbe_ppe_ffdc");

    fapi2::buffer<uint64_t> l_raminstr;
    fapi2::buffer<uint64_t> l_data64;
    fapi2::buffer<uint32_t> l_data32;
    bool l_ppe_halt_state = false;

    o_v_ppe_reg_ffdc.clear ();
    o_v_ppe_reg_ffdc.assign (REG_FFDC_IDX_MAX, default_64);

    // Halt the PPE before grabbing the state
    // Ignore a fail here, the halted state is checked after the XIR loop
    l_rc = ppe_halt(i_target, i_base_address);

    FAPI_IMP ("Collecting XIRs");

    for (uint8_t i = REG_FFDC_IDX_XIR_FIRST ; i < REG_FFDC_IDX_XIR_MAX; ++i)
    {
        uint64_t l_address = 0;

        switch (i)
        {
            case REG_FFDC_IDX_XSR_IAR:
                l_address = i_base_address + PPE_XIDBGPRO;
                break;

            case REG_FFDC_IDX_IR_EDR:
                l_address = i_base_address + PPE_XIRAMEDR;
                break;

            case REG_FFDC_IDX_LR_SPRG0:
                l_address = i_base_address + PPE_XIRAMDBG;
                break;
        }

        l_rc = getScom (i_target, l_address, l_data64);

        if (l_rc == fapi2::FAPI2_RC_SUCCESS)
        {
            o_v_ppe_reg_ffdc[i] = l_data64;
        }

        // On error, we already have the default on input, continue to next
    } // XIRs Loop

    // Optimizing getScom of i_base_address + PPE_XIRAMDBG as we already have
    // it from the last XIR_ID_SPRG0 iteration in the above XIR loop
    l_ppe_halt_state = (l_data64.getBit(0, 1)) ? true : false;

    // Get SPRs if PPE halted
    if (l_ppe_halt_state)
    {
        FAPI_INF ("Collecting SPRs");

        // Collect 3 32 bit SPRs, but pack them into a 64 bit member of
        // o_v_ppe_reg_ffdc, based on indexes defined in SbePpeRegFfdcIdx_t
        for (uint8_t i = SPR_IDX_LR; i < SPR_IDX_MAX; ++i)
        {
            uint16_t l_sprNr = 0;
            uint8_t l_sprFfdcIdx = REG_FFDC_IDX_SRR0_SRR1;

            switch (i)
            {
                case SPR_IDX_LR:
                    l_sprNr = LR;
                    l_sprFfdcIdx = REG_FFDC_IDX_LR_SPRG0;
                    break;

                case SPR_IDX_SRR0:
                    l_sprNr = SRR0;
                    break;

                case SPR_IDX_SRR1:
                    l_sprNr = SRR1;
                    break;
            }

            // SPR to R0
            l_raminstr.flush<0>().insertFromRight (
                ppe_getMxsprInstruction(MFSPR_BASE_OPCODE, 0, l_sprNr), 0, 32 );

            l_rc = ppe_pollHaltState(i_target, i_base_address);

            if (l_rc != fapi2::FAPI2_RC_SUCCESS)
            {
                FAPI_ERR ("ppe_pollHaltState fail! Idx %d SPR#%d", i, l_sprNr);
                continue; // try ramming out the next SPR
            }

            l_rc = fapi2::putScom (
                       i_target, i_base_address + PPE_XIRAMEDR, l_raminstr );

            if (l_rc != fapi2::FAPI2_RC_SUCCESS)
            {
                FAPI_ERR ("putScom XIRAMEDR fail!. Idx %d SPR#%d", i, l_sprNr);
                continue; // try ramming out the next SPR
            }

            // R0 to SPRG0
            l_raminstr.flush<0>().insertFromRight (
                ppe_getMxsprInstruction(MTSPR_BASE_OPCODE, 0, SPRG0), 0, 32 );

            l_rc = ppe_RAMRead(i_target, i_base_address, l_raminstr, l_data32);

            if (l_rc == fapi2::FAPI2_RC_SUCCESS)
            {
                if (i % 2)
                {
                    // pack into bits 0-31 of ithe ffdc vector element
                    o_v_ppe_reg_ffdc[l_sprFfdcIdx] =
                        (o_v_ppe_reg_ffdc[l_sprFfdcIdx] & 0x00000000FFFFFFFFULL) |
                        ((uint64_t) l_data32() << 32);
                }
                else
                {
                    // pack into bits 32-63 of the ffdc vector element
                    o_v_ppe_reg_ffdc[l_sprFfdcIdx] =
                        (o_v_ppe_reg_ffdc[l_sprFfdcIdx] & 0xFFFFFFFF00000000ULL) |
                        (l_data32());
                }
            }
            else
            {
                FAPI_ERR ("ppe_RAMRead fail! Idx %d SPR#%d", i, l_sprNr);
                continue; // try ramming out the next SPR
            }

            FAPI_IMP ("Idx %d SPR# %d Value= 0x%.8X", i, l_sprNr, l_data32());
        }   // SPR Loop
    }

    // else .. exit with defaulted SPR values, as PPE is not halted

    FAPI_IMP ( "<< p9_sbe_ppe_ffdc: #Regs %d", o_v_ppe_reg_ffdc.size() );
    // Always return success
    return fapi2::FAPI2_RC_SUCCESS;
}
