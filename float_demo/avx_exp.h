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

#pragma once

#include <emmintrin.h>
#include "avx_test_base.h"

template <typename FLOAT_TYPE>
    requires std::same_as<FLOAT_TYPE, float>
class ExponentAVX : public AVXAcceleratedTest<FLOAT_TYPE, ExponentAVX<FLOAT_TYPE>> {
public:
    static inline __m256 approximate_value(__m256 x) {
        __m256 EXP_A = _mm256_set1_ps(static_cast<FLOAT_TYPE>(128 / M_LN2));
        __m256i EXP_BC = _mm256_set1_epi32((16256 - 7) << 16);
        __m256 mulProduct = _mm256_mul_ps(x, EXP_A);
        __m256i opInt = _mm256_cvtps_epi32(mulProduct);
        __m256i op2 = _mm256_slli_epi32(opInt, 16);
        __m256i res = _mm256_adds_epi16(op2, EXP_BC);

        // Value of A constant is 2^7/ln(2)
        //constexpr FLOAT_TYPE EXP_A = static_cast<FLOAT_TYPE>(128 / M_LN2);
        // Value of B constant is. Here we have exponent shift as 127 according
        // IEEE-754 standard and then we get B=127*2^7
        //constexpr int16_t EXP_B = 16256;

        // Value of B constant according (A.9) formula from the article:
        // C = 2^7 * ln(3/(8 * ln(2)) + 1/2)/ln(2)
        //constexpr int16_t EXP_C = 7;

        //p.n.i = 0;
        //p.n.j = static_cast<int16_t>(EXP_A * x) + (EXP_B - EXP_C);

        return _mm256_castsi256_ps(res);
    }

    static inline __m256 reference_value(__m256 x) {
        __m256 res = _mm256_exp_ps(x);
        return res;
    }

    std::string getFunctionName() override {
        return "Exponent (AVX)";
    }

    bool enableNegatives() override {
        return true;
    }
};
