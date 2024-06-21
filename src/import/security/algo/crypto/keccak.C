/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/security/algo/crypto/keccak.C $                    */
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
#include <stdint.h>
#include <keccak.H>

#ifdef __PPE__
    #include <ppe42_string.h>
#else
    #include <string.h>
#endif

static const uint64_t keccak_rcs[KECCAK_ROUNDS] =
{
    0x0000000000000001ULL,
    0x0000000000008082ULL,
    0x800000000000808aULL,
    0x8000000080008000ULL,
    0x000000000000808bULL,
    0x0000000080000001ULL,
    0x8000000080008081ULL,
    0x8000000000008009ULL,
    0x000000000000008aULL,
    0x0000000000000088ULL,
    0x0000000080008009ULL,
    0x000000008000000aULL,
    0x000000008000808bULL,
    0x800000000000008bULL,
    0x8000000000008089ULL,
    0x8000000000008003ULL,
    0x8000000000008002ULL,
    0x8000000000000080ULL,
    0x000000000000800aULL,
    0x800000008000000aULL,
    0x8000000080008081ULL,
    0x8000000000008080ULL,
    0x0000000080000001ULL,
    0x8000000080008008ULL
};

#define ROTL64(i, n) ((i << n) ^ (i >> (64-n)))

static void keccak_permute(Keccak_state* state)
{
    uint64_t* A = state->A;
    int i, j;
    uint64_t CZ[5], DZ[5], tmp[2];

    static const uint8_t m5t[25] =
    {
        0, 1, 2, 3, 4, 0, 1, 2, 3, 4, 0, 1, 2, 3, 4, 0, 1, 2, 3, 4, 0, 1, 2, 3, 4
    };
    static const uint8_t Aseq[] =
    {
        10, 7, 11, 17, 18, 3, 5, 16, 8, 21, 24, 4, 15, 23, 19, 13, 12, 2, 20, 14, 22, 9, 6
    };
    static const uint8_t ROTseq[] =
    {
        3, 6, 10, 15, 21, 28, 36, 45, 55, 2, 14, 27, 41, 56, 8, 25, 43, 62, 18, 39, 61, 20, 44
    };

    /**
     *  For i_r from 12+2l-n_r to 12+2l-1, A=Rnd(A,i_r)
     *  l=6, n_r=24 => 0 to 23 => 24 rounds
     *  Rnd(A, i_r) = ι(χ(π(ρ(θ(A)))), i_r)
     */
    for (i = 0; i < KECCAK_ROUNDS; ++i)
    {
        /* θ(A) */
        for (j = 0; j < 5; ++j)
        {
            CZ[j] = A[j] ^ A[j + 5] ^ A[j + 10] ^ A[j + 15] ^ A[j + 20];
        }

        for (j = 0; j < 5; ++j)
        {
            DZ[j] = CZ[m5t[j + 4]] ^ ROTL64(CZ[m5t[j + 1]], 1);
        }

        /**
         * for (int j = 0; j < KECCAK_P_64L; ++j)
         *  A[j] ^= DZ[j % 5];
        */
        for (j = 0; j < KECCAK_P_64L; ++j)
        {
            A[j] ^= DZ[m5t[j]];
        }


        /* pi(ρ(θ(A))) */
        /** A[0] = A[0] */
        /**
         * int x = 1, y = 0;
         * for (int t = 0; t <= 23; ++t) {
         *   A'[y*5+x] = ROTL64(A[y*5+x], ((t+1)*(t+2)/2) % 64);
         *   (x, y)= (y, (2x+3y) mod 5)
         * }
         *
         * for (int t = 0; t < KECCAK_P_64L; ++t) {
         *   A[t] = A['idx'((x + 3y) mod 5', x]
         *
        */
        tmp[0] = ROTL64(A[1], 1);

        for (j = 0; j < 23; ++j)
        {
            tmp[(j + 1) % 2] = ROTL64(A[Aseq[j]], ROTseq[j]);
            A[Aseq[j]] = tmp[j % 2];
        }

        A[1] = tmp[1];

        /* χ(π(ρ(θ(A))))
         * for (int t = 0; t < KECCAK_P_64L; ++t) {
         *   A[t] = A[t] ^ ~A['idx'((x + 1) mod 5', x] & A['idx'((x + 2) mod 5', x]
         */
        for (j = 0; j < 5; ++j)
        {
            for (int z = 0; z < 5; ++z)
            {
                CZ[z] = A[m5t[z + 4] + j * 5] ^ (~A[z + j * 5] & A[m5t[z + 1] + j * 5]);
            }

            for (int z = 0; z < 5; ++z)
            {
                A[m5t[z + 4] + j * 5] = CZ[z];
            }
        }

        /* ι(χ(π(ρ(θ(A)))): Add round constant */
        A[0] ^= keccak_rcs[i];
    }

}

#define KECCAK_MIN(a, b) (a < b ? (a) : (b))

static void keccak_sponge_init(Keccak_state* state)
{
    memset(state->A, 0, KECCAK_P_64L * 8);
    state->idx = 0;
}

static void keccak_sponge_absorb_update(Keccak_state* state, const uint8_t* N, size_t Nlen, size_t rBytes)
{
    uint64_t* A = state->A;
    size_t i = state->idx;

    /** Number of bytes in the first block */
    size_t fb_bytes = (i > 0 ? KECCAK_MIN(Nlen, rBytes - i) : 0);
    /** Number of full blocks */
    size_t fl_blocks = (Nlen - fb_bytes) / rBytes;
    /** Number of bytes in the last block */
    size_t lb_bytes = (Nlen - fb_bytes - fl_blocks * rBytes);

    /** First partial block */
    for (; i < state->idx + fb_bytes; ++i)
    {
        A[i / 8] ^= ((uint64_t) * N++) << 8 * (i % 8);
    }

    if (i == rBytes)
    {
        keccak_permute(state);
        i = 0;
    }

    /** Full blocks */
    for (size_t j = 0; j < fl_blocks; ++j)
    {
        for (; i < rBytes / 8; ++i, N += 8)
        {
            A[i] ^=
                ((uint64_t) N[0]) ^
                (((uint64_t) N[1]) << 8) ^
                (((uint64_t) N[2]) << 16) ^
                (((uint64_t) N[3]) << 24) ^
                (((uint64_t) N[4]) << 32) ^
                (((uint64_t) N[5]) << 40) ^
                (((uint64_t) N[6]) << 48) ^
                (((uint64_t) N[7]) << 56);
        }

        keccak_permute(state);
        i = 0;
    }

    /** Last partial block */
    for (; i < lb_bytes; ++i)
    {
        A[i / 8] ^= ((uint64_t) * N++) << 8 * (i % 8);
    }

    state->idx = i;
}

/**
 * Final absorb part of Keccak Sponge, includes padding.
 * Does the first permute for squeezing
*/
static void keccak_sponge_absorb_final(Keccak_state* state, size_t rBytes, int domsep)
{
    uint64_t* A = state->A;
    size_t i = state->idx;

    A[i / 8] ^= ((uint64_t) domsep) << (8 * (i % 8));
    A[rBytes / 8 - 1] ^= UINT64_C(0x8000000000000000);

    state->idx = 0;
}

static void keccak_sponge_squeeze_blocks_update(Keccak_state* state, uint8_t* out, size_t blocks, size_t rBytes)
{
    uint64_t* A = state->A;

    for (size_t j = 0; j < blocks; ++j)
    {
        keccak_permute(state);

        for (size_t i = 0; i < rBytes; ++i)
        {
            *out++ = A[i / 8] >> 8 * (i % 8);
        }
    }
}

static void keccak_sponge_squeeze_update(Keccak_state* state, size_t Ni, uint8_t* out, size_t rBytes)
{
    uint64_t* A = state->A;
    size_t i = state->idx;

    /** Number of bytes in the first block */
    size_t fb_bytes = (i > 0 ? KECCAK_MIN(Ni, rBytes - i) : 0);
    /** Number of full blocks */
    size_t fl_blocks = (Ni - fb_bytes) / rBytes;
    /** Number of bytes in the last block */
    size_t lb_bytes = (Ni - fb_bytes - fl_blocks * rBytes);

    /** First partial block */
    for (; i < state->idx + fb_bytes; ++i)
    {
        *out++ = A[i / 8] >> 8 * (i % 8);
    }

    /** Full blocks */
    for (size_t j = 0; j < fl_blocks; ++j)
    {
        keccak_permute(state);

        for (i = 0; i < rBytes / 8; ++i, out += 8)
        {
            out[0] = A[i];
            out[1] = A[i] >> 8;
            out[2] = A[i] >> 16;
            out[3] = A[i] >> 24;
            out[4] = A[i] >> 32;
            out[5] = A[i] >> 40;
            out[6] = A[i] >> 48;
            out[7] = A[i] >> 56;
        }

        i = 0;
    }

    if (i == 0 && lb_bytes)
    {
        keccak_permute(state);
    }

    /** Last partial block */
    for (; i < lb_bytes; ++i)
    {
        *out++ = A[i / 8] >> 8 * (i % 8);
    }

    state->idx = i;
}

static void keccak_sponge_wipe(Keccak_state* state)
{
    volatile uint64_t* tmp = (volatile uint64_t*)state->A;

    for (int i = 0; i < KECCAK_P_64L; ++i)
    {
        tmp[i] = 0;
    }
}

/** Incremental API for SHAKE128 / SHAKE256 */

void shake128_absorb(Keccak_state* state, const uint8_t* in, size_t inlen)
{
    keccak_sponge_absorb_update(state, in, inlen, SHAKE128_RATE);
}

void shake256_absorb(Keccak_state* state, const uint8_t* in, size_t inlen)
{
    keccak_sponge_absorb_update(state, in, inlen, SHAKE256_RATE);
}

void shake128_init(Keccak_state* state)
{
    keccak_sponge_init(state);
}

void shake128_finalize(Keccak_state* state)
{
    keccak_sponge_absorb_final(state, SHAKE128_RATE, SHAKE_DOMAIN_SEP);
}

void shake128_squeeze(uint8_t* out, size_t outlen, Keccak_state* state)
{
    keccak_sponge_squeeze_update(state, outlen, out, SHAKE256_RATE);
}

void shake128_absorb_once(Keccak_state* state, const uint8_t* in, size_t inlen)
{
    keccak_sponge_init(state);
    keccak_sponge_absorb_update(state, in, inlen, SHAKE128_RATE);
    keccak_sponge_absorb_final(state, SHAKE128_RATE, SHAKE_DOMAIN_SEP);
}

void shake128_squeezeblocks(uint8_t* out, size_t nblocks, Keccak_state* state)
{
    keccak_sponge_squeeze_blocks_update(state, out, nblocks, SHAKE128_RATE);
}

void shake128_wipe(Keccak_state* state)
{
    keccak_sponge_wipe(state);
}

void shake256_init(Keccak_state* state)
{
    keccak_sponge_init(state);
}

void shake256_finalize(Keccak_state* state)
{
    keccak_sponge_absorb_final(state, SHAKE256_RATE, SHAKE_DOMAIN_SEP);
}

void shake256_squeeze(uint8_t* out, size_t outlen, Keccak_state* state)
{
    keccak_sponge_squeeze_update(state, outlen, out, SHAKE256_RATE);
}

void shake256_absorb_once(Keccak_state* state, const uint8_t* in, size_t inlen)
{
    keccak_sponge_init(state);
    keccak_sponge_absorb_update(state, in, inlen, SHAKE256_RATE);
    keccak_sponge_absorb_final(state, SHAKE256_RATE, SHAKE_DOMAIN_SEP);
}

void shake256_squeezeblocks(uint8_t* out, size_t nblocks, Keccak_state* state)
{
    keccak_sponge_squeeze_blocks_update(state, out, nblocks, SHAKE256_RATE);
}

void shake256_wipe(Keccak_state* state)
{
    keccak_sponge_wipe(state);
}

/** Non-incremental API for SHAKE128 / SHAKE256 */

void shake128(uint8_t* out, size_t outlen, const uint8_t* in, size_t inlen)
{

    Keccak_state state;

    size_t blocks = outlen / SHAKE128_RATE;
    size_t rem = blocks * SHAKE128_RATE;

    shake128_absorb_once(&state, in, inlen);
    shake128_squeezeblocks(out, blocks, &state);
    shake128_squeeze(out + rem, outlen - rem, &state);
    shake128_wipe(&state);
}

void shake256(uint8_t* out, size_t outlen, const uint8_t* in, size_t inlen)
{

    Keccak_state state;

    size_t blocks = outlen / SHAKE256_RATE;
    size_t rem = blocks * SHAKE256_RATE;

    shake256_absorb_once(&state, in, inlen);
    shake256_squeezeblocks(out, blocks, &state);
    shake256_squeeze(out + rem, outlen - rem, &state);
    shake256_wipe(&state);
}

/** Non-incremental API for SHA3-256 / SHA3-512 */
void sha3_256(uint8_t h[32], const uint8_t* in, size_t inlen)
{
    Keccak_state state;
    keccak_sponge_init(&state);
    keccak_sponge_absorb_update(&state, in, inlen, SHA3_256_RATE);
    keccak_sponge_absorb_final(&state, SHA3_256_RATE, SHA3_DOMAIN_SEP);
    keccak_sponge_squeeze_update(&state, 32, h, SHA3_256_RATE);
    keccak_sponge_wipe(&state);
}

void sha3_512(uint8_t h[64], const uint8_t* in, size_t inlen)
{
    Keccak_state state;
    keccak_sponge_init(&state);
    keccak_sponge_absorb_update(&state, in, inlen, SHA3_512_RATE);
    keccak_sponge_absorb_final(&state, SHA3_512_RATE, SHA3_DOMAIN_SEP);
    keccak_sponge_squeeze_update(&state, 64, h, SHA3_512_RATE);
    keccak_sponge_wipe(&state);
}

/** Incremental API for SHA3-512 */

void sha3_512_init(Keccak_state* state)
{
    keccak_sponge_init(state);
}

void sha3_512_update(Keccak_state* state, const void* data, size_t len)
{
    keccak_sponge_absorb_update(state, (const uint8_t*)data, len, SHA3_512_RATE);
}

void sha3_512_final(uint8_t md[64], Keccak_state* state)
{
    keccak_sponge_absorb_final(state, SHA3_512_RATE, SHA3_DOMAIN_SEP);
    keccak_sponge_squeeze_update(state, 64, md, SHA3_512_RATE);
    keccak_sponge_wipe(state);
}
