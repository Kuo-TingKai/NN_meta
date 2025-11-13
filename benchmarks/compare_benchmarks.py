#!/usr/bin/env python3
"""
Compare benchmark results from C++ and PyTorch implementations
This script parses output from both benchmarks and generates a comparison report
"""

import re
import subprocess
import sys
from typing import Dict, List, Tuple
from dataclasses import dataclass

@dataclass
class BenchmarkResult:
    operation: str
    framework: str
    mean_us: float
    median_us: float
    stddev_us: float
    iterations: int

def parse_cpp_output(output: str) -> List[BenchmarkResult]:
    """Parse C++ benchmark output"""
    results = []
    lines = output.split('\n')
    
    current_operation = None
    current_stats = {}
    
    for i, line in enumerate(lines):
        # Match operation name
        if 'Statistics:' in line:
            # Extract operation name from previous lines
            if i > 0:
                op_line = lines[i-1].strip()
                current_operation = op_line.replace(' Statistics:', '')
        
        # Match statistics
        if 'Mean:' in line:
            match = re.search(r'Mean:\s+([\d.]+)\s+μs', line)
            if match:
                current_stats['mean'] = float(match.group(1))
        elif 'Median:' in line:
            match = re.search(r'Median:\s+([\d.]+)\s+μs', line)
            if match:
                current_stats['median'] = float(match.group(1))
        elif 'StdDev:' in line:
            match = re.search(r'StdDev:\s+([\d.]+)\s+μs', line)
            if match:
                current_stats['stddev'] = float(match.group(1))
        elif 'Iterations:' in line:
            match = re.search(r'Iterations:\s+(\d+)', line)
            if match:
                current_stats['iterations'] = int(match.group(1))
                
                # Create result if we have all data
                if 'mean' in current_stats and 'median' in current_stats:
                    results.append(BenchmarkResult(
                        operation=current_operation or "Unknown",
                        framework="C++ (Meta)",
                        mean_us=current_stats['mean'],
                        median_us=current_stats['median'],
                        stddev_us=current_stats.get('stddev', 0.0),
                        iterations=current_stats.get('iterations', 0)
                    ))
                    current_stats = {}
    
    return results

def parse_pytorch_output(output: str) -> List[BenchmarkResult]:
    """Parse PyTorch benchmark output"""
    results = []
    lines = output.split('\n')
    
    current_operation = None
    current_stats = {}
    
    for i, line in enumerate(lines):
        if 'Statistics:' in line:
            if i > 0:
                op_line = lines[i-1].strip()
                current_operation = op_line.replace(' Statistics:', '')
        
        if 'Mean:' in line:
            match = re.search(r'Mean:\s+([\d.]+)\s+μs', line)
            if match:
                current_stats['mean'] = float(match.group(1))
        elif 'Median:' in line:
            match = re.search(r'Median:\s+([\d.]+)\s+μs', line)
            if match:
                current_stats['median'] = float(match.group(1))
        elif 'StdDev:' in line:
            match = re.search(r'StdDev:\s+([\d.]+)\s+μs', line)
            if match:
                current_stats['stddev'] = float(match.group(1))
        elif 'Iterations:' in line:
            match = re.search(r'Iterations:\s+(\d+)', line)
            if match:
                current_stats['iterations'] = int(match.group(1))
                
                if 'mean' in current_stats and 'median' in current_stats:
                    results.append(BenchmarkResult(
                        operation=current_operation or "Unknown",
                        framework="PyTorch",
                        mean_us=current_stats['mean'],
                        median_us=current_stats['median'],
                        stddev_us=current_stats.get('stddev', 0.0),
                        iterations=current_stats.get('iterations', 0)
                    ))
                    current_stats = {}
    
    return results

def generate_comparison_report(cpp_results: List[BenchmarkResult], 
                                pytorch_results: List[BenchmarkResult]):
    """Generate a comparison report"""
    print("\n" + "=" * 80)
    print("BENCHMARK COMPARISON REPORT")
    print("=" * 80 + "\n")
    
    # Group results by operation
    cpp_by_op = {}
    pytorch_by_op = {}
    
    for result in cpp_results:
        if result.operation not in cpp_by_op:
            cpp_by_op[result.operation] = []
        cpp_by_op[result.operation].append(result)
    
    for result in pytorch_results:
        if result.operation not in pytorch_by_op:
            pytorch_by_op[result.operation] = []
        pytorch_by_op[result.operation].append(result)
    
    # Print comparison table
    print(f"{'Operation':<30} {'Framework':<20} {'Mean (μs)':<15} {'Median (μs)':<15} {'StdDev':<10}")
    print("-" * 90)
    
    all_operations = set(cpp_by_op.keys()) | set(pytorch_by_op.keys())
    
    for op in sorted(all_operations):
        if op in cpp_by_op:
            for result in cpp_by_op[op]:
                print(f"{result.operation:<30} {result.framework:<20} "
                      f"{result.mean_us:<15.3f} {result.median_us:<15.3f} "
                      f"{result.stddev_us:<10.3f}")
        
        if op in pytorch_by_op:
            for result in pytorch_by_op[op]:
                print(f"{result.operation:<30} {result.framework:<20} "
                      f"{result.mean_us:<15.3f} {result.median_us:<15.3f} "
                      f"{result.stddev_us:<10.3f}")
        print()
    
    # Calculate speedup
    print("\n" + "=" * 80)
    print("SPEEDUP ANALYSIS")
    print("=" * 80 + "\n")
    
    for op in sorted(all_operations):
        if op in cpp_by_op and op in pytorch_by_op:
            cpp_result = cpp_by_op[op][0]  # Take first match
            pytorch_result = pytorch_by_op[op][0]
            
            speedup = pytorch_result.mean_us / cpp_result.mean_us
            
            print(f"{op}:")
            print(f"  C++ (Meta):  {cpp_result.mean_us:.3f} μs")
            print(f"  PyTorch:     {pytorch_result.mean_us:.3f} μs")
            print(f"  Speedup:     {speedup:.2f}x {'faster' if speedup > 1.0 else 'slower'}")
            print()

def main():
    print("Running C++ benchmark...")
    try:
        cpp_output = subprocess.check_output(
            ['./build/benchmark_cpp'],
            cwd='..',
            stderr=subprocess.STDOUT,
            text=True
        )
        print("C++ benchmark completed.")
    except subprocess.CalledProcessError as e:
        print(f"Error running C++ benchmark: {e}")
        print(e.output)
        sys.exit(1)
    except FileNotFoundError:
        print("Error: C++ benchmark executable not found. Please build it first.")
        print("Run: cd build && cmake --build . --target benchmark_cpp")
        sys.exit(1)
    
    print("\nRunning PyTorch benchmark...")
    try:
        pytorch_output = subprocess.check_output(
            ['python3', 'benchmark_pytorch.py'],
            cwd='benchmarks',
            stderr=subprocess.STDOUT,
            text=True
        )
        print("PyTorch benchmark completed.")
    except subprocess.CalledProcessError as e:
        print(f"Error running PyTorch benchmark: {e}")
        print(e.output)
        sys.exit(1)
    except FileNotFoundError:
        print("Error: PyTorch benchmark script not found.")
        sys.exit(1)
    
    # Parse results
    cpp_results = parse_cpp_output(cpp_output)
    pytorch_results = parse_pytorch_output(pytorch_output)
    
    # Generate report
    generate_comparison_report(cpp_results, pytorch_results)

if __name__ == "__main__":
    main()

