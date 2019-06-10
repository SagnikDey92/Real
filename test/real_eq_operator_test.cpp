#include <catch2/catch.hpp>
#include <real/real.hpp>
#include <test_helpers.hpp>

TEST_CASE("Operator ==") {

    SECTION("a == b") {

        SECTION("without precision exception") {
            SECTION("Explicit == Explicit") {
                boost::real::real a("15");
                boost::real::real b("15");
                
                CHECK(a == b);
            }

            SECTION("Explicit == Addition") {
                boost::real::real a("10");
                boost::real::real b("05");
                boost::real::real c("05");
                boost::real::real d = b + c;

                CHECK(a == d);
            }

            SECTION("Explicit == Subtraction") {
                boost::real::real a("20");
                boost::real::real b("15");
                boost::real::real c("-05");
                boost::real::real d = b - c;

                CHECK(a == d);
            }

            SECTION("Explicit == multiplication") {
                boost::real::real a("4");
                boost::real::real b("2");
                boost::real::real c("2");
                boost::real::real d = b * c;

                CHECK(a == d);
            }

            SECTION("Addition == Explicit") {
                boost::real::real a("15");
                boost::real::real b("15");
                boost::real::real c = a + b;
                boost::real::real d("30");

                CHECK(c == d);
            }

            SECTION("Addition == Addition") {
                boost::real::real a("15");
                boost::real::real b("15");
                boost::real::real c = a + b;
                boost::real::real d = a + b;

                CHECK(c == d);
            }

            SECTION("Addition == Subtraction") {
                boost::real::real a("15");
                boost::real::real b("15");
                boost::real::real c = a + b;
                boost::real::real d("35");
                boost::real::real e("05");
                boost::real::real f = d - e;

                CHECK(c == f);
            }

            SECTION("Addition == multiplication") {
                boost::real::real a("15");
                boost::real::real b("15");
                boost::real::real c = a + b;
                boost::real::real d("15");
                boost::real::real e("2");
                boost::real::real f = d * e;

                CHECK(c == f);
            }

            SECTION("Subtraction == Explicit") {
                boost::real::real a("15");
                boost::real::real b("15");
                boost::real::real c = a - b;
                boost::real::real d("0");

                CHECK(c == d);
            }

            SECTION("Subtraction == Addition") {
                boost::real::real a("15");
                boost::real::real b("15");
                boost::real::real c = a - b;
                boost::real::real d("-15");
                boost::real::real e = a + d;

                CHECK(c == e);
            }

            SECTION("Subtraction == Subtraction") {
                boost::real::real a("15");
                boost::real::real b("15");
                boost::real::real c = a - b;
                boost::real::real d = a - b;

                CHECK(c == d);
            }

            SECTION("Subtraction == multiplication") {
                boost::real::real a("15");
                boost::real::real b("15");
                boost::real::real c = a - b;
                boost::real::real d("0");
                boost::real::real e = a * d;

                CHECK(c == e);
            }

            SECTION("multiplication == Explicit") {
                boost::real::real a("15");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d("30");

                CHECK(c == d);
            }

            SECTION("multiplication == Addition") {
                boost::real::real a("15");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d = a + a;

                CHECK(c == d);
            }

            SECTION("multiplication == Subtraction") {
                boost::real::real a("15");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d("-28");
                boost::real::real e = b - d;

                CHECK(c == e);
            }

            SECTION("multiplication == multiplication") {
                boost::real::real a("15");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d = a * b;

                CHECK(c == d);
            }
        }




        SECTION("With precision exception") {
            /*
            SECTION("Explicit == Algorithm") {
                boost::real::real a("111");
                boost::real::real b(one_one_one, 1);

                CHECK(a == b);
            }
            */
            SECTION("Explicit == Explicit") {
                boost::real::real a("1555555555555555555");
                boost::real::real b("1555555555555555555");

                CHECK(a == b);
            }

            SECTION("Explicit == Addition") {
                boost::real::real a("22222222222220000000000");
                boost::real::real b("11111111111110000000000");
                boost::real::real c("11111111111110000000000");
                boost::real::real d = b + c;

                CHECK(a == d);
            }

            SECTION("Explicit == Subtraction") {
                boost::real::real a("22222222222222222222");
                boost::real::real b("22222222222222222223");
                boost::real::real c("1");
                boost::real::real d = b - c;

                CHECK(a == d);
            }

            SECTION("Explicit == multiplication") {
                boost::real::real a("22222222222220000000000");
                boost::real::real b("11111111111110000000000");
                boost::real::real c("2");
                boost::real::real d = b * c;

                CHECK(a == d);
            }

            SECTION("Addition == Explicit") {
                boost::real::real a("11111111111111");
                boost::real::real b("11111111111111");
                boost::real::real c = a + b;
                boost::real::real d("22222222222222");

                CHECK(c == d);
            }

            SECTION("Addition == Addition") {
                boost::real::real a("11111111111111");
                boost::real::real b("11111111111111");
                boost::real::real c = a + b;
                boost::real::real d = a + b;

                CHECK(c == d);
            }

            SECTION("Addition == Subtraction") {
                boost::real::real a("11111111111111");
                boost::real::real b("11111111111111");
                boost::real::real c = a + b;
                boost::real::real d("22222222222223");
                boost::real::real e("00000000000001");
                boost::real::real f = d - e;

                CHECK(c == f);
            }

            SECTION("Addition == multiplication") {
                boost::real::real a("11111111111111");
                boost::real::real b("11111111111111");
                boost::real::real c = a + b;
                boost::real::real d("2");
                boost::real::real e = a * d;

                CHECK(c == e);
            }

            SECTION("Subtraction == Explicit") {
                boost::real::real a("11111111111112");
                boost::real::real b("00000000000001");
                boost::real::real c = a - b;
                boost::real::real d("11111111111111");

                CHECK(c == d);
            }

            SECTION("Subtraction == Addition") {
                boost::real::real a("11111111111112");
                boost::real::real b("00000000000001");
                boost::real::real c = a - b;
                boost::real::real d("-00000000000001");
                boost::real::real e = a + d;

                CHECK(c == e);
            }

            SECTION("Subtraction == Subtraction") {
                boost::real::real a("11111111111112");
                boost::real::real b("00000000000001");
                boost::real::real c = a - b;
                boost::real::real d = a - b;

                CHECK(c == d);
            }

            SECTION("Subtraction == multiplication") {
                boost::real::real a("11111111111112");
                boost::real::real b("00000000000001");
                boost::real::real c = a - b;
                boost::real::real d("11111111111111");
                boost::real::real e("1");
                boost::real::real f = d * e;

                CHECK(c == f);
            }

            SECTION("multiplication == Explicit") {
                boost::real::real a("11111111111111");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d("22222222222222");

                CHECK(c == d);
            }

            SECTION("multiplication == Addition") {
                boost::real::real a("11111111111111");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d = a + a;

                CHECK(c == d);
            }

            SECTION("multiplication == Subtraction") {
                boost::real::real a("11111111111111");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d("22222222222223");
                boost::real::real e("00000000000001");
                boost::real::real f = d - e;

                CHECK(c == f);
            }

            SECTION("multiplication == multiplication") {
                boost::real::real a("11111111111111");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d = a * b;

                CHECK(c == d);
            }
        }
    }



    SECTION("a < b") {

        SECTION("without precision exception") {
            SECTION("Explicit == Explicit") {
                boost::real::real a("1");
                boost::real::real b("15");

                CHECK_FALSE(a == b);
            }

            SECTION("Explicit == Addition") {
                boost::real::real a("1");
                boost::real::real b("05");
                boost::real::real c("1");
                boost::real::real d = b + c;

                CHECK_FALSE(a == d);
            }

            SECTION("Explicit == Subtraction") {
                boost::real::real a("1");
                boost::real::real b("15");
                boost::real::real c("02");
                boost::real::real d = b - c;

                CHECK_FALSE(a == d);
            }

            SECTION("Explicit == multiplication") {
                boost::real::real a("3");
                boost::real::real b("2");
                boost::real::real c("2");
                boost::real::real d = b * c;

                CHECK_FALSE(a == d);
            }

            SECTION("Addition == Explicit") {
                boost::real::real a("15");
                boost::real::real b("15");
                boost::real::real c = a + b;
                boost::real::real d("4");

                CHECK_FALSE(c == d);
            }

            SECTION("Addition == Addition") {
                boost::real::real a("15");
                boost::real::real b("1");
                boost::real::real c = a + b;
                boost::real::real d = a + a;

                CHECK_FALSE(c == d);
            }

            SECTION("Addition == Subtraction") {
                boost::real::real a("15");
                boost::real::real b("1");
                boost::real::real c = a + b;
                boost::real::real d("35");
                boost::real::real e("05");
                boost::real::real f = d - e;

                CHECK_FALSE(c == f);
            }

            SECTION("Addition == multiplication") {
                boost::real::real a("15");
                boost::real::real b("15");
                boost::real::real c = a + b;
                boost::real::real d("2");
                boost::real::real e("2");
                boost::real::real f = d * e;

                CHECK_FALSE(c == f);
            }

            SECTION("Subtraction == Explicit") {
                boost::real::real a("15");
                boost::real::real b("15");
                boost::real::real c = a - b;
                boost::real::real d("1");

                CHECK_FALSE(c == d);
            }

            SECTION("Subtraction == Addition") {
                boost::real::real a("15");
                boost::real::real b("15");
                boost::real::real c = a - b;
                boost::real::real d("15");
                boost::real::real e = a + d;

                CHECK_FALSE(c == e);
            }

            SECTION("Subtraction == Subtraction") {
                boost::real::real a("15");
                boost::real::real b("1");
                boost::real::real c = a - a;
                boost::real::real d = a - b;

                CHECK_FALSE(c == d);
            }

            SECTION("Subtraction == multiplication") {
                boost::real::real a("15");
                boost::real::real b("15");
                boost::real::real c = a - b;
                boost::real::real d = a * b;

                CHECK_FALSE(c == d);
            }

            SECTION("multiplication == Explicit") {
                boost::real::real a("15");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d("4");

                CHECK_FALSE(c == d);
            }

            SECTION("multiplication == Addition") {
                boost::real::real a("15");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d = a + b;

                CHECK_FALSE(c == d);
            }

            SECTION("multiplication == Subtraction") {
                boost::real::real a("15");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d("5");
                boost::real::real e = d - a;

                CHECK_FALSE(c == e);
            }

            SECTION("multiplication == multiplication") {
                boost::real::real a("15");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d = b * b;

                CHECK_FALSE(c == d);
            }
        }



        SECTION("With precision exception") {

            SECTION("Explicit == Explicit") {
                boost::real::real a("1555555555555555550");
                boost::real::real b("1555555555555555555");

                CHECK_FALSE(a == b);
            }

            SECTION("Explicit == Addition") {
                boost::real::real a("43567453452342365868");
                boost::real::real b("9347507114507409570");
                //407247081106740667736968840309864556760
                boost::real::real c("63495863466936834975983475945345922");
                boost::real::real d("407183585243273730901992856833919210838");
                boost::real::real e = a * b;
                boost::real::real f = c + d;

                CHECK(e == f);
            }

            SECTION("Explicit == Subtraction") {
                boost::real::real a("22222222222222222222");
                boost::real::real b("22222222222222222224");
                boost::real::real c("2");
                boost::real::real d = b - c;

                CHECK(a == d);
            }

            SECTION("Explicit == multiplication") {
                boost::real::real a("22222222222220000000000");
                boost::real::real b("11111111111110000000000");
                boost::real::real c("2");
                boost::real::real d = b * c;

                CHECK(a == d);
            }

            SECTION("Addition == Explicit") {
                boost::real::real a("11111111111111");
                boost::real::real b("11111111111111");
                boost::real::real c = a + b;
                boost::real::real d("22222222222222");

                CHECK(c == d);
            }

            SECTION("Addition == Addition") {
                boost::real::real a("11111111111110");
                boost::real::real b("11111111111111");
                boost::real::real c = a + b;
                boost::real::real d = a + b;

                CHECK(c == d);
            }

            SECTION("Addition == Subtraction") {
                boost::real::real a("11111111111110");
                boost::real::real b("11111111111111");
                boost::real::real c = a + b;
                boost::real::real d("22222222222223");
                boost::real::real e("00000000000002");
                boost::real::real f = d - e;

                CHECK(c == f);
            }

            SECTION("Addition == multiplication") {
                boost::real::real a("11111111111111");
                boost::real::real b("11111111111111");
                boost::real::real c = a + b;
                boost::real::real d("2");
                boost::real::real e = a * d;

                CHECK(c == e);
            }

            SECTION("Subtraction == Explicit") {
                boost::real::real a("11111111111111");
                boost::real::real b("00000000000001");
                boost::real::real c = a - b;
                boost::real::real d("11111111111110");

                CHECK(c == d);
            }

            SECTION("Subtraction == Addition") {
                boost::real::real a("11111111111111");
                boost::real::real b("00000000000001");
                boost::real::real c = a - b;
                boost::real::real d("-00000000000001");
                boost::real::real e = a + d;

                CHECK(c == e);
            }

            SECTION("Subtraction == Subtraction") {
                boost::real::real a("11111111111111");
                boost::real::real b("00000000000001");
                boost::real::real c = a - b;
                boost::real::real d = a - b;

                CHECK(c == d);
            }

            SECTION("Subtraction == multiplication") {
                boost::real::real a("11111111111111");
                boost::real::real b("00000000000001");
                boost::real::real c = a + b;
                boost::real::real d("11111111111112");
                boost::real::real e("1");
                boost::real::real f = d * e;

                CHECK(c == f);
            }

            SECTION("multiplication == Explicit") {
                boost::real::real a("1111111111111");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d("2222222222222");

                CHECK(c == d);
            }

            SECTION("multiplication == Addition") {
                boost::real::real a("1111111111111");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d = a + a;

                CHECK(c == d);
            }

            SECTION("multiplication == Subtraction") {
                boost::real::real a("1111111111111");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d("2222222222223");
                boost::real::real e("0000000000001");
                boost::real::real f = d - e;

                CHECK(c == f);
            }

            SECTION("multiplication == multiplication") {
                boost::real::real a("1111111111111");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d = a * b;

                CHECK(c == d);
            }
        }

    }

    SECTION("a > b") {

        SECTION("without precision exception") {
            SECTION("Explicit == Explicit") {
                boost::real::real a("2");
                boost::real::real b("15");

                CHECK_FALSE(a == b);
            }

            SECTION("Explicit == Addition") {
                boost::real::real a("15");
                boost::real::real b("05");
                boost::real::real c("05");
                boost::real::real d = b + c;

                CHECK_FALSE(a == d);
            }

            SECTION("Explicit == Subtraction") {
                boost::real::real a("15");
                boost::real::real b("15");
                boost::real::real c("05");
                boost::real::real d = b - c;

                CHECK_FALSE(a == d);
            }

            SECTION("Explicit == multiplication") {
                boost::real::real a("5");
                boost::real::real b("2");
                boost::real::real c("2");
                boost::real::real d = b * c;

                CHECK_FALSE(a == d);
            }

            SECTION("Addition == Explicit") {
                boost::real::real a("2");
                boost::real::real b("15");
                boost::real::real c = a + b;
                boost::real::real d("3");

                CHECK_FALSE(c == d);
            }

            SECTION("Addition == Addition") {
                boost::real::real a("15");
                boost::real::real b("2");
                boost::real::real c = a + b;
                boost::real::real d = a + a;

                CHECK_FALSE(c == d);
            }

            SECTION("Addition == Subtraction") {
                boost::real::real a("15");
                boost::real::real b("15");
                boost::real::real c = a + b;
                boost::real::real d("35");
                boost::real::real e("1");
                boost::real::real f = d - e;

                CHECK_FALSE(c == f);
            }

            SECTION("Addition == multiplication") {
                boost::real::real a("15");
                boost::real::real b("2");
                boost::real::real c = a + b;
                boost::real::real d("15");
                boost::real::real e("2");
                boost::real::real f = d * e;

                CHECK_FALSE(c == f);
            }

            SECTION("Subtraction == Explicit") {
                boost::real::real a("15");
                boost::real::real b("1");
                boost::real::real c = a - b;
                boost::real::real d("0");

                CHECK_FALSE(c == d);
            }

            SECTION("Subtraction == Addition") {
                boost::real::real a("15");
                boost::real::real b("1");
                boost::real::real c = a - b;
                boost::real::real d("-15");
                boost::real::real e = a + d;

                CHECK_FALSE(c == e);
            }

            SECTION("Subtraction == Subtraction") {
                boost::real::real a("15");
                boost::real::real b("1");
                boost::real::real c = a - b;
                boost::real::real d = a - a;

                CHECK_FALSE(c == d);
            }

            SECTION("Subtraction == multiplication") {
                boost::real::real a("15");
                boost::real::real b("1");
                boost::real::real c = a - b;
                boost::real::real d("0");
                boost::real::real e = a * d;

                CHECK_FALSE(c == e);
            }

            SECTION("multiplication == Explicit") {
                boost::real::real a("2");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d("3");

                CHECK_FALSE(c == d);
            }

            SECTION("multiplication == Addition") {
                boost::real::real a("3");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d = a + b;

                CHECK_FALSE(c == d);
            }

            SECTION("multiplication == Subtraction") {
                boost::real::real a("15");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d("05");
                boost::real::real e = b - d;

                CHECK_FALSE(c == e);
            }

            SECTION("multiplication == multiplication") {
                boost::real::real a("15");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d = a * a;

                CHECK_FALSE(c == d);
            }
        }



        SECTION("With precision exception") {
            SECTION("Explicit == Explicit") {
                boost::real::real a("15555555555555555");
                boost::real::real b("15555555555555555");

                CHECK(a == b);
            }

            SECTION("Explicit == Addition") {
                boost::real::real a("22222222222220000000000");
                boost::real::real b("11111111111110000000000");
                boost::real::real c("11111111111110000000000");
                boost::real::real d = b + c;

                CHECK(a == d);
            }

            SECTION("Explicit == Subtraction") {
                boost::real::real a("22222222222222222221");
                boost::real::real b("22222222222222222222");
                boost::real::real c("1");
                boost::real::real d = b - c;

                CHECK(a == d);
            }

            SECTION("Explicit == multiplication") {
                boost::real::real a("22222222222220000000");
                boost::real::real b("11111111111110000000");
                boost::real::real c("2");
                boost::real::real d = b * c;

                CHECK(a == d);
            }

            SECTION("Addition == Explicit") {
                boost::real::real a("11111111111111");
                boost::real::real b("11111111111111");
                boost::real::real c = a + b;
                boost::real::real d("22222222222222");

                CHECK(c == d);
            }

            SECTION("Addition == Addition") {
                boost::real::real a("11111111111111");
                boost::real::real b("11111111111111");
                boost::real::real c = a + a;
                boost::real::real d = b + b;

                CHECK(c == d);
            }

            SECTION("Addition == Subtraction") {
                boost::real::real a("11111111111111");
                boost::real::real b("11111111111122");
                boost::real::real c = a + b;
                boost::real::real d("22222222222223");
                boost::real::real e("-00000000000010");
                boost::real::real f = d - e;

                CHECK(c == f);
            }

            SECTION("Addition == multiplication") {
                boost::real::real a("11111111111111");
                boost::real::real b("11111111111111");
                boost::real::real c = a + b;
                boost::real::real d("2");
                boost::real::real e = a * d;

                CHECK(c == e);
            }

            SECTION("Subtraction == Explicit") {
                boost::real::real a("11111111111112");
                boost::real::real b("00000000000001");
                boost::real::real c = a - b;
                boost::real::real d("11111111111111");

                CHECK(c == d);
            }

            SECTION("Subtraction == Addition") {
                boost::real::real a("11111111111115");
                boost::real::real b("00000000000001");
                boost::real::real c = a - b;
                boost::real::real d("-00000000000001");
                boost::real::real e = a + d;

                CHECK(c == e);
            }

            SECTION("Subtraction == Subtraction") {
                boost::real::real a("11111111111114");
                boost::real::real b("00000000000001");
                boost::real::real c = a - b;
                boost::real::real d("-00000000000001");
                boost::real::real e = a + d;

                CHECK(c == e);
            }

            SECTION("Subtraction == multiplication") {
                boost::real::real a("11111111111112");
                boost::real::real b("00000000000001");
                boost::real::real c = a - b;
                boost::real::real d("11111111111111");
                boost::real::real e("1");
                boost::real::real f = d * e;

                CHECK(c == f);
            }

            SECTION("multiplication == Explicit") {
                boost::real::real a("11111111111111");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d("22222222222222");

                CHECK(c == d);
            }

            SECTION("multiplication == Addition") {
                boost::real::real a("11111111111111");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d("11111111111111");
                boost::real::real e = a + d;

                CHECK(c == e);
            }

            SECTION("multiplication == Subtraction") {
                boost::real::real a("11111111111111");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d("22222222222221");
                boost::real::real e("00000000000001");
                boost::real::real f = d + e;

                CHECK(c == f);
            }

            SECTION("multiplication == multiplication") {
                boost::real::real a("11111111111111");
                boost::real::real b("2");
                boost::real::real c = a * b;
                boost::real::real d("11111111111111");
                boost::real::real e = d * b;
                CHECK(c == e);
            }
        }
    }
}
