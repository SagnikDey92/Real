#ifndef BOOST_REAL_BOUNDARY_HELPER_HPP
#define BOOST_REAL_BOUNDARY_HELPER_HPP

#include <algorithm>
#include "interval.hpp"

#include <iostream>

namespace boost {
    namespace real {
        namespace helper {

            /**
             * @author Laouen Mayal Louan Belloli
             *
             * @brief checks whether the lhs vector elements are lower than the rhs elements if those
             * elements are interpreted as a number digits.
             *
             * @pre lhs and rhs must be aligned, i.e. two digits in the same index represent the same unit.
             *
             * @param lhs - a std::vector<unsigned int> used as the left side operand
             * @param rhs - a std::vector<unsigned int> used as the right side operand
             *
             * @return a bool that is true if and only if, the number represented by lsh is lower than
             * the represented number by rhs.
             */
            template <typename T = int>
            bool aligned_vectors_is_lower(const std::vector<T> &lhs, const std::vector<T> &rhs, bool equal = false) {

                // Check if lhs is lower than rhs
                auto lhs_it = lhs.cbegin();
                auto rhs_it = rhs.cbegin();
                while (rhs_it != rhs.end() && lhs_it != lhs.end() && *lhs_it == *rhs_it) {
                    ++lhs_it;
                    ++rhs_it;
                }

                if (rhs_it != rhs.cend() && lhs_it != lhs.cend())
                    return *lhs_it < *rhs_it;

                if (rhs_it == rhs.cend() && lhs_it == lhs.cend() && equal)
                    return false;

                bool lhs_all_zero = std::all_of(lhs_it, lhs.cend(), [](int i){ return i == 0; });
                bool rhs_all_zero = std::all_of(rhs_it, rhs.cend(), [](int i){ return i == 0; });

                return lhs_all_zero && !rhs_all_zero;
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
                            std::vector<T> &result,
                            const T base = 29) {

                int carry = 0;
                std::vector<T> temp;
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
                    
                    T digit;
                    int orig_carry = carry;
                    carry = 0;
                    if ((base - lhs_digit) < rhs_digit) {
                        T min = std::min(lhs_digit, rhs_digit);
                        T max = std::max(lhs_digit, rhs_digit);
                        if (min <= base/2) {
                            T remaining = base/2 - min;
                            digit = (max - base/2) - remaining - 2;
                        } else {
                            digit = (min - base/2) + (max - base/2) - 2;
                        }
                        carry = 1;
                    }
                    else {
                        carry = 0;
                        digit = rhs_digit + lhs_digit;
                    }
                    if (digit < base || orig_carry == 0) {
                        digit += orig_carry;
                    }
                    else {
                        carry = 1;
                        digit = 0;
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
                                 std::vector<T> &result,
                                 unsigned long long int base = 29) {

                std::vector<T> temp;
                int fractional_length = std::max((int)lhs.size() - lhs_exponent, (int)rhs.size() - rhs_exponent);
                int integral_length = std::max(lhs_exponent, rhs_exponent);
                int borrow = 0;
                // we walk the numbers from the lowest to the highest digit
                for (int i = fractional_length - 1; i >= -integral_length; i--) {

                    T digit = 0;

                    T lhs_digit = 0;
                    if (0 <= lhs_exponent + i && lhs_exponent + i < (int)lhs.size()) {
                        lhs_digit = lhs[lhs_exponent + i];
                    }

                    T rhs_digit = 0;
                    if (0 <= rhs_exponent + i && rhs_exponent + i < (int)rhs.size()) {
                        rhs_digit = rhs[rhs_exponent + i];
                    }

                    if (lhs_digit < borrow) {
                        digit = (base - rhs_digit) + 1 - borrow;
                    } else {
                        lhs_digit -= borrow;
                        borrow = 0;
                        
                        if (lhs_digit < rhs_digit) {
                        ++borrow;
                        digit = (base - (rhs_digit -1)) + lhs_digit;
                        } else {
                            digit = lhs_digit - rhs_digit;
                        }
                        
                    }                    
                    temp.insert(temp.begin(), digit);
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
                    std::vector<T>& result,
                    unsigned long long int base = 30
            ) {

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

            template <typename T>
            std::vector<T> divide_vectors(
                    const std::vector<T>& dividend,
                    const std::vector<T>& divisor,
                    std::vector<T>& cotient
            ) {
                
                std::vector<T> aligned_dividend = dividend;
                std::vector<T> aligned_divisor = divisor;
                int idx = 0;
                while(idx < aligned_dividend.size() && aligned_dividend[idx] == 0)
                    idx++;
                aligned_dividend.erase(aligned_dividend.begin(), aligned_dividend.begin() + idx);

                if(aligned_dividend.empty()) {
                    cotient.clear();
                    return std::vector<T>();
                }
                if ((aligned_dividend.size() == aligned_divisor.size() && 
                        aligned_vectors_is_lower(aligned_dividend, aligned_divisor)) || 
                            aligned_dividend.size() < aligned_divisor.size()) {
                    cotient.clear();
                    return aligned_dividend;
                }

                std::vector<T> current_dividend(
                        aligned_dividend.begin(),
                        aligned_dividend.begin() + aligned_divisor.size()
                );
                auto next_digit = aligned_dividend.begin() + aligned_divisor.size();
                std::vector<T> residual = aligned_dividend;

                // TODO: This loop end criteria generate a whole division, a precision stop criteria
                // TODO: must be implemented for numbers like 1/3 that are periodic numbers to allow
                // TODO: calculate floating point result with some desired precision
                while (true) {
                    // Obtain the smaller part of the dividend that is greater than the divisor

                    // Obtaining the greater digit by which the divisor can be multiplied and still be lower than the dividend
                    // TODO: when using a higher base, this search could be done using binary search to improve performance
                    bool flg = false;
                    if (next_digit == aligned_dividend.end())
                        flg = true;
                    std::vector<T> closest;
                    int digit = 0;
                    do {
                        digit++;
                        std::vector<T> multiplier = {digit};
                        multiply_vectors(aligned_divisor, (int)aligned_divisor.size(), multiplier, 1, closest, 10);
                        int idx = 0;
                        while(idx < closest.size() && closest[idx]==0) 
                            ++idx;
                        closest.erase(closest.begin(), closest.begin() + idx);

                    } while(
                            closest.size() < current_dividend.size() ||
                            (
                                    closest.size() == current_dividend.size() &&
                                    !boost::real::helper::aligned_vectors_is_lower(
                                    current_dividend, closest, true)
                            ) // closes <= current_dividend
                    );

                    // i should be in [1, 10] and i - 1 in [0, 9]
                    // The found digit is the next digit in the cotient result
                    cotient.push_back(digit-1);

                    // Update the residual for the next iteration where more digits of the dividend will be considered
                    std::vector<T> multiplier = {digit-1};
                    multiply_vectors(aligned_divisor, (int)aligned_divisor.size(), multiplier, 1, closest, 10);
                    residual.clear();
                    subtract_vectors(current_dividend, (int)current_dividend.size(), closest, (int)closest.size(), residual, 9);
                    int idx = 0;
                    while(idx < residual.size() && residual[idx]==0) 
                        ++idx;
                    residual.erase(residual.begin(), residual.begin() + idx);
                    current_dividend = residual;
                    current_dividend.push_back(*next_digit);
                    if (flg)
                        break;
                    ++next_digit;
                }
                // TODO: once the stop criteria is improved, the integer part is not the whole number
                idx = 0;
                while(idx < cotient.size() && cotient[idx] == 0)
                    idx++;
                cotient.erase(cotient.begin(), cotient.begin() + idx);
                return residual;
            }
            
        }
    }
}

#endif //BOOST_REAL_BOUNDARY_HELPER_HPP
