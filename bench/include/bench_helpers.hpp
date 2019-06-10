#ifndef BOOST_REAL_BENCH_HELPERS_HPP
#define BOOST_REAL_BENCH_HELPERS_HPP


#include <real/real.hpp>

template<> std::optional<unsigned int> boost::real::real<>::maximum_precision = 10;
template<> std::optional<unsigned int> boost::real::real_algorithm<>::maximum_precision = 10;

#endif //BOOST_REAL_BENCH_HELPERS_HPP
