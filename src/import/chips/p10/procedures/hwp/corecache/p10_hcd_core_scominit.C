/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_hcd_core_scominit.C $ */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019                             */
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
/// @file  p10_hcd_core_scominit.C
/// @brief
///

///
/// *HWP HW Maintainer: David Du         <daviddu@us.ibm.com>
/// *HWP FW Maintainer: Prem Shanker Jha <premjha2@in.ibm.com>
/// *HWP Consumed by  : SBE,QME
///

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "p10_hcd_core_scominit.H"
#include <p10_scom_c.H>
#include <p10_fbc_utils.H>


//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Procedure: p10_hcd_core_scominit
//------------------------------------------------------------------------------

namespace
{
///
/// @brief Initialize the L2 topology id table entries
/// @param[in] c                Reference to core target
/// @param[in] topo_scoms       Vector where each element is the content to write
///                             into the topology id table SCOM register.
///                             topo_scoms[0] contains reg value for entries  0.. 7
///                             topo_scoms[1] contains reg value for entries  8..15
///                             topo_scoms[2] contains reg value for entries 16..23
///                             topo_scoms[3] contains reg value for entries 24..31
///                             assert(topo_scoms.size() == 4)
/// @return fapi::ReturnCode    FAPI2_RC_SUCCESS on success, error otherwise
///
fapi2::ReturnCode init_topo_id_tables(const fapi2::Target < fapi2::TARGET_TYPE_CORE
                                      | fapi2::TARGET_TYPE_MULTICAST > & c,
                                      const std::vector<uint64_t>& topo_scoms)
{
    using namespace scomt::c;

    PREP_L2_L2MISC_L2CERRS_TOPOTABLE0(c);
    FAPI_TRY(PUT_L2_L2MISC_L2CERRS_TOPOTABLE0(c, topo_scoms[0]));
    PREP_L2_L2MISC_L2CERRS_TOPOTABLE1(c);
    FAPI_TRY(PUT_L2_L2MISC_L2CERRS_TOPOTABLE1(c, topo_scoms[1]));
    PREP_L2_L2MISC_L2CERRS_TOPOTABLE2(c);
    FAPI_TRY(PUT_L2_L2MISC_L2CERRS_TOPOTABLE2(c, topo_scoms[2]));
    PREP_L2_L2MISC_L2CERRS_TOPOTABLE3(c);
    FAPI_TRY(PUT_L2_L2MISC_L2CERRS_TOPOTABLE3(c, topo_scoms[3]));

fapi_try_exit:
    return fapi2::current_err;
}
};

fapi2::ReturnCode
p10_hcd_core_scominit(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST > & i_target)
{
    std::vector<uint64_t> l_topo_scoms;
    fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP> l_chip = i_target.getParent<fapi2::TARGET_TYPE_PROC_CHIP>();

    FAPI_INF(">>p10_hcd_core_scominit");

    // Get the register values for the SCOMs to setup the topology id table
    FAPI_TRY(topo::get_topology_table_scoms(l_chip, l_topo_scoms));
    // Setup the topology id tables for L2 via multicast
    FAPI_TRY(init_topo_id_tables(i_target, l_topo_scoms));

fapi_try_exit:
    FAPI_INF("<<p10_hcd_core_scominit");
    return fapi2::current_err;
}
