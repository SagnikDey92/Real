#ifndef BOOST_REAL_REAL_HELPERS_HPP
#define BOOST_REAL_REAL_HELPERS_HPP

#include <list>
#include <utility>

#include <real/interval.hpp>
#include <real/boundary.hpp>
#include <real/boundary_helper.hpp>

namespace boost {
    namespace real {
        namespace helper {

            template <typename T = int>
            boundary<T> abs(const boundary<T>& b) {
                boundary result = b;
                result.positive = true;
                return result;
            }

            /**
             * @author Laouen Mayal Louan Belloli
             *
             * @brief Implement the addition between two boost::real::boundary variables.
             *
             * @param lhs - a boost::real::boundary representing the left operand.
             * @param rhs - a boost::real::boundary representing the right operand.
             * @param result - a boost::real::boundary to store the result.
             */

            template <typename T = int>
            void add_boundaries(const boundary<T> &lhs,
                                const boundary<T> &rhs,
                                boundary<T> &result) {
                if (lhs.positive == rhs.positive) {
                    result.exponent = add_vectors(lhs.digits,
                                                  lhs.exponent,
                                                  rhs.digits,
                                                  rhs.exponent,
                                                  result.digits);
                    result.positive = lhs.positive;
                } else if (abs(rhs) < abs(lhs)) {
                    result.exponent = subtract_vectors(lhs.digits,
                                                       lhs.exponent,
                                                       rhs.digits,
                                                       rhs.exponent,
                                                       result.digits);
                    result.positive = lhs.positive;
                } else {
                    result.exponent = subtract_vectors(rhs.digits,
                                                       rhs.exponent,
                                                       lhs.digits,
                                                       lhs.exponent,
                                                       result.digits);
                    result.positive = rhs.positive;
                }

                result.normalize();
            }

            /**
             * @author Laouen Mayal Louan Belloli
             *
             * @brief Implement the subtraction between two boost::real::boundary variables.
             *
             * @param lhs - a boost::real::boundary representing the left operand.
             * @param rhs - a boost::real::boundary representing the right operand.
             * @param result - a boost::real::boundary to store the result.
             */

            template <typename T = int>
            void subtract_boundaries(const boundary<T> &lhs,
                                     const boundary<T> &rhs,
                                     boundary<T> &result) {
                if (lhs.positive != rhs.positive) {
                    result.exponent = add_vectors(lhs.digits,
                                                      lhs.exponent,
                                                      rhs.digits,
                                                      rhs.exponent,
                                                      result.digits);
                    result.positive = lhs.positive;
                } else {

                    if (abs(rhs) < abs(lhs)) {
                        result.exponent = subtract_vectors(lhs.digits,
                                                               lhs.exponent,
                                                               rhs.digits,
                                                               rhs.exponent,
                                                               result.digits);
                        result.positive = lhs.positive;
                    } else {
                        result.exponent = subtract_vectors(rhs.digits,
                                                               rhs.exponent,
                                                               lhs.digits,
                                                               lhs.exponent,
                                                               result.digits);
                        result.positive = !lhs.positive;
                    }
                }

                result.normalize();
            }

            /**
             * @author Laouen Mayal Louan Belloli
             *
             * @brief Implement the multiplication between two boost::real::boundary variables.
             *
             * @param lhs - a boost::real::boundary representing the left operand.
             * @param rhs - a boost::real::boundary representing the right operand.
             * @param result - a boost::real::boundary to store the result.
             */
            
            template <typename T = int>
            void multiply_boundaries(const boundary<T> &lhs,
                                     const boundary<T> &rhs,
                                     boundary<T> &result) {

                result.positive = lhs.positive == rhs.positive;
                result.exponent = multiply_vectors(lhs.digits,
                                                       lhs.exponent,
                                                       rhs.digits,
                                                       rhs.exponent,
                                                       result.digits);

                result.normalize();
            }

            template <typename T = int>
            void set_division_result (boundary<T>& numerator,
                                      boundary<T>& denominator,
                                      boundary<T>& ret,
                                      int precision) {

                    /// @TODO replace this with something more efficient. binary search is probably
                    // not very efficient
                    // it also completely recalculates on each precision increase
                    // instead, could use previous information to make better "guesses"
                    // for our iteration scheme.

                    /// @TODO convert div by negative exponents 
                    // 1/.001 = 1/(1/1000) = 1000
                    // 1.46 / .12 = 1.46 * 100 * 1/12
                    // 1 / .23 = 1 * 100 * (1/23)
                    // etc.,
                    // after this, no division by D < 1

                    unsigned long long int base = 30;
                    
                    boost::real::boundary<T> left;
                    boost::real::boundary<T> right;
                    boost::real::boundary<T> residual;
                    boost::real::boundary<T> tmp;
                    boost::real::boundary<T> half;
                    boost::real::boundary<T> distance;
                    boost::real::boundary<T> min_boundary_n;
                    boost::real::boundary<T> min_boundary_p;

                    bool positive = (numerator.positive == denominator.positive);
                    numerator = helper::abs(numerator);
                    denominator = helper::abs(denominator);

                    min_boundary_n.digits = {1};
                    ///@TODO ensure exponent doesn't overflow
                    min_boundary_n.exponent = -1 * precision;
                    min_boundary_n.positive = false;

                    min_boundary_p.digits = {1};
                    min_boundary_p.exponent = -1 * precision;

                    int H = base/2;
                    half.digits = {H};
                    half.exponent = 0;

                    tmp.digits = {1};
                    tmp.exponent = 1;
                    ///@TODO: remember signs at the end of this function

                // N < D --> 0 < abs(Q) < 1
                if (numerator < denominator) {
                        left = boundary<T>(); // 0
                        right = tmp; // 1
                    } else { // assuming D > 1. N > D ---> 1 < N / D < N
                        left = tmp; // 1
                        right = numerator;
                    }
                // Example: say we have 144 / 12. At min precision, this is
                // [100, 200] / [10, 20]
                // so, our quotient upper bound would be 200/10, and 
                // the lower bound would be 100/20.
                // std::cout << "N/D: " << numerator << '/' << denominator << '\n';

                /// @TODO: the following
                // if numerator == 0, return 0 
                // if denominator == 1, return numerator
                // if denominator == 0, throw error

                // distance = (right - left) / 2
                subtract_boundaries(
                        right,
                        left,
                        tmp 
                        );
                multiply_boundaries(
                        tmp,                                    
                        half,
                        distance
                    );
                // ret = denominator + distance
                add_boundaries(
                        denominator,
                        distance,
                        ret
                );
                // residual = ret * denom - num, equals zero if numerator/denominator = ret
                multiply_boundaries(
                        ret,
                        denominator,
                        residual
                );
                subtract_boundaries(
                        residual,
                        numerator,
                        residual
                );
                // calculate the result
                while (((helper::abs(residual) > min_boundary_p)) &&
                        (distance.exponent > (-1 * (precision ))) ){
                    // result too small, try halfway between ret and numerator 
                    if (residual < min_boundary_n) {
                        left = ret;
                    }
                    // distance is halved
                    tmp = distance;
                    multiply_boundaries(
                            tmp,
                            half,
                            distance
                        ); 
                    // truncate insignificant digits of distance
                    // using +5 because +1 truncates too much, for some reason
                    /// @TODO figure out truncation and precision
                    while (distance.size() > precision + 5)
                        distance.digits.pop_back();

                    // iterate ret
                    add_boundaries(
                            left,
                            distance,
                            ret
                    );
                    // truncate insignificant digits of ret
                    while (ret.size() > precision + 5 )
                        ret.digits.pop_back();

                    // recalculate residual  N/D = Q ---> QD - N = 0
                    multiply_boundaries(
                            ret,
                            denominator,
                            tmp 
                    );
                    subtract_boundaries(
                            tmp,
                            numerator,
                            residual
                    );
                } // now ret is correct, or at least within +-epsilon of correct value 
                // truncate ret
                /// @TODO verify this is within max precision, should be easy proof
                while (ret.size() > precision)
                    ret.digits.pop_back();
                if (positive)
                    ret.positive = true;
                else
                    ret.positive = false;
            }
        }
    }
}

#endif //BOOST_REAL_REAL_HELPERS_HPP