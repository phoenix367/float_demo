﻿/*

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

constexpr size_t SAMPLES_COUNT = 10000000;
using FloatType = float;

#define DECLARE_TEST(TestName) \
    (TestSuite<FloatType, \
        SAMPLES_COUNT>(std::make_unique<TestName<FloatType>>())\
    )
#define DECLARE_TEST_WITH_RANGE(TestName, RangeMin, RangeMax) \
    (TestSuite<FloatType, \
        SAMPLES_COUNT>(std::make_unique<TestName<FloatType>>(), RangeMin, RangeMax)\
    )

int main()
{
    auto tests = {
        DECLARE_TEST_WITH_RANGE(ExponentAVX, 1.0e-10f, 70.0f),
        DECLARE_TEST_WITH_RANGE(Exponent, 1.0e-10f, 70.0f),
        DECLARE_TEST(DivideTo256AVX),
        DECLARE_TEST(SquareRootAVX),
        DECLARE_TEST(InverseSquareRootAVX),
        DECLARE_TEST(InvertNumberAVX),
        DECLARE_TEST(InvertNumberVerA),
        DECLARE_TEST(InvertNumberVerB),
        DECLARE_TEST(SquareRoot),
        DECLARE_TEST(InverseSquareRoot),
        DECLARE_TEST(DivideTo256)
    };

    for (auto it = tests.begin(); it != tests.end(); it++) {
        it->run_single_float();
    }
}