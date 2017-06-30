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
/// *HWP Level           : 1
/// *HWP Consumed by     : SBE
///
/// @verbatim
///
/// Procedure Summary:
///   - Reads a PPE's minimalistic state in terms of select XIRs and SPRs
///
/// @endverbatim
#if 0

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <fapi2.H>
#include <p9_sbe_ppe_ffdc.H>
#include <p9_sbe_ppe_utils.H>

//-----------------------------------------------------------------------------
/**
 * @brief Collectes PPE XIRs and SPRs
 * @param[in]    i_target        Chip Target
 * @param[in]    i_base_address  Base address of the PPE to be used for access
 * @param[inout] io_v_ppe_xirs   vector of PPE XIRs collected
 *               On input, initialized with XIR_IDX_MAX elements defaulted
 *               On ouptut, elements updated with actual XIR values read
 * @param[inout] io_v_ppe_sprs   vector of PPE SPRs collected
 *               On input, initialized with SPR_IDX_MAX elements defaulted
 *               On output, elements updated with actual SPR values read
 * @return       FAPI2_RC_SUCCESS
 */
fapi2::ReturnCode
p9_sbe_ppe_ffdc (
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
    const uint64_t i_base_address,
    std::vector<uint64_t>& io_v_ppe_xirs,
    std::vector<uint32_t>& io_v_ppe_sprs )
{
    fapi2::ReturnCode l_rc;
    FAPI_INF(">> p9_sbe_ppe_ffdc: Base Addr: 0x%08llX", i_base_address);

    fapi2::buffer<uint64_t> l_raminstr;
    fapi2::buffer<uint64_t> l_data64;
    fapi2::buffer<uint32_t> l_data32;
    bool l_ppe_halt_state = false;

    // Halt the PPE before grabbing the state
    // Ignore a fail here, the halted state is checked after the XIR loop
    l_rc = ppe_halt(i_target, i_base_address);

    FAPI_INF ("Collecting XIRs");

    for (uint8_t i = XIR_IDX_IAR_XSR; i < XIR_IDX_MAX; ++i)
    {
        uint64_t l_address = 0;

        switch (i)
        {
            case XIR_IDX_IAR_XSR:
                l_address = i_base_address + PPE_XIDBGPRO;
                break;

            case XIR_IDX_EDR_IR:
                l_address = i_base_address + PPE_XIRAMEDR;
                break;

            case XIR_IDX_SPRG0:
                l_address = i_base_address + PPE_XIRAMDBG;
                break;
        }

        l_rc = getScom (i_target, l_address, l_data64);

        if (l_rc == fapi2::FAPI2_RC_SUCCESS)
        {
            io_v_ppe_xirs[i] = l_data64;
        }

        // On error, we already have the default on input, continue to next

        FAPI_INF ( "XIR Idx %d Addr 0x%.8X Val: 0x%16llX",
                   i, l_address, io_v_ppe_xirs[i]);
    } // XIRs Loop

    // Optimizing getScom of i_base_address + PPE_XIRAMDBG as we already have
    // it from the last XIR_ID_SPRG0 iteration in the above XIR loop
    l_ppe_halt_state = (l_data64.getBit(0, 1)) ? true : false;

    // Get SPRs if PPE halted
    if (l_ppe_halt_state)
    {
        FAPI_INF ("Collecting SPRs");

        for (uint8_t i = 0; i < SPR_IDX_MAX; ++i)
        {
            uint16_t l_sprNr = 0;

            switch (i)
            {
                case SPR_IDX_LR:
                    l_sprNr = LR;
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
                ppe_getMfsprInstruction(0, l_sprNr), 0, 32 );

            // TODO Do we need this for every SPR RAM access?
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
                FAPI_ERR ("putScom XIRAMEDR fail!. Idx %d SPR#%d Inst 0x%16llX",
                          i, l_sprNr, l_raminstr);
                continue; // try ramming out the next SPR
            }

            // R0 to SPRG0
            l_raminstr.flush<0>().insertFromRight (
                ppe_getMtsprInstruction(0, SPRG0), 0, 32 );

            l_rc = ppe_RAMRead(i_target, i_base_address, l_raminstr, l_data32);

            if (l_rc == fapi2::FAPI2_RC_SUCCESS)
            {
                io_v_ppe_sprs[i] = l_data32;
            }
            else
            {
                FAPI_ERR ("ppe_RAMRead fail! Idx %d SPR#%d Inst 0x%16llX",
                          i, l_sprNr, l_raminstr);
                continue; // try ramming out the next SPR
            }

            FAPI_INF ( "Idx %d SPR# %d Value= 0x%.8X",
                       i, l_sprNr, io_v_ppe_sprs[i] );
        }   // SPR Loop
    }
    // else .. exit with defaulted SPR values, as PPE is not halted
    else // TODO cleanup after test, convert FAPI_INF to FAPI_DBG, etc.
    {
        FAPI_INF("PPE is not Halted .. not collecting SPRs");
    }

    FAPI_INF ("<< p9_sbe_ppe_ffdc");
    // Always return success
    return fapi2::FAPI2_RC_SUCCESS;
}

#endif

