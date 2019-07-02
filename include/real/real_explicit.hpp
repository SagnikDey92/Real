#ifndef BOOST_REAL_REAL_EXPLICIT_HPP
#define BOOST_REAL_REAL_EXPLICIT_HPP

#include <vector>
#include <iostream>
#include <initializer_list>
#include <string>
#include <regex>
#include <limits>
#include <algorithm>
#include <utility>

#include <real/real_exception.hpp>
#include <real/real_helpers.hpp>
#include <real/interval.hpp>

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
            // TODO: Replace this by a boost::real::boundary type
            // TODO: Add normalizations to the constructors
            std::vector<T> _digits = {};
            int _exponent = 1;
            bool _positive = true;

            // The number max precision is the same as the explicit number digits size.
            unsigned int _maximum_precision = 1;
        public:

            /**
             * @author Laouen Mayal Louan Belloli
             *
             * @brief is a forward iterator that iterates a boost::real::real_explicit number approximation
             * intervals. The iterator calculates the initial interval with the initial precision and
             * then it increases the precision in each iteration (++) and recalculates the interval.
             */
            class const_precision_iterator {
            private:

                // Iterator precision
                int _n;

                // Base
                T base = 1233;

                // Internal number to iterate
                real_explicit const* _real_ptr = nullptr;

                void check_and_swap_boundaries() {
                    if (!this->_real_ptr->_positive) {
                        this->approximation_interval.swap_bounds();
                    }
                }

            public:

                // Number approximation_interval boundaries
                boost::real::interval<T> approximation_interval;

                /**
                 * @brief **Default constructor:** Constructs an empty
                 * boost::real::real_explicit::const_precision_iterator that points to nullptr.
                 */
                const_precision_iterator() = default;

                /**
                 * @brief **Copy constructor:**
                 * Construct a new boost::real::real_explicit::const_precision_iterator which is
                 * a copy of the other iterator.
                 *
                 * @param other - the boost::real::real::const_precision_iterator to copy.
                 */
                const_precision_iterator(const const_precision_iterator& other) = default;

                /**
                 * @brief *Pointer constructor:* Construct a new boost::real::real_explicit::const_precision_iterator
                 * pointing to the boost::real::real_explicit number to iterate the number approximation intervals.
                 *
                 * The iterator will start pointing the lowest precision interval.
                 *
                 * @param real_number - the boost::real::real number to iterate.
                 */
                explicit const_precision_iterator(real_explicit const* real_number) : _n(1), _real_ptr(real_number) {
                    this->approximation_interval.lower_bound.exponent = this->_real_ptr->_exponent;
                    this->approximation_interval.upper_bound.exponent = this->_real_ptr->_exponent;
                    this->approximation_interval.lower_bound.positive = this->_real_ptr->_positive;
                    this->approximation_interval.upper_bound.positive = this->_real_ptr->_positive;

                    int first_digit = this->_real_ptr->_digits[0];
                    this->approximation_interval.lower_bound.digits.push_back(first_digit);

                    if (first_digit == base) {
                        this->approximation_interval.upper_bound.digits.push_back(1);
                        this->approximation_interval.upper_bound.exponent++;
                    } else if (this->_n < (int)this->_real_ptr->_digits.size()) {
                        this->approximation_interval.upper_bound.digits.push_back(first_digit + 1);
                    } else {
                        this->approximation_interval.upper_bound.digits.push_back(first_digit);
                    }

                    this->check_and_swap_boundaries();
                }

                /**
                 * @brief It recalculates the approximation interval boundaries increasing the used
                 * precision, the new pointed approximation interval is smaller than the current one.
                 */
                void operator++() {
                    this->iterate_n_times(1);
                }

                /**
                 * @brief It recalculates the approximation interval boundaries increasing the used
                 * precision n times, the new pointed approximation interval is smaller than the current one.
                 */
                void iterate_n_times(int n) {
                    // If the explicit number full precision has been already reached (the end)
                    // is the end of the iterator
                    if (this->_n >= (int)this->_real_ptr->_digits.size()) {
                        // TODO: Remove commented lines that are depreciated code
                        //this->approximation_interval.lower_bound.push_back(0);
                        //this->approximation_interval.upper_bound.push_back(0);
                        //this->_n++;
                        return;
                    }

                    // If the number is negative, boundaries are interpreted as mirrored:
                    // First, the operation is made as positive, and after boundary calculation
                    // boundaries are swapped to come back to the negative representation.
                    this->check_and_swap_boundaries();

                    // If the explicit number just reaches the full precision (the end)
                    // then set both boundaries are equals.
                    if (this->_n + n >= (int)this->_real_ptr->_digits.size()) {

                        for(int i = this->_n; i < (int)this->_real_ptr->_digits.size(); i++) {
                            this->approximation_interval.lower_bound.push_back(this->_real_ptr->_digits[i]);
                        }
                        this->approximation_interval.upper_bound = this->approximation_interval.lower_bound;


                    } else {

                        // If the explicit number didn't reaches the full precision (the end)
                        // then the number interval is defined by truncation.

                        for(int i = 0; i < n; i++) {
                            this->approximation_interval.lower_bound.push_back(this->_real_ptr->_digits[this->_n]);
                        }

                        this->approximation_interval.upper_bound.clear();
                        this->approximation_interval.upper_bound.digits.resize(this->approximation_interval.lower_bound.size());

                        int carry = 1;
                        for (int i = (int)this->approximation_interval.lower_bound.size() - 1; i >= 0; --i) {
                            if (this->approximation_interval.lower_bound[i] > base - carry) {
                                this->approximation_interval.upper_bound[i] = 0;
                            } else {
                                this->approximation_interval.upper_bound[i] = this->approximation_interval.lower_bound[i] + carry;
                                carry = 0;
                            }
                        }

                        if (carry > 0) {
                            this->approximation_interval.upper_bound.push_front(carry);
                            this->approximation_interval.upper_bound.exponent = this->approximation_interval.lower_bound.exponent + 1;
                        } else {
                            this->approximation_interval.upper_bound.exponent = this->approximation_interval.lower_bound.exponent;
                        }
                    }

                    // Left normalization of boundaries representation
                    this->approximation_interval.lower_bound.normalize_left();
                    this->approximation_interval.upper_bound.normalize_left();

                    this->check_and_swap_boundaries();
                    this->_n = std::min(this->_n + n, (int)this->_real_ptr->_digits.size());
                }

                /**
                 * @brief It compares by value equality; two boost::real::real_explicit::const_precision_iterators
                 * are equals if they are pointing to the same real number and are in the same precision iteration.
                 *
                 * @param other - A boost::real::real_explicit::const_precision_iterator that is the right side operand
                 * @return a bool that is true if and only if both iterators are equals.
                 */
                bool operator==(const const_precision_iterator& other) const {
                    // uninitialized iterators are never equals
                    if (this->_real_ptr == nullptr || other._real_ptr == nullptr) {
                        return false;
                    }

                    return (other._real_ptr == this->_real_ptr) &&
                            (other._n == this->_n) &&
                            (other.approximation_interval == this->approximation_interval);
                }

                /**
                 * @brief It compares by value not equal; two boost::real::real_explicit::const_precision_iterators.
                 *
                 * @param other - A boost::real::real_explicit::const_precision_iterator that is the right side operand
                 * @return a bool that is true if and only if both iterators are not equals.
                 */
                bool operator!=(const const_precision_iterator& other) const {
                    return !(*this == other);
                }
            };

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
            explicit real_explicit(const std::string& integer_part, const std::string& decimal_part, int exponent, bool positive) {
                this->_positive = positive;
                if (integer_part.empty() && decimal_part.empty()) {
                    this->_digits = {0};
                    this->_exponent = 0;
                    return;
                }
                this->_exponent = exponent;
                for (const auto& c : integer_part ) {
                    this->_digits.push_back(c - '0');
                }
                for (const auto& c : decimal_part ) {
                    this->_digits.push_back(c - '0');
                }
                //changing base below
                exponent = 0;
                //int b = 30;
                T b = 1233;
                std::vector<int> base;
                std::vector<int> one = {1};
                while (b!=0) {
                    base.push_back(b%10);
                    b /=10;
                }
                std::reverse(base.begin(), base.end());
                boost::real::helper::add_vectors(base, base.size(), one, 1, base, 9);
                int curr_size = this->_digits.size();
                
                for (int i = 0; i<this->_exponent-curr_size; ++i) {
                    this->_digits.push_back(0);
                }
                
                while (this->_digits.size()>1) {
                    std::vector<T> quotient;
                    std::vector<T> rem = boost::real::helper::divide_vectors(this->_digits, base, quotient);
                    if (rem.empty()) {
                        this->_digits = quotient;
                        ++exponent; 
                    }
                    else
                        break;
                }
                
                std::vector<T> new_digits;
                while (!this->_digits.empty()) {
                    std::vector<T> quotient;
                    //std::vector<T> rem;
                    std::vector<T> rem = boost::real::helper::divide_vectors(this->_digits, base, quotient);
                    T result = 0;
                    for (auto d : rem)  
                    {
                        result = result * 10 + d;
                    }
                    new_digits.push_back(result);
                    this->_digits = quotient;
                }
                
                std::reverse (new_digits.begin(), new_digits.end());
                exponent += new_digits.size();

                this->_digits = new_digits;
                this->_exponent = exponent;
                this->_maximum_precision = (int)this->_digits.size();

            };

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
                    this->_positive = true;
                    integer_part = integer_part.substr(1);
                }
                else if (integer_part[0] == '-') {
                    this->_positive = false;
                    integer_part = integer_part.substr(1);
                }
                integer_part = regex_replace(integer_part, std::regex("(0?+)([[:digit:]]?+)"), "$2");
                int i = decimal_part.length() - 1;
                while (decimal_part[i] == '0' && i >= 0) {
                    --i;
                }
                decimal_part = decimal_part.substr(0, i + 1);
                //decimal and integer parts are stripped of zeroes
                int exponent = integer_part.length() + add_exponent;
                if (decimal_part.empty()) {
                    i = integer_part.length() - 1;
                    while (integer_part[i] == '0' && i >= 0)
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
                    this->_digits = {0};
                    this->_exponent = 0;
                    return;
                }
                this->_exponent = exponent;
                this->_maximum_precision = (int)this->_digits.size();
                for (const auto& c : integer_part ) {
                    this->_digits.push_back(c - '0');
                }
                for (const auto& c : decimal_part ) {
                    this->_digits.push_back(c - '0');
                }
                //changing base below
                exponent = 0;
                //int b = 30;
                T b = 1233;
                std::vector<int> base;
                std::vector<int> one = {1};
                while (b!=0) {
                    base.push_back(b%10);
                    b /=10;
                }
                std::reverse(base.begin(), base.end());
                boost::real::helper::add_vectors(base, base.size(), one, 1, base);
                int curr_size = this->_digits.size();

                for (int i = 0; i<this->_exponent-curr_size; ++i) {
                    this->_digits.push_back(0);
                }

                while (this->_digits.size()>1) {
                    std::vector<T> quotient;
                    std::vector<T> rem = boost::real::helper::divide_vectors(this->_digits, base, quotient);
                    if (rem.empty()) {
                        this->_digits = quotient;
                        ++exponent; 
                    }
                    else
                        break;
                }

                std::vector<T> new_digits;
                while (!this->_digits.empty()) {
                    std::vector<T> quotient;
                    std::vector<T> rem = boost::real::helper::divide_vectors(this->_digits, base, quotient);
                    T result = 0;
                    for (auto d : rem)  
                    {
                        result = result * 10 + d;
                    }
                    new_digits.push_back(result);
                    this->_digits = quotient;
                }
                std::reverse (new_digits.begin(), new_digits.end());
                exponent += new_digits.size();

                this->_digits = new_digits;
                this->_exponent = exponent;
            };

            /**
             * @brief *Initializer list constructor with exponent:* Creates a boost::real::real_explicit
             * instance that represents the number where the exponent is used to set the number
             * integer part and the elements of the digits list are the digits the number in the same order.
             * The number is set as positive.
             *
             * @param digits - an initializer_list<int> that represents the number digits.
             * @param exponent - an integer representing the number exponent.
             */
            real_explicit(std::initializer_list<T> digits, int exponent) :
                    _digits(digits),
                    _exponent(exponent),
                    _maximum_precision((int)this->_digits.size())
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
            real_explicit(std::initializer_list<T> digits, int exponent, bool positive):
                    _digits(digits),
                    _exponent(exponent),
                    _positive(positive),
                    _maximum_precision((int)this->_digits.size())
            {};

            /**
             * @brief Returns the maximum allowed precision, if that precision is reached and an
             * operator needs more precision, a precision_exception should be thrown.
             *
             * @return an integer with the maximum allowed precision.
             */
            unsigned int max_precision() const {
                return this->_maximum_precision;
            }

            /**
             * @brief Set a new maximum precision for the instance.
             *
             * @param maximum_precision - an unsigned int to set as the new precision.
             */
            void set_maximum_precision(unsigned int maximum_precision) {
                this->_maximum_precision = maximum_precision;
            }

            /**
             * @return An integer with the number exponent
             */
            int exponent() const {
                return this->_exponent;
            }

            /**
             * @return A bool indicating if the number is positive (true) or negative (false)
             */
            bool positive() const {
                return this->_positive;
            }

            /**
             * @return a const reference to the vector holding the number digits
             */
            const std::vector<T>& digits() const {
                return this->_digits;
            }

            /**
             * @brief Construct a new boost::real::real_explicit::const_precision_iterator that iterates the number
             * approximation intervals in increasing order according to the approximation precision.
             *
             * The iterator starts pointing to the interval with the minimum precision.
             *
             * @return a boost::real::real_explicit::const_precision_iterator of the number.
             */
            const_precision_iterator cbegin() const {
                return const_precision_iterator(this);
            }

            /**
             * @brief Constructs a new boost::real::real_explicit::const_precision_iterator that iterates the number
             * approximation intervals in increasing order according to the approximation precision.
             *
             * The iterator starts pointing to the interval with the maximum allowed precision.
             *
             * @return a boost::real::real_explicit::const_precision_iterator of the number.
             */
            const_precision_iterator cend() const {
                const_precision_iterator it(this);
                it.iterate_n_times((int)this->_digits.size() + 1);
                return it;
            }

            /**
             * @brief Returns the n-th digit the number.
             *
             * @param n - an unsigned int number indicating the index of the requested digit.
             * @return an integer with the value of the number n-th digit.
             */
            T operator[](unsigned int n) const {
                if (n < this->_digits.size()) {
                    return this->_digits.at(n);
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

/**
 * @bief overload of the << operator for std::ostream and boost::real::real_explicit
 *
 * @param os - The std::ostream object where to print the r number.
 * @param r - the boost::real::real_explicit number to print
 * @return a reference of the modified os object.
 */
template <typename T = int>
std::ostream& operator<<(std::ostream& os, const boost::real::real_explicit<T>& r) {
    auto it = r.cbegin();
    for (unsigned int i = 0; i <= r.max_precision(); i++) {
        ++it;
    }
    os << it.approximation_interval;
    return os;
}


#endif //BOOST_REAL_REAL_EXPLICIT_HPP
