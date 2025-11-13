#!/usr/bin/env python3
"""
PyTorch Benchmark for comparison with C++ Metaprogramming implementation
"""

import torch
import time
import statistics
import numpy as np
from typing import List, Tuple

def benchmark_matmul_pytorch():
    """Benchmark matrix multiplication in PyTorch"""
    print("\n=== Matrix Multiplication Benchmark (PyTorch) ===\n")
    
    iterations = 1000
    warmup = 100
    
    # Small matrices (4x4)
    a = torch.randn(4, 4, dtype=torch.float32)
    b = torch.randn(4, 4, dtype=torch.float32)
    
    # Warmup
    for _ in range(warmup):
        _ = torch.matmul(a, b)
    
    # Benchmark
    times = []
    for _ in range(iterations):
        start = time.perf_counter()
        result = torch.matmul(a, b)
        torch.cuda.synchronize() if torch.cuda.is_available() else None
        end = time.perf_counter()
        times.append((end - start) * 1e6)  # Convert to microseconds
    
    print_stats("MatMul (4x4) - PyTorch", times, iterations)
    
    # Medium matrices (32x32)
    a = torch.randn(32, 32, dtype=torch.float32)
    b = torch.randn(32, 32, dtype=torch.float32)
    
    for _ in range(warmup):
        _ = torch.matmul(a, b)
    
    times = []
    for _ in range(iterations):
        start = time.perf_counter()
        result = torch.matmul(a, b)
        torch.cuda.synchronize() if torch.cuda.is_available() else None
        end = time.perf_counter()
        times.append((end - start) * 1e6)
    
    print_stats("MatMul (32x32) - PyTorch", times, iterations)
    
    # Large matrices (128x128)
    a = torch.randn(128, 128, dtype=torch.float32)
    b = torch.randn(128, 128, dtype=torch.float32)
    
    for _ in range(warmup // 10):
        _ = torch.matmul(a, b)
    
    times = []
    for _ in range(iterations // 10):
        start = time.perf_counter()
        result = torch.matmul(a, b)
        torch.cuda.synchronize() if torch.cuda.is_available() else None
        end = time.perf_counter()
        times.append((end - start) * 1e6)
    
    print_stats("MatMul (128x128) - PyTorch", times, iterations // 10)


def benchmark_relu_pytorch():
    """Benchmark ReLU activation in PyTorch"""
    print("\n=== ReLU Activation Benchmark (PyTorch) ===\n")
    
    iterations = 10000
    warmup = 1000
    
    # Small tensor
    input_tensor = torch.randn(16, dtype=torch.float32)
    
    for _ in range(warmup):
        _ = torch.relu(input_tensor)
    
    times = []
    for _ in range(iterations):
        start = time.perf_counter()
        result = torch.relu(input_tensor)
        torch.cuda.synchronize() if torch.cuda.is_available() else None
        end = time.perf_counter()
        times.append((end - start) * 1e6)
    
    print_stats("ReLU (16) - PyTorch", times, iterations)
    
    # Medium tensor
    input_tensor = torch.randn(1024, dtype=torch.float32)
    
    for _ in range(warmup):
        _ = torch.relu(input_tensor)
    
    times = []
    for _ in range(iterations):
        start = time.perf_counter()
        result = torch.relu(input_tensor)
        torch.cuda.synchronize() if torch.cuda.is_available() else None
        end = time.perf_counter()
        times.append((end - start) * 1e6)
    
    print_stats("ReLU (1024) - PyTorch", times, iterations)
    
    # Large tensor
    input_tensor = torch.randn(4096, dtype=torch.float32)
    
    for _ in range(warmup // 10):
        _ = torch.relu(input_tensor)
    
    times = []
    for _ in range(iterations // 10):
        start = time.perf_counter()
        result = torch.relu(input_tensor)
        torch.cuda.synchronize() if torch.cuda.is_available() else None
        end = time.perf_counter()
        times.append((end - start) * 1e6)
    
    print_stats("ReLU (4096) - PyTorch", times, iterations // 10)


def benchmark_linear_layer_pytorch():
    """Benchmark linear layer forward pass in PyTorch"""
    print("\n=== Linear Layer Forward Pass Benchmark (PyTorch) ===\n")
    
    iterations = 1000
    warmup = 100
    
    # Small layer (64 -> 32)
    layer = torch.nn.Linear(64, 32, bias=True)
    input_tensor = torch.randn(64, dtype=torch.float32)
    
    for _ in range(warmup):
        _ = layer(input_tensor)
    
    times = []
    for _ in range(iterations):
        start = time.perf_counter()
        result = layer(input_tensor)
        torch.cuda.synchronize() if torch.cuda.is_available() else None
        end = time.perf_counter()
        times.append((end - start) * 1e6)
    
    print_stats("Linear (64->32) - PyTorch", times, iterations)
    
    # Medium layer (256 -> 128)
    layer = torch.nn.Linear(256, 128, bias=True)
    input_tensor = torch.randn(256, dtype=torch.float32)
    
    for _ in range(warmup):
        _ = layer(input_tensor)
    
    times = []
    for _ in range(iterations):
        start = time.perf_counter()
        result = layer(input_tensor)
        torch.cuda.synchronize() if torch.cuda.is_available() else None
        end = time.perf_counter()
        times.append((end - start) * 1e6)
    
    print_stats("Linear (256->128) - PyTorch", times, iterations)
    
    # Large layer (1024 -> 512)
    layer = torch.nn.Linear(1024, 512, bias=True)
    input_tensor = torch.randn(1024, dtype=torch.float32)
    
    for _ in range(warmup // 10):
        _ = layer(input_tensor)
    
    times = []
    for _ in range(iterations // 10):
        start = time.perf_counter()
        result = layer(input_tensor)
        torch.cuda.synchronize() if torch.cuda.is_available() else None
        end = time.perf_counter()
        times.append((end - start) * 1e6)
    
    print_stats("Linear (1024->512) - PyTorch", times, iterations // 10)


def benchmark_elementwise_pytorch():
    """Benchmark element-wise operations in PyTorch"""
    print("\n=== Element-wise Operations Benchmark (PyTorch) ===\n")
    
    iterations = 10000
    warmup = 1000
    
    # Small tensors
    a = torch.randn(16, dtype=torch.float32)
    b = torch.randn(16, dtype=torch.float32)
    
    for _ in range(warmup):
        _ = a + b
    
    times = []
    for _ in range(iterations):
        start = time.perf_counter()
        result = a + b
        torch.cuda.synchronize() if torch.cuda.is_available() else None
        end = time.perf_counter()
        times.append((end - start) * 1e6)
    
    print_stats("Add (16) - PyTorch", times, iterations)
    
    # Medium tensors
    a = torch.randn(1024, dtype=torch.float32)
    b = torch.randn(1024, dtype=torch.float32)
    
    for _ in range(warmup):
        _ = a + b
    
    times = []
    for _ in range(iterations):
        start = time.perf_counter()
        result = a + b
        torch.cuda.synchronize() if torch.cuda.is_available() else None
        end = time.perf_counter()
        times.append((end - start) * 1e6)
    
    print_stats("Add (1024) - PyTorch", times, iterations)


def print_stats(name: str, times: List[float], iterations: int):
    """Print benchmark statistics"""
    mean_time = statistics.mean(times)
    median_time = statistics.median(times)
    stddev_time = statistics.stdev(times) if len(times) > 1 else 0.0
    min_time = min(times)
    max_time = max(times)
    
    print(f"\n{name} Statistics:")
    print(f"  Iterations: {iterations}")
    print(f"  Mean:   {mean_time:.3f} μs")
    print(f"  Median: {median_time:.3f} μs")
    print(f"  StdDev: {stddev_time:.3f} μs")
    print(f"  Min:    {min_time:.3f} μs")
    print(f"  Max:    {max_time:.3f} μs")


def main():
    print("========================================")
    print("PyTorch Benchmark Suite")
    print("========================================")
    
    if torch.cuda.is_available():
        print(f"Using GPU: {torch.cuda.get_device_name(0)}")
    else:
        print("Using CPU")
    
    benchmark_matmul_pytorch()
    benchmark_relu_pytorch()
    benchmark_linear_layer_pytorch()
    benchmark_elementwise_pytorch()
    
    print("\n========================================")
    print("Benchmark Complete!")
    print("========================================")


if __name__ == "__main__":
    main()

