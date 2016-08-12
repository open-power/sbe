/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: import/chips/p9/procedures/hwp/perv/p9_sbe_setup_boot_freq.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
/// @file  p9_sbe_setup_boot_freq.C
/// @brief Setup Boot Frequency
///
// *HW Owner    : Sudheendra K Srivathsa <sudheendraks@in.ibm.com>
// *FW Owner    : Sangeetha T S <sangeet2@in.ibm.com>
// *Team        : PM
// *Consumed by : SBE
// *Level       : 2
///
/// @verbatim
///
/// Procedure Summary:
///   - Read frequency ATTR and write to the Quad PPM DPLL Freq Ctrl register
///
/// @endverbatim

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <fapi2.H>
#include "p9_sbe_setup_boot_freq.H"
#include "p9_quad_scom_addresses.H"

enum P9_SBE_SETUP_BOOT_FREQ_CONSTANTS
{

// Default configuration settings

// Default boot_frequency in terms of a multiplier of the refclk frequency/8
// This is value used if the mailbox value is zero
//
// Value implemented is 3.0GHz, @todo, RTC 140053 - Should it be 2 GHz for P9 ?
//
// 3000MHz / 16.667MHz = ~180 => 0xB4
//
// Note:  the above is aligned, as a value, to 0:10, written as bits 17:27 of PPM DPLL freq ctrl register
// Bits 0:7 are DPLL.MULT_INTG(0:7), and Bits 8:10 are DPLL.MULT_FRAC(0:2)
//
    DEFAULT_BOOT_FREQUENCY_MULTIPLIER =  0x00B4,

};

//-----------------------------------------------------------------------------
// Procedure
//-----------------------------------------------------------------------------

fapi2::ReturnCode
BootFreqInitAttributes(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
                       uint16_t& i_boot_frequency_multiplier)
{

    i_boot_frequency_multiplier = DEFAULT_BOOT_FREQUENCY_MULTIPLIER;

    FAPI_TRY(FAPI_ATTR_GET(fapi2::ATTR_BOOT_FREQ_MULT, i_target, i_boot_frequency_multiplier));

    // If attribute values are zero, use the default values (hardcoded)

    // check BOOT FREQ MULT
    if (i_boot_frequency_multiplier == 0)
    {
        // Default voltage if mailbox value is not set

        // @todo, L3 phase Eventually, this should replaced with an error point
        // to indicate that the mailbox -> attributes haven't been setup

        i_boot_frequency_multiplier  = DEFAULT_BOOT_FREQUENCY_MULTIPLIER;
        FAPI_INF("DPLL boot frequency not set in attributes.  Setting to default of %d (%x)",
                 i_boot_frequency_multiplier, i_boot_frequency_multiplier);
    }
    else
    {
        FAPI_INF("DPLL boot frequency = %d (%x)",
                 i_boot_frequency_multiplier, i_boot_frequency_multiplier);
    }

fapi_try_exit:
    return fapi2::current_err;
} // BootFreqInitAttributes


fapi2::ReturnCode
setDPLLFrequency(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target,
                 const uint16_t i_DpllBootFreqMult
                )
{
    fapi2::buffer<uint64_t> l_data;


    auto l_present_eqs = i_target.getChildren<fapi2::TARGET_TYPE_EQ>(fapi2::TARGET_STATE_FUNCTIONAL);

    l_data.insertFromRight<17, 11>(i_DpllBootFreqMult);

    for(auto l_tlst : l_present_eqs)
    {
        FAPI_TRY(fapi2::putScom(l_tlst,  EQ_QPPM_DPLL_FREQ, l_data));
        //@todo,Determine ff_slew rate value RTC 140053
        FAPI_TRY(fapi2::putScom(l_tlst,  EQ_QPPM_DPLL_CTRL,  0));

    }

fapi_try_exit:
    return fapi2::current_err;
}


// Hardware procedure
fapi2::ReturnCode
p9_sbe_setup_boot_freq(const fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target)
{
    // Boot frequency variable
    uint16_t l_boot_frequency_multiplier;

    // Read Boot freq mult attribute
    FAPI_TRY(BootFreqInitAttributes(i_target, l_boot_frequency_multiplier));

    // Set Boot Frequency

    FAPI_TRY(setDPLLFrequency(i_target,
                              l_boot_frequency_multiplier),
             "Setting Boot Frequency");

fapi_try_exit:
    return fapi2::current_err;

} // Procedure
