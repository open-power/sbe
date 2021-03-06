/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdiplcontrol.C $                      */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2019                        */
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
/*
 * @file: ppe/sbe/sbefw/sbecmdiplcontrol.C
 *
 * @brief This file contains the SBE istep chipOps
 *
 */
#include "sbecmdiplcontrol.H"
#include "sbefifo.H"
#include "sbetrace.H"
#include "sbe_sp_intf.H"
#include "sbeFifoMsgUtils.H"
#include "assert.h"
#include "sberegaccess.H"
#include "sbestates.H"
#include "sbecmdcntrldmt.H"
#include "sbeglobals.H"
// TODO Workaround
#include "plat_target_parms.H"

#include "p9_misc_scom_addresses.H"
#include "p9_perv_scom_addresses.H"
#include "p9_perv_scom_addresses_fld.H"
#include "p9n2_quad_scom_addresses.H"

#include "p9_suspend_io.H"

#include <p9_sbe_attr_setup.H>

using namespace fapi2;

static const uint32_t PEC_PHB_IOVALID_BIT_SHIFT = 59;
static const uint64_t PEC_PHB_IOVALID_BIT_MASK = 0x1ULL;

#ifdef __SBEFW_SEEPROM__
/* ----------------------------------- start SEEPROM CODE */
//Utility function to mask special attention
//----------------------------------------------------------------------------
ReturnCode maskSpecialAttn( const Target<TARGET_TYPE_CORE>& i_target )
{
#define SBE_FUNC "maskSpecialAttn "
    SBE_ENTER(SBE_FUNC);
    ReturnCode rc = FAPI2_RC_SUCCESS;
    do
    {
        uint64_t maskData = 0;
        const  uint64_t ecMask = 0xffc0000000000000;
        rc = getscom_abs_wrap (&i_target, P9N2_EX_SPA_MASK, &maskData );
        if( rc )
        {
            SBE_ERROR(SBE_FUNC" Failed to read P9N2_EX_SPA_MASK");
            break;
        }
        maskData = maskData | ecMask;
        rc = putscom_abs_wrap (&i_target, P9N2_EX_SPA_MASK, maskData );
        if( rc )
        {
            SBE_ERROR(SBE_FUNC" Failed to write P9N2_EX_SPA_MASK");
            break;
        }
    }while(0);
    SBE_EXIT(SBE_FUNC);
    return rc;
#undef SBE_FUNC
}
/* end SEEPROM CODE ----------------------------- */
#endif // ifdef __SBEFW_SEEPROM__

#ifndef __SBEFW_SEEPROM__
/* ----------------------------------- start PIBMEM CODE */
p9_suspend_io_FP_t p9_suspend_io_hwp = &p9_suspend_io;

//----------------------------------------------------------------------------
ReturnCode performTpmReset()
{
    #define SBE_FUNC "performTpmReset "
    SBE_ENTER(SBE_FUNC);
    ReturnCode rc = FAPI2_RC_SUCCESS;
    do
    {
        constexpr uint64_t tpmBitMask = 0x0008000000000000ULL;
        plat_target_handle_t tgtHndl;
        uint64_t regData = 0;

        // Clear TPM deconfig bit here.
        // It is possible that PHYP did TPM deconfig. start cbs clears this
        // bit. But in MPIPL this bit is not cleared, so HB IPL will fail.
        // bit 12 of PU_SECURITY_SWITCH_REGISTER_SCOM1 clear TPM deconfig
        // state.
        constexpr uint64_t tpmDeconfigMask = 0x0008000000000000ULL;
        rc = putscom_abs_wrap(&tgtHndl, PU_SECURITY_SWITCH_REGISTER_SCOM1,
                              tpmDeconfigMask);
        if( rc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC" Failed to clear TPM deconfig bit");
            break;
        }

        rc = getscom_abs_wrap (&tgtHndl,
                               PU_PRV_MISC_PPE,
                               &regData);
        if( rc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC" Failed to read SBE internal reg for TPM reset");
            break;
        }

        // To do TPM reset, first we should set bit 12 of PU_PRV_MISC_PPE
        // and then clear it up.
        regData = regData | tpmBitMask;
        rc = putscom_abs_wrap(&tgtHndl, PU_PRV_MISC_PPE, regData);
        if( rc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC" Failed to set TPM mask");
            break;
        }

        regData = regData & ( ~tpmBitMask);
        rc = putscom_abs_wrap(&tgtHndl, PU_PRV_MISC_PPE, regData);
        if( rc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC" Failed to clear TPM mask");
            break;
        }

    }while(0);
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode updatePhbFunctionalState( void )
{
    #define SBE_FUNC "updatePhbFunctionalState"
    SBE_ENTER(SBE_FUNC);
    ReturnCode rc = FAPI2_RC_SUCCESS;
    const uint64_t pci_cplt_conf1[3] = {PEC_0_CPLT_CONF1, PEC_1_CPLT_CONF1, PEC_2_CPLT_CONF1};
    // TODO workaround
    extern std::vector<fapi2::plat_target_handle_t> G_vec_targets;
    Target<TARGET_TYPE_PROC_CHIP > procTgt = plat_getChipTarget();
    auto phbTgt_vec = procTgt.getChildren<fapi2::TARGET_TYPE_PHB>();

    for (auto &phbTgt : phbTgt_vec)
    {
        //Get the PHB id
        uint8_t phb_id = 0;
        uint8_t pci_id = 0;
        uint8_t phbPerPciCnt = 0;
        uint64_t data = 0;

        FAPI_ATTR_GET(fapi2::ATTR_CHIP_UNIT_POS, phbTgt, phb_id);
        if(phb_id == 1 || phb_id == 2)
        {
            pci_id = 1;
            phbPerPciCnt = phb_id - 1; // To rotate per phb cnt within PCI from 0 to 1
        }
        else if(phb_id > 2)
        {
            pci_id = 2;
            phbPerPciCnt = phb_id - 3; // To rotate per phb cnt within PCI from 0 to 1
        }
        rc = getscom_abs_wrap (&procTgt, pci_cplt_conf1[pci_id], &data);
        if(rc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC" Failed to read Pec[%d] Chiplet Config1 register",pci_id);
            break;
        }
        // Fetch bit4 from D000009 for PHB0
        // Fetch bit4/5 from E000009 for PHB1/2
        // Fetch bit4/5/6 from F000009 for PHB3/4/5
        if( !((data >> (PEC_PHB_IOVALID_BIT_SHIFT - phbPerPciCnt)) & PEC_PHB_IOVALID_BIT_MASK) )
        {
            SBE_INFO(SBE_FUNC "PHB[%d] setting up as Non-Functional", phb_id);
            static_cast<plat_target_handle_t&>(phbTgt.operator ()()).setFunctional(false);
            G_vec_targets.at(PHB_TARGET_OFFSET+ phb_id) = (fapi2::plat_target_handle_t)(phbTgt.get());
        }
    }
    SBE_EXIT(SBE_FUNC);
    return rc;
#undef SBE_FUNC
}

//----------------------------------------------------------------------------
ReturnCode resetCrespErrLatch( void )
{
    #define SBE_FUNC "resetCrespErrLatch"
    SBE_ENTER(SBE_FUNC);
    ReturnCode rc = FAPI2_RC_SUCCESS;
    static const uint64_t BIT_63_MASK = 0x01;
    do
    {
        Target<TARGET_TYPE_PROC_CHIP > procTgt = plat_getChipTarget();
        uint64_t data;
        rc = getscom_abs_wrap (&procTgt, PU_PB_CENT_SM0_PB_CENT_MODE,
                                                &data);
        if( rc )
        {
            break;
        }
        data = data | BIT_63_MASK;
        rc = putscom_abs_wrap (&procTgt, PU_PB_CENT_SM0_PB_CENT_MODE,
                                               data);
        if( rc )
        {
            break;
        }
        data = data &(~BIT_63_MASK);
        rc = putscom_abs_wrap (&procTgt, PU_PB_CENT_SM0_PB_CENT_MODE,
                                              data);
        if( rc )
        {
            break;
        }
    }while(0);
    if( rc )
    {
        SBE_ERROR(SBE_FUNC" Failed to reset Cresp error latch");
    }
    SBE_EXIT(SBE_FUNC);
    return rc;
#undef SBE_FUNC
}

///////////////////////////////////////////////////////////////////////
// @brief sbeHandleSuspendIO Sbe suspend IO function
//
// @return  RC from the underlying FIFO utility
///////////////////////////////////////////////////////////////////////
uint32_t sbeHandleSuspendIO(uint8_t *i_pArg)
{
    #define SBE_FUNC " sbeHandleSuspendIO "
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    ReturnCode fapiRc = FAPI2_RC_SUCCESS;
    uint32_t len = 0;
    sbeRespGenHdr_t respHdr;
    respHdr.init();
    sbeResponseFfdc_t ffdc;
    Target<TARGET_TYPE_PROC_CHIP > procTgt = plat_getChipTarget();

    do
    {
        // Dequeue the EOT entry as no more data is expected.
        rc = sbeUpFifoDeq_mult (len, NULL);
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);

        // Update the PHB functional State before suspend io procedure
        fapiRc = updatePhbFunctionalState();
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC "updatePhbFunctionalState failed");
            respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                               SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
            ffdc.setRc(fapiRc);
            break;
        }

        SBE_EXEC_HWP(fapiRc, p9_suspend_io_hwp,  procTgt, false);
        if( fapiRc != FAPI2_RC_SUCCESS )
        {
            SBE_ERROR(SBE_FUNC "p9_suspend_io hwp failed");
            respHdr.setStatus( SBE_PRI_GENERIC_EXECUTION_FAILURE,
                               SBE_SEC_GENERIC_FAILURE_IN_EXECUTION);
            ffdc.setRc(fapiRc);
            break;
        }
    }while(0);

    // Create the Response to caller
    do
    {
        // If there was a FIFO error, will skip sending the response,
        // instead give the control back to the command processor thread
        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);
        rc = sbeDsSendRespHdr( respHdr, &ffdc);
    }while(0);

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}
//----------------------------------------------------------------------------

ReturnCode performAttrSetup( )
{
    #define SBE_FUNC "performAttrSetup "
    SBE_ENTER("performAttrSetup ");
    Target<TARGET_TYPE_PROC_CHIP > proc = plat_getChipTarget();
    ReturnCode rc = FAPI2_RC_SUCCESS;
    do
    {
        SBE_EXEC_HWP(rc, p9_sbe_attr_setup, proc)
        if( rc != FAPI2_RC_SUCCESS )
        {
            break;
        }
        // Apply the gard records
        rc = plat_ApplyGards();

        //Getting CBS_CS register value
        fapi2::buffer<uint64_t> tempReg = 0;
        plat_target_handle_t hndl;
        rc = getscom_abs_wrap(&hndl,
                              PERV_CBS_CS_SCOM, tempReg.pointer());
        if( rc != FAPI2_RC_SUCCESS )
        {
            break;
        }
        SBE_GLOBAL->sbeFWSecurityEnabled =
                    tempReg.getBit<PERV_CBS_CS_SECURE_ACCESS_BIT>();
     }while(0);
    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}

/* end PIBMEM CODE ----------------------------- */
#endif //#ifndef __SBEFW_SEEPROM__
