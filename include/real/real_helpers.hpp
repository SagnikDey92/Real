#ifndef BOOST_REAL_REAL_HELPERS_HPP
#define BOOST_REAL_REAL_HELPERS_HPP

#include <list>
#include <utility>
#include <limits>

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
                                      int precision,
                                      bool is_upper,
                                      boundary<T>& tmp,
                                      boundary<T>& residual) {

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
                    tmp.clear();
                    residual.clear();

                    T base = (std::numeric_limits<T>::max() /4)*2 - 1;
                    
                    boost::real::boundary<T> left;
                    boost::real::boundary<T> right;
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

                    T H = base/2 + 1;
                    half.digits = {H};
                    half.exponent = 0;

                    tmp.digits = {1};
                    tmp.exponent = 1;

                    if (denominator == tmp) {
                        ret = numerator;
                        ret.positive = positive;
                        return;
                    }

                    if (denominator == numerator) { 
                        ret = tmp;
                        ret.positive = positive;
                        return;
                    }

                    ///@TODO: remember signs at the end of this function

                    boost::real::boundary<T> zero; 

                    if (denominator == zero)
                        throw(boost::real::divide_by_zero());
                    else if ((denominator > zero) && (denominator < tmp)) // 0 < d < 1
                        throw(boost::real::invalid_denominator());

                // N < D --> 0 < abs(Q) < 1
                if (numerator < denominator) {
                        left = boost::real::boundary<T>(); // 0
                        right = tmp; // 1
                    } else { // assuming D > 1. N > D ---> 1 < N / D < N
                        left = tmp; // 1
                        right = numerator;
                    }

                // Example: say we have 144 / 12. At min precision, this is
                // [100, 200] / [10, 20]
                // so, our quotient upper bound would be 200/10, and 
                // the lower bound would be 100/20.

                /// @TODO: the following
                // if numerator == 0, return 0 
                // if denominator == 1, return numerator
                // if denominator == 0, throw error

                // distance = (right - left) / 2
                boost::real::helper::subtract_boundaries(
                        right,
                        left,
                        tmp 
                        );
                boost::real::helper::multiply_boundaries(
                        tmp,                                    
                        half,
                        distance
                    );
                // ret = left + distance
                boost::real::helper::add_boundaries(
                        left,
                        distance,
                        ret
                );
                // residual = ret * denom - num, equals zero if numerator/denominator = ret
                boost::real::helper::multiply_boundaries(
                        ret,
                        denominator,
                        tmp 
                );
                boost::real::helper::subtract_boundaries(
                        tmp,
                        numerator,
                        residual
                );
                // calculate the result
                // continue the loop while we are still inaccurate (up to max precision), or while
                // we are on the wrong side of the answer
                boost::real::boundary<T> old_residual = residual;
                while ((boost::real::helper::abs(residual) > min_boundary_p) || 
                        (is_upper && (residual < min_boundary_n)) || (!is_upper && (residual > min_boundary_p))) /* &&
                        distance.exponent > min_boundary_p.exponent) */{
                    old_residual = residual;
                    // std::cout << "[res, ret] [" << residual << ", " << ret << "]\n";

                    // result too small, try halfway between ret and numerator 
                    if (residual < min_boundary_n) {
                        left = ret;
                    }
                    // distance is halved
                    tmp = distance;
                    boost::real::helper::multiply_boundaries(
                            tmp,
                            half,
                            distance
                        ); 
                    // truncate insignificant digits of distance
                    // NOTE: The loop will not terminate if you truncate too much, for certain
                    // divisions. 5 seems to work well, so I'm leaving it as that.
                    // we may want to look at this in closer detail
                    while (distance.size() > precision + 5)
                        distance.digits.pop_back();

                    // iterate ret
                    boost::real::helper::add_boundaries(
                            left,
                            distance,
                            ret
                    );
                    // truncate insignificant digits of ret
                    while (ret.size() > precision + 5)
                        ret.digits.pop_back();

                    // recalculate residual  N/D = Q ---> QD - N = residual
                    boost::real::helper::multiply_boundaries(
                            ret,
                            denominator,
                            tmp 
                    );
                    boost::real::helper::subtract_boundaries(
                            tmp,
                            numerator,
                            residual
                    );
                    residual.normalize();
                    if (old_residual == residual)
                        break;
                } // end while
                // now ret is correct, or at least within +-epsilon of correct value 
                // truncate ret
                while (ret.size() > precision)
                    ret.digits.pop_back();

                if (positive)
                    ret.positive = true;
                else
                    ret.positive = false;

                // recalculate residual for the final ret value
                boost::real::helper::multiply_boundaries(
                        ret,
                        denominator,
                        tmp 
                );
                boost::real::helper::subtract_boundaries(
                        tmp,
                        numerator,
                        residual
                );
                // note we have to normalize before comparison, because -0.0 != zero 
                residual.normalize();
                // perhaps, try rounding up/down here to see if that makes the residual = 0.

                if (residual != zero) { // then, we are not fully accurate, and we must round up/truncate
                                        // note truncation was already done before this
                    if(is_upper) { // round up
                        if (ret.digits.back() != base)
                            ++(ret.digits.back());
                        else { // back == 9
                            int index = precision;
                            bool keep_carrying = true;

                            while((index > 0) && keep_carrying) { // bring the carry back
                                if(ret.digits[index] != base) {
                                    ++ret.digits[index];
                                    keep_carrying = false;
                                } else // digits[index] == 9, we keep carrying
                                    ret.digits[index] = 0;
                                --index;
                            }

                            if ((index == 0) && keep_carrying) { // i.e., .999 should become 1.000
                                if(ret.digits[index] == base) {
                                    ret.digits[index] = 0;
                                    ret.push_front(1);
                                    ++ret.exponent;
                                }
                                else
                                    ++ret.digits[index];
                            }
                        }
                    } else { // !is_upper, we have a truncated result.
                        ;
                    }
                } else {
                    ; // residual is zero, and we have a fully accurate result.
                }
                ret.normalize();
                // std::cout << "[res, ret] [" << residual << ", " << ret << "]\n";
                // std::cout << "isupper: " << (is_upper ? "true" : "false") << '\n';
            } // end division lambda definition
        }
    }
}

#endif //BOOST_REAL_REAL_HELPERS_HPP