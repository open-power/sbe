/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/sbefw/verification/ecverify.H $                           */
/*                                                                        */
/* OpenPOWER sbe Project                                                  */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2020,2021                        */
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
#ifndef __EC_VERIFY_CORE_
#define __EC_VERIFY_CORE_

#define  EC_PRIMEBITS  521    /* P521 */

#define  EC_STACKTRACE  1       /* debug only; currently, glibc      */
#define  NO_EC_DOUBLE_XY 1      /* do not implement ec_double_xy */

#define EC_HASHBYTES   64     /* SHA-256 */
#define EC_COORDBYTES  66     /* P-521   */

#define  BN_PRIME_MSW        0x1ff
#define  BN_PRIME_MSW_MASK   0x1ff    /* equal, as coincidence, for P521 */
#define  BN_PRIME_MSW_BITS   (EC_PRIMEBITS % BN_BITS)



typedef uint8_t ecc_key_t[2*EC_COORDBYTES];
typedef uint8_t ecc_signature_t[2*EC_COORDBYTES];

typedef uint32_t     bn_t;
typedef uint16_t     hbn_t;         /* half-bignumber */
typedef uint_fast8_t bnindex_t;


#define  BN_BITS   (8*sizeof(bn_t))
#define  HBN_BITS  (8*sizeof(hbn_t))

#define  EC_PRIMEBYTES  ((EC_PRIMEBITS +7) /8)

#define  BN_MAXBIT  (((bn_t) 1) << (BN_BITS -1))

#define  BITS2BN(bits) (((bits) +BN_BITS -1) / BN_BITS)

// we only deal with big numbers of fixed size
#define  NWORDS   BITS2BN( EC_PRIMEBITS )
#define  BNBYTES  (NWORDS*sizeof(bn_t))

#define  BN_MSW(p)     ((p)[0])
#define  BN_LSW(p)     ((p)[ NWORDS-1 ])
#define  bn_is_odd(p)  (1 & BN_LSW(p))

typedef struct {
    bn_t ec_prime[ NWORDS ];
    bn_t ec_order[ NWORDS ];
    bn_t prime_px[ NWORDS ];
    bn_t prime_py[ NWORDS ];
    bn_t ec_order_qn[ NWORDS ];
    bn_t coeff_a[ NWORDS ];
    bn_t coeff_b[ NWORDS ];
} consts_t;


const consts_t consts = {
//const bn_t ec_prime[ NWORDS ] =
    {
        BN_PRIME_MSW,
        0xffffffff,0xffffffff,
        0xffffffff,0xffffffff,
        0xffffffff,0xffffffff,
        0xffffffff,0xffffffff,
        0xffffffff,0xffffffff,
        0xffffffff,0xffffffff,
        0xffffffff,0xffffffff,
        0xffffffff,0xffffffff,
    },

//const bn_t ec_order[ NWORDS ] =
    {
        0x000001ff,
        0xffffffff,0xffffffff,
        0xffffffff,0xffffffff,
        0xffffffff,0xffffffff,
        0xffffffff,0xfffffffa,
        0x51868783,0xbf2f966b,
        0x7fcc0148,0xf709a5d0,
        0x3bb5c9b8,0x899c47ae,
        0xbb6fb71e,0x91386409,
    },

//const bn_t prime_px[ NWORDS ] = {
    {
        0x000000c6,
        0x858e06b7,0x0404e9cd,
        0x9e3ecb66,0x2395b442,
        0x9c648139,0x053fb521,
        0xf828af60,0x6b4d3dba,
        0xa14b5e77,0xefe75928,
        0xfe1dc127,0xa2ffa8de,
        0x3348b3c1,0x856a429b,
        0xf97e7e31,0xc2e5bd66,
    },

//const bn_t prime_py[ NWORDS ] = {
    {
        0x00000118,
        0x39296a78,0x9a3bc004,
        0x5c8a5fb4,0x2c7d1bd9,
        0x98f54449,0x579b4468,
        0x17afbd17,0x273e662c,
        0x97ee7299,0x5ef42640,
        0xc550b901,0x3fad0761,
        0x353c7086,0xa272c240,
        0x88be9476,0x9fd16650,
    },

//--------------------------  mod mul by order (n)  -------
// MS 521 bits of Q/N, fractional part
//
// static const bn_t ec_order_qn[ NWORDS ] =
    {
                        0,0,
                        0,0,
                        0,0,
                        0x0,
        0x00000000,0x00000005,
        0xae79787c,0x40d06994,
        0x8033feb7,0x08f65a2f,
        0xc44a3647,0x7663b851,
        0x449048e1,0x6ec79bf6,
    },
    {
        0x00000000,
    0x00000000,0x00000000,
    0x00000000,0x00000000,
    0x00000000,0x00000000,
    0x00000000,0x00000000,
    0x00000000,0x00000000,
    0x00000000,0x00000000,
    0x00000000,0x00000000,
    0x00000000,0x00000003,
    },
    {
        0x00000051,
    0x953eb961,0x8e1c9a1f,
    0x929a21a0,0xb68540ee,
    0xa2da725b,0x99b315f3,
    0xb8b48991,0x8ef109e1,
    0x56193951,0xec7e937b,
    0x1652c0bd,0x3bb1bf07,
    0x3573df88,0x3d2c34f1,
    0xef451fd4,0x6b503f00,
    },
} ;



int ec_verify (const unsigned char *publicpt,    /* 2*EC_COORDBYTES */
               const unsigned char *hash,        /*   EC_HASHBYTES  */
               const unsigned char *signature);   /* 2*EC_COORDBYTES */

void bn_read_pt(bn_t *r, const unsigned char *data);
void __attribute__((noinline)) BN_COPY (bn_t *dst, const bn_t *src);
bn_t bn_sub    (bn_t *a, const bn_t *b) ;
void bn_add    (bn_t *a, const bn_t *b) ;
void bn_mul    (bn_t *r, const bn_t *a, const bn_t *b) ;
void bn_modadd (bn_t *a, const bn_t *b) ;
void bn_modsub (bn_t *a, const bn_t *b) ;

int bn_cmp (const bn_t a[NWORDS], const bn_t b[NWORDS]) ;
void bn_modmul_prime (bn_t *a, const bn_t *b);
int ec_multiply (bn_t *x, bn_t *y, bn_t *z, const bn_t *k);
void ec_projective2affine (bn_t *x, const bn_t *z);

inline const consts_t* __attribute__((pure)) consts_p()
{
    return &consts;
};




#endif
