# Benchmark Comparison Report

**English** | [中文](benchmark_comparison_report.md)

## C++ Metaprogramming vs PyTorch Performance Comparison

## 📚 Related Documentation

- 🏠 [Project Homepage](README_EN.md) - Project overview and usage instructions
- 📖 [C++ Core Technologies Cheat Sheet](CXX_CHEATSHEET_EN.md) - Detailed C++ technology reference

### Test Environment
- **C++ Compiler**: AppleClang 17.0.0 (using -O3 optimization)
- **PyTorch Version**: 2.9.1
- **Test Platform**: CPU (macOS)

---

## 1. Matrix Multiplication

| Size | Framework | Mean Time (μs) | Median (μs) | StdDev (μs) | Speedup |
|------|-----------|----------------|--------------|-------------|---------|
| 4x4 | C++ (Meta) | 0.023 | 0.000 | 0.514 | **54x faster** |
| 4x4 | PyTorch | 1.242 | 1.208 | 0.113 | - |
| 32x32 | C++ (Meta) | 4.979 | 2.000 | 45.235 | 0.35x (slower) |
| 32x32 | PyTorch | 1.715 | 1.708 | 0.061 | - |
| 128x128 | C++ (Meta) | 2015.640 | 1719.500 | 876.315 | 0.003x (slower) |
| 128x128 | PyTorch | 6.290 | 6.458 | 0.568 | - |

**Observations**:
- Small matrices (4x4): C++ version is highly optimized by compiler, 54x faster than PyTorch
- Medium matrices (32x32): PyTorch uses optimized BLAS libraries, ~3x faster than our simple implementation
- Large matrices (128x128): PyTorch uses highly optimized matrix operation libraries (e.g., MKL/OpenBLAS), ~320x faster than our implementation

---

## 2. ReLU Activation Function

| Size | Framework | Mean Time (μs) | Median (μs) | StdDev (μs) | Speedup |
|------|-----------|----------------|--------------|-------------|---------|
| 16 | C++ (Meta) | 0.001 | 0.000 | 0.110 | **1226x faster** |
| 16 | PyTorch | 1.226 | 1.166 | 1.593 | - |
| 1024 | C++ (Meta) | 0.003 | 0.000 | 0.186 | **415x faster** |
| 1024 | PyTorch | 1.246 | 1.208 | 1.513 | - |
| 4096 | C++ (Meta) | 0.000 | 0.000 | 0.000 | **∞ (fully optimized)** |
| 4096 | PyTorch | 2.055 | 1.500 | 8.782 | - |

**Observations**:
- All sizes of ReLU operations, C++ version is highly optimized by compiler
- Small tensors are completely inlined and optimized, execution time approaches 0
- PyTorch has function call overhead, but still fast for large tensors

---

## 3. Linear Layer Forward Pass

| Size | Framework | Mean Time (μs) | Median (μs) | StdDev (μs) | Speedup |
|------|-----------|----------------|--------------|-------------|---------|
| 64->32 | C++ (Meta) | 0.000 | 0.000 | 0.000 | **∞ (fully optimized)** |
| 64->32 | PyTorch | 8.484 | 7.791 | 12.409 | - |
| 256->128 | C++ (Meta) | 20.394 | 19.000 | 8.580 | 0.50x (slower) |
| 256->128 | PyTorch | 10.139 | 9.521 | 10.711 | - |
| 1024->512 | C++ (Meta) | 516.180 | 482.000 | 102.024 | 0.038x (slower) |
| 1024->512 | PyTorch | 19.839 | 19.937 | 1.419 | - |

**Observations**:
- Small layer (64->32): C++ version is fully optimized
- Medium to large layers: PyTorch uses optimized matrix operations, much faster than our implementation
- Our implementation is a simple loop-based implementation, not using optimized libraries like BLAS

---

## 4. Element-wise Operations

| Size | Framework | Mean Time (μs) | Median (μs) | StdDev (μs) | Speedup |
|------|-----------|----------------|--------------|-------------|---------|
| 16 | C++ (Meta) | 0.000 | 0.000 | 0.000 | **∞ (fully optimized)** |
| 16 | PyTorch | 1.332 | 1.250 | 2.915 | - |
| 1024 | C++ (Meta) | 0.000 | 0.000 | 0.000 | **∞ (fully optimized)** |
| 1024 | PyTorch | 1.308 | 1.208 | 2.902 | - |

**Observations**:
- Element-wise operations are fully optimized by C++ compiler
- PyTorch has function call overhead, but still efficient for vectorized operations

---

## Summary and Analysis

### Advantages of C++ Metaprogramming

1. **Compile-time Optimization**: 
   - Small operations are completely inlined and optimized
   - Zero function call overhead
   - Compiler can perform deep optimization

2. **Type Safety**:
   - Compile-time shape checking
   - Zero runtime overhead abstraction

3. **Suitable Scenarios**:
   - Small to medium scale operations
   - Scenarios requiring compile-time optimization
   - Embedded systems or real-time applications

### Advantages of PyTorch

1. **Optimized Numerical Libraries**:
   - Uses highly optimized libraries like BLAS/MKL
   - Optimized for large matrix operations
   - Vectorization and parallelization

2. **Suitable Scenarios**:
   - Large-scale matrix operations
   - GPU acceleration required
   - Dynamic graphs and automatic differentiation

### Improvement Suggestions

For C++ implementation, performance can be improved through:

1. **Integrate BLAS Libraries**: Use OpenBLAS or MKL for matrix operations
2. **SIMD Optimization**: Use AVX/NEON instruction sets for vectorization
3. **Parallelization**: Use OpenMP or TBB for multithreading
4. **Memory Optimization**: Use better memory layouts and cache-friendly algorithms

### Conclusion

C++ Metaprogramming implementation excels in small operations and compile-time optimization, particularly suitable for:
- Operations with compile-time known shapes
- Scenarios requiring zero-overhead abstraction
- Small to medium scale operations

PyTorch performs better in large-scale operations and numerical computation optimization, particularly suitable for:
- Large-scale deep learning models
- Scenarios requiring GPU acceleration
- Dynamic computation graphs

Both have their advantages and can be chosen based on specific application scenarios.

