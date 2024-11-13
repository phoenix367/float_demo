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

#include <cmath>
#include <cinttypes>

#include "test_basic.h"

template <typename FLOAT_TYPE>
    requires std::same_as<FLOAT_TYPE, float>
class Exponent : public TestBasic<FLOAT_TYPE> {
private:
#pragma pack(push)
#pragma pack(1)
    union FloatParts {
        float f;
        struct {
            int16_t i;
            int16_t j;
        } n;
    };
#pragma pack(pop)

public:
    inline FLOAT_TYPE approximate_value(FLOAT_TYPE x) {
        // Code of this function is based on this article
        // https://nic.schraudolph.org/pubs/Schraudolph99.pdf.
        // Normalized constant values was recomputed for
        // float32 type.
        FloatParts p;

        // We ensure that float32 number has 23 bit for mantissa and
        // 8 bit for exponent. According FloatParts structure we should
        // set to zero first 16 bits of the number and then we get
        // constant for further computations as 2^(23 - 16) or 2^7=128

        // Value of A constant is 2^7/ln(2)
        constexpr FLOAT_TYPE EXP_A = static_cast<FLOAT_TYPE>(128 / M_LN2);
        // Value of B constant is. Here we have exponent shift as 127 according
        // IEEE-754 standard and then we get B=127*2^7
        constexpr int16_t EXP_B = 16256;

        // Value of B constant according (A.9) formula from the article:
        // C = 2^7 * ln(3/(8 * ln(2)) + 1/2)/ln(2)
        constexpr int16_t EXP_C = 7;

        p.n.i = 0;
        p.n.j = static_cast<int16_t>(EXP_A * x) + (EXP_B - EXP_C);

        return p.f;
    }

    inline FLOAT_TYPE reference_value(FLOAT_TYPE x) {
        return std::exp(x);
    }

    std::string getFunctionName() override {
        return "Exponent";
    }

    DEFINE_DEFAULT_COMPUTE_FUNCTION

    bool enableNegatives() override {
        return true;
    }
};
