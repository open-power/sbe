/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeFFDC.H $                                    */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2024                        */
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
#ifndef __SBE_FFDC_H
#define __SBE_FFDC_H

#include "fapi2.H"
#include "sbefifo.H"
#include "plat_attributes.H"
#include "pk_trace.h"
#include "sbeFFDCType.H"
#include "sbeSpMsg.H"
#include "sbeHostUtils.H"
#include "sbe_build_info.H"
#include "sbeMemAccessInterface.H"

//PIBMEM attribute dump
extern G_sbe_attrs_t G_sbe_attrs;

// CollectRegisterFfdc dump
extern fapi2::SbeFfdcData_t G_sbe_reg_ffdc;

/*
 * @brief - Capture Async failure FFDC
 *
 * @param[in] primRc Primary failure rc
 * @param[in] secRc  Secondary failure rc
 *
 */
void captureAsyncFFDC(uint32_t primRc,
                      uint32_t secRc);

//Configuration of user data blobs present in SBE FFDC
//Data is sent in the order defined here
//Definition - Identifier
//             length of the blob
//             pointer to the data
const sbeFFDCUserData_t sbeFFDCUserDataArray[] =
                                              {{{SBE_FFDC_TRACE_DUMP,
                                                sizeof(PkTraceBuffer)},
                                                (const void *)&g_pk_trace_buf,
                                                },
                                               {{SBE_FFDC_ATTR_DUMP,
                                                sizeof(G_sbe_attrs_t)},
                                                (const void *)&G_sbe_attrs,
                                                },
                                               {{SBE_FFDC_COLLECT_REG_DUMP,
                                                sizeof(SbeFfdcData_t)},
                                                (const void *)&G_sbe_reg_ffdc,
                                                },
                                              };

#define NUM_USER_DATA_ELE (sizeof(sbeFFDCUserDataArray)/sizeof(sbeFFDCUserData_t))

//SBE internal FFDC package class
class SbeFFDCPackage
{
private:
    //Disable copy constructor
    SbeFFDCPackage(SbeFFDCPackage const &) = delete;
    //Disable assignment operator
    SbeFFDCPackage& operator=(SbeFFDCPackage const &) = delete;

    sbeResponseFfdc_t iv_sbeFFDCHeader;
    //FFDC user data header
    sbeFFDCDataHeader_t iv_sbeFFDCDataHeader;

    /*
     * @bried updateUserDataHeader - method to update user data fields
     *                               based on input config
     *
     * @param[in] i_fieldsConfig   - input fields configuration
     */
    void updateUserDataHeader(uint32_t i_fieldsConfig);

public:
    /*ctor
     *
     */
    SbeFFDCPackage()
    {
        //Making sure data is indeed aligned
        static_assert((sizeof(G_sbe_attrs_t) % 4) == 0,
                        "G_sbe_attrs not 4byte aligned");
        static_assert((sizeof(PkTraceBuffer) % 4) == 0,
                        "g_pk_trace_buf not 4byte aligned");
        static_assert((sizeof(SbeFfdcData_t) % 4) == 0,
                        "G_sbe_reg_ffdc not 4byte aligned");

        iv_sbeFFDCHeader.fapiRc = fapi2::FAPI2_RC_PLAT_ERR_SEE_DATA;

        iv_sbeFFDCDataHeader.primaryStatus = SBE_PRI_OPERATION_SUCCESSFUL;
        iv_sbeFFDCDataHeader.secondaryStatus = SBE_SEC_OPERATION_SUCCESSFUL;
        iv_sbeFFDCDataHeader.fwCommitID = SBE_COMMIT_ID;
        iv_sbeFFDCDataHeader.ddLevel = SBE_FFDC_DD1;

        //length and dumpFields will be filled up depending on the fields
        //to be sent in send APIs
        iv_sbeFFDCDataHeader.dumpFields = {0};
    }

    /*
     * @brief collectAsyncHwpFfdc    - method to check and force collect
     *                                 HWP FFDC to SBE global FFDC region,
     *                                 asynchronous to the HWP execution
     * @return                       - SBE secondary RC
     */
    //uint32_t collectAsyncHwpFfdc (void);

    /*
     * @brief sendOverFIFO           - method to pack and send SBE internal FFDC
     *                                 only if isSendInternalFFDCSet() is true
     *                                 over FIFO interface
     * @param[in] i_fieldsConfig     - bitmap indicating the field
     *                                 to be sent in FFDC
     * @param[out] o_bytesSent       - number of bytes sent
     * @param[in] i_skipffdcBitCheck - Boolean to indicate whether
     *                                 ffdc bit should be checked or not.
     *                                 By default it is false.
     * @param[in]                    - i_type FIFO type
     *                                 The default fifo type is FIFO:0
     *
     * @return                       - SBE secondary RC
     */
    uint32_t sendOverFIFO(const uint32_t i_fieldsConfig,
                          uint32_t &o_bytesSent,
                          const bool i_skipffdcBitCheck = false,
                          sbeFifoType i_type = SBE_FIFO);

    /* @brief sendOverHostIntf       - method to pack and send SBE internal FFDC
     *                                 only if isSendInternalFFDCSet() is true
     *                                 over HOST interface
     *
     * @param[in] i_fieldsConfig     - bitmap indicating the field
     *                                 to be sent in FFDC
     * @param[in] i_pMemInterface    - pointer to memory interface object
     * @param[in] i_allocatedSize    - size allocated for FFDC
     * @param[in] i_skipffdcBitCheck - Boolean to indicate whether
     *                                 ffdc bit should be checked or not.
     *                                 By default it is false.
     *
     * @return                       - SBE secondary RC
     */
    uint32_t sendOverHostIntf(const uint32_t i_fieldsConfig,
                              sbeMemAccessInterface *i_pMemInterface,
                              uint32_t i_allocatedSize,
                              const bool i_skipffdcBitCheck = false);
};

#endif //__SBE_FFDC_H
