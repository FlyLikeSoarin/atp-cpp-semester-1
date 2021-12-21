#ifndef UNTITLED_BIGINTEGER_H
#define UNTITLED_BIGINTEGER_H

#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;
using std::cout;
using std::cin;
using std::ostream;
using std::istream;

int intSize(int num) {
    return (int)std::to_string(num).size();
}

class BigInteger {
private:
    vector<int> number;
    bool sign = true;


public:
    const int BASE = 1e4;

    BigInteger() = default;

    BigInteger(const BigInteger& bigint) = default;

    BigInteger(const int num) {
        number.clear();
        if (num == 0) {
            number.push_back(0);
            sign = true;
        } else {
            int num2 = num;
            if (num2 < 0) {
                num2 *= -1;
                sign = false;
            } else {
                sign = true;
            }

            while (num2 > 0) {
                number.push_back(num2 % BASE);
                num2 /= BASE;
            }
        }
    }

    ~BigInteger() = default;

    BigInteger& operator=(const BigInteger& bigint) {
        if (this != &bigint) {
            sign = bigint.sign;
            number = bigint.number;
        }
        return *this;
    }

    BigInteger& operator=(const int num) {
        BigInteger bigint = num;
        *this = bigint;
        return *this;
    }

    BigInteger operator-() {
        BigInteger result = *this;
        if (result != 0) result.sign = !result.sign;
        return result;
    }

    BigInteger& operator+=(const BigInteger& bigint) {
        if ((sign && bigint.sign) || (!sign && !bigint.sign)) {
            size_t size = bigint.number.size();
            while (number.size() < size) {
                number.push_back(0);
            }
            for (size_t i = 0; i < size; ++i) {
                number[i] += bigint.number[i];
            }
            for (size_t i = 0; i < number.size(); ++i) {
                if (number[i] >= BASE) {
                    number[i] -= BASE;
                    if (i == number.size() - 1) number.push_back(0);
                    number[i + 1]++;
                }
            }
            update(*this);
            return *this;
        } else if (sign && !bigint.sign) {
            BigInteger copy = bigint;
            copy.sign = !copy.sign;
            *this -= copy;
            update(*this);
            return *this;
        } else {
            BigInteger result = bigint;
            sign = true;
            result -= *this;
            *this = result;
            update(*this);
            return *this;
        }
    }

    BigInteger& operator-=(const BigInteger& bigint) {
        if ((!sign && bigint.sign) || (sign && !bigint.sign)) {
            BigInteger copy = bigint;
            copy.sign = !copy.sign;
            *this += copy;
            update(*this);
            return *this;
        } else if ((*this > bigint && sign && bigint.sign) || (*this < bigint && !sign && !bigint.sign)) {
            size_t size = bigint.number.size();
            for (size_t i = 0; i < size; ++i) {
                number[i] -= bigint.number[i];
            }
            for (size_t i = 0; i < number.size(); ++i) {
                if (number[i] < 0) {
                    number[i] += BASE;
                    if (i == number.size() - 1) number.push_back(BASE);
                    number[i + 1]--;
                }
            }
            update(*this);
            return *this;
        } else {
            BigInteger other = bigint;
            for (size_t i = 0; i < number.size(); ++i) {
                other.number[i] -= number[i];
            }
            for (size_t i = 0; i < other.number.size(); ++i) {
                if (other.number[i] < 0) {
                    other.number[i] += BASE;
                    if (i == other.number.size() - 1) other.number.push_back(BASE);
                    other.number[i + 1]--;
                }
            }
            *this = other;
            if (*this != 0) {
                sign = !sign;
            } else {
                sign = true;
            }
            update(*this);
            return *this;
        }
    }

    BigInteger& operator+=(const int num) {
        BigInteger bigint = num;
        *this += bigint;
        return *this;
    }

    BigInteger& operator-=(const int num) {
        BigInteger bigint = num;
        *this -= bigint;
        return *this;
    }

    BigInteger& operator*=(const BigInteger& bigint) {
        BigInteger result;
        result.number.assign(number.size() + bigint.number.size() + 2, 0);
        int carry = 0;
        size_t i = 0;
        while (i < number.size()) {
            size_t j = 0;
            while (j < bigint.number.size() || carry) {
                int current;
                if (j < bigint.number.size()) {
                    current = result.number[i + j] + number[i] * bigint.number[j] + carry;
                } else {
                    current = result.number[i + j] + carry;
                }
                result.number[i + j] = current % BASE;
                carry = current / BASE;
                ++j;
            }
            ++i;
        }
        update(result);
        result.sign = ((sign + bigint.sign) % 2 == 0);
        if (result.number.size() == 1 && result.number[0] == 0) sign = true;
        *this = result;
        return *this;
    }

    BigInteger& operator*=(const int num) {
        BigInteger bigint = num;
        *this *= bigint;
        return *this;
    }

    BigInteger& operator/=(const BigInteger& bigint) {
        if (number.size() == 1 && number[0] == 0) {
            sign = true;
            return *this;
        }
        BigInteger denominator = bigint;
        if (denominator == 1) {
            return *this;
        } else if (denominator == -1) {
            sign = !sign;
            return *this;
        }
        BigInteger result;
        result.number.assign(number.size() + 1, 0);
        result.sign = ((sign + bigint.sign) % 2 == 0);

        sign = true;
        denominator.sign = true;

        if(number.size() == 0) {
            return *this;
        }
        if (*this < denominator) {
            *this = 0;
            sign = true;
            return *this;
        }

        BigInteger current = 0;
        for (int i = (int)number.size() - 1; i >= 0; --i) {
            bool flag = false;
            while (i >= 0 && current < denominator) {
                current *= BASE;
                current.number[0] = number[i];
                --i;
                flag = true;
            }
            if (flag) ++i;

            int left = 0, right = BASE;
            while (right - left > 1) {
                int mid = (right + left) >> 1;
                BigInteger product = denominator * mid;
                if (product <= current) {
                    left = mid;
                } else {
                    right = mid;
                }
            }
            result.number[i] = left;
            current -= left * denominator;
            update(current);
        }

        *this = result;
        update(*this);
        if (number.size() == 1 && number[0] == 0) {
            sign = true;
        }
        return *this;
    }

    BigInteger& operator/=(const int num) {
        BigInteger bigint = num;
        *this /= bigint;
        return *this;
    }

    BigInteger& operator%=(const BigInteger& bigint) {
        BigInteger quotient = *this / bigint;
        *this -= quotient * bigint;
        update(*this);
        return *this;
    }

    BigInteger& operator%=(const int num) {
        BigInteger bigint = num;
        *this %= bigint;
        return *this;
    }

    BigInteger& operator++() {
        *this += 1;
        return *this;
    }
    BigInteger& operator--() {
        *this -= 1;
        return *this;
    }

    BigInteger operator++(int) {
        BigInteger x = *this;
        ++(*this);
        return x;
    }
    BigInteger operator--(int) {
        BigInteger x = *this;
        --(*this);
        return x;
    }

    operator bool() const {
        return *this != 0;
    }

    BigInteger& shift() {
        for (size_t i = 0; i < number.size(); ++i) {
            number[i] *= 10;
        }
        for (size_t i = 0; i < number.size(); ++i) {
            if (number[i] >= BASE) {
                int del = number[i] / BASE;
                if (i == number.size() - 1) number.push_back(0);
                number[i + 1] += del;
                number[i] -= del * BASE;
            }
        }
        return *this;
    }

    string toString() const {
        string result = "";
        if (!number.empty() && !(number.size() == 1 && number[0] == 0)) {
            if (!sign &&  !(number.size() == 1 && number[0] == 0)) result += '-';
            for (int i = (int) (number.size() - 1); i >= 0; --i) {
                string add = std::to_string(number[i]);
                for (size_t j = 0; j < (4 - add.size()) && (i != (int) (number.size() - 1)); ++j) {
                    result += '0';
                }
                result += add;
            }
        } else {
            result = "0";
        }
        return result;
    }

    int getSize() const {
        return (int)number.size();
    }

    int operator[](size_t idx) const {
        return (int)number[idx];
    }

    void changeSign() {
        if (!(*this == 0)) sign = !sign;
    }

    bool getSign() const {
        return sign;
    }

    void update(BigInteger& b) {
        while (b.number.size() > 1 && b.number.back() == 0) {
            b.number.pop_back();
        }
        if (b.number.size() == 1 && b.number.back() == 0) b.sign = true;
    }

    friend BigInteger operator+(const int num, const BigInteger& bigint);
    friend BigInteger operator-(const int num, const BigInteger& bigint);
    friend BigInteger operator*(const int num, const BigInteger& bigint);
    friend BigInteger operator/(const int num, const BigInteger& bigint);
    friend BigInteger operator%(const int num, const BigInteger& bigint);
    friend BigInteger operator+(const BigInteger& bigint, const int num);
    friend BigInteger operator-(const BigInteger& bigint, const int num);
    friend BigInteger operator*(const BigInteger& bigint, const int num);
    friend BigInteger operator/(const BigInteger& bigint, const int num);
    friend BigInteger operator%(const BigInteger& bigint, const int num);
    friend BigInteger operator+(const BigInteger& bigint1, const BigInteger& bigint2);
    friend BigInteger operator-(const BigInteger& bigint1, const BigInteger& bigint2);
    friend BigInteger operator*(const BigInteger& bigint1, const BigInteger& bigint2);
    friend BigInteger operator/(const BigInteger& bigint1, const BigInteger& bigint2);
    friend BigInteger operator%(const BigInteger& bigint1, const BigInteger& bigint2);

    friend bool operator<(const BigInteger& bigint1, const BigInteger& bigint2);
    friend bool operator<(const int& num, const BigInteger& bigint);
    friend bool operator<(const BigInteger& bigint, const int& num);
    friend bool operator<=(const BigInteger& bigint1, const BigInteger& bigint2);
    friend bool operator<=(const int& num, const BigInteger& bigint);
    friend bool operator<=(const BigInteger& bigint, const int& num);
    friend bool operator>(const BigInteger& bigint1, const BigInteger& bigint2);
    friend bool operator>(const int& num, const BigInteger& bigint);
    friend bool operator>(const BigInteger& bigint, const int& num);
    friend bool operator>=(const BigInteger& bigint1, const BigInteger& bigint2);
    friend bool operator>=(const int& num, const BigInteger& bigint);
    friend bool operator>=(const BigInteger& bigint, const int& num);
    friend bool operator==(const BigInteger& bigint1, const BigInteger& bigint2);
    friend bool operator==(const int& num, const BigInteger& bigint);
    friend bool operator==(const BigInteger& bigint, const int& num);
    friend bool operator!=(const BigInteger& bigint1, const BigInteger& bigint2);
    friend bool operator!=(const int& num, const BigInteger& bigint);
    friend bool operator!=(const BigInteger& bigint, const int& num);

    friend ostream& operator<<(ostream& out, const BigInteger& bigint);
    friend istream& operator>>(istream& in, BigInteger& bigint);
};

ostream& operator<<(ostream& out, const BigInteger& bigint) {
    if (bigint.getSize() == 0 || (bigint.getSize() == 1 && bigint[0] == 0)){
        out << 0;
    } else {
        if (!bigint.getSign()) out << '-';
        for (int i = (int) (bigint.getSize() - 1); i >= 0; --i) {
            for (size_t j = 0; (j < (size_t)(4 - intSize(bigint[i]))) && (i != (int) (bigint.getSize() - 1)); ++j) {
                out << '0';
            }
            out << bigint[i];
        }
    }
    return out;
}

istream& operator>>(istream& in, BigInteger& bigint) {
    string str;
    in >> str;
    bigint = 0;
    if (!str.empty() && str != "-0") {
        bigint.sign = (str[0] != '-');
        int power = 1;
        for (int i = (int)(str.size() - 1); i >= (str[0] == '-'); --i) {
            if (power * 10 <= 1e4) {
                bigint.number.back() += (power * (str[i] - '0'));
                power *= 10;
            } else {
                bigint.number.push_back(str[i] - '0');
                power = 10;
            }
        }
    }
    return in;
}

BigInteger operator+(const int num, const BigInteger& bigint) {
    BigInteger result = num;
    result += bigint;
    return result;
}

BigInteger operator-(const int num, const BigInteger& bigint) {
    BigInteger result = num;
    result -= bigint;
    return result;
}

BigInteger operator*(const int num, const BigInteger& bigint) {
    BigInteger result = num;
    result *= bigint;
    return result;
}

BigInteger operator/(const int num, const BigInteger& bigint) {
    BigInteger result = num;
    result /= bigint;
    return result;
}

BigInteger operator%(const int num, const BigInteger& bigint) {
    BigInteger result = num;
    result %= bigint;
    return result;
}

BigInteger operator+(const BigInteger& bigint, const int num) {
    BigInteger result = bigint;
    result += num;
    return result;
}

BigInteger operator-(const BigInteger& bigint, const int num) {
    BigInteger result = bigint;
    result -= num;
    return result;
}

BigInteger operator*(const BigInteger& bigint, const int num) {
    BigInteger result = bigint;
    result *= num;
    return result;
}

BigInteger operator/(const BigInteger& bigint, const int num) {
    BigInteger result = bigint;
    result /= num;
    return result;
}

BigInteger operator%(const BigInteger& bigint, const int num) {
    BigInteger result = bigint;
    result %= num;
    return result;
}

BigInteger operator+(const BigInteger& bigint1, const BigInteger& bigint2) {
    BigInteger result = bigint1;
    result += bigint2;
    return result;
}

BigInteger operator-(const BigInteger& bigint1, const BigInteger& bigint2) {
    BigInteger result = bigint1;
    result -= bigint2;
    return result;
}

BigInteger operator*(const BigInteger& bigint1, const BigInteger& bigint2) {
    BigInteger result = bigint1;
    result *= bigint2;
    return result;
}

BigInteger operator/(const BigInteger& bigint1, const BigInteger& bigint2) {
    BigInteger result = bigint1;
    result /= bigint2;
    return result;
}

BigInteger operator%(const BigInteger& bigint1, const BigInteger& bigint2) {
    BigInteger result = bigint1;
    result %= bigint2;
    return result;
}

bool operator<(const BigInteger& bigint1, const BigInteger& bigint2) {
    if (!bigint1.sign && bigint2.sign) {
        return true;
    } else if (bigint1.sign && !bigint2.sign) {
        return false;
    } else {
        bool result;
        if (bigint1.number.size() < bigint2.number.size()) result = true;
        if (bigint1.number.size() > bigint2.number.size()) result = false;
        if (bigint2.number.size() == bigint1.number.size()) {
            int i = (int)bigint1.number.size() - 1;
            for (; i >= 0; --i) {
                if (bigint1.number[i] < bigint2.number[i]) {
                    result = true;
                    break;
                } else if (bigint1.number[i] > bigint2.number[i]) {
                    result = false;
                    break;
                }
            }
            if (i == -1) return false;
            else return bigint1.sign == result;
        }
        return bigint1.sign == result;
    }
}

bool operator<(const int& num, const BigInteger& bigint) {
    BigInteger bigint2 = num;
    return (bigint2 < bigint);
}

bool operator<(const BigInteger& bigint, const int& num) {
    BigInteger bigint2 = num;
    return (bigint < bigint2);
}

bool operator>(const BigInteger& bigint1, const BigInteger& bigint2) {
    return (bigint2 < bigint1);
}

bool operator>(const int& num, const BigInteger& bigint) {
    return (bigint < num);
}

bool operator>(const BigInteger& bigint, const int& num) {
    return (num < bigint);
}

bool operator<=(const BigInteger& bigint1, const BigInteger& bigint2) {
    return (bigint1 < bigint2 || bigint1 == bigint2);
}

bool operator<=(const int& num, const BigInteger& bigint) {
    return (num < bigint || num == bigint);
}

bool operator<=(const BigInteger& bigint, const int& num) {
    return (bigint < num || bigint == num);
}

bool operator>=(const BigInteger& bigint1, const BigInteger& bigint2) {
    return (bigint2 <= bigint1);
}

bool operator>=(const int& num, const BigInteger& bigint) {
    return (bigint <= num);
}

bool operator>=(const BigInteger& bigint, const int& num) {
    return (num <= bigint);
}

bool operator==(const BigInteger& bigint1, const BigInteger& bigint2) {
    return !(bigint1 < bigint2 || bigint2 < bigint1);
}

bool operator==(const int& num, const BigInteger& bigint) {
    return !(num < bigint || bigint < num);
}

bool operator==(const BigInteger& bigint, const int& num) {
    return !(bigint < num || num < bigint);
}

bool operator!=(const BigInteger& bigint1, const BigInteger& bigint2) {
    return !(bigint1 == bigint2);
}

bool operator!=(const int& num, const BigInteger& bigint) {
    return !(num == bigint);
}

bool operator!=(const BigInteger& bigint, const int& num) {
    return !(bigint == num);
}

class Rational {
private:
    BigInteger numerator;
    BigInteger denominator;

    void updateSign() {
        if (!denominator.getSign()) {
            denominator.changeSign();
            numerator.changeSign();
        }
    }

    static BigInteger GCD(const BigInteger& bigint1, const BigInteger& bigint2) {
        BigInteger b1 = bigint1;
        BigInteger b2 = bigint2;
        if (!b1.getSign()) b1.changeSign();
        if (!b2.getSign()) b2.changeSign();
        while (b1 > 0 && b2 > 0) {
            if (b1 > b2) {
                b1 %= b2;
            } else {
                b2 %= b1;
            }
        }
        if (b1 > 0) {
            return b1;
        } else {
            return b2;
        }
    }

public:
    Rational() = default;

    Rational(const BigInteger& bigint) {
        numerator = bigint;
        denominator = 1;
    }

    Rational(const int num) {
        numerator = num;
        denominator = 1;
    }

    Rational(const Rational& rational) {
        numerator = rational.numerator;
        denominator = rational.denominator;
    }

    ~Rational() = default;

    Rational& operator=(const Rational& rational) {
        if (this != &rational) {
            numerator = rational.numerator;
            denominator = rational.denominator;
        }
        return *this;
    }

    Rational& operator=(const int num) {
        numerator = num;
        denominator = 1;
        return *this;
    }

    Rational& operator=(const BigInteger& bigint) {
        numerator = bigint;
        denominator = 1;
        return *this;
    }

    Rational operator-() {
        Rational result = *this;
        result.numerator.changeSign();
        return result;
    }

    Rational& operator+=(const Rational& rational) {
        Rational copy = rational;
        numerator *= copy.denominator;
        numerator += denominator * copy.numerator;
        denominator *= copy.denominator;
        BigInteger gcd = GCD(numerator, denominator);
        numerator /= gcd;
        denominator /= gcd;
        return *this;
    }

    Rational& operator-=(const Rational& rational) {
        Rational copy = rational;
        numerator *= copy.denominator;
        numerator -= denominator * copy.numerator;
        denominator *= copy.denominator;
        BigInteger gcd = GCD(numerator, denominator);
        numerator /= gcd;
        denominator /= gcd;
        return *this;
    }

    Rational& operator+=(const int num) {
        Rational rational = num;
        *this += rational;
        return *this;
    }

    Rational& operator+=(const BigInteger& bigint) {
        Rational rational = bigint;
        *this += rational;
        return *this;
    }

    Rational& operator-=(const int num) {
        Rational rational = num;
        *this -= rational;
        return *this;
    }

    Rational& operator-=(const BigInteger& bigint) {
        Rational rational = bigint;
        *this -= rational;
        return *this;
    }

    Rational& operator*=(const Rational& rational) {
        Rational copy = rational;
        numerator *= copy.numerator;
        denominator *= copy.denominator;
        updateSign();
        BigInteger gcd = GCD(numerator, denominator);
        numerator /= gcd;
        denominator /= gcd;
        return *this;
    }

    Rational& operator*=(const int num) {
        Rational rational = num;
        *this *= rational;
        return *this;
    }

    Rational& operator*=(const BigInteger& bigint) {
        Rational rational = bigint;
        *this *= rational;
        return *this;
    }

    Rational& operator/=(const Rational& rational) {
        Rational copy = rational;
        numerator *= copy.denominator;
        denominator *= copy.numerator;
        updateSign();
        BigInteger gcd = GCD(numerator, denominator);
        numerator /= gcd;
        denominator /= gcd;
        return *this;
    }

    Rational& operator/=(const int num) {
        Rational rational = num;
        *this /= rational;
        return *this;
    }

    Rational& operator/=(const BigInteger& bigint) {
        Rational rational = bigint;
        *this /= rational;
        return *this;
    }

    string asDecimal(const size_t precision) const {
        Rational copy = *this;
        string result;
        if (!copy.numerator.getSign()) {
            result += '-';
            copy.numerator.changeSign();
        }
        BigInteger quotient = copy.numerator;
        quotient /= copy.denominator;
        result += quotient.toString();
        if (precision > 0) result += '.';
        copy.numerator -= quotient * copy.denominator;

        size_t count = 0;
        while (count < precision) {
            if (copy.numerator == 0) break;
            copy.numerator.shift();
            while (copy.numerator < copy.denominator) {
                if (count >= precision) break;
                copy.numerator.shift();
                result += "0";
                count++;
            }
            if (count >= precision) break;
            quotient = copy.numerator;
            quotient /= copy.denominator;
            copy.numerator -= quotient * copy.denominator;
            result += quotient.toString();
            count += intSize(quotient[0]);
        }

        while (count < precision) {
            result += "0";
            count++;
        }

        while (count > precision) {
            result.pop_back();
            count--;
        }

        bool flag = true;
        for (size_t i = 0; i < result.size(); ++i) {
            if (result[i] != '-' && result[i] != '.' && result[i] != '0') {
                flag = false;
                break;
            }
        }
        if (flag && result[0] == '-') {
            result = result.substr(1, result.size() - 1);
        }

        return result;
    }

    string toString() const {
        string result = "";
        result += numerator.toString();
        if (denominator != 1) {
            result += '/';
            result += denominator.toString();
        }
        return result;
    }

    operator double() const {
        string decimal = this->asDecimal(20);
        double result = std::stod(decimal);
        return result;
    }

    friend Rational operator+(const int num, const Rational& rational);
    friend Rational operator-(const int num, const Rational& rational);
    friend Rational operator*(const int num, const Rational& rational);
    friend Rational operator/(const int num, const Rational& rational);
    friend Rational operator+(const Rational& rational, const int num);
    friend Rational operator-(const Rational& rational, const int num);
    friend Rational operator*(const Rational& rational, const int num);
    friend Rational operator/(const Rational& rational, const int num);
    friend Rational operator+(const BigInteger& bigint, const Rational& rational);
    friend Rational operator-(const BigInteger& bigint, const Rational& rational);
    friend Rational operator*(const BigInteger& bigint, const Rational& rational);
    friend Rational operator/(const BigInteger& bigint, const Rational& rational);
    friend Rational operator+(const Rational& rational, const BigInteger& bigint);
    friend Rational operator-(const Rational& rational, const BigInteger& bigint);
    friend Rational operator*(const Rational& rational, const BigInteger& bigint);
    friend Rational operator/(const Rational& rational, const BigInteger& bigint);
    friend Rational operator+(const Rational& rational, const int num);
    friend Rational operator-(const Rational& rational, const int num);
    friend Rational operator*(const Rational& rational, const int num);
    friend Rational operator/(const Rational& rational, const int num);
    friend Rational operator+(const Rational& rational1, const Rational& rational2);
    friend Rational operator-(const Rational& rational1, const Rational& rational2);
    friend Rational operator*(const Rational& rational1, const Rational& rational2);
    friend Rational operator/(const Rational& rational1, const Rational& rational2);

    friend bool operator<(const Rational& rational1, const Rational& rational2);
    friend bool operator<(const int& num, const Rational& rational);
    friend bool operator<(const Rational& rational, const int& num);
    friend bool operator<=(const Rational& rational1, const Rational& rational2);
    friend bool operator<=(const int& num, const Rational& rational);
    friend bool operator<=(const Rational& rational, const int& num);
    friend bool operator>(const Rational& rational1, const Rational& rational2);
    friend bool operator>(const int& num, const Rational& rational);
    friend bool operator>(const Rational& rational, const int& num);
    friend bool operator>=(const Rational& rational1, const Rational& rational2);
    friend bool operator>=(const int& num, const Rational& rational);
    friend bool operator>=(const Rational& rational, const int& num);
    friend bool operator==(const Rational& rational1, const Rational& rational2);
    friend bool operator==(const int& num, const Rational& rational);
    friend bool operator==(const Rational& rational, const int& num);
    friend bool operator!=(const Rational& rational1, const Rational& rational2);
    friend bool operator!=(const int& num, const Rational& rational);
    friend bool operator!=(const Rational& rational, const int& num);
};

Rational operator+(const int num, const Rational& rational) {
    Rational result = num;
    result += rational;
    return result;
}

Rational operator-(const int num, const Rational& rational) {
    Rational result = num;
    result -= rational;
    return result;
}

Rational operator*(const int num, const Rational& rational) {
    Rational result = num;
    result *= rational;
    return result;
}

Rational operator/(const int num, const Rational& rational) {
    Rational result = num;
    result /= rational;
    return result;
}

Rational operator+(const Rational& rational, const int num) {
    Rational result = rational;
    result += num;
    return result;
}

Rational operator-(const Rational& rational, const int num) {
    Rational result = rational;
    result -= num;
    return result;
}

Rational operator*(const Rational& rational, const int num) {
    Rational result = rational;
    result *= num;
    return result;
}

Rational operator/(const Rational& rational, const int num) {
    Rational result = rational;
    result /= num;
    return result;
}

Rational operator+(const BigInteger& bigint, const Rational& rational) {
    Rational result = bigint;
    result += rational;
    return result;
}

Rational operator-(const BigInteger& bigint, const Rational& rational) {
    Rational result = bigint;
    result -= rational;
    return result;
}

Rational operator*(const BigInteger& bigint, const Rational& rational) {
    Rational result = bigint;
    result *= rational;
    return result;
}

Rational operator/(const BigInteger& bigint, const Rational& rational) {
    Rational result = bigint;
    result /= rational;
    return result;
}

Rational operator+(const Rational& rational, const BigInteger& bigint) {
    Rational result = rational;
    result += bigint;
    return result;
}

Rational operator-(const Rational& rational, const BigInteger& bigint) {
    Rational result = rational;
    result -= bigint;
    return result;
}

Rational operator*(const Rational& rational, const BigInteger& bigint) {
    Rational result = rational;
    result *= bigint;
    return result;
}

Rational operator/(const Rational& rational, const BigInteger& bigint) {
    Rational result = rational;
    result /= bigint;
    return result;
}

Rational operator+(const Rational& rational1, const Rational& rational2) {
    Rational result = rational1;
    result += rational2;
    return result;
}

Rational operator-(const Rational& rational1, const Rational& rational2) {
    Rational result = rational1;
    result -= rational2;
    return result;
}

Rational operator*(const Rational& rational1, const Rational& rational2) {
    Rational result = rational1;
    result *= rational2;
    return result;
}

Rational operator/(const Rational& rational1, const Rational& rational2) {
    Rational result = rational1;
    result /= rational2;
    return result;
}

bool operator<(const Rational& rational1, const Rational& rational2) {
    BigInteger num1 = rational1.numerator * rational2.denominator;
    BigInteger num2 = rational2.numerator * rational1.denominator;
    return (num1 < num2);
}

bool operator<(const int& num, const Rational& rational) {
    Rational rational2 = num;
    return (rational2 < rational);
}

bool operator<(const Rational& rational, const int& num) {
    Rational rational2 = num;
    return (rational < rational2);
}

bool operator>(const Rational& rational1, const Rational& rational2) {
    return (rational2 < rational1);
}

bool operator>(const int& num, const Rational& rational) {
    return (rational < num);
}

bool operator>(const Rational& rational, const int& num) {
    return (num < rational);
}

bool operator<=(const Rational& rational1, const Rational& rational2) {
    return (rational1 < rational2 || rational1 == rational2);
}

bool operator<=(const int& num, const Rational& rational) {
    return (num < rational || num == rational);
}

bool operator<=(const Rational& rational, const int& num) {
    return (rational < num || rational == num);
}

bool operator>=(const Rational& rational1, const Rational& rational2) {
    return (rational2 <= rational1);
}

bool operator>=(const int& num, const Rational& rational) {
    return (rational <= num);
}

bool operator>=(const Rational& rational, const int& num) {
    return (num <= rational);
}

bool operator==(const Rational& rational1, const Rational& rational2) {
    return !(rational1 < rational2 || rational2 < rational1);
}

bool operator==(const int& num, const Rational& rational) {
    return !(num < rational || rational < num);
}

bool operator==(const Rational& rational, const int& num) {
    return !(rational < num || num < rational);
}

bool operator!=(const Rational& rational1, const Rational& rational2) {
    return !(rational1 == rational2);
}

bool operator!=(const int& num, const Rational& rational) {
    return !(num == rational);
}


bool operator!=(const Rational& rational, const int& num) {
    return !(rational == num);
}

#endif //UNTITLED_BIGINTEGER_H

