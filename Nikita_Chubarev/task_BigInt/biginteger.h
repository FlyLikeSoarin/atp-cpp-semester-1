#ifndef MY_BIGINTER_H
#define MY_BIGINTER_H

#define DEBUG(x) std::cerr<< #x << ": " << std::to_string(x)<<'\n'

#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;

class BigInteger {
public:
    BigInteger(int num) : sign(make_sign(num)) {
        if (num != 0) {
            num = num * sign;
            while (num > 0) {
                numbers.push_back(num % BASE);
                num /= BASE;
            }
        } else {
            numbers.push_back(0);
        }
    }

    BigInteger(long long num) : sign(make_sign(num)) {
        if (num != 0) {
            num = sign * num;
            while (num > 0) {
                numbers.push_back(num % BASE);
                num /= BASE;
            }
        } else {
            numbers.push_back(0);
        }
    }

    BigInteger(unsigned long int num) : sign(1) {
	if (num == 0) {
	      sign = 0;
	}	      
        while (num != 0) {
            numbers.push_back(num % BASE);
            num /= BASE;
        }
    }

    BigInteger() = default;

    BigInteger(const BigInteger& num) : sign(num.sign) {
        numbers = num.numbers;
    }

    ~BigInteger() = default;

    BigInteger& operator = (BigInteger num) {
        if (&*this == &num) {
            return *this;
        }
        clear();
        sign = num.sign;
        numbers = num.numbers;
        return *this;
    }

    BigInteger& operator += (const BigInteger& num_2) {
        return do_algebra_sum(num_2, 1);
    }

    BigInteger& operator -= (const BigInteger& num_2) {
        return do_algebra_sum(num_2, -1);
    }

    BigInteger& operator *= (const BigInteger& num_2) {
        return do_multiplication(num_2);
    }

    BigInteger& operator /= (const BigInteger& num_2) {
        return do_division(num_2);
    }

    BigInteger& operator %= (const BigInteger& num_2) {
        return do_modular_reward(num_2);
    }

    string toString() const {
        string str_num;
        if (sign == -1) {
            str_num.push_back('-');
        }
        for (size_t i = 1; i <= numbers.size(); ++i) {
            string new_part = std::to_string(numbers[numbers.size() - i]);
            size_t num_zero = 0;
            if (i != 1) {
                num_zero = BASE_LENGTH - new_part.size();
            }
            str_num += string(num_zero, '0');
            str_num += new_part;
        }
        return str_num;
    }

    void clear() {
        numbers.clear();
    }

    BigInteger operator - () const {
        BigInteger copy = *this;
        copy.sign *= -1;
        return copy;
    }

    BigInteger& operator ++ () {
        BigInteger one(1);
        *this += one;
        return *this;
    }

    BigInteger operator ++ (int) {
        BigInteger copy = *this;
        ++(*this);
        return copy;
    }

    bool operator < (const BigInteger& num_2) const {
        return (BigInteger::cmp_value(*this, num_2));
    }

    bool operator > (const BigInteger& num_2) const {
        return num_2 < *this;
    }

    bool operator == (const BigInteger& num_2) const {
        return (!(*this < num_2) && !(num_2 < *this));
    }

    bool operator != (const BigInteger& num_2) const {
        return !(*this == num_2);
    }

    bool operator <= (const BigInteger& num_2) const {
        return (*this < num_2) || (*this == num_2);
    }

    bool operator >= (const BigInteger& num_2) const {
        return num_2 <= *this;
    }

    explicit operator bool() const{
        BigInteger zero(0);
        return (*this != zero);
    }
    explicit operator int() const {
        int num = 0;
        int step = 1;
        for (int a: numbers) {
            num += a * step;
            step *= BASE;
        }
        num *= sign;
        return num;
    }
    explicit operator long() const {
        long num = 0;
        long step = 1;
        for (long a: numbers) {
            num += a * step;
            step *= BASE;
        }
        num *= sign;
        return num;
    }

    friend std::ostream& operator << (std::ostream& out_stream, const BigInteger& num);
    friend std::istream& operator >> (std::istream& in_stream, BigInteger& num);

private:
    vector<long long> numbers;
    int sign = 0;

    static const int BASE = 1000000000;
    static const int BASE_LENGTH = 9;

    static void add_abs(vector<long long>& new_num, const vector<long long>& left, const vector<long long>& right) {
        //new_num = |left| + |right|
        long long add_to_next = 0;
        size_t i = 0;
        while (i < left.size() || i < right.size() || add_to_next > 0) {
            long long x = (i < left.size() ? left[i] : 0) + (i < right.size() ? right[i] : 0) + add_to_next;
            add_to_next = x / BASE;
            if (i >= new_num.size()) {
                new_num.push_back(0);
            }
            new_num[i] = x % BASE;
            ++i;
        }
    }

    static void subtract_abs(vector<long long>& new_num, const vector<long long>& left, const vector<long long>& right) {
        //new_num = |left| - |right|, |left| >= |right|
        long long take_from_next = 0;
        size_t i = 0;
        while (i < left.size() || take_from_next > 0) {
            long long x = left[i] - (i < right.size() ? right[i] : 0) - take_from_next;
            take_from_next = (x < 0 ? 1 : 0);
            if (i >= new_num.size()) {
                new_num.push_back(0);
            }
            new_num[i] = x + take_from_next * BASE;
            ++i;
        }
    }

    static void multiply_abs(vector<long long>& new_num, const vector<long long>& left, const vector<long long>& right) {
        long long add_to_next = 0;
        vector<long long> sub_result(new_num.size(), 0);
        for (size_t i = 0; i < right.size(); ++i) {
            size_t j = 0;
            while (j < left.size() || add_to_next > 0) {
                long long x = (j < left.size() ? left[j] : 0) * right[i] + add_to_next;

                add_to_next = x / BASE;
                if (i + j >= sub_result.size()) {
                    sub_result.push_back(0);
                }
                sub_result[i + j] += x % BASE;
                add_to_next += sub_result[i + j] / BASE;
                sub_result[i + j] %= BASE;
                ++j;
            }
        }
        new_num = sub_result;
    }

    static void multiply_abs(vector<long long>& new_num, const vector<long long>& left, long long right) {
        long long add_to_next = 0;

        size_t j = 0;
        while (j < left.size() || add_to_next > 0) {
            long long x = (j < left.size() ? left[j] : 0) * right + add_to_next;
            add_to_next = x / BASE;
            if (j >= new_num.size()) {
                new_num.push_back(0);
            }
            new_num[j] += x % BASE;
            ++j;
        }
    }

    static long long find_quotient(const vector<long long>& dividend, const vector<long long>& divider, long long min_quotient,
                             long long max_quotient, vector<long long>& rest) {
        vector<long long> divider_by_median;
        if (max_quotient - min_quotient == 1) {
            multiply_abs(divider_by_median, divider, min_quotient);
            subtract_abs(rest, dividend, divider_by_median);
            delete_nulls(rest);
            return min_quotient;
        } else {
            long long median_quotient = (min_quotient + max_quotient) / 2;
            multiply_abs(divider_by_median, divider, median_quotient);
            if (cmp_value_abs(dividend, divider_by_median)) {
                return find_quotient(dividend, divider, min_quotient, median_quotient, rest);
            } else if (cmp_value_abs(divider_by_median, dividend)) {
                return find_quotient(dividend, divider, median_quotient, max_quotient, rest);
            } else {
                rest = {0};
                return median_quotient;
            }
        }
    }

    static void divide_abs(vector<long long>& new_num, const vector<long long>& left, const vector<long long>& right, vector<long long>& rest) {
        vector<long long> result_num{0};
        rest = left;
        if (left.size() >= right.size()) {
            size_t start_ind_dividend = left.size() - right.size() + 1;
            size_t end_ind_dividend = left.size();
            vector<long long> sub_num = vector<long long>(left.begin() + long(start_ind_dividend - 1),
                                              left.begin() + long(end_ind_dividend));
            if (cmp_value_abs(sub_num, right)) {
                --start_ind_dividend;
                if (start_ind_dividend != 0) {
                    sub_num.insert(sub_num.begin(), left[start_ind_dividend - 1]);
                }
            }
            size_t len_result_num = left.size() - (end_ind_dividend - start_ind_dividend);
            if (len_result_num != 0) {
                result_num = vector<long long>(len_result_num, 0);
            }
            while (start_ind_dividend > 0) {
                delete_nulls(sub_num);
                if (!cmp_value_abs(sub_num, right)) {
                    rest.clear();
                    result_num[start_ind_dividend - 1] = find_quotient(sub_num, right, 1, BASE, rest);
                    sub_num.clear();
                    sub_num.insert(sub_num.begin(), rest.begin(), rest.end());
                }
                --start_ind_dividend;
                if (start_ind_dividend > 0) {
                    sub_num.insert(sub_num.begin(), left[start_ind_dividend - 1]);
                }
            }
        }
        new_num = result_num;
    }


    static int make_sign(long long num) {
        if (num > 0) return 1;
        else if (num < 0) return -1;
        else return 0;
    }

    static bool cmp_value(const BigInteger& left, const BigInteger& right) {
        if (left.sign != right.sign) {
            return left.sign < right.sign;
        }
        bool both_are_negative = (left.sign == -1);
        return both_are_negative != cmp_value_abs(left, right);
    }

    static bool cmp_value_abs(const BigInteger& left, const BigInteger& right) {
        size_t l_left = left.numbers.size();
        size_t l_right = right.numbers.size();
        if (l_left != l_right) {
            return l_left < l_right;
        }
        for (size_t ind = 1; ind <= l_left; ++ind) {
            if (left.numbers[l_left - ind] != right.numbers[l_right - ind]) {
                return (left.numbers[l_left - ind] < right.numbers[l_right - ind]);
            }
        }
        return false;
    }

    static bool cmp_value_abs(const vector<long long>& left, const vector<long long>& right) {
        size_t l_left = left.size();
        size_t l_right = right.size();
        if (l_left != l_right) {
            return l_left < l_right;
        }
        for (size_t ind = 1; ind <= l_left; ++ind) {
            if (left[l_left - ind] != right[l_right - ind]) {
                return (left[l_left - ind] < right[l_right - ind]);
            }
        }
        return false;
    }

    static const BigInteger& max(const BigInteger& left, const BigInteger& right,
                                 bool cmp(const BigInteger&, const BigInteger&)) {
        if (cmp(left, right)) {
            return right;
        }
        return left;
    }

    static const BigInteger& min(const BigInteger& left, const BigInteger& right,
                                 bool cmp(const BigInteger&, const BigInteger&)) {
        if (cmp(left, right)) {
            return left;
        }
        return right;
    }

    static void delete_nulls(vector<long long>& num) {
        while (num.back() == 0 && num.size() > 1) {
            num.pop_back();
        }
    }


    BigInteger& do_algebra_sum(const BigInteger& num_2, int op) {
        if (sign == op * num_2.sign) {
            add_abs(numbers, numbers, num_2.numbers);
        } else {
            const BigInteger& max_num = max(*this, num_2, cmp_value_abs);
            const BigInteger& min_num = min(*this, num_2, cmp_value_abs);
            if (op == -1) {
                sign = (&max_num == &num_2 ? -num_2.sign : sign);
            } else {
                sign = max_num.sign;
            }
            subtract_abs(numbers, max_num.numbers, min_num.numbers);
            delete_nulls(numbers);
            if (numbers.back() == 0) {
                sign = 0;
            }
        }
        return *this;
    }

    BigInteger& do_multiplication(const BigInteger& num_2) {
        this->sign *= num_2.sign;
        multiply_abs(numbers, numbers, num_2.numbers);
        delete_nulls(numbers);
        return *this;
    }

    BigInteger& do_division(const BigInteger& num_2) {
        this->sign *= num_2.sign;
        vector<long long> rest{0};
        divide_abs(numbers, numbers, num_2.numbers, rest);
        delete_nulls(numbers);
        if (numbers.back() == 0) {
            sign = 0;
        }
        return *this;
    }

    BigInteger& do_modular_reward(const BigInteger& num_2) {
        vector<long long>rest{0};
        divide_abs(numbers, numbers, num_2.numbers, rest);
        numbers = rest;
        if (rest.size() == 1 && rest[0] == 0) {
            sign = 0;
        }
        return *this;
    }
};

BigInteger operator + (const BigInteger& num_1, const BigInteger& num_2) {
    BigInteger new_num = num_1;
    new_num += num_2;
    return new_num;
}

BigInteger operator - (const BigInteger& num_1, const BigInteger& num_2) {
    BigInteger new_num = num_1;
    new_num -= num_2;
    return new_num;
}

BigInteger operator * (const BigInteger& num_1, const BigInteger& num_2) {
    BigInteger new_num = num_1;
    new_num *= num_2;
    return new_num;
}

BigInteger operator / (const BigInteger& num_1, const BigInteger& num_2) {
    BigInteger new_num = num_1;
    new_num /= num_2;
    return new_num;
}

BigInteger operator % (const BigInteger& num_1, const BigInteger& num_2) {
    BigInteger new_num = num_1;
    new_num %= num_2;
    return new_num;
}

std::ostream& operator << (std::ostream& out_stream, const BigInteger& num) {
    out_stream << num.toString();
    return out_stream;
}

std::istream& operator >> (std::istream& in_stream, BigInteger& num) {
    num.numbers.clear();
    std::string num_str;
    in_stream >> num_str;

    if (num_str.empty()) {
        num.sign = 0;
    } else {
        if (num_str[0] == '-') {
            num.sign = -1;
            num_str = std::string(num_str.begin() + 1, num_str.end());
        } else {
            num.sign = 1;
        }
        size_t len = num_str.size();
        size_t i = 1;
        while ((i - 1) * BigInteger::BASE_LENGTH < len) {
            size_t j = len > i * BigInteger::BASE_LENGTH ? len - i * BigInteger::BASE_LENGTH : 0;
            std::string x_ = std::string(num_str.begin() + long(j),
                                         num_str.begin() + long(len - (i - 1) * BigInteger::BASE_LENGTH));
            long long x = std::stoi(x_);
            num.numbers.push_back(x);
            ++i;
        }
        BigInteger::delete_nulls(num.numbers);
        if (num.numbers.back() == 0) {
            num.sign = 0;
        }
    }
    return in_stream;
}


class Rational {
public:
    Rational(const BigInteger& rat) {
        numerator = rat;
        denominator = BigInteger(1);
    }
    Rational(int rat) {
        numerator = BigInteger(rat);
        denominator = BigInteger(1);
    }
    Rational(const BigInteger& num, const BigInteger& den) {
        if (den < BigInteger(0)) {
            denominator = -den;
            numerator = - num;
        } else if (den > BigInteger(0)) {
            numerator = num;
            denominator = den;
        } else {
            numerator = 0;
            denominator = 1;
        }
    }

    Rational(int num, int den) {
        numerator = BigInteger(num);
        denominator = BigInteger(den);
    }

    Rational() {
        numerator = BigInteger(0);
        denominator = BigInteger(1);
    }

    ~Rational() = default;

    Rational& operator = (const Rational& rat_num) {
        numerator.clear();
        denominator.clear();
        numerator = rat_num.numerator;
        denominator = rat_num.denominator;
        return *this;
    }
    
    Rational& do_right_rational() {
        BigInteger nod = euclidean_algorithm((numerator > BigInteger(0) ? numerator  : -numerator), denominator);
        numerator /= nod;
        denominator /= nod;
        return *this;
    }

    Rational& operator += (const Rational& rat_num) {
        return do_rational_sum(rat_num, 1);
    }

    Rational& operator -= (const Rational& rat_num) {
        return do_rational_sum(rat_num, -1);
    }

    Rational& operator *= (const Rational& rat_num) {
        return do_rational_multiplication(rat_num);
    }

    Rational& operator /= (const Rational& rat_num) {
        return do_rational_division(rat_num);
    }

    Rational operator - () {
        Rational new_rat(-numerator, denominator);
        return new_rat;
    }

    bool operator < (const Rational& rat) const {
        if (denominator == rat.denominator) {
            return (numerator < rat.numerator);
        } else {
            return ((numerator * rat.denominator) < (rat.numerator * denominator));
        }
    }

    bool operator > (const Rational& rat) const {
        return rat < *this;
    }

    bool operator == (const Rational& rat) const {
        return (!(*this < rat) && !(*this > rat));
    }

    bool operator != (const Rational& rat) const {
        return !(*this == rat);
    }

    bool operator <= (const Rational& rat) const {
        return (*this < rat || *this == rat);
    }

    bool operator >= (const Rational& rat) const {
        return (*this > rat || *this == rat);
    }

    explicit operator double() const {
        double ran = 0;
        BigInteger int_part = numerator / denominator;
        BigInteger float_part = (numerator - int_part * denominator) * BigInteger(BASE_SQR) * BigInteger(BASE_SQR);
        float_part /= denominator;
        ran += long(int_part);

        return ran;
    }

    string toString() const{
        string str_rat;
        str_rat = numerator.toString();
        if (denominator != BigInteger(1)) {
            str_rat += '/';
            str_rat += denominator.toString();
        }
        return str_rat;
    }

    string asDecimal(size_t precision=0) {
        string str_decimal;


        BigInteger int_part = (numerator > BigInteger(0) ? numerator : -numerator) / denominator;


        BigInteger float_part = ((numerator > BigInteger(0) ? numerator : -numerator)
                - int_part * denominator) * BigInteger(10);
        for (size_t i = 0; i < precision; ++i) {
            float_part *= BigInteger(10);
        }

        float_part /= denominator;
        if ((float_part % BigInteger(10)) >= BigInteger(5)) {
            if (precision == 0) {
                int_part += BigInteger(1);
            } else {
                float_part += BigInteger(10);
            }
        }
        if ((numerator < BigInteger(0)) && ((int_part != BigInteger(0)) || (float_part != BigInteger(0)))) {
            str_decimal.push_back('-');
        }
        str_decimal += int_part.toString();
        if (precision != 0) {
            str_decimal += '.';
            string str_float_part = (float_part >  BigInteger(0) ? float_part : -float_part).toString();
            str_float_part.pop_back();
            if (str_float_part.size() < precision) {
                string nulls_to_add_forward(precision - str_float_part.size(), '0');
                str_decimal += nulls_to_add_forward;
            }
            str_decimal += str_float_part;
        }

        return str_decimal;
    }
private:
    BigInteger numerator;
    BigInteger denominator;

    static const int BASE_SQR = 100000000;

    Rational& do_rational_sum(const Rational& rat, int op) {
        if (denominator != rat.denominator) {
            numerator *= rat.denominator;
            if (op == 1) {
                numerator += rat.numerator * denominator;
            } else {
                numerator -= rat.numerator * denominator;
            }
            denominator *= rat.denominator;
        } else {
            if (op == 1) {
                numerator += rat.numerator;
            } else {
                numerator -= rat.numerator;
            }
        }
        if (numerator == BigInteger(0)) {
            denominator = BigInteger(1);
        }

        if (denominator < BigInteger(0)) {
            denominator *= BigInteger(-1);
            numerator *= BigInteger(-1);
        }

        do_right_rational();
        return *this;
    }

    Rational& do_rational_multiplication(const Rational& rat) {
        numerator *= rat.numerator;
        denominator *= rat.denominator;

        if (denominator < BigInteger(0)) {
            denominator *= BigInteger(-1);
            numerator *= BigInteger(-1);
        }
        do_right_rational();
        return *this;
    }

    Rational& do_rational_division(const Rational& rat) {
        numerator *= rat.denominator;
        denominator *= (rat.numerator > BigInteger(0) ? rat.numerator : -rat.numerator);
        numerator *= (rat.numerator > BigInteger(0) ? BigInteger(1) : BigInteger(-1));
        do_right_rational();
        return *this;
    }

    static BigInteger euclidean_algorithm(const BigInteger& a, const BigInteger& b) {
        if (a % b == BigInteger(0)) {
            return b;
        }
        return euclidean_algorithm(b, a % b);
    }

    friend std::ostream& operator << (std::ostream& out_stream, const Rational& rat);
};

std::ostream& operator << (std::ostream& out_stream, const Rational& rat) {
    string str_rat = rat.toString();
    out_stream << str_rat;
    return  out_stream;
}


Rational operator + (const Rational& rat_1, const Rational& rat) {
    Rational new_rat = rat_1;
    new_rat += rat;
    return new_rat;
}

Rational operator - (const Rational& rat_1, const Rational& rat) {
    Rational new_rat = rat_1;
    new_rat -= rat;
    return new_rat;
}

Rational operator * (const Rational& rat_1, const Rational& rat) {
    Rational new_rat = rat_1;
    new_rat *= rat;
    return new_rat;
}

Rational operator / (const Rational& rat_1, const Rational& rat) {
    Rational new_rat = rat_1;
    new_rat /= rat;
    return new_rat;
}

#endif
