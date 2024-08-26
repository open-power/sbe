/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/security/algo/crypto/keccak.H $                    */
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
// SPDX-License-Identifier: Apache-2.0
// EKB-Mirror-To: hw/ppe

#ifndef MLCA_KECCAK_H
#define MLCA_KECCAK_H

#include <stdint.h>
#include <stddef.h>

#define KECCAK_P            1600
#define KECCAK_P_64L        (KECCAK_P / 64)
#define KECCAK_ROUNDS       24
#define SHAKE128_RATE       ((KECCAK_P - 256) / 8)
#define SHAKE128_CAPACITY   (KECCAK_P - (SHAKE128_RATE))
#define SHAKE256_RATE       ((KECCAK_P - 512) / 8)
#define SHAKE256_CAPACITY   (KECCAK_P - (SHAKE256_RATE))
#define SHAKE_DOMAIN_SEP    0x1F
#define SHA3_DOMAIN_SEP     0x06
#define SHA3_256_RATE       ((KECCAK_P - 512) / 8)
#define SHA3_512_RATE       ((KECCAK_P - 1024) / 8)

struct KeccakF1600State
{
    uint64_t A[KECCAK_P_64L]; /** KeccakF1600 state array, 1600 bit */
    size_t   idx;             /** index in state (bytes) during absorbing */
};

typedef struct KeccakF1600State Keccak_state;

/** Non-incremental API for SHAKE128 / SHAKE256 */
/**
 * SHAKE128 non-incremental call
*/
void shake128(uint8_t* out, size_t outlen, const uint8_t* in, size_t inlen);
/**
 * SHAKE256 non-incremental call
*/
void shake256(uint8_t* out, size_t outlen, const uint8_t* in, size_t inlen);

/** Non-incremental API for SHA3-256 / SHA3-512 */
/**
 * SHA3-256 non-incremental call
*/
void sha3_256(uint8_t h[32], const uint8_t* in, size_t inlen);

/**
 * SHA3-512 non-incremental call
*/
void sha3_512(uint8_t h[64], const uint8_t* in, size_t inlen);

/** Incremental API for SHA3-512 */

/**
 * SHA3-512 state initialization
*/
void sha3_512_init(Keccak_state* state);

/**
 * SHA3-512 update bytewise
*/
void sha3_512_update(Keccak_state* state, const void* data, size_t len);

/**
 * SHA3-512 finalization, outputting the message digest
*/
void sha3_512_final(uint8_t md[64], Keccak_state* c);

/** Incremental API for SHAKE128 / SHAKE256
 *
 * Valid call sequences for absorbing are:
 * 1.
 * - shake(128,256)_init (once)
 * - shake(128,256)_absorb (n times)
 * - shake(128,256)_finalize (once)
 *
 * 2.
 * - shake(128,256)_init (once)
 * - shake(128,256)_absorb_once (once)
 *
 * Valid call sequences for squeezing are:
 * 1.
 * - shake(128,256)_squeezeblocks (0 to n times)
 * - shake(128,256)_squeeze (0 to n times)
 *
 * The incremental sequence shall be finalized with
 * - shake(128,256)_wipe (once)
 */

/**
 * SHAKE128 state initialization
*/
void shake128_init(Keccak_state* state);

/**
 * SHAKE128 incremental absorbing with bytewise inputs
*/
void shake128_absorb(Keccak_state* state, const uint8_t* in, size_t inlen);

/**
 * SHAKE128 finalization after incremental absorbing
*/
void shake128_finalize(Keccak_state* state);

/**
 * SHAKE128 non-incremental absorbing with bytewise input
*/
void shake128_absorb_once(Keccak_state* state, const uint8_t* in, size_t inlen);

/**
 * SHAKE128 incremental squeeze bytewise
*/
void shake128_squeeze(uint8_t* out, size_t outlen, Keccak_state* state);

/**
 * SHAKE128 incremental squeezeing blocks bytewise
*/
void shake128_squeezeblocks(uint8_t* out, size_t nblocks, Keccak_state* state);

/**
 * SHAKE128 final state wipe
*/
void shake128_wipe(Keccak_state* state);

/**
 * SHAKE256 state initialization
*/
void shake256_init(Keccak_state* state);

/**
 * SHAKE256 incremental absorbing with bytewise inputs
*/
void shake256_absorb(Keccak_state* state, const uint8_t* in, size_t inlen);

/**
 * SHAKE256 finalization after incremental absorbing
*/
void shake256_finalize(Keccak_state* state);

/**
 * SHAKE256 incremental squeeze bytewise
*/
void shake256_squeeze(uint8_t* out, size_t outlen, Keccak_state* state);

/**
 * SHAKE256 incremental squeezeing blocks bytewise
*/
void shake256_squeezeblocks(uint8_t* out, size_t nblocks, Keccak_state* state);

/**
 * SHAKE256 final state wipe
*/
void shake256_wipe(Keccak_state* state);


#endif
