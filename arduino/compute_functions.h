/*

Copyright 2024 Ivan Gubochkin (igubochkin@gmail.com)

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this
   list of conditions and the following disclaimer in the documentation and/or other
   materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be
   used to endorse or promote products derived from this software without specific
   prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef COMPUTE_FUNCTIONS_H
#define COMPUTE_FUNCTIONS_H

#include <math.h>

static inline float approximate_sqrt(float x) {
  union {
    float f;
    uint32_t d;
  } floatNumber;

  floatNumber.f = x;

  // adjust bias
  floatNumber.d += static_cast<uint32_t>(127) << 23;
  // approximation of square root
  floatNumber.d >>= 1;

  return floatNumber.f;
}

static inline float approximate_isqrt(float x) {
  union {
    float f;
    uint32_t d;
  } floatNumber;

  floatNumber.f = x;
  floatNumber.d = 0x5F375A86 - (floatNumber.d >> 1);

  return floatNumber.f;
}

static inline float approximate_div256(float x) {
  union {
    float f;
    uint32_t d;
    struct {
      uint16_t i;
      uint16_t j;
    } n;
  } floatNumber;

  const uint16_t exponent256 = uint16_t(1024); // ((135 << 23) - (127 << 23)) >> 16
  const uint16_t mask = uint16_t(0x7F80);

  floatNumber.f = x;
  if ((floatNumber.n.j & mask) <= exponent256) {
    floatNumber.d = 0;
  } else {
    floatNumber.n.j = floatNumber.n.j - exponent256;
  }

  return floatNumber.f;
}

static inline float approximate_div256_nozerocheck(float x) {
  union {
    float f;
    uint32_t d;
  } floatNumber;

  const uint32_t exponent256 = uint32_t(67108864); // (135 << 23) - (127 << 23)

  floatNumber.f = x;
  floatNumber.d = floatNumber.d - exponent256;

  return floatNumber.f;
}

static inline float approximate_inverse_number(float x) {
  union {
    float f;
    uint32_t d;
  } floatNumber;

  // re-interpret as a 32 bit integer
  floatNumber.f = x;
  // adjust exponent
  floatNumber.d = uint32_t(0x7F000000) - floatNumber.d;

  return floatNumber.f;
}

static inline float approximate_exp(float x) {
  // Code of this function is based on this article
  // https://nic.schraudolph.org/pubs/Schraudolph99.pdf.
  // Normalized constant values was recomputed for
  // float32 type.
  union {
    float f;
    struct {
      int16_t i;
      int16_t j;
    } n;
  } p;

  // We ensure that float32 number has 23 bit for mantissa and
  // 8 bit for exponent. According FloatParts structure we should
  // set to zero first 16 bits of the number and then we get
  // constant for further computations as 2^(23 - 16) or 2^7=128

  // Value of A constant is 2^7/ln(2)
  const float EXP_A = static_cast<float>(128 / M_LN2);
  // Value of B constant is. Here we have exponent shift as 127 according
  // IEEE-754 standard and then we get B=127*2^7
  const int16_t EXP_B = 16256;

  // Value of B constant according (A.9) formula from the article:
  // C = 2^7 * ln(3/(8 * ln(2)) + 1/2)/ln(2)
  const int16_t EXP_C = 7;

  p.n.i = 0;
  p.n.j = static_cast<int16_t>(EXP_A * x) + (EXP_B - EXP_C);

  return p.f;
}

static inline float reference_sqrt(float x) {
  return sqrtf(x);
}

static inline float reference_isqrt(float x) {
  return 1.0f / sqrtf(x);
}

static inline float reference_div256(float x) {
  return x / 256.0f;
}

static inline float reference_inverse_number(float x) {
  return 1.0f / x;
}

static inline float reference_exp(float x) {
  return expf(x);
}

#endif /* COMPUTE_FUNCTIONS_H */
