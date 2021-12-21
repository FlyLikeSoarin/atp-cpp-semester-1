#include <vector>
#include <iostream>
#include <string>
 
class BigInteger;
BigInteger operator*(const BigInteger&, const BigInteger&);
BigInteger operator/(const BigInteger&, const BigInteger&);
BigInteger operator-(const BigInteger&, const BigInteger&);
class BigInteger {
public:
  BigInteger() {}
  BigInteger(long long num) : sign_(num >= 0 ? 1 : -1) {
    num = (num >= 0 ? num : -num);
    if (num == 0) {
      bigint_.push_back(0);
      size_ = 1;
    }
    while (num != 0) {
      bigint_.push_back(num % base_);
      num /= base_;
      ++size_;
    }
  }
  BigInteger(const BigInteger& num) = default;
  BigInteger& operator=(const BigInteger& num) {
    size_ = num.size_;
    sign_ = num.sign_;
    bigint_.resize(num.size_);
    for (size_t i = 0; i < num.size_; ++i) {
      bigint_[i] = num.bigint_[i];
    }
    return *this;
  }
  ~BigInteger() = default;
 
  explicit operator bool() {
    return *this != 0;
  }
 
  std::string toString() const {
    std::string str;
    if (sign_ == -1) {
      str.push_back('-');
    }
    str += std::to_string(bigint_[size_ - 1]);
    for (int i = size_ - 2; i >= 0; --i) {
      long long a = bigint_[i];
      size_t cnt = 0;
      if (a == 0) {
        cnt = 1;
      }
      while (a != 0) {
        ++cnt;
        a /= 10;
      }
      for (size_t j = 0; j < length_base_ - cnt; ++j) {
        str.push_back('0');
      }
      str += std::to_string(bigint_[i]);
    }
    return str;
  }
 
  BigInteger operator-() const {
    BigInteger copy = *this;
    if (copy != 0) {
      copy.sign_ = sign_ * (-1);
    }
    return copy;
  }
 
  BigInteger& operator+=(long long num) {
    return *this = PlusLongLong(num, true);
  }
 
  BigInteger& operator+=(const BigInteger& num) {
    return *this = PlusBigInt(num, true);
  }
 
  BigInteger& operator-=(long long num) {
    return *this = PlusLongLong(num, false);
  }
 
  BigInteger& operator-=(const BigInteger& num) {
    return *this = PlusBigInt(num, false);
  }
 
  BigInteger& operator++() {
    return *this += 1;
  }
  BigInteger operator++(int) {
    BigInteger copy = *this;
    *this += 1;
    return copy;
  }
 
  BigInteger& operator--() {
    return *this -= 1;
  }
 
  BigInteger operator--(int) {
    BigInteger copy = *this;
    *this -= 1;
    return copy;
  }
 
  BigInteger& operator*=(const BigInteger& num) {
    std::vector<long long> result(size_ + num.size_, 0);
    for (size_t ind2 = 0; ind2 < num.size_; ++ind2) {
      for (size_t ind1 = 0; ind1 < size_; ++ind1) {
        result[ind1 + ind2] += bigint_[ind1] * num.bigint_[ind2];
        result[ind1 + ind2 + 1] += result[ind1 + ind2] / base_;
        result[ind1 + ind2] %= base_;
      }
    }
    bigint_ = result;
    delZeros();
    sign_ = (sign_ == num.sign_ || (size_ == 1 && bigint_[0] == 0) ? 1 : -1);
    return *this;
  }

  BigInteger& operator/=(const BigInteger& num) {
    int res_sign = sign_ * num.sign_;
    BigInteger divider = (num < 0 ? -num : num);
    BigInteger divisible = 0;
    BigInteger result = 0;
    sign_ = 1;
    for (int ind1 = bigint_.size() - 1; ind1 >= 0; --ind1) {
      divisible *= base_;
      divisible += bigint_[ind1];
      size_t left = 0;
      size_t right = base_;
      while (right - left > 1) {
        size_t mid = (left + right) >> 1;
        if (divider * mid <= divisible) {
          left = mid;
        }
        else {
          right = mid;
        }
      }
      divisible -= divider * left;
      result *= base_;
      result += left;
    }
    if (result != 0) {
      result.sign_ = res_sign;
    }
    return *this = result;
  }
 
  BigInteger& operator%=(const BigInteger& num) {
    return *this = DivideRemainder(num, '%');
  }
 
  friend bool operator<(const BigInteger&, const BigInteger&);
  friend bool operator>(const BigInteger&, const BigInteger&);
  friend bool operator<=(const BigInteger&, const BigInteger&);
  friend bool operator>=(const BigInteger&, const BigInteger&);
  friend bool operator==(const BigInteger&, const BigInteger&);
  friend bool operator!=(const BigInteger&, const BigInteger&);
  friend std::istream& operator>>(std::istream&, BigInteger&);
  friend std::ostream& operator<<(std::ostream&, const BigInteger&);
 
private:
  int sign_ = 1;
  size_t size_ = 0;
  std::vector<long long> bigint_;
  static const size_t base_ = 1000000000;
  static const size_t length_base_ = 9;
 
  int cmp_abs_(const BigInteger& num) const {
    if (size_ != num.size_)
      return (size_ > num.size_ ? 1 : -1);
    for (int i = size_ - 1; i >= 0; --i) {
      if (bigint_[i] != num.bigint_[i])
        return (bigint_[i] > num.bigint_[i] ? 1 : -1);
    }
    return 1;
  }
 
  void Put(size_t index, long long value) {
    if (size_ > index) {
      bigint_[index] = value;
    }
    else {
      bigint_.push_back(value);
      ++size_;
    }
  }
 
  void delZeros() {
    while (bigint_.size() != 1 && bigint_[bigint_.size() - 1] == 0) {
      bigint_.pop_back();
    }
    size_ = bigint_.size();
  }

  BigInteger& PlusBigInt(const BigInteger& num, bool isPositive) {
    int res_sign;
    if (isPositive) {
      res_sign = (sign_ == num.sign_ ? sign_ : sign_ * cmp_abs_(num);
    }
    else {
      res_sign = (sign_ != num.sign_ ? sign_ : num.sign * cmp_abs_(num);
    }
    long long carry = 0;
    for (size_t i = 0; i < num.size_ || carry != 0; ++i) {
      long long sum = res_sign * (sign_ * (i < size_ ? bigint_[i] : 0) +
        (isPositive ? 1 : -1) *  num.sign_ * (i < num.size_ ? num.bigint_[i] : 0))
        + carry;
      carry = (sum < 0 ? -1 : sum / base_);
      Put(i, (sum + base_) % base_);
    }
    delZeros();
    sign_ = res_sign;
  }

  BigInteger& PlusLongLong(long long num, bool isPositive) {
    int num_sign = (num >= 0 ? 1 : -1);
    int res_sign;
    if (isPositive) {
      res_sign = (sign_ == num_sign ? sign_ : sign_ * cmp_abs_(num));
    }
    else {
      res_sign = (sign != num.sign_ ? sign_ : num.sign * cmp_abs_(num);
    }
    long long carry = 0;
    for (size_t i = 0; carry != 0 || num != 0; ++i) {
      long long sum = res_sign * (sign_ * (i < size_ ? bigint_[i] : 0) +
        (isPositive ? 1 : -1) * num_sign * (num % base_)) + carry;
      carry = (sum < 0 ? -1 : sum / base_);
      Put(i, (sum + base_) % base_);
      num /= base_;
    }
    delZeros();
    sign_ = res_sign;
    return *this;
  }
 
  BigInteger& DivideRemainder(BigInteger num, char query) {
    int res_sign = (sign_ == num.sign_ || cmp_abs_(num) != 1 ? 1 : -1);
    bool flag = (sign_ == -1);
    sign_ = 1;
    BigInteger result = 0;
    for (int ind1 = size_ - 1; ind1 >= 0; --ind1) {
      BigInteger div = 0;
      for (size_t i = 0; i < size_t(ind1); ++i) {
        ++div.size_;
        div.bigint_.push_back(0);
      }
      size_t left = 0;
      size_t right = base_;
      while (right - left > 1) {
        size_t mid = (left + right) >> 1;
        div.bigint_[ind1] = mid;
        if (num.sign_ * num * div <= *this) {
          left = mid;
        }
        else {
          right = mid;
        }
      }
      div.bigint_[ind1] = left;
      result += div;
      *this -= num.sign_ * num * div;
    }
    if (query == '/') {
      *this = result;
      sign_ = res_sign;
      return *this;
    }
    else {
      if (flag) {
        sign_ *= -1;
      }
      return *this;
    }
  }
};
 
bool operator<(const BigInteger& first, const BigInteger& second) {
  if (first.sign_ != second.sign_) {
    return first.sign_ == -1;
  }
  if (first.size_ != second.size_) {
    return first.sign_ * first.size_ < second.sign_ * second.size_;
  }
  for (int i = first.size_ - 1; i >= 0; --i) {
    if (first.bigint_[i] != second.bigint_[i]) {
      return first.sign_ * first.bigint_[i] < second.sign_ * second.bigint_[i];
    }
  }
  return false;
}
 
bool operator==(const BigInteger& first, const BigInteger& second) {
  if (first.sign_ != second.sign_ || first.size_ != second.size_) {
    return false;
  }
  for (size_t i = 0; i < first.size_; ++i) {
    if (first.bigint_[i] != second.bigint_[i])
      return false;
  }
  return true;
}
 
bool operator>(const BigInteger& first, const BigInteger& second) {
  return second < first;
}
 
bool operator!=(const BigInteger& first, const BigInteger& second) {
  return !(first == second);
}
 
bool operator<=(const BigInteger& first, const BigInteger& second) {
  return first < second || first == second;
}
 
bool operator>=(const BigInteger& first, const BigInteger& second) {
  return second < first || first == second;
}
 
BigInteger operator+(const BigInteger& first, const BigInteger& second) {
  BigInteger copy = first;
  copy += second;
  return copy;
}
 
BigInteger operator-(const BigInteger& first, const BigInteger& second) {
  BigInteger copy = first;
  copy -= second;
  return copy;
}
 
BigInteger operator*(const BigInteger& first, const BigInteger& second) {
  BigInteger copy = first;
  copy *= second;
  return copy;
}
 
BigInteger operator/(const BigInteger& first, const BigInteger& second) {
  BigInteger copy = first;
  copy /= second;
  return copy;
}
 
BigInteger operator%(const BigInteger& first, const BigInteger& second) {
  BigInteger copy = first;
  copy %= second;
  return copy;
}
std::istream& operator>>(std::istream& in, BigInteger& num) {
  std::string str;
  in >> str;
  if (str[0] == '-') {
    num.sign_ = -1;
    str = str.substr(1, str.size() - 1);
  }
  else {
    num.sign_ = 1;
  }
  num.size_ = (str.size() + num.length_base_ - 1) / num.length_base_;
  num.bigint_.resize(num.size_);
  int start = str.size() - num.length_base_;
  for (size_t i = 0; i < num.size_; ++i) {
    num.bigint_[i] = std::stoi(start >= 0 ? (str.substr(start, num.length_base_)) : str.substr(0, num.length_base_ + start));
    start -= num.length_base_;
  }
  return in;
}
 
std::ostream& operator<<(std::ostream& out, const BigInteger& num) {
  out << num.toString();
  return out;
}
 
class Rational {
public:
  Rational() {}
  Rational(const BigInteger& num) : numerator_(num) {}
  Rational(long long num) : numerator_(num) {}
  Rational(const Rational& num) : numerator_(num.numerator_), denominator_(num.denominator_) {};
 
  Rational& operator=(const Rational& num) {
    numerator_ = num.numerator_;
    denominator_ = num.denominator_;
    return *this;
  }
  ~Rational() = default;
 
  friend bool operator<(const Rational&, const Rational&);
  friend bool operator>(const Rational&, const Rational&);
  friend bool operator==(const Rational&, const Rational&);
  friend bool operator!=(const Rational&, const Rational&);
  friend bool operator<=(const Rational&, const Rational&);
  friend bool operator>=(const Rational&, const Rational&);
 
  Rational operator-() const {
    Rational copy = *this;
    copy.numerator_ *= -1;
    return copy;
  }
  Rational& operator+=(const Rational& num) {
    if (denominator_ == num.denominator_) {
      numerator_ += num.numerator_;
      reduce_();
      return *this;
    }
    numerator_ = numerator_ * num.denominator_ + num.numerator_ * denominator_;
    denominator_ *= num.denominator_;
    reduce_();
    return *this;
  }
 
  Rational& operator-=(const Rational& num) {
    if (denominator_ == num.denominator_) {
      numerator_ -= num.numerator_;
      reduce_();
      return *this;
    }
    numerator_ = numerator_ * num.denominator_ - num.numerator_ * denominator_;
    denominator_ *= num.denominator_;
    reduce_();
    return *this;
  }
 
  Rational& operator*=(const Rational& num) {
    numerator_ *= num.numerator_;
    denominator_ *= num.denominator_;
    reduce_();
    return *this;
  }
 
  Rational& operator/=(const Rational& num) {
    numerator_ *= num.denominator_;
    denominator_ *= num.numerator_;
    if (denominator_ < 0) {
      numerator_ = -numerator_;
      denominator_ = -denominator_;
    }
    reduce_();
    return *this;
  }
 
  std::string toString() const {
    std::string str;
    str += numerator_.toString();
    if (denominator_ != 1) {
      str.push_back('/');
      str += denominator_.toString();
    }
    return str;
  }
 
  std::string asDecimal(size_t precision = 0) const {
    std::string str;
    if (numerator_ < 0) {
      str.push_back('-');
      str += ((-numerator_) / denominator_).toString();
    }
    else {
      str += (numerator_ / denominator_).toString();
    }
    if (precision > 0) {
      str.push_back('.');
      BigInteger copy_num = numerator_;
      if (copy_num < 0) {
        copy_num = -copy_num;
      }
      for (size_t i = 0; i < precision; ++i) {
        copy_num %= denominator_;
        copy_num *= 10;
        str += (copy_num / denominator_).toString();
      }
    }
    return str;
  }
 
  explicit operator double() const {
    return std::stod(asDecimal(18));
  }
 
private:
  BigInteger numerator_;
  BigInteger denominator_ = 1;
  void reduce_() {
    BigInteger a;
    if (numerator_ < 0) {
      a = -numerator_;
    }
    else {
      a = numerator_;
    }
    BigInteger b = denominator_;
    while (b != 0) {
      a %= b;
      std::swap(a, b);
    }
    numerator_ /= a;
    denominator_ /= a;
  }
};
 
bool operator<(const Rational& first, const Rational& second) {
  return first.numerator_ * second.denominator_ < second.numerator_ * first.denominator_;
}
bool operator==(const Rational& first, const Rational& second) {
  return first.numerator_ * second.denominator_ == second.numerator_ * first.denominator_;
}
bool operator>(const Rational& first, const Rational& second) {
  return second < first;
}
bool operator!=(const Rational& first, const Rational& second) {
  return !(first == second);
}
bool operator<=(const Rational& first, const Rational& second) {
  return first < second || first == second;
}
bool operator>=(const Rational& first, const Rational& second) {
  return second < first || first == second;
}
 
Rational operator+(const Rational& first, const Rational& second) {
  Rational copy = first;
  copy += second;
  return copy;
}
Rational operator-(const Rational& first, const Rational& second) {
  Rational copy = first;
  copy -= second;
  return copy;
}
Rational operator*(const Rational& first, const Rational& second) {
  Rational copy = first;
  copy *= second;
  return copy;
}
Rational operator/(const Rational& first, const Rational& second) {
  Rational copy = first;
  copy /= second;
  return copy;
}
