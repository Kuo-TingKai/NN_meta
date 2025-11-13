#pragma once

#include <chrono>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <functional>
#include <map>

/**
 * @brief Benchmark utilities for performance measurement
 */

class BenchmarkTimer {
private:
    std::chrono::high_resolution_clock::time_point start_time_;
    std::string name_;
    
public:
    BenchmarkTimer(const std::string& name) : name_(name) {
        start_time_ = std::chrono::high_resolution_clock::now();
    }
    
    ~BenchmarkTimer() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            end_time - start_time_).count();
        std::cout << "  " << name_ << ": " << duration << " μs\n";
    }
    
    double elapsed_microseconds() const {
        auto end_time = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(
            end_time - start_time_).count();
    }
    
    double elapsed_milliseconds() const {
        return elapsed_microseconds() / 1000.0;
    }
    
    double elapsed_seconds() const {
        return elapsed_microseconds() / 1000000.0;
    }
};

class BenchmarkStats {
private:
    std::vector<double> times_;
    std::string name_;
    
public:
    BenchmarkStats(const std::string& name) : name_(name) {}
    
    void add_time(double time_us) {
        times_.push_back(time_us);
    }
    
    void run_benchmark(std::function<void()> func, int iterations = 100, int warmup = 10) {
        times_.clear();
        times_.reserve(iterations);
        
        // Warmup
        for (int i = 0; i < warmup; ++i) {
            func();
        }
        
        // Actual benchmark
        for (int i = 0; i < iterations; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            func();
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                end - start).count();
            times_.push_back(duration);
        }
    }
    
    double mean() const {
        if (times_.empty()) return 0.0;
        return std::accumulate(times_.begin(), times_.end(), 0.0) / times_.size();
    }
    
    double median() const {
        if (times_.empty()) return 0.0;
        std::vector<double> sorted = times_;
        std::sort(sorted.begin(), sorted.end());
        size_t n = sorted.size();
        if (n % 2 == 0) {
            return (sorted[n/2 - 1] + sorted[n/2]) / 2.0;
        } else {
            return sorted[n/2];
        }
    }
    
    double stddev() const {
        if (times_.empty()) return 0.0;
        double m = mean();
        double sum_sq_diff = 0.0;
        for (double t : times_) {
            double diff = t - m;
            sum_sq_diff += diff * diff;
        }
        return std::sqrt(sum_sq_diff / times_.size());
    }
    
    double min() const {
        if (times_.empty()) return 0.0;
        return *std::min_element(times_.begin(), times_.end());
    }
    
    double max() const {
        if (times_.empty()) return 0.0;
        return *std::max_element(times_.begin(), times_.end());
    }
    
    void print_stats() const {
        std::cout << "\n" << name_ << " Statistics:\n";
        std::cout << "  Iterations: " << times_.size() << "\n";
        std::cout << "  Mean:   " << std::fixed << std::setprecision(3) 
                  << mean() << " μs\n";
        std::cout << "  Median: " << std::fixed << std::setprecision(3) 
                  << median() << " μs\n";
        std::cout << "  StdDev: " << std::fixed << std::setprecision(3) 
                  << stddev() << " μs\n";
        std::cout << "  Min:    " << std::fixed << std::setprecision(3) 
                  << min() << " μs\n";
        std::cout << "  Max:    " << std::fixed << std::setprecision(3) 
                  << max() << " μs\n";
    }
    
    double get_mean() const { return mean(); }
    double get_median() const { return median(); }
};

struct BenchmarkResult {
    std::string operation;
    std::string framework;
    double mean_time_us;
    double median_time_us;
    double stddev_us;
    int iterations;
    
    BenchmarkResult(const std::string& op, const std::string& fw, 
                   double mean, double median, double stddev, int iter)
        : operation(op), framework(fw), mean_time_us(mean), 
          median_time_us(median), stddev_us(stddev), iterations(iter) {}
};

void print_comparison_table(const std::vector<BenchmarkResult>& results) {
    std::cout << "\n" << std::string(80, '=') << "\n";
    std::cout << "BENCHMARK COMPARISON TABLE\n";
    std::cout << std::string(80, '=') << "\n\n";
    
    std::cout << std::left << std::setw(25) << "Operation"
              << std::setw(20) << "Framework"
              << std::setw(15) << "Mean (μs)"
              << std::setw(15) << "Median (μs)"
              << std::setw(10) << "StdDev"
              << "\n";
    std::cout << std::string(85, '-') << "\n";
    
    for (const auto& result : results) {
        std::cout << std::left << std::setw(25) << result.operation
                  << std::setw(20) << result.framework
                  << std::fixed << std::setprecision(3)
                  << std::setw(15) << result.mean_time_us
                  << std::setw(15) << result.median_time_us
                  << std::setw(10) << result.stddev_us
                  << "\n";
    }
    
    std::cout << "\n";
    
    // Calculate speedup
    std::map<std::string, std::vector<BenchmarkResult>> by_operation;
    for (const auto& result : results) {
        by_operation[result.operation].push_back(result);
    }
    
    std::cout << "SPEEDUP ANALYSIS:\n";
    std::cout << std::string(80, '-') << "\n";
    for (const auto& [op, op_results] : by_operation) {
        if (op_results.size() >= 2) {
            // Find C++ result
            auto cpp_it = std::find_if(op_results.begin(), op_results.end(),
                [](const BenchmarkResult& r) { return r.framework == "C++ (Meta)"; });
            
            if (cpp_it != op_results.end()) {
                double cpp_time = cpp_it->mean_time_us;
                std::cout << "\n" << op << ":\n";
                for (const auto& result : op_results) {
                    if (result.framework != "C++ (Meta)") {
                        double speedup = result.mean_time_us / cpp_time;
                        std::cout << "  " << result.framework << " vs C++ (Meta): "
                                  << std::fixed << std::setprecision(2) << speedup
                                  << "x " << (speedup > 1.0 ? "slower" : "faster") << "\n";
                    }
                }
            }
        }
    }
    std::cout << "\n";
}

