#include <benchmark/benchmark.h>
#include <real/real.hpp>

// ensure this is >= to MAX_NUM_DIGITS_XX for all benchmarks, else we will get
// a precision error and the benchmarks will not be meaningful.
std::optional<size_t> boost::real::const_precision_iterator<int>::maximum_precision = 10;

BENCHMARK_MAIN();
