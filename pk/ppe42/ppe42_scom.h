//-----------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2015
// *! All Rights Reserved -- Property of IBM
// *! *** IBM Confidential ***
//-----------------------------------------------------------------------------

/// \file  ppe42_scom.h
/// \brief Include file for PK SCOMs
///

#ifndef __PK_SCOM_H__
#define __PK_SCOM_H__

/// SCOM operations return non-zero error codes that may or may not indicate
/// an actual error, depending on which SCOM is begin accessed.  This error
/// code will appear in the MSR[SIBRC] field, bits[9:11] right after the 
/// SCOM OP returns.  The error code value increases with the severity of the 
/// error.
#define PCB_ERROR_NONE              0
#define PCB_ERROR_RESOURCE_OCCUPIED 1
#define PCB_ERROR_CHIPLET_OFFLINE   2
#define PCB_ERROR_PARTIAL_GOOD      3
#define PCB_ERROR_ADDRESS_ERROR     4
#define PCB_ERROR_CLOCK_ERROR       5
#define PCB_ERROR_PACKET_ERROR      6
#define PCB_ERROR_TIMEOUT           7

/// @brief putscom with absolute address
/// @param [in] i_address   Fully formed SCOM address
/// @param [in] i_data      uint64_t data to be written
///
/// @retval     On PPE42 platform, unmasked errors will take machine check interrupts
uint32_t putscom_abs( uint32_t i_address, uint64_t i_data);

/// @brief getscom with absolute address
/// @param [in]  i_address   Fully formed SCOM address
/// @param [in] *o_data      Pointer to uint64_t data read
///
/// @retval     On PPE42 platform, unmasked errors will take machine check interrupts
uint32_t getscom_abs( uint32_t i_address, uint64_t *o_data);


/// @brief Implementation of PPE putscom functionality
/// @param [in] i_chiplet   Chiplet ID (@todo Should only be right justified)
/// @param [in] i_address   Base SCOM address
/// @param [in] i_data      uint64_t data to be written
///
/// @retval     On PPE42 platform, unmasked errors will take machine check interrupts
uint32_t _putscom( uint32_t i_chiplet, uint32_t i_address, uint64_t i_data);


/// @brief Implementation of PPE getscom functionality
/// @param [in] i_chiplet   Chiplet ID (@todo Should only be right justified)
/// @param [in] i_address   Base SCOM address
/// @param [in] i_data      Pointer to uint64_t data read
///
/// @retval     On PPE42 platform, unmasked errors will take machine check interrupts
uint32_t _getscom( uint32_t i_chiplet, uint32_t i_address, uint64_t *o_data);


/// Macro to abstract the underlying putscom function so that it might be replaced
/// later with different implementations.  Used directly by low level PPE calls 
/// but also used by the FAPI2 API implementation
/*
#define putscom (_m_chiplet, _m_address, _m_data) { \
    _putscom( _m_chiplet, _m_address, _m_data); \
};
*/
extern inline uint32_t putscom(uint32_t i_chiplet, uint32_t i_address, uint64_t i_data)
{
    return _putscom(i_chiplet, i_address, i_data);
}

/// Macro to abstract the underlying getscom function so that it might be replaced
/// later with different implementations.  Used directly by low level PPE calls 
/// but also used by the FAPI2 API implementation
/*
#define getscom (i_chiplet, i_address, o_data) { \
    _getscom( i_chiplet, i_address, o_data); \
};
*/
extern inline uint32_t getscom(uint32_t i_chiplet, uint32_t i_address, uint64_t *o_data)
{
    return _getscom(i_chiplet, i_address, o_data);
}

#endif  // __PK_SCOM_H__
