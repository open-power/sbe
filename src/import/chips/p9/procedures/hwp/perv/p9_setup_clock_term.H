/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/perv/p9_setup_clock_term.H $ */
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
//------------------------------------------------------------------------------
/// @file  p9_setup_clock_term.H
///
/// @brief Setup the clock termination correctly for system/chip type
//------------------------------------------------------------------------------
// *HWP HW Owner        : Anusha Reddy Rangareddygari <anusrang@in.ibm.com>
// *HWP HW Backup Owner : Srinivas V Naga <srinivan@in.ibm.com>
// *HWP FW Owner        : Brian Silver <bsilver@us.ibm.com>
// *HWP Team            : Perv
// *HWP Level           : 3
// *HWP Consumed by     : FSP:HB
//------------------------------------------------------------------------------


#ifndef _P9_SETUP_CLOCK_TERM_H_
#define _P9_SETUP_CLOCK_TERM_H_


#include <fapi2.H>


namespace p9SetupClockTerm
{
enum P9_SETUP_CLOCK_TERM_Public_Constants
{
    OSCSWITCH_RC3 = 0x0080C000,
    OSCSWITCH_RC4 = 0x0,
    DISABLE_WRITE_PROTECTION = 0x4453FFFF,
    P9C_OSCSWITCH_RC3_BOTHSRC0 = 0x00009000,
    P9C_OSCSWITCH_RC3_BOTHSRC1 = 0x00006000,
    P9C_OSCSWITCH_RC3_SRC0 = 0x0000C000,
    P9C_OSCSWITCH_RC3_SRC1 = 0x00003000
};
}

typedef enum { BOTH_SRC0, BOTH_SRC1, SRC0, SRC1, SRC_NONE } pci_clk_req_enum;

//            pci_clk_req_enum          enumarated data type with allowed PCI refclock configurations
//                                      BOTH_SRC0:  redundant PCI clock available, SRC0 configured as primary source
//                                      BOTH_SRC1:  redundant PCI clock available, SRC1 configured as primary source
//                                      SRC0:       redundant PCI clock but forced to use SRC0 only as other clock may be garded/deconfigured
//                                      SRC1:       redundant PCI clock but forced to use SRC1 only as other clock may be garded/deconfigured
//                                      SRC_NONE:   non-redundant PCI clock setup



typedef fapi2::ReturnCode (*p9_setup_clock_term_FP_t)(const
        fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>&, const pci_clk_req_enum);

/// @brief Setup the clock termination correctly, since this is the first procedure run against the chips it also clears the GP write protect
///
/// @param[in]     i_target_chip   Reference to TARGET_TYPE_PROC_CHIP target
//                 pci_clk_req_enum  enumarated data type with allowed PCI refclock configurations
//                                      BOTH_SRC0:  redundant PCI clock available, SRC0 configured as primary source
//                                      BOTH_SRC1:  redundant PCI clock available, SRC1 configured as primary source
//                                      SRC0:       redundant PCI clock but forced to use SRC0 only as other clock may be garded/deconfigured
//                                      SRC1:       redundant PCI clock but forced to use SRC1 only as other clock may be garded/deconfigured
//                                      SRC_NONE:   non-redundant PCI clock setup
/// @return  FAPI2_RC_SUCCESS if success, else error code.
extern "C"
{
    fapi2::ReturnCode p9_setup_clock_term(const
                                          fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip, const pci_clk_req_enum  i_pci_clk_req = BOTH_SRC0 );
}

#endif
