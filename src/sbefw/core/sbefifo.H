/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/core/sbefifo.H $                                    */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2019                        */
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
 * @file: ppe/sbe/sbefw/sbefifo.H
 *
 * @brief This file contains basic SBE FIFO hardware specific
 *        definitions and operations.
 *
 */

#ifndef __SBEFW_SBEFIFO_H
#define __SBEFW_SBEFIFO_H

#include "sbeexeintf.H"
#include "sbetrace.H"
#include "ppe42_scom.h"
#include "sbe_sp_intf.H"
#include "sbeSpMsg.H"

/**
  * @brief SBE FIFO Access addresses
  *
  */
//Base address for SBE FIFO
const uint32_t SBE_FIFO_BASE                     =  0x000B0000;
//Base address for HB FIFO
const uint32_t SBE_HB_FIFO_BASE                  =  0x000B0020;

/**
  * @brief Enum of SBE Fifo type.
  */
enum sbeFifoType
{
   SBE_FIFO,
   SBE_HB_FIFO
};

/**
  * Macro for dynamically determining the base address
  */
#define FIFO_BASE_ADDR(FIFO)  ( (FIFO == SBE_HB_FIFO) ? SBE_HB_FIFO_BASE : SBE_FIFO_BASE )

const uint32_t SBE_UPSTREAM_FIFO_DEQ_ADD         = 0x0000;
const uint32_t SBE_UPSTREAM_FIFO_STATUS          = 0x0001;
const uint32_t SBE_UPSTREAM_FIFO_SIGNAL_EOT      = 0x0002;
const uint32_t SBE_UPSTREAM_FIFO_REQ_RESET       = 0x0003;
const uint32_t SBE_UPSTREAM_FIFO_PERFORM_RESET   = 0x0004;
const uint32_t SBE_UPSTREAM_FIFO_ACK_EOT         = 0x0005;

const uint32_t SBE_DOWNSTREAM_FIFO_ENQ_ADD       = 0x0010;
const uint32_t SBE_DOWNSTREAM_FIFO_STATUS        = 0x0011;
const uint32_t SBE_DOWNSTREAM_FIFO_SIGNAL_EOT    = 0x0012;
const uint32_t SBE_DOWNSTREAM_FIFO_REQ_RESET     = 0x0013;
const uint32_t SBE_DOWNSTREAM_FIFO_PERFORM_RESET = 0x0014;
const uint32_t SBE_DOWNSTREAM_FIFO_ACK_EOT       = 0x0015;


// Write this data to send EOT to DS FIFO. The register to send EOT
// is 32 bit only. But our scom operations are 64 bit. So set a bit
// in higher word to trigger EOT.
static const uint64_t DOWNSTREAM_EOT_DATA = 0x100000000ull;

/**
  * @brief SBE Upstream FIFO Status bits
  *
  */

typedef struct
{
    uint32_t   valid_flag:1;         // Bit 0
    uint32_t   eot_flag:1;           // Bit 1
    uint32_t   parity_err:1;         // Bit 2
    uint32_t   reserved3_5:3;        // Bit 3:5
    uint32_t   req_upfifo_reset:1;   // Bit 6
    uint32_t   req_downfifo_reset:1; // Bit 7
    uint32_t   signaling_eot:1;      // Bit 8
    uint32_t   reserved9:1;          // Bit 9
    uint32_t   fifo_full:1;          // Bit 10
    uint32_t   fifo_empty:1;         // Bit 11
    uint32_t   fifo_entry_count:4;   // Bit 12:15
    uint32_t   fifo_valid_flags:8;   // Bit 16:23
    uint32_t   fifo_eot_flags:8;     // Bit 24:31

} sbe_upfifo_status_t ;

/**
  * @brief 64-bit DW structure for Upstream FIFO Dequeue
  *        or Downstream FIFO Enqueue
  *        For Upstream FIFO,
  *            Bit 0 - 31 : Data, Bit 32 - 63 : Status
  *        For Downstream FIFO,
  *            Bit 0 - 31 : Data, Bit 32 - 63 : Unused
  *
  */
typedef struct
{
    uint32_t fifo_data;

    // The following status field is applicable only for
    // upstream FIFO access and will remain reserved for
    // downstream FIFO access
    union
    {
        sbe_upfifo_status_t statusOrReserved;
        uint32_t status;
    };
} sbeFifoEntry_t ;


/**
  * @brief 64-bit DW structure for Upstream FIFO Status Reg Read
  *            Bit 0 - 31 : Status Data, Bit 32 - 63 : Unused
  *
  */
typedef struct
{
    sbe_upfifo_status_t upfifo_status;
    uint32_t reserved;
} sbeUpFifoStatusReg_t ;


/**
  * @brief SBE Downstream FIFO Status bits
  *
  */
typedef struct
{
    uint32_t   reserved0_1:2;        // Bit 0:1
    uint32_t   parity_err:1;         // Bit 2
    uint32_t   reserved3_5:3;        // Bit 3:5
    uint32_t   req_downfifo_reset:1; // Bit 6
    uint32_t   req_upfifo_reset:1;   // Bit 7
    uint32_t   signaling_eot:1;      // Bit 8
    uint32_t   reserved9:1;          // Bit 9
    uint32_t   fifo_full:1;          // Bit 10
    uint32_t   fifo_empty:1;         // Bit 11
    uint32_t   fifo_entry_count:4;   // Bit 12:15
    uint32_t   fifo_valid_flags:8;   // Bit 16:23
    uint32_t   fifo_eot_flags:8;     // Bit 24:31

} sbe_downfifo_status_t ;

/**
  * @brief 64-bit DW structure for Downstream FIFO Status Reg Read
  *          Bit 0 - 31 : Status Data, Bit 32 - 63 : Unused
  *
  */
typedef struct
{
    sbe_downfifo_status_t downfifo_status;
    uint32_t reserved;
} sbeDownFifoStatusReg_t;

/*****************************************************************/
/** Upstream FIFO access utilities **/
/*****************************************************************/

/**
  * @brief sbeUpFifoDeq : Read entry and status from Upstream FIFO
  *
  * @param[out] 64-Bit Data read from Upstream FIFO
  *
  * @param sbeFifoType: i_type
  *
  * @return     Rc from the underlying scom utility
  *
  */
extern inline uint32_t sbeUpFifoDeq (uint64_t *o_data, const sbeFifoType i_type = SBE_FIFO)
{
    /* For SBE FIFO (PIB) access, chiplet ID should be passed as 0 */
    return getscom_abs(SBE_UPSTREAM_FIFO_DEQ_ADD + FIFO_BASE_ADDR(i_type), o_data);
}


/**
  * @brief sbeUpFifoPerformReset : Perform Upstream FIFO reset request
  *
  * @param sbeFifoType: i_type
  *
  * @return     Rc from the underlying scom utility
  *
  */
extern inline uint32_t sbeUpFifoPerformReset (const sbeFifoType i_type = SBE_FIFO)
{
    SBE_INFO("sbeUpFifoPerformReset");
    return putscom_abs(SBE_UPSTREAM_FIFO_PERFORM_RESET + FIFO_BASE_ADDR(i_type), ((uint64_t)0x1)<<32);
}


/**
  * @brief sbeUpFifoAckEot : Acknowledge EOT in Upstream FIFO
  *
  * @param sbeFifoType: i_type
  *
  * @return     Rc from the underlying scom utility
  *
  */
extern inline uint32_t sbeUpFifoAckEot (const sbeFifoType i_type = SBE_FIFO)
{
    SBE_INFO("sbeUpFifoAckEot");
    return putscom_abs(SBE_UPSTREAM_FIFO_ACK_EOT + FIFO_BASE_ADDR(i_type), ((uint64_t)0x1)<<32);
}


/*****************************************************************/
/** Downstream FIFO access utilities **/
/*****************************************************************/

/**
  * @brief sbeDownFifoEnq : Write data into Downstream FIFO
  *
  * @param[in] 64-Bit Data write into Downstream FIFO
  *            Bit 0-31  : Data
  *            Bit 32-63 : Unused
  *
  * @param sbeFifoType: i_type
  *
  * @return    Rc from the underlying scom utility
  */
extern inline uint32_t sbeDownFifoEnq (const uint64_t i_data, const sbeFifoType i_type = SBE_FIFO)
{
    SBE_DEBUG(">sbeDownFifoEnq");
    return putscom_abs(SBE_DOWNSTREAM_FIFO_ENQ_ADD + FIFO_BASE_ADDR(i_type), i_data);
}


/**
  * @brief sbeDownFifoGetStatus : Read status from downstream FIFO
  *
  * @param[out] 64-Bit Read status from downstream FIFO
  *            Bit 0-31  : Data
  *            Bit 32-63 : Unused
  *
  * @param sbeFifoType: i_type
  *
  * @return    Rc from the underlying scom utility
  */
extern inline uint32_t sbeDownFifoGetStatus (uint64_t *o_data, const sbeFifoType i_type = SBE_FIFO)
{
    return getscom_abs(SBE_DOWNSTREAM_FIFO_STATUS + FIFO_BASE_ADDR(i_type), o_data);
}

/**
  * @brief sbeDownFifoWriteEot : Write the EOT for Down Stream Fifo
  *
  * @param sbeFifoType: i_type
  *
  * @return     Rc from the underlying scom utility
  *
  */
extern inline uint32_t sbeDownFifoWriteEot ( const sbeFifoType i_type = SBE_FIFO )
{
    return putscom_abs(SBE_DOWNSTREAM_FIFO_SIGNAL_EOT + FIFO_BASE_ADDR(i_type), DOWNSTREAM_EOT_DATA);
}

#endif // __SBEFW_SBEFIFO_H
