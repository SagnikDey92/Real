#ifndef BOOST_REAL_REAL_EXCEPTION_HPP
#define BOOST_REAL_REAL_EXCEPTION_HPP

#include <exception>

namespace boost {
    namespace real {
        struct precision_exception : public std::exception {

            const char * what () const throw () override {
                return "The boost::real number precision is too slow to compare both numbers";
            }
        };

        struct none_operation_exception : public std::exception {

            const char * what () const throw () override {
                return "The boost::real number operation cannot be NONE";
            }
        };

        struct invalid_representation_exception : public std::exception {

            const char * what () const throw () override {
                return "The boost::real number method cannot be called for real number for the current representation";
            }
        };

        struct invalid_string_number_exception : public std::exception {

            const char * what () const throw () override {
                return "The string passed to construct the boost::real number is invalid";
            }
        };

        struct undefined_max_precision_exception : public std::exception {

            const char * what () const throw () override {
                return "The maximum precision for boost::real has not been defined";
            }
        };

        struct divide_by_zero : public std::exception {

            const char * what () const throw () override {
                return "Divison by zero is undefined";
            }
        };

        struct invalid_denominator : public std::exception {
            const char * what () const throw () override {
                return "Divison with denominators 0 < d < 1 is undefined.";
            }
        };

        struct divergent_division_result_exception : public std::exception {
            const char * what () const throw () override {
                return "The divisor approximation interval contains 0, so the quotient is unbounded";
            }
        };
    }
}

#endif //BOOST_REAL_REAL_EXCEPTION_HPP
