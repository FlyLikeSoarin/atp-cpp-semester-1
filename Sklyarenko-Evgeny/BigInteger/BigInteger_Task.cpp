#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using std::vector;
using std::string;

int base = 1e9;

class BigInteger {
 public:
  [[nodiscard]] bool is_minus() const {
    return is_negative;
  }
  void remove_zeros() {
    while (arr.size() > 1 && arr.back() == 0) {
      arr.pop_back();
    }
    if (arr.size() == 1 && arr.back() == 0) is_negative = false;
  }
  BigInteger() {
    arr.push_back(0);
  }
  BigInteger(int num) {
    arr.clear();
    if (num < 0) {
      is_negative = true;
      num = -num;
    }
    if (num == 0) {
      arr = {0};
    }
    while (num != 0) {
      arr.push_back(num % base);
      num /= base;
    }
  }
  BigInteger operator-() const {
    BigInteger tmp = *this;
    tmp.is_negative = !is_negative;
    tmp.remove_zeros();
    return tmp;
  }
  BigInteger &operator=(const BigInteger &num) {
    if (*this != num) {
      arr = num.arr;
      is_negative = num.is_negative;
    }
    return *this;
  }
  bool abs_compare_lower(const BigInteger &num) const {
    if (arr.size() != num.arr.size()) {return arr.size() < num.arr.size();}
    for (int i = arr.size() - 1; i >= 0; --i) {
      if (arr[i] != num.arr[i]) {return arr[i] < num.arr[i];}
    }
    return true;
  }
  bool operator!=(const BigInteger &num) const {
    return !(num == *this);
  }
  bool operator>(const BigInteger &num) const {
    if (!num.is_negative && is_negative) return false;
    if (num.is_negative && !is_negative) return true;
    if (num.is_negative && is_negative) return abs_compare_lower(num);
    return !abs_compare_lower(num);
  }
  bool operator<(const BigInteger &num) const {
    return !((*this == num) || (*this > num));
  }
  bool operator<=(const BigInteger &num) const {
    return ((*this < num) || (*this == num));
  }
  bool operator>=(const BigInteger &num) const {
    return ((*this > num) || (*this == num));
  }
  [[nodiscard]] string toString() const {
    string s;
    if (is_negative) s += '-';
    for (int i = int(arr.size()) - 1; i > -1; --i) {
      string tmp = std::to_string(arr[i]);
      if (tmp.size() < 9 && i != int(arr.size()) - 1) {
        while (tmp.size() < 9) tmp = '0' + tmp;
      }
      s += tmp;
    }
    return s;
  }
  BigInteger &operator*=(const BigInteger &num) {
    /*vector<int> result;
    result.resize(arr.size() + num.arr.size());
    for (int i = 0; i < int(arr.size()); ++i) {
      int addible = 0;
      for (int j = 0; j < int(num.arr.size()) || addible != 0; ++j) {
        long long current = result[i + j] + arr[i] * 1LL * (j < int(num.arr.size()) ? num.arr[j] : 0) + addible;
        result[i + j] = static_cast<int>(current % base);
        addible = static_cast<int>(current / base);
      }
    }
    arr = result;
    is_negative = (is_negative + num.is_negative) % 2;
    remove_zeros();
    return *this;*/
    is_negative = (is_negative + num.is_negative) % 2;
    vector<long long> numbers(arr.size() + num.arr.size());
    for (int i = 0; i < int(num.arr.size()); ++i) {
      for (int j = 0; j < int(arr.size()); ++j) {
        numbers[i + j] += arr[j] * num.arr[i];
        numbers[i + j + 1] += numbers[i + j] / base;
        numbers[i + j] %= base;
      }
    }
    arr = numbers;
    remove_zeros();
    return *this;
  }

  /*static BigInteger pr(const BigInteger &num1, const BigInteger &num2, int l1, int r1, int l2, int r2) {
    int n = std::max(r1 - l1, r2 - l2);
    if (n == 0) {
      BigInteger tmp;
      long long tmp1 = num1.arr[l1] * num2.arr[l2];
      tmp.arr.resize(l1 + 1, 0);
      tmp.arr[l1] = tmp1 % base;
      tmp.arr[l1 + 1] = (tmp1 / base) % base;
    } else {
      return pr(num1, num2, l1, r1 / 2, l2, r2 / 2) + (pr(num1, num2, l1, r1 / 2, l2, r2 / 2))
    }
  }
  BigInteger &operator*=(const BigInteger &num) {
    string num1 = toString(), num2 = num.toString();
    int sz = std::max(num1.size(), num2.size());
    if (sz == 1) {
      *this = arr[0] * num.arr[0];
    }
  }*/
  /*void abs_sum(const BigInteger& num) {
    arr.resize(std::max(arr.size(), num.arr.size()) + 1);
    for (size_t i = 0; i < num.arr.size(); ++i) {
      arr[i] += num.arr[i];
      if (arr[i] > 0) {
        ++arr[i + 1];
        arr[i] -= base;
      }
    }
    is_negative = false;
  }
  void abs_dif(const BigInteger& num) {
    arr.resize(std::max(arr.size(), num.arr.size()));
    for (size_t i = 0; i < num.arr.size(); ++i) {
      arr[i] -= num.arr[i];
      if (arr[i] < 0 && i != num.arr.size() - 1) {
        arr[i] += base;
        --arr[i + 1];
      }
    }
    is_negative = false;
    if (arr[arr.size() - 1] < 0) {
      is_negative = true;
      arr[arr.size() - 1] *= -1;
      for (int i = int(arr.size() - 2); i >= 0; --i) {
        arr[i] = base - arr[i];
        --arr[i + 1];
      }
    }
  }*/
  BigInteger &operator+=(const BigInteger &num) {
    if (!((is_negative + num.is_negative)) % 2) {
      long long addible = 0;
      int size = std::max(int(arr.size()), int(num.arr.size())) + 1;
      for (int i = 0; i < size; ++i) {
        if (i >= int(arr.size())) {
          arr.push_back(0);
        }
        long long num1 = arr[i];
        long long num2 = (i < int(num.arr.size()) ? num.arr[i] : 0);
        arr[i] = (num1 + num2 + addible) % base;
        addible = (num1 + num2 + addible) / base;
        if ((i >= int(num.arr.size()) - 1) && (addible == 0)) break;
      }
    } else if (abs_compare_lower(num)) {
      is_negative = !is_negative;
      long long addible = 0;
      for (int i = 0; i < int(num.arr.size()); ++i) {
        if (i >= int(arr.size())) arr.push_back(0);
        if (num.arr[i] >= arr[i] + addible) {
          arr[i] = num.arr[i] - arr[i] - addible;
          addible = 0;
        }
        else {
          arr[i] = base + num.arr[i] - arr[i] - addible;
          addible = 1;
        }
      }
    }
    else {
      long long addible = 0;
      for (int i = 0; i < int(arr.size()); ++i) {
        if ((i >= int(num.arr.size())) && (addible == 0)) break;
        long long num1 = (i < int(num.arr.size()) ? num.arr[i] : 0);
        if (arr[i] >= num1 + addible) {
          arr[i] = arr[i] - num1 - addible;
          addible = 0;
        }
        else {
          arr[i] = base + arr[i] - num1 - addible;
          addible = 1;
        }
      }
    }
    remove_zeros();
    return *this;
    /*bool flag = is_negative;
    if (is_negative == num.is_negative) {
      abs_sum(num);
      is_negative = flag;
    }
    else {
      abs_dif(num);
      if (!flag) {
        is_negative = !is_negative;
      }
    }
    remove_zeros();
    return *this;*/
  }
  BigInteger& operator-=(const BigInteger& num) {
    if (((is_negative + num.is_negative)) % 2) {
      long long addible = 0;
      int size = std::max(int(arr.size()), int(num.arr.size())) + 1;
      for (int i = 0; i < size; ++i) {
        if (i >= int(arr.size())) {
          arr.push_back(0);
        }
        long long num1 = arr[i];
        long long num2 = (i < int(num.arr.size()) ? num.arr[i] : 0);
        arr[i] = (num1 + num2 + addible) % base;
        addible = (num1 + num2 + addible) / base;
        if ((i >= int(num.arr.size()) - 1) && (addible == 0)) break;
      }
    } else if (abs_compare_lower(num)) {
      is_negative = !is_negative;
      long long addible = 0;
      for (int i = 0; i < int(num.arr.size()); ++i) {
        if (i >= int(arr.size())) arr.push_back(0);
        if (num.arr[i] >= arr[i] + addible) {
          arr[i] = num.arr[i] - arr[i] - addible;
          addible = 0;
        }
        else {
          arr[i] = base + num.arr[i] - arr[i] - addible;
          addible = 1;
        }
      }
    }
    else {
      long long addible = 0;
      for (int i = 0; i < int(arr.size()); ++i) {
        if ((i >= int(num.arr.size())) && (addible == 0)) break;
        long long num1 = (i < int(num.arr.size()) ? num.arr[i] : 0);
        if (arr[i] >= num1 + addible) {
          arr[i] = arr[i] - num1 - addible;
          addible = 0;
        }
        else {
          arr[i] = base + arr[i] - num1 - addible;
          addible = 1;
        }
      }
    }
    remove_zeros();
    return *this;
  }
  vector<long long> del(const BigInteger& num) {
    bool flag = (is_negative + num.is_negative) % 2;
    vector<long long> tmp(num.arr.size());
    vector<long long> result(arr.size());
    vector<long long> counter;
    long long ub = 0;
    long long bb = 0;
    long long cb = 0;
    for (size_t i = 0; i < arr.size(); ++i) {
      tmp.insert(tmp.begin(), arr[arr.size() - 1 - i]);
      tmp[tmp.size() - 2] += tmp[tmp.size() - 1] * base;
      tmp.pop_back();
      ub = tmp[tmp.size() - 1] / num.arr[num.arr.size() - 1];
      bb = tmp[tmp.size() - 1] / (num.arr[num.arr.size() - 1] + 1);
      cb = (ub + bb) / 2;
      while (ub != bb) {
        counter = tmp;
        for (size_t j = 0; j < num.arr.size(); ++j) {
          counter[j] -= cb * num.arr[j];
          if (counter[j] < 0) {
            if (j != num.arr.size() - 1) {
              counter[j + 1] -= std::abs(counter[j]) / base;
              if (std::abs(counter[j]) % base) --counter[j + 1];
              counter[j] %= base;
              counter[j] += base;
            }
          }
        }
        if (counter[num.arr.size() - 1] < 0) {
          if (ub - bb == 1) ub = bb;
          else {
            ub = cb;
            cb = (bb + ub) / 2;
          }
        }
        else {
          if (ub - bb == 1) {
            if (cb == ub) bb = ub;
            else cb = ub;
          }
          else {
            bb = cb;
            cb = (bb + ub) / 2;
          }
        }
      }
      result[i] = bb;
      for (size_t j = 0; j < num.arr.size(); ++j) {
        tmp[j] -= bb * num.arr[j];
        if (tmp[j] < 0) {
          tmp[j + 1] -= std::abs(tmp[j]) / base;
          if (std::abs(tmp[j]) % base != 0) --tmp[j + 1];
          tmp[j] %= base;
          tmp[j] += base;
        }
      }
    }
    std::reverse(result.begin(), result.end());
    arr = result;
    is_negative = flag;
    return tmp;
  }
  BigInteger &operator/=(const BigInteger &num) {
    del(num);
    remove_zeros();
    return *this;
  }
  BigInteger &operator%=(const BigInteger &num) {
      arr = del(num);
      remove_zeros();
      return *this;
  }
  BigInteger &operator++() {
    return (*this += 1);
  }
  BigInteger &operator--() {
    return (*this -= 1);
  }
  BigInteger operator++(int) {
    BigInteger tmp = *this;
    *this += 1;
    return tmp;
  }
  BigInteger operator--(int) {
    BigInteger tmp = *this;
    *this -= 1;
    return tmp;
  }
  explicit operator bool() const {
    if (*this == 0) return false;
    else return true;
  }
  explicit operator int() const {
    string s;
    for (int i : arr) s += std::to_string(i);
    return std::stoi(s);
  }
  ~BigInteger() = default;
  friend std::istream &operator>>(std::istream &in, BigInteger &num);
  friend std::ostream &operator<<(std::ostream &out, const BigInteger &num);
  friend bool operator==(const BigInteger &num1, const BigInteger &num2);
  friend BigInteger operator+(const BigInteger &num1, const BigInteger &num2);
  friend BigInteger operator-(const BigInteger &num1, const BigInteger &num2);
  friend BigInteger operator*(const BigInteger &num1, const BigInteger &num2);
  friend BigInteger operator/(const BigInteger &num1, const BigInteger &num2);
  friend BigInteger operator%(const BigInteger &num1, const BigInteger &num2);
 private:
  vector<long long> arr;
  bool is_negative = false;
};

bool operator==(const BigInteger &num1, const BigInteger &num2) {
  if (num1.arr.size() != num2.arr.size() || num1.is_negative != num2.is_negative) return false;
  else {
    for (int i = 0; i < int(num2.arr.size()); ++i) {
      if (num2.arr[i] != num1.arr[i]) return false;
    }
  }
  return true;
}

std::istream &operator>>(std::istream &in, BigInteger &num) {
  in.tie(nullptr);
  bool flag = false;
  num.arr.clear();
  char t;
  in.get(t);
  while (std::isspace(t)) in.get(t);
  if (t == '-') {
    flag = true;
    in.get(t);
  }
  while (!std::isspace(t) && !in.eof()) {
    num *= 10;
    num += (t - '0');
    in.get(t);
  }
  num.is_negative = flag;
  return in;
}
std::ostream &operator<<(std::ostream &out, const BigInteger &num) {
  out.tie(nullptr);
  out << num.toString();
  return out;
}

BigInteger operator+(const BigInteger &num1, const BigInteger &num2) {
  BigInteger tmp = num1;
  tmp += num2;
  return tmp;
}
BigInteger operator-(const BigInteger &num1, const BigInteger &num2) {
  BigInteger tmp = num1;
  tmp -= num2;
  return tmp;
}
BigInteger operator*(const BigInteger &num1, const BigInteger &num2) {
  BigInteger tmp = num1;
  tmp *= num2;
  return tmp;
}
BigInteger operator/(const BigInteger &num1, const BigInteger &num2) {
  BigInteger tmp = num1;
  tmp /= num2;
  return tmp;
}
BigInteger operator%(const BigInteger &num1, const BigInteger &num2) {
  BigInteger tmp = num1;
  tmp %= num2;
  return tmp;
}

BigInteger gcd(BigInteger num1, BigInteger num2) {
  if (num1.is_minus()) num1 = -num1;
  if (num2.is_minus()) num2 = -num2;
  while (num1 != 0 && num2 != 0) {
    if (num1 > num2) num1 %= num2;
    else num2 %= num1;
  }
  return num1 + num2;
}

class Rational {
 private:
  BigInteger numenator;
  BigInteger denominator;
 public:
  void reduce() {
    if (numenator == 0) {
      denominator = 1;
    }
    else {
      BigInteger del = gcd(numenator, denominator);
      if (del >= 1) {
        numenator /= del;
        denominator /= del;
      } else {
        denominator = (denominator.is_minus() ? -1 : 1);
      }
      if (denominator < 0) {
        numenator = -numenator;
        denominator = -denominator;
      }
    }
  }
  Rational() : numenator(0), denominator(1) {
  }
  Rational(int num) {
    numenator = num;
    denominator = 1;
  }
  Rational(const BigInteger &num1) {
    numenator = num1;
    denominator = 1;
  }
  bool operator!=(const Rational &num) const {
    return !(*this == num);
  }
  bool operator>(const Rational &num) const {
    if (numenator.is_minus() && !num.numenator.is_minus()) return false;
    else if (!numenator.is_minus() && num.numenator.is_minus()) return true;
    else if (numenator.is_minus() && num.numenator.is_minus()) {
      return (numenator * num.denominator < num.numenator * denominator);
    } else {
      return (numenator * num.denominator > num.numenator * denominator);
    }
  }
  bool operator<(const Rational &num) const {
    return !((*this == num) || (*this > num));
  }
  bool operator<=(const Rational &num) const {
    return ((*this < num) || (*this == num));
  }
  bool operator>=(const Rational &num) const {
    return ((*this > num) || (*this == num));
  }
  Rational operator-() const {
    Rational tmp = 0;
    tmp -= *this;
    return tmp;
  }
  [[nodiscard]] string toString() const {
    string s;
    s += numenator.toString();
    if (denominator > 1) {
      s += '/';
      s += denominator.toString();
    }
    return s;
  }
  Rational &operator=(const Rational &num) = default;
  Rational &operator+=(const Rational &num) {
    numenator = numenator * num.denominator + num.numenator * denominator;
    denominator *= num.denominator;
    reduce();
    return *this;
  }
  Rational &operator-=(const Rational &num) {
    numenator = numenator * num.denominator - num.numenator * denominator;
    denominator *= num.denominator;
    reduce();
    return *this;
  }
  Rational &operator/=(const Rational &num) {
    numenator *= num.denominator;
    denominator *= num.numenator;
    reduce();
    return *this;
  }
  Rational &operator*=(const Rational &num) {
    numenator *= num.numenator;
    denominator *= num.denominator;
    reduce();
    return *this;
  }
  [[nodiscard]] string asDecimal(size_t precision = 0) const {
    BigInteger num = (numenator < 0 ? -numenator : numenator);
    for (size_t i = 0; i < precision; ++i) {
      num *= 10;
    }
    string s = (num / denominator).toString();
    if (s.size() < precision + 1) {
      s = string(precision + 1 - s.size(), '0') + s;
    }
    string s1 = s.substr(0, s.size() - precision);
    if (precision != 0) {
      s1 += '.';
      s1 += s.substr(s.size() - precision, precision);
    }
    if (numenator.is_minus()) s1 = "-" + s1;
    return s1;
  }
  explicit operator double() const {
    double tmp = std::stod(asDecimal(16));
    return tmp;
  }
  explicit operator int() const {
    return int(numenator);
  }
  ~Rational() = default;
  friend bool operator==(const Rational &num, const Rational &num1);
  friend Rational operator+(const Rational &num1, const Rational &num2);
  friend Rational operator-(const Rational &num1, const Rational &num2);
  friend Rational operator*(const Rational &num1, const Rational &num2);
  friend Rational operator/(const Rational &num1, const Rational &num2);
};

bool operator==(const Rational &num, const Rational &num1) {
  if (num.numenator == num1.numenator && num.denominator == num1.denominator) return true;
  return false;
}
std::ostream& operator<<(std::ostream& out, const Rational& num) {
  out.tie(nullptr);
  out << num.toString();
  return out;
}
std::istream &operator>>(std::istream &in, Rational &num) {
  in.tie(nullptr);
  bool flag = false;
  char t;
  in.get(t);
  while (std::isspace(t)) in.get(t);
  if (t == '-') {
    flag = true;
    in.get(t);
  }
  while (!std::isspace(t) && !in.eof()) {
    num *= 10;
    num += (t - '0');
    in.get(t);
  }
  if (flag) num *= -1;
  return in;
}
Rational operator+(const Rational &num1, const Rational &num2) {
  Rational tmp = num1;
  tmp += num2;
  tmp.reduce();
  return tmp;
}
Rational operator-(const Rational &num1, const Rational &num2) {
  Rational tmp = num1;
  tmp -= num2;
  tmp.reduce();
  return tmp;
}
Rational operator*(const Rational &num1, const Rational &num2) {
  Rational tmp = num1;
  tmp *= num2;
  tmp.reduce();
  return tmp;
}
Rational operator/(const Rational &num1, const Rational &num2) {
  Rational tmp = num1;
  tmp /= num2;
  tmp.reduce();
  return tmp;
}
