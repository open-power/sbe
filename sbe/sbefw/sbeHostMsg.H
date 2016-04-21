/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: sbe/sbefw/sbeHostMsg.H $                                      */
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

extern sbePsu2SbeCmdReqHdr_t g_sbePsu2SbeCmdReqHdr;


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
    // mbxReg 4
    uint64_t  primStatus:16;
    uint64_t  secStatus:16;
    uint64_t  seqID:16;
    uint64_t  cmdClass:8;
    uint64_t  command:8;

    /**
      * @brief set the primary and secondary status
      *
      * @param[in] i_prim  Primary status
      * @param[in] i_sec   Secondary status
      *
      */
    void setStatus(const uint16_t i_prim, const uint16_t i_sec)
    {
        primStatus = i_prim;
        secStatus  = i_sec;
    }

    /**
      * @brief initialize the response fields contained in PSU Mbx3
      *
      */
    void init()
    {
        primStatus   = SBE_PRI_OPERATION_SUCCESSFUL;
        secStatus    = SBE_SEC_OPERATION_SUCCESSFUL;
        seqID        = g_sbePsu2SbeCmdReqHdr.seqID;
        cmdClass     = g_sbePsu2SbeCmdReqHdr.cmdClass;
        command      = g_sbePsu2SbeCmdReqHdr.command;
    }
} sbeSbe2PsuRespHdr_t;

extern sbeSbe2PsuRespHdr_t g_sbeSbe2PsuRespHdr;

#endif // __SBEFW_SBEHOST_MSG_H
