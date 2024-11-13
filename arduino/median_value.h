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

#ifndef MEDIAN_VALUE_H
#define MEDIAN_VALUE_H

#include "heaps.h"

template <typename FLOAT_TYPE>
class MedianValue {
public:
  MedianValue() = default;

  void addNumber(FLOAT_TYPE num) {
    // Insert into max-heap by default
    maxHeap.insert(num);

    // Ensure the largest in max-heap is <= smallest in min-heap
    if (maxHeap.size > 0 && minHeap.size > 0 && maxHeap.getMax() > minHeap.getMin()) {
      FLOAT_TYPE maxTop = maxHeap.extractMax();
      minHeap.insert(maxTop);
    }

    // Balance the sizes of the heaps
    if (maxHeap.size > minHeap.size + 1) {
      minHeap.insert(maxHeap.extractMax());
    } else if (minHeap.size > maxHeap.size) {
      maxHeap.insert(minHeap.extractMin());
    }
  }

  FLOAT_TYPE getMedian() const {
    if (maxHeap.size > minHeap.size) {
      return maxHeap.getMax();
    } else {
      return (maxHeap.getMax() + minHeap.getMin()) / static_cast<FLOAT_TYPE>(2);
    }
  }

private:
  MaxHeap<FLOAT_TYPE> maxHeap; // Stores the lower half
  MinHeap<FLOAT_TYPE> minHeap; // Stores the upper half
};

#endif /* MEDIAN_VALUE_H */
