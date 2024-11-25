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

template <typename FLOAT_TYPE, typename size_t REPEAT_COUNT>
    requires std::same_as<FLOAT_TYPE, float>
class InvertNumberAVX : public AVXAcceleratedTest<FLOAT_TYPE, InvertNumberAVX<FLOAT_TYPE, REPEAT_COUNT>, REPEAT_COUNT> {
public:
    static inline __m256 approximate_value(__m256 x) {
        __m256i const_int = _mm256_set1_epi32(0x7EEEEEEE);
        __m256i res = _mm256_sub_epi32(const_int, _mm256_castps_si256(x));

        return _mm256_castsi256_ps(res);
    }

    static inline __m256 reference_value(__m256 x) {
        __m256 const_1 = _mm256_set1_ps(1.0f);
        __m256 res = _mm256_div_ps(const_1, x);

        return res;
    }

    std::string getFunctionName() override {
        return "Number inversion (AVX)";
    }

    bool enableNegatives() override {
        return true;
    }
};
