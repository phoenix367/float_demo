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

#include <fort.hpp>
#include <list>

#include "avx_invert_number.h"
#include "avx_inv_sqrt.h"
#include "avx_sqrt.h"
#include "avx_div_256.h"
#include "avx_exp.h"
#include "exp_flt.h"
#include "avx_exp.h"
#include "invert_number.h"
#include "sqrt_fwd.h"
#include "inv_sqrt.h"
#include "div_256.h"
#include "test_suite.h"
#include "cpu_utils.h"

constexpr size_t SAMPLES_COUNT = 10000000;
constexpr size_t REPEAT_COUNT = 1000;

using FloatType = float;
using ResultsList = std::list< FunctionTestResult >;

constexpr float EXPONENT_MIN_RANGE = static_cast<FloatType>(1.0e-10);
constexpr float EXPONENT_MAX_RANGE = static_cast<FloatType>(70);

#define DECLARE_TEST(TestName) \
    (TestSuite<FloatType, \
        SAMPLES_COUNT, REPEAT_COUNT>(std::make_unique<TestName<FloatType, REPEAT_COUNT>>())\
    )
#define DECLARE_TEST_WITH_RANGE(TestName, RangeMin, RangeMax) \
    (TestSuite<FloatType, \
        SAMPLES_COUNT, REPEAT_COUNT>(std::make_unique<TestName<FloatType, REPEAT_COUNT>>(), RangeMin, RangeMax)\
    )

void printLogo() {
    std::cout 
        << R"(______ _             _    ______                     )" << std::endl
        << R"(|  ___| |           | |   |  _  \                    )" << std::endl
        << R"(| |_  | | ___   __ _| |_  | | | |___ _ __ ___   ___  )" << std::endl
        << R"(|  _| | |/ _ \ / _` | __| | | | / _ \ '_ ` _ \ / _ \ )" << std::endl
        << R"(| |   | | (_) | (_| | |_  | |/ /  __/ | | | | | (_) |)" << std::endl
        << R"(\_|   |_|\___/ \__,_|\__| |___/ \___|_| |_| |_|\___/ )" << std::endl
        << std::endl;

    std::cout
        << "This is a simple benchmark to check floating point hacks performance." << std::endl
        << "Copyright (c) Ivan Gubochkin (igubochkin@gmail.com), 2024" << std::endl
        << std::endl;
}

void printInputParameters() {
    std::cout << "Benchmark parameters:" << std::endl;

    std::cout << "Random samples per test: " << SAMPLES_COUNT << std::endl;
    std::cout << "Test repeat count:       " << REPEAT_COUNT << std::endl;
    std::cout << std::endl;
}

void printSummary(const ResultsList& results) {
    std::cout << "Benchmark results summary" << std::endl;
    std::cout << "Accuracy:" << std::endl;
    
    fort::char_table  tableAccuracy;
    fort::char_table  tablePerformance;
    fort::char_table  tableErrors;

    tableAccuracy << fort::header
        << "Function name" << "Mean rel. error" << "Median" << "95% error" <<
        "Max rel. error" << fort::endr;

    tablePerformance << fort::header
        << "Function name" << "Reference, op./uS" << "Approximated, op./uS" 
        << "Performance increase" << fort::endr;

    tableErrors << fort::header
        << "Function name" << "Test range" << "Valid samples"
        << "Unexp. inf." << "Unexp. den." << fort::endr;

    for (const auto& result : results) {
        tableAccuracy 
            << result.functionName << result.avgRelativeError 
            << result.medianRelativeError << result.relativeError95 
            << result.maxRelativeError << fort::endr;

        tablePerformance
            << result.functionName
            << result.referenceOpUSec << result.approximateOpUSec
            << result.approximateOpUSec / result.referenceOpUSec << fort::endr;

        tableErrors << fort::separator
            << result.functionName
            << result.rangeStr
            << result.validExperiments
            << result.infCount
            << result.denCount << fort::endr;
    }

    std::cout << tableAccuracy.to_string() << std::endl;

    std::cout << "Performance:" << std::endl;
    std::cout << tablePerformance.to_string() << std::endl;

    std::cout << "Errors:" << std::endl;
    std::cout << tableErrors.to_string() << std::endl;
}

void printCPUInfo() {
    std::cout << "CPU Info:" << std::endl;
    {
        const auto quantities = iware::cpu::quantities();
        std::cout << "\n"
            "  Quantities:\n"
            << "    Logical CPUs : " << quantities.logical << '\n'
            << "    Physical CPUs: " << quantities.physical << '\n'
            << "    CPU packages : " << quantities.packages << '\n';
    }

    {
        std::cout << "\n"
            "  Caches:\n";
        for (auto i = 1u; i <= 3; ++i) {
            const auto cache = iware::cpu::cache(i);
            std::cout << "    L" << i << ":\n"
                << "      Size         : " << cache.size << "B\n"
                << "      Line size    : " << cache.line_size << "B\n"
                << "      Associativity: " << static_cast<unsigned int>(cache.associativity) << '\n'
                << "      Type         : " << cache_type_name(cache.type) << '\n';
        }
    }

    {
        std::cout << "\n"
            << "  Architecture: " << architecture_name(iware::cpu::architecture()) << '\n'
            << "  Frequency: " << iware::cpu::frequency() << " Hz\n"
            << "  Endianness: " << endianness_name(iware::cpu::endianness()) << '\n'
            << "  Model name: " << iware::cpu::model_name() << '\n'
            << "  Vendor ID: " << iware::cpu::vendor_id() << '\n';
    }

    {
        std::cout << std::boolalpha
            << "\n"
            "  Instruction set support:\n";
        for (auto&& set : { std::make_pair("3D-now!", iware::cpu::instruction_set_t::s3d_now),  //
                          std::make_pair("MMX    ", iware::cpu::instruction_set_t::mmx),      //
                          std::make_pair("SSE    ", iware::cpu::instruction_set_t::sse),      //
                          std::make_pair("SSE2   ", iware::cpu::instruction_set_t::sse2),     //
                          std::make_pair("SSE3   ", iware::cpu::instruction_set_t::sse3),     //
                          std::make_pair("AVX    ", iware::cpu::instruction_set_t::avx),      //
                          std::make_pair("AVX2   ", iware::cpu::instruction_set_t::avx2),     //
                          std::make_pair("AVX512 ", iware::cpu::instruction_set_t::avx_512),     //
                          std::make_pair("Neon   ", iware::cpu::instruction_set_t::neon) })
            std::cout << "    " << set.first << ": " << iware::cpu::instruction_set_supported(set.second) << '\n';
    }

    std::cout << std::endl;
}

int main()
{
    auto tests = {
        DECLARE_TEST_WITH_RANGE(ExponentAVX, EXPONENT_MIN_RANGE, EXPONENT_MAX_RANGE),
        DECLARE_TEST_WITH_RANGE(Exponent, EXPONENT_MIN_RANGE, EXPONENT_MAX_RANGE),
        DECLARE_TEST(DivideTo256AVX),
        DECLARE_TEST(DivideTo256),
        DECLARE_TEST(SquareRootAVX),
        DECLARE_TEST(SquareRoot),
        DECLARE_TEST(InverseSquareRootAVX),
        DECLARE_TEST(InverseSquareRoot),
        DECLARE_TEST(InvertNumberAVX),
        DECLARE_TEST(InvertNumberVerA),
        DECLARE_TEST(InvertNumberVerB)
    };

    printLogo();
    printCPUInfo();
    printInputParameters();

    ResultsList results;
    for (auto it = tests.begin(); it != tests.end(); it++) {
        results.push_back(it->run_single_float());
    }

    printSummary(results);
}
