/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/sbeHostUtils.H $                                    */
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
 * @file: ppe/sbe/sbefw/sbeHostUtils.H
 *
 * @brief This file contains basic SBE PSU hardware specific
 *        definitions and operations.
 *
 */

#ifndef __SBEFW_SBEHOSTUTILS_H
#define __SBEFW_SBEHOSTUTILS_H

#include <stdint.h>
#include "ppe42_scom.h"
#include "sbeHostMsg.H"

/**
  * @brief SBE PSU Access register addresses
  *
  */
const uint32_t SBE_HOST_PSU_BASE                 = 0x000D0050;

const uint32_t SBE_HOST_PSU_MBOX_REG0            = SBE_HOST_PSU_BASE + 0x0000;
const uint32_t SBE_HOST_PSU_MBOX_REG1            = SBE_HOST_PSU_BASE + 0x0001;
const uint32_t SBE_HOST_PSU_MBOX_REG2            = SBE_HOST_PSU_BASE + 0x0002;
const uint32_t SBE_HOST_PSU_MBOX_REG3            = SBE_HOST_PSU_BASE + 0x0003;
const uint32_t SBE_HOST_PSU_MBOX_REG4            = SBE_HOST_PSU_BASE + 0x0004;
const uint32_t SBE_HOST_PSU_MBOX_REG5            = SBE_HOST_PSU_BASE + 0x0005;
const uint32_t SBE_HOST_PSU_MBOX_REG6            = SBE_HOST_PSU_BASE + 0x0006;
const uint32_t SBE_HOST_PSU_MBOX_REG7            = SBE_HOST_PSU_BASE + 0x0007;


const uint32_t SBE_PSU2SBE_DOORBELL_REG_RW       = 0x000D0060;
const uint32_t SBE_PSU2SBE_DOORBELL_REG_AND      = 0x000D0061;
const uint32_t SBE_PSU2SBE_DOORBELL_REG_OR       = 0x000D0062;
const uint32_t SBE_PSU2HOST_DOORBELL_REG_RW       = 0x000D0063;
const uint32_t SBE_PSU2HOST_DOORBELL_REG_AND      = 0x000D0064;
const uint32_t SBE_PSU2HOST_DOORBELL_REG_OR       = 0x000D0065;


/**
  * @brief SBE PSU door bell register Bit definitions
  */
typedef enum
{
    // Bit 0 AND flag for PSU->SBE Doorbell Register;
    // When this is set by the host firmware, it would trigger an
    // interrupt to the SBE about a waiting message in the Host/SBE
    // Mailbox Registers. SBE is responsible for clearing this
    // bit upon being interrupted.
    SBE_PSU2SBE_DOORBELL_CLEAR_BIT0      = 0x7FFFFFFFFFFFFFFFull,
} sbe_psu2sbeDoorbellReg_UpdateFlags;

typedef enum
{
    // Bit 0 OR flag for SBE->PSU Doorbell Register;
    // When this is set by SBE, it would trigger an interrupt to host
    // firmware about a waiting response in the Host/SBE Mailbox Registers 4-7
    SBE_SBE2PSU_DOORBELL_SET_BIT0      = 0x8000000000000000ull,

    // Bit 1 OR flag for SBE->PSU Doorbell Register;
    // When this is set by SBE, it would trigger an interrupt to host
    // firmware indicating that the message in Host/SBE mailbox registers 0-3
    // has been read by SBE and is being processed
    SBE_SBE2PSU_DOORBELL_SET_BIT1      = 0x4000000000000000ull,

    // Bit 2 OR flag for SBE->PSU Doorbell Register;
    // When this is set by SBE, it would trigger an interrupt to host
    // firmware to trigger Stop15 exit on thread 0 on the master core.
    // This would be used to trigger hostboot in istep 16
    SBE_SBE2PSU_DOORBELL_SET_BIT2      = 0x2000000000000000ull,
} sbe_sbe2psuDoorbellReg_UpdateFlags;


/*****************************************************************/
/** PSU->SBE register access utilities **/
/*****************************************************************/

/**
  * @brief sbeClearPsu2SbeDbBitX : Clear given bit in PSU->SBE DB register
  *
  * @param[in] 64-Bit Scom Data indicating bit position to be cleared
  *
  * @return    RC from the underlying scom utility
  *
  */
inline uint32_t sbeClearPsu2SbeDbBitX (const uint64_t i_bit)
{
    return putscom_abs (SBE_PSU2SBE_DOORBELL_REG_AND, i_bit) ;
}


/**
  * @brief sbeReadPsu2SbeDbReg : Read PSU->SBE DB register
  *
  * @param[out] 64-Bit Data read from PSU->SBE DB register
  *
  * @return     RC from the underlying scom utility
  *
  */
inline uint32_t sbeReadPsu2SbeDbReg (uint64_t *o_data)
{
    return getscom_abs (SBE_PSU2SBE_DOORBELL_REG_RW, o_data) ;
}


/**
  * @brief sbeReadPsu2SbeMbxReg : Read from PSU->SBE MBX register(s)
  *
  * @param[in]     i_addr
  *    Starting Mbx Register Address
  * @param[in]     i_count
  *    Number of Mbx registers to be read.
  * @param[in] i_isFinalRead
  *    Indicates if its a final read operation for this chip op and
  *    internally handle the ack
  *    By default it is false.
  * @param[out]    o_pData
  *    Contents of the Mbx registers read into the buffer
  * @return  Return Code
  *    SUCCESS or TBD
  */
uint32_t sbeReadPsu2SbeMbxReg (uint32_t       i_addr,
                               const uint8_t  i_count,
                               uint64_t       *o_pData,
                               bool i_isFinalRead = false);

/*****************************************************************/
/** SBE->PSU register access utilities **/
/*****************************************************************/

/**
  * @brief sbeSetSbe2PsuDbBitX : Set Bit-X in SBE->PSU DB register
  *
  * @param[in] 64-Bit Scom Data indicating bit position
  *
  * @return    RC from the underlying scom utility
  *
  */
inline uint32_t sbeSetSbe2PsuDbBitX (const uint64_t i_bit)
{
    return putscom_abs (SBE_PSU2HOST_DOORBELL_REG_OR, i_bit) ;
}

/**
  * @brief sbeClearSbe2PsuDbBitX : Clear Bit-X in SBE->PSU DB register
  *
  * @param[in] 64-Bit Scom Data indicating bit position
  *
  * @return    RC from the underlying scom utility
  *
  */
inline uint32_t sbeClearSbe2PsuDbBitX (const uint64_t i_bit)
{
    return putscom_abs (SBE_PSU2HOST_DOORBELL_REG_AND, i_bit) ;
}

/**
  * @brief sbeReadSbe2PsuDbReg : Read SBE->PSU DB register
  *
  * @param[out] 64-Bit Data read from SBE->PSU DB register
  *
  * @return     RC from the underlying scom utility
  *
  */
inline uint32_t sbeReadSbe2PsuDbReg (uint64_t *o_data)
{
    return getscom_abs (SBE_PSU2HOST_DOORBELL_REG_RW, o_data) ;
}

/**
  * @brief sbeAcknowledgeHost : Acknowldge by setting bit 1 in
  *      SBE->PSU DB register if the host had requested for an ack
  *
  * @return    RC from the underlying scom utility
  *
  */
uint32_t sbeAcknowledgeHost();

/**
  * @brief sbeIntrHostUponRespWaiting : Interrupt Host by
  *      setting bit 0 in SBE->PSU DB register if the host had
  *      requested for response
  *
  * @return    RC from the underlying scom utility
  *
  */
uint32_t sbeIntrHostUponRespWaiting();


/**
  * @brief sbeWriteSbe2PsuMbxReg : Write to SBE->PSU MBX register(s)
  *
  * @param[in]     i_addr
  *    Starting Mbx Register Address
  * @param[in]     i_pData
  *    Contains the data to be written into given Mbx registers
  * @param[in]     i_count
  *    Number of Mbx registers to be written.
  * @param[in] i_setBit0ToIntrHB
  *    Indicates whether to write Bit0 to interrupt the Host,
  *    By default it is false.
  * @return  Return Code
  *    SUCCESS or TBD
  */
uint32_t sbeWriteSbe2PsuMbxReg (uint32_t        i_addr,
                                const uint64_t  *i_pData,
                                const uint8_t   i_count,
                                bool            i_setBit0ToIntrHB = false);

/* @brief - Send PSU Chip Op response
 *
 * @param[in] - i_sbe2PsuRespHdr - Response header
 * @param[in] - i_fapiRc - fapi rc of the relevant hwp call
 * @param[in/out] - io_rc - rc status of the PSU access utility
 *
 * @return - void
 */
void sbePSUSendResponse(sbeSbe2PsuRespHdr_t &i_sbe2PsuRespHdr,
                        uint32_t &i_fapiRc,
                        uint32_t &io_rc);
#endif // __SBEFW_SBEHOSTUTILS_H
