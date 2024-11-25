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
#include <random>
#include <cmath>
#include <sstream>

template<typename FLOAT_TYPE>
    requires std::floating_point<FLOAT_TYPE>
class LogScaleRNG {
public:
    LogScaleRNG(FLOAT_TYPE rangeMin, FLOAT_TYPE rangeMax, bool enableNegatives = true)
        : _e2(_rd())
        , _enableNegatives(enableNegatives) {
        FLOAT_TYPE rangeMinLog = logf(rangeMin);
        FLOAT_TYPE rangeMaxLog = logf(rangeMax);
        _rangeMin = rangeMin;
        _rangeMax = rangeMax;

        _dist = std::uniform_real_distribution<FLOAT_TYPE>(rangeMinLog, rangeMaxLog);
        _signDist = std::uniform_int_distribution<int>(0, 1);
    }

    FLOAT_TYPE getRandomNumber() {
        FLOAT_TYPE number;

        do {
            FLOAT_TYPE logNumber = _dist(_e2);
            number = exp(logNumber);
            if (_enableNegatives && _signDist(_e2)) {
                number = -number;
            }
        } while (!std::isfinite(number) && !std::isnormal(number));

        return number;
    }

    const std::string getRangeStr() const {
        std::ostringstream resultStream;

        if (_enableNegatives) {
            resultStream << "[" << -_rangeMax << ".." << -_rangeMin << "] and \n["
                << _rangeMin << ".." << _rangeMax << "]";
        } else {
            resultStream << "[" << _rangeMin << ".." << _rangeMax << "]";
        }

        return resultStream.str();
    }

private:
    std::random_device _rd;
    std::mt19937 _e2;
    std::uniform_real_distribution<FLOAT_TYPE> _dist;
    std::uniform_int_distribution<int> _signDist;
    bool _enableNegatives;
    FLOAT_TYPE _rangeMin;
    FLOAT_TYPE _rangeMax;
};
