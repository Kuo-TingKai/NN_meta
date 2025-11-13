# C++ Metaprogramming for NN Compilation

**English** | [中文](README.md)

A simple executable example project demonstrating how to use C++ Metaprogramming techniques to implement a neural network compiler.

## 📚 Related Documentation

- 📖 [C++ Core Technologies Cheat Sheet](CXX_CHEATSHEET_EN.md) - Detailed C++ technology reference
- 📊 [Benchmark Comparison Report](benchmark_comparison_report_EN.md) - Performance comparison analysis

## Project Overview

This project demonstrates the application of the following C++ Metaprogramming techniques in NN compilers:

1. **Template Metaprogramming (T-MP)**: For creating type-safe tensor operations
2. **Expression Templates**: For optimizing tensor expressions and avoiding temporary objects
3. **Constexpr**: For compile-time computation and optimization
4. **Type System**: For compile-time shape checking and inference
5. **Compile-time Kernel Generation**: For generating optimized code based on tensor shapes

## Project Structure

```
nn_meta/
├── CMakeLists.txt          # CMake build configuration
├── README.md               # This file (Chinese)
├── README_EN.md            # This file (English)
├── CXX_CHEATSHEET.md       # C++ core technologies quick reference (Chinese)
├── CXX_CHEATSHEET_EN.md    # C++ core technologies quick reference (English)
├── benchmark_comparison_report.md  # Benchmark comparison report (Chinese)
├── benchmark_comparison_report_EN.md  # Benchmark comparison report (English)
├── run_benchmarks.sh       # Benchmark execution script
├── include/                # Header files directory
│   ├── tensor.hpp          # Tensor class (using T-MP)
│   ├── expression_template.hpp  # Expression templates
│   ├── nn_compiler.hpp      # NN compiler utilities
│   └── benchmark.hpp        # Benchmark utilities
├── src/
│   ├── main.cpp            # Main program and examples
│   └── benchmark_cpp.cpp   # C++ benchmark program
└── benchmarks/             # Benchmark scripts directory
    ├── benchmark_pytorch.py    # PyTorch benchmark
    └── compare_benchmarks.py   # Comparison report generator
```

## Build and Execution

### Prerequisites

- C++20 or higher compiler (GCC 10+, Clang 10+, MSVC 2019+)
- CMake 3.15 or higher
- Python 3.6+ (optional, for benchmark comparison)
- PyTorch (optional, for benchmark comparison)

### Build Steps

```bash
# Create build directory
mkdir build
cd build

# Configure project
cmake ..

# Build
cmake --build .

# Execute
./NNMetaProgramming
```

Or use a single command:

```bash
mkdir -p build && cd build && cmake .. && cmake --build . && ./NNMetaProgramming
```

### Build Benchmark Program

```bash
cd build
cmake --build . --target benchmark_cpp
./benchmark_cpp
```

## Feature Demonstrations

### 1. Template Metaprogramming: Type-safe Tensors

Create tensors with compile-time known shapes using Template Metaprogramming:

```cpp
Tensor<float, 2, 3> tensor_a{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
// Shape [2, 3] is determined at compile time, type system guarantees type safety
```

### 2. Expression Templates: Optimized Expressions

Use expression templates to avoid creating temporary objects:

```cpp
auto expr_result = expr(a) + expr(b);
// No temporary tensor objects are created, evaluated directly during computation
```

### 3. Compile-time Kernel Generation

Select optimal implementation based on tensor size at compile time:

```cpp
auto result = matmul(mat_a, mat_b);
// For small matrices, use fully unrolled loops
// For large matrices, use runtime loops
```

### 4. Constexpr Compile-time Computation

Use `constexpr` to perform computations at compile time:

```cpp
constexpr std::size_t fact_5 = constexpr_utils::factorial(5);
// Computed at compile time, zero runtime overhead
```

### 5. Type-safe Neural Network Layers

Define layers with compile-time known input/output shapes:

```cpp
LinearLayer<float, 3, 2> layer;  // 3 inputs -> 2 outputs
// Type system guarantees shape matching
```

## C++ Core Technologies Reference

This project uses extensive modern C++ techniques. For detailed technical reference, see:

📖 **[C++ Core Technologies Cheat Sheet](CXX_CHEATSHEET_EN.md)** | [中文](CXX_CHEATSHEET.md)

Includes detailed explanations and examples of:
- Template Metaprogramming
- Constexpr and compile-time computation
- Variadic Templates
- Fold Expressions
- if constexpr
- Type Traits
- Expression Templates
- CRTP Pattern
- Lambda Expressions
- And other modern C++ features

## Key Technical Points

### Advantages of Template Metaprogramming

1. **Type Safety**: Compile-time checking of tensor shape matching
2. **Zero-overhead Abstraction**: Compiler can completely optimize away abstraction layers
3. **Compile-time Optimization**: Generate optimal code based on shapes

### Advantages of Expression Templates

1. **Avoid Temporary Objects**: Expressions are computed only when evaluated, no intermediate tensors created
2. **Lazy Evaluation**: Can optimize entire expression trees
3. **Performance Improvement**: Reduces memory allocation and copying

### Advantages of Constexpr

1. **Compile-time Computation**: Constants computed at compile time, zero runtime overhead
2. **Type Deduction**: Compiler can deduce more information for optimization
3. **Metaprogramming Enhancement**: Makes template metaprogramming more powerful and easier to use

## Benchmark Comparison

This project includes benchmark comparison functionality with mainstream deep learning frameworks (PyTorch).

📊 **[Detailed Benchmark Report](benchmark_comparison_report_EN.md)** | [中文](benchmark_comparison_report.md)

### Running Benchmarks

#### Method 1: Using Automation Script (Recommended)

```bash
./run_benchmarks.sh
```

This script will:
1. Automatically build the C++ benchmark program
2. Run C++ benchmark
3. Run PyTorch benchmark
4. Generate comparison report

#### Method 2: Manual Execution

```bash
# 1. Build C++ benchmark
cd build
cmake --build . --target benchmark_cpp

# 2. Run C++ benchmark
./benchmark_cpp

# 3. Run PyTorch benchmark
cd ../benchmarks
python3 benchmark_pytorch.py

# 4. Generate comparison report (requires manual output parsing)
```

### Benchmark Contents

Benchmarks include comparisons of the following operations:

1. **Matrix Multiplication (MatMul)**
   - Small matrices (4x4)
   - Medium matrices (32x32)
   - Large matrices (128x128)

2. **ReLU Activation Function**
   - Small tensors (16 elements)
   - Medium tensors (1024 elements)
   - Large tensors (4096 elements)

3. **Linear Layer Forward Pass**
   - Small layer (64 -> 32)
   - Medium layer (256 -> 128)
   - Large layer (1024 -> 512)

4. **Element-wise Operations**
   - Tensor addition

### Benchmark Results Interpretation

Benchmarks output the following statistics:
- **Mean**: Average execution time (microseconds)
- **Median**: Median execution time (microseconds)
- **StdDev**: Standard deviation
- **Min/Max**: Minimum/maximum execution time

Comparison reports show:
- Execution times for each framework
- Relative speedup
- Performance analysis

### Notes

1. **Compiler Optimization**: C++ benchmark uses `-O3` optimization, which may cause some small operations to be completely optimized away
2. **PyTorch Version**: Ensure PyTorch is installed (`pip install torch`)
3. **System Load**: Recommend running benchmarks when system load is low for accurate results
4. **CPU vs GPU**: PyTorch benchmark automatically detects GPU usage, C++ version currently uses CPU only

## Relationship with NN Compilers

These techniques are widely used in actual NN compilers (such as TVM, TensorFlow XLA):

1. **Efficient Kernel Generation**: Generate highly optimized CUDA/OpenCL kernels based on specific layer/tensor shapes
2. **Tensor Operation Optimization**: Use expression templates to optimize complex tensor operation chains
3. **Type System**: Check model correctness at compile time, avoid runtime errors
4. **Compile-time Optimization**: Use constexpr and template specialization for compile-time optimization

## Future Directions

As C++ standards evolve, the following features will further enhance NN compiler development:

- **Reflection (C++23/26)**: Allow compile-time inspection and manipulation of code structure
- **Metaclasses (C++26/29)**: Allow defining how classes are created, simplifying layer definitions
- **Enhanced Constexpr**: More powerful compile-time computation capabilities

## References

- [C++ Template Metaprogramming](https://en.cppreference.com/w/cpp/language/templates)
- [Expression Templates](https://en.wikipedia.org/wiki/Expression_templates)
- [Constexpr](https://en.cppreference.com/w/cpp/language/constexpr)
- [TVM Compiler Stack](https://tvm.apache.org/)

## License

This project is for learning and demonstration purposes only.

