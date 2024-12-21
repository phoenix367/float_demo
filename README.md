# Float Demo

A simple demonstration of floating-point arithmetic and its unique behaviors, aimed at exploring the quirks and edge cases of floating-point calculations in C++.

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

## Contribution

Contributions are welcome! Please fork the repository and create a pull request with your improvements or additional examples.

## License

This project is available under BSD open-source license. See the ``LICENSE.txt`` file for details.

## References

- https://bits.stephan-brumme.com/
- https://nic.schraudolph.org/pubs/Schraudolph99.pdf (N. N. Schraudolph, "A Fast, Compact Approximation of the Exponential Function," in Neural Computation, vol. 11, no. 4, pp. 853-862, 15 May 1999, doi: 10.1162/089976699300016467.)
- http://www.machinedlearnings.com/2011/06/fast-approximate-logarithm-exponential.html
- https://en.wikipedia.org/wiki/Fast_inverse_square_root
