/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/measurement/sbemTPMSequences.C $                    */
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
/*
 * @file: p10_sbe_spi_cmd.C
 *
 * @brief This file contains and handle for SPI operations
 *
 */

#include "p10_sbe_spi_cmd.H"
#include "sbemtrace.H"
#include "endian.h"
#include "sbeutil.H"
#include "sbe_link.H"
#include "sbeXipUtils.H"
#include "sbestates.H"

using namespace fapi2;

fapi2::ReturnCode initializeTPM()
{
    #define SBEM_FUNC " initializeTPM "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    do
    {
        uint8_t spi_engine  = 4;
        Target<TARGET_TYPE_PROC_CHIP> i_target_chip =  plat_getChipTarget();
        SpiControlHandle handle = SpiControlHandle(i_target_chip, spi_engine);

        //Init SPI.
        //TODO:Use SBE method to initilize TPM.
        rc = p10_spi_clock_init(handle);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "p10_spi_clock_init failed with rc 08%08X", rc);
            break;
        }
    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}

fapi2::ReturnCode tpmReadandWriteSequence()
{
    #define SBEM_FUNC " tpmReadandWriteSequence "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    do
    {
        uint8_t spi_engine  = 4;
        Target<TARGET_TYPE_PROC_CHIP> i_target_chip =  plat_getChipTarget();
        SpiControlHandle handle = SpiControlHandle(i_target_chip, spi_engine);

        uint32_t tpmLocality = 0;      // TPM locality (0-4)
        uint32_t offsetAddr = 0x0F00;  // Relative address to read from TPM.
        uint8_t  rbuf[8];
        uint32_t readBytes = 2;

        // Read TPM through SPI.
        SBEM_INFO(SBEM_FUNC "Read TPM through SPI.");
        rc = spi_tpm_read_secure(handle, tpmLocality, offsetAddr, readBytes, (uint8_t *)&rbuf);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "spi_tpm_read_secure failed with rc 08%08X", rc);
            break;
        }
        for(uint32_t i = 0; i < readBytes; i++)
        {
            SBEM_INFO(SBEM_FUNC "TPM read data is 0x%02X", rbuf[i]);
        }

        //Write TPM through SPI.
        SBEM_INFO(SBEM_FUNC "Write TPM through SPI");
        offsetAddr = 0x0;
        readBytes = 1;
        uint8_t tpmData = 0x80;
        uint8_t *buf = &tpmData;
        rc = spi_tpm_write_with_wait(handle, tpmLocality, offsetAddr, readBytes, buf);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "spi_tpm_write_with_wait failed with rc 08%08X", rc);
            break;
        }

        //Read TPM through SPI.
        SBEM_INFO(SBEM_FUNC "Now Read TPM through SPI.");
        rc = spi_tpm_read_secure(handle, tpmLocality, offsetAddr, readBytes, (uint8_t *)&rbuf);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "spi_tpm_read_secure failed with rc 08%08X", rc);
            break;
        }
        for(uint32_t i = 0; i < readBytes; i++)
        {
            SBEM_INFO(SBEM_FUNC "TPM read data is 0x%02X", rbuf[i]);
        }
    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}

uint32_t checkSbeRole()
{
#define SBEM_FUNC " checkSbeRole "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode l_rc = fapi2::FAPI2_RC_SUCCESS;
    uint32_t sbeRole = SBE_ROLE_SLAVE;
    do
    {
        Target<TARGET_TYPE_PROC_CHIP> target =  plat_getChipTarget();
        fapi2::buffer<uint64_t> cbsreg;
        fapi2::buffer<uint64_t> scratchreg8;
        fapi2::buffer<uint64_t> scratchreg6;
        l_rc = getscom_abs_wrap (&target, 0x50004, &cbsreg());
        SBE_INFO(SBE_FUNC "FSXCOMP_FSXLOG_CBS_ENVSTAT_RO value is 0x%08X %08X",
            ((cbsreg >> 32) & 0xFFFFFFFF), (cbsreg & 0xFFFFFFFF));

        //Read Scratch register 8.
        l_rc = getscom_abs_wrap (&target, 0x5003F, &scratchreg8());
        SBE_INFO(SBE_FUNC "SCRTATCH_REG 8 value is 0x%08X %08X",
            ((scratchreg8 >> 32) & 0xFFFFFFFF), (scratchreg8 & 0xFFFFFFFF));

        if(scratchreg8.getBit<5>())
        {
            //Read Scratch Reg 6.
            l_rc = getscom_abs_wrap (&target, 0x5003D, &scratchreg6());
            SBE_INFO(SBE_FUNC "SCRATCH_REG 6 value is 0x%08X %08X",
                      ((scratchreg6 >> 32) & 0xFFFFFFFF), (scratchreg6 & 0xFFFFFFFF));
            if ( scratchreg6.getBit<24>() )
            {
                sbeRole = SBE_ROLE_MASTER;
                if( !((cbsreg >> 32) & 0x8000000) )
                {
                    sbeRole = SBE_ROLE_SLAVE;
                }
            }
            else
            {
               //ALT master
               if( ((cbsreg >> 32) & 0x8000000) )
                {   
                    sbeRole = SBE_ROLE_ALT_MASTER;
                }
            }
        }
        else
        {
            if( (cbsreg >> 32) & 0x8000000 )
            {
                sbeRole = SBE_ROLE_MASTER;
            }
        }
    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return sbeRole;
    #undef SBEM_FUNC
}

fapi2::ReturnCode performTPMSequences()
{
    #define SBEM_FUNC " performTPMSequences "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode l_rc = fapi2::FAPI2_RC_SUCCESS;
    do
    {
        // TPM access is only on master not on slave and alt master.
        // Skip TPM read and Write on proc other than master.
        uint32_t sbeRole = checkSbeRole();
        if(sbeRole == SBE_ROLE_SLAVE)
        {
            SBEM_INFO(SBE_FUNC "Current Proc is slave. Do not access TPM");
            break;
        }
        else if(sbeRole == SBE_ROLE_ALT_MASTER)
        {
            SBEM_INFO(SBE_FUNC "Current Proc is alt master. Poison TPM");
            break;
        }
        else
        {
            SBEM_INFO(SBE_FUNC "Current Proc is master. Access TPM");
        }
        l_rc = tpmReadandWriteSequence();
        if (l_rc)
        {
            SBEM_ERROR(SBEM_FUNC "tpmReadandWriteVerify failed with rc 0x%08X", l_rc);
            break;
        }
        SBEM_INFO(SBEM_FUNC "Completed tpmReadandWriteVerify");
    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return l_rc;
    #undef SBEM_FUNC
}
