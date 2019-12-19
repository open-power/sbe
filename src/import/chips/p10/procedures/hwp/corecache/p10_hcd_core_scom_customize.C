/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_hcd_core_scom_customize.C $ */
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
/// @file  p10_hcd_core_scom_customize.C
/// @brief
///


// *HWP HWP Owner          : David Du         <daviddu@us.ibm.com>
// *HWP Backup HWP Owner   : Greg Still       <stillgs@us.ibm.com>
// *HWP FW Owner           : Prem Shanker Jha <premjha2@in.ibm.com>
// *HWP Team               : PM
// *HWP Consumed by        : SBE:QME
// *HWP Level              : 1


//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "p10_hcd_core_scom_customize.H"
#include "p10_hcd_common.H"

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Procedure: p10_hcd_core_scom_customize
//------------------------------------------------------------------------------


fapi2::ReturnCode
p10_hcd_core_scom_customize(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST > & i_target)
{
    FAPI_INF(">>p10_hcd_core_scom_customize");

//This procedure is limited to istep15/16 and QME Platform, it holds no function for istep4 or cronus
#ifdef __PPE_QME

#ifndef HCD_QME_SKIP_BCE_SCOM

#include "p10_hcd_memmap_qme_sram.H"
#include "ppehw_common.h"

    uint32_t l_core_select   = i_target.getCoreSelect();
    uint32_t l_core_skip     = 0;
    uint32_t l_core_mask     = 0;
    uint32_t l_scom_entry    = 0;
    ScomEntry_t* pQmeScomRes = 0;
    QmeHeader_t* pQmeImgHdr  = (QmeHeader_t*)(QME_SRAM_HEADER_ADDR);

    fapi2::Target < fapi2::TARGET_TYPE_SYSTEM > l_sys;
    fapi2::ATTR_CONTAINED_IPL_TYPE_Type         l_attr_contained_ipl_type;
    FAPI_TRY( FAPI_ATTR_GET( fapi2::ATTR_CONTAINED_IPL_TYPE, l_sys, l_attr_contained_ipl_type ) );

    if( l_attr_contained_ipl_type != ENUM_ATTR_CONTAINED_IPL_TYPE_CACHE )
    {
        for (l_core_mask = 8; l_core_mask != 0; l_core_mask--, l_core_skip += pQmeImgHdr->g_qme_scom_core_length)
        {
            if (l_core_select & l_core_mask)
            {
                pQmeScomRes = (ScomEntry_t*)(QME_SRAM_BASE_ADDR +
                                             pQmeImgHdr->g_qme_scom_core_offset + l_core_skip);

                FAPI_DBG("p9_hcd_core_scomcust core[%d] scom_offset [%08X] size = %d",
                         l_core_mask,
                         (QME_SRAM_BASE_ADDR + (pQmeImgHdr->g_qme_scom_offset << 5) + core_offset),
                         sizeof(ScomEntry_t));

                for(l_scom_entry = 0;
                    (pQmeScomRes->scomEntryTag & 0x4000); //tag.last_entry
                    l_scom_entry++, pQmeScomRes++)
                {
                    if( pQmeScomRes->scomEntryTag & 0x8000 ) //tag.valid_entry, skip invalid until last entry
                    {
                        FAPI_DBG("scom[%d] addr[%x] data[%08X%08X]",
                                 l_scom_entry, pQmeScomRes->scomEntryAddress,
                                 (pQmeScomRes->scomEntryData >> 32), (pQmeScomRes->scomEntryData & 0xFFFFFFFF));

                        // Only scom the intended core
                        QME_PUTSCOM(PPE_SCOM_ADDR_MC_WR(pQmeScomRes->scomEntryAddress, l_core_mask),
                                    pQmeScomRes->scomEntryData);
                    }
                }
            }
        }
    }

#endif

#endif

    FAPI_INF("<<p10_hcd_core_scom_customize");
    return fapi2::current_err;
}
