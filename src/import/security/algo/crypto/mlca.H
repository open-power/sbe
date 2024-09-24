/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/security/algo/crypto/mlca.H $                      */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2024                             */
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
/* EKB-Mirror-To: hw/ppe                                                  */
#pragma once

#include <stdint.h>
#include <stddef.h>

/**
 * Flags to be defined by platform for ML-DSA variant .
 * - USE_DYNAMIC_ALLOC : To use dynamic memory allocation
 * - NO_CRYSTALS_CIP, NO_CRYSTALS_KEX : Disable kyber algo
 * - NO_DILITHIUM_SIGN      : Disable the dilithium sign algo (r2/r3)
 * - NO_MLDSA_SIGN          : Disable ML-DSA sign algo ()
 * - NO_DILITHIUM_VERIFY    : Disable the dilithium verify algo (r2/r3)
 * - NO_MLDSA_VERIFY        : Disable ML-DSA verify algo
 *
 * ML-DSA Variant support
 * - DIL_MLDSA_44
 * - DIL_MLDSA_65
 * - DIL_MLDSA_87
 *
 * DIL_MLDSA_87 used in this project to verify the signature
 */

/// Dilithium public key size (bytes) @ref DIL_MLDSA_87_PUB_BYTES
#define DIL_MLDSA_87_CRYPTO_PUBLICKEY_SIZE     (2592)

/// Dilithium signature size (bytes) @ref DIL_MLDSA_SIGBYTES8x7
#define DIL_MLDSA_87_CRYPTO_SIG_SIZE           (4627)


// Enable of USE_DYNAMIC_ALLOC, The caller must support the mlca_alloc
// And mlca_free functions, which are used by dilithum
#if defined(USE_DYNAMIC_ALLOC)
    /**
    * @brief Allocate heap space of given size
    *
    * @param[in] size Size to allocate
    *
    * @return void* ( Generic pointer)
    */
    void* mlca_alloc(uint32_t size);

    /**
    * @brief Release allocated heap space by mlca_alloc
    *
    * @param[in] ptr Start address of allocated space
    *
    * @return void
    */
    void mlca_free(void* ptr);

#endif

/**
 * @brief ML-DSA dilithium verification
 *
 * @param[in] sig Pointer to input signature
 * @param[in] siglen Length of signature
 * @param[in] m Pointer to message
 * @param[in] mlen Length of message
 * @param[in] pk Pointer to bit-packed public key
 * @param[in] pkbytes Length of public key
 *
 * @return 0 (failure) or >0 (written bytecount)
 */
int mldsa_verify(const uint8_t* sig, size_t siglen,
                 const uint8_t* m, size_t mlen,
                 const uint8_t* pk, size_t pkbytes);