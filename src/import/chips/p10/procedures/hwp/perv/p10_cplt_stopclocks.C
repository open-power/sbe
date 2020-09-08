/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/perv/p10_cplt_stopclocks.C $ */
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
//------------------------------------------------------------------------------
/// @file  p10_cplt_stopclocks.C
///
/// @brief stop clocks for Nest, PCI, MC, PAU, IOHS
//------------------------------------------------------------------------------
// *HWP HW Maintainer   : Anusha Reddy (anusrang@in.ibm.com)
// *HWP FW Maintainer   : Raja Das (rajadas2@in.ibm.com)
// *HWP Consumed by     : FSP:HB
//------------------------------------------------------------------------------


#include "p10_cplt_stopclocks.H"
#include <p10_perv_sbe_cmn.H>
#include <p10_common_stopclocks.H>
#include <target_filters.H>

enum P10_CPLT_STOPCLOCKS_Private_Constants
{
    CLOCK_CMD = 0x2,
    CLOCK_TYPES = 0x7,
    DONT_STARTMASTER = 0x0,
    DONT_STARTSLAVE = 0x0,
    REGIONS_ALL_INCLUDING_PLL = 0x7FFF,
};

static fapi2::ReturnCode p10_chiplet_stopclocks(const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet);

fapi2::ReturnCode p10_cplt_stopclocks(const
                                      fapi2::Target<fapi2::TARGET_TYPE_PROC_CHIP>& i_target_chip,
                                      const bool i_stop_nest,
                                      const bool i_stop_pci ,
                                      const bool i_stop_mc  ,
                                      const bool i_stop_pau ,
                                      const bool i_stop_axon )
{
    fapi2::ReturnCode l_rc;

    fapi2::TargetFilter l_io_filter = fapi2::TARGET_FILTER_NONE;

    FAPI_INF("Entering ...");

#ifndef __PPE__
    FAPI_INF("p10_cplt_stopclocks: Input arguments \n\t i_stop_nest  = %s \n\t i_stop_pci = %s \n\t i_stop_mc  = %s \n\t i_stop_pau = %s \n\t i_stop_axon = %s",
             btos(i_stop_nest), btos(i_stop_pci), btos(i_stop_mc), btos(i_stop_pau),  btos(i_stop_axon));
#endif


    if (i_stop_pci)
    {
        l_io_filter = static_cast<fapi2::TargetFilter>(l_io_filter |
                      fapi2::TARGET_FILTER_ALL_PCI);
    }

    if (i_stop_mc)
    {
        l_io_filter = static_cast<fapi2::TargetFilter>(l_io_filter |
                      fapi2::TARGET_FILTER_ALL_MC);
    }

    if(i_stop_pau)
    {

        l_io_filter = static_cast<fapi2::TargetFilter>(l_io_filter |
                      fapi2::TARGET_FILTER_ALL_PAU);
    }

    if(i_stop_axon)
    {

        l_io_filter = static_cast<fapi2::TargetFilter>(l_io_filter |
                      fapi2::TARGET_FILTER_ALL_IOHS);
    }


    for (auto l_trgt_chplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
         (l_io_filter, fapi2::TARGET_STATE_FUNCTIONAL))
    {
        l_rc = p10_chiplet_stopclocks(l_trgt_chplt);

        FAPI_ASSERT_NOEXIT(l_rc == fapi2::FAPI2_RC_SUCCESS,
                           fapi2::CPLT_STOPCLOCKS_ERR()
                           .set_TARGET_CHIPLET(l_trgt_chplt),
                           "p10_cplt_stopclocks returned error");
    }

    // Nest chiplet to be clock stopped last.
    if (i_stop_nest)
    {
        for (auto l_nest_chplt : i_target_chip.getChildren<fapi2::TARGET_TYPE_PERV>
             (static_cast<fapi2::TargetFilter>(fapi2::TARGET_FILTER_ALL_NEST),
              fapi2::TARGET_STATE_FUNCTIONAL))
        {
            l_rc = p10_chiplet_stopclocks(l_nest_chplt);

            FAPI_ASSERT_NOEXIT(l_rc == fapi2::FAPI2_RC_SUCCESS,
                               fapi2::CPLT_STOPCLOCKS_ERR()
                               .set_TARGET_CHIPLET(l_nest_chplt),
                               "p10_cplt_stopclocks returned error");
        }
    }

    FAPI_INF("Exiting ...");

    return fapi2::FAPI2_RC_SUCCESS;

}

/// @brief -- Stopclocks for chiplets
///        -- Check if chiplet is accessible,Raise chiplet fence, Call clock_start_stop module,
///           Set vital fence and flushmode, raise pg fences
///
///
/// @param[in]     i_target_chiplet   Reference to TARGET_TYPE_PERV target
static  fapi2::ReturnCode p10_chiplet_stopclocks(const fapi2::Target<fapi2::TARGET_TYPE_PERV>& i_target_chiplet)
{
    bool l_chiplet_accessible = 0;

    FAPI_INF("Entering ...");

    FAPI_TRY(p10_common_stopclock_chiplet_accessible(i_target_chiplet, l_chiplet_accessible));

    if(l_chiplet_accessible)
    {
        FAPI_DBG("Raise chiplet fence");
        FAPI_TRY(p10_common_stopclocks_raise_fence(i_target_chiplet));

        FAPI_DBG("Call module clock start stop for nest chiplets ");
        FAPI_TRY(p10_perv_sbe_cmn_clock_start_stop(i_target_chiplet, CLOCK_CMD,
                 DONT_STARTSLAVE, DONT_STARTMASTER, REGIONS_ALL_INCLUDING_PLL, CLOCK_TYPES));

        FAPI_DBG("Set vital fence and flushmode");
        FAPI_TRY(p10_common_stopclocks_set_vitalfence_flushmode(i_target_chiplet));

        FAPI_DBG("Call p10_common_stopclocks_cplt_ctrl_action_function ");
        FAPI_TRY(p10_common_stopclocks_cplt_ctrl_action_function(i_target_chiplet, REGIONS_ALL_INCLUDING_PLL));
    }

    FAPI_INF("Exiting ...");

fapi_try_exit:
    return fapi2::current_err;
}
