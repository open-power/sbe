/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_hcd_cache_arrayinit.C $ */
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
/// @file  p10_hcd_cache_arrayinit.C
/// @brief Perform array initialization to the EQ cache region
///


// *HWP HWP Owner          : David Du         <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still       <stillgs@us.ibm.com>
// *HWP FW Owner           : Prem Shanker Jha <premjha2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:QME
// *HWP Level              : 2


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "p10_hcd_cache_arrayinit.H"
#include "p10_hcd_common.H"

#ifdef __PPE_QME
    #include "p10_scom_eq.H"
    using namespace scomt::eq;
#else
    #include "p10_scom_eq.H"
    using namespace scomt::eq;
#endif

#if !defined P10_HCD_CORECACHE_SKIP_ARRAY || !defined P10_HCD_CORECACHE_SKIP_FLUSH
    #include <p10_perv_sbe_cmn.H>
#endif

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

enum P10_HCD_CACHE_ARRAYINIT_CONSTANTS
{
    LOOP_COUNTER = 0x0000000000042FFF,
    START_ABIST_MATCH_VALUE = 0x0000000F00000000
};

//------------------------------------------------------------------------------
// Procedure: p10_hcd_cache_arrayinit
//------------------------------------------------------------------------------


fapi2::ReturnCode
p10_hcd_cache_arrayinit(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > & i_target)
{
    fapi2::Target < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > eq_target =
        i_target.getParent < fapi2::TARGET_TYPE_EQ | fapi2::TARGET_TYPE_MULTICAST > ();
#if !defined P10_HCD_CORECACHE_SKIP_ARRAY || !defined P10_HCD_CORECACHE_SKIP_FLUSH
    fapi2::Target < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST, fapi2::MULTICAST_AND > perv_target =
        eq_target.getParent < fapi2::TARGET_TYPE_PERV | fapi2::TARGET_TYPE_MULTICAST > ();
    uint32_t                l_regions  = i_target.getCoreSelect();
#endif

    fapi2::buffer<uint64_t> l_scomData = 0;

    FAPI_INF(">>p10_hcd_cache_arrayinit");

    FAPI_DBG("Assert sdis_n(flushing LCBES condition) via CPLT_CONF0[34]");
    FAPI_TRY( HCD_PUTSCOM_Q( eq_target, CPLT_CONF0_WO_OR, SCOM_1BIT(34) ) );

#ifndef P10_HCD_CORECACHE_SKIP_ARRAY

    //Here explains how to prepare l_regions variable
    //for the perv/sbe submodule of arrayinit and scan0
    //
    //Below is the pseudo code of submodule that processes
    //i_regions input paramter to 64bit scan region register data
    //
    //  i_regions.extractToRight<1, 15>(l_regions);
    //  data64.insertFromRight<4, 15>(l_regions);
    //
    //Note only 15bits length of data is extracted and inserted <--- very important
    //
    //Also note, the insert and extract do indicate starting bit and length
    //but how they actually work is to align LSB, which is starting+length
    //then take the limited length from LSB to starting bit
    //Such function do NOT align on starting bit and truncate from
    //starting bit to length, which is confusion that people may have
    //intuitively due to given input parameters are starting and length
    //
    //In another word, what they really are underneth is this
    //  (data & length_mask) << shift_amount_to_the_field
    //For example, in the (4,15) case, it would be
    //  (data & 0xEFFF) << (63-19) // get masked value to bit19
    //It would be very illogicall if they align on MSB and truncate/mask
    //data on lower bits, because then you would ALWAYS change the data value
    //if your field is limited field like 3bits or 5bits field from even uint8_t
    //
    //Also recall, the big-endian/little-endian is about how data being stored on hw
    //the compiler would always try to retain the value of the data as in decimal when
    //downcasting like (uint16_t)var32 or the shift between 8/16/32/64b container happen
    //
    //So here is what submodule does in net:
    //
    //1) submodule() takes an uint16_t container i_regions as input parameter
    //2) submodule() cuts top bit off from the 16-bit container(see above)
    //3) submodule() then shift and align the value on LSB to starting bit
    //   the bit1 of the container to bit4 of scan region register
    //   the bit2 of the container to bit5 of scan region register
    //   ...
    //   the bit14 of the container to bit17 of scan region register
    //   the bit15 of the container to bit18 of scan region register
    //   <end of the container>
    //
    //Here is the complete map:
    //
    //x x  x  x  x  x  x   x   x   x   x   x   C0  C1  C2  C3 core_select
    //0 1  2  3  4  5  6   7   8   9   10  11  12  13  14  15 uint16_t container
    //x 4  5  6  7  8  9   10  11  12  13  14  15  16  17  18 scan region register
    //X Pv C0 C1 C2 C3 L30 L31 L32 L33 QME ADJ MA0 MA1 MA2 MA3
    //
    //Now I need to shift C3  to bit5 postion of l_regions(then C0-2 would be right)
    //              shift L33 to bit9 postion of l_regions(then L30-2 would be right)
    //
    //My SHIFT16(b) Marco is actually just (15-b)
    //where b is the least-significant bit of my data to
    //the location that I am trying to align to in destination data
    //and 15-b will give the shift amount to make it right in 16b container
    //likewise SHIFT32/64 is for 32b container and 64b container respectively
    //
    //For example, I want to shift C3 to bit5
    //my macro would just be SHIFT16(5)
    //15-b=15-5=10, which means, we shift left by 10
    //from bit15(the end of the data), that moves us to bit5.
    //
    //The usage of SHIFT16/32/64(b) marco is that
    //you know where you want your LSB to be,
    //then just let (b) be that number.
    //
    //It is essentially the same as the fapi.insterFromRight(4, 15)
    //which does shift value from LSB/Right to location of 4+15=19
    //then only take data value between bit4 and bit19 at new data
    //or bit1 and bit15 from the given data.
    //While here I only shift, but not truncate or limit data length
    //unless there is additional integer type casting or custom masking
    //(then it would be the same as fapi insert)

    FAPI_DBG("Arrayinit selected L3 regions");
    FAPI_TRY(p10_perv_sbe_cmn_array_init_module(perv_target,
             (l_regions << SHIFT16(9)),
             LOOP_COUNTER,
             START_ABIST_MATCH_VALUE,
             false));

#endif

#ifndef P10_HCD_CORECACHE_SKIP_FLUSH

    //--------------------------------------------
    // perform scan0 module for pervasive chiplet
    //--------------------------------------------
    // Each scan0 will rotate the ring 8191 latches (2**13 - 1) and the longest
    // ring is defined by P9_HCD_SCAN_FUNC_REPEAT. When the design ALWAYS has
    // all stumps less than 8191, the loop can be removed.

    FAPI_DBG("Scan0 region:l3 type:all_but_gptr_repr_time rings");

    for(uint32_t l_loop = 0; l_loop < P10_HCD_SCAN0_FUNC_REPEAT; l_loop++)
        FAPI_TRY(p10_perv_sbe_cmn_scan0_module(perv_target,
                                               (l_regions << SHIFT16(9)),
                                               HCD_SCAN0_TYPE_ALL_BUT_GPTR_REPR_TIME));

#endif

    FAPI_DBG("Drop sdis_n(flushing LCBES condition) via CPLT_CONF0[34]");
    FAPI_TRY( HCD_PUTSCOM_Q( eq_target, CPLT_CONF0_WO_CLEAR, SCOM_1BIT(34) ) );

fapi_try_exit:

    FAPI_INF("<<p10_hcd_cache_arrayinit");

    return fapi2::current_err;

}
