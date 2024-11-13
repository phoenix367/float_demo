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

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS �AS IS� AND ANY
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

#include <immintrin.h>

#include "test_basic.h"

template <typename FLOAT_TYPE, typename TargetTest>
class AVXAcceleratedTest : public TestBasic<FLOAT_TYPE> {
public:
    virtual ~AVXAcceleratedTest() = default;

    TestBasic<FLOAT_TYPE>::DelayInfo compute(
        const std::vector<FLOAT_TYPE>& in,
        std::vector<FLOAT_TYPE>& targets,
        std::vector<FLOAT_TYPE>& actuals
    ) override {
        size_t processedSize = (in.size() / 8) * 8;
        targets.resize(processedSize);
        actuals.resize(processedSize);

        const FLOAT_TYPE* inputPtr = &in[0];
        if (reinterpret_cast<size_t>(inputPtr) % 32) {
            throw std::runtime_error("Invalid data alignment");
        }

        FLOAT_TYPE* targetPtr = &targets[0];
        FLOAT_TYPE* actualPtr = &actuals[0];

        std::vector<double> targetDelays(REPEAT_COUNT);
        for (size_t r = 0; r < REPEAT_COUNT; r++) {
            auto start = std::chrono::system_clock::now();
            for (size_t i = 0; i < processedSize; i += 8) {
                __m256 inputValue = _mm256_load_ps(inputPtr + i);
                __m256 res = TargetTest::reference_value(inputValue);
                _mm256_store_ps(targetPtr + i, res);
            }

            auto stop = std::chrono::system_clock::now();
            auto targetDelay = std::chrono::duration_cast<std::chrono::microseconds>(
                stop - start);
            targetDelays[r] = static_cast<double>(targetDelay.count());
        }

        std::vector<double> actualDelays(REPEAT_COUNT);
        for (size_t r = 0; r < REPEAT_COUNT; r++) {
            auto start = std::chrono::system_clock::now();
            for (size_t i = 0; i < processedSize; i += 8) {
                __m256 inputValue = _mm256_load_ps(inputPtr + i);
                __m256 res = TargetTest::approximate_value(inputValue);
                _mm256_store_ps(actualPtr + i, res);
            }

            auto stop = std::chrono::system_clock::now();
            auto actualDelay = std::chrono::duration_cast<std::chrono::microseconds>(
                stop - start);
            actualDelays[r] = static_cast<double>(actualDelay.count());
        }

        typename TestBasic<FLOAT_TYPE>::DelayInfo delayInfo;
        delayInfo.referenceDuration = TestBasic<FLOAT_TYPE>::DurationType(
            TestBasic<FLOAT_TYPE>::getDelay(targetDelays));
        delayInfo.approximateDuration = TestBasic<FLOAT_TYPE>::DurationType(
            TestBasic<FLOAT_TYPE>::getDelay(actualDelays));

        return delayInfo;
    }
};
