#pragma once

#include <array>
#include <cstddef>
#include <iostream>
#include <type_traits>

/**
 * @brief Tensor class using Template Metaprogramming
 * 
 * This demonstrates how T-MP can be used to create type-safe,
 * compile-time optimized tensor operations for NN compilation.
 */
template<typename T, std::size_t... Dims>
class Tensor {
public:
    // Calculate total size at compile time
    static constexpr std::size_t total_size = (Dims * ...);
    
    // Store shape information
    static constexpr std::array<std::size_t, sizeof...(Dims)> shape = {Dims...};
    static constexpr std::size_t rank = sizeof...(Dims);
    
private:
    std::array<T, total_size> data_;
    
public:
    // Default constructor
    constexpr Tensor() : data_{} {}
    
    // Constructor with initializer list
    constexpr Tensor(std::initializer_list<T> init) {
        std::size_t i = 0;
        for (const auto& val : init) {
            if (i < total_size) {
                data_[i++] = val;
            }
        }
    }
    
    // Access element using compile-time index calculation
    template<std::size_t... Indices>
    constexpr T& at() {
        static_assert(sizeof...(Indices) == rank, "Number of indices must match tensor rank");
        return data_[calculate_index<Indices...>()];
    }
    
    template<std::size_t... Indices>
    constexpr const T& at() const {
        static_assert(sizeof...(Indices) == rank, "Number of indices must match tensor rank");
        return data_[calculate_index<Indices...>()];
    }
    
    // Runtime access (for dynamic indexing)
    template<typename... Args>
    constexpr T& operator()(Args... indices) {
        return data_[calculate_index_runtime(indices...)];
    }
    
    template<typename... Args>
    constexpr const T& operator()(Args... indices) const {
        return data_[calculate_index_runtime(indices...)];
    }
    
    // Get raw data pointer
    constexpr T* data() { return data_.data(); }
    constexpr const T* data() const { return data_.data(); }
    
    // Get shape
    constexpr const auto& get_shape() const { return shape; }
    constexpr std::size_t size() const { return total_size; }
    
    // Print tensor (for debugging)
    void print() const {
        std::cout << "Tensor<" << typeid(T).name() << ">(";
        for (std::size_t i = 0; i < rank; ++i) {
            std::cout << shape[i];
            if (i < rank - 1) std::cout << ", ";
        }
        std::cout << ")\n";
    }
    
private:
    // Compile-time index calculation using fold expression
    template<std::size_t... Indices>
    static constexpr std::size_t calculate_index() {
        std::array<std::size_t, rank> indices_arr = {Indices...};
        std::size_t index = 0;
        std::size_t multiplier = 1;
        
        for (std::ptrdiff_t i = rank - 1; i >= 0; --i) {
            index += indices_arr[i] * multiplier;
            multiplier *= shape[i];
        }
        return index;
    }
    
    // Runtime index calculation
    template<typename... Args>
    constexpr std::size_t calculate_index_runtime(Args... args) const {
        std::array<std::size_t, rank> indices = {static_cast<std::size_t>(args)...};
        std::size_t index = 0;
        std::size_t multiplier = 1;
        
        for (std::ptrdiff_t i = rank - 1; i >= 0; --i) {
            index += indices[i] * multiplier;
            multiplier *= shape[i];
        }
        return index;
    }
};

// Type traits for tensor operations
template<typename T>
struct is_tensor : std::false_type {};

template<typename T, std::size_t... Dims>
struct is_tensor<Tensor<T, Dims...>> : std::true_type {};

template<typename T>
constexpr bool is_tensor_v = is_tensor<T>::value;

