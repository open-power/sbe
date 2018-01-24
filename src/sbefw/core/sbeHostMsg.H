/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbeHostMsg.H $                                 */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016,2018                        */
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
        // There are 4 seeprom devices each of 64KB,
        // aso there is 1 ecc byte per 8 bytes of data
        uint32_t const MAX_SEEPROM_SIZE = ((65536 - (65536 % 9)) / 9) * 8 * 4;

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

#endif // __SBEFW_SBEHOST_MSG_H
