# C++ 核心技術 Cheat Sheet

[English](CXX_CHEATSHEET_EN.md) | **中文**

本專案使用的 C++ 核心技術快速參考表

## 📚 相關文檔

- 🏠 [專案主頁](README.md) - 專案概述和使用說明
- 📊 [Benchmark 比較報告](benchmark_comparison_report.md) - 性能比較分析

## 目錄
- [Template Metaprogramming](#template-metaprogramming)
- [Constexpr 與編譯時計算](#constexpr-與編譯時計算)
- [Variadic Templates](#variadic-templates)
- [Fold Expressions](#fold-expressions)
- [if constexpr](#if-constexpr)
- [Type Traits](#type-traits)
- [Expression Templates](#expression-templates)
- [CRTP 模式](#crtp-模式)
- [Lambda 表達式](#lambda-表達式)
- [其他技術](#其他技術)

---

## Template Metaprogramming

| 技術 | 語法 | 用途 | 範例 | 文件位置 |
|------|------|------|------|----------|
| **類別模板** | `template<typename T> class X` | 創建型別參數化的類別 | `template<typename T, std::size_t... Dims> class Tensor` | `tensor.hpp:14` |
| **函數模板** | `template<typename T> T func()` | 創建型別參數化的函數 | `template<typename T, std::size_t M, std::size_t N, std::size_t K> constexpr Tensor<T, M, K> matmul(...)` | `nn_compiler.hpp:24` |
| **非型別模板參數** | `template<int N> class X` | 使用值作為模板參數 | `template<typename T, std::size_t... Dims>` | `tensor.hpp:14` |
| **模板特化** | `template<> class X<int>` | 為特定型別提供特殊實現 | `template<typename T, std::size_t... Dims> struct is_tensor<Tensor<T, Dims...>>` | `tensor.hpp:117` |
| **模板別名** | `template<typename T> using Alias = X<T>` | 創建模板型別別名 | - | - |
| **依賴型別名稱** | `typename T::type` | 指定依賴型別 | `lhs_.template eval<Indices...>()` | `expression_template.hpp:44` |

---

## Constexpr 與編譯時計算

| 技術 | 語法 | 用途 | 範例 | 文件位置 |
|------|------|------|------|----------|
| **constexpr 變數** | `constexpr T x = value` | 編譯時常數 | `static constexpr std::size_t total_size = (Dims * ...)` | `tensor.hpp:18` |
| **constexpr 函數** | `constexpr T func()` | 可在編譯時執行的函數 | `constexpr Tensor<T, Dims...> relu(...)` | `nn_compiler.hpp:60` |
| **constexpr 建構函數** | `constexpr X() {}` | 可在編譯時創建物件 | `constexpr Tensor() : data_{} {}` | `tensor.hpp:29` |
| **constexpr if** | `if constexpr (condition)` | 編譯時條件分支 | `if constexpr (M <= 4 && N <= 4 && K <= 4)` | `nn_compiler.hpp:29` |
| **constexpr 遞迴** | `constexpr T fact(int n)` | 編譯時遞迴計算 | `constexpr std::size_t factorial(std::size_t n)` | `nn_compiler.hpp:139` |

---

## Variadic Templates

| 技術 | 語法 | 用途 | 範例 | 文件位置 |
|------|------|------|------|----------|
| **參數包** | `template<typename... Args>` | 接受任意數量的模板參數 | `template<typename T, std::size_t... Dims>` | `tensor.hpp:14` |
| **參數包展開** | `Args...` | 展開參數包 | `std::array<std::size_t, sizeof...(Dims)> shape = {Dims...}` | `tensor.hpp:21` |
| **sizeof...** | `sizeof...(Args)` | 獲取參數包大小 | `static_assert(sizeof...(Indices) == rank)` | `tensor.hpp:44` |
| **參數包轉發** | `f(Args... args)` | 轉發參數包 | `template<typename... Args> constexpr T& operator()(Args... indices)` | `tensor.hpp:56` |
| **參數包模式匹配** | `{args...}` | 初始化列表展開 | `std::array<std::size_t, rank> indices = {static_cast<std::size_t>(args)...}` | `tensor.hpp:101` |

---

## Fold Expressions

| 技術 | 語法 | 用途 | 範例 | 文件位置 |
|------|------|------|------|----------|
| **二元左折疊** | `(pack op ...)` | 從左到右折疊 | `(Dims * ...)` | `tensor.hpp:18` |
| **二元右折疊** | `(... op pack)` | 從右到左折疊 | - | - |
| **一元左折疊** | `(op ... pack)` | 一元運算符左折疊 | - | - |
| **一元右折疊** | `(pack op ...)` | 一元運算符右折疊 | - | - |
| **逗號折疊** | `(expr, ...)` | 展開表達式列表 | `((output.data()[I] = ...), ...)` | `nn_compiler.hpp:66` |

---

## if constexpr

| 技術 | 語法 | 用途 | 範例 | 文件位置 |
|------|------|------|------|----------|
| **編譯時條件** | `if constexpr (condition)` | 編譯時分支選擇 | `if constexpr (M <= 4 && N <= 4 && K <= 4)` | `nn_compiler.hpp:29` |
| **編譯時型別檢查** | `if constexpr (std::is_same_v<T, int>)` | 根據型別選擇實現 | `if constexpr (sizeof...(Dims1) != sizeof...(Dims2))` | `nn_compiler.hpp:81` |
| **條件編譯優化** | `if constexpr (size <= threshold)` | 根據大小選擇算法 | `if constexpr (Tensor<T, Dims...>::total_size <= 16)` | `nn_compiler.hpp:63` |

---

## Type Traits

| 技術 | 語法 | 用途 | 範例 | 文件位置 |
|------|------|------|------|----------|
| **自定義 Type Trait** | `template<typename T> struct trait` | 定義型別特性 | `template<typename T> struct is_tensor : std::false_type {}` | `tensor.hpp:114` |
| **Type Trait 變數模板** | `template<typename T> constexpr bool trait_v` | 型別特性值 | `template<typename T> constexpr bool is_tensor_v = is_tensor<T>::value` | `tensor.hpp:121` |
| **std::false_type / true_type** | `: std::false_type` | 布林型別特性基類 | `struct is_tensor : std::false_type {}` | `tensor.hpp:115` |
| **std::is_same_v** | `std::is_same_v<T1, T2>` | 檢查兩個型別是否相同 | - | - |
| **std::enable_if** | `std::enable_if_t<condition, T>` | 條件型別啟用 | - | - |

---

## Expression Templates

| 技術 | 語法 | 用途 | 範例 | 文件位置 |
|------|------|------|------|----------|
| **表達式基類** | `class ExprBase<Derived>` | CRTP 基類 | `class ExpressionBase<Derived>` | `expression_template.hpp:20` |
| **二元表達式** | `BinaryExpression<LHS, RHS, Op>` | 延遲求值表達式 | `class BinaryExpression<LHS, RHS, Op>` | `expression_template.hpp:32` |
| **運算符重載** | `operator+(const Expr&, const Expr&)` | 創建表達式樹 | `constexpr auto operator+(const ExpressionBase<LHS>&, const ExpressionBase<RHS>&)` | `expression_template.hpp:104` |
| **延遲求值** | `expr.eval()` | 在需要時才計算 | `template<std::size_t... Indices> constexpr auto eval() const` | `expression_template.hpp:43` |
| **表達式包裝** | `TensorExpression<Tensor>` | 將張量包裝為表達式 | `class TensorExpression : public ExpressionBase<TensorExpression<T, Dims...>>` | `expression_template.hpp:122` |

---

## CRTP 模式

| 技術 | 語法 | 用途 | 範例 | 文件位置 |
|------|------|------|------|----------|
| **CRTP 基類** | `template<typename Derived> class Base` | 靜態多型 | `template<typename Derived> class ExpressionBase` | `expression_template.hpp:19` |
| **派生類別轉換** | `static_cast<Derived&>(*this)` | 轉換為派生型別 | `return static_cast<const Derived&>(*this)` | `expression_template.hpp:23` |
| **CRTP 方法調用** | `derived().method()` | 調用派生類方法 | `lhs.derived(), rhs.derived()` | `expression_template.hpp:106` |

---

## Lambda 表達式

| 技術 | 語法 | 用途 | 範例 | 文件位置 |
|------|------|------|------|----------|
| **立即調用 Lambda** | `[&]<...>(...){}(args)` | 在表達式中執行代碼 | `[&]<std::size_t... I>(std::index_sequence<I...>) { ... }(std::make_index_sequence<M*K>{})` | `nn_compiler.hpp:31` |
| **泛型 Lambda** | `[&]<typename T>(T x){}` | C++20 泛型 Lambda | `[&]<std::size_t... I>(std::index_sequence<I...>)` | `nn_compiler.hpp:31` |
| **捕獲引用** | `[&]` | 按引用捕獲所有變數 | `[&]<std::size_t... I>(...) { ... }` | `nn_compiler.hpp:31` |
| **Lambda 折疊** | `([&](){...}(), ...)` | 在參數包上展開 Lambda | `([&]() { ... }(), ...)` | `nn_compiler.hpp:32` |

---

## 其他技術

| 技術 | 語法 | 用途 | 範例 | 文件位置 |
|------|------|------|------|----------|
| **std::index_sequence** | `std::index_sequence<I...>` | 編譯時整數序列 | `std::make_index_sequence<M * K>{}` | `nn_compiler.hpp:41` |
| **std::make_index_sequence** | `std::make_index_sequence<N>` | 生成索引序列 | `std::make_index_sequence<Tensor<T, Dims...>::total_size>{}` | `nn_compiler.hpp:67` |
| **static_assert** | `static_assert(condition, msg)` | 編譯時斷言 | `static_assert(sizeof...(Indices) == rank, "Number of indices must match tensor rank")` | `tensor.hpp:44` |
| **auto 型別推導** | `auto x = expr` | 自動推導型別 | `constexpr auto get_tensor_shape(...)` | `nn_compiler.hpp:19` |
| **decltype** | `decltype(expr)` | 獲取表達式型別 | - | - |
| **std::array** | `std::array<T, N>` | 固定大小陣列 | `std::array<T, total_size> data_` | `tensor.hpp:25` |
| **std::initializer_list** | `std::initializer_list<T>` | 初始化列表 | `constexpr Tensor(std::initializer_list<T> init)` | `tensor.hpp:32` |
| **volatile** | `volatile T x` | 防止編譯器優化 | `volatile float sum = 0.0f` | `benchmark_cpp.cpp:37` |

---

## 技術組合使用範例

### 範例 1: 編譯時張量大小計算
```cpp
// 使用 Fold Expression + Variadic Templates + constexpr
template<typename T, std::size_t... Dims>
class Tensor {
    static constexpr std::size_t total_size = (Dims * ...);  // Fold expression
    static constexpr std::array<std::size_t, sizeof...(Dims)> shape = {Dims...};  // Variadic
};
```

### 範例 2: 條件編譯優化
```cpp
// 使用 if constexpr + Lambda + index_sequence
template<typename T, std::size_t M, std::size_t N, std::size_t K>
constexpr Tensor<T, M, K> matmul(...) {
    if constexpr (M <= 4 && N <= 4 && K <= 4) {
        [&]<std::size_t... I>(std::index_sequence<I...>) {
            ([&]() { /* unroll loop */ }(), ...);
        }(std::make_index_sequence<M * K>{});
    }
}
```

### 範例 3: Expression Templates + CRTP
```cpp
// 使用 CRTP + Expression Templates
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

## 性能優化技巧

| 技術 | 說明 | 效果 |
|------|------|------|
| **編譯時展開** | 使用 `if constexpr` + `index_sequence` 展開小循環 | 消除循環開銷 |
| **零開銷抽象** | 使用模板和 constexpr 創建抽象層 | 編譯器完全優化 |
| **延遲求值** | Expression Templates 延遲計算 | 避免臨時物件 |
| **型別特化** | 為小尺寸提供特化版本 | 更好的優化機會 |
| **constexpr 函數** | 編譯時計算常數 | 零運行時開銷 |

---

## C++ 標準版本要求

| 技術 | C++ 標準 | 說明 |
|------|----------|------|
| Fold Expressions | C++17 | `(pack op ...)` |
| if constexpr | C++17 | 編譯時條件分支 |
| 泛型 Lambda | C++20 | `[&]<typename T>(T x){}` |
| constexpr 增強 | C++14/17/20 | 逐步增強 |
| Concepts | C++20 | 型別約束（本專案未使用） |
| Modules | C++20 | 模組系統（本專案未使用） |

---

## 相關文件位置

| 技術類別 | 主要文件 |
|----------|----------|
| Template Metaprogramming | `include/tensor.hpp` |
| Expression Templates | `include/expression_template.hpp` |
| 編譯時優化 | `include/nn_compiler.hpp` |
| Benchmark 工具 | `include/benchmark.hpp` |
| 使用範例 | `src/main.cpp`, `src/benchmark_cpp.cpp` |

---

## 快速參考

### 最常用的技術組合

1. **型別安全張量**: `template<typename T, std::size_t... Dims>` + `constexpr`
2. **編譯時優化**: `if constexpr` + `std::index_sequence` + Lambda
3. **零開銷抽象**: Expression Templates + CRTP
4. **編譯時計算**: `constexpr` + Fold Expressions
5. **型別檢查**: Type Traits + `static_assert`

---

**最後更新**: 2025-11-13  
**C++ 標準**: C++20  
**編譯器要求**: GCC 10+, Clang 10+, MSVC 2019+

