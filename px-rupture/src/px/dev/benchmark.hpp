// name: benchmark.hpp
// auth: is0urce
// type: c++
// desc: time measurement

#pragma once

#include <chrono>

namespace px {

    ///
    /// measure function execution time
    /// @param [in,out] args function arguments
    /// @return time in microseconds
    ///
    template <unsigned int N = 1, typename F, typename ... Args>
    auto benchmark(F func, Args &&... args) {
        auto t0 = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i != N; ++i) {
            func(args...);
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
    }

    ///
    /// measure function execution time
    /// this is most ugly templated variant
    /// @param [in,out] args function arguments
    /// @return time in microseconds
    ///
    template <unsigned int N = 1, typename FType, FType F, typename ... Args>
    auto benchmark_fn(Args &&... args) {
        auto t0 = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i != N; ++i) {
            F(args...);
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
    }
}