#include <iostream>
#include <iomanip>
#include "tensor.hpp"
#include "expression_template.hpp"
#include "nn_compiler.hpp"

using namespace std;

// Helper function to print tensor values
template<typename T, std::size_t... Dims>
void print_tensor_values(const Tensor<T, Dims...>& t, const string& name) {
    cout << name << " = [";
    for (std::size_t i = 0; i < t.size(); ++i) {
        cout << t.data()[i];
        if (i < t.size() - 1) cout << ", ";
    }
    cout << "]\n";
}

int main() {
    cout << "=== C++ Metaprogramming for NN Compilation Demo ===\n\n";
    
    // ============================================================
    // 1. Template Metaprogramming: Type-safe Tensor Operations
    // ============================================================
    cout << "1. Template Metaprogramming: Type-safe Tensors\n";
    cout << "-----------------------------------------------\n";
    
    // Create tensors with compile-time shape information
    Tensor<float, 2, 3> tensor_a{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
    Tensor<float, 3, 2> tensor_b{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
    
    tensor_a.print();
    tensor_b.print();
    
    cout << "\nTensor A shape: [";
    for (const auto& dim : tensor_a.get_shape()) {
        cout << dim << " ";
    }
    cout << "]\n";
    
    cout << "Tensor A total size: " << tensor_a.size() << "\n";
    cout << "Tensor A rank: " << tensor_a.rank << "\n\n";
    
    // Compile-time indexed access
    cout << "tensor_a(0, 0) = " << tensor_a(0, 0) << "\n";
    cout << "tensor_a(1, 2) = " << tensor_a(1, 2) << "\n\n";
    
    // ============================================================
    // 2. Expression Templates: Optimized Tensor Expressions
    // ============================================================
    cout << "2. Expression Templates: Optimized Operations\n";
    cout << "-----------------------------------------------\n";
    
    Tensor<float, 2, 2> a{1.0f, 2.0f, 3.0f, 4.0f};
    Tensor<float, 2, 2> b{5.0f, 6.0f, 7.0f, 8.0f};
    
    print_tensor_values(a, "A");
    print_tensor_values(b, "B");
    
    // Using expression templates (no temporary objects created)
    auto expr_a = expr(a);
    auto expr_b = expr(b);
    auto expr_result = expr_a + expr_b;
    
    // Evaluate expression
    Tensor<float, 2, 2> result;
    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 2; ++j) {
            result(i, j) = expr_result(i, j);
        }
    }
    
    print_tensor_values(result, "A + B");
    
    // Scalar multiplication (simplified for demo)
    Tensor<float, 2, 2> scaled;
    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 2; ++j) {
            scaled(i, j) = 2.0f * a(i, j);
        }
    }
    print_tensor_values(scaled, "2 * A");
    cout << "\n";
    
    // ============================================================
    // 3. Compile-time Kernel Generation: Matrix Multiplication
    // ============================================================
    cout << "3. Compile-time Kernel: Matrix Multiplication\n";
    cout << "-----------------------------------------------\n";
    
    Tensor<float, 2, 3> mat_a{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
    Tensor<float, 3, 2> mat_b{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
    
    print_tensor_values(mat_a, "Matrix A (2x3)");
    print_tensor_values(mat_b, "Matrix B (3x2)");
    
    // Compile-time optimized matrix multiplication
    auto mat_result = matmul(mat_a, mat_b);
    print_tensor_values(mat_result, "A @ B (2x2)");
    cout << "\n";
    
    // ============================================================
    // 4. Compile-time Optimized Activation Functions
    // ============================================================
    cout << "4. Compile-time Optimized: ReLU Activation\n";
    cout << "-----------------------------------------------\n";
    
    Tensor<float, 6> input{-2.0f, -1.0f, 0.0f, 1.0f, 2.0f, 3.0f};
    print_tensor_values(input, "Input");
    
    auto relu_output = relu(input);
    print_tensor_values(relu_output, "ReLU(Input)");
    cout << "\n";
    
    // ============================================================
    // 5. Type-safe Neural Network Layer
    // ============================================================
    cout << "5. Type-safe Neural Network Layer\n";
    cout << "-----------------------------------------------\n";
    
    // Define a linear layer: 3 inputs -> 2 outputs
    LinearLayer<float, 3, 2> linear_layer;
    
    // Set weights and bias (in real scenario, these would be learned)
    // Weights: 2x3 matrix
    linear_layer.get_weights()(0, 0) = 0.1f; linear_layer.get_weights()(0, 1) = 0.2f; 
    linear_layer.get_weights()(0, 2) = 0.3f;
    linear_layer.get_weights()(1, 0) = 0.4f; linear_layer.get_weights()(1, 1) = 0.5f; 
    linear_layer.get_weights()(1, 2) = 0.6f;
    
    // Bias: 2 elements
    linear_layer.get_bias()(0) = 0.1f;
    linear_layer.get_bias()(1) = 0.2f;
    
    // Forward pass
    Tensor<float, 3> layer_input{1.0f, 2.0f, 3.0f};
    auto layer_output = linear_layer.forward(layer_input);
    
    print_tensor_values(layer_input, "Layer Input (3)");
    print_tensor_values(layer_output, "Layer Output (2)");
    cout << "\n";
    
    // ============================================================
    // 6. Compile-time Shape Validation
    // ============================================================
    cout << "6. Compile-time Shape Validation\n";
    cout << "-----------------------------------------------\n";
    
    Tensor<float, 2, 3> shape_a;
    Tensor<float, 2, 3> shape_b;
    Tensor<float, 3, 2> shape_c;
    
    cout << "Shape A matches Shape B: " 
         << (shapes_match(shape_a, shape_b) ? "true" : "false") << "\n";
    cout << "Shape A matches Shape C: " 
         << (shapes_match(shape_a, shape_c) ? "true" : "false") << "\n";
    cout << "\n";
    
    // ============================================================
    // 7. Constexpr Compile-time Calculations
    // ============================================================
    cout << "7. Constexpr: Compile-time Calculations\n";
    cout << "-----------------------------------------------\n";
    
    constexpr std::size_t fact_5 = constexpr_utils::factorial(5);
    constexpr std::size_t pow_2_8 = constexpr_utils::power<2, 8>();
    
    cout << "factorial(5) = " << fact_5 << " (computed at compile time)\n";
    cout << "2^8 = " << pow_2_8 << " (computed at compile time)\n";
    
    constexpr std::array<std::size_t, 3> arr{2, 3, 4};
    constexpr std::size_t sum = constexpr_utils::array_sum(arr);
    cout << "sum([2, 3, 4]) = " << sum << " (computed at compile time)\n";
    cout << "\n";
    
    // ============================================================
    // Summary
    // ============================================================
    cout << "=== Summary ===\n";
    cout << "This demo showcases:\n";
    cout << "1. Template Metaprogramming for type-safe tensor operations\n";
    cout << "2. Expression Templates for zero-overhead abstractions\n";
    cout << "3. Compile-time kernel generation (matrix multiplication)\n";
    cout << "4. Constexpr for compile-time calculations\n";
    cout << "5. Type-safe neural network layer definitions\n";
    cout << "6. Compile-time shape validation\n";
    cout << "\n";
    cout << "These techniques are fundamental for building efficient\n";
    cout << "NN compilers and deep learning frameworks in C++.\n";
    
    return 0;
}

