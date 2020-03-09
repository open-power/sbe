/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power_dft/sbecmdexitcachecontained.C $          */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2020                        */
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
#include "sbecmdexitcachecontained.H"
#include "sbetrace.H"
#include "sbeHostUtils.H"
#include "sbeglobals.H"
#include "p10_sbe_exit_cache_contained.H"
#include "fapi2_mem_access.H"
#include "p10_getmempba.H"
#include "p10_sbe_apply_xscom_inits.H"

using namespace fapi2;

#define ALIGN_SIZE 128
uint32_t sbePsuExitCacheContainedMode(uint8_t *i_pArg)
{
#define SBE_FUNC " sbePsuExitCacheContainedMode"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t fapiRc = FAPI2_RC_SUCCESS;
    Target<fapi2::TARGET_TYPE_PROC_CHIP> procTgt = plat_getChipTarget();
    do 
    {
        sbePsuExitCacheContainedMode_t req = {};
        rc = sbeReadPsu2SbeMbxReg(SBE_HOST_PSU_MBOX_REG1,
             sizeof(req)/sizeof(uint64_t),(uint64_t*)&req,true);

        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);

        SBE_INFO(SBE_FUNC " Addr[0x%08X%08X] ,NO. of Xscom entries[0x%08X] and "
                "Exit cache contained Step[0x%08X]",
                SBE::higher32BWord(req.address),SBE::lower32BWord(req.address),
                req.xscomEntries,req.stepInfo);

        p10_sbe_exit_cache_contained_step_t stepInfo = 
                                            p10_sbe_exit_cache_contained_step_t(req.stepInfo);
        //If the Step Flow is not RUN_ALL execute only the required flow
        if(p10_sbe_exit_cache_contained_step_t::RUN_ALL != stepInfo)
        {
            SBE_EXEC_HWP(fapiRc, p10_sbe_exit_cache_contained,procTgt,req.xscomEntries,
                    reinterpret_cast<void*>(req.address),stepInfo);
            if(fapiRc != FAPI2_RC_SUCCESS)
            {
                SBE_ERROR(SBE_FUNC "Failed in p10_sbe_exit_cache_contained() Addr[0x%08X%08X] "
                        "Number of Xscom entries[0x%08X]",
                        SBE::higher32BWord(req.address),SBE::lower32BWord(req.address),
                        req.xscomEntries);
            }
            break;
        }

        //Step Flow = RUN_ALL
        //Completion of Exit Cache contained mode will be done in 3 steps
        //#1:  p10_sbe_exit_cache_contained(STOP_HB| REVERT_MCS_SETUP)
        //#2:  Fetch the XSCOM address-data pair from the memory address shared
        //by the hostboot and populate local buffer. Execute procedure
        //p10_sbe_exit_cache_contained(SETUP_MEMORY_BARS).
        //#3:  p10_sbe_exit_cache_contained(PURGE_HB | RESUME_HB)

        //STEP1:
        stepInfo = (p10_sbe_exit_cache_contained_step_t::STOP_HB |
                    p10_sbe_exit_cache_contained_step_t::REVERT_MCS_SETUP);
        SBE_EXEC_HWP(fapiRc, p10_sbe_exit_cache_contained,procTgt,req.xscomEntries,
                reinterpret_cast<void*>(req.address),stepInfo);
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC "Failed in p10_sbe_exit_cache_contained() Addr[0x%08X%08X] "
            "Number of Xscom entries[0x%08X] and Step:STOP_HB| REVERT_MCS_SETUP",
            SBE::higher32BWord(req.address),SBE::lower32BWord(req.address),req.xscomEntries);
            break;
        }

        //STEP2:
        fapiRc = sbeFetchAndApplyXSCOMInit(req.xscomEntries,reinterpret_cast<void*>(req.address));
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC "Failed in sbeFetchAndApplyXSCOMInit() Addr[0x%08X%08X] "
            "Number of Xscom entries[0x%08X]",
            SBE::higher32BWord(req.address),SBE::lower32BWord(req.address),req.xscomEntries);
            break;
        }

        //STEP3:
        stepInfo = (p10_sbe_exit_cache_contained_step_t::PURGE_HB |
                    p10_sbe_exit_cache_contained_step_t::RESUME_HB);
        SBE_EXEC_HWP(fapiRc, p10_sbe_exit_cache_contained,procTgt,req.xscomEntries,
                     reinterpret_cast<void*>(req.address),stepInfo);
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC "Failed in p10_sbe_exit_cache_contained() Addr[0x%08X%08X] "
            "Number of Xscom entries[0x%08X] and Step:PURGE_HB | RESUME_HB",
            SBE::higher32BWord(req.address),SBE::lower32BWord(req.address),req.xscomEntries);
            break;
        }
    }while(0);

    // Send the response
    sbePSUSendResponse(SBE_GLOBAL->sbeSbe2PsuRespHdr, fapiRc, rc);
    if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
    {
        SBE_ERROR( SBE_FUNC" Failed to write SBE_HOST_PSU_MBOX_REG4. rc[0x%X]", rc);
    }
    SBE_EXIT(SBE_FUNC);
    return rc;
#undef SBE_FUNC
}

uint32_t sbeFetchAndApplyXSCOMInit(const size_t i_xscomPairSize,const void* i_xscomInitAddress)
{
#define SBE_FUNC " sbePsuExitCacheContainedMode"
    SBE_ENTER(SBE_FUNC);
    fapi2::ReturnCode fapiRc = fapi2::FAPI2_RC_SUCCESS;
    uint64_t xscomInitAddress = reinterpret_cast<uint64_t>(i_xscomInitAddress);
    uint32_t regInitSize = i_xscomPairSize;
    Target<fapi2::TARGET_TYPE_PROC_CHIP> procTgt = plat_getChipTarget();
    //SBE has restriction to allocate maximum buffer size of 1024 bytes.
    //So maximum of 1020/16=64 pairs of XSCOM address and data pair can be read
    //at once.
    const uint32_t MAX_XSCOM_ENTRIES_PER_ITER = 64;
    uint32_t flags = fapi2::SBE_MEM_ACCESS_FLAGS_TARGET_PBA |
                       fapi2::SBE_MEM_ACCESS_FLAGS_FAST_MODE_ON |
                       fapi2::SBE_MEM_ACCESS_FLAGS_LCO_MODE;

    do
    {

        //Address MAX_XSCOM_ENTRIES_PER_ITER pairs of XSCOM at once.
        uint32_t numOfPairs =((regInitSize > MAX_XSCOM_ENTRIES_PER_ITER)?MAX_XSCOM_ENTRIES_PER_ITER : regInitSize);

        //Memory allocated for performing the PBA operation needs to be 128bytes
        //aligned.
        uint32_t sizeInBytes = numOfPairs * sizeof(xscomsInits_t);
        uint32_t alignedBytes = sizeInBytes + ( ALIGN_SIZE - (sizeInBytes % ALIGN_SIZE ));
        uint32_t alignedEntrySize = alignedBytes / sizeof(xscomsInits_t);
        xscomsInits_t reg_inits[alignedEntrySize ] = {0};
 
        //Populate the Xscom Init address and data list from the memory shared by
        //hostboot
        SBE_EXEC_HWP(fapiRc, p10_getmempba,procTgt,xscomInitAddress,alignedBytes,
                      reinterpret_cast<uint8_t*>(reg_inits), flags);
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC "Failed in p10_getmempba() ,Addr[0x%08X%08X] "
            "bytes[0x%08X]",SBE::higher32BWord(xscomInitAddress),
            SBE::lower32BWord(xscomInitAddress), alignedBytes);
            break;
        }

        //Perform the XSCOM operation for the obtained address - data pair
        SBE_EXEC_HWP(fapiRc,p10_sbe_apply_xscom_inits,procTgt,numOfPairs,reg_inits);
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC "Failed in p10_sbe_apply_xscom_inits():"
                      "numOfPairs=%d",numOfPairs);
            break;
        }

        //Modify the number of remaining entries
        regInitSize = (regInitSize - numOfPairs);
        //Modify the address from where the next set of XSCOM inits needs to be
        //fetched.
        xscomInitAddress = reinterpret_cast<uint64_t>(xscomInitAddress + sizeInBytes);

    }
    while(regInitSize);  //While there any register entries remaining to be initialized.

fapi_try_exit:
    SBE_EXIT(SBE_FUNC);
    return fapiRc;
#undef SBE_FUNC
}

