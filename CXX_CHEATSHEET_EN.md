# C++ Core Technologies Cheat Sheet

**English** | [中文](CXX_CHEATSHEET.md)

Quick reference table for C++ core technologies used in this project

## 📚 Related Documentation

- 🏠 [Project Homepage](README_EN.md) - Project overview and usage instructions
- 📊 [Benchmark Comparison Report](benchmark_comparison_report_EN.md) - Performance comparison analysis

## Table of Contents
- [Template Metaprogramming](#template-metaprogramming)
- [Constexpr and Compile-time Computation](#constexpr-and-compile-time-computation)
- [Variadic Templates](#variadic-templates)
- [Fold Expressions](#fold-expressions)
- [if constexpr](#if-constexpr)
- [Type Traits](#type-traits)
- [Expression Templates](#expression-templates)
- [CRTP Pattern](#crtp-pattern)
- [Lambda Expressions](#lambda-expressions)
- [Other Technologies](#other-technologies)

---

## Template Metaprogramming

| Technique | Syntax | Purpose | Example | File Location |
|-----------|--------|---------|---------|---------------|
| **Class Template** | `template<typename T> class X` | Create type-parameterized classes | `template<typename T, std::size_t... Dims> class Tensor` | `tensor.hpp:14` |
| **Function Template** | `template<typename T> T func()` | Create type-parameterized functions | `template<typename T, std::size_t M, std::size_t N, std::size_t K> constexpr Tensor<T, M, K> matmul(...)` | `nn_compiler.hpp:24` |
| **Non-type Template Parameter** | `template<int N> class X` | Use values as template parameters | `template<typename T, std::size_t... Dims>` | `tensor.hpp:14` |
| **Template Specialization** | `template<> class X<int>` | Provide special implementation for specific types | `template<typename T, std::size_t... Dims> struct is_tensor<Tensor<T, Dims...>>` | `tensor.hpp:117` |
| **Template Alias** | `template<typename T> using Alias = X<T>` | Create template type aliases | - | - |
| **Dependent Type Name** | `typename T::type` | Specify dependent types | `lhs_.template eval<Indices...>()` | `expression_template.hpp:44` |

---

## Constexpr and Compile-time Computation

| Technique | Syntax | Purpose | Example | File Location |
|-----------|--------|---------|---------|---------------|
| **constexpr Variable** | `constexpr T x = value` | Compile-time constant | `static constexpr std::size_t total_size = (Dims * ...)` | `tensor.hpp:18` |
| **constexpr Function** | `constexpr T func()` | Function executable at compile time | `constexpr Tensor<T, Dims...> relu(...)` | `nn_compiler.hpp:60` |
| **constexpr Constructor** | `constexpr X() {}` | Create objects at compile time | `constexpr Tensor() : data_{} {}` | `tensor.hpp:29` |
| **constexpr if** | `if constexpr (condition)` | Compile-time conditional branch | `if constexpr (M <= 4 && N <= 4 && K <= 4)` | `nn_compiler.hpp:29` |
| **constexpr Recursion** | `constexpr T fact(int n)` | Compile-time recursive computation | `constexpr std::size_t factorial(std::size_t n)` | `nn_compiler.hpp:139` |

---

## Variadic Templates

| Technique | Syntax | Purpose | Example | File Location |
|-----------|--------|---------|---------|---------------|
| **Parameter Pack** | `template<typename... Args>` | Accept arbitrary number of template parameters | `template<typename T, std::size_t... Dims>` | `tensor.hpp:14` |
| **Parameter Pack Expansion** | `Args...` | Expand parameter pack | `std::array<std::size_t, sizeof...(Dims)> shape = {Dims...}` | `tensor.hpp:21` |
| **sizeof...** | `sizeof...(Args)` | Get parameter pack size | `static_assert(sizeof...(Indices) == rank)` | `tensor.hpp:44` |
| **Parameter Pack Forwarding** | `f(Args... args)` | Forward parameter pack | `template<typename... Args> constexpr T& operator()(Args... indices)` | `tensor.hpp:56` |
| **Parameter Pack Pattern Matching** | `{args...}` | Initialize list expansion | `std::array<std::size_t, rank> indices = {static_cast<std::size_t>(args)...}` | `tensor.hpp:101` |

---

## Fold Expressions

| Technique | Syntax | Purpose | Example | File Location |
|-----------|--------|---------|---------|---------------|
| **Binary Left Fold** | `(pack op ...)` | Fold from left to right | `(Dims * ...)` | `tensor.hpp:18` |
| **Binary Right Fold** | `(... op pack)` | Fold from right to left | - | - |
| **Unary Left Fold** | `(op ... pack)` | Unary operator left fold | - | - |
| **Unary Right Fold** | `(pack op ...)` | Unary operator right fold | - | - |
| **Comma Fold** | `(expr, ...)` | Expand expression list | `((output.data()[I] = ...), ...)` | `nn_compiler.hpp:66` |

---

## if constexpr

| Technique | Syntax | Purpose | Example | File Location |
|-----------|--------|---------|---------|---------------|
| **Compile-time Condition** | `if constexpr (condition)` | Compile-time branch selection | `if constexpr (M <= 4 && N <= 4 && K <= 4)` | `nn_compiler.hpp:29` |
| **Compile-time Type Check** | `if constexpr (std::is_same_v<T, int>)` | Select implementation based on type | `if constexpr (sizeof...(Dims1) != sizeof...(Dims2))` | `nn_compiler.hpp:81` |
| **Conditional Compile Optimization** | `if constexpr (size <= threshold)` | Select algorithm based on size | `if constexpr (Tensor<T, Dims...>::total_size <= 16)` | `nn_compiler.hpp:63` |

---

## Type Traits

| Technique | Syntax | Purpose | Example | File Location |
|-----------|--------|---------|---------|---------------|
| **Custom Type Trait** | `template<typename T> struct trait` | Define type characteristics | `template<typename T> struct is_tensor : std::false_type {}` | `tensor.hpp:114` |
| **Type Trait Variable Template** | `template<typename T> constexpr bool trait_v` | Type trait value | `template<typename T> constexpr bool is_tensor_v = is_tensor<T>::value` | `tensor.hpp:121` |
| **std::false_type / true_type** | `: std::false_type` | Boolean type trait base class | `struct is_tensor : std::false_type {}` | `tensor.hpp:115` |
| **std::is_same_v** | `std::is_same_v<T1, T2>` | Check if two types are the same | - | - |
| **std::enable_if** | `std::enable_if_t<condition, T>` | Conditionally enable type | - | - |

---

## Expression Templates

| Technique | Syntax | Purpose | Example | File Location |
|-----------|--------|---------|---------|---------------|
| **Expression Base Class** | `class ExprBase<Derived>` | CRTP base class | `class ExpressionBase<Derived>` | `expression_template.hpp:20` |
| **Binary Expression** | `BinaryExpression<LHS, RHS, Op>` | Lazy evaluation expression | `class BinaryExpression<LHS, RHS, Op>` | `expression_template.hpp:32` |
| **Operator Overload** | `operator+(const Expr&, const Expr&)` | Create expression tree | `constexpr auto operator+(const ExpressionBase<LHS>&, const ExpressionBase<RHS>&)` | `expression_template.hpp:104` |
| **Lazy Evaluation** | `expr.eval()` | Compute only when needed | `template<std::size_t... Indices> constexpr auto eval() const` | `expression_template.hpp:43` |
| **Expression Wrapper** | `TensorExpression<Tensor>` | Wrap tensor as expression | `class TensorExpression : public ExpressionBase<TensorExpression<T, Dims...>>` | `expression_template.hpp:122` |

---

## CRTP Pattern

| Technique | Syntax | Purpose | Example | File Location |
|-----------|--------|---------|---------|---------------|
| **CRTP Base Class** | `template<typename Derived> class Base` | Static polymorphism | `template<typename Derived> class ExpressionBase` | `expression_template.hpp:19` |
| **Derived Class Conversion** | `static_cast<Derived&>(*this)` | Convert to derived type | `return static_cast<const Derived&>(*this)` | `expression_template.hpp:23` |
| **CRTP Method Call** | `derived().method()` | Call derived class method | `lhs.derived(), rhs.derived()` | `expression_template.hpp:106` |

---

## Lambda Expressions

| Technique | Syntax | Purpose | Example | File Location |
|-----------|--------|---------|---------|---------------|
| **Immediately Invoked Lambda** | `[&]<...>(...){}(args)` | Execute code in expression | `[&]<std::size_t... I>(std::index_sequence<I...>) { ... }(std::make_index_sequence<M*K>{})` | `nn_compiler.hpp:31` |
| **Generic Lambda** | `[&]<typename T>(T x){}` | C++20 generic lambda | `[&]<std::size_t... I>(std::index_sequence<I...>)` | `nn_compiler.hpp:31` |
| **Capture by Reference** | `[&]` | Capture all variables by reference | `[&]<std::size_t... I>(...) { ... }` | `nn_compiler.hpp:31` |
| **Lambda Fold** | `([&](){...}(), ...)` | Expand lambda over parameter pack | `([&]() { ... }(), ...)` | `nn_compiler.hpp:32` |

---

## Other Technologies

| Technique | Syntax | Purpose | Example | File Location |
|-----------|--------|---------|---------|---------------|
| **std::index_sequence** | `std::index_sequence<I...>` | Compile-time integer sequence | `std::make_index_sequence<M * K>{}` | `nn_compiler.hpp:41` |
| **std::make_index_sequence** | `std::make_index_sequence<N>` | Generate index sequence | `std::make_index_sequence<Tensor<T, Dims...>::total_size>{}` | `nn_compiler.hpp:67` |
| **static_assert** | `static_assert(condition, msg)` | Compile-time assertion | `static_assert(sizeof...(Indices) == rank, "Number of indices must match tensor rank")` | `tensor.hpp:44` |
| **auto Type Deduction** | `auto x = expr` | Automatic type deduction | `constexpr auto get_tensor_shape(...)` | `nn_compiler.hpp:19` |
| **decltype** | `decltype(expr)` | Get expression type | - | - |
| **std::array** | `std::array<T, N>` | Fixed-size array | `std::array<T, total_size> data_` | `tensor.hpp:25` |
| **std::initializer_list** | `std::initializer_list<T>` | Initializer list | `constexpr Tensor(std::initializer_list<T> init)` | `tensor.hpp:32` |
| **volatile** | `volatile T x` | Prevent compiler optimization | `volatile float sum = 0.0f` | `benchmark_cpp.cpp:37` |

---

## Technology Combination Examples

### Example 1: Compile-time Tensor Size Calculation
```cpp
// Using Fold Expression + Variadic Templates + constexpr
template<typename T, std::size_t... Dims>
class Tensor {
    static constexpr std::size_t total_size = (Dims * ...);  // Fold expression
    static constexpr std::array<std::size_t, sizeof...(Dims)> shape = {Dims...};  // Variadic
};
```

### Example 2: Conditional Compile Optimization
```cpp
// Using if constexpr + Lambda + index_sequence
template<typename T, std::size_t M, std::size_t N, std::size_t K>
constexpr Tensor<T, M, K> matmul(...) {
    if constexpr (M <= 4 && N <= 4 && K <= 4) {
        [&]<std::size_t... I>(std::index_sequence<I...>) {
            ([&]() { /* unroll loop */ }(), ...);
        }(std::make_index_sequence<M * K>{});
    }
}
```

### Example 3: Expression Templates + CRTP
```cpp
// Using CRTP + Expression Templates
template<typename Derived>
class ExpressionBase {
    constexpr const Derived& derived() const {
        return static_cast<const Derived&>(*this);  // CRTP
    }
};

template<typename LHS, typename RHS, typename Op>
class BinaryExpression : public ExpressionBase<BinaryExpression<LHS, RHS, Op>> {
    // Expression template implementation
};
```

---

## Performance Optimization Tips

| Technique | Description | Effect |
|-----------|-------------|--------|
| **Compile-time Unrolling** | Use `if constexpr` + `index_sequence` to unroll small loops | Eliminate loop overhead |
| **Zero-overhead Abstraction** | Use templates and constexpr to create abstraction layers | Compiler fully optimizes |
| **Lazy Evaluation** | Expression Templates delay computation | Avoid temporary objects |
| **Type Specialization** | Provide specialized versions for small sizes | Better optimization opportunities |
| **constexpr Functions** | Compile-time constant computation | Zero runtime overhead |

---

## C++ Standard Version Requirements

| Technique | C++ Standard | Description |
|-----------|--------------|-------------|
| Fold Expressions | C++17 | `(pack op ...)` |
| if constexpr | C++17 | Compile-time conditional branch |
| Generic Lambda | C++20 | `[&]<typename T>(T x){}` |
| constexpr Enhancement | C++14/17/20 | Progressive enhancement |
| Concepts | C++20 | Type constraints (not used in this project) |
| Modules | C++20 | Module system (not used in this project) |

---

## Related File Locations

| Technology Category | Main Files |
|---------------------|------------|
| Template Metaprogramming | `include/tensor.hpp` |
| Expression Templates | `include/expression_template.hpp` |
| Compile-time Optimization | `include/nn_compiler.hpp` |
| Benchmark Utilities | `include/benchmark.hpp` |
| Usage Examples | `src/main.cpp`, `src/benchmark_cpp.cpp` |

---

## Quick Reference

### Most Common Technology Combinations

1. **Type-safe Tensor**: `template<typename T, std::size_t... Dims>` + `constexpr`
2. **Compile-time Optimization**: `if constexpr` + `std::index_sequence` + Lambda
3. **Zero-overhead Abstraction**: Expression Templates + CRTP
4. **Compile-time Computation**: `constexpr` + Fold Expressions
5. **Type Checking**: Type Traits + `static_assert`

---

**Last Updated**: 2025-11-13  
**C++ Standard**: C++20  
**Compiler Requirements**: GCC 10+, Clang 10+, MSVC 2019+

