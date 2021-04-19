/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/app/power/sbecmdmemconfig.C $                       */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2019,2021                        */
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
#include "sbecmdmemconfig.H"
#include "sbetrace.H"
#include "sbeHostUtils.H"
#include "sbeglobals.H"
#include "fapi2_mem_access.H"
#include "p10_getmempba.H"
#include "p10_scom_proc_9.H"
#include "plat_target_utils.H"

using namespace fapi2;

#define ALIGN_SIZE 128
uint32_t sbePsuMemConfig(uint8_t *i_pArg)
{
#define SBE_FUNC " sbePsuMemConfig"
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t fapiRc = FAPI2_RC_SUCCESS;
    Target<fapi2::TARGET_TYPE_PROC_CHIP> procTgt = plat_getChipTarget();
    do
    {
        sbePsuMemConfig_t req = {};
        rc = sbeReadPsu2SbeMbxReg(SBE_HOST_PSU_MBOX_REG1,
             sizeof(req)/sizeof(uint64_t),(uint64_t*)&req,true);

        CHECK_SBE_RC_AND_BREAK_IF_NOT_SUCCESS(rc);

        SBE_INFO(SBE_FUNC " Data Addr[0x%08X%08X]",
                SBE::higher32BWord(req.data_address),SBE::lower32BWord(req.data_address));

        //Memory allocated for performing the PBA operation needs to be 128bytes
        //aligned.
        uint32_t flags = fapi2::SBE_MEM_ACCESS_FLAGS_TARGET_PBA |
                       fapi2::SBE_MEM_ACCESS_FLAGS_FAST_MODE_ON |
                       fapi2::SBE_MEM_ACCESS_FLAGS_LCO_MODE;
        uint32_t sizeInBytes = sizeof(SbeMemConfigData_t);
        uint32_t alignedBytes = sizeInBytes + ( ALIGN_SIZE - (sizeInBytes % ALIGN_SIZE ));
        uint8_t pibMemData[alignedBytes] = {0};

        //Populate the config data from the memory shared by hostboot
        SBE_EXEC_HWP(fapiRc, p10_getmempba,procTgt, req.data_address, alignedBytes,
                      pibMemData, flags);
        if(fapiRc != FAPI2_RC_SUCCESS)
        {
            SBE_ERROR(SBE_FUNC "Failed in p10_getmempba() ,Addr[0x%08X%08X] "
            "bytes[0x%08X]",SBE::higher32BWord(req.data_address),
            SBE::lower32BWord(req.data_address), alignedBytes);
            break;
        }

        SbeMemConfigData_t *configData = reinterpret_cast<SbeMemConfigData_t*>(pibMemData);
        if(!configData->validateVersion())
        {
            SBE_ERROR("Invalid version recieved: %d", configData->i2c_config_version);
            SBE_GLOBAL->sbeSbe2PsuRespHdr.setStatus(
                SBE_PRI_INVALID_DATA,
                SBE_SEC_INVALID_I2C_CONFIG_VERSION);
            break;
        }
        if(!configData->validateTargetTypesSupported())
        {
            SBE_ERROR("Invalid number of target types supported recieved: %d",
                configData->i2c_config_types_supported);
            SBE_GLOBAL->sbeSbe2PsuRespHdr.setStatus(
                SBE_PRI_INVALID_DATA,
                SBE_SEC_INVALID_I2C_TARGET_SUPPORT);
            break;
        }
        for(int i = 0; i < configData->ocmb_chips_max_number; i++)
        {
            plat_DIMMTargetInit(i, DIMM_OCMB_TARGET, configData->ocmb_chips[i]);
        }

        for(int i = 0; i < configData->pmic_chips_max_number; i++)
        {
            plat_DIMMTargetInit(i, DIMM_PMIC_TARGET, configData->pmic_chips[i]);
        }

        for(int i = 0; i < configData->gi2c_chips_max_number; i++)
        {
            plat_DIMMTargetInit(i, DIMM_GI2C_TARGET, configData->gi2c_chips[i]);
        }

        plat_PrintTargets();
    }while(0);

    // Send the response
    sbePSUSendResponse(SBE_GLOBAL->sbeSbe2PsuRespHdr, fapiRc, rc);
    if(rc != SBE_SEC_OPERATION_SUCCESSFUL)
    {
        SBE_ERROR( SBE_FUNC" Failed to write SBE_HOST_PSU_MBOX_REG4. rc[0x%X]", rc);
        //Not breaking here
    }

    SBE_EXIT(SBE_FUNC);
    return rc;
#undef SBE_FUNC
}

