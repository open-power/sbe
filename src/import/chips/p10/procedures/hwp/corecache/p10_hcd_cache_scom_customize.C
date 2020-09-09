/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p10/procedures/hwp/corecache/p10_hcd_cache_scom_customize.C $ */
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
/// @file  p10_hcd_cache_scom_customize.C
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

#include "p10_hcd_cache_scom_customize.H"
#include "p10_hcd_common.H"
#ifdef __PPE_QME
    #include "p10_hcd_memmap_qme_sram.H"
    #include "ppehw_common.h"
#endif

//------------------------------------------------------------------------------
// Constant Definitions
//------------------------------------------------------------------------------

// Misc constants for HWP
enum
{
    VALID_L3_SCOM_RESTORE       =  0x80010000,
    VALID_L2_SCOM_RESTORE       =  0x80020000,
    REGION_SELECT_MASK          =  0x0000F000,
    SHIFT_RIGHT_CORE_SELECT     =  0x0c,
    LAST_SCOM_RESTORE_ENTRY     =  0x40000000,
    SCOM_RESTORE_HEADER         =  24,
};

//------------------------------------------------------------------------------
// Procedure: p10_hcd_cache_scom_customize
//------------------------------------------------------------------------------

fapi2::ReturnCode
p10_hcd_cache_scom_customize(
    const fapi2::Target < fapi2::TARGET_TYPE_CORE | fapi2::TARGET_TYPE_MULTICAST > & i_target,
    RESTORE_ENTRY_TYPE i_entryType )
{
    FAPI_INF(">>p10_hcd_cache_scom_customize");

//This procedure is limited to istep15/16 and QME Platform, it holds no function for istep4 or cronus
#ifdef __PPE_QME

    uint32_t l_core_select    = i_target.getCoreSelect();
    uint32_t l_scom_entry     = 0;
    uint32_t l_max_scom_entry = 0;
    uint32_t l_mask_address   = 0;
    uint32_t l_validEntry     = 0;
    uint32_t l_offset         = 0;
    uint64_t l_scom_data      = 0;
    ScomEntry_t* pQmeScomRes  = 0;
    QmeHeader_t* pQmeImgHdr   = (QmeHeader_t*)(QME_SRAM_HEADER_ADDR);

    if( CACHE_RESTORE_ENTRY == i_entryType )
    {
        l_max_scom_entry  = pQmeImgHdr->g_qme_L3ScomLength;
        l_validEntry      = VALID_L3_SCOM_RESTORE;
        l_offset          = 0;
    }
    else
    {
        l_max_scom_entry  = pQmeImgHdr->g_qme_coreL2ScomLength;
        l_validEntry      = VALID_L2_SCOM_RESTORE;
        l_offset          = SCOM_RESTORE_HEADER;
    }

    l_max_scom_entry = ( l_max_scom_entry / SCOM_RESTORE_ENTRY_SIZE );

    fapi2::Target < fapi2::TARGET_TYPE_SYSTEM > l_sys;
    pQmeScomRes = (ScomEntry_t*)( QME_SRAM_BASE_ADDR + pQmeImgHdr->g_qme_scom_offset + l_offset );

    FAPI_DBG( "p10_hcd_cache_scomcust core[%d] scom_offset [%08X] size = %d",
              l_core_select, ( QME_SRAM_BASE_ADDR + ( pQmeImgHdr->g_qme_scom_offset )),
              sizeof(ScomEntry_t));

    for( l_scom_entry = 0; l_scom_entry < l_max_scom_entry; l_scom_entry++, pQmeScomRes++ )
    {
        //Restore relevant and valid Core-L2/ L3 SCOM restore entry by
        //checking validity bit, region select and core select
        if( !( pQmeScomRes->scomEntryAddress & l_validEntry ))
        {
            continue;
        }

        if( l_core_select == (( pQmeScomRes->scomEntryAddress & REGION_SELECT_MASK )  >> SHIFT_RIGHT_CORE_SELECT ))
        {
            l_mask_address  =  pQmeScomRes->scomEntryAddress & 0x000FFFFF;
            l_scom_data     =  pQmeScomRes->scomEntryData;
            FAPI_DBG("scom[%d] addr[%x] data[%08X%08X]",
                     l_scom_entry, l_mask_address, ( l_scom_data >> 32 ),
                     ( l_scom_data & 0xFFFFFFFF ) );

            // Only scom the intended core
            PPE_PUTSCOM(l_mask_address, l_scom_data );

        } //if( l_core_select ==

        if( pQmeScomRes->scomEntryAddress & LAST_SCOM_RESTORE_ENTRY )
        {
            break;
        }

    } //for( l_scom_entry = 0;

#endif
    FAPI_INF("<<p10_hcd_cache_scom_customize");

    return fapi2::current_err;
}
