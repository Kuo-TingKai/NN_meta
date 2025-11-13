#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include "tensor.hpp"
#include "nn_compiler.hpp"
#include "benchmark.hpp"

using namespace std;

// Helper to initialize tensor with random values
template<typename T, std::size_t... Dims>
void random_init(Tensor<T, Dims...>& tensor, T min_val = T(0), T max_val = T(1)) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<T> dis(min_val, max_val);
    
    for (std::size_t i = 0; i < tensor.size(); ++i) {
        tensor.data()[i] = dis(gen);
    }
}

// Benchmark: Matrix Multiplication
void benchmark_matmul() {
    cout << "\n=== Matrix Multiplication Benchmark ===\n";
    
    constexpr int iterations = 1000;
    constexpr int warmup = 100;
    
    // Small matrices (4x4)
    {
        Tensor<float, 4, 4> a, b;
        random_init(a, -1.0f, 1.0f);
        random_init(b, -1.0f, 1.0f);
        
        BenchmarkStats stats("MatMul (4x4) - C++ (Meta)");
        volatile float sum = 0.0f; // Prevent optimization
        stats.run_benchmark([&]() {
            auto result = matmul(a, b);
            sum += result(0, 0); // Use result to prevent optimization
        }, iterations, warmup);
        (void)sum; // Suppress unused warning
        
        stats.print_stats();
    }
    
    // Medium matrices (32x32)
    {
        Tensor<float, 32, 32> a, b;
        random_init(a, -1.0f, 1.0f);
        random_init(b, -1.0f, 1.0f);
        
        BenchmarkStats stats("MatMul (32x32) - C++ (Meta)");
        volatile float sum = 0.0f;
        stats.run_benchmark([&]() {
            auto result = matmul(a, b);
            sum += result(0, 0);
        }, iterations, warmup);
        (void)sum;
        
        stats.print_stats();
    }
    
    // Large matrices (128x128)
    {
        Tensor<float, 128, 128> a, b;
        random_init(a, -1.0f, 1.0f);
        random_init(b, -1.0f, 1.0f);
        
        BenchmarkStats stats("MatMul (128x128) - C++ (Meta)");
        volatile float sum = 0.0f;
        stats.run_benchmark([&]() {
            auto result = matmul(a, b);
            sum += result(0, 0);
        }, iterations / 10, warmup / 10);
        (void)sum;
        
        stats.print_stats();
    }
}

// Benchmark: ReLU Activation
void benchmark_relu() {
    cout << "\n=== ReLU Activation Benchmark ===\n";
    
    constexpr int iterations = 10000;
    constexpr int warmup = 1000;
    
    // Small tensor
    {
        Tensor<float, 16> input;
        random_init(input, -2.0f, 2.0f);
        
        BenchmarkStats stats("ReLU (16) - C++ (Meta)");
        volatile float sum = 0.0f;
        stats.run_benchmark([&]() {
            auto result = relu(input);
            sum += result(0);
        }, iterations, warmup);
        (void)sum;
        
        stats.print_stats();
    }
    
    // Medium tensor
    {
        Tensor<float, 1024> input;
        random_init(input, -2.0f, 2.0f);
        
        BenchmarkStats stats("ReLU (1024) - C++ (Meta)");
        volatile float sum = 0.0f;
        stats.run_benchmark([&]() {
            auto result = relu(input);
            sum += result(0);
        }, iterations, warmup);
        (void)sum;
        
        stats.print_stats();
    }
    
    // Large tensor
    {
        Tensor<float, 4096> input;
        random_init(input, -2.0f, 2.0f);
        
        BenchmarkStats stats("ReLU (4096) - C++ (Meta)");
        volatile float sum = 0.0f;
        stats.run_benchmark([&]() {
            auto result = relu(input);
            sum += result(0);
        }, iterations / 10, warmup / 10);
        (void)sum;
        
        stats.print_stats();
    }
}

// Benchmark: Linear Layer Forward Pass
void benchmark_linear_layer() {
    cout << "\n=== Linear Layer Forward Pass Benchmark ===\n";
    
    constexpr int iterations = 1000;
    constexpr int warmup = 100;
    
    // Small layer (64 -> 32)
    {
        LinearLayer<float, 64, 32> layer;
        random_init(layer.get_weights(), -0.1f, 0.1f);
        random_init(layer.get_bias(), -0.01f, 0.01f);
        
        Tensor<float, 64> input;
        random_init(input, -1.0f, 1.0f);
        
        BenchmarkStats stats("Linear (64->32) - C++ (Meta)");
        volatile float sum = 0.0f;
        stats.run_benchmark([&]() {
            auto result = layer.forward(input);
            sum += result(0);
        }, iterations, warmup);
        (void)sum;
        
        stats.print_stats();
    }
    
    // Medium layer (256 -> 128)
    {
        LinearLayer<float, 256, 128> layer;
        random_init(layer.get_weights(), -0.1f, 0.1f);
        random_init(layer.get_bias(), -0.01f, 0.01f);
        
        Tensor<float, 256> input;
        random_init(input, -1.0f, 1.0f);
        
        BenchmarkStats stats("Linear (256->128) - C++ (Meta)");
        volatile float sum = 0.0f;
        stats.run_benchmark([&]() {
            auto result = layer.forward(input);
            sum += result(0);
        }, iterations, warmup);
        (void)sum;
        
        stats.print_stats();
    }
    
    // Large layer (1024 -> 512)
    {
        LinearLayer<float, 1024, 512> layer;
        random_init(layer.get_weights(), -0.1f, 0.1f);
        random_init(layer.get_bias(), -0.01f, 0.01f);
        
        Tensor<float, 1024> input;
        random_init(input, -1.0f, 1.0f);
        
        BenchmarkStats stats("Linear (1024->512) - C++ (Meta)");
        volatile float sum = 0.0f;
        stats.run_benchmark([&]() {
            auto result = layer.forward(input);
            sum += result(0);
        }, iterations / 10, warmup / 10);
        (void)sum;
        
        stats.print_stats();
    }
}

// Benchmark: Element-wise Operations
void benchmark_elementwise() {
    cout << "\n=== Element-wise Operations Benchmark ===\n";
    
    constexpr int iterations = 10000;
    constexpr int warmup = 1000;
    
    // Small tensors
    {
        Tensor<float, 16> a, b;
        random_init(a, -1.0f, 1.0f);
        random_init(b, -1.0f, 1.0f);
        Tensor<float, 16> result;
        
        BenchmarkStats stats("Add (16) - C++ (Meta)");
        volatile float sum = 0.0f;
        stats.run_benchmark([&]() {
            for (std::size_t i = 0; i < 16; ++i) {
                result.data()[i] = a.data()[i] + b.data()[i];
            }
            sum += result.data()[0];
        }, iterations, warmup);
        (void)sum;
        
        stats.print_stats();
    }
    
    // Medium tensors
    {
        Tensor<float, 1024> a, b;
        random_init(a, -1.0f, 1.0f);
        random_init(b, -1.0f, 1.0f);
        Tensor<float, 1024> result;
        
        BenchmarkStats stats("Add (1024) - C++ (Meta)");
        volatile float sum = 0.0f;
        stats.run_benchmark([&]() {
            for (std::size_t i = 0; i < 1024; ++i) {
                result.data()[i] = a.data()[i] + b.data()[i];
            }
            sum += result.data()[0];
        }, iterations, warmup);
        (void)sum;
        
        stats.print_stats();
    }
}

int main() {
    cout << "========================================\n";
    cout << "C++ Metaprogramming Benchmark Suite\n";
    cout << "========================================\n";
    
    benchmark_matmul();
    benchmark_relu();
    benchmark_linear_layer();
    benchmark_elementwise();
    
    cout << "\n========================================\n";
    cout << "Benchmark Complete!\n";
    cout << "========================================\n";
    
    return 0;
}

