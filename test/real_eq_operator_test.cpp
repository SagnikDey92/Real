#include <catch2/catch.hpp>
#include <real/real.hpp>
#include <test_helpers.hpp>

TEST_CASE("Operator ==") {

    SECTION("Subtraction == Addition") {
        
        boost::real::real a("-1");
        boost::real::real b("3");

        std::cout<<a<<"\n"<<b<<"\n"<<a/b;
    }

}