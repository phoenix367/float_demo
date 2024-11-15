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

#include <memory>
#include <numeric>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <tuple>
#include <fort.hpp>
#include <iostream>

#include "test_basic.h"
#include "log_scale_rng.h"

struct FunctionTestResult {
    std::string functionName;
    double avgRelativeError;
    double medianRelativeError;
    double relativeError95;
    double maxRelativeError;
    double referenceOpUSec;
    double approximateOpUSec;
    size_t totalExperiments;
    size_t validExperiments;
};

template<
    typename FloatType,
    typename size_t SAMPLES_COUNT
>
class TestSuite {
public:
    TestSuite(
        std::unique_ptr<TestBasic<FloatType>>&& testPtr,
        FloatType rangeMin = std::numeric_limits<FloatType>::min(),
        FloatType rangeMax = std::numeric_limits<FloatType>::max()
    )
    : _testPtr(std::move(testPtr))
    , _rng(rangeMin, rangeMax, testPtr->enableNegatives()) {

    }

    FunctionTestResult run_single_float() const {
        std::vector<FloatType> inputValues(SAMPLES_COUNT);
        std::vector<FloatType> referenceValues, actualValues;

        std::cout << "Run numeric test for function \"" << _testPtr->getFunctionName() <<
            "\"" << std::endl;

        std::for_each(inputValues.begin(), inputValues.end(), [this](auto& v) {
            v = _rng.getRandomNumber();
            });
        std::cout << "Generated " << SAMPLES_COUNT << " random samples for check." << std::endl;
        std::cout << "Compute approximated and reference values..." << std::endl;
        auto durationInfo = _testPtr->compute(inputValues, referenceValues, actualValues);
        if (referenceValues.size() != actualValues.size()) {
            throw std::runtime_error(
                "Size of array with reference values should be equal"
                " to sizeof array with actual values");
        }

        size_t processedExperiments = referenceValues.size();
        std::vector<double> relErrors(processedExperiments);
        std::vector<double>::iterator errorIt = relErrors.begin();

        std::cout << "Evaluate results..." << std::endl;
        std::cout << "Handled " << processedExperiments << " experiments." << std::endl;
        size_t infCount = 0, denCount = 0;

        for (size_t i = 0; i < processedExperiments; i++) {
            FloatType reference = referenceValues[i];
            FloatType result = actualValues[i];

            FloatType error = std::fabs(reference - result) / std::fabs(reference);
            if (std::isfinite(error) && std::isnormal(reference)) {
                *errorIt++ = error;

                if (error > static_cast<FloatType>(0.9)) {
                    std::cerr << "WARNING: too high value of relative error: " << error <<
                        ". Input value is " << inputValues[i] << ", expected value: " << reference <<
                        ", actual value: " << result << std::endl;
                }
            }

            if (std::isnormal(reference) && std::isfinite(reference)) {
                if (!std::isfinite(result)) {
                    infCount++;
                }
                else if (!std::isnormal(result)) {
                    denCount++;
                }
            }
        }

        size_t validExperiments = errorIt - relErrors.begin();
        std::sort(relErrors.begin(), errorIt);
        auto maxError = std::max_element(relErrors.begin(), errorIt);
        double errorSum = std::accumulate(relErrors.begin(), errorIt, 0.0);
        double median, percentile_95;

        std::tie(median, percentile_95) = compute_median_and_percentile(relErrors);
        double meanError = errorSum / validExperiments;

        std::cout << "Test results (accuracy):" << std::endl;
        fort::char_table  tableAccuracy;
        tableAccuracy.column(0).set_cell_text_align(fort::text_align::center);

        tableAccuracy << fort::header
            << "Function name" << "Mean rel. error" << "Median" << "95% error" << 
            "Max rel. error" << fort::endr
            << _testPtr->getFunctionName() << meanError << median << percentile_95 << *maxError << fort::endr;
        std::cout << tableAccuracy.to_string() << std::endl;

        std::cout << "Test results (performance):" << std::endl;
        fort::char_table  tablePerformance;
        tablePerformance << fort::header
            << "Reference op./uS" << "Approximated op./uS" << fort::endr
            << SAMPLES_COUNT / durationInfo.referenceDuration.count() 
            << SAMPLES_COUNT / durationInfo.approximateDuration.count() << fort::endr;
        std::cout << tablePerformance.to_string() << std::endl;

        FunctionTestResult testResult;

        testResult.approximateOpUSec = SAMPLES_COUNT / durationInfo.approximateDuration.count();
        testResult.referenceOpUSec = SAMPLES_COUNT / durationInfo.referenceDuration.count();
        testResult.functionName = _testPtr->getFunctionName();
        testResult.avgRelativeError = meanError;
        testResult.medianRelativeError = median;
        testResult.relativeError95 = percentile_95;
        testResult.maxRelativeError = *maxError;
        testResult.totalExperiments = relErrors.size();
        testResult.validExperiments = validExperiments;

        return testResult;
    }

private:
    static std::pair<double, double> compute_median_and_percentile(
        const std::vector<double>& sorted_values
    ) {
        if (sorted_values.empty()) {
            throw std::invalid_argument("The input vector is empty.");
        }

        // Calculate the median
        double median;
        size_t n = sorted_values.size();
        if (n % 2 == 0) {
            // Even number of elements
            median = (sorted_values[n / 2 - 1] + sorted_values[n / 2]) / 2.0;
        }
        else {
            // Odd number of elements
            median = sorted_values[n / 2];
        }

        // Calculate the 0.95 percentile
        size_t percentile_index = static_cast<size_t>(0.95 * n);
        double percentile_95 = sorted_values[percentile_index - 1]; // 0.95 percentile is at index 0.95*n - 1

        return { median, percentile_95 };
    }

private:
    mutable LogScaleRNG<FloatType> _rng;
    std::unique_ptr<TestBasic<FloatType>> _testPtr;
};
