/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: sbe/sbefw/sbeFFDC.C $                                         */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
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
#include "sbefifo.H"
#include "sbetrace.H"
#include "sbe_sp_intf.H"
#include "sbeFifoMsgUtils.H"
#include "sberegaccess.H"
#include "sbeFFDC.H"

/*
 * @brief sendOverFIFO - method to pack and send SBE internal FFDC
 *                       only if isSendInternalFFDCSet() is true
 *                       over FIFO interface
 * @param[in] i_primStatus   - Primary status of Chip op
 * @param[in] i_secStatus    - Secondary status of Chip op
 * @param[in] i_fieldsConfig - bitmap indicating the field
 *                             to be sent in FFDC
 *
 * @param[out] o_bytesSent -number of bytes sent
 *
 * @return - SBE secondary RC
 */
uint32_t SbeFFDCPackage::sendOverFIFO(uint32_t i_primStatus,
                                      uint32_t i_secStatus,
                                      uint32_t i_fieldsConfig,
                                      uint32_t &o_bytesSent)
{
    #define SBE_FUNC "sbeSendFFDCPackageFIFO "
    SBE_ENTER(SBE_FUNC);
    uint32_t rc = SBE_SEC_OPERATION_SUCCESSFUL;
    uint32_t length = 0;

    do
    {
        //check if SBE internal FFDC should be generated
        if(SbeRegAccess::theSbeRegAccess().isSendInternalFFDCSet() == false)
        {
            SBE_DEBUG(SBE_FUNC" isSendInternalFFDCSet()=false, "
                    "not generating SBE InternalFFDC");
            rc = SBE_SEC_OPERATION_SUCCESSFUL;
            break;
        }

        //reset sent bytes
        o_bytesSent = 0;
        //Update the user data header with dump fields configuration
        iv_sbeFFDCDataHeader.dumpFields.set(i_fieldsConfig);
        iv_sbeFFDCHeader.lenInWords = (sizeof(sbeResponseFfdc_t) +
                                       sizeof(sbeFFDCDataHeader_t))
                                       /sizeof(uint32_t);
        //Update the length in ffdc package header base on required fields
        for(auto &sbeFFDCUserData:sbeFFDCUserDataArray)
        {
            if(sbeFFDCUserData.userDataId.fieldId & i_fieldsConfig)
            {
                iv_sbeFFDCHeader.lenInWords +=
                                        (sbeFFDCUserData.userDataId.fieldLen +
                                         sizeof(sbeFFDCUserDataIdentifier_t))
                                         /sizeof(uint32_t);
            }
        }

        SBE_DEBUG(SBE_FUNC "length of FFDC package in words [%d]",
                (uint32_t)(iv_sbeFFDCHeader.lenInWords));

        //Send FFDC package header
        length = sizeof(iv_sbeFFDCHeader) / sizeof(uint32_t);
        rc = sbeDownFifoEnq_mult(length,
                                 (uint32_t *)(&(iv_sbeFFDCHeader)));
        if( rc!= SBE_SEC_OPERATION_SUCCESSFUL)
        {
            break;
        }
        o_bytesSent += length;

        //Send FFDC user data header
        length = sizeof(iv_sbeFFDCDataHeader) / sizeof(uint32_t);
        rc = sbeDownFifoEnq_mult(length,
                                 (uint32_t *)(&(iv_sbeFFDCDataHeader)));
        if( rc!= SBE_SEC_OPERATION_SUCCESSFUL)
        {
            break;
        }
        o_bytesSent += length;

        //Send FFDC user data blobs
        for(auto &sbeFFDCUserData:sbeFFDCUserDataArray)
        {
            if(sbeFFDCUserData.userDataId.fieldId & i_fieldsConfig)
            {
                //Send User data identifer and length
                length = sizeof(sbeFFDCUserDataIdentifier_t) / sizeof(uint32_t);
                rc = sbeDownFifoEnq_mult(length,
                                    (uint32_t*)&(sbeFFDCUserData.userDataId));
                if( rc!= SBE_SEC_OPERATION_SUCCESSFUL)
                {
                    break;
                }
                o_bytesSent += length;

                //Send User data
                length = sbeFFDCUserData.userDataId.fieldLen / sizeof(uint32_t);
                rc = sbeDownFifoEnq_mult(length,
                                        (uint32_t*)sbeFFDCUserData.userDataPtr);
                if( rc!= SBE_SEC_OPERATION_SUCCESSFUL)
                {
                    break;
                }
                o_bytesSent += length;
            }
        }

        SBE_DEBUG(SBE_FUNC "Number of words sent [%d]", o_bytesSent);
    } while(false);

    SBE_EXIT(SBE_FUNC);
    return rc;
    #undef SBE_FUNC
}
