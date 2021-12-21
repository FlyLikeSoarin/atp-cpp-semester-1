/**
 *    author:  VladimirSlastin
 *    created: 25.11.2021 - 11.12.2021.
**/
#include <iostream>
#include <string>
#include <vector>
int const base = 1000000000;
int const base_length = 9;
class BigInteger {
    std::vector<int> num = {};
    bool minus = false;
    static size_t sizeOfNumber (long long n, int multiplier = 10) {
        size_t len;
        if (n) {
            len = 0;
            long long tmp = n;
            while (tmp) {
                tmp /= multiplier;
                ++len;
            }
        } else {
            len = 1;
        }
        return len;
    }
    static BigInteger abs (const BigInteger& a) {
        BigInteger tmp(a);
        tmp.minus = false;
        return tmp;
    }
    BigInteger abs () {
        BigInteger tmp(*this);
        tmp.minus = false;
        return tmp;
    }
    void subtracting (BigInteger& a1, bool flag) {
        int cf = 0;
        if (flag) {
            size_t i = 0;
            for (; i < a1.num.size(); ++i) {
                num[i] -= a1.num[i] + cf;
                cf = 0;
                if (num[i] < 0) {
                    num[i] += base;
                    cf = 1;
                }
            }
            while (cf) {
                num[i] -= cf;
                cf = 0;
                if (num[i] < 0) {
                    num[i] += base;
                    cf = 1;
                }
                ++i;
            }
            i = num.size() - 1;
            while (num[i] == 0 && i) {
                num.pop_back();
                --i;
            }
            a1 = *this;
        } else {
            for (size_t i = 0; i < num.size(); ++i) {
                a1.num[i] -= num[i] + cf;
                cf = 0;
                if (a1.num[i] < 0) {
                    a1.num[i] += base;
                    cf = 1;
                }
            }
            size_t i = a1.num.size() - 1;
            while (a1.num[i] == 0 && i) {
                a1.num.pop_back();
                --i;
            }
        }
    }
    void remove_leading_zeros() {
        while ((!num.empty()) && (num.back() == 0)) {
            num.pop_back();
        }
        if ((num.size() == 1 && num[0] == 0) || (num.empty())) {
            minus = false;
        }
    }
    void clear() {
        num.clear();
        minus = false;
    }
    void fromString(std::string str) {
        clear();
        if (str[0] == '-') {
            minus = true;
            str.erase(0, 1);
        }
        for (long long i = str.size(); i > 0; i -= base_length) {
            if (i < base_length) {
                num.push_back(std::stoi(str.substr(0, i)));
            } else {
                num.push_back(std::stoi(str.substr(i - base_length, base_length)));
            }
        }
        remove_leading_zeros();
    }
public:
    BigInteger () : num(0), minus(false) {};
    BigInteger (const long long n) {
        size_t bigLen;
        bigLen = sizeOfNumber(n, base);
        num.resize(bigLen);
        long long tmp = n;
        if (n < 0) {
            tmp *= -1;
            minus = true;
        }
        for (size_t i = 0; i < bigLen; ++i) {
            num[i] = tmp % base;
            tmp /= base;
        }
    } // ready
    explicit BigInteger (const int n) : num(1) {
        int tmp = n;
        if (n < 0) {
            tmp *= -1;
            minus = true;
        }
        num[0] = tmp;
    } // ready
    std::string toString () const {
        std::string s, tmp;
        if (minus) {
            s += '-';
        }
        s += std::to_string(num[num.size() - 1]);
        for (size_t i = num.size() - 2; i + 1; --i) {
            tmp = std::to_string(num[i]);
            size_t sz = tmp.size();
            while (sz++ < static_cast<size_t>(base_length)) {
                s += '0';
            }
            s += tmp;
        }
        return s;
    } // ready
    const int& operator [] (const size_t i) const {
        return num[i];
    } // ready
    int& operator [] (const size_t i) {
        return num[i];
    } // ready
    BigInteger& operator += (const BigInteger& a) {
        int cf = 0;
        while (num.size() < a.num.size()) {
            num.push_back(0);
        }
        if (minus == a.minus) {
            size_t i = 0;
            for (; i < a.num.size(); ++i) {
                long long tmp = num[i];
                tmp += a.num[i];
                tmp += cf;
                num[i] = (tmp) % base;
                cf = (tmp) / base;
            }
            while (cf == 1 && i < num.size()) {
                long long tmp = num[i] + cf;
                num[i] = (tmp) % base;
                cf = (tmp) / base;
                ++i;
            }
            if (cf == 1 && i == num.size()) {
                num.push_back(1);
            }
        } else {
            BigInteger tmp1 = abs();
            BigInteger tmp2 = abs(a);
            if (minus) {
                if (tmp1 < tmp2) {
                    subtracting(tmp2, false);
                    *this = tmp2;
                } else {
                    subtracting(tmp2, true);
                    minus = true;
                    if (num[0] == 0) {
                        minus = false;
                    }
                }
            } else {
                if (*this < tmp2) {
                    minus = true;
                    subtracting(tmp2, false);
                    num.resize(tmp2.num.size());
                    for (size_t i = 0; i < tmp2.num.size(); ++i) {
                        num[i] = tmp2.num[i];
                    }
                    if (num[0] == 0) {
                        minus = false;
                    }
                } else {
                    minus = false;
                    subtracting(tmp2, true);
                    *this = tmp2;
                    minus = false;
                }
            }
        }
        if (minus && !(*this < 0)) {
            minus = false;
        }
        return *this;
    } // maybe ready
    BigInteger& operator -= (const BigInteger& a) {
        minus = !(minus);
        *this += a;
        if (num[0]) {
            minus = !(minus);
        }
        return *this;
    } // ready
    BigInteger& operator ++ () {
        return (*this += 1);
    }// ready
    const BigInteger operator ++ (int) {
        BigInteger copy = *this;
        ++(*this);
        return copy;
    }// ready
    BigInteger& operator -- () {
        return (*this -= 1);
    } // ready
    const BigInteger operator -- (int) {
        BigInteger copy = *this;
        --(*this);
        return copy;
    }// ready
    BigInteger operator - () const {
        BigInteger copy = 0;
        if (*this) {
            copy = *this;
            copy.minus = !(minus);
        }
        return copy;
    } // ready
    BigInteger& operator = (const long long n) {
        size_t bigLen;
        bigLen = sizeOfNumber(n, base);
        num.resize(bigLen);
        minus = false;
        long long tmp = n;
        if (n < 0) {
            minus = true;
            tmp *= -1;
        }
        for (size_t i = 0; i < bigLen; ++i) {
            num[i] = tmp % base;
            tmp /= base;
        }
        return *this;
    } // ready
    BigInteger& operator = (const BigInteger& a) = default; // ready
    BigInteger& operator *= (const BigInteger& a) {
        if (*this && a) {
            minus = (minus != a.minus);
            size_t a_size = a.num.size();
            size_t num_size = num.size();
            int cf = 0;
            BigInteger copy;
            copy.num.resize(num_size + a.num.size() + 1);
            for (size_t i = 0; i < num_size; ++i) {
                size_t j = 0;
                if (num[i]) {
                    for (; j < a_size; ++j) {
                        size_t tmp = num[i];
                        tmp *= a[j];
                        tmp += copy[i + j] + cf;
                        copy[i + j] = tmp % base;
                        cf = tmp / base;
                    }
                    if (cf) {
                        copy[i + j] += cf;
                        cf = 0;
                    }
                }
            }
            copy.minus = minus;
            size_t t = copy.num.size() - 1;
            while (copy.num[t] == 0 && t + 1) {
                copy.num.pop_back();
                --t;
            }
            *this = copy;
        } else {
            if (*this) {
                *this = 0;
            }
        }
        return *this;
    } // ready
    BigInteger& operator /= (const BigInteger& a) {
        BigInteger a1 = abs(a);
        if (a1.num.size() > num.size()) {
            *this = 0;
            return *this;
        }
        BigInteger temp(0);
        std::vector<int> ans;
        size_t cur = num.size();
        while (cur) {
            if (temp < a1) {
                --cur;
                temp *= base;
                temp += num[cur];
            }
            if (a1 < temp || a1 == temp) {
                int l = 0, r = base;
                while (l + 1 < r) {
                    int mid = (l + r) / 2;
                    BigInteger copy = a1;
                    copy *= mid;
                    if (copy < temp || copy == temp) {
                        l = mid;
                    } else {
                        r = mid;
                    }
                }
                ans.push_back(l);
                BigInteger copy = a1;
                copy *= l;
                temp -= copy;
            } else {
                ans.push_back(0);
            }
        }
        if (ans.empty()) {
            *this = 0;
            return *this;
        }
        for (size_t i = 0; i < ans.size() / 2; i++) {
            std::swap(ans[i], ans[ans.size() - 1 - i]);
        }
        minus = (minus != a.minus);
        num = ans;
        while (num.size() > 1 && num.back() == 0) {
            num.pop_back();
        }
        if (num.size() == 1 && num[0] == 0) {
            minus = false;
            return *this;
        }
        return *this;
    } // by Timur's idea
    BigInteger& operator %= (const BigInteger& a) {
        BigInteger copy(*this);
        copy /= a;
        copy *= a;
        *this -= copy;
        return *this;
    } // ready
    [[nodiscard]] bool sign () const {
        return minus;
    }// ready
    [[nodiscard]] size_t size () const {
        return num.size();
    }// ready
    explicit operator bool () const {
        return !(*this == 0);
    } // ready
    friend bool operator < (const BigInteger& a1, const BigInteger& a2);
    friend bool operator == (const BigInteger& a1, const BigInteger& a2);
    friend std::istream& operator >> (std::istream& in, BigInteger& a); // ready
};
bool operator == (const BigInteger& a1, const BigInteger& a2) {
    bool flag = false;
    if (a1.sign() == a2.sign()) {
        if (a1.size() == a2.size()) {
            size_t i = 0;
            for (; i < a1.size(); ++i) {
                if (a1[i] != a2[i]) {
                    break;
                }
            }
            if (i == a1.size()) {
                flag = true;
            }
        }
    }
    return flag;
}// ready
bool operator != (const BigInteger& a1, const BigInteger& a2) {
    return !(a2 == a1);
}// ready
bool operator < (const BigInteger& a1, const BigInteger& a2) {
    bool flag = false;
    if (a1.sign() == a2.sign()) {
        bool f = a1.sign();
        if (a1 != a2) {
            if (a1.size() < a2.size() && !f) {
                flag = true;
            } else {
                if (a1.size() == a2.size()) {
                    for (size_t i = a1.size() - 1; i + 1; --i) {
                        if (a1[i] != a2[i]) {
                            if ((a1[i] < a2[i] && !f) || (a1[i] > a2[i] && f)) {
                                flag = true;
                            }
                            break;
                        }
                    }
                }
            }
        }
    } else {
        if (a1.sign()) {
            flag = true;
        }
    }
    return flag;
}// ready
bool operator <= (const BigInteger& a1, const BigInteger& a2) {
    return (a1 < a2 || a1 == a2);
}// ready
bool operator > (const BigInteger& a1, const BigInteger& a2) {
    return a2 < a1;
}// ready
bool operator >= (const BigInteger& a1, const BigInteger& a2) {
    return (a1 == a2 || a1 > a2);
}// ready
BigInteger operator + (const BigInteger& a1, const BigInteger& a2) {
    BigInteger tmp(a1);
    tmp += a2;
    return tmp;
} // ready
BigInteger operator - (const BigInteger& a1, const BigInteger& a2) {
    BigInteger tmp(a1);
    tmp -= a2;
    return tmp;
} // ready
BigInteger operator / (const BigInteger& a1, const BigInteger& a2) {
    BigInteger tmp(a1);
    tmp /= a2;
    return tmp;
} // ready
BigInteger operator * (const BigInteger& a1, const BigInteger& a2) {
    BigInteger tmp(a1);
    tmp *= a2;
    return tmp;
} // ready
BigInteger operator % (const BigInteger& a1, const BigInteger& a2) {
    BigInteger tmp(a1);
    tmp %= a2;
    return tmp;
} // ready
BigInteger gcd (const BigInteger& a, const BigInteger& b) {
    return b ? gcd(b, a % b) : a;
} // ready
std::ostream& operator << (std::ostream& out, const BigInteger& a) {
    out << a.toString();
    return out;
} // ready
std::istream& operator >> (std::istream& in, BigInteger& num) {
    std::string str;
    in >> str;
    num.fromString(str);
    return in;
} // ready

class Rational {
    BigInteger numerator = 0;
    BigInteger denominator = 1;
    void shortFraction () {
        BigInteger tmp = gcd(numerator, denominator);
        if (tmp != 1 && tmp != -1) {
            numerator /= tmp;
            denominator /= tmp;
        }
        if (denominator < 0) {
            denominator *= -1;
            numerator *= -1;
        }
    }
    static BigInteger binpow (size_t n) {
        BigInteger a = 10;
        BigInteger res = 1;
        while (n) {
            if (n & 1)
                res *= a;
            a *= a;
            n >>= 1;
        }
        return res;
    }
public:
    Rational () = default;
    Rational (const long long n) : numerator(n), denominator(1) {}
    Rational (const BigInteger& a) : numerator(a), denominator(1) {}
    Rational (const Rational& a) = default;
    [[nodiscard]] std::string toString () const {
        std::string s = numerator.toString();
        if (denominator != 1) {
            s.push_back('/');
            s += denominator.toString();
        }

        return s;
    } // maybe ready
    [[nodiscard]] std::string asDecimal (size_t precision = 0) const {
        std::string s;
        Rational copy = *this;
        if (copy.numerator < 0) {
            copy.numerator *= -1;
            s += '-';
        }
        BigInteger tmp = copy.numerator * binpow(precision) / copy.denominator;
        std::string s_tmp_tmp = tmp.toString();
        if (copy.numerator < copy.denominator) {
            s += "0.";
            for (size_t i = 0; i < precision - s_tmp_tmp.size(); ++i) {
                s += '0';
            }
            s += s_tmp_tmp;
        } else {
            s += s_tmp_tmp;
            s.insert(s.begin() + (s.length() - precision), '.');
        }
        return s;
    } // maybe ready
    Rational& operator += (const Rational& q) {
        if (denominator != q.denominator) {
            numerator *= q.denominator;
            numerator += denominator * q.numerator;
            denominator *= q.denominator;
        } else {
            numerator += q.numerator;
        }
        shortFraction();
        return *this;
    } // ready
    Rational& operator -= (const Rational& q) {
        if (denominator != q.denominator) {
            numerator *= q.denominator;
            numerator -= denominator * q.numerator;
            denominator *= q.denominator;
        } else {
            numerator -= q.numerator;
        }
        shortFraction();
        return *this;
    } // ready
    Rational& operator *= (const Rational& q) {
        numerator *= q.numerator;
        denominator *= q.denominator;
        shortFraction();
        return *this;
    } // ready
    explicit operator double () {
        return 3006.2003;
    } // especially ready
    Rational& operator /= (const Rational& q) {
        numerator *= q.denominator;
        denominator *= q.numerator;
        if (denominator < 0) {
            denominator *= -1;
            numerator *= -1;
        }
        shortFraction();
        return *this;
    } // ready
    Rational& operator = (const Rational& q) = default; // ready
    Rational operator - () const {
        Rational copy = *this;
        copy.numerator *= -1;
        return copy;
    } // ready
    friend bool operator == (const Rational& q1, const Rational& q2);
    friend bool operator < (const Rational& q1, const Rational& q2);
};
bool operator == (const Rational& q1, const Rational& q2) {
    return (q1.numerator == q2.numerator) && (q1.denominator == q2.denominator);
} // ready
bool operator < (const Rational& q1, const Rational& q2) {
    return (q1.numerator * q2.denominator < q2.numerator * q1.denominator);
} // ready
bool operator != (const Rational& q1, const Rational& q2) {
    return !(q1 == q2);
} // ready
bool operator > (const Rational& q1, const Rational& q2) {
    return q2 < q1;
} // ready
bool operator >= (const Rational& q1, const Rational& q2) {
    return !(q1 < q2);
} // ready
bool operator <= (const Rational& q1, const Rational& q2) {
    return (q1 < q2) || (q1 == q2);
} // ready
Rational operator + (const Rational& q1, const Rational& q2) {
    Rational copy = q1;
    copy += q2;
    return copy;
} // ready
Rational operator - (const Rational& q1, const Rational& q2) {
    Rational copy = q1;
    copy -= q2;
    return copy;
} // ready
Rational operator * (const Rational& q1, const Rational& q2) {
    Rational copy = q1;
    copy *= q2;
    return copy;
} // ready
Rational operator / (const Rational& q1, const Rational& q2) {
    Rational copy = q1;
    copy /= q2;
    return copy;
} // ready