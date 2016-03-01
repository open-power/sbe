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
const uint32_t SBE_FIFO_BASE                     =  0x000B0000;

const uint32_t SBE_UPSTREAM_FIFO_DEQ_ADD         = SBE_FIFO_BASE + 0x0000;
const uint32_t SBE_UPSTREAM_FIFO_STATUS          = SBE_FIFO_BASE + 0x0001;
const uint32_t SBE_UPSTREAM_FIFO_SIGNAL_EOT      = SBE_FIFO_BASE + 0x0002;
const uint32_t SBE_UPSTREAM_FIFO_REQ_RESET       = SBE_FIFO_BASE + 0x0003;
const uint32_t SBE_UPSTREAM_FIFO_PERFORM_RESET   = SBE_FIFO_BASE + 0x0004;
const uint32_t SBE_UPSTREAM_FIFO_ACK_EOT         = SBE_FIFO_BASE + 0x0005;

const uint32_t SBE_DOWNSTREAM_FIFO_ENQ_ADD       = SBE_FIFO_BASE + 0x0010;
const uint32_t SBE_DOWNSTREAM_FIFO_STATUS        = SBE_FIFO_BASE + 0x0011;
const uint32_t SBE_DOWNSTREAM_FIFO_SIGNAL_EOT    = SBE_FIFO_BASE + 0x0012;
const uint32_t SBE_DOWNSTREAM_FIFO_REQ_RESET     = SBE_FIFO_BASE + 0x0013;
const uint32_t SBE_DOWNSTREAM_FIFO_PERFORM_RESET = SBE_FIFO_BASE + 0x0014;
const uint32_t SBE_DOWNSTREAM_FIFO_ACK_EOT       = SBE_FIFO_BASE + 0x0015;

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
    sbe_upfifo_status_t statusOrReserved;
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

// @TODO via RTC 142985: move to a better file with final fix
// Do a R-M-W to retain un-related bits
/**
 *  @brief Update SBE Messaging Register with the SBE states
 */
const uint32_t SBE_SCOM_MESSAGE_REGISTER          = 0x50009;
const uint64_t SBE_STATE_PK_READY                 = 0x8000000000000000ull;

#define SBE_UPDATE_SBE_MSG_REG(value) \
    putscom_abs(SBE_SCOM_MESSAGE_REGISTER, (value));

/*****************************************************************/
/** Upstream FIFO access utilities **/
/*****************************************************************/

/**
  * @brief sbeUpFifoDeq : Read entry and status from Upstream FIFO
  *
  * @param[out] 64-Bit Data read from Upstream FIFO
  *
  * @return     Rc from the underlying scom utility
  *
  */
extern inline uint32_t sbeUpFifoDeq (uint64_t *o_data)
{
    /* For SBE FIFO (PIB) access, chiplet ID should be passed as 0 */
    return getscom_abs(SBE_UPSTREAM_FIFO_DEQ_ADD, o_data);
}


/**
  * @brief sbeUpFifoPerformReset : Perform Upstream FIFO reset request
  *
  * @return     Rc from the underlying scom utility
  *
  */
extern inline uint32_t sbeUpFifoPerformReset (void)
{
    SBE_TRACE(">sbeUpFifoPerformReset");
    return putscom_abs(SBE_UPSTREAM_FIFO_PERFORM_RESET, ((uint64_t)0x1)<<32);
}


/**
  * @brief sbeUpFifoAckEot : Acknowledge EOT in Upstream FIFO
  *
  * @return     Rc from the underlying scom utility
  *
  */
extern inline uint32_t sbeUpFifoAckEot (void)
{
    SBE_DEBUG("sbeUpFifoAckEot");
    return putscom_abs(SBE_UPSTREAM_FIFO_ACK_EOT, ((uint64_t)0x1)<<32);
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
  * @return    Rc from the underlying scom utility
  */
extern inline uint32_t sbeDownFifoEnq (const uint64_t i_data)
{
    SBE_DEBUG(">sbeDownFifoEnq");
    return putscom_abs(SBE_DOWNSTREAM_FIFO_ENQ_ADD, i_data);
}


/**
  * @brief sbeDownFifoGetStatus : Read status from downstream FIFO
  *
  * @param[out] 64-Bit Read status from downstream FIFO
  *            Bit 0-31  : Data
  *            Bit 32-63 : Unused
  *
  * @return    Rc from the underlying scom utility
  */
extern inline uint32_t sbeDownFifoGetStatus (uint64_t *o_data)
{
    return getscom_abs(SBE_DOWNSTREAM_FIFO_STATUS, o_data);
}

#endif // __SBEFW_SBEFIFO_H
