//
// Created by vsvood on 11/18/21.
//

#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

#ifndef BIGINT_BIGINTEGER_H
#define BIGINT_BIGINTEGER_H

class BigInteger {
 public:
  BigInteger();
  BigInteger(int source);

  BigInteger& operator+=(const BigInteger& other);
  BigInteger& operator-=(const BigInteger& other);
  BigInteger& operator*=(const BigInteger& other);
  BigInteger& operator/=(const BigInteger& other);
  BigInteger& operator%=(const BigInteger& other);

  BigInteger& operator++();
  BigInteger& operator--();
  const BigInteger operator++(int);
  const BigInteger operator--(int);

  BigInteger operator<<(uint64_t shift) const;

  BigInteger operator-() const;
  BigInteger babs() const;

  bool operator<(const BigInteger& other) const;
  bool operator>(const BigInteger& other) const;
  bool operator<=(const BigInteger& other) const;
  bool operator>=(const BigInteger& other) const;
  bool operator==(const BigInteger& other) const;
  bool operator!=(const BigInteger& other) const;

  bool is_negative() const;

  [[nodiscard]] std::string toString() const;
  std::string toStringAsFixedPoint(uint64_t precision) const;

  explicit operator bool() const;

  friend std::istream& operator>>(std::istream& in, BigInteger& number);
  friend std::ostream& operator<<(std::ostream& out, const BigInteger& number);

  static const uint64_t base;

 private:
  [[nodiscard]] BigInteger RightShift() const;
  [[nodiscard]] uint64_t size() const;

  bool is_negative_;
  uint64_t last_significant_digit_;
  std::vector<uint32_t> big_int_;
};

BigInteger operator+(const BigInteger& first, const BigInteger& second);
BigInteger operator-(const BigInteger& first, const BigInteger& second);
BigInteger operator*(const BigInteger& first, const BigInteger& second);
BigInteger operator/(const BigInteger& first, const BigInteger& second);
BigInteger operator%(const BigInteger& first, const BigInteger& second);

const uint64_t BigInteger::base = 1'000'000'000;

BigInteger::BigInteger()
    : is_negative_(false), last_significant_digit_(0), big_int_(1, 0) {}

BigInteger::BigInteger(int source)
    : is_negative_(source < 0),
      last_significant_digit_(source >= static_cast<int64_t>(base)),
      big_int_{static_cast<unsigned int>(abs(source)) %
                   static_cast<unsigned int>(base),
               static_cast<unsigned int>(abs(source)) /
                   static_cast<unsigned int>(base)} {}

BigInteger& BigInteger::operator+=(const BigInteger& other) {
  ////////std::cerr << "+=" << *this << ' ' << other << '\n';
  if (is_negative_ != other.is_negative_) {
    is_negative_ = !is_negative_;
    *this -= other;
    is_negative_ = !is_negative_;
    return *this;
  }

  uint64_t accumulator = 0;
  uint64_t new_last_significant_digit = 0;
  for (uint64_t i = 0; i <= std::max(this->last_significant_digit_,
                                     other.last_significant_digit_) ||
                       accumulator;
       ++i) {
    if (i == this->size()) {
      big_int_.push_back(0);
    }
    accumulator += big_int_[i];
    accumulator += (i < other.size()) ? other.big_int_[i] : 0;
    big_int_[i] = accumulator % base;
    if (big_int_[i] != 0) new_last_significant_digit = i;
    accumulator /= base;
  }
  last_significant_digit_ = new_last_significant_digit;
  return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& other) {
  if (is_negative_ != other.is_negative_) {
    is_negative_ = !is_negative_;
    *this += other;
    is_negative_ = !is_negative_;
    return *this;
  }

  BigInteger other_number = other;
  if ((*this < other_number) != is_negative_) {
    std::swap(*this, other_number);
    is_negative_ = !is_negative_;
  }
  int64_t accumulator = 0;
  uint64_t new_last_significant_digit = 0;
  for (uint64_t i = 0; i <= std::max(this->last_significant_digit_,
                                     other_number.last_significant_digit_) ||
                       accumulator;
       ++i) {
    if (i == this->size()) {
      big_int_.push_back(0);
    }
    accumulator += big_int_[i];
    accumulator -= (i < other_number.size()) ? other_number.big_int_[i] : 0;
    big_int_[i] = (accumulator + base) % base;
    if (big_int_[i] != 0) new_last_significant_digit = i;
    if (accumulator < 0) {
      accumulator = -1;
    } else {
      accumulator = 0;
    }
  }
  last_significant_digit_ = new_last_significant_digit;
  return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger& other) {
  BigInteger old = *this;

  uint64_t number_of_digits =
      last_significant_digit_ + other.last_significant_digit_ + 2;
  big_int_.clear();
  big_int_.resize(number_of_digits, 0);

  uint64_t sum = 0;
  uint64_t accumulator = 0;
  uint64_t new_last_significant_digit = 0;
  for (uint64_t i = 0; i <= last_significant_digit_; ++i) {
    if (old.big_int_[i] == 0) {
      continue;
    }
    accumulator = 0;
    for (uint64_t j = 0; j <= other.last_significant_digit_ || sum; ++j) {
      accumulator = old.big_int_[i];
      accumulator *= (j < other.size()) ? other.big_int_[j] : 0;
      sum += accumulator % base;
      sum += big_int_[i + j];
      big_int_[i + j] = sum % base;
      if (big_int_[i + j] != 0) new_last_significant_digit = i + j;
      accumulator /= base;
      sum /= base;
      sum += accumulator;
    }
  }

  last_significant_digit_ = new_last_significant_digit;
  is_negative_ = (is_negative_ != other.is_negative_);

  return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger& other) {
  /*if (*this == 0) {
    return *this;
  }*/
  BigInteger old = this->babs();
  bool negative_sign = is_negative_ != other.is_negative_;
  *this = BigInteger(0);
  BigInteger partial_dividend = 0;
  BigInteger divider = other.babs();
  bool last_digit_processed = false;
  while (!last_digit_processed) {
    partial_dividend *= base;
    partial_dividend += static_cast<int>(old.big_int_[old.last_significant_digit_]);
    if (old.last_significant_digit_ != 0) {
      --old.last_significant_digit_;
    } else {
      last_digit_processed = true;
    }
    int32_t less_quotient = 0;
    int32_t greater_quotient = base;
    while (greater_quotient - less_quotient > 1) {
      int32_t medium_quotient = (less_quotient + greater_quotient)/2;
      if (divider*medium_quotient <= partial_dividend) {
        less_quotient = medium_quotient;
      } else {
        greater_quotient = medium_quotient;
      }
    }
    partial_dividend -= divider*less_quotient;
    *this *= base;
    *this += less_quotient;
  }
  this->is_negative_ = negative_sign;
  return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger& other) {
  // //std::cerr << "% " << *this << ' ' << other << '\n';
  BigInteger quotient = *this;
  (quotient /= other) *= other;
  *this -= quotient;
  // //std::cerr << *this << "\n\n";
  return *this;
}

BigInteger& BigInteger::operator++() { return *this += 1; }

BigInteger& BigInteger::operator--() { return *this -= 1; }

const BigInteger BigInteger::operator++(int) {
  BigInteger copy = *this;
  *this += 1;
  return copy;
}

const BigInteger BigInteger::operator--(int) {
  BigInteger copy = *this;
  *this -= 1;
  return copy;
}

BigInteger BigInteger::operator<<(uint64_t shift) const {
  BigInteger result;
  result.big_int_.resize(this->last_significant_digit_ + shift + 1, 0);
  for (uint64_t i = shift; i <= shift + last_significant_digit_; ++i) {
    result.big_int_[i] = this->big_int_[i - shift];
  }
  result.last_significant_digit_ = result.big_int_.size() - 1;
  result.is_negative_ = this->is_negative_;
  return result;
}

BigInteger BigInteger::operator-() const {
  ////////std::cerr << "called - for" << *this << '\n';
  BigInteger result = *this;
  result.is_negative_ = !is_negative_;
  return result;
}

bool BigInteger::operator<(const BigInteger& other) const {
  if (other.last_significant_digit_ == 0 && last_significant_digit_ == 0 &&
      big_int_[0] == 0 && other.big_int_[0] == 0) {
    return false;
  }
  if (is_negative_ > other.is_negative_) {
    return true;
  }
  if (is_negative_ < other.is_negative_) {
    return false;
  }
  if (last_significant_digit_ < other.last_significant_digit_) {
    return !is_negative_;
  }
  if (last_significant_digit_ > other.last_significant_digit_) {
    return is_negative_;
  }
  for (uint64_t i = last_significant_digit_ + 1; i > 0; --i) {
    if (big_int_[i - 1] < other.big_int_[i - 1]) return !is_negative_;
    if (big_int_[i - 1] > other.big_int_[i - 1]) return is_negative_;
  }
  return is_negative_;
}

bool BigInteger::operator>(const BigInteger& other) const {
  return !(*this < other) && !(*this == other);
}

bool BigInteger::operator<=(const BigInteger& other) const {
  return (*this < other) || (*this == other);
}

bool BigInteger::operator>=(const BigInteger& other) const {
  return (*this > other) || (*this == other);
}

bool BigInteger::operator==(const BigInteger& other) const {
  return !(*this < other) && !(other < *this);
}

bool BigInteger::operator!=(const BigInteger& other) const {
  return (*this < other) || (other < *this);
}

std::istream& operator>>(std::istream& in, BigInteger& number) {
  std::string string_representation;
  in >> string_representation;
  // ////std::cerr << "in:" << string_representation << '\n';
  number.big_int_.clear();
  number.last_significant_digit_ = 0;
  number.is_negative_ = false;
  uint32_t digit = 0;
  uint64_t place = 1;
  for (auto p = string_representation.rbegin();
       p < string_representation.rend(); ++p) {
    if (*p == '-') {
      number.is_negative_ = true;
      break;
    }
    if (place < BigInteger::base) {
      digit += place * (*p - '0');
      place *= 10;
    } else {
      number.big_int_.push_back(digit);
      if (number.big_int_[number.big_int_.size() - 1] != 0) {
        number.last_significant_digit_ = number.big_int_.size() - 1;
      }
      digit = (*p - '0');
      place = 10;
    }
  }

  number.big_int_.push_back(digit);
  if (number.big_int_[number.big_int_.size() - 1] != 0) {
    number.last_significant_digit_ = number.big_int_.size() - 1;
  }

  // ////std::cerr << "in:" << number << '\n';

  return in;
}

std::ostream& operator<<(std::ostream& out, const BigInteger& number) {
  ////////std::cerr << "out:";
  if (number.is_negative_ &&
      (number.last_significant_digit_ != 0 || number.big_int_[0] != 0)) {
    ////////std::cerr << '-';
    out << '-';
  }
  out << number.big_int_[number.last_significant_digit_];
  ////////std::cerr << number.big_int_[number.last_significant_digit_];
  for (uint64_t i = number.last_significant_digit_; i > 0; --i) {
    for (uint64_t j =
             (number.big_int_[i - 1] == 0) ? 0 : log10(number.big_int_[i - 1]);
         j < 8; ++j) {
      ////////std::cerr << '0';
      out << '0';
    }
    ////////std::cerr << number.big_int_[i - 1];
    out << number.big_int_[i - 1];
  }
  ////////std::cerr << '\n';
  return out;
}

BigInteger BigInteger::RightShift() const {
  BigInteger result = *this;
  for (uint64_t i = 0; i <= last_significant_digit_; ++i) {
    result.big_int_[i] /= 2;
    if (i + 1 < result.big_int_.size() && result.big_int_[i + 1] % 2 == 1) {
      result.big_int_[i] += base / 2;
    }
    if (result.big_int_[i] != 0) result.last_significant_digit_ = i;
  }

  return result;
}

uint64_t BigInteger::size() const { return big_int_.size(); }

std::string BigInteger::toString() const {
  std::stringstream str_stream;
  str_stream << *this;
  return str_stream.str();
}

std::string BigInteger::toStringAsFixedPoint(uint64_t precision) const {
  uint64_t number_of_decimal_places =
      static_cast<uint64_t>(log10(BigInteger::base));
  uint64_t number_of_additional_digits =
      (precision + number_of_decimal_places) / number_of_decimal_places;
  BigInteger copy = *this;
  if (copy.is_negative()) {
    copy -= pow(10, number_of_decimal_places - precision % number_of_decimal_places - 1) *
            5;
  } else {
    copy += pow(10, (2 * number_of_decimal_places - precision - 1) %
                    number_of_decimal_places) *
            5;
  }
  std::stringstream result;
  /*if (is_negative_ && (last_significant_digit_ != 0 || copy.big_int_[0] != 0)) {
    result << '-';
  }*/
  bool is_null = true;
  if (last_significant_digit_ >= number_of_additional_digits) {
    result << copy.big_int_[last_significant_digit_];
    if (copy.big_int_[last_significant_digit_] != 0) {
      is_null = false;
    }
  } else {
    result << 0;
  }
  for (uint64_t i = last_significant_digit_; i > number_of_additional_digits;
       --i) {
    for (uint64_t j = (copy.big_int_[i - 1] == 0) ? 0
                                                  : log10(copy.big_int_[i - 1]);
         j < 8; ++j) {
      result << '0';
    }
    result << copy.big_int_[i - 1];
    if (copy.big_int_[i - 1] != 0) {
      is_null = false;
    }
  }
  if (precision == 0) {
    if (is_negative_ && !is_null) {
      return '-' + result.str();
    }
    return result.str();
  }

  result << '.';
  uint64_t last_pos = 0;
  for (uint64_t i = number_of_additional_digits - 1;
       (number_of_additional_digits - i) * number_of_decimal_places <=
       precision;
       --i) {
    if (i >= copy.big_int_.size()) {
      result << "000000000";
      continue;
    }
    for (uint64_t j = (copy.big_int_[i] == 0) ? 0 : log10(copy.big_int_[i]);
         j < 8; ++j) {
      result << '0';
    }
    result << copy.big_int_[i];
    if (copy.big_int_[i] != 0) {
      is_null = false;
    }
    last_pos = i - 1;
  }
  precision %= number_of_decimal_places;
  if (precision == 0)  {
    if (is_negative_ && !is_null) {
      return '-' + result.str();
    }
    return result.str();
  }

  uint64_t delimiter = pow(10, number_of_decimal_places - precision);
  uint32_t last_digit = copy.big_int_[last_pos] / delimiter;
  for (uint64_t j =
           (last_digit == 0) ? 0 : log10(last_digit);
       j < 8-(number_of_decimal_places - precision); ++j) {
    result << '0';
  }
  result << last_digit;
  if (last_digit != 0) {
    is_null = false;
  }

  if (is_negative_ && !is_null) {
    return '-' + result.str();
  }
  return result.str();
}

BigInteger::operator bool() const {
  if (last_significant_digit_ == 0 && big_int_[0] == 0) return false;
  return true;
}

bool BigInteger::is_negative() const { return is_negative_; }

BigInteger BigInteger::babs() const {
  if (this->is_negative_) {
    return -*this;
  }
  return *this;
}

BigInteger operator+(const BigInteger& first, const BigInteger& second) {
  BigInteger sum = first;
  sum += second;
  return sum;
}

BigInteger operator-(const BigInteger& first, const BigInteger& second) {
  BigInteger difference = first;
  difference -= second;
  return difference;
}

BigInteger operator*(const BigInteger& first, const BigInteger& second) {
  BigInteger product = first;
  product *= second;
  return product;
}

BigInteger operator/(const BigInteger& first, const BigInteger& second) {
  BigInteger quotient = first;
  quotient /= second;
  return quotient;
}

BigInteger operator%(const BigInteger& first, const BigInteger& second) {
  BigInteger remainder = first;
  remainder %= second;
  return remainder;
}

////////////////////////////////////////deadline////////////////////////////////////////////
/// FixMe

class Rational {
 public:
  Rational();
  Rational(int source);
  Rational(const BigInteger& source);

  Rational& operator=(const Rational& other);

  Rational& operator+=(const Rational& other);
  Rational& operator-=(const Rational& other);
  Rational& operator*=(const Rational& other);
  Rational& operator/=(const Rational& other);

  Rational& operator++();
  Rational& operator--();
  Rational operator++(int);
  Rational operator--(int);

  Rational operator-() const;

  bool operator<(const Rational& other) const;
  bool operator>(const Rational& other) const;
  bool operator<=(const Rational& other) const;
  bool operator>=(const Rational& other) const;
  bool operator==(const Rational& other) const;
  bool operator!=(const Rational& other) const;

  [[nodiscard]] std::string toString() const;
  std::string asDecimal(size_t precision = 0) const;

  explicit operator double() const;

 private:
  void Reduce();
  static BigInteger bgcd(const BigInteger& first, const BigInteger& second);

  BigInteger enumerator_;
  BigInteger denominator_;
};

Rational operator+(const Rational& first, const Rational& second);
Rational operator-(const Rational& first, const Rational& second);
Rational operator*(const Rational& first, const Rational& second);
Rational operator/(const Rational& first, const Rational& second);
Rational operator%(const Rational& first, const Rational& second);

Rational::Rational() : enumerator_(0), denominator_(1) {}

Rational::Rational(int source) : enumerator_(source), denominator_(1) {}

Rational::Rational(const BigInteger& source)
    : enumerator_(source), denominator_(1) {}

Rational& Rational::operator+=(const Rational& other) {
  //std::cerr << "+= " << this->toString() << ' ' << other.toString() << '\n';
  enumerator_ *= other.denominator_;
  enumerator_ += other.enumerator_ * denominator_;
  denominator_ *= other.denominator_;
  //std::cerr << "= " << this->toString() << '\n';
  Reduce();
  //std::cerr << "= " << this->toString() << '\n';
  return *this;
}

Rational& Rational::operator-=(const Rational& other) {
  //std::cerr << "-= " << this->toString() << ' ' << other.toString() << '\n';
  enumerator_ *= other.denominator_;
  enumerator_ -= other.enumerator_ * denominator_;
  denominator_ *= other.denominator_;
  //std::cerr << "= " << this->toString() << '\n';
  Reduce();
  //std::cerr << "= " << this->toString() << '\n';
  return *this;
}

Rational& Rational::operator*=(const Rational& other) {
  //std::cerr << "*="  << this->toString() << ' ' << other.toString() << '\n';
  enumerator_ *= other.enumerator_;
  denominator_ *= other.denominator_;
  //std::cerr << "= " << this->toString() << '\n';
  Reduce();
  //std::cerr << "= " << this->toString() << '\n';
  return *this;
}

Rational& Rational::operator/=(const Rational& other) {
  //std::cerr << "/="  << this->toString() << ' ' << other.toString() << '\n';
  enumerator_ *= other.denominator_;
  denominator_ *= other.enumerator_;
  //std::cerr << "= " << this->toString() << '\n';
  Reduce();
  //std::cerr << "= " << this->toString() << '\n';
  return *this;
}

Rational& Rational::operator++() {
  enumerator_ += denominator_;
  Reduce();
  return *this;
}

Rational& Rational::operator--() {
  enumerator_ -= denominator_;
  Reduce();
  return *this;
}

Rational Rational::operator++(int) {
  Rational copy = *this;
  enumerator_ += denominator_;
  Reduce();
  return copy;
}

Rational Rational::operator--(int) {
  Rational copy = *this;
  enumerator_ -= denominator_;
  Reduce();
  return copy;
}

Rational Rational::operator-() const {
  Rational copy = *this;
  copy.enumerator_ = -copy.enumerator_;
  return copy;
}

bool Rational::operator<(const Rational& other) const {
  return enumerator_ * other.denominator_ < denominator_ * other.enumerator_;
}

bool Rational::operator>(const Rational& other) const {
  return !(*this < other) && !(*this == other);
}

bool Rational::operator<=(const Rational& other) const {
  return (*this < other) || (*this == other);
}

bool Rational::operator>=(const Rational& other) const {
  return (*this > other) || (*this == other);
}

bool Rational::operator==(const Rational& other) const {
  //std::cerr << "==\n";
  return !(*this < other) && !(other < *this);
}

bool Rational::operator!=(const Rational& other) const {
  //std::cerr << "!=\n";
  return (*this < other) || (other < *this);
}

std::string Rational::toString() const {
  std::stringstream str_stream;
  str_stream << enumerator_;
  if (denominator_ != 1) {
    str_stream << '/' << denominator_;
  }
  return str_stream.str();
}

std::string Rational::asDecimal(size_t precision) const {
  //std::cerr << "decimal\n";
  uint64_t number_of_additional_digits =
      (precision + static_cast<uint64_t>(log10(BigInteger::base))) /
      static_cast<uint64_t>(log10(BigInteger::base));
  BigInteger result =
      (enumerator_ << number_of_additional_digits) / denominator_;
  std::string decimal = result.toStringAsFixedPoint(precision);
  return decimal;
}

Rational::operator double() const { return 0; }

void Rational::Reduce() {
  BigInteger gcd = bgcd(enumerator_, denominator_);
  enumerator_ /= gcd;
  denominator_ /= gcd;
  if (denominator_.is_negative()) {
    enumerator_ = -enumerator_;
    denominator_ = -denominator_;
  }
}

BigInteger Rational::bgcd(const BigInteger& first, const BigInteger& second) {
  BigInteger first_number = first;
  BigInteger second_number = second;
  while (second_number) {
    first_number %= second_number;
    std::swap(first_number, second_number);
  }
  return first_number;
}

Rational& Rational::operator=(const Rational& other) {
  //std::cerr << "x = " << this->toString() << ' ' << other.toString() << '\n';
  enumerator_ = other.enumerator_;
  denominator_ = other.denominator_;
  //std::cerr << this->toString();
  return *this;
}

Rational operator+(const Rational& first, const Rational& second) {
  //std::cerr << "+\n";
  Rational result = first;
  result += second;
  return result;
}

Rational operator-(const Rational& first, const Rational& second) {
  //std::cerr << "-\n";
  Rational result = first;
  result -= second;
  return result;
}

Rational operator*(const Rational& first, const Rational& second) {
  //std::cerr << "*\n";
  Rational result = first;
  result *= second;
  return result;
}

Rational operator/(const Rational& first, const Rational& second) {
  //std::cerr << "/\n";
  Rational result = first;
  result /= second;
  return result;
}

#endif  // BIGINT_BIGINTEGER_H
