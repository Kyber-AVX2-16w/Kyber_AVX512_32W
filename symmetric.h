#ifndef SYMMETRIC_H
#define SYMMETRIC_H

#include <stddef.h>
#include <stdint.h>
#include "params.h"

#ifdef KYBER_90S

#include "sha2.h"
#include "aes256ctr.h"

#if (KYBER_SSBYTES != 32)
#error "90s variant of Kyber can only generate keys of length 256 bits"
#endif

typedef aes256ctr_ctx xof_state;

#define XOF_BLOCKBYTES AES256CTR_BLOCKBYTES

#define hash_h(OUT, IN, INBYTES) sha256(OUT, IN, INBYTES)
#define hash_g(OUT, IN, INBYTES) sha512(OUT, IN, INBYTES)
#define xof_absorb(STATE, SEED, X, Y) \
        aes256ctr_init(STATE, SEED, (X) | ((uint16_t)(Y) << 8))
#define xof_squeezeblocks(OUT, OUTBLOCKS, STATE) \
        aes256ctr_squeezeblocks(OUT, OUTBLOCKS, STATE)
#define prf(OUT, OUTBYTES, KEY, NONCE) \
        aes256ctr_prf(OUT, OUTBYTES, KEY, NONCE)
#define kdf(OUT, IN, INBYTES) sha256(OUT, IN, INBYTES)

#else

#include "fips202.h"
#include "fips202x8.h"

typedef keccak_state xof_state;

#define kyber_shake128_absorb KYBER_NAMESPACE(kyber_shake128_absorb)
void kyber_shake128_absorb(keccak_state *s,
                           const uint8_t seed[KYBER_SYMBYTES],
                           uint8_t x,
                           uint8_t y);

#define kyber_shake256_prf KYBER_NAMESPACE(kyber_shake256_prf)
void kyber_shake256_prf(uint8_t *out,
                        size_t outlen,
                        const uint8_t key[KYBER_SYMBYTES],
                        uint8_t nonce);

#define XOF_BLOCKBYTES SHAKE128_RATE

#define hash_h(OUT, IN, INBYTES) sha3_256(OUT, IN, INBYTES)
#define hash_hx8(OUT0, OUT1, OUT2, OUT3, OUT4, OUT5, OUT6, OUT7, IN0, IN1, IN2, IN3, IN4, IN5, IN6, IN7, INBYTES) sha3x8_256(OUT0, OUT1, OUT2, OUT3, OUT4, OUT5, OUT6, OUT7, IN0, IN1, IN2, IN3, IN4, IN5, IN6, IN7, INBYTES)
#define hash_g(OUT, IN, INBYTES) sha3_512(OUT, IN, INBYTES)
#define hash_gx8(OUT0, OUT1, OUT2, OUT3, OUT4, OUT5, OUT6, OUT7, IN0, IN1, IN2, IN3, IN4, IN5, IN6, IN7, INBYTES) sha3x8_512(OUT0, OUT1, OUT2, OUT3, OUT4, OUT5, OUT6, OUT7, IN0, IN1, IN2, IN3, IN4, IN5, IN6, IN7, INBYTES)
#define xof_absorb(STATE, SEED, X, Y) kyber_shake128_absorb(STATE, SEED, X, Y)
#define xof_squeezeblocks(OUT, OUTBLOCKS, STATE) \
        shake128_squeezeblocks(OUT, OUTBLOCKS, STATE)
#define prf(OUT, OUTBYTES, KEY, NONCE) \
        kyber_shake256_prf(OUT, OUTBYTES, KEY, NONCE)
#define kdf(OUT, IN, INBYTES) shake256(OUT, KYBER_SSBYTES, IN, INBYTES)
#define kdfx8(OUT0, OUT1, OUT2, OUT3, OUT4, OUT5, OUT6, OUT7, IN0, IN1, IN2, IN3, IN4, IN5, IN6, IN7, INBYTES) shake256x8(OUT0, OUT1, OUT2, OUT3, OUT4, OUT5, OUT6, OUT7, KYBER_SSBYTES, IN0, IN1, IN2, IN3, IN4, IN5, IN6, IN7, INBYTES)

#endif /* KYBER_90S */

#endif /* SYMMETRIC_H */
