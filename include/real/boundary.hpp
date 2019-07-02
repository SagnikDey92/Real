#ifndef BOOST_REAL_BOUNDARY_HPP
#define BOOST_REAL_BOUNDARY_HPP

#include <vector>
#include <sstream>
#include <algorithm>
#include <limits>
#include <string>
#include <iterator>

#include <real/boundary_helper.hpp>

namespace boost {
    namespace real {

        /**
         * @author Laouen Mayal Louan Belloli
         *
         * @brief Explicitly represents a number as a vector of digits with a sign and an exponent
         */
        template <typename T = int>
        struct boundary {
            std::vector<T> digits = {};
            int exponent = 0;
            bool positive = true;

            /**
             * @brief *default constructor*: It constructs a representation of the number zero.
             */
            boundary() = default;

            /**
             * @brief *Copy constructor:* It constructs a new boost::real::boundary that is a copy of the
             * other boost::real::boundary.
             *
             * @param other - The boost::real::boundary to copy.
             */
            boundary(const boundary &other) = default;

            /**
             * @brief Default asignment operator.
             *
             * @param other - The boost::real::boundary to copy.
             */
            boundary &operator=(const boundary& other) = default;

            /**
             * @brief *Lower comparator operator:* It compares the *this boost::real::boundary with the other
             * boost::real::boundary to determine if *this is lower than other.
             *
             * @param other - The right side operand boost::real::boundary to compare with *this.
             * @return a bool that is true if and only if *this is lower than other.
             */
            bool operator<(const boundary& other) const {

                if (this->positive != other.positive) {
                    return !this->positive;
                }

                if (this->positive) {
                    if (this->exponent == other.exponent) {
                        return boost::real::helper::aligned_vectors_is_lower(this->digits,
                                                                             other.digits);
                    }

                    return this->exponent < other.exponent;
                }

                if (this->exponent == other.exponent) {
                    return boost::real::helper::aligned_vectors_is_lower(other.digits,
                                                                         this->digits);
                }

                return other.exponent < this->exponent;
            }

            /**
             * @brief *Greater comparator operator:* It compares the *this boost::real::boundary with the other
             * boost::real::boundary to determine if *this is greater than other.
             *
             * @param other - The right side operand boost::real::boundary to compare with *this.
             * @return a bool that is true if and only if *this is greater than other.
             */
            bool operator>(const boundary& other) const {

                if (this->positive != other.positive) {
                    return this->positive;
                }

                if (this->positive) {
                    if (this->exponent == other.exponent) {
                        return boost::real::helper::aligned_vectors_is_lower(other.digits, this->digits);
                    }

                    return this->exponent > other.exponent;
                }

                if (this->exponent == other.exponent) {
                    return boost::real::helper::aligned_vectors_is_lower(this->digits, other.digits);
                }

                return other.exponent > this->exponent;
            }

            /**
             * @brief *Equality comparator operator:* It compares the *this boost::real::boundary with the other
             * boost::real::boundary to determine if *this and other are equals or not.
             *
             * @param other - The right side operand boost::real::boundary to compare with *this.
             * @return a bool that is true if and only if *this is equal to other.
             */
            bool operator==(const boundary& other) const {
                return !(*this < other || other < *this);
            }

            bool operator!=(const boundary& other) const {
                return !(*this == other);
            }

            /**
             * @brief Generates a string representation of the boost::real::boundary.
             *
             * @return a string that represents the state of the boost::real::boundary
             */
            std::string as_string() const {
                std::string result = "";            
                //@TODO remove this - and remember it later
                /*
                if (!this->positive) {
                    result = "-";
                }
                */

                // If the number is too large, scientific notation is used to print it.
                /* @TODO add back later
                if ((this->exponent < -10) || (this->exponent > (int)this->digits.size() + 10)) {
                    result += "0|.";

                    for (const auto& d: this->digits) {
                        result += std::to_string(d);
                        result += "|";
                    }

                    result += "e" + std::to_string(this->exponent);
                    return result;
                }
                */

                //@TODO remember the negative
                if (this->exponent <= 0) {
                    result += ".";

                    for (int i = this->exponent; i < (int) this->digits.size(); ++i) {
                        if (i < 0) {
                            result += "0";
                            result += " ";
                        } else {
                            result += std::to_string(this->digits[i]);
                            result += " ";
                        }
                    }
                } else {

                    int digit_amount = std::max(this->exponent, (int) this->digits.size());
                    for (int i = 0; i < digit_amount; ++i) {

                        if (i == this->exponent) {
                            result += ".";
                        }

                        if (i < (int) this->digits.size()) {
                            result += std::to_string(this->digits[i]);
                            result += " ";
                        } else {
                            result += "0";
                            result += " ";
                        }
                    }

                    if (result.back() == '.') {
                        result.pop_back();
                    }
                }
                
                //Form new string below in base 10.
                std::vector<int> new_result = {0};
                std::size_t dot_pos = result.find('.');
                std::string integer_part;
                std::string decimal_part;
                if (dot_pos == std::string::npos) {
                    integer_part = result;
                    decimal_part = "";
                } else {
                    integer_part = result.substr(0, dot_pos);
                    decimal_part = result.substr(dot_pos + 1);
                }
                std::stringstream ss1(integer_part);
                std::istream_iterator<std::string> begin1(ss1);
                std::istream_iterator<std::string> end1;
                std::vector<std::string> integer(begin1, end1);
                std::stringstream ss2(decimal_part);
                std::istream_iterator<std::string> begin2(ss2);
                std::istream_iterator<std::string> end2;
                std::vector<std::string> decimal(begin2, end2);
                std::reverse (decimal.begin(), decimal.end()); 

                //integer and decimal are string vectors with the "digits" in diff base
                T b = 1233;
                std::vector<int> base;
                std::vector<int> one = {1};
                while (b!=0) {
                    base.push_back(b%10);
                    b /=10;
                }
                std::reverse(base.begin(), base.end());
                boost::real::helper::add_vectors(base, base.size(), one, 1, base, 9);
                while(!integer.empty()) {
                    std::vector<int> temp;
                    std::string num = integer.back();
                    integer.pop_back();
                    for (auto j : num) {
                        temp.push_back(j - '0');
                    }
                    boost::real::helper::add_vectors(new_result, new_result.size(), temp, temp.size(), new_result, 9);
                    for (int i = 0; i<integer.size(); ++i) {
                        std::vector<int> temp;
                        std::string tempstr = integer[i];
                        for (int j = 0; j<tempstr.length(); ++j) {
                            temp.push_back(tempstr[j] - '0'); 
                        }
                        boost::real::helper::multiply_vectors(temp, temp.size(), base, base.size(), temp, 10);
                        int idx = 0;
                        while(idx < temp.size() && temp[idx]==0) 
                            ++idx;
                        temp.erase(temp.begin(), temp.begin() + idx);
                        std::stringstream ss;
                        std::copy( temp.begin(), temp.end(), std::ostream_iterator<int>(ss, ""));
                        std::string str = ss.str();
                        integer[i] = str;
                    }
                }

                std::stringstream ss;
                std::copy( new_result.begin(), new_result.end(), std::ostream_iterator<int>(ss, ""));
                std::string res_decimal = ss.str();
                std::vector<int> new_base = base;
                std::vector<std::vector<int>> powers = {base};
                for (size_t i = 0; i<decimal.size(); ++i) {
                    boost::real::helper::multiply_vectors(new_base, new_base.size(), base, base.size(), new_base, 10);
                    int idx = 0;
                    while(idx < new_base.size() && new_base[idx]==0) 
                        ++idx;
                    new_base.erase(new_base.begin(), new_base.begin() + idx);
                    powers.push_back(new_base);
                }
                size_t precision = powers.back().size() + 1;
                std::string zeroes = "";
                for (size_t i = 0; i<precision; ++i)
                    zeroes = zeroes + "0";
                std::vector<int> fraction = {0};
                auto pwr = powers.cbegin();
                while(!decimal.empty()) {
                    std::string tempstr = decimal.back();
                    decimal.pop_back();
                    tempstr = tempstr + zeroes;
                    std::vector<int> temp;
                    for (int j = 0; j<tempstr.length(); ++j) {
                        temp.push_back(tempstr[j] - '0'); 
                    }
                    std::vector<int> k = *pwr++;
                    std::vector<int> q;
                    boost::real::helper::divide_vectors(temp, k, q);
                    boost::real::helper::add_vectors(fraction, fraction.size(), q, q.size(), fraction, 9);
                }
                //@TODO The decimal part. And dont forget negative. Also, add exponent notation later.
                std::stringstream sslast;
                std::copy( fraction.begin(), fraction.end(), std::ostream_iterator<int>(sslast, ""));
                std::string fractionstr = sslast.str();
                while (fractionstr.length() < precision)
                    fractionstr = "0" + fractionstr;
                
                
                return (positive ? "" : "-") + res_decimal + "." + fractionstr;
            }

            /**
             * @brief Swaps the boost::real::boundary value with the value of the other boost::real::boundary.
             * This operation is a more preformant form of swapping to boost::real::boundaries.
             *
             * @param other - The boost::real::boundary to swap with *this.
             */
            void swap(boundary &other) {
                this->digits.swap(other.digits);
                std::swap(this->exponent, other.exponent);
                std::swap(this->positive, other.positive);
            }

            /**
             * @brief add the digit parameter as a new digit of the boost::real::boundary. The digit
             * is added in the right side of the number.
             *
             * @param digit - The new digit to add.
             */
            void push_back(int digit) {
                this->digits.push_back(digit);
            }

            /**
             * @brief add the digit parameter as a new digit of the boost::real::boundary. The digit
             * is added in the left side of the number.
             *
             * @param digit - The new digit to add.template <typename T = int>
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
             */
            void push_front(int digit) {
                this->digits.insert(this->digits.begin(), digit);
            }

            /**
             * @brief Removes extra zeros at the sides to convert the number representation into a
             * normalized representation.
             */
            void normalize() {
                while (this->digits.size() > 1 && this->digits.front() == 0) {
                    this->digits.erase(this->digits.begin());
                    this->exponent--;
                }

                while (this->digits.size() > 1 && this->digits.back() == 0) {
                    this->digits.pop_back();
                }

                // Zero could have many representation, and the normalized is the next one.
                if (this->digits.size() == 1 && this->digits.front() == 0) {
                    this->exponent = 0;
                    this->positive = true;
                }
            }

            /**
             * @brief Removes extra zeros at the left side to convert the number representation
             * into a semi normalized representation.
             */
            void normalize_left() {
                while (this->digits.size() > 1 && this->digits.front() == 0) {
                    this->digits.erase(this->digits.begin());
                    this->exponent--;
                }
            }

            /**
             * @brief ir clears the number digits.
             */
            void clear() {
                this->digits.clear();
            }

            /**
             * @brief Returns the n-th digit of the boost::real::boundary.
             *
             * @param n - an int number indicating the index of the requested digit.
             * @return an integer with the value of the number n-th digit.
             */
            T &operator[](int n) {
                return this->digits[n];
            }

            /**
             * @brief It returns the number of digits of the boost::real::boundary
             *
             * @return an unsigned long representing the number of digits of the boost::real::boundary
             */
            unsigned long size() {
                return this->digits.size();
            }

            friend std::ostream& operator<<(std::ostream& os, const boost::real::boundary<T>& boundary) {
                return os << boundary.as_string();
            }
        };
    }
}

#endif //BOOST_REAL_BOUNDARY_HPP
