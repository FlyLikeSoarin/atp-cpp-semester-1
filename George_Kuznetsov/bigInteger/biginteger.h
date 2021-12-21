#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class BigInteger {
public:
    //Constructors
    BigInteger() = default;
    BigInteger(long long num) {
        size = 1;
        blocks = std::vector <long long> (size);
        positive = num >= 0;
        blocks[0] = std::abs(num);
    }
    BigInteger(const std::string& num) {
        if (num[0] == '-') {
            positive = false;
        }
        long long in_size = num.size() - 1 + positive;
        size = in_size / pr + 1;
        blocks = std::vector <long long> (size);
        int counter = 0;
        for (long long i = num.size() - 1; i >= 1 - positive; i -= pr) {
            if (i >= pr - 1 + 1 - positive) {
                blocks[counter] = std::stoi(num.substr(i - pr + 1, pr));
                counter++;
            } else {
                blocks[counter] = std::stoi(num.substr(1 - positive, i + positive));
            }
        }
        resize();
        adduction();
    }
    BigInteger(const BigInteger& num) {
        size = num.size;
        blocks = num.blocks;
        positive = num.positive;
    }
    //Operators
    BigInteger& operator += (const BigInteger& num) {
        bool old_sign = positive;
        if (positive == num.positive) {
            tot_unsigned(num);
            positive = old_sign;
        } else {
            subtract_unsigned(num);
            if (!old_sign) {
                positive = !positive;
            }
        }
        resize();
        adduction();
        return *this;
    }
    BigInteger& operator -= (const BigInteger& num) {
        bool old_sign = positive;
        if (positive == num.positive) {
            subtract_unsigned(num);
            if (!old_sign) {
                positive = !positive;
            }
        } else {
            tot_unsigned(num);
            positive = old_sign;
        }
        resize();
        adduction();
        return *this;
    }
    BigInteger& operator *= (const BigInteger& num) {
        std::vector <long long> copy(size + num.size);
        for (long long i = 0; i < size; ++i) {
            for (int j = 0; j < num.size; ++j) {
                copy[i + j] += blocks[i] * num.blocks[j];
                if (copy[i + j] > base) {
                    copy[i + j + 1] += copy[i + j] / base;
                    copy[i + j] %= base;
                }
            }
        }
        positive = !(positive ^ num.positive);
        blocks = copy;
        size += num.size;
        resize();
        adduction();
        return *this;
    }
    BigInteger& operator /= (const BigInteger& num) {
        divide_unsigned(num);
        positive = !(positive ^ num.positive);
        resize();
        adduction();
        return *this;
    }
    BigInteger& operator %= (const BigInteger& num) {
        blocks = divide_unsigned(num);
        size = blocks.size();
        resize();
        adduction();
        return *this;
    }
    BigInteger& operator = (const BigInteger& num) {
        size = num.size;
        blocks = num.blocks;
        positive = num.positive;
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
        BigInteger copy = *this;
        ++(*this);
        return copy;
    }
    BigInteger operator--(int) {
        BigInteger copy = *this;
        --(*this);
        return copy;
    }
    //Implicit conversions
    explicit operator int() {
        long long result = 0;
        if (size > 1) result += blocks[1];
        if (size > 0) result += blocks[0] * base;
        return int(result);
    }
    explicit operator long long() {
        return blocks[0];
    }
    explicit operator bool() {
        bool sooth = false;
        if (positive && size > 0 && blocks[0] > 0) sooth = true;
        return sooth;
    }
    friend bool operator < (const BigInteger& num_1,
                            const BigInteger& num_2);
    //Methods
    void clear() {
        blocks.clear();
        size = 0;
    }
    [[nodiscard]] std::string toString() const {
        std::string output;
        if (!positive) {
            output.push_back('-');
        }
        for (long long i = size - 1; i >= 0; --i) {
            auto number = std::to_string(blocks[i]);
            if (i == size - 1) {
                output += number;
            } else {
                output += std::string(9 - number.length(), '0') + number;
            }
        }
        return output;
    }
    [[nodiscard]] BigInteger abs() const {
        BigInteger copy = *this;
        copy.positive = true;
        return copy;
    }
private:
    std::vector <long long> blocks; // in reverse order
    long long size = 0; //amount of blocks
    int base = 1e9;
    bool positive = true;
    int pr = 9;
    void adduction() {
        for (long long i = 0; i < size; ++i) {
            if (blocks[i] >= base) {
                if (i == size - 1) {
                    blocks.push_back(0);
                    size++;
                }
                blocks[i + 1] += blocks[i] / base;
                blocks[i] %= base;
            }
            if (blocks[i] < 0) {
                if (i == size - 1) {
                    blocks[i] *= -1;
                    positive = false;
                } else {
                    blocks[i] += base;
                    blocks[i + 1]--;
                }
            }
        }
        if (size == 1 && blocks[0] == 0) {
            positive = true;
        }
    }
    void resize() {
        for (long long i = size - 1; i > 0; --i) {
            if (blocks[i] == 0) {
                blocks.pop_back();
                size--;
            } else {
                i = 0;
            }
        }
    }
    void tot_unsigned(const BigInteger& num) {
        blocks.resize(std::max(size, num.size) + 1);
        for (long long i = 0; i < num.size; ++i) {
            blocks[i] += num.blocks[i];
            if (blocks[i] >= base) {
                blocks[i + 1]++;
                blocks[i] -= base;
            }
        }
        positive = true;
        size = std::max(size, num.size) + 1;
    }
    void subtract_unsigned(const BigInteger& num) {
        blocks.resize(std::max(size, num.size));
        for (long long i = 0; i < num.size; ++i) {
            blocks[i] -= num.blocks[i];
            if (blocks[i] < 0 && i != num.size - 1) {
                blocks[i] += base;
                blocks[i + 1]--;
            }
        }
        positive = true;
        size = std::max(size, num.size);
        if (blocks[size - 1] < 0) {
            positive = false;
            blocks[size - 1] *= -1;
            for (long long i = size - 2; i >= 0; --i) {
                blocks[i] = base - blocks[i];
                blocks[i + 1]--;
            }
        }
    }
    std::vector <long long> divide_unsigned(const BigInteger& num) {
        std::vector <long long> carry(num.size);
        std::vector <long long> result(size);
        std::vector <long long> count_arr;
        long long upper_bound = 0;
        long long bottom_bound = 0;
        long long current_bound = 0;
        for (long long i = 0; i < size; ++i) {
            carry.insert(carry.begin(), blocks[size - 1 - i]);
            carry[carry.size() - 2] += carry[carry.size() - 1] * base;
            carry.pop_back();
            //quick check
            upper_bound = carry[carry.size() - 1] / num.blocks[num.size - 1];
            bottom_bound = carry[carry.size() - 1] / (num.blocks[num.size - 1] + 1);
            current_bound = (upper_bound + bottom_bound) / 2;
            while (upper_bound != bottom_bound) {
                count_arr = carry;
                for (long long j = 0; j < num.size; ++j) {
                    count_arr[j] -= current_bound * num.blocks[j];
                    if (count_arr[j] < 0) {
                        if (j != num.size - 1) {
                            count_arr[j + 1] -= std::abs(count_arr[j]) / base;
                            if (std::abs(count_arr[j]) % base) count_arr[j + 1]--;
                            count_arr[j] %= base;
                            count_arr[j] += base;
                        }
                    }
                }
                if (count_arr[num.size - 1] < 0) {
                    if (upper_bound - bottom_bound == 1) upper_bound = bottom_bound;
                    else {
                        upper_bound = current_bound;
                        current_bound = (bottom_bound + upper_bound) / 2;
                    }
                } else {
                    if (upper_bound - bottom_bound == 1) {
                        if (current_bound == upper_bound) bottom_bound = upper_bound;
                        else current_bound = upper_bound;
                    } else {
                        bottom_bound = current_bound;
                        current_bound = (bottom_bound + upper_bound) / 2;
                    }
                }
            }
            result[i] = bottom_bound;
            for (long long j = 0; j < num.size; ++j) {
                carry[j] -= bottom_bound * num.blocks[j];
                if (carry[j] < 0) {
                    carry[j + 1] -= std::abs(carry[j]) / base;
                    if (std::abs(carry[j]) % base != 0) carry[j + 1]--;
                    carry[j] %= base;
                    if (carry[j] < 0) carry[j] += base;
                }
            }
        }
        std::reverse(result.begin(), result.end());
        blocks = result;
        size = result.size();
        return carry;
    }
};
//Operators
BigInteger operator + (const BigInteger& num_1,
                       const BigInteger& num_2) {
    BigInteger copy = num_1;
    copy += num_2;
    return copy;
}
BigInteger operator - (const BigInteger& num_1,
                       const BigInteger& num_2) {
    BigInteger copy = num_1;
    copy -= num_2;
    return copy;
}
BigInteger operator * (const BigInteger& num_1,
                       const BigInteger& num_2) {
    BigInteger copy = num_1;
    copy *= num_2;
    return copy;
}
BigInteger operator / (const BigInteger& num_1,
                       const BigInteger& num_2) {
    BigInteger copy = num_1;
    copy /= num_2;
    return copy;
}
BigInteger operator % (const BigInteger& num_1,
                       const BigInteger& num_2) {
    BigInteger copy = num_1;
    copy %= num_2;
    return copy;
}
BigInteger operator - (const BigInteger& num) {
    BigInteger copy = num;
    copy *= -1;
    return copy;
}
//Logic operators
bool operator < (const BigInteger& num_1,
                 const BigInteger& num_2) {
    int result = 0;
    // 0 - less
    //1 - more
    if (num_1.positive && !num_2.positive) {
        return false;
    } else if (!num_1.positive && num_2.positive) {
        return true;
    } else {
        if (num_2.size < num_1.size) {
            result = 1;
        } else if (num_2.size == num_1.size) {
            long long i = num_1.size - 1;
            while(num_1.blocks[i] == num_2.blocks[i] && i > 0) --i;
            if (num_1.blocks[i] > num_2.blocks[i]) {
                result = 1;
            } else if (num_1.blocks[i] < num_2.blocks[i]) {
                result = 0;
            } else {
                return false;
            }
        }
        if (!num_1.positive) {
            result += 1;
            result %= 2;
        }
        return !result;
    }

}
bool operator > (const BigInteger& num_1,
                 const BigInteger& num_2) {
    return num_2 < num_1;
}
bool operator == (const BigInteger& num_1,
                  const BigInteger& num_2) {
    return !((num_2 < num_1) || (num_1 < num_2));
}
bool operator != (const BigInteger& num_1,
                  const BigInteger& num_2) {
    return !(num_1 == num_2);
}
bool operator <= (const BigInteger& num_1,
                  const BigInteger& num_2) {
    return (num_1 < num_2) || (num_1 == num_2);
}
bool operator >= (const BigInteger& num_1,
                  const BigInteger& num_2) {
    return (num_1 > num_2) || (num_1 == num_2);
}
//Output operator
std::ostream& operator << (std::ostream& output,
                           const BigInteger& biggie) {
    output << biggie.toString();
    return output;
}
//Input operator
std::istream& operator >> (std::istream& input, BigInteger& biggie) {
    input.tie(nullptr);
    biggie.clear();
    std::string input_num;
    input >> input_num;
    biggie = BigInteger(input_num);
    return input;
}
//######################################################################################################################
//######################################################################################################################
class Rational {
public:
    //Constructors
    Rational() =default;
    Rational(const BigInteger& num): numerator(num) {}
    Rational(int num): numerator(num) {}
    //Operators
    Rational& operator += (const Rational& num) {
        numerator *= num.denominator;
        numerator += num.numerator * denominator;
        denominator *= num.denominator;
        shrink();
        return *this;
    }
    Rational& operator -= (const Rational& num) {
        numerator *= num.denominator;
        numerator -= num.numerator * denominator;
        denominator *= num.denominator;
        shrink();
        return *this;
    }
    Rational& operator *= (const Rational& num) {
        numerator *= num.numerator;
        denominator *= num.denominator;
        shrink();
        return *this;
    }
    Rational& operator /= (const Rational& num) {
        numerator *= num.denominator;
        if (num.numerator < 0) {
            numerator *= -1;
            denominator *= (-num.numerator);
        } else {
            denominator *= num.numerator;
        }
        shrink();
        return *this;
    }
    friend bool operator < (const Rational& num_1,
                            const Rational& num_2);
    explicit operator double() {
        std::string output = asDecimal(20);
        return std::atof(output.c_str());
    }
    //Methods
    std::string toString() const {
        std::string output = numerator.toString();
        if (denominator != 1) {
            output.push_back('/');
            output += denominator.toString();
        }
        return output;
    }
    std::string asDecimal(size_t precision = 0) {
        std::string precised;
        BigInteger copy = numerator;
        if (numerator * denominator < 0 && copy / denominator == 0) precised += '-';
        precised += (copy / denominator).toString();
        copy = copy.abs();
        copy %= denominator;
        for (size_t i = 0; i <= precision; ++i) {
            if (i == 0) precised.push_back('.');
            else {
                copy *= 10;
                precised += (copy / denominator).toString();
                copy %= denominator;
            }
        }
        return precised;
    }
private:
    BigInteger numerator;
    BigInteger denominator = 1;
    //Methods
    BigInteger gcd(const BigInteger& num_1,
                   const BigInteger& num_2) {
        if (num_2 == 0) return num_1;
        return gcd(num_2, num_1 % num_2);
    }
    void shrink() {
        BigInteger num = numerator.abs();
        BigInteger den = denominator.abs();
        if (denominator != 1) {
            while (den != 0) {
                std::swap(num, den);
                den %= num;
            }
            BigInteger gcf = num;
            if (gcf != 1) {
                numerator /= gcf;
                denominator /= gcf;
            }
        }
    }
};
//Operators
Rational operator + (const Rational& num_1,
                     const Rational& num_2) {
    Rational copy = num_1;
    copy += num_2;
    return copy;
}
Rational operator - (const Rational& num_1,
                     const Rational& num_2) {
    Rational copy = num_1;
    copy -= num_2;
    return copy;
}
Rational operator * (const Rational& num_1,
                     const Rational& num_2) {
    Rational copy = num_1;
    copy *= num_2;
    return copy;
}
Rational operator / (const Rational& num_1,
                     const Rational& num_2) {
    Rational copy = num_1;
    copy /= num_2;
    return copy;
}
Rational operator - (const Rational& num_1) {
    Rational copy = num_1;
    copy *= -1;
    return copy;

}
//Logical operators
bool operator < (const Rational& num_1,
                 const Rational& num_2) {
    return num_1.numerator * num_2.denominator < num_2.numerator * num_1.denominator;
}
bool operator > (const Rational& num_1,
                 const Rational& num_2) {
    return num_2 < num_1;
}
bool operator == (const Rational& num_1,
                  const Rational& num_2) {
    return !(num_2 < num_1 || num_1 < num_2);
}
bool operator != (const Rational& num_1,
                  const Rational& num_2) {
    return !(num_1 == num_2);
}
bool operator <= (const Rational& num_1,
                  const Rational& num_2) {
    return !(num_1 > num_2);
}
bool operator >= (const Rational& num_1,
                  const Rational& num_2) {
    return !(num_1 < num_2);
}
std::istream& operator >> (std::istream& input,
                           Rational& rattie) {
    input.tie(nullptr);
    std::string input_num;
    input >> input_num;
    rattie = Rational(input_num);
    return input;
}
std::ostream& operator << (std::ostream& output,
                           const Rational& biggie) {
    output << biggie.toString();
    return output;
}
