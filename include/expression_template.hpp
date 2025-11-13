#pragma once

#include "tensor.hpp"
#include <type_traits>

/**
 * @brief Expression Templates for optimized tensor operations
 * 
 * This demonstrates how expression templates can eliminate temporary objects
 * and enable compile-time optimization of tensor expressions, which is crucial
 * for NN compilation performance.
 */

// Forward declarations
template<typename T, std::size_t... Dims>
class TensorExpression;

// Base expression class
template<typename Derived>
class ExpressionBase {
public:
    constexpr const Derived& derived() const {
        return static_cast<const Derived&>(*this);
    }
    
    constexpr Derived& derived() {
        return static_cast<Derived&>(*this);
    }
};

// Binary operation expression
template<typename LHS, typename RHS, typename Op>
class BinaryExpression : public ExpressionBase<BinaryExpression<LHS, RHS, Op>> {
private:
    const LHS& lhs_;
    const RHS& rhs_;
    
public:
    constexpr BinaryExpression(const LHS& lhs, const RHS& rhs)
        : lhs_(lhs), rhs_(rhs) {}
    
    template<std::size_t... Indices>
    constexpr auto eval() const {
        return Op::apply(lhs_.template eval<Indices...>(), 
                        rhs_.template eval<Indices...>());
    }
    
    template<typename... Args>
    constexpr auto operator()(Args... args) const {
        return Op::apply(lhs_(args...), rhs_(args...));
    }
};

// Addition operation
struct AddOp {
    template<typename T1, typename T2>
    static constexpr auto apply(const T1& a, const T2& b) {
        return a + b;
    }
};

// Multiplication operation
struct MulOp {
    template<typename T1, typename T2>
    static constexpr auto apply(const T1& a, const T2& b) {
        return a * b;
    }
};

// Scalar multiplication
template<typename Scalar>
struct ScalarMulOp {
    Scalar scalar_;
    
    constexpr ScalarMulOp(Scalar s) : scalar_(s) {}
    
    template<typename T>
    constexpr auto apply(const T& val) const {
        return val * scalar_;
    }
};

// Expression wrapper for scalars
template<typename T>
class ScalarExpression : public ExpressionBase<ScalarExpression<T>> {
private:
    T value_;
    
public:
    constexpr ScalarExpression(T val) : value_(val) {}
    
    template<std::size_t... Indices>
    constexpr T eval() const {
        return value_;
    }
    
    template<typename... Args>
    constexpr T operator()(Args... /* args */) const {
        return value_;
    }
};

// Overload operators for tensor expressions
template<typename LHS, typename RHS>
constexpr auto operator+(const ExpressionBase<LHS>& lhs, const ExpressionBase<RHS>& rhs) {
    return BinaryExpression<LHS, RHS, AddOp>(lhs.derived(), rhs.derived());
}

template<typename LHS, typename RHS>
constexpr auto operator*(const ExpressionBase<LHS>& lhs, const ExpressionBase<RHS>& rhs) {
    return BinaryExpression<LHS, RHS, MulOp>(lhs.derived(), rhs.derived());
}

// Scalar multiplication
template<typename T, typename Expr>
constexpr auto operator*(T scalar, const ExpressionBase<Expr>& expr) {
    return BinaryExpression<ScalarExpression<T>, Expr, MulOp>(
        ScalarExpression<T>(scalar), expr.derived());
}

// Make Tensor compatible with expression templates
template<typename T, std::size_t... Dims>
class TensorExpression : public ExpressionBase<TensorExpression<T, Dims...>> {
private:
    const Tensor<T, Dims...>& tensor_;
    
public:
    constexpr TensorExpression(const Tensor<T, Dims...>& t) : tensor_(t) {}
    
    template<std::size_t... Indices>
    constexpr T eval() const {
        return tensor_.template at<Indices...>();
    }
    
    template<typename... Args>
    constexpr T operator()(Args... args) const {
        return tensor_(args...);
    }
};

// Helper function to create tensor expression
template<typename T, std::size_t... Dims>
constexpr auto expr(const Tensor<T, Dims...>& t) {
    return TensorExpression<T, Dims...>(t);
}

