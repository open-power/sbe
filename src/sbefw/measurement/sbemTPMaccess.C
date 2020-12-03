/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/measurement/sbemTPMaccess.C $                       */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020,2021                        */
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
#include "sbemTPMaccess.H"
#include "sbemtrace.H"

static const size_t TPM_MAX_SPI_TRANSMIT_SIZE = 64;

fapi2::ReturnCode tpmIsExpecting(SpiControlHandle &handle, bool &isExpect )
{
   #define SBEM_FUNC " tpmIsExpecting "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    isExpect = false;
    tpm_sts_reg_t status;
    do
    {
        uint32_t tpmLocality = 0;      // TPM locality (0-4)
        uint32_t offsetAddr = TPM_INT_STATUS_0;  // Relative address to read from TPM.
        uint32_t readBytes = 1;

        SBEM_INFO(SBEM_FUNC "Read TPM Status Register.");
        rc = spi_tpm_read_secure(handle, tpmLocality, offsetAddr, readBytes, (uint8_t *)&status);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "spi_tpm_read_secure failed while reading TPM Status register with rc 0x%08X", rc);
            break;
        }
        SBEM_INFO(SBEM_FUNC "TPM read data is 0x%02X", status.value);
        if(status.fields.expect)
        {
            isExpect = true;
        }
    }while(0);
    SBEM_INFO(SBEM_FUNC "isExpect is %d", isExpect);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}

fapi2::ReturnCode tpmIsDataAvail(SpiControlHandle &handle, bool &dataAvail )
{
   #define SBEM_FUNC " tpmIsDataAvail "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    dataAvail = false;
    tpm_sts_reg_t status;
    do
    {
        uint32_t tpmLocality = 0;      // TPM locality (0-4)
        uint32_t offsetAddr = TPM_INT_STATUS_0;  // Relative address to read from TPM.
        uint32_t readBytes = 1;

        SBEM_INFO(SBEM_FUNC "Read TPM Status Register.");
        rc = spi_tpm_read_secure(handle, tpmLocality, offsetAddr, readBytes, (uint8_t *)&status);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "spi_tpm_read_secure failed while reading TPM Status register with rc 0x%08X", rc);
            break;
        }
        SBEM_INFO(SBEM_FUNC "TPM read data is 0x%02X", status.value);
        if(status.fields.dataAvail)
        {
            dataAvail = true;
        }
    }while(0);
    SBEM_INFO(SBEM_FUNC "dataAvail is %d", dataAvail);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}

fapi2::ReturnCode tpmIsCommandReady(SpiControlHandle &handle, bool &isCmdReady )
{
   #define SBEM_FUNC " tpmIsCommandReady "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    isCmdReady = false;
    tpm_sts_reg_t status;
    do
    {
        uint32_t tpmLocality = 0;      // TPM locality (0-4)
        uint32_t offsetAddr = TPM_INT_STATUS_0;  // Relative address to read from TPM.
        uint32_t readBytes = 1;

        SBEM_DEBUG(SBEM_FUNC "Read TPM Status Register.");
        rc = spi_tpm_read_secure(handle, tpmLocality, offsetAddr, readBytes, (uint8_t *)&status);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "spi_tpm_read_secure failed while reading TPM Status register with rc 0x%08X", rc);
            break;
        }
        SBEM_INFO(SBEM_FUNC "TPM read data is 0x%02X", status.value);
        if(status.fields.isCommandReady)
        {
            isCmdReady = true;
        }
    }while(0);
    SBEM_INFO(SBEM_FUNC "TPM isCommand Ready is %d", isCmdReady);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}

fapi2::ReturnCode tpmWriteCommandReady(SpiControlHandle &handle)
{
    #define SBEM_FUNC " tpmWriteCommandReady "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    tpm_sts_reg_t stsReg;
    stsReg.value = 0;
    stsReg.fields.isCommandReady = 1;
    do
    {
        uint32_t tpmLocality = 0;      // TPM locality (0-4)
        uint32_t offsetAddr = TPM_INT_STATUS_0;  // Relative address to read from TPM.
        uint32_t readBytes = 1;
        rc = spi_tpm_write_with_wait(handle, tpmLocality, offsetAddr, readBytes, (uint8_t *)&stsReg);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "spi_tpm_write_with_wait failed while setting isCommandReady bit with rc 0x%08X", rc);
            break;
        }
    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}

fapi2::ReturnCode tpmPollForCommandReady(SpiControlHandle &handle)
{
   #define SBEM_FUNC " tpmPollForCommandReady "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    tpm_sts_reg_t status;
    do
    {
        uint32_t tpmLocality = 0;      // TPM locality (0-4)
        uint32_t offsetAddr = TPM_INT_STATUS_0;  // Relative address to read from TPM.
        uint32_t readBytes = 1;

        for (uint32_t delay = 0; delay < TPM_TIMEOUT_B; delay += 10)
        {
            rc = spi_tpm_read_secure(handle, tpmLocality, offsetAddr, readBytes, (uint8_t *)&status);
            if( (rc != fapi2::FAPI2_RC_SUCCESS) || ((rc == fapi2::FAPI2_RC_SUCCESS) && (status.fields.isCommandReady)) )
            {
                SBEM_ERROR(SBEM_FUNC "spi_tpm_read_secure either failed while reading TPM Status register with rc 0x%08X or cmdReady %d",
                                     rc, status.fields.isCommandReady);
                break;
            }
            //TODO Add delay.
            //fapi2::delay(1000000, 1000000);
        }

        if((rc == fapi2::FAPI2_RC_SUCCESS) && (!status.fields.isCommandReady))
        {
            SBE_INFO(SBE_FUNC "isCommandReady Bit is not set. Set and poll");
            // The first write to command ready may have just aborted
            // an outstanding command, we will write it again and poll once
            // more
            rc = tpmWriteCommandReady(handle);
            if( rc != fapi2::FAPI2_RC_SUCCESS )
            {
                SBEM_ERROR(SBEM_FUNC "tpmWriteCommandReady failed with rc 0x%08X", rc);
                break;
            }
            else
            {
                // Ok, poll again
                // Operation TIMEOUT_B defined by TCG spec for command ready
                bool isCommandReady = false;
                for (uint32_t delay = 0; delay < TPM_TIMEOUT_B; delay += 10)
                {
                    rc = tpmIsCommandReady(handle, isCommandReady);
                    if((rc != fapi2::FAPI2_RC_SUCCESS) && ((rc == fapi2::FAPI2_RC_SUCCESS) && (isCommandReady)))
                    {
                        break;
                    }
                    // TODO Add delay.
                    //fapi2::delay(1000000, 1000000);
                }
            }
            
        }
        if((rc == fapi2::FAPI2_RC_SUCCESS) && (!status.fields.isCommandReady))
        {
            rc = fapi2::FAPI2_RC_FALSE;
            break;
        }

    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}

fapi2::ReturnCode tpmReadBurstCount(SpiControlHandle &handle,
                                    uint16_t & o_burstCount)
{
    #define SBEM_FUNC " tpmReadBurstCount "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    do
    {
        uint32_t tpmLocality = 0;      // TPM locality (0-4)
        uint32_t offsetAddr = TPM_INT_BURSTCOUNT_0;  // Relative address to read from TPM.
        uint16_t value = 0;
        uint32_t readBytes = 2;
        rc = spi_tpm_read_secure(handle, tpmLocality, offsetAddr, readBytes, (uint8_t *)&value);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "spi_tpm_read_secure failed while reading burst count with rc 0x%08X", rc);
            break;
        }
        uint8_t lowByte = (value & 0x00FF);
        uint8_t highByte = (value & 0xFF00) >> 8;
        o_burstCount = (lowByte << 8) | highByte;
    }while(0);
    SBEM_DEBUG(SBEM_FUNC "Burst count is 0x%04X", o_burstCount);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}

fapi2::ReturnCode tpmWriteTpmGo(SpiControlHandle &handle)
{
    #define SBEM_FUNC " tpmWriteTpmGo "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    tpm_sts_reg_t stsReg;
    stsReg.value = 0;
    stsReg.fields.tpmGo = 1;
    do
    {
        uint32_t tpmLocality = 0;      // TPM locality (0-4)
        uint32_t offsetAddr = TPM_INT_STATUS_0;  // Relative address to read from TPM.
        uint32_t readBytes = 1;
        rc = spi_tpm_write_with_wait(handle, tpmLocality, offsetAddr, readBytes, (uint8_t *)&stsReg);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "spi_tpm_write_with_wait failed while setting tpmGo bit with rc 0x%08X", rc);
            break;
        }
    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}

fapi2::ReturnCode tpmReadSTSRegValid(SpiControlHandle &handle, tpm_sts_reg_t & o_stsReg)
{
    #define SBEM_FUNC " tpmReadSTSRegValid "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    uint32_t polls = 0;
    do
    {
        uint32_t tpmLocality = 0;      // TPM locality (0-4)
        uint32_t offsetAddr = TPM_INT_STATUS_0;  // Relative address to read from TPM.
        uint32_t readBytes = 1;
        rc = spi_tpm_read_secure(handle, tpmLocality, offsetAddr, readBytes, (uint8_t *)&o_stsReg);
        if(rc != fapi2::FAPI2_RC_SUCCESS)
        {
            SBEM_ERROR(SBEM_FUNC "spi_tpm_read_secure failed while reading status with rc 0x%08X", rc);
            break;
        }
        SBEM_INFO(SBEM_FUNC "TPM status register value is 0x%02X", o_stsReg.value);
        if((polls > MAX_STSVALID_POLLS) && !(o_stsReg.fields.stsValid))
        {
            SBEM_INFO(SBEM_FUNC "TPM timeout waiting for stsValid");
            rc = fapi2::FAPI2_RC_FALSE;
            break;
        }
        else if(!o_stsReg.fields.stsValid)
        {
            fapi2::delay(1000000, 1000000);
            polls++;
        }
    }while(!o_stsReg.fields.stsValid);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}

fapi2::ReturnCode tpmPollForDataAvail(SpiControlHandle &handle)
{
    #define SBEM_FUNC " tpmPollForDataAvail "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    tpm_sts_reg_t status;
    do
    {
        // Use the longer timeout B here since some of the TPM commands may take
        // more than timeout A to complete
        for (uint32_t delay = 0; delay < TPM_TIMEOUT_B; delay += 10)
        {
            rc = tpmReadSTSRegValid(handle, status);
            if( (rc != fapi2::FAPI2_RC_SUCCESS) || ((rc == fapi2::FAPI2_RC_SUCCESS) && (status.fields.dataAvail)))
            {
                SBEM_ERROR(SBEM_FUNC "Either tpmReadSTSRegValid failed while with rc 0x%08X or data is available %d", rc, status.fields.dataAvail);
                break;
            }
            if((rc != fapi2::FAPI2_RC_SUCCESS) && (rc == fapi2::FAPI2_RC_FALSE))
            {
                SBEM_ERROR(SBEM_FUNC "Polling loop within tpmReadSTSRegValid timed out");
                rc = fapi2::FAPI2_RC_SUCCESS;
            }
            fapi2::delay(1000000, 1000000);
        }
        if((rc == fapi2::FAPI2_RC_SUCCESS) && (!status.fields.dataAvail))
        {
            SBEM_ERROR(SBEM_FUNC "Timeout polling for dataAvail!");
            rc = fapi2::FAPI2_RC_FALSE;
        }
    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}

fapi2::ReturnCode tpmReadFifo(SpiControlHandle &handle,
                              void * o_buffer, 
                              uint32_t & io_buflen)
{
    #define SBEM_FUNC " tpmReadFifo "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    uint32_t delay = 0;
    uint32_t curByte = 0;
    uint8_t* bytePtr = (uint8_t*)o_buffer;
    uint8_t* curBytePtr = nullptr;
    uint16_t burstCount = 0;
    bool dataAvail = false;
    bool firstRead = true;
    uint16_t dataLen = 0;
    uint32_t responseSize = 0;
    uint32_t dataLeft = io_buflen;
    do
    {
        // all command responses are at least 10 bytes of data
        // 2 byte tag + 4 byte response size + 4 byte response code
        const uint32_t MIN_COMMAND_RESPONSE_SIZE = 10;

        // Verify the TPM has data waiting for us
        rc = tpmPollForDataAvail(handle);
        if(rc != fapi2::FAPI2_RC_SUCCESS)
        {
            SBEM_ERROR(SBEM_FUNC "tpmPollForDataAvail failed with rc 0x%08X", rc);
            break;
        }

        // Read burst count.
        rc = tpmReadBurstCount(handle, burstCount);
        if(rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "tpmReadBurstCount failed with rc 0x%08X", rc);
            break;
        }
        else if(burstCount == 0)
        {
            //TODO: Add some delay
            delay += 10;
            continue;
        }
        // Read some data.
        if (firstRead)
        {
            dataLen = MIN_COMMAND_RESPONSE_SIZE;
        }
        else if (burstCount < dataLeft)
        {
            dataLen = burstCount;
        }
        else
        {
            dataLen = dataLeft;
        }
        // Check for a buffer overflow
        if (curByte + dataLen > io_buflen)
        {
            // TPM is expecting more data even though we think we are done
            SBEM_INFO(SBEM_FUNC "TPM is expecting more data even though we think we are done");
            rc = fapi2::FAPI2_RC_FALSE;
            break;
        }

        delay = 0;
        curBytePtr = &(bytePtr[curByte]);
        SBEM_INFO(SBEM_FUNC " Read %d byte from FIFO", dataLen);
        uint32_t tpmLocality = 0;               // TPM locality (0-4)
        uint32_t offsetAddr = TPM_DATA_FIFO_0;  // Relative address to read from TPM.
        rc = spi_tpm_read_secure(handle, tpmLocality, offsetAddr, dataLen, curBytePtr);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "spi_tpm_read_secure failed while reading data from FIFO with rc 0x%08X", rc);
            break;
        }
        SBEM_INFO(SBEM_FUNC "Read %d byte", dataLen);
        if (firstRead)
        {
            SBEM_INFO(SBEM_FUNC, "Inside firstRead" );
            //responseSize = *(reinterpret_cast<uint32_t*>((curBytePtr + 2)));
            responseSize = curBytePtr[2] << 24 | curBytePtr[3] << 16 | curBytePtr[4] << 8 | curBytePtr[5];
            SBEM_INFO(SBEM_FUNC, "tpmReadFifo: total size = 0x%08X", responseSize );
            dataLeft = responseSize;
            firstRead = false;
        }
        curByte += dataLen;
        dataLeft -= dataLen;

        rc = tpmIsDataAvail(handle, dataAvail);
        if((rc != fapi2::FAPI2_RC_SUCCESS) || ((rc == fapi2::FAPI2_RC_SUCCESS) && (!dataAvail)) )
        {
            SBEM_ERROR(SBEM_FUNC "Either tpmIsDataAvail failed with rc 0x%08X or dataAvail is %d", rc, dataAvail);
            break;
        }
        if ((dataLeft == 0) && dataAvail)
        {
            // Either the available STS is wrong or
            // responseSize in firstRead response was wrong
            SBEM_INFO(SBEM_FUNC "data should not be available anymore"
                                " (response size in response: 0x%04X)", responseSize);
            rc = fapi2::FAPI2_RC_FALSE;
            break;
        }
    }while(delay < TPM_TIMEOUT_D);
    do
    {
        if (!rc && delay >= TPM_TIMEOUT_D)
        {
            SBEM_INFO(SBEM_FUNC "tpmReadFifo: timeout");
            rc = fapi2::FAPI2_RC_FALSE;
            break;
        }
        if (rc == fapi2::FAPI2_RC_SUCCESS)
        {
            // We read it properly tell the caller the result length
            io_buflen = curByte;
        }
        else
        {
            io_buflen = 0;
        }
    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}

fapi2::ReturnCode tpmWriteFifo(SpiControlHandle &handle,
                              void * i_buffer, 
                              uint32_t i_buflen)
{
    #define SBEM_FUNC " tpmWriteFifo "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    uint32_t delay = 0;
    uint32_t curByte = 0;
    uint8_t* bytePtr = (uint8_t*)i_buffer;
    uint8_t* curBytePtr = nullptr;
    uint16_t burstCount = 0;
    uint32_t length = i_buflen - 1;
    uint32_t tx_len = 0;
    bool expect = false;
    do
    {
        // Read burst count.
        rc = tpmReadBurstCount(handle, burstCount);
        if(rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "tpmReadBurstCount failed with rc 0x%08X", rc);
            break;
        }
        else if(burstCount == 0)
        {
            //TODO: Add delay to allow the TPM time.
            delay += 10;
            continue;
        }

        // Single operations are limited to TPM SPI transmit size
        if (burstCount > TPM_MAX_SPI_TRANSMIT_SIZE)
        {
            burstCount = TPM_MAX_SPI_TRANSMIT_SIZE;
        }

        //Send in some data
        delay = 0;
        curBytePtr = &(bytePtr[curByte]);
        tx_len = (curByte + burstCount > length ?
                  (length - curByte) :
                  burstCount);
        SBEM_INFO(SBEM_FUNC "tpmWriteFifo: send some data %d tx_len", tx_len);
        uint32_t tpmLocality = 0;               // TPM locality (0-4)
        uint32_t offsetAddr = TPM_DATA_FIFO_0;  // Relative address to read from TPM.
        rc = spi_tpm_write_with_wait(handle, tpmLocality, offsetAddr, tx_len, curBytePtr);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "spi_tpm_write_with_wait failed with rc 0x%08X", rc);
            break;
        }
        curByte += tx_len;

        // TPM should be expecting more data from the command
        SBEM_INFO(SBEM_FUNC "TPM should be expecting more data from the command");
        rc = tpmIsExpecting(handle, expect);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "tpmIsExpecting failed with rc 0x%08X", rc);
            break;
        }
        if(( rc == fapi2::FAPI2_RC_SUCCESS) && (!expect))
        {
            SBEM_ERROR(SBEM_FUNC "TPM is not expecting any further data, expect is %d", expect);
            rc = fapi2::FAPI2_RC_FALSE;
            break;
        }
        // Everything but the last byte sent?
        SBEM_INFO(SBEM_FUNC "Everything but the last byte sent");
        if (curByte >= length)
        {
            break;
        }
    }while(delay < TPM_TIMEOUT_D);

    if((rc ==fapi2::FAPI2_RC_SUCCESS) && (delay < TPM_TIMEOUT_D))
    {
        delay = 0;
        // Send the final byte.
        SBEM_INFO(SBEM_FUNC "Send the final byte");
        do
        {
            // Read burst count.
            rc = tpmReadBurstCount(handle, burstCount);
            if( rc != fapi2::FAPI2_RC_SUCCESS )
            {
                SBEM_ERROR(SBEM_FUNC "tpmReadBurstCount failed with rc 0x%08X", rc);
                break;
            }
            else if(burstCount == 0)
            {
                //TODO: Add delay
                delay += 10;
                continue;
            }
            // Send in some data
             delay = 0;
             curBytePtr = &(bytePtr[curByte]);
             SBEM_INFO(SBEM_FUNC "tpmWriteReg final byte, 0x%02X", *curBytePtr);
             uint32_t tpmLocality = 0;               // TPM locality (0-4)
             uint32_t offsetAddr = TPM_DATA_FIFO_0;  // Relative address to read from TPM.
             rc = spi_tpm_write_with_wait(handle, tpmLocality, offsetAddr, tx_len, curBytePtr);
             if( rc != fapi2::FAPI2_RC_SUCCESS )
             {
                 SBEM_ERROR(SBEM_FUNC "spi_tpm_write_with_wait failed with rc 0x%08X", rc);
             }
             break;
        }while(delay < TPM_TIMEOUT_D);
    }
    do
    {
        if((rc == fapi2::FAPI2_RC_SUCCESS) && (delay >= TPM_TIMEOUT_D))
        {
            SBE_ERROR(SBE_FUNC "TPM has timed out");
            rc = fapi2::FAPI2_RC_FALSE;
            break;
        }
        if(rc == fapi2::FAPI2_RC_SUCCESS)
        {
            SBEM_INFO(SBEM_FUNC "Check if TPM is expecting more data.");
            rc = tpmIsExpecting(handle, expect);
            if(( rc != fapi2::FAPI2_RC_SUCCESS) && (expect))
            {
                SBEM_ERROR(SBEM_FUNC "tpmIsExpecting failed or expecting more data with rc 0x%08X %d", rc, expect);
                rc = fapi2::FAPI2_RC_FALSE;
                break;
            }
        }
    }while(0);
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}

fapi2::ReturnCode tpmTransmit( SpiControlHandle &handle,
                         void * io_buffer,
                         uint32_t & io_buflen,
                         uint32_t i_commandlen)
{
    #define SBEM_FUNC " tpmTransmit "
    SBEM_ENTER(SBEM_FUNC);
    fapi2::ReturnCode rc = fapi2::FAPI2_RC_SUCCESS;
    SBEM_INFO(SBEM_FUNC "TPM TRANSMIT START buflen 0x%08X and cmdlen 0x%08X",
                         io_buflen, i_commandlen);
    do
    {
        // Verify the TPM is ready to receive our command.
        bool isCmdReady = false;
        rc = tpmIsCommandReady(handle, isCmdReady );
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
            SBEM_ERROR(SBEM_FUNC "tpmIsCommandReady failed with rc 0x%08X", rc);
            break;
        }
        SBEM_INFO(SBEM_FUNC "isCmdReady is %d", isCmdReady);
        if(!isCmdReady)
        {
            // set TPM into command ready state
            rc = tpmWriteCommandReady(handle);
            if( rc != fapi2::FAPI2_RC_SUCCESS )
            {
                SBEM_ERROR(SBEM_FUNC "tpmWriteCommandReady failed with rc 0x%08X", rc);
                break;
            }

            // Verify the TPM is now ready to receive our command
            rc = tpmPollForCommandReady(handle);
            if( rc != fapi2::FAPI2_RC_SUCCESS )
            {
                SBEM_ERROR(SBEM_FUNC "tpmPollForCommandReady failed with rc 0x%08X", rc);
                break;
            }
        }
        SBEM_INFO(SBEM_FUNC "isCmdReady is set. Go write the command to FIFO.");

        // Write the command into the TPM FIFO
        rc = tpmWriteFifo(handle, io_buffer, i_commandlen);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
             SBEM_ERROR(SBEM_FUNC "tpmWriteFifo failed with rc 0x%08X", rc);
             break;
        }

        // Set the tpmGo bit
        SBEM_INFO(SBEM_FUNC "Set the tpmGo bit");
        rc = tpmWriteTpmGo(handle);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
             SBEM_ERROR(SBEM_FUNC "tpmWriteTpmGo failed with rc 0x%08X", rc);
             break;
        }

        // Read the response from the TPM FIFO
        SBEM_INFO(SBEM_FUNC "Read the response from TPM FIFO");
        rc = tpmReadFifo(handle, io_buffer, io_buflen);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
             SBEM_ERROR(SBEM_FUNC "tpmReadFifo failed with rc 0x%08X", rc);
             break;
        }
        SBEM_INFO(SBEM_FUNC "Buffer length after response is 0x%08X", io_buflen);

        // Set write command ready.
        SBEM_INFO(SBEM_FUNC "Set write command ready");
        rc = tpmWriteCommandReady(handle);
        if( rc != fapi2::FAPI2_RC_SUCCESS )
        {
             SBEM_ERROR(SBEM_FUNC "tpmWriteCommandReady failed with rc 0x%08X", rc);
             break;
        }
    }while(0);
    
    SBEM_EXIT(SBEM_FUNC);
    return rc;
    #undef SBEM_FUNC
}
