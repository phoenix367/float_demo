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

#include <concepts>
#include <cmath>
#include <cinttypes>

#include "test_basic.h"

template <typename FLOAT_TYPE>
    requires std::same_as<FLOAT_TYPE, float>
class DivideTo256 : public TestBasic<FLOAT_TYPE> {
public:
    inline FLOAT_TYPE approximate_value(FLOAT_TYPE x) {
        constexpr uint32_t exponent256 = (135 << 23) - (127 << 23);
        uint32_t* i = (uint32_t*)&x;

        *i = (*i - exponent256) * ((*i & 0x7F800000) > exponent256);

        return x;
    }

    inline FLOAT_TYPE reference_value(FLOAT_TYPE x) {
        return x / static_cast<FLOAT_TYPE>(256);
    }

    std::string getFunctionName() override {
        return "Divide to 256";
    }

    DEFINE_DEFAULT_COMPUTE_FUNCTION

    bool enableNegatives() override {
        return true;
    }
};
