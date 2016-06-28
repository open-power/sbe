/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: sbe/hwpf/include/plat/plat_hw_access.H $                      */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
    _m_rc = getscom_abs(getEffectiveAddress(_m_base, _m_offset), _m_data)

/// PutScom
#define PLAT_PUTSCOM(_m_rc, _m_base, _m_offset, _m_data) \
    _m_rc = putscom_abs(getEffectiveAddress(_m_base, _m_offset), _m_data)

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

    // This function read the 64 bit data from the hardware
    // @param[in] i_ringAddress - absolute ring address
    // @param [out]: 64 bit data
    // @param [in] i_bitShiftValue - Bit shift value that needs to rotate
    // @note- If the ring length is divisble by 64, then bitshift will always be
    // 64, else need to store the ring_length mod 64 and send that value in the
    // last iteration.
    // @return fapi::ReturnCode. FAPI2_RC_SUCCESS if success, else error code.
    fapi2::ReturnCode getRing_granule_data(const uint32_t i_ringAddress,
                                    uint64_t *o_data,
                                    const uint32_t i_bitShiftValue);

    // This function verify the check word data is matching or not and will
    // clean up the scan region data
    // @param[in] i_ringAddress - absolute ring address
    // @param[in] i_ringMode - Ring mode value
    // @return fapi::ReturnCode. FAPI2_RC_SUCCESS if success, else error code.
    fapi2::ReturnCode getRing_verifyAndcleanup(const uint32_t i_ringAddress,
                                        const fapi2::RingMode i_ringMode);
}


#endif // PLATHWACCESS_H_

