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

#include "avx_test_base.h"

template <typename FLOAT_TYPE>
    requires std::same_as<FLOAT_TYPE, float>
class DivideTo256AVX : public AVXAcceleratedTest<FLOAT_TYPE, DivideTo256AVX<FLOAT_TYPE>> {
public:
    static inline __m256 approximate_value(__m256 x) {
        __m256i exponent256 = _mm256_set1_epi32((135 << 23) - (127 << 23));
        __m256i exponent_mask = _mm256_set1_epi32(0x7F800000);
        __m256i op1 = _mm256_sub_epi32(_mm256_castps_si256(x), exponent256);
        __m256i op2 = _mm256_and_epi32(_mm256_castps_si256(x), exponent_mask);
        __m256i mask = _mm256_cmpgt_epi32(op2, exponent256);
        __m256i res = _mm256_and_epi32(op1, mask);

        //constexpr uint32_t exponent256 = (135 << 23) - (127 << 23);
        //uint32_t* i = (uint32_t*)&x;

        //*i = (*i - exponent256) * ((*i & 0x7F800000) > exponent256);

        return _mm256_castsi256_ps(res);
    }

    static inline __m256 reference_value(__m256 x) {
        __m256 const_1 = _mm256_set1_ps(256.0f);
        __m256 res = _mm256_div_ps(x, const_1);

        return res;
    }

    std::string getFunctionName() override {
        return "Divide to 256 (AVX)";
    }

    bool enableNegatives() override {
        return true;
    }
};
