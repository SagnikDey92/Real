#include <catch2/catch.hpp>
#include <real/real.hpp>
#include <test_helpers.hpp>

TEST_CASE("Operator ==") {
    SECTION("Checking special division cases") {
        SECTION("Divide by zero") {
            auto a = "00.00e-3"_r;
            auto b = 123.45_r;

            CHECK_THROWS_AS(b/a == 42_r, boost::real::divide_by_zero_exception);
        }
        SECTION("Divide by one") {
            auto a = "12.34"_r;
            auto b = 1_r;

            CHECK(a/b == a);
        }
        SECTION("Dividing zero") {
            auto a = 0_r;
            auto b = 1.34_r;

            CHECK(a/b == a);
        }
        SECTION("Testing decimal printing") {
            auto a = "990483.1766673839"_r;
            auto b = "2341.566"_r;
            
            std::cout<<a/b;
        }
    }
}