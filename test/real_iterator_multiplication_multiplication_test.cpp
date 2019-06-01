#include <catch2/catch.hpp>
#include <map>

#include <real/real.hpp>
#include <test_helpers.hpp>

TEST_CASE("Operators * *  boost::real::const_precision_iterator") {
    boost::real::interval<> expected_interval({});

    SECTION("With carry") {

        // Explicit numbers
        boost::real::real a("1.19");
        boost::real::real b("1.19");
        boost::real::real c("1.19");

        SECTION("(a * b) * c") {
            boost::real::real result = (a * b) * c;

            auto result_it = result.cbegin();

            // ([1, 2] * [1, 2]) * [1, 2]
            // [1, 4] * [1, 2]
            // [1, 8]
            expected_interval.lower_bound.positive = true;
            expected_interval.upper_bound.positive = true;
            expected_interval.lower_bound.exponent = 1;
            expected_interval.upper_bound.exponent = 1;
            expected_interval.lower_bound.digits = {1};
            expected_interval.upper_bound.digits = {8};
            CHECK(expected_interval == result_it.approximation_interval);

            // ([1.1, 1.2] * [1.1, 1.2]) * [1.1, 1.2]
            // [1.21, 1.44] * [1.1, 1.2]
            // [1.331, 1.728]
            ++result_it;
            expected_interval.lower_bound.digits = {1,3,3,1};
            expected_interval.upper_bound.digits = {1,7,2,8};
            CHECK(expected_interval == result_it.approximation_interval);

            // ([1.19, 1.19] * [1.19, 1.19]) * [1.19, 1.19]
            // [1.4161, 1.4161] * [1.19, 1.19]
            // [1.685159, 1.685159]
            ++result_it;
            expected_interval.lower_bound.digits = {1,6,8,5,1,5,9};
            expected_interval.upper_bound.digits = {1,6,8,5,1,5,9};
            CHECK(expected_interval == result_it.approximation_interval);

            // Full precision reached, no more changes are made
            ++result_it;
            CHECK(expected_interval == result_it.approximation_interval);
        }

        SECTION("a * (b * c)") {
            boost::real::real result = a * (b * c);

            auto result_it = result.cbegin();

            // [1, 2] * ([1, 2] * [1, 2])
            // [1, 2] * [1, 4]
            // [1, 8]
            expected_interval.lower_bound.positive = true;
            expected_interval.upper_bound.positive = true;
            expected_interval.lower_bound.exponent = 1;
            expected_interval.upper_bound.exponent = 1;
            expected_interval.lower_bound.digits = {1};
            expected_interval.upper_bound.digits = {8};
            CHECK(expected_interval == result_it.approximation_interval);

            // [1.1, 1.2] * ([1.1, 1.2] * [1.1, 1.2])
            // [1.1, 1.2] * [1.21, 1.44]
            // [1.331, 1.728]
            ++result_it;
            expected_interval.lower_bound.digits = {1,3,3,1};
            expected_interval.upper_bound.digits = {1,7,2,8};
            CHECK(expected_interval == result_it.approximation_interval);

            // [1.19, 1.19] * ([1.19, 1.19] * [1.19, 1.19])
            // [1.19, 1.19] * [1.4161, 1.4161]
            // [1.685159, 1.685159]
            ++result_it;
            expected_interval.lower_bound.digits = {1,6,8,5,1,5,9};
            expected_interval.upper_bound.digits = {1,6,8,5,1,5,9};
            CHECK(expected_interval == result_it.approximation_interval);

            // Full precision reached, no more changes are made
            ++result_it;
            CHECK(expected_interval == result_it.approximation_interval);
        }
    }

    SECTION("Without carry") {

        // Explicit numbers
        boost::real::real a("1.11");
        boost::real::real b("1.11");
        boost::real::real c("1.11");

        SECTION("(a * b) * c") {
            boost::real::real result = (a * b) * c;

            auto result_it = result.cbegin();

            // ([1, 2] * [1, 2]) * [1, 2]
            // [1, 4] * [1, 2]
            // [1, 8]
            expected_interval.lower_bound.positive = true;
            expected_interval.upper_bound.positive = true;
            expected_interval.lower_bound.exponent = 1;
            expected_interval.upper_bound.exponent = 1;
            expected_interval.lower_bound.digits = {1};
            expected_interval.upper_bound.digits = {8};
            CHECK(expected_interval == result_it.approximation_interval);

            // ([1.1, 1.2] * [1.1, 1.2]) * [1.1, 1.2]
            // [1.21, 1.44] * [1.1, 1.2]
            // [1.331, 1.728]
            ++result_it;
            expected_interval.lower_bound.digits = {1,3,3,1};
            expected_interval.upper_bound.digits = {1,7,2,8};
            CHECK(expected_interval == result_it.approximation_interval);

            // ([1.11, 1.11] * [1.11, 1.11]) * [1.11, 1.11]
            // [1.2321, 1.2321] * [1.11, 1.11]
            // [1.367631, 1.367631]
            ++result_it;
            expected_interval.lower_bound.digits = {1,3,6,7,6,3,1};
            expected_interval.upper_bound.digits = {1,3,6,7,6,3,1};
            CHECK(expected_interval == result_it.approximation_interval);

            // Full precision reached, no more changes are made
            ++result_it;
            CHECK(expected_interval == result_it.approximation_interval);
        }

        SECTION("a * (b * c)") {
            boost::real::real result = a * (b * c);

            auto result_it = result.cbegin();

            // [1, 2] * ([1, 2] * [1, 2])
            // [1, 2] * [1, 4]
            // [1, 8]
            expected_interval.lower_bound.positive = true;
            expected_interval.upper_bound.positive = true;
            expected_interval.lower_bound.exponent = 1;
            expected_interval.upper_bound.exponent = 1;
            expected_interval.lower_bound.digits = {1};
            expected_interval.upper_bound.digits = {8};
            CHECK(expected_interval == result_it.approximation_interval);

            // [1.1, 1.2] * ([1.1, 1.2] * [1.1, 1.2])
            // [1.1, 1.2] * [1.21, 1.44]
            // [1.331, 1.728]
            ++result_it;
            expected_interval.lower_bound.digits = {1,3,3,1};
            expected_interval.upper_bound.digits = {1,7,2,8};
            CHECK(expected_interval == result_it.approximation_interval);

            // [1.11, 1.11] * ([1.11, 1.11] * [1.11, 1.11])
            // [1.11, 1.11] * [1.2321, 1.2321]
            // [1.367631, 1.367631]
            ++result_it;
            expected_interval.lower_bound.digits = {1,3,6,7,6,3,1};
            expected_interval.upper_bound.digits = {1,3,6,7,6,3,1};
            CHECK(expected_interval == result_it.approximation_interval);

            // Full precision reached, no more changes are made
            ++result_it;
            CHECK(expected_interval == result_it.approximation_interval);
        }
    }
}