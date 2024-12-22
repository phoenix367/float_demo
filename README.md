# Float Demo

A simple demonstration of floating-point arithmetic and its unique behaviors, aimed at exploring the quirks and edge cases of floating-point calculations in C++. The benchmark measures performance of several floating operations: square root, inverse square root, exponent, inverse number, division to specific constant.

## Disclaimer

This is just demo code and author doesn't provide any guarantees about its correctness and bug-free. Please be careful with benchmark results interpretation.

## Features

- **Floating-Point Operations**: Demonstrates common pitfalls and hacks involving floating-point arithmetic.
- **Educational**: Aims to provide insights into how floating-point numbers work in programming and how can we hack some FP-operations.
- **Lightweight**: Simple codebase to facilitate understanding and experimentation.

## Prerequisites

- Microsoft Visual Studio C++ 2022 Community edition or compatible IDE
- Arduino IDE 2.3.3 or higher if you would like to build and run benchmark on Arduino UNO board

## Getting Started

### Clone the Repository

```bash
git clone https://github.com/phoenix367/float_demo.git
cd float_demo
git checkout
```

### Build instructions

#### For x86 platform

- Open the solution file ``float_demo.sln`` in Visual Studio (or use a compatible IDE).
- Build ``float_demo`` project.
- Run the executable to explore the floating-point benchmark.

#### For Arduino platform

- Run Arduino IDE and open ``float_demo.ino`` sketch. You can found in ``arduino`` folder.
- Connect Arduino UNO board via USB cable
- Build and upload sketch to the board
- Connect serial terminal to board virtual COM-port to see benchmark outputs


### Example usage
The project includes sample cases that highlight floating-point behaviors such as:

- Precision errors.
- Rounding anomalies.
- Overflow and underflow scenarios.

Run the program to see these behaviors and modify the source code to experiment with your own examples.

## Some performance measures

There was made some performance measurements for two platforms: x86-based laptop and Arduino UNO board.

### x86 platform

Target processor: ``Intel(R) Core(TM) i5-8350U CPU @ 1.70GHz``

**Accuracy measurements**:
```
+---------------------------+-----------------+-----------+-----------+----------------+
| Function name             | Mean rel. error | Median    | 95% error | Max rel. error |
+---------------------------+-----------------+-----------+-----------+----------------+
| Exponent (AVX)            | 0.0240699       | 0.0273437 | 0.0296321 | 0.0398002      |
| Exponent                  | 0.0241446       | 0.0273437 | 0.0313367 | 0.0423965      |
| Divide to 256 (AVX)       | 0               | 0         | 0         | 0              |
| Divide to 256             | 0               | 0         | 0         | 0              |
| Square root (AVX)         | 0.020141        | 0.0150564 | 0.0546945 | 0.0606601      |
| Square root               | 0.0201444       | 0.0150623 | 0.0547016 | 0.0606601      |
| Inverse Square root (AVX) | 0.0229901       | 0.0255445 | 0.0338016 | 0.0345307      |
| Inverse Square root       | 0.0229894       | 0.0255425 | 0.0337852 | 0.0343655      |
| Number inversion (AVX)    | 0.0281091       | 0.0243195 | 0.0659718 | 0.13332        |
| Number inversion (ver. A) | 0.0281062       | 0.0243226 | 0.0659718 | 0.133302       |
| Number inversion (ver. B) | 0.082048        | 0.0924384 | 0.124757  | 0.125          |
+---------------------------+-----------------+-----------+-----------+----------------+
```

**Performance measurements**:
```
+---------------------------+-------------------+----------------------+----------------------+
| Function name             | Reference, op./uS | Approximated, op./uS | Performance increase |
+---------------------------+-------------------+----------------------+----------------------+
| Exponent (AVX)            | 1590.89           | 2125.93              | 1.33631              |
| Exponent                  | 171.391           | 195.199              | 1.13891              |
| Divide to 256 (AVX)       | 750.043           | 2117.42              | 2.82307              |
| Divide to 256             | 511.376           | 665.674              | 1.30173              |
| Square root (AVX)         | 2075.04           | 1998.92              | 0.963315             |
| Square root               | 1031.59           | 1127.28              | 1.09276              |
| Inverse Square root (AVX) | 2009.7            | 2125.51              | 1.05763              |
| Inverse Square root       | 552.853           | 1045.12              | 1.89042              |
| Number inversion (AVX)    | 1459.24           | 2071.18              | 1.41935              |
| Number inversion (ver. A) | 760.084           | 1363.11              | 1.79337              |
| Number inversion (ver. B) | 654.226           | 1025.42              | 1.56739              |
+---------------------------+-------------------+----------------------+----------------------+
```

**Floating-point errors**
```
+---------------------------+-----------------------------------+---------------+-------------+-------------+
| Function name             | Test range                        | Valid samples | Unexp. inf. | Unexp. den. |
+---------------------------+-----------------------------------+---------------+-------------+-------------+
| Exponent (AVX)            | [-70..-1e-10] and                 | 10000000      | 0           | 0           |
|                           | [1e-10..70]                       |               |             |             |
+---------------------------+-----------------------------------+---------------+-------------+-------------+
| Exponent                  | [-70..-1e-10] and                 | 10000000      | 0           | 0           |
|                           | [1e-10..70]                       |               |             |             |
+---------------------------+-----------------------------------+---------------+-------------+-------------+
| Divide to 256 (AVX)       | [-3.40282e+38..-1.17549e-38] and  | 9684847       | 0           | 0           |
|                           | [1.17549e-38..3.40282e+38]        |               |             |             |
+---------------------------+-----------------------------------+---------------+-------------+-------------+
| Divide to 256             | [-3.40282e+38..-1.17549e-38] and  | 9684839       | 0           | 0           |
|                           | [1.17549e-38..3.40282e+38]        |               |             |             |
+---------------------------+-----------------------------------+---------------+-------------+-------------+
| Square root (AVX)         | [1.17549e-38..3.40282e+38]        | 10000000      | 0           | 0           |
+---------------------------+-----------------------------------+---------------+-------------+-------------+
| Square root               | [1.17549e-38..3.40282e+38]        | 10000000      | 0           | 0           |
+---------------------------+-----------------------------------+---------------+-------------+-------------+
| Inverse Square root (AVX) | [1.17549e-38..3.40282e+38]        | 9999999       | 0           | 0           |
+---------------------------+-----------------------------------+---------------+-------------+-------------+
| Inverse Square root       | [1.17549e-38..3.40282e+38]        | 10000000      | 0           | 0           |
+---------------------------+-----------------------------------+---------------+-------------+-------------+
| Number inversion (AVX)    | [-3.40282e+38..-1.17549e-38] and  | 9921147       | 0           | 3804        |
|                           | [1.17549e-38..3.40282e+38]        |               |             |             |
+---------------------------+-----------------------------------+---------------+-------------+-------------+
| Number inversion (ver. A) | [-3.40282e+38..-1.17549e-38] and  | 9921317       | 0           | 3844        |
|                           | [1.17549e-38..3.40282e+38]        |               |             |             |
+---------------------------+-----------------------------------+---------------+-------------+-------------+
| Number inversion (ver. B) | [-3.40282e+38..-1.17549e-38] and  | 9921065       | 0           | 0           |
|                           | [1.17549e-38..3.40282e+38]        |               |             |             |
+---------------------------+-----------------------------------+---------------+-------------+-------------+
```

## Contribution

Contributions are welcome! Please fork the repository and create a pull request with your improvements or additional examples.

## License

This project is available under BSD open-source license. See the ``LICENSE.txt`` file for details.

## References

1. https://bits.stephan-brumme.com/
2. https://nic.schraudolph.org/pubs/Schraudolph99.pdf (N. N. Schraudolph, "A Fast, Compact Approximation of the Exponential Function," in Neural Computation, vol. 11, no. 4, pp. 853-862, 15 May 1999, doi: 10.1162/089976699300016467.)
3. http://www.machinedlearnings.com/2011/06/fast-approximate-logarithm-exponential.html
4. https://en.wikipedia.org/wiki/Fast_inverse_square_root
