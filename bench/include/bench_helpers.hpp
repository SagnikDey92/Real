#ifndef BOOST_REAL_BENCH_HELPERS_HPP
#define BOOST_REAL_BENCH_HELPERS_HPP


#include <real/real.hpp>

template<> unsigned int boost::real::real<int>::maximum_precision = 10;
template<> unsigned int boost::real::real_algorithm<int>::maximum_precision = 10;

#endif //BOOST_REAL_BENCH_HELPERS_HPP
