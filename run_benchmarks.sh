#!/bin/bash

# Script to run all benchmarks and generate comparison report

set -e

echo "=========================================="
echo "Running Benchmark Comparison Suite"
echo "=========================================="
echo ""

# Check if build directory exists
if [ ! -d "build" ]; then
    echo "Building project..."
    mkdir -p build
    cd build
    cmake ..
    cmake --build .
    cd ..
fi

# Build benchmark if needed
if [ ! -f "build/benchmark_cpp" ]; then
    echo "Building benchmark_cpp..."
    cd build
    cmake --build . --target benchmark_cpp
    cd ..
fi

# Make Python scripts executable
chmod +x benchmarks/benchmark_pytorch.py
chmod +x benchmarks/compare_benchmarks.py

echo "Running C++ benchmark..."
echo "----------------------------------------"
./build/benchmark_cpp

echo ""
echo "Running PyTorch benchmark..."
echo "----------------------------------------"
cd benchmarks
python3 benchmark_pytorch.py
cd ..

echo ""
echo "Generating comparison report..."
echo "----------------------------------------"
cd benchmarks
python3 compare_benchmarks.py
cd ..

echo ""
echo "=========================================="
echo "Benchmark comparison complete!"
echo "=========================================="

