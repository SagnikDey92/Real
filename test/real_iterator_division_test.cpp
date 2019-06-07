#include <catch2/catch.hpp>

#include <real/real.hpp>
#include <test_helpers.hpp>

TEST_CASE("Operator / boost::real::const_precision_iterator") {
        boost::real::real a("144"); // [100, 200]
        boost::real::real b("12");  // [10 , 20]
        // expect [5, 20]
        boost::real::real c(".5"); 

        // SECTION("a/b") {
        //     boost::real::real result = a/b;

        //     auto result_it = result.cbegin();

        //     boost::real::interval expected_interval({});
        //     expected_interval.lower_bound.positive = true;
        //     expected_interval.upper_bound.positive = true;
        //     expected_interval.lower_bound.exponent = 1;
        //     expected_interval.upper_bound.exponent = 2;
        //     expected_interval.lower_bound.digits = {5};
        //     expected_interval.upper_bound.digits = {2};
        //     CHECK(expected_interval == result_it.approximation_interval);
        // }

         SECTION("b/a") {
             boost::real::real result = b/a; 
             // 12 / 144 
             // [10, 20] / [100, 200]
             // --> [10/200, 20/100] = [.05, .2]

             auto result_it = result.cbegin();

             boost::real::interval expected_interval({});
             expected_interval.lower_bound.positive = true;
             expected_interval.upper_bound.positive = true;
             expected_interval.lower_bound.exponent = -1;
             expected_interval.upper_bound.exponent = 0;
             expected_interval.lower_bound.digits = {5};
             expected_interval.upper_bound.digits = {2};
             CHECK(expected_interval == result_it.approximation_interval);
        }
}
