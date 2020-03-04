/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeHostMsg.H $                                 */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2020                        */
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
 * @file: ppe/sbe/sbefw/sbeHostMsg.H
 *
 * @brief This file contains the message structures for SBE Host
 *        communication.
 *
 */

#ifndef __SBEFW_SBEHOST_MSG_H
#define __SBEFW_SBEHOST_MSG_H

#include <stdint.h>
#include "sbe_host_intf.H"
#include "sbe_sp_intf.H"
#include "sbe_link.H"

/*****************************************************************/
/*            SBE->PSU request structures                        */
/*****************************************************************/

/**
  * @brief structure for Host->SBE command request format denoting
  *        mininum header (as of now, contained in mbx 0)
  */
typedef struct
{
    // mbxReg0
    uint64_t  res:16;
    uint64_t  flags:16;
    uint64_t  seqID:16;
    uint64_t  cmdClass:8;
    uint64_t  command:8;

    /**
      * @brief initialize the fields contained in PSU Mbx0
      *
      */
    void init()
    {
        res       = 0;
        flags     = 0;
        seqID     = 0;
        cmdClass  = SBE_PSU_CMD_CLASS_UNKNOWN;
        command   = SBE_PSU_CMD_UNKNOWN;
    }
} sbePsu2SbeCmdReqHdr_t;

/* @brief Address and size of memory allocated by Host
 *        for FFDC/pass through commands
 */
typedef struct
{
    uint32_t size;
    uint64_t addr;
} sbeHostAddr_t;

/* @brief Set FFDC Address message
 */
typedef struct
{
    uint64_t ffdcDataSize:32;
    uint64_t passCmdDataSize:32;
    uint64_t ffdcAddr;
    uint64_t passCmdDataAddr;

    void getFFDCAddr(sbeHostAddr_t &i_hostFFDCAddr)
    {
        i_hostFFDCAddr.size = ffdcDataSize;
        i_hostFFDCAddr.addr = ffdcAddr;
    }

    void getPassThroughCmdAddr(sbeHostAddr_t &i_hostPassCmdAddr)
    {
        i_hostPassCmdAddr.size = passCmdDataSize;
        i_hostPassCmdAddr.addr = passCmdDataAddr;
    }
} sbeSetFFDCAddrReq_t;

/* @brief Read SBE MEM structure
 */
typedef struct
{
    uint64_t offset:32;
    uint64_t size:32;
    uint64_t responseAddr;

    // validate request parameters
    bool validateReq()
    {
        // On ppe reading 8 bytes is optimal. So offset
        // should be multiple of 8.
        uint32_t const OFFSET_ALLIGNMENT = 8;
        // As we use PBA operation, size should be multiple
        // of 128bytes.
        uint32_t const SIZE_ALLIGNMENT = 128;
        // There 1 seeprom devices of 512KB, also there is 1 ecc byte per
        // 8 bytes of data, total usable size is (512KB/9)*8
        uint32_t const MAX_SEEPROM_SIZE = ((524288 - (524288 % 9)) / 9) * 8;

        return ( !(( offset % OFFSET_ALLIGNMENT != 0) ||
                  ( size % SIZE_ALLIGNMENT != 0 ) ||
                  ( ( offset + size ) > MAX_SEEPROM_SIZE )) );
    }
    // Return effective seeprom address
    uint64_t * getEffectiveAddr()
    {
        return ( uint64_t *)( SBE_SEEPROM_BASE_ORIGIN + offset );
    }
} sbeReadMemReq_t;

/*****************************************************************/
/*            SBE->PSU response structures                       */
/*****************************************************************/

/**
  * @brief SBE->Host Generic response structure
  *
  */
typedef struct
{
    uint64_t mbxReg4;
    uint64_t mbxReg5;
    uint64_t mbxReg6;
    uint64_t mbxReg7;
} sbeSbe2PsuGenericResp_t ;

/**
  * @brief Structure for SBE->Host response header contained in
  *        mbx4 register
  *
  */
typedef struct
{
    private:
    // mbxReg 4
    uint64_t  _primStatus:16;
    uint64_t  _secStatus:16;
    uint64_t  _seqID:16;
    uint64_t  _cmdClass:8;
    uint64_t  _command:8;

    public:
    const uint16_t primStatus() const { return _primStatus; }
    const uint16_t secStatus() const  { return _secStatus; }
    const uint16_t seqID() const  { return _seqID; }
    const uint8_t cmdClass() const  { return _cmdClass; }
    const uint8_t command() const  { return _command; }

    /**
      * @brief set the primary and secondary status
      *
      * @param[in] i_prim  Primary status
      * @param[in] i_sec   Secondary status
      *
      */
    void setStatus(const uint16_t i_prim, const uint16_t i_sec);

    /**
      * @brief initialize the response fields contained in PSU Mbx3
      *
      */
    void init();
} sbeSbe2PsuRespHdr_t;

/* @brief Format to dump out the architected register on the host memory
 * Host may use this format to fetch all the register data/state
 */

#define DUMP_STRUCT_VERSION_ID 0x1

/* Processor architected dump header. This header is per processor*/
typedef struct { 
    uint8_t  ownerId;        /* FSP or SBE */
    uint8_t  version;        /* Interface version number*/
    uint16_t core_cnt;       /* Cores per chip */
    uint16_t thread_count;   /* Max Threads per proc chip */
    uint16_t reg_cnt;        /* Max number of registers per thread */
} sbeArchRegDumpProcHdr_t;   //8Bytes


/* @brief Format of PIR */
typedef struct
{
    uint32_t procGrpId:21;
    uint32_t procChipId:3;
    uint32_t chipUnitNum:6;
    uint32_t thread:2;
} sbe_pir_t; //4Bytes

/*Thread Specific header*/
typedef struct
{
    sbe_pir_t pir;           // PIR value of thread corrsponding to the register
    uint32_t  coreState:8;   // State of core in which this thread is present
                             // Expect register data only if core status is '0'
    uint32_t  reserved:24;
} sbeArchRegDumpThreadHdr_t; //8Bytes


/**
 *  @brief Defines the structure for storing the SPR/GPR register data  
 *  
 *  @var isRegDataValid   :'1'- Variable regVal will contain valid register data
 *                         '0'- Variable regVal will contain fapiRC value
 *
 *  @var isLastReg        :'1' - Data is collected for the last SPR/GPR register
 *                         '0' - otherwise.
 *
 *  @var isFfdcPresent    :'1' - Failing FAPI FFDC is shared
 *                         '0' - Failing FFDC is not present
 *
 *  @var reserved0        :Reserved for future use and padding
 *
 *  @var regType          :Indicates type of register(SPR/GPR)  
 *
 *  @var reserved1        :Reserved for future use and padding
 *
 *  @var regNum           :Address of the SPR/GPR register
 *
 *  @var regVal           :if isRegDataValid is '0'  - Valid Register value
 *                            isRegDataValid is '!=0'- fapiRC value.
 */
typedef struct
{
    uint64_t isRegDataValid:1;
    uint64_t isLastReg:1;
    uint64_t isFfdcPresent:1;
    uint64_t reseverd0:5;
    uint64_t regType:8;
    uint64_t reserved1:16;
    uint64_t regNum:32;
    uint64_t regVal;
} sbeArchRegDumpEntries_t;



#endif // __SBEFW_SBEHOST_MSG_H
