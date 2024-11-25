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

#include <vector>
#include <string>
#include <chrono>
#include <utility>
#include <algorithm>
#include <numeric>

#define DEFINE_DEFAULT_COMPUTE_FUNCTION                                                 \
    TestBasicType::DelayInfo compute(                                           \
        const std::vector<FLOAT_TYPE>& in,                                              \
        std::vector<FLOAT_TYPE>& targets,                                               \
        std::vector<FLOAT_TYPE>& actuals                                                \
    ) override {                                                                        \
        targets.resize(in.size());                                                      \
        actuals.resize(in.size());                                                      \
                                                                                        \
        std::vector<double> targetDelays(REPEAT_COUNT);                                 \
        for (size_t r = 0; r < REPEAT_COUNT; r++) {                                     \
            auto start = std::chrono::system_clock::now();                              \
            for (size_t i = 0; i < in.size(); i++) {                                    \
                targets[i] = reference_value(in[i]);                                    \
            }                                                                           \
            auto stop = std::chrono::system_clock::now();                               \
            auto targetDelay = std::chrono::duration_cast<std::chrono::microseconds>(   \
                stop - start);                                                          \
            targetDelays[r] = static_cast<double>(targetDelay.count());                 \
        }                                                                               \
                                                                                        \
        std::vector<double> actualDelays(REPEAT_COUNT);                                 \
        for (size_t r = 0; r < REPEAT_COUNT; r++) {                                     \
            auto start = std::chrono::system_clock::now();                              \
            for (size_t i = 0; i < in.size(); i++) {                                    \
                actuals[i] = approximate_value(in[i]);                                  \
            }                                                                           \
            auto stop = std::chrono::system_clock::now();                               \
            auto actualDelay = std::chrono::duration_cast<std::chrono::microseconds>(   \
                stop - start);                                                          \
            actualDelays[r] = static_cast<double>(actualDelay.count());                 \
        }                                                                               \
                                                                                        \
        typename TestBasicType::DelayInfo delayInfo;                                    \
        delayInfo.referenceDuration = TestBasicType::DurationType(                      \
            TestBasicType::getDelay(targetDelays));                                     \
        delayInfo.approximateDuration = TestBasicType::DurationType(                    \
            TestBasicType::getDelay(actualDelays));                                     \
                                                                                        \
        return delayInfo;                                                               \
    }

template<typename FLOAT_TYPE, typename size_t REPEAT_COUNT>
class TestBasic {
public:
    using FloatType = FLOAT_TYPE;
    using DurationType = std::chrono::duration<double, std::micro>;
    
    struct DelayInfo {
        DurationType referenceDuration;
        DurationType approximateDuration;
    };

public:
    virtual ~TestBasic() = default;

    virtual DelayInfo compute(const std::vector<FLOAT_TYPE>& in,
        std::vector<FLOAT_TYPE>& targets, std::vector<FLOAT_TYPE>& actuals) = 0;
    virtual std::string getFunctionName() = 0;
    virtual bool enableNegatives() = 0;

protected:
    double getDelay(std::vector<double>& delays) {
        const size_t PERCENTILE_10 = static_cast<size_t>(0.1 * delays.size());
        const size_t PERCENTILE_90 = static_cast<size_t>(0.9 * delays.size());

        std::sort(delays.begin(), delays.end());

        std::vector<double> filteredDelays;
        std::copy(delays.begin() + PERCENTILE_10, delays.begin() + PERCENTILE_90,
            std::back_inserter(filteredDelays));

        auto sumDelay = std::accumulate(filteredDelays.begin(), filteredDelays.end(), 0.0);
        return sumDelay / filteredDelays.size();
    }
};
