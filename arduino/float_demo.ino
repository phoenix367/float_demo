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

#include <math.h>
#include <float.h>

#include "compute_functions.h"
#include "median_value.h"
#include "log_scale_rng.h"
#include "progress_bar.h"

#define ENABLE_PROGRESS_BAR       0

const int ITEMS_COUNT      =      64;
const uint32_t PASS_COUNT  =      5000;

float inputValues[ITEMS_COUNT];
float approxValues[ITEMS_COUNT];
float referenceValues[ITEMS_COUNT];

#define TEST_SUITE(testName, logMin, logMax, logScale, refFunc, approxFunc) \
  { \
    LogScaleRNG<float> logRNG(logMin, logMax, logScale); \
    uint32_t successfulTests = 0; \
    double cumulativeRelativeError = 0.0; \
    double maxRelativeError = 0.0; \
    uint32_t timeDiffRef = 0; \
    uint32_t timeDiffApp = 0; \
    MedianValue<float> mv; \
    Serial.print(F("Test name: ")); \
    Serial.println(testName); \
    ProgressBar pb(PASS_COUNT); \
    \
    if (ENABLE_PROGRESS_BAR) {\
      pb.begin(); \
    } \
    for (uint32_t pass = 0; pass < PASS_COUNT; pass++) { \
      for (int i = 0; i < ITEMS_COUNT; i++) { \
        inputValues[i] = logRNG.getRandomNumber(); \
      } \
      auto timeStartRef = micros(); \
      for (int i = 0; i < ITEMS_COUNT; i++) { \
        referenceValues[i] = refFunc(inputValues[i]); \
      } \
      auto timeEndRef = micros(); \
      auto timeStartApp = micros(); \
      for (int i = 0; i < ITEMS_COUNT; i++) { \
        approxValues[i] = approxFunc(inputValues[i]); \
      } \
      auto timeEndApp = micros(); \
      for (long i = 0; i < ITEMS_COUNT; i++) { \
        if (referenceValues[i] == 0.0f || !isfinite(approxValues[i])) { \
          continue; \
        } \
        float absDiff = fabs(referenceValues[i] - approxValues[i]); \
        float relError; \
        if (absDiff == 0.0f && referenceValues[i] == 0.0f) { \
          relError = 0.0f; \
        } else if (referenceValues[i] == 0.0f) { \
          continue; \
        } else { \
          relError = fabs(referenceValues[i] - approxValues[i]) / fabs(referenceValues[i]); \
        } \
        if (relError > 0.9) { \
          Serial.print("WARNING: relative error exceeds limit for value "); \
          Serial.println(inputValues[i]); \
        } \
        cumulativeRelativeError += relError; \
        maxRelativeError = max(maxRelativeError, relError); \
        mv.addNumber(relError); \
        successfulTests++; \
      } \
      timeDiffRef += timeEndRef - timeStartRef; \
      timeDiffApp += timeEndApp - timeStartApp; \
      if (ENABLE_PROGRESS_BAR) {\
        pb.update(pass + 1); \
      } \
    } \
    if (ENABLE_PROGRESS_BAR) {\
      pb.end(); \
    } \
    cumulativeRelativeError /= successfulTests; \
    timeDiffRef /= PASS_COUNT; \
    timeDiffApp /= PASS_COUNT; \
    \
    Serial.println(F("Test finished")); \
    Serial.print(F("Valid samples: ")); \
    Serial.print(successfulTests); \
    Serial.print(F(" of ")); \
    Serial.println(PASS_COUNT * ITEMS_COUNT); \
    Serial.print(F("Reference function performance, op/uSec: ")); \
    Serial.println(double(ITEMS_COUNT) / timeDiffRef, 4); \
    Serial.print(F("Approximated function performance, op/uSec: ")); \
    Serial.println(double(ITEMS_COUNT) / timeDiffApp, 4); \
    Serial.print(F("Performance increase: x")); \
    Serial.println(double(timeDiffRef) / timeDiffApp, 2); \
    Serial.print(F("Average error: ")); \
    Serial.println(cumulativeRelativeError, 5); \
    Serial.print(F("Median error: ")); \
    Serial.println(mv.getMedian(), 5); \
    Serial.print(F("Maximum relative error: ")); \
    Serial.println(maxRelativeError, 5); \
    Serial.println(); \
  }

void printLogo() {
  Serial.println(F("░█▀▀░█░░░█▀█░█▀█░▀█▀░░░█▀▄░█▀▀░█▄█░█▀█"));
  Serial.println(F("░█▀▀░█░░░█░█░█▀█░░█░░░░█░█░█▀▀░█░█░█░█"));
  Serial.println(F("░▀░░░▀▀▀░▀▀▀░▀░▀░░▀░░░░▀▀░░▀▀▀░▀░▀░▀▀▀"));
  Serial.println();
}

void setup() {
  Serial.begin(9600);

  printLogo();
  Serial.println(F("Start tests..."));

  TEST_SUITE(F("Exponent"), 5.9e-9f, 22.0f, true, reference_exp, approximate_exp);
  TEST_SUITE(F("Inverse number"), 5.9e-9f, 3.5e9f, true, reference_inverse_number, approximate_inverse_number);
  TEST_SUITE(F("Division to 256"), 5.9e-9f, FLT_MAX, true, reference_div256, approximate_div256);
  TEST_SUITE(F("Division to 256 (no zero check)"), 5.9e-9f, FLT_MAX, true, reference_div256, 
    approximate_div256_nozerocheck);
  TEST_SUITE(F("Square Root Test"), 5.9e-9f, 3.5e9f, false, reference_sqrt, approximate_sqrt);
  TEST_SUITE(F("Inverse Square Root Test"), 5.9e-9f, 3.5e9f, false, reference_isqrt, approximate_isqrt);

  Serial.println(F("All Done!"));
}

void loop() {

}
