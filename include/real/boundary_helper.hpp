#ifndef BOOST_REAL_BOUNDARY_HELPER_HPP
#define BOOST_REAL_BOUNDARY_HELPER_HPP

#include <algorithm>
#include "interval.hpp"

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
            bool aligned_vectors_is_lower(const std::vector<T> &lhs, const std::vector<T> &rhs) {

                // Check if lhs is lower than rhs
                auto lhs_it = lhs.cbegin();
                auto rhs_it = rhs.cbegin();
                while (rhs_it != rhs.end() && lhs_it != lhs.end() && *lhs_it == *rhs_it) {
                    ++lhs_it;
                    ++rhs_it;
                }

                if (rhs_it != rhs.cend() && lhs_it != lhs.cend()) {
                    return *lhs_it < *rhs_it;
                }

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
                            unsigned long long int base = 30) {
                int carry = 0;
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
                                 std::vector<T> &result,
                                 unsigned long long int base = 30) {

                std::vector<int> temp;
                int fractional_length = std::max((int)lhs.size() - lhs_exponent, (int)rhs.size() - rhs_exponent);
                int integral_length = std::max(lhs_exponent, rhs_exponent);
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
        }
    }
}

#endif //BOOST_REAL_BOUNDARY_HELPER_HPP
