/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/hwpf/plat_hw_access.H $                                   */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2017                        */
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
/**
 *  @file plat_hw_access.H
 *
 *  @brief Define platform specific calls for hardware accesses.
 */

#ifndef PLATHWACCESS_H_
#define PLATHWACCESS_H_

#include <plat_includes.H>
#include "hw_access_def.H"
#include <return_code.H>
#include <fapi2_target.H>

// PIB errors
enum PCB_ERRORS
{
    PIB_NO_ERROR      = 0x0,
    PIB_XSCOM_ERROR   = 0x1,
    PIB_OFFLINE_ERROR = 0x2,
    PIB_PARTIAL_ERROR = 0x3,
    PIB_ADDRESS_ERROR = 0x4,
    PIB_CLOCK_ERROR   = 0x5,
    PIB_PARITY_ERROR  = 0x6,
    PIB_TIMEOUT_ERROR = 0x7
};

/// PIB Error Mask

#define PLAT_SET_PIB_ERROR_MASK(_m_mask) \
    {   /*  Read MSR */ \
        uint32_t msr_data = mfmsr(); \
        /* Set SEM field */ \
        msr_data &= ~(BITS(0,8)); \
        msr_data |= (uint32_t)(i_mask << 24); \
        /* Write MSR */ \
        mtmsr(msr_data); \
    };

#define PLAT_GET_PIB_ERROR_MASK(_m_mask) \
        uint8_t _m_mask; \
        uint32_t _sem = mfmsr(); \
        _m_mask = (uint8_t)((_sem & MSR_SEM) >> (32-(MSR_SEM_START_BIT + MSR_SEM_LEN)));

// Building block PPE instructions
#define PPE_MFMSR(_m_data) \
asm volatile \
    ( \
    "mfmsr %[data] \n" \
    :  [data]"=&r"(*_m_data) \
    :  "[data]"(*_m_data) \
    );

#define PPE_MTMSR(_m_data) \
asm volatile \
    ( \
    "mtmsr %[data] \n" \
    :  [data]"=&r"(*_m_data) \
    :  "[data]"(*_m_data) \
    );

/// GetScom
#define PLAT_GETSCOM(_m_rc, _m_base, _m_offset, _m_data) \
    _m_rc = fapi2::getscom_abs_wrap(&(_m_base), _m_offset, _m_data)

/// PutScom
#define PLAT_PUTSCOM(_m_rc, _m_base, _m_offset, _m_data) \
    _m_rc = fapi2::putscom_abs_wrap(&(_m_base), _m_offset, _m_data)

/// GetCFAM
#define PLAT_GETCFAM(_m_base, _m_offset, _m_data) \
    static_assert( K == TARGET_TYPE_NONE, \
                  "getCfamRegister is not supported by PPE platforms")

/// PutCFAM
#define PLAT_PUTCFAM(_m_base, _m_offset, _m_data) \
     static_assert( K == TARGET_TYPE_NONE, \
                  "putCfamRegister is not supported by PPE platforms")

/// ModifyCFAM
#define PLAT_MODCFAM(_m_base, _m_offset, _m_data, _m_mode) \
     static_assert( K == TARGET_TYPE_NONE, \
                  "modifyCfamRegister is not supported by PPE platforms")

namespace fapi2
{
    // This function loads the scan region data for the given ring address and
    // updates the check word data
    // @param[in] : ring addtress
    // @param[in]:  ring mode
    // @return fapi::ReturnCode. FAPI2_RC_SUCCESS if success, else error code.
    fapi2::ReturnCode getRing_setup(const uint32_t i_ringAddress,
                             const fapi2::RingMode i_ringMode);

    // This function verify the check word data is matching or not and will
    // clean up the scan region data
    // @param[in] i_ringAddress - absolute ring address
    // @param[in] i_ringMode - Ring mode value
    // @return fapi::ReturnCode. FAPI2_RC_SUCCESS if success, else error code.
    fapi2::ReturnCode getRing_verifyAndcleanup(const uint32_t i_ringAddress,
                                        const fapi2::RingMode i_ringMode);

    typedef union plat_target_handle plat_target_handle_t;
    ///
    /// @brief Platform wrapper over PK getscom_abs
    ///
    /// @param [in]  i_addr The SCOM address
    /// @param [out] o_data The data read
    ///
    /// @return fapi::ReturnCode. FAPI2_RC_SUCCESS if success, else error code.
    ///
    fapi2::ReturnCode getscom_abs_wrap(const void *i_target,
                                    const uint32_t i_addr, uint64_t *o_data);

    ///
    /// @brief Platform wrapper over PK putscom_abs
    ///
    /// @param [in] i_addr The SCOM address
    /// @param [in] i_data The data to write
    ///
    /// @return fapi::ReturnCode. FAPI2_RC_SUCCESS if success, else error code.
    ///
    fapi2::ReturnCode putscom_abs_wrap(const void *i_target,
                                    const uint32_t i_addr, uint64_t i_data);

    ///
    /// @brief Plat implementation of putScomUnderMask
    ///
    /// @param [in] i_target Pointer to fapi2::Target as a void*
    /// @param [in] i_addr The SCOM address
    /// @param [in] i_data The data to write (after applying the mask)
    /// @param [in] i_mask The data mask
    ///
    /// @return fapi2::ReturnCode. FAPI2_RC_SUCCESS if success, else error code.
    fapi2::ReturnCode putscom_under_mask(const void *i_target,
                                         uint32_t i_addr,
                                         uint64_t i_data,
                                         uint64_t i_mask);
    ///
    /// @brief Platform wrapper to handle pib errors on scoms
    ///
    /// @param [in] i_addr    The SCOM address
    /// @param [io] io_data   The data pointer
    /// @param [in] i_pibErr  PIB error code
    /// @param [in] i_isRead  True if read operation, false otherwise
    ///
    /// @return PCB-PIB return code
    ///
    uint32_t p9_handlePibErr( const uint32_t i_addr, uint64_t *io_data,
                             const uint8_t i_pibErr, const bool i_isRead);
}


#endif // PLATHWACCESS_H_

