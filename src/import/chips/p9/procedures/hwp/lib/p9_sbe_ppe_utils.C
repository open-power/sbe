/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/lib/p9_sbe_ppe_utils.C $   */
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
/// @file  p9_sbe_ppe_utils.C
/// @brief Commonly used PPE utilities on the SBE
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
/// @endverbatim

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <fapi2.H>
#include <p9_sbe_ppe_utils.H>
#include <p9_hcd_common.H>
#include <p9_misc_scom_addresses_fld.H>

//-----------------------------------------------------------------------------

uint32_t ppe_getMxsprInstruction ( const uint32_t i_opcode,
                                   const uint16_t i_Rs,
                                   const uint16_t i_Spr )
{
    uint32_t mxsprInstOpcode = 0;
    uint32_t temp = (( i_Spr & 0x03FF ) << 11);
    mxsprInstOpcode = ( temp  & 0x0000F800 ) << 5;
    mxsprInstOpcode |= ( temp & 0x001F0000 ) >> 5;
    mxsprInstOpcode |= i_opcode;
    mxsprInstOpcode |= ( i_Rs & 0x001F ) << 21;

    return mxsprInstOpcode;
}
//-----------------------------------------------------------------------------

fapi2::ReturnCode ppe_pollHaltState(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
    const uint64_t i_base_address)
{
    fapi2::buffer<uint64_t> l_data64;

    // Halt state entry should be very fast on PPEs (eg nanoseconds)
    // Try only using the SCOM access time to delay.
    uint32_t l_timeout_count = 20;

    do
    {
        FAPI_TRY(getScom ( i_target,
                           i_base_address + PPE_XIRAMDBG,
                           l_data64 ),
                 "Failed reading XIRAMDBG register!" );
    }
    while ((! l_data64.getBit<PU_PPE_XIRAMDBG_XSR_HS>()) &&
           (--l_timeout_count != 0));

    FAPI_ASSERT ((l_data64.getBit<PU_PPE_XIRAMDBG_XSR_HS>()),
                 fapi2::SBE_PPE_UTILS_HALT_TIMEOUT_ERR()
                 .set_TARGET(i_target)
                 .set_ADDRESS(i_base_address),
                 "PPE Halt Timeout!");

fapi_try_exit:
    return fapi2::current_err;
}
//-----------------------------------------------------------------------------

fapi2::ReturnCode ppe_halt(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
    const uint64_t i_base_address)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_TRY ( getScom ( i_target,
                         i_base_address + PPE_XIRAMDBG,
                         l_data64 ),
               "Error reading PPE Halt State" );

    // Halt the PPE only if it is not already halted
    if (! l_data64.getBit<PU_PPE_XIRAMDBG_XSR_HS>())
    {
        FAPI_INF("   Send HALT command via XCR...");
        l_data64.flush<0>().insertFromRight(p9hcd::HALT, PU_PPE_XIXCR_XCR,
                                            PU_PPE_XIXCR_XCR_LEN);

        FAPI_TRY(putScom(i_target, i_base_address + PPE_XIXCR, l_data64),
                 "Error in PUTSCOM in XCR to generate Halt condition");

        FAPI_TRY(ppe_pollHaltState(i_target, i_base_address));
    }

fapi_try_exit:
    return fapi2::current_err;
}
//-----------------------------------------------------------------------------

fapi2::ReturnCode ppe_RAMRead(
    const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
    const uint64_t i_base_address,
    const fapi2::buffer<uint64_t>& i_instruction,
    fapi2::buffer<uint32_t>& o_data)
{
    fapi2::buffer<uint64_t> l_data64;
    FAPI_TRY(ppe_pollHaltState(i_target, i_base_address));
    FAPI_TRY(fapi2::putScom(i_target, i_base_address + PPE_XIRAMEDR, i_instruction));
    FAPI_TRY(ppe_pollHaltState(i_target, i_base_address));
    FAPI_TRY(fapi2::getScom(i_target, i_base_address + PPE_XIRAMDBG, l_data64),
             "Error in GETSCOM");
    l_data64.extractToRight(o_data, 32, 32);
    FAPI_DBG("    RAMREAD o_data: 0X%08X", o_data);

fapi_try_exit:
    return fapi2::current_err;
}
//-----------------------------------------------------------------------------
