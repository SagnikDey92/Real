#ifndef BOOST_REAL_REAL_EXPLICIT_HPP
#define BOOST_REAL_REAL_EXPLICIT_HPP

#include <vector>
#include <iostream>
#include <initializer_list>
#include <string>
#include <regex>

#include <real/real_exception.hpp>
#include <real/interval.hpp>
#include <real/exact_number.hpp>

namespace boost {
    namespace real {

        /**
         * @author Laouen Mayal Louan Belloli
         *
         * @brief boost::real::real_explicit is a C++ class that fully represents real numbers as
         * a vector of digits, a sign and an exponent.
         */
        template <typename T = int>
        class real_explicit {

            // Number representation as a vector of digits with an integer part and a sign (+/-)
            // TODO: Add normalizations to the constructors
            exact_number<T> explicit_number;

            // The number max precision is the same as the explicit number digits size.
            unsigned int _maximum_precision = 1;
            
        public:

            /**
             * @brief *Default constructor:* Constructs an empty boost::real::real_explicit with
             * undefined representation and behaviour.
             */
            real_explicit() = default;

            /**
             * @brief *Copy constructor:* Creates a copy of the boost::real::real_explicit number other.
             *
             * @param other - the boost::real::real instance to copy.
             */
            real_explicit(const real_explicit& other)  = default;

            /**
             * @brief *String constructor:* Creates a boost::real::real_explicit instance by
             * parsing the string. The string must have a valid number, otherwise, the
             * constructor will throw an boost::real::invalid_string_number exception.
             *
             * @param number - a valid string representing a number.
             *
             * @throws boost::real::invalid_string_number exception
             */
            
            std::vector<T> divide_vectors(
                    const std::vector<T>& dividend,
                    const std::vector<T>& divisor,
                    std::vector<T>& quotient
            ) {
                
                for(auto i:dividend)
                    std::cout<<i;
                std::cout<<"\t/\t";
                for(auto i:divisor)
                    std::cout<<i;
                std::cout<<"\t=\t";
                exact_number<T> tmp;
                std::vector<T> aligned_dividend = dividend;
                std::vector<T> aligned_divisor = divisor;
                int idx = 0;
                while(idx < aligned_dividend.size() && aligned_dividend[idx] == 0)
                    idx++;
                aligned_dividend.erase(aligned_dividend.begin(), aligned_dividend.begin() + idx);

                if(aligned_dividend.empty()) {
                    quotient.clear();
                    return std::vector<T>();
                }
                if ((aligned_dividend.size() == aligned_divisor.size() && 
                        tmp.aligned_vectors_is_lower(aligned_dividend, aligned_divisor)) || 
                            aligned_dividend.size() < aligned_divisor.size()) {
                    quotient.clear();
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
                        //multiply_vectors(aligned_divisor, (int)aligned_divisor.size(), multiplier, 1, closest, (T)10);
                        tmp = (exact_number(aligned_divisor)*exact_number(multiplier));
                        closest = tmp.digits;
                        while (tmp.exponent - (int)tmp.digits.size() > 0) {
                            closest.push_back(0);
                            tmp.exponent--;
                        }
                        int idx = 0;
                        while(idx < closest.size() && closest[idx]==0) 
                            ++idx;
                        closest.erase(closest.begin(), closest.begin() + idx);

                    } while(
                            closest.size() < current_dividend.size() ||
                            (
                                    closest.size() == current_dividend.size() &&
                                    !tmp.aligned_vectors_is_lower(
                                    current_dividend, closest, true)
                            ) // closes <= current_dividend
                    );

                    // i should be in [1, 10] and i - 1 in [0, 9]
                    // The found digit is the next digit in the quotient result
                    quotient.push_back(digit-1);

                    // Update the residual for the next iteration where more digits of the dividend will be considered
                    std::vector<T> multiplier = {digit-1};
                    //multiply_vectors(aligned_divisor, (int)aligned_divisor.size(), multiplier, 1, closest, (T)10);
                    tmp = (exact_number(aligned_divisor)*exact_number(multiplier));
                    closest = tmp.digits;
                    while (tmp.exponent - (int)tmp.digits.size() > 0) {
                        closest.push_back(0);
                        tmp.exponent--;
                    }
                    residual.clear();
                    //subtract_vectors(current_dividend, (int)current_dividend.size(), closest, (int)closest.size(), residual, (T)9);
                    tmp = (exact_number(current_dividend) - exact_number(closest));
                    residual = tmp.digits;
                    while (tmp.exponent - (int)tmp.digits.size() > 0) {
                        residual.push_back(0);
                        tmp.exponent--;
                    }
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
                while(idx < quotient.size() && quotient[idx] == 0)
                    idx++;
                quotient.erase(quotient.begin(), quotient.begin() + idx);
                for(auto i:quotient)
                    std::cout<<i;
                std::cout<<"\t(";
                for(auto i:residual)
                    std::cout<<i;
                std::cout<<")\n\n";
                return residual;
            }

            explicit real_explicit(const std::string& number) {
                std::regex decimal("((\\+|-)?[[:digit:]]*)(\\.(([[:digit:]]+)?))?((e|E)(((\\+|-)?)[[:digit:]]+))?");
                if (!std::regex_match (number, decimal))
                    throw boost::real::invalid_string_number_exception();
                //Know at this point that representation is valid
                std::string decimal_part = regex_replace(number, decimal, "$5");
                std::string integer_part = regex_replace(number, decimal, "$1");
                std::string exp = regex_replace(number, decimal, "$8");
                int add_exponent = exp.length() == 0 ? 0 : std::stoi(exp);
                if (integer_part[0] == '+') {
                    explicit_number.positive = true;
                    integer_part = integer_part.substr(1);
                }
                else if (integer_part[0] == '-') {
                    explicit_number.positive = false;
                    integer_part = integer_part.substr(1);
                }
                integer_part = regex_replace(integer_part, std::regex("(0?+)([[:digit:]]?+)"), "$2");
                size_t i = decimal_part.length() - 1;
                while (decimal_part[i] == '0' && i > 0) {
                    --i;
                }
                decimal_part = decimal_part.substr(0, i + 1);
                //decimal and integer parts are stripped of zeroes
                int exponent = integer_part.length() + add_exponent;
                if (decimal_part.empty()) {
                    i = integer_part.length() - 1;
                    while (integer_part[i] == '0' && i > 0)
                        --i;
                    integer_part = integer_part.substr(0, i + 1);
                }
                if (integer_part.empty()) {
                    i = 0;
                    while (decimal_part[i] == '0' && i < decimal_part.length()) {
                        ++i;
                        --exponent;
                    }
                    decimal_part = decimal_part.substr(i);
                }
                if (integer_part.empty() && decimal_part.empty()) {
                    explicit_number.digits = {0};
                    explicit_number.exponent = 0;
                    return;
                }
                explicit_number.exponent = exponent;
                for (const auto& c : integer_part ) {
                    explicit_number.digits.push_back(c - '0');
                }
                for (const auto& c : decimal_part ) {
                    explicit_number.digits.push_back(c - '0');
                }
                this->_maximum_precision = explicit_number.digits.size();
                //changing base below.
                exponent = 0;
                //int b = 30;
                std::vector<T> base = {3, 0};
                int curr_size = explicit_number.digits.size();
                
                for (int i = 0; i<explicit_number.exponent-curr_size; ++i) {
                    explicit_number.digits.push_back(0);
                }
                
                while (explicit_number.digits.size() > 1) {
                    std::vector<T> quotient;
                    std::vector<T> rem = divide_vectors(explicit_number.digits, base, quotient);
                    if (rem.empty()) {
                        explicit_number.digits = quotient;
                        ++exponent; 
                    }
                    else
                        break;
                }
                
                std::vector<T> new_digits;
                while (!explicit_number.digits.empty()) {
                    std::vector<T> quotient;
                    //std::vector<T> rem;
                    std::vector<T> rem = divide_vectors(explicit_number.digits, base, quotient);
                    T result = 0;
                    for (auto d : rem)  
                    {
                        result = result * 10 + d;
                    }
                    
                    new_digits.push_back(result);
                    explicit_number.digits = quotient;
                }
                
                std::reverse (new_digits.begin(), new_digits.end());
                exponent += (int)new_digits.size();
                explicit_number.digits = new_digits;
                explicit_number.exponent = exponent;
                this->_maximum_precision = (int)explicit_number.digits.size();
            }           

            /**
             * @brief *Initializer list constructor with exponent:* Creates a boost::real::real_explicit
             * instance that represents the number where the exponent is used to set the number
             * integer part and the elements of the digits list are the digits the number in the same order.
             * The number is set as positive.
             *
             * @param digits - an initializer_list<int> that represents the number digits.
             * @param exponent - an integer representing the number exponent.
             */
            real_explicit(std::initializer_list<int> digits, int exponent) : explicit_number(digits, exponent),
                                                                            _maximum_precision((int)explicit_number.digits.size())
            {};

            /**
             * @brief *Initializer list constructor with exponent and sign:* Creates a
             * boost::real::real_explicit instance that represents the number where the exponent
             * is used to set the number integer part and the elements of the digit list are the
             * digits of the number in the same order. This constructor uses the sign to determine
             * if the number is positive or negative.
             *
             * @param digits - an initializer_list<int> that represents the number digits.
             * @param exponent - an integer representing the number exponent.
             * @param positive - a bool that represents the number sign. If positive is set to true,
             * the number is positive, otherwise is negative.
             */
            real_explicit(std::initializer_list<int> digits, int exponent, bool positive) : 
                                                                                explicit_number(digits,exponent,positive) ,
                                                                                _maximum_precision((int)explicit_number.digits.size())
            {};

            /**
             * @return An integer with the number exponent
             */
            int exponent() const {
                return explicit_number.exponent;
            }

            /**
             * @return A bool indicating if the number is positive (true) or negative (false)
             */
            bool positive() const {
                return explicit_number.positive;
            }

            /**
             * @return a const reference to the vector holding the number digits
             */
            const std::vector<int>& digits() const {
                return explicit_number.digits;
            }

            /**
             * @brief Constructs a new boost::real::real_explicit::const_precision_iterator that iterates the number
             * approximation intervals in increasing order according to the approximation precision.
             *
             * The iterator starts pointing to the interval with the maximum allowed precision.
             *
             * @return a boost::real::real_explicit::const_precision_iterator of the number.
             */

            /**
             * @brief Returns the n-th digit the number.
             *
             * @param n - an unsigned int number indicating the index of the requested digit.
             * @return an integer with the value of the number n-th digit.
             */
            int operator[](unsigned int n) const {
                if (n < explicit_number.digits.size()) {
                    return explicit_number.digits.at(n);
                }

                return 0;
            }

            /**
             * @brief It assigns a new copy of the other boost::real::real_explicit number in the *this boost::real::real_explicit number.
             *
             * @param other - the boost::real::real_explicit number to copy.
             * @return a reference of *this with the new represented number.
             */
            real_explicit& operator=(const real_explicit& other) = default;
        };
    }
}



#endif //BOOST_REAL_REAL_EXPLICIT_HPP
