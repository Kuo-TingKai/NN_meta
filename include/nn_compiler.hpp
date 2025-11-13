#pragma once

#include "tensor.hpp"
#include "expression_template.hpp"
#include <array>
#include <type_traits>

/**
 * @brief NN Compiler utilities using Metaprogramming
 * 
 * This demonstrates how C++ Metaprogramming can be used for:
 * 1. Compile-time kernel generation
 * 2. Type-safe layer definitions
 * 3. Shape inference at compile time
 */

// Compile-time shape inference
template<typename T, std::size_t... Dims>
constexpr auto get_tensor_shape(const Tensor<T, Dims...>&) {
    return std::array<std::size_t, sizeof...(Dims)>{Dims...};
}

// Matrix multiplication kernel (compile-time optimized)
template<typename T, std::size_t M, std::size_t N, std::size_t K>
constexpr Tensor<T, M, K> matmul(const Tensor<T, M, N>& a, const Tensor<T, N, K>& b) {
    Tensor<T, M, K> result;
    
    // Compile-time unrolled loops for small matrices
    if constexpr (M <= 4 && N <= 4 && K <= 4) {
        // Fully unroll for small matrices
        [&]<std::size_t... I>(std::index_sequence<I...>) {
            ([&]() {
                constexpr std::size_t i = I / K;
                constexpr std::size_t j = I % K;
                T sum = 0;
                for (std::size_t k = 0; k < N; ++k) {
                    sum += a(i, k) * b(k, j);
                }
                result(i, j) = sum;
            }(), ...);
        }(std::make_index_sequence<M * K>{});
    } else {
        // Runtime loops for larger matrices
        for (std::size_t i = 0; i < M; ++i) {
            for (std::size_t j = 0; j < K; ++j) {
                T sum = 0;
                for (std::size_t k = 0; k < N; ++k) {
                    sum += a(i, k) * b(k, j);
                }
                result(i, j) = sum;
            }
        }
    }
    
    return result;
}

// ReLU activation function (compile-time optimized)
template<typename T, std::size_t... Dims>
constexpr Tensor<T, Dims...> relu(const Tensor<T, Dims...>& input) {
    Tensor<T, Dims...> output;
    
    if constexpr (Tensor<T, Dims...>::total_size <= 16) {
        // Compile-time unroll for small tensors
        [&]<std::size_t... I>(std::index_sequence<I...>) {
            ((output.data()[I] = input.data()[I] > T(0) ? input.data()[I] : T(0)), ...);
        }(std::make_index_sequence<Tensor<T, Dims...>::total_size>{});
    } else {
        // Runtime loop for larger tensors
        for (std::size_t i = 0; i < Tensor<T, Dims...>::total_size; ++i) {
            output.data()[i] = input.data()[i] > T(0) ? input.data()[i] : T(0);
        }
    }
    
    return output;
}

// Compile-time shape validation
template<typename T1, std::size_t... Dims1, typename T2, std::size_t... Dims2>
constexpr bool shapes_match(const Tensor<T1, Dims1...>&, const Tensor<T2, Dims2...>&) {
    if constexpr (sizeof...(Dims1) != sizeof...(Dims2)) {
        return false;
    } else {
        return ((Dims1 == Dims2) && ...);
    }
}

// Type-safe layer definition
template<typename InputType, typename OutputType>
class Layer {
public:
    using Input = InputType;
    using Output = OutputType;
    
    static_assert(is_tensor_v<InputType>, "Input must be a Tensor");
    static_assert(is_tensor_v<OutputType>, "Output must be a Tensor");
    
    virtual OutputType forward(const InputType& input) const = 0;
};

// Linear (Fully Connected) Layer
template<typename T, std::size_t InSize, std::size_t OutSize>
class LinearLayer : public Layer<Tensor<T, InSize>, Tensor<T, OutSize>> {
private:
    Tensor<T, OutSize, InSize> weights_;
    Tensor<T, OutSize> bias_;
    
public:
    constexpr LinearLayer() : weights_{}, bias_{} {}
    
    constexpr LinearLayer(const Tensor<T, OutSize, InSize>& w, const Tensor<T, OutSize>& b)
        : weights_(w), bias_(b) {}
    
    constexpr Tensor<T, OutSize> forward(const Tensor<T, InSize>& input) const {
        // Compute: output = input * weights^T + bias
        // weights_ is [OutSize, InSize], so we compute: output[i] = sum(input[j] * weights_[i][j]) + bias[i]
        Tensor<T, OutSize> output;
        
        for (std::size_t i = 0; i < OutSize; ++i) {
            T sum = T(0);
            for (std::size_t j = 0; j < InSize; ++j) {
                sum += input(j) * weights_(i, j);
            }
            output(i) = sum + bias_(i);
        }
        
        return output;
    }
    
    constexpr auto& get_weights() { return weights_; }
    constexpr const auto& get_weights() const { return weights_; }
    constexpr auto& get_bias() { return bias_; }
    constexpr const auto& get_bias() const { return bias_; }
};

// Compile-time constant calculations
namespace constexpr_utils {
    // Calculate factorial at compile time
    constexpr std::size_t factorial(std::size_t n) {
        return (n == 0 || n == 1) ? 1 : n * factorial(n - 1);
    }
    
    // Calculate power at compile time
    template<std::size_t Base, std::size_t Exp>
    constexpr std::size_t power() {
        if constexpr (Exp == 0) return 1;
        else if constexpr (Exp == 1) return Base;
        else return Base * power<Base, Exp - 1>();
    }
    
    // Compile-time array sum
    template<std::size_t N>
    constexpr std::size_t array_sum(const std::array<std::size_t, N>& arr) {
        std::size_t sum = 0;
        for (std::size_t i = 0; i < N; ++i) {
            sum += arr[i];
        }
        return sum;
    }
}

