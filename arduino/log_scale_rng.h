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

#ifndef LOG_SCALE_RNG_H
#define LOG_SCALE_RNG_H

#include <math.h>

template<typename FLOAT_TYPE>
class LogScaleRNG {
public:
  LogScaleRNG(FLOAT_TYPE rangeMin, FLOAT_TYPE rangeMax, bool enableNegatives = true)
  : _enableNegatives(enableNegatives) {
    _rangeMinLog = logf(rangeMin);
    FLOAT_TYPE rangeMaxLog = logf(rangeMax);

    _rndRange = rangeMaxLog - _rangeMinLog;
  }

  FLOAT_TYPE getRandomNumber() {
      FLOAT_TYPE number;

      do {
        FLOAT_TYPE logNumber = getFloatRandom();
        number = exp(logNumber);
        if (_enableNegatives && getSignRandom()) {
          number = -number;
        }

      } while (!isfinite(number));

      return number;
  }

private:
  FLOAT_TYPE getFloatRandom() {
    long rndInt = random();
    FLOAT_TYPE normFloat = rndInt / static_cast<FLOAT_TYPE>(INT32_MAX);
    FLOAT_TYPE result = normFloat * _rndRange + _rangeMinLog;

    return result;
  }

  bool getSignRandom() {
    long rndInt = random();
    bool res = rndInt > INT32_MAX / 2;

    return res;
  }

private:
    FLOAT_TYPE _rndRange;
    FLOAT_TYPE _rangeMinLog;
    bool _enableNegatives;
};

#endif /* LOG_SCALE_RNG_H */
