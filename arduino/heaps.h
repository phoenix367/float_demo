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

#ifndef HEAPS_H
#define HEAPS_H

const int MAX_HEAP_SIZE = 32;

// Helper function to swap two elements
template<typename T>
void swap(T &a, T &b) {
    T temp = a;
    a = b;
    b = temp;
}

// Max-Heap implementation
template<typename T>
class MaxHeap {
public:
  T heap[MAX_HEAP_SIZE];
  int size;

  MaxHeap() : size(0) {}

  void insert(T value) {
      if (size == MAX_HEAP_SIZE) {
        // Don't insert if heap is full
        return;
      }

      heap[size] = value;
      int i = size;
      size++;

      // Bubble up to maintain the max-heap property
      while (i != 0 && heap[(i - 1) / 2] < heap[i]) {
          swap(heap[i], heap[(i - 1) / 2]);
          i = (i - 1) / 2;
      }
  }

  T extractMax() {
    if (size == 0) {
      Serial.println("ERROR: MaxHeap is empty");
      while (1);
    }

    T root = heap[0];
    heap[0] = heap[size - 1];
    size--;
    maxHeapify(0);

    return root;
  }

  T getMax() {
    if (size == 0) {
      Serial.println("ERROR: MaxHeap is empty");
      while (1);
    }

    return heap[0];
  }

private:
  void maxHeapify(int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size && heap[left] > heap[largest]) {
      largest = left;
    }

    if (right < size && heap[right] > heap[largest]) {
      largest = right;
    }

    if (largest != i) {
      swap(heap[i], heap[largest]);
      maxHeapify(largest);
    }
  }
};

// Min-Heap implementation
template<typename T>
class MinHeap {
public:
  T heap[MAX_HEAP_SIZE];
  int size;

  MinHeap() : size(0) {}

  void insert(T value) {
    if (size == MAX_HEAP_SIZE) {
      // Don't insert if heap is full
      return;
    }

    heap[size] = value;
    int i = size;
    size++;

    // Bubble up to maintain the min-heap property
    while (i != 0 && heap[(i - 1) / 2] > heap[i]) {
      swap(heap[i], heap[(i - 1) / 2]);
      i = (i - 1) / 2;
    }
  }

  T extractMin() {
    if (size == 0) {
      Serial.println("ERROR: MinHeap is empty");
      while (1);
    }

    T root = heap[0];
    heap[0] = heap[size - 1];
    size--;
    minHeapify(0);
    return root;
  }

  T getMin() {
    if (size == 0) {
      Serial.println("ERROR: MinHeap is empty");
      while (1);
    }

    return heap[0];
  }

private:
  void minHeapify(int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size && heap[left] < heap[smallest]) {
        smallest = left;
    }

    if (right < size && heap[right] < heap[smallest]) {
        smallest = right;
    }

    if (smallest != i) {
        swap(heap[i], heap[smallest]);
        minHeapify(smallest);
    }
  }
};

#endif /* HEAPS_H */
