#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using std::vector;
using std::string;

class BigInteger {
public:
    BigInteger() = default;
    BigInteger(BigInteger const &) = default;
    BigInteger(long long number);

    explicit operator bool() const;

    friend bool operator >(const BigInteger& first, const BigInteger& second);

    const BigInteger operator ++(int);
    const BigInteger operator --(int);
    BigInteger& operator ++();
    BigInteger& operator --();
    BigInteger operator -() const;

    BigInteger& operator +=(const BigInteger& number);
    BigInteger& operator -=(const BigInteger& number);
    BigInteger& operator *=(const BigInteger& number);
    BigInteger& operator /=(const BigInteger& number);
    BigInteger& operator %=(const BigInteger& number);

    friend std::istream& operator >> (std::istream& in, BigInteger& number);

    [[nodiscard]] string toString() const;
private:
    static const long long base_ = 1e7;
    vector<long long> digits_;//числа в обратном порядке для упрощения операций
    int sign_ = 1;

    void normalize(bool full = true);
    void delete_zeros();
};
bool operator ==(const BigInteger& first, const BigInteger& second);
bool operator !=(const BigInteger& first, const BigInteger& second);
bool operator <(const BigInteger& first, const BigInteger& second);
bool operator <=(const BigInteger& first, const BigInteger& second);
bool operator >=(const BigInteger& first, const BigInteger& second);

BigInteger operator +(const BigInteger& first, const BigInteger& second);
BigInteger operator -(const BigInteger& first, const BigInteger& second);
BigInteger operator *(const BigInteger& first, const BigInteger& second);
BigInteger operator /(const BigInteger& first, const BigInteger& second);
BigInteger operator %(const BigInteger& first, const BigInteger& second);

std::ostream& operator << (std::ostream& out, const BigInteger& number);

BigInteger::BigInteger(long long number) {
    sign_ = 2 * static_cast<int>(number >= 0) - 1;
    if (number == 0) {
        digits_.resize(1);
    }
    number *= sign_;
    while (number) {
        digits_.push_back(number % base_);
        number /= base_;
    }
}

BigInteger::operator bool() const {
    return *this > 0;
}

bool operator >(const BigInteger& first, const BigInteger& second) {
    if (first.sign_ + second.sign_ == 0) { //only one is positive
        return (first.sign_ == 1);
    }
    if (first.digits_.size() != second.digits_.size()) {
        return (first.digits_.size() > second.digits_.size()) && (first.sign_ == 1);
    }
    for (int i = first.digits_.size()-1; i >= 0; --i) {
        if (first.digits_[i] != second.digits_[i]) {
            return (first.digits_[i] > second.digits_[i]) && (first.sign_ == 1);
        }
    }
    return false;
}

bool operator ==(const BigInteger& first, const BigInteger& second) {
    return !(first > second) && !(second > first);
}

bool operator !=(const BigInteger& first, const BigInteger& second) {
    return !(first == second);
}

bool operator >=(const BigInteger& first, const BigInteger& second) {
    return (first > second) || (first == second);
}

bool operator <(const BigInteger& first, const BigInteger& second) {
    return !(first > second) && (first != second);
}

bool operator <=(const BigInteger& first, const BigInteger& second) {
    return !(first > second);
}

BigInteger& BigInteger::operator ++() {
    digits_[0] += sign_;
    normalize(false);
    return *this;
}

BigInteger& BigInteger::operator --() {
    digits_[0] -= sign_;
    normalize(false);
    return *this;
}

const BigInteger BigInteger::operator ++(int) {
    BigInteger copy = *this;
    digits_[0] += sign_ * 1;
    normalize(false);
    return copy;
}

const BigInteger BigInteger::operator --(int) {
    BigInteger copy = *this;
    digits_[0] -= sign_ * 1;
    normalize(false);
    return copy;
}

BigInteger BigInteger::operator -() const {
    BigInteger copy = *this;
    copy.sign_ *= (copy != 0) ? -1 : 1;
    return copy;
}

BigInteger& BigInteger::operator +=(const BigInteger& number) {
    size_t i = 0;
    int sign = static_cast<int>(sign_ == number.sign_) * 2 - 1;
    for ( ; i < std::min(digits_.size(), number.digits_.size()); ++i) {
        digits_[i] += sign * number.digits_[i];
    }
    if (digits_.size() < number.digits_.size()) {
        for ( ; i < number.digits_.size(); ++i) {
            digits_.push_back(sign * number.digits_[i]);
        }
    }
    normalize();
    return *this;
}

BigInteger& BigInteger::operator -=(const BigInteger& number) {
    *this += -number;
    return *this;
}

BigInteger& BigInteger::operator *=(const BigInteger& number) {
    BigInteger result;
    result.digits_.resize(digits_.size() + number.digits_.size());
    result.sign_ = static_cast<int>(sign_ == number.sign_) * 2 - 1;
    long long rank = 0;
    size_t offset = 0;
    for (size_t i = 0; i < number.digits_.size(); ++i, ++offset) {
        if (number.digits_[i] == 0) {
            continue;
        }
        for (size_t j = 0; j < digits_.size(); ++j) {
            result.digits_[j + offset] += digits_[j] * number.digits_[i];
        }
        rank = result.digits_[offset] / base_;
        result.digits_[offset] %= base_;
        result.digits_[offset + 1] += rank;
    }
    for ( ; offset < result.digits_.size() - 1; ++offset) {
        rank = result.digits_[offset] / base_;
        result.digits_[offset] %= base_;
        result.digits_[offset + 1] += rank;
    }
    result.delete_zeros();
    return *this = result;
}

BigInteger& BigInteger::operator /=(const BigInteger& number) {
    if ((number*number.sign_) > (*this * sign_) || digits_.empty()) {
        return *this = 0;
    }
    BigInteger result(0);
    BigInteger temp(0);
    for (int i = digits_.size()-1; i >= 0; --i) {
        temp *= base_;
        temp += digits_[i];
        result *= base_;
        //BinSearch
        long long left = 0, right = base_, middle = (left+right) / 2;
        while(true) {
            if(temp <  number * (number.sign_ * middle)) {
                right = middle;
            } else {
                left = middle;
            }
            if(right - left == 1){
                break;
            }
            middle = (right + left) / 2;
        }
        result += left;
        temp -= number * (number.sign_ * left);
    }
    result.sign_ = static_cast<int>(sign_ == number.sign_) * 2 - 1;
    *this = result;
    return *this;
}

BigInteger& BigInteger::operator %=(const BigInteger& number) {
    return *this -= (*this / number) * number;
}

BigInteger operator +(const BigInteger& first, const BigInteger& second) {
    BigInteger copy = first;
    return copy += second;
}

BigInteger operator -(const BigInteger& first, const BigInteger& second) {
    BigInteger copy = first;
    return copy -= second;
}

BigInteger operator *(const BigInteger& first, const BigInteger& second) {
    BigInteger copy = first;
    return copy *= second;
}

BigInteger operator /(const BigInteger& first, const BigInteger& second) {
    BigInteger copy = first;
    return copy /= second;
}

BigInteger operator %(const BigInteger& first, const BigInteger& second) {
    BigInteger copy = first;
    return copy %= second;
}

std::istream& operator >> (std::istream& in, BigInteger& number) {
    string str;
    in >> str;
    number.digits_.clear();
    number.sign_ = 1;
    bool negative = false;
    long long num_digit = 1;
    long long digit = 0;
    int i = str.size();
    while (--i >= 0) {
        if (str[i] == '-') {
            negative = true;
        } else if (num_digit < BigInteger::base_) {
            digit += num_digit * (str[i] - '0');
            num_digit *= 10;
        } else {
            number.digits_.push_back(digit);
            digit = (str[i] - '0');
            num_digit = 10;
        }
    }
    number.digits_.push_back(digit);
    if (negative) {
        number.sign_ = -1;
    }
    return in;
}

std::ostream& operator << (std::ostream& out, const BigInteger& number) {
    string str = number.toString();
    out << str;
    return out;
}

[[nodiscard]] string BigInteger::toString() const {
    string str;
    size_t i = 0;
    while (i < digits_.size()) {
        string temp = std::to_string(digits_[i++]);
        std::reverse(temp.begin(), temp.end());
        str += temp;
        while (i != digits_.size() && str.size() % 7 != 0) {
            str += "0";
        }
    }
    if (sign_ == -1) {
        str += "-";
    }
    std::reverse(str.begin(), str.end());
    return str;
}

void BigInteger::normalize(bool full) {
    for (size_t i = 0; i < digits_.size()-1; ++i) {
        if (digits_[i] < 0) {
            long long rank = -1 * (digits_[i] + 1) / base_ + 1;
            digits_[i] += rank * base_;
            digits_[i+1] -= rank;
        } else if (digits_[i] >= base_) {
            long long rank = digits_[i] / base_;
            digits_[i] %= base_;
            digits_[i+1] += rank;
        } else if (!full) {
            return;
        }
    }
    size_t l = digits_.size()-1;
    if (digits_[l] < 0) {
        sign_ *= -1;
        for (long long & digit : digits_) {
            digit *= -1;
        }
        normalize();
    }
    if (digits_[l] >= base_) {
        long long rank = digits_[l] / base_;
        digits_[l] %= base_;
        digits_.push_back(rank);
        normalize();
    }
    delete_zeros();
}

void BigInteger::delete_zeros() {
    size_t l = digits_.size()-1;
    while (digits_[l] == 0 && l-- >= 1) {
        digits_.pop_back();
    }
}



class Rational {
public:
    Rational() = default;
    Rational(int number);
    Rational(const BigInteger& number);
    Rational(const BigInteger& p, const BigInteger& q);

    explicit operator double() const;

    friend bool operator >(const Rational& first, const Rational& second);

    Rational operator -() const;
    Rational& operator +=(const Rational& number);
    Rational& operator -=(const Rational& number);
    Rational& operator *=(const Rational& number);
    Rational& operator /=(const Rational& number);


    [[nodiscard]] string toString() const;
    [[nodiscard]] string asDecimal(size_t precision = 0) const;

private:
    mutable BigInteger p_ = 0;
    mutable BigInteger q_ = 1;
    mutable int sign_ = 1;
    void reduce() const;
    static BigInteger gcd(BigInteger first, BigInteger second);
};

bool operator ==(const Rational& first, const Rational& second);
bool operator !=(const Rational& first, const Rational& second);
bool operator <(const Rational& first, const Rational& second);
bool operator <=(const Rational& first, const Rational& second);
bool operator >=(const Rational& first, const Rational& second);

Rational operator +(const Rational& first, const Rational& second);
Rational operator -(const Rational& first, const Rational& second);
Rational operator *(const Rational& first, const Rational& second);
Rational operator /(const Rational& first, const Rational& second);

Rational::Rational(int number) : p_(BigInteger(number)), q_(1), sign_(1) {
    sign_ = ((p_ >= 0) xor (q_ > 0)) ? -1 : 1;
    p_ *= (p_ < 0) ? -1 : 1;
    q_ *= (q_ < 0) ? -1 : 1;
    //reduce();
}

Rational::Rational(const BigInteger& number) : p_(number), q_(1) {
    sign_ = ((p_ >= 0) xor (q_ > 0)) ? -1 : 1;
    p_ *= (p_ < 0) ? -1 : 1;
    q_ *= (q_ < 0) ? -1 : 1;
    //reduce();
}

Rational::Rational(const BigInteger& p, const BigInteger& q) : p_(p), q_(q) {
    sign_ = ((p_ >= 0) xor (q_ > 0)) ? -1 : 1;
    p_ *= (p_ < 0) ? -1 : 1;
    q_ *= (q_ < 0) ? -1 : 1;
    //reduce();
}

Rational::operator double() const {
    return std::stof(asDecimal(25));
}

bool operator >(const Rational& first, const Rational& second) {
    if (first.sign_ + second.sign_ == 0) { //only one is positive
        return (first.sign_ == 1);
    }
    return (first.p_ * second.q_ > second.p_ * first.q_) && (first.sign_ == 1);
}

bool operator ==(const Rational& first, const Rational& second) {
    return !(first > second) && !(second > first);
}

bool operator !=(const Rational& first, const Rational& second) {
    return !(first == second);
}

bool operator >=(const Rational& first, const Rational& second) {
    return (first > second) || (first == second);
}

bool operator <(const Rational& first, const Rational& second) {
    return !(first > second) && (first != second);
}

bool operator <=(const Rational& first, const Rational& second) {
    return !(first > second);
}

Rational Rational::operator -() const {
    Rational copy = *this;
    copy.sign_ *= (copy != 0) ? -1 : 1;
    return copy;
}

Rational& Rational::operator +=(const Rational& number) {
    int sign = static_cast<int>(sign_ == number.sign_) * 2 - 1;
    if (sign == 1) {
        p_ = p_ * number.q_ + number.p_ * q_;
    } else {
        p_ = p_ * number.q_ - number.p_ * q_;
    }
    q_ *= number.q_;
    sign_ = ((p_ >= 0) xor (q_ > 0)) ? -1 : 1;
    p_ *= (p_ < 0) ? -1 : 1;
    q_ *= (q_ < 0) ? -1 : 1;
    //reduce();
    return *this;
}

Rational& Rational::operator -=(const Rational& number) {
    return *this += -number;
}

Rational& Rational::operator *=(const Rational& number) {
    sign_ = static_cast<int>(sign_ == number.sign_) * 2 - 1;
    p_ = p_ * number.p_;
    q_ = q_ * number.q_;
    sign_ = (p_ == 0) ? 1 : sign_;
    //reduce();
    return *this;
}

Rational& Rational::operator /=(const Rational& number) {
    sign_ = static_cast<int>(sign_ == number.sign_) * 2 - 1;
    return *this *= Rational(number.q_, number.p_);
}

Rational operator +(const Rational& first, const Rational& second) {
    Rational copy = first;
    return copy += second;
}

Rational operator -(const Rational& first, const Rational& second) {
    Rational copy = first;
    return copy -= second;
}

Rational operator *(const Rational& first, const Rational& second) {
    Rational copy = first;
    return copy *= second;
}

Rational operator /(const Rational& first, const Rational& second) {
    Rational copy = first;
    return copy /= second;
}

[[nodiscard]] string Rational::toString() const {
    reduce();
    string str;
    if (sign_ == -1) {
        str = "-";
    }
    str += p_.toString();
    if (q_ != 1) {
        str+= "/" + q_.toString();
    }
    return str;
}

[[nodiscard]] string Rational::asDecimal(size_t precision) const {
    string result;
    if (sign_ == -1) {
        result = "-";
    }
    BigInteger pow = 1;
    for (size_t i = 0; i < precision; ++i) {
        pow *= 10;
    }
    string source = ((p_ * pow) / q_).toString();
    if (source.length() <= precision) {
        result += "0.";
        for (size_t i = 0; i < precision - source.length(); ++i) {
            result += "0";
        }
        result += source;
    } else {
        result += source.substr(0, source.length()-precision) + "." +
                  source.substr(source.length()-precision, precision);
    }
    return result;
}

void Rational::reduce() const {
    if (p_ < 0) {
        p_ *= -1;
        sign_ *= -1;
    }
    if (p_ == 0) {
        sign_ = 1;
    }
    if (q_ < 0) {
        q_ *= -1;
        sign_ *= -1;
    }
    BigInteger del = gcd(p_, q_);
    p_ /= del;
    q_ /= del;
}

BigInteger Rational::gcd(BigInteger first, BigInteger second) {
    while (first != second) {
        if (first == 0) {
            return second;
        } else if (second == 0) {
            return first;
        } else if (first > second) {
            first %= second;
        } else {
            second %= first;
        }
    }
    return first;
}
