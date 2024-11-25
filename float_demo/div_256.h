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

template <typename FLOAT_TYPE, typename size_t REPEAT_COUNT>
    requires std::same_as<FLOAT_TYPE, float>
class DivideTo256 : public TestBasic<FLOAT_TYPE, REPEAT_COUNT> {
public:
    using TestBasicType = typename TestBasic<FLOAT_TYPE, REPEAT_COUNT>;

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

    TestBasic<FLOAT_TYPE, REPEAT_COUNT>::DelayInfo compute(const std::vector<FLOAT_TYPE>& in, std::vector<FLOAT_TYPE>& targets, std::vector<FLOAT_TYPE>& actuals) override {
        targets.resize(in.size());
        actuals.resize(in.size());
        std::vector<double> targetDelays(REPEAT_COUNT);

        for (size_t r = 0; r < REPEAT_COUNT; r++) {
            auto start = std::chrono::system_clock::now();
            for (size_t i = 0; i < in.size(); i++) {
                targets[i] = reference_value(in[i]);
            }

            auto stop = std::chrono::system_clock::now();
            auto targetDelay = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            targetDelays[r] = static_cast<double>(targetDelay.count());
        }

        std::vector<double> actualDelays(REPEAT_COUNT);

        for (size_t r = 0; r < REPEAT_COUNT; r++) {
            auto start = std::chrono::system_clock::now();
            for (size_t i = 0; i < in.size(); i++) {
                actuals[i] = approximate_value(in[i]);
            } 
            auto stop = std::chrono::system_clock::now(); 
            auto actualDelay = std::chrono::duration_cast<std::chrono::microseconds>(stop - start); 
            actualDelays[r] = static_cast<double>(actualDelay.count());
        }

        typename TestBasic<FLOAT_TYPE, REPEAT_COUNT>::DelayInfo delayInfo;
        delayInfo.referenceDuration = TestBasic<FLOAT_TYPE, REPEAT_COUNT>::DurationType(TestBasic<FLOAT_TYPE, REPEAT_COUNT>::getDelay(targetDelays));
        delayInfo.approximateDuration = TestBasic<FLOAT_TYPE, REPEAT_COUNT>::DurationType(TestBasic<FLOAT_TYPE, REPEAT_COUNT>::getDelay(actualDelays));
        
        return delayInfo;
    }

    bool enableNegatives() override {
        return true;
    }
};
