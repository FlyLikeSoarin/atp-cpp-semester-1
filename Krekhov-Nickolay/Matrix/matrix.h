#include <iostream>
#include <vector>
#include <string>
using std::vector;
using std::cin;
using std::cout;

class BigInteger;

BigInteger operator*(const BigInteger& arg1, const BigInteger& arg2);
bool operator<=(const BigInteger& arg1, const BigInteger& arg2);
bool operator>=(const BigInteger& arg1, const BigInteger& arg2);

class BigInteger {


 public:

//Constructors
  BigInteger() {
    sign_ = false;
    digits_.push_back(0);
  }


  BigInteger(const BigInteger& num) : sign_(num.sign_), digits_(num.digits_) {}

  BigInteger(const long long num) {
    (num >= 0) ? sign_ = false : sign_ = true;
    if (num >= BASE) {
      digits_.push_back(num * ((!sign_) ? 1 : -1) % BASE);
      digits_.push_back(num * ((!sign_) ? 1 : -1) / BASE);
    } else {
      digits_.push_back(num * ((!sign_) ? 1 : -1));
    }
  }
  BigInteger(const std::string& S) {
    fromString(S);
  }

  long long size() const {
    return digits_.size();
  }
  bool isPositive() const {
    return !sign_;
  }
  void makePositive() const {
    sign_ = false;
  }
  void makeNegative() const {
    sign_ = true;
  }

  bool isEven() const {
    return !(digits_[0] % 2);
  }
  std::string toString() const {
    std::string result = "";
    if ((digits_.empty()) || ((digits_[0] == 0) & (digits_.size() == 1))) {
      result = "0";
    } else {
      if (sign_) {
        result += "-";
      }
      result += std::to_string(digits_[digits_.size() - 1]);
      for (long long i = digits_.size() - 2; i >= 0; i--) {
        result += add_zeros(std::to_string(digits_[i]));
      }

    }
    return result;
  }

// operators
  BigInteger& operator=(const BigInteger& num) {
    BigInteger copy(num);
    swap(copy);
    return *this;
  }

  const BigInteger operator-() const {
    BigInteger copy = *this;
    copy.sign_ = !sign_;
    return copy;
  }

  BigInteger& operator+=(const BigInteger& arg) {
    if ((!sign_ && !arg.sign_) || (sign_ && arg.sign_)) return sum(arg);
    else if (!sign_ && arg.sign_) {
      if (compare_less_abs(arg)) {

        differnceFromSmaller(arg);
        sign_ = true;
        return *this;
      } else {
        return differenceFromLarger(arg);
      }
    } else {
      if (compare_less_abs(arg)) {
        sign_ = false;
        differnceFromSmaller(arg);
        return *this;
      } else {

        return differenceFromLarger(arg);
      }
    }
  }

  BigInteger& operator-=(const BigInteger& arg) {
    if (arg == (*this)) {
      *this = 0;
      return *this;
    }
    arg.sign_ = !arg.sign_;
    *this += arg;
    arg.sign_ = !arg.sign_;
    return *this;
  }

  BigInteger& operator*=(const BigInteger& arg) {
    BigInteger result;
    result.digits_.resize(size() + arg.size() + 1, 0);
    for (long long i = 0; i < size(); ++i) {
      uint64_t temporary = 0;

      for (long long j = 0; j < arg.size() || temporary != 0; ++j) {
        uint64_t cur = result.digits_[i + j] + digits_[i] * (j < arg.size() ? arg.digits_[j] : 0) + temporary;
        result.digits_[i + j] = (cur % BASE);
        temporary = (cur / BASE);
      }
    }
    result.remove_leading_zeros();
    bool sign;
    if ((arg.sign_ && sign_) || (!arg.sign_ && !sign_)) sign = false;
    else sign = true;
    *this = result;
    sign_ = sign;
    return *this;
  }

  BigInteger& operator/=(const BigInteger& arg) {
    return divAndMod(arg, false);
  }


  BigInteger& operator%=(const BigInteger& arg) {
    return divAndMod(arg,true);
  }

  explicit operator bool() const {
    return !(*this == 0);
  }

  BigInteger& operator++() {
    return *this += 1;
  }
  BigInteger operator++(int) {
    BigInteger value = *this;
    *this += 1;
    return value;
  }

  BigInteger& operator--() {
    return *this -= 1;
  }
  BigInteger operator--(int) {
    BigInteger value = *this;
    *this -= 1;
    return value;
  }





 private:
  friend bool operator<(const BigInteger& arg1, const BigInteger& arg2);
  friend bool operator==(const BigInteger& arg1, const BigInteger& arg2);
  friend std::istream& operator>>(std::istream& in, BigInteger& num);

  mutable bool sign_ = false;
  vector<long long> digits_;

  static const long long BASE = 1000000000;
  static const int long long BASE_LENGTH = 9;

//secondary functions

  void shift_right() {
    if (size() == 0) {
      digits_.push_back(0);
      return;
    }
    digits_.push_back(digits_[size() - 1]);

    for (size_t i = size() - 2; i > 0; --i) digits_[i] = digits_[i - 1];
    digits_[0] = 0;
  }

  static std::string add_zeros(std::string str) {
    while (str.size() < BASE_LENGTH) {
      str = "0" + str;
    }
    return str;
  }

  bool compare_less_abs(const BigInteger& arg2) const {
    if (digits_.size() != arg2.digits_.size()) {
      return digits_.size() < arg2.digits_.size();
    } else {
      for (long long i = digits_.size() - 1; i >= 0; --i) {
        if (digits_[i] != arg2.digits_[i]) return digits_[i] < arg2.digits_[i];
      }
      return false;
    }
  }

  void swap(BigInteger& num) {
    std::swap(sign_, num.sign_);
    std::swap(digits_, num.digits_);
  }

  void clear() {
    digits_.clear();
    sign_ = false;
  }

  bool isZero() const {
    if (digits_.empty() || (digits_.size() == 1 && digits_[0] == 0)) return true;
    else return false;
  }

  void remove_leading_zeros() {
    while ((!digits_.empty()) && (digits_.back() == 0)) {
      digits_.pop_back();
    }
    if ((digits_.size() == 1 && digits_[0] == 0) ||(digits_.empty())) {
      sign_ = false;
    }
  }

  void fromString(std::string str) {
    clear();
    if (str[0] == '-') {
      sign_ = true;
      str.erase(0, 1);
    }
    for (long long i = str.size(); i > 0; i -= BASE_LENGTH) {
      if (i < BASE_LENGTH) {
        digits_.push_back(std::stoi(str.substr(0, i)));
      } else {
        digits_.push_back(std::stoi(str.substr(i - 9, 9)));
      }
    }
    remove_leading_zeros();

  }
  BigInteger& sum(const BigInteger& num) {
    long long temporary = 0;
    for (long long numberOfDigit = 0; (temporary != 0) || (numberOfDigit < std::max(size(), num.size()));
         ++numberOfDigit) {
      if (size() == numberOfDigit) {
        digits_.push_back(0);
      }
      digits_[numberOfDigit] += temporary + (numberOfDigit < num.size() ? num.digits_[numberOfDigit] : 0);
      temporary = digits_[numberOfDigit] >= BASE;
      digits_[numberOfDigit] -= temporary * BASE;
    }
    remove_leading_zeros();
    return *this;
  }
  BigInteger& differenceFromLarger(const BigInteger& num) { // 1000-555 *this >= num
    long long temporary = 0;
    for (long long numberOfDigit = 0; (temporary != 0) || (numberOfDigit < std::max(size(), num.size()));
         ++numberOfDigit) {
      digits_[numberOfDigit] -= temporary + (numberOfDigit < num.size() ? num.digits_[numberOfDigit] : 0);
      temporary = digits_[numberOfDigit] < 0;
      digits_[numberOfDigit] += temporary * BASE;
    }
    remove_leading_zeros();
    return *this;
  }
  BigInteger& differnceFromSmaller(const BigInteger& num) {  //555-10000 *this<num;
    long long temporary = 0;
    for (long long numberOfDigit = 0; (temporary != 0) || (numberOfDigit < std::max(size(), num.size()));
         ++numberOfDigit) {
      if (size() == numberOfDigit) {
        digits_.push_back(0);
      }
      digits_[numberOfDigit] =
          num.digits_[numberOfDigit] - temporary - (numberOfDigit < size() ? digits_[numberOfDigit] : 0);
      temporary = digits_[numberOfDigit] < 0;
      digits_[numberOfDigit] += temporary * BASE;
    }
    remove_leading_zeros();
    return *this;

  }

  BigInteger& divAndMod(const BigInteger& arg,bool divOrMod) { //div=false,mod=true;
    if (arg.size() == 1) {
      long long divider = arg.digits_[0];
      long long temporary = 0;
      long long composition;
      bool sign1 = (sign_ != arg.sign_);
      for (long long i = size() - 1; i >= 0; --i) {
        composition = digits_[i] + temporary * BASE;
        digits_[i] = composition / divider;
        temporary = composition % divider;
      }
      if (divOrMod) {
        remove_leading_zeros();
        *this = temporary;
        sign_ = sign1;
        return *this;
      } else {
        sign_ = (sign_ != arg.sign_);
        remove_leading_zeros();
        return *this;
      }
    } else {
      if (compare_less_abs(arg)) {
        if (divOrMod){
          sign_ = arg.sign_ != sign_;
          return *this;
        } else {
          *this = 0;
          return *this;
        }
      }

      BigInteger tmp;
      BigInteger result;
      BigInteger firstPart;
      result.digits_.resize(size(), 0);
      bool sign2 = arg.sign_;
      bool sign = arg.sign_;
      arg.sign_ = false;

      long long left;
      long long middle;
      long long right;
      long long quotient;

      for (long long i = size() - 1; i >= 0; --i) {
        firstPart.shift_right();
        firstPart.digits_[0] = digits_[i];
        firstPart.remove_leading_zeros();
        left = 0;
        quotient = 0;
        right = BASE * 10;
        if (30 * arg >= firstPart) {
          right = 30;
          while (left <= right) {
            middle = (left + right) / 2;
            tmp = arg * middle;

            if (tmp <= firstPart) {
              quotient = middle;
              left = middle + 1;
            } else {
              right = middle - 1;
            }
          }
          firstPart -= (quotient * arg);
          result.digits_[i] = quotient;
          if (firstPart == 0) {
            firstPart.digits_.clear();
          }

        } else {
          while (left <= right) {
            middle = (left + right) / 2;
            tmp = arg * middle;

            if (tmp <= firstPart) {
              quotient = middle;
              left = middle + 1;
            } else {
              right = middle - 1;
            }
          }
          firstPart -= (quotient * arg);
          result.digits_[i] = quotient;
        }
      }
      if (divOrMod){
        arg.sign_ = sign2;
        firstPart.sign_ = (sign_ != arg.sign_);
        *this = firstPart;
      } else {
        arg.sign_ = sign;
        result.sign_ = (sign_ != arg.sign_);
        *this = result;
      }
      remove_leading_zeros();
      return *this;

    }
  }





};

bool operator<(const BigInteger& arg1, const BigInteger& arg2) {
  if (arg1 == arg2) return false;
  else if (arg1.sign_ && !arg2.sign_) return true;
  else if (!arg1.sign_ && arg2.sign_) return false;
  else if (arg1.sign_ && arg2.sign_) return arg2.compare_less_abs(arg1);
  else return (arg1.compare_less_abs(arg2));
}

bool operator==(const BigInteger& arg1, const BigInteger& arg2) {
  if (arg1.sign_ != arg2.sign_) return false;
  if (arg1.isZero() && arg2.isZero()) return true;
  if (arg1.digits_.size() != arg2.digits_.size()) return false;
  for (size_t i = 0; i < arg1.digits_.size(); ++i) {
    if (arg1.digits_[i] != arg2.digits_[i]) return false;
  }
  return true;
}

bool operator!=(const BigInteger& arg1, const BigInteger& arg2) {
  return !(arg1 == arg2);
}

bool operator<=(const BigInteger& arg1, const BigInteger& arg2) {
  return (arg1 < arg2) || (arg1 == arg2);
}
bool operator>=(const BigInteger& arg1, const BigInteger& arg2) {
  return !(arg1 < arg2);
}
bool operator>(const BigInteger& arg1, const BigInteger& arg2) {
  return !(arg1 <= arg2);
}

BigInteger operator+(const BigInteger& arg1, const BigInteger& arg2) {
  BigInteger copy = arg1;
  copy += arg2;
  return copy;
}
BigInteger operator-(const BigInteger& arg1, const BigInteger& arg2) {
  BigInteger copy = arg1;
  copy -= arg2;
  return copy;
}

BigInteger operator*(const BigInteger& arg1, const BigInteger& arg2) {
  BigInteger copy = arg1;
  copy *= arg2;
  return copy;
}

BigInteger operator/(const BigInteger& arg1, const BigInteger& arg2) {
  BigInteger copy = arg1;
  copy /= arg2;
  return copy;
}

BigInteger operator%(const BigInteger& arg1, const BigInteger& arg2) {
  BigInteger copy = arg1;
  copy %= arg2;
  return copy;
}

std::istream& operator>>(std::istream& in, BigInteger& num) {

  std::string S;
  in >> S;
  num.fromString(S);
  return in;
}

std::ostream& operator<<(std::ostream& out, const BigInteger& num) {
  out << num.toString();
  return out;
}

class Rational {

 public:
  Rational(
      const BigInteger& num) : numerator_(num), denominator_(1)
  {}
  Rational(long long
           num) : numerator_(num), denominator_(1)
  {}
  Rational(
      const Rational& num) = default;
  Rational() =
  default;

  std::string toString() {
    std::string result;
    normalize();
    result = numerator_.toString() + ((denominator_ != 1) ? '/' + denominator_.toString() : "");
    return result;
  }

  explicit operator double() const {
    return std::atof(asDecimal(16).c_str());
  }
  const Rational operator-() const {
    Rational copy = *this;
    if (numerator_.isPositive()) copy.numerator_.makeNegative();
    else copy.numerator_.makePositive();
    return copy;
  }

  Rational& operator=(const Rational& num) {
    Rational copy(num);
    swap(copy);
    return *this;
  }

  Rational& operator+=(const Rational& arg) {
    normalize();
    if (arg.denominator_.isPositive() == arg.numerator_.isPositive()) {
      arg.denominator_.makePositive();
      arg.numerator_.makePositive();

    } else {
      arg.denominator_.makePositive();
      arg.numerator_.makeNegative();

    }
    numerator_ = numerator_ * arg.denominator_ + arg.numerator_ * denominator_;
    denominator_ *= arg.denominator_;
    normalize();
    return *this;
  }
  Rational& operator-=(const Rational& arg) {
    normalize();
    if (arg.denominator_.isPositive() == arg.numerator_.isPositive()) {
      arg.denominator_.makePositive();
      arg.numerator_.makePositive();
    } else {
      arg.denominator_.makePositive();
      arg.numerator_.makeNegative();
    }
    numerator_ = numerator_ * arg.denominator_ - arg.numerator_ * denominator_;
    denominator_ *= arg.denominator_;
    normalize();
    return *this;
  }
  Rational& operator*=(const Rational& arg) {
    if (*this == 0) {
      *this = 0;
      return *this;
    }
    normalize();
    numerator_ = numerator_ * arg.numerator_;
    denominator_ = denominator_ * arg.denominator_;
    normalize();

    return *this;
  }
  Rational& operator/=(const Rational& arg) {
    if (*this == 0) {
      *this = 0;
      return *this;
    } else if (this == &arg) {
      *this = 1;
      return *this;
    }
    normalize();
    numerator_ = numerator_ * arg.denominator_;
    denominator_ = denominator_ * arg.numerator_;
    normalize();
    return *this;
  }

  std::string asDecimal(size_t precision) const {
    bool sign = (denominator_.isPositive() != numerator_.isPositive());
    numerator_.makePositive();
    denominator_.makePositive();
    std::string result = "";
    if (sign) {
      result += '-';
    }
    result += (numerator_ / denominator_).toString();
    if (precision != 0) {
      result += '.';
      BigInteger rem = numerator_ % denominator_;
      rem.makePositive();
      for (size_t i = 0; i < precision; ++i) {
        rem *= 10;
      }
      denominator_.makePositive();
      if (sign) {
        numerator_.makeNegative();
      }
      std::string partAfterPoint = "";
      partAfterPoint += (rem / denominator_).toString();
      std::string zeroes = "";
      while (partAfterPoint.size() + zeroes.size() < precision) {
        zeroes += '0';
      }
      result += zeroes + partAfterPoint;
    }
    return result;
  }



 private:

  friend bool operator<(const Rational& arg1, const Rational& arg2);
  friend bool operator==(const Rational& arg1, const Rational& arg2);

  BigInteger numerator_ = 1;
  BigInteger denominator_ = 1;
//fast gcd for bigInt;
  static BigInteger gcd(BigInteger a, BigInteger b) {
    BigInteger ans = 1;
    while (a != 0 && b != 0) {
      if (!a.isEven() && !b.isEven()) {
        a > b ? a -= b : b -= a;
      }
      if (a.isEven() && b.isEven()) {
        a /= 2;
        b /= 2;
        ans *= 2;
      }
      if (a.isEven() && !b.isEven()){
        a/=2;
      }
      if (!a.isEven() && b.isEven()){
        b/=2;
      }
    }

    ans *= (a==0) ? b : a;
    return ans;

  }

  void swap(Rational& num) {
    std::swap(numerator_, num.numerator_);
    std::swap(denominator_, num.denominator_);
  }

  void normalize() {

    if (numerator_.isPositive() == denominator_.isPositive()) {
      numerator_.makePositive();
      denominator_.makePositive();
    } else {
      numerator_.makeNegative();
      denominator_.makePositive();
    }
    bool sign = !numerator_.isPositive();
    numerator_.makePositive();
    BigInteger del = gcd(numerator_, denominator_);
    numerator_ /= del;
    denominator_ /= del;
    if (sign) {
      numerator_.makeNegative();
    }
  }


};


bool operator<(const Rational& arg1, const Rational& arg2) {
  if (arg1 == arg2) return false;
  else if ((arg1.numerator_.isPositive() == arg1.denominator_.isPositive())
      && (arg2.numerator_.isPositive() != arg2.denominator_.isPositive()))
    return false;
  else if ((arg1.numerator_.isPositive() != arg1.denominator_.isPositive())
      && (arg2.numerator_.isPositive() == arg2.denominator_.isPositive()))
    return true;
  else {
    return arg2.denominator_ * arg1.numerator_ < arg2.numerator_ * arg1.denominator_;
  }
}

bool operator==(const Rational& arg1, const Rational& arg2) {
  return arg2.denominator_ * arg1.numerator_ == arg2.numerator_ * arg1.denominator_;
}

bool operator!=(const Rational& arg1, const Rational& arg2) {
  return !(arg1 == arg2);
}

bool operator<=(const Rational& arg1, const Rational& arg2) {
  return (arg1 < arg2) || (arg1 == arg2);
}
bool operator>=(const Rational& arg1, const Rational& arg2) {
  return !(arg1 < arg2);
}
bool operator>(const Rational& arg1, const Rational& arg2) {
  return !(arg1 <= arg2);
}

Rational operator+(const Rational& arg1, const Rational& arg2) {
  Rational copy = arg1;
  copy += arg2;
  return copy;
}
Rational operator-(const Rational& arg1, const Rational& arg2) {
  Rational copy = arg1;
  copy -= arg2;
  return copy;
}

Rational operator*(const Rational& arg1, const Rational& arg2) {
  Rational copy = arg1;
  copy *= arg2;
  return copy;
}

Rational operator/(const Rational& arg1, const Rational& arg2) {
  Rational copy = arg1;
  copy /= arg2;
  return copy;
}

std::istream& operator>>(std::istream& in, Rational& num) {

  BigInteger arg;
  in >> arg;
  num = Rational(arg);
  return in;
}





//TEMPLATE INSTANCING------------------------------------------

template<size_t N, size_t divider, bool endOfRecursion = false>
struct IsSimpleHelper {
  static const bool
      ans = (N == 2) || ((N % divider != 0) && (IsSimpleHelper<N, divider + 1, (divider * divider > N)>::ans));
};

template<size_t N, size_t divider>
struct IsSimpleHelper<N, divider, true> {
  static const bool ans = true;
};

template<size_t N>
struct IsSimple {
  static const bool ans = IsSimpleHelper<N, 2, false>::ans;
};

//-----------------------------------------------------------------------------
//-------------------------RESIDUE----------------------------------------------
template<size_t N>
class Residue {
 public:
  template<size_t M>
  friend bool operator==(const Residue<M>& arg1, const Residue<M>& arg2);
  template<size_t M>
  friend bool operator<(const Residue<M>& arg1, const Residue<M>& arg2);


  //_________CONSTRUCTORS______________________________________
  explicit Residue(const long long& number) : num(remainder(number)) {}
  Residue(const Residue<N>& other) : num(other.num) {}
  explicit Residue() : num(0) {}
  ~Residue()=default;

  //------------OPERATORS------------------------------
  Residue<N>& operator+=(const Residue<N>& other) {
    num += other.num;
    num %= N;
    return *this;
  }

  Residue<N>& operator-=(const Residue<N>& other) {
    num = remainder(num -= other.num);
    return *this;
  }
  Residue<N>& operator*=(const Residue<N>& other) {
    num *= other.num;
    num = remainder(num);
    return *this;
  }

  Residue<N>& operator/=(const Residue<N>& other) {
    static_assert(IsSimple<N>::ans);
    return *this *= inverseElement(other);
  }

  Residue<N>& operator=(const Residue<N>& other) {
    if (this == &other) {
      return *this;
    }
    Residue<N> copy = other;
    swap(copy);
    return *this;
  }
  explicit operator int() const {
    return num;
  }
 private:
  static size_t remainder(const long long& num) {
    return (num >= 0) ? num % N : N - ((-num) % N);
  }
  void swap(Residue<N>& other) {
    std::swap(num, other.num);
  }
  static Residue<N> inverseElement(Residue<N> elem) {
    Residue<N> result(1);
    size_t degree = N - 2;
    while (degree > 0) {
      if (degree % 2 == 0) {
        elem *= elem;
        degree /= 2;
      } else {
        result *= elem;
        --degree;
      }
    }
    return result;
  }

  size_t num;
};
template<size_t N>
bool operator==(const Residue<N>& arg1, const Residue<N>& arg2) {
  return (arg1.num == arg2.num);
}
template<size_t N>
bool operator!=(const Residue<N>& arg1, const Residue<N>& arg2) {
  return !(arg1 == arg2);
}

template<size_t N>
bool operator<(const Residue<N>& arg1, const Residue<N>& arg2) {
  return (arg1.num < arg2.num);
}
template<size_t N>
bool operator<=(const Residue<N>& arg1, const Residue<N>& arg2) {
  return (arg1 < arg2) && (arg1 == arg2);
}
template<size_t N>
bool operator>(const Residue<N>& arg1, const Residue<N>& arg2) {
  return arg2 < arg1;
}
template<size_t N>
bool operator>=(const Residue<N>& arg1, const Residue<N>& arg2) {
  return !(arg1 < arg2);
}

template<size_t N>
Residue<N> operator+(const Residue<N>& arg1, const Residue<N>& arg2) {
  Residue<N> copy = arg1;
  return copy += arg2;
}

template<size_t N>
Residue<N> operator-(const Residue<N>& arg1, const Residue<N>& arg2) {
  Residue<N> copy = arg1;
  return copy -= arg2;
}
template<size_t N>
Residue<N> operator*(const Residue<N>& arg1, const Residue<N>& arg2) {
  Residue<N> copy = arg1;
  return copy *= arg2;
}
template<size_t N>
Residue<N> operator/(const Residue<N>& arg1, const Residue<N>& arg2) {
  Residue<N> copy = arg1;
  return copy /= arg2;
}

template<size_t M, size_t N, typename Field=Rational>
class Matrix {
 public:
  //_________________________CONSTRUCTORS_________________________-
  Matrix() : matrix_(N, std::vector<Field>(N, Field(0))) {
    static_assert(N == M);
    for (size_t i = 0; i < N; ++i) {
      matrix_[i][i] = Field(1);
    }
  }

  explicit Matrix(const std::vector<std::vector<Field>>& matrix) : matrix_(matrix) {}

  Matrix(const std::initializer_list<std::initializer_list<int>>& list) : matrix_(std::vector(M, std::vector(N, Field(0)))) {

    size_t row = 0;
    for (auto& i: list) {
      size_t column = 0;
      for (auto& j: i) {
        matrix_[row][column] = Field(j);
        ++column;
      }
      ++row;
    }
  }

  Matrix(const Matrix<M, N, Field>& other) = default;
  ~Matrix()=default;

  //________OPEPATORS_____________________

  Matrix<M, N, Field>& operator+=(const Matrix<M, N, Field>& other) {
    for (size_t i = 0; i < M; ++i) {
      for (size_t j = 0; j < N; j++) {
        matrix_[i][j] += other.matrix_[i][j];
      }
    }
    return *this;
  }

  Matrix<M, N, Field>& operator-=(const Matrix<M, N, Field>& other) {
    for (size_t i = 0; i < M; ++i) {
      for (size_t j = 0; j < N; j++) {
        matrix_[i][j] -= other.matrix_[i][j];
      }
    }
    return *this;
  }

  Matrix<M, N, Field>& operator*=(const Field& coefficient) {
    for (size_t i = 0; i < M; ++i) {
      for (size_t j = 0; j < N; j++) {
        matrix_[i][j] *= coefficient;
      }
    }
    return *this;
  }

  Matrix<M, N, Field>& operator*=(const Matrix<M, N, Field>& other) {
    static_assert(N == M);
    *this=*this*other;
    return *this;
  }
  const std::vector<Field>& operator[](size_t index) const {
    return matrix_[index];
  }
  std::vector<Field>& operator[](size_t index) {
    return matrix_[index];
  }

  //_______________________MAIN FUNCTIONS___________________________________

  std::vector<Field> getRow(size_t row) const {
    return matrix_[row];
  }
  std::vector<Field> getColumn(size_t column) const {
    std::vector<Field> ans(M);
    for (size_t i = 0; i < M; ++i) {
      ans[i] = matrix_[i][column];
    }
    return ans;
  }

  size_t rank() const {
    size_t rank = 0;
    Matrix<M, N, Field> copy(matrix_);
    straightStepGauss(copy);
    for (size_t row = 0; row < M; ++row) {
      if (copy[row] != std::vector(N, Field(0))) {
        ++rank;
      }
    }
    return rank;
  }

  Field det() const {
    static_assert(N == M);
    Matrix<M, N, Field> copy(matrix_);
    long long parity = straightStepGauss(copy);
    Field productDiag = copy[0][0];

    for (size_t i = 1; i < N; ++i) {
      productDiag *= copy[i][i];
    }

    return Field(parity) * productDiag;
  }

  Field trace() const {
    static_assert(N == M);
    Field trace = matrix_[0][0];

    for (size_t i = 1; i < N; ++i) {
      trace += matrix_[i][i];
    }
    return trace;
  }

  void invert() {
    *this = inverse(*this);
  }

  Matrix<N, M, Field> inverted() const {
    static_assert(N == M);
    Matrix<N, M, Field> copy(matrix_);
    copy.invert();
    return copy;
  }

  Matrix<N, M, Field> transposed() const {
    std::vector<std::vector<Field>> matrixTransposed(N, std::vector<Field>(M));
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        matrixTransposed[i][j] = matrix_[j][i];
      }
    }
    return Matrix<N, M, Field>(matrixTransposed);
  }

 private:



  static long long straightStepGauss(Matrix<M, N, Field>& matr) { //make the straighgt Gauss's step and return parity of permutations
    long long sign = 1;
    size_t row;
    Field coef1;
    Field coef2;
    size_t ColumnOfZeroes = 0;
    for (size_t j = 0; j < N; ++j) {
      coef1 = Field(0);
      row = size_t(-1);
      for (size_t i = j - ColumnOfZeroes; (i < M) && (coef1 == Field(0)); ++i) {
        if (matr[i][j] != Field(0)) {
          coef1 = matr[i][j];
          row = i;
        }
      }
      if (row != size_t(-1)) {
        for (size_t i = row + 1; i < M; ++i) {
          coef2 = matr[i][j];
          for (size_t k = j; k < N; ++k) {
            matr[i][k] -= (coef2 * matr[row][k]) / (coef1);
          }
        }
        if (row != j) {
          sign *= -1;
          std::swap(matr[row], matr[j - ColumnOfZeroes]);
        }
      } else {
        ++ColumnOfZeroes;
      }
    }
    return sign;
  }

  static Matrix<M, N, Field> inverse(Matrix<M, N, Field>& matr) { // create the unity matrix and make same actions as with "matr"
    static_assert(N == M);
    Matrix<N, M, Field> reverse;
    size_t row;
    Field coef1;
    Field coef2;
    for (size_t j = 0; j < N; ++j) {
      coef1 = Field(0);
      row = size_t(-1);
      for (size_t i = j; (i < M) && (coef1 == Field(0)); ++i) {
        if (matr[i][j] != Field(0)) {
          coef1 = matr[i][j];
          row = i;
        }
      }
      if (row != size_t(-1)) {
        for (size_t i = row + 1; i < M; ++i) {
          coef2 = matr[i][j];
          for (size_t k = 0; k < N; ++k) {
            matr[i][k] -= (coef2 * matr[row][k]) / (coef1);
            reverse[i][k] -= (coef2 * reverse[row][k]) / (coef1);
          }
        }
        if (row != j) {
          std::swap(matr[row], matr[j]);
          std::swap(reverse[row], reverse[j]);
        }
      }

    }
//Straight Step of Gauss is end, So start Reverse Step of Gauss
    for (size_t j = N; j-- > 0;) {
      for (size_t i = 0; i < N; ++i) {
        reverse[j][i] /= matr[j][j];
      }
      matr[j][j] /= matr[j][j];

      for (size_t i = j; i-- > 0;) {
        coef2 = matr[i][j];
        for (size_t k = 0; k < N; ++k) {
          matr[i][k] -= (coef2) * (matr[j][k]);
          reverse[i][k] -= (coef2) * (reverse[j][k]);
        }
      }
    }
    return reverse;
  }


  std::vector<std::vector<Field>> matrix_;
};

template<size_t M, size_t N, typename Field=Rational>
Matrix<M, N, Field> operator*(const Field& arg1, const Matrix<M, N, Field>& arg2) {
  Matrix<M, N, Field> copy(arg2);
  copy *= arg1;
  return copy;
}

template<size_t M, size_t N, size_t K, typename Field=Rational>
Matrix<M, K, Field> operator*(const Matrix<M, N, Field>& arg1, const Matrix<N, K, Field>& arg2) {
  Matrix<M, K, Field> ans;
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < K; ++j) {
      ans[i][j] = Field(0);
      for (size_t k = 0; k < N; ++k) {
        ans[i][j] += arg1[i][k] * arg2[k][j];
      }
    }
  }
  return ans;
}

template<size_t M, size_t N, typename Field=Rational>
bool operator==(const Matrix<M, N, Field>& arg1, const Matrix<M, N, Field>& arg2) {
  for (size_t row = 0; row < M; ++row) {
    for (size_t column = 0; column < N; ++column) {
      if (arg1[row][column] != arg2[row][column]) {
        return false;
      }
    }
  }
  return true;
}
template<size_t M, size_t N, typename Field=Rational>
bool operator!=(const Matrix<M, N, Field>& arg1, const Matrix<M, N, Field>& arg2) {
  return !(arg1 == arg2);
}
template<size_t M, size_t N, typename Field=Rational>
Matrix<M, N, Field> operator-(const Matrix<M, N, Field>& arg1, const Matrix<M, N, Field>& arg2) {
  Matrix<M, N, Field> copy(arg1);
  copy -= arg2;
  return copy;
}
template<size_t M, size_t N, typename Field=Rational>
Matrix<M, N, Field> operator+(const Matrix<M, N, Field>& arg1, const Matrix<M, N, Field>& arg2) {
  Matrix<M, N, Field>& copy(arg1);
  copy += arg2;
  return copy;
}

template<size_t N, typename Field=Rational>
using SquareMatrix = Matrix<N, N, Field>;

