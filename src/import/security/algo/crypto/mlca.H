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

/* Flags to be defined by platform for Dilithium R2 V variant .
   NO_CRYSTALS_CIP , NO_CRYSTALS_KEX , NO_CRYSTALS_R3,
   NO_DILITHIUM_R2_3, NO_DILITHIUM_R2_4, USE_DYNAMIC_ALLOC

    Dilithium variants supported (K/L):
        round2
            Dil III   Dil-5/4
            Dil IV    Dil-6/5
            Dil V     Dil-8/7
*/

// Dilithium public key size (bytes)
#define DILITHIUM_R2_5_CRYPTO_PUBLICKEY_SIZE     2336

// Dilithium secrete key size (bytes)
#define DILITHIUM_R2_5_CRYPTO_SECRETKEY_SIZE     5136

// Dilithium signature size (bytes)
#define DILITHIUM_R2_5_CRYPTO_SIG_SIZE           4668

typedef uint8_t dilithium_r2_5_crypto_pub_key_t[DILITHIUM_R2_5_CRYPTO_PUBLICKEY_SIZE];
typedef uint8_t dilithium_r2_5_crypto_signature_t[DILITHIUM_R2_5_CRYPTO_SIG_SIZE];


// Enable of USE_DYNAMIC_ALLOC, The caller must support the mlca_alloc
// And mlca_free functions, which are used in the r2_verify function
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
 * @brief Verifies dilithium signature
 *
 * @param[in] sig Pointer to input signature
 * @param[in] siglen Length of signature
 * @param[in] m Pointer to message
 * @param[in] mlen Length of message
 * @param[in] pk Pointer to bit-packed public key
 * @param[in] pkbytes Length of public key
 *
 * @return > 0 if signature could be verified correctly and 0 otherwise
 */
int r2_verify(const uint8_t* sig, size_t siglen, const uint8_t* m,
              size_t mlen, const uint8_t* pk, size_t pkbytes);
