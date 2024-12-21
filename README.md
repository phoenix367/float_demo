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
| Exponent (AVX)            | 0.0240685       | 0.0273437 | 0.0296367 | 0.0398003      |
| Exponent                  | 0.024138        | 0.0273437 | 0.0313344 | 0.0423967      |
| Divide to 256 (AVX)       | 0               | 0         | 0         | 0              |
| Divide to 256             | 0               | 0         | 0         | 0              |
| Square root (AVX)         | 0.020135        | 0.0150496 | 0.0546812 | 0.0606601      |
| Square root               | 0.020136        | 0.0150609 | 0.0546848 | 0.0606601      |
| Inverse Square root (AVX) | 0.0229898       | 0.0255443 | 0.033802  | 0.0345307      |
| Inverse Square root       | 0.022987        | 0.0255439 | 0.0337845 | 0.0343655      |
| Number inversion (AVX)    | 0.0281149       | 0.0243268 | 0.0659706 | 0.133302       |
| Number inversion (ver. A) | 0.0280952       | 0.0243137 | 0.0659722 | 0.13332        |
| Number inversion (ver. B) | 0.0820083       | 0.0923755 | 0.124757  | 0.125          |
+---------------------------+-----------------+-----------+-----------+----------------+
```

**Performance measurements**:
```
+---------------------------+-------------------+----------------------+----------------------+
| Function name             | Reference, op./uS | Approximated, op./uS | Performance increase |
+---------------------------+-------------------+----------------------+----------------------+
| Exponent (AVX)            | 1407.54           | 1997.29              | 1.41899              |
| Exponent                  | 160.94            | 175.946              | 1.09324              |
| Divide to 256 (AVX)       | 627.595           | 1907.24              | 3.03897              |
| Divide to 256             | 438.431           | 559.315              | 1.27572              |
| Square root (AVX)         | 1787.43           | 1894.05              | 1.05965              |
| Square root               | 886.839           | 1107.17              | 1.24845              |
| Inverse Square root (AVX) | 1725.11           | 1729.94              | 1.0028               |
| Inverse Square root       | 446.495           | 841.988              | 1.88577              |
| Number inversion (AVX)    | 1284.8            | 1873.83              | 1.45846              |
| Number inversion (ver. A) | 673.187           | 1117.7               | 1.66031              |
| Number inversion (ver. B) | 673.385           | 1144.24              | 1.69924              |
+---------------------------+-------------------+----------------------+----------------------+
```

**Floating-point errors**
```
Errors:
+---------------------------+-----------------------------------+---------------+-------------+-------------+
| Function name             | Test range                        | Valid samples | Unexp. inf. | Unexp. den. |
+---------------------------+-----------------------------------+---------------+-------------+-------------+
| Exponent (AVX)            | [-70..-1e-10] and                 | 10000000      | 0           | 0           |
|                           | [1e-10..70]                       |               |             |             |
+---------------------------+-----------------------------------+---------------+-------------+-------------+
| Exponent                  | [-70..-1e-10] and                 | 10000000      | 0           | 0           |
|                           | [1e-10..70]                       |               |             |             |
+---------------------------+-----------------------------------+---------------+-------------+-------------+
| Divide to 256 (AVX)       | [-3.40282e+38..-1.17549e-38] and  | 9685321       | 0           | 0           |
|                           | [1.17549e-38..3.40282e+38]        |               |             |             |
+---------------------------+-----------------------------------+---------------+-------------+-------------+
| Divide to 256             | [-3.40282e+38..-1.17549e-38] and  | 9685627       | 0           | 0           |
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
| Number inversion (AVX)    | [-3.40282e+38..-1.17549e-38] and  | 9921307       | 0           | 3970        |
|                           | [1.17549e-38..3.40282e+38]        |               |             |             |
+---------------------------+-----------------------------------+---------------+-------------+-------------+
| Number inversion (ver. A) | [-3.40282e+38..-1.17549e-38] and  | 9920824       | 0           | 3983        |
|                           | [1.17549e-38..3.40282e+38]        |               |             |             |
+---------------------------+-----------------------------------+---------------+-------------+-------------+
| Number inversion (ver. B) | [-3.40282e+38..-1.17549e-38] and  | 9921429       | 0           | 0           |
|                           | [1.17549e-38..3.40282e+38]        |               |             |             |
+---------------------------+-----------------------------------+---------------+-------------+-------------+
```

## Contribution

Contributions are welcome! Please fork the repository and create a pull request with your improvements or additional examples.

## License

This project is available under BSD open-source license. See the ``LICENSE.txt`` file for details.

## References

- https://bits.stephan-brumme.com/
- https://nic.schraudolph.org/pubs/Schraudolph99.pdf (N. N. Schraudolph, "A Fast, Compact Approximation of the Exponential Function," in Neural Computation, vol. 11, no. 4, pp. 853-862, 15 May 1999, doi: 10.1162/089976699300016467.)
- http://www.machinedlearnings.com/2011/06/fast-approximate-logarithm-exponential.html
- https://en.wikipedia.org/wiki/Fast_inverse_square_root
