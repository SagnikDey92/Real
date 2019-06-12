#ifndef BOOST_REAL_REAL_HELPERS_HPP
#define BOOST_REAL_REAL_HELPERS_HPP

#include <list>
#include <utility>

#include <real/interval.hpp>
#include <real/boundary.hpp>

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
             * @brief Implement the addition between two std::vector<unsigned int> that represent numbers.
             * The numbers in the vectors are the digits and each vector has a corresponding exponent
             * to correctly align the vectors for the addition.
             *
             * @param lhs - a std::vector<unsigned int> representing the left operand.
             * @param lhs_exponent - a int representing the exponent of the left operand.
             * @param rhs - a std::vector<unsigned int> representing the right operand.
             * @param rhs_exponent - a int representing the exponent of the right operand.
             * @param result - a std::vector<unsigned int> that is used to store the result.
             * @return a integer representing the exponent of the result.
             */
            template <typename T = int>
            int add_vectors(const std::vector<T> &lhs,
                            int lhs_exponent,
                            const std::vector<T> &rhs,
                            int rhs_exponent,
                            std::vector<T> &result) {
                int carry = 0;
                unsigned long long int base = 30;
                std::vector<int> temp;
                int fractional_length = std::max((int)lhs.size() - lhs_exponent, (int)rhs.size() - rhs_exponent);
                int integral_length = std::max(lhs_exponent, rhs_exponent);

                // we walk the numbers from the lowest to the highest digit
                for (int i = fractional_length - 1; i >= -integral_length; i--) {

                    T lhs_digit = 0;
                    if (0 <= lhs_exponent + i && lhs_exponent + i < (int)lhs.size()) {
                        lhs_digit = lhs[lhs_exponent + i];
                    }

                    T rhs_digit = 0;
                    if (0 <= rhs_exponent + i && rhs_exponent + i < (int)rhs.size()) {
                        rhs_digit = rhs[rhs_exponent + i];
                    }

                    unsigned long long int digit = carry + lhs_digit + rhs_digit;

                    if (digit > base-1) {
                        carry = 1;
                        digit -= base;
                    } else {
                        carry = 0;
                    }

                    temp.insert(temp.begin(), digit);
                    }
                if (carry == 1) {
                    temp.insert(temp.begin(), 1);
                    integral_length++;
                }
                result = temp;
                return integral_length;
            }

            /**
             * @author Laouen Mayal Louan Belloli
             *
             * @brief Implement the subtraction between two std::vector<unsigned int> that represent numbers.
             * The numbers in the vectors are the digit and each vector has a corresponding exponent
             * to correctly align the vectors for the addition.
             *
             * @pre lhs >= rhs.
             *
             * @param lhs - a std::vector<unsigned int> representing the left operand.
             * @param lhs_exponent - a int representing the exponent of the left operand.
             * @param rhs - a std::vector<unsigned int> representing the right operand.
             * @param rhs_exponent - a int representing the exponent of the right operand.
             * @param result - a std::vector<unsigned int> that is used to store the result.
             * @return a integer representing the exponent of the result.
             */
            template <typename T = int>
            int subtract_vectors(const std::vector<T> &lhs,
                                 int lhs_exponent,
                                 const std::vector<T> &rhs,
                                 int rhs_exponent,
                                 std::vector<T> &result) {

                std::vector<int> temp;
                int fractional_length = std::max((int)lhs.size() - lhs_exponent, (int)rhs.size() - rhs_exponent);
                int integral_length = std::max(lhs_exponent, rhs_exponent);
                unsigned long long int base = 30;
                int borrow = 0;
                // we walk the numbers from the lowest to the highest digit
                for (int i = fractional_length - 1; i >= -integral_length; i--) {

                    T lhs_digit = 0;
                    if (0 <= lhs_exponent + i && lhs_exponent + i < (int)lhs.size()) {
                        lhs_digit = lhs[lhs_exponent + i];
                    }

                    T rhs_digit = 0;
                    if (0 <= rhs_exponent + i && rhs_exponent + i < (int)rhs.size()) {
                        rhs_digit = rhs[rhs_exponent + i];
                    }

                    if (lhs_digit < borrow) {
                        lhs_digit += (base - borrow);
                    } else {
                        lhs_digit -= borrow;
                        borrow = 0;
                    }

                    if (lhs_digit < rhs_digit) {
                        lhs_digit += base;
                        borrow++;
                    }

                    temp.insert(temp.begin(), lhs_digit - rhs_digit);
                }
                result = temp;

                return lhs_exponent;
            }

            /**
             * @author Laouen Mayal Louan Belloli
             *
             * @brief Implement the multiplication between two std::vector<unsigned int> that represent numbers.
             * The numbers in the vectors are the digit and each vector has a corresponding exponent
             * to correctly align the vectors for the addition.
             *
             * @param lhs - a std::vector<unsigned int> representing the left operand.
             * @param lhs_exponent - a int representing the exponent of the left operand.
             * @param rhs - a std::vector<unsigned int> representing the right operand.
             * @param rhs_exponent - a int representing the exponent of the right operand.
             * @param result - a std::vector<unsigned int> that is used to store the result.
             * @return a integer representing the exponent of the result.
             */
            template <typename T = int>
            int multiply_vectors(
                    const std::vector<T>& lhs,
                    int lhs_exponent,
                    const std::vector<T>& rhs,
                    int rhs_exponent,
                    std::vector<T>& result
            ) {

                unsigned long long int base = 30;
                // will keep the result number in vector in reverse order
                // Digits: .123 | Exponent: -3 | .000123 <--- Number size is the Digits size less the exponent
                // Digits: .123 | Exponent: 2  | 12.3
                std::vector<T> temp;
                size_t new_size = lhs.size() + rhs.size();
                if (lhs_exponent < 0) new_size -= lhs_exponent; // <--- Less the exponent
                if (rhs_exponent < 0) new_size -= rhs_exponent; // <--- Less the exponent

                for (int i = 0; i < (int)new_size; i++) temp.push_back(0);
                // TODO: Check why the assign method crashes.
                //result.assign(new_size, 0);

                // Below two indexes are used to find positions
                // in result.
                auto i_n1 = (int) temp.size() - 1;
                // Go from right to left in lhs
                for (int i = (int)lhs.size()-1; i>=0; i--) {
                    int carry = 0;

                    // To shift position to left after every
                    // multiplication of a digit in rhs
                    int i_n2 = 0;

                    // Go from right to left in rhs
                    for (int j = (int)rhs.size()-1; j>=0; j--) {

                        // Multiply current digit of second number with current digit of first number
                        // and add result to previously stored result at current position.
                        unsigned long long int sum = lhs[i]*rhs[j] + temp[i_n1 - i_n2] + carry;

                        // Carry for next iteration
                        carry = sum / base;

                        // Store result
                        temp[i_n1 - i_n2] = sum % base;

                        i_n2++;
                    }

                    // store carry in next cell
                    if (carry > 0) {
                        temp[i_n1 - i_n2] += carry;
                    }

                    // To shift position to left after every
                    // multiplication of a digit in lhs.
                    i_n1--;
                }

                int fractional_part = ((int)lhs.size() - lhs_exponent) + ((int)rhs.size() - rhs_exponent);
                int result_exponent = (int)temp.size() - fractional_part;
                
                result = temp;
                return result_exponent;
            }

            template <typename T = int>
            std::pair <std::vector<T>, T> long_division(std::vector<T> number, unsigned long long int divisor) { 
                std::vector<T> quotient; 
                int rem;
                int idx = 0; 
                unsigned long long int temp = number[idx]; 
                while (temp < divisor) 
                {
                    idx++;
                    if(idx>=number.size())
                        break;
                    temp = temp*10 + (number[idx]); 
                }

                if (number.size() <= idx)
                    rem = temp;

                while (number.size() > idx) 
                {  
                    quotient.push_back(temp / divisor); 
                    rem = temp%divisor;
                    temp = (temp % divisor)*10 + number[++idx]; 
                } 
                return make_pair(quotient, rem); 
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