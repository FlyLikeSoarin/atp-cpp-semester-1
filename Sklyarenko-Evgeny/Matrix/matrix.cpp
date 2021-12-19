#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using std::vector;
using std::string;

int base = 1e9;

class BigInteger {
 private:
  vector<long long> arr;
  bool is_negative = false;
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
  void change_row() {
    if (arr.empty()) {
      arr.push_back(0);
      return;
    }
    arr.push_back(arr.back());
    for (int i = int(arr.size()) - 2; i > 0; --i) arr[i] = arr[i - 1];
    arr[0] = 0;
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
    tmp.is_negative = (is_negative + 1) % 2;
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
    if (arr.size() != num.arr.size()) return arr.size() < num.arr.size();
    for (int i = arr.size() - 1; i >= 0; --i) {
      if (arr[i] != num.arr[i]) return arr[i] < num.arr[i];
    }
    return true;
  }
  bool operator!=(const BigInteger &num) const {
    return !(num == *this);
  }
  bool operator>(const BigInteger &num) const {
    if (is_negative && !num.is_negative) return false;
    else if (!is_negative && num.is_negative) return true;
    else if (is_negative && num.is_negative) {
      if (arr.size() > num.arr.size()) return false;
      else if (arr.size() < num.arr.size()) return true;
      else {
        for (int i = int(num.arr.size() - 1); i > -1; --i) {
          if (arr[i] > num.arr[i]) return false;
          else if (arr[i] < num.arr[i]) return true;
        }
      }
    } else {
      if (arr.size() < num.arr.size()) return false;
      else if (arr.size() > num.arr.size()) return true;
      else {
        for (int i = int(num.arr.size()) - 1; i > -1; --i) {
          if (arr[i] > num.arr[i]) return true;
          else if (arr[i] < num.arr[i]) return false;
        }
      }
    }
    return false;
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
  BigInteger &operator+=(const BigInteger &num) {
    if (!((is_negative + num.is_negative) % 2)) {
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
  BigInteger &operator/=(const BigInteger &num) {
    bool flag = (is_negative + num.is_negative) % 2;
    is_negative = false;
    BigInteger div = (num < 0 ? -num : num);
    BigInteger new_div = 0;
    int counter = int(arr.size()) - 1;
    while (new_div < div) {
      new_div *= base;
      new_div += static_cast<int>(arr[counter--]);
    }
    BigInteger result = 0;

    while (counter >= -1) {
      long long l, r;
      if (new_div.arr.size() - div.arr.size() >= 2) {
        l = base - 1;
        r = base;
      }
      else if (new_div.arr.size() - div.arr.size() == 1) {
        l = base / (div.arr[div.arr.size() - 1] + 1);
        r = base;
      }
      else if (new_div.arr.size() == div.arr.size()) {
        l = new_div.arr[new_div.arr.size() - 1] / (div.arr[div.arr.size() - 1] + 1);
        r = (new_div.arr[new_div.arr.size() - 1] + 1) / div.arr[div.arr.size() - 1] + 1;
      }
      else {
        l = 0;
        r = 1;
      }
      while (l < r - 1) {
        long long m = (l + r) / 2;
        if (div * m <= new_div) l = m;
        else r = m;
      }
      result *= base;
      result += l;
      if (counter != -1) {
        new_div -= div * l;
        new_div *= base;
        new_div += static_cast<int>(arr[counter]);
      }
      --counter;
    }
    *this = result;
    is_negative = flag;
    remove_zeros();
    return *this;
  }
  BigInteger &operator%=(const BigInteger &num) {
    return (*this = (*this - num * (*this / num)));
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
  ~BigInteger() = default;
  friend std::istream &operator>>(std::istream &in, BigInteger &num);
  friend std::ostream &operator<<(std::ostream &out, const BigInteger &num);
  friend bool operator==(const BigInteger &num1, const BigInteger &num2);
  friend BigInteger operator+(const BigInteger &num1, const BigInteger &num2);
  friend BigInteger operator-(const BigInteger &num1, const BigInteger &num2);
  friend BigInteger operator*(const BigInteger &num1, const BigInteger &num2);
  friend BigInteger operator/(const BigInteger &num1, const BigInteger &num2);
  friend BigInteger operator%(const BigInteger &num1, const BigInteger &num2);
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
  void irreducible() {
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
    irreducible();
    return *this;
  }
  Rational &operator-=(const Rational &num) {
    numenator = numenator * num.denominator - num.numenator * denominator;
    denominator *= num.denominator;
    irreducible();
    return *this;
  }
  Rational &operator/=(const Rational &num) {
    numenator *= num.denominator;
    denominator *= num.numenator;
    irreducible();
    return *this;
  }
  Rational &operator*=(const Rational &num) {
    numenator *= num.numenator;
    denominator *= num.denominator;
    irreducible();
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
  num = 0;
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
  tmp.irreducible();
  return tmp;
}
Rational operator-(const Rational &num1, const Rational &num2) {
  Rational tmp = num1;
  tmp -= num2;
  tmp.irreducible();
  return tmp;
}
Rational operator*(const Rational &num1, const Rational &num2) {
  Rational tmp = num1;
  tmp *= num2;
  tmp.irreducible();
  return tmp;
}
Rational operator/(const Rational &num1, const Rational &num2) {
  Rational tmp = num1;
  tmp /= num2;
  tmp.irreducible();
  return tmp;
}

template<size_t N>
class Residue;
template <size_t N>
std::ostream& operator<<(std::ostream& out, Residue<N>& num);

template <size_t N, size_t D, bool flag = false>
struct IsPrime {
  static const bool isprime = (N % D != 0) && (IsPrime<N, D + 1, (D * D >= N)>::isprime);
};

template <size_t N, size_t D>
struct IsPrime <N, D, true>{
  static const bool isprime = true;
};

template<size_t N>
class Residue {
 private:
  size_t Number;
 public:
  Residue(): Number(0) {}
  Residue(int k): Number((k >= 0) ? (k % N) : ((N - -k % N) % N)) {
    if (k >= 0) k = k % N;
    else k = (N - -k % N) % N;
  }
  Residue(const Residue<N>& num): Number(num.Number) {}
  Residue<N>& operator+=(const Residue<N>& Num) {
    Number = (Number + Num.Number) % N;
    return *this;
  }
  Residue<N>& operator-=(const Residue<N>& Num) {
    Number += N;
    Number -= Num.Number;
    Number %= N;
    return *this;
  }
  Residue<N>& operator*=(const Residue<N>& Num) {
    Number = (Number * Num.Number) % N;
    return *this;
  }
  template<typename = typename std::enable_if<IsPrime<N, 2>::isprime>>
  Residue<N>& operator/=(const Residue<N>& num1) {
    size_t result = 1;
    size_t tmp = N - 2;
    while (tmp) {
      if (tmp & 1) {
        result = (result * num1.Number) % N;
      }
      if (tmp != 1)  result = (result * result) % N;
      tmp >>= 1;
    }
    this->Number *= result;
    this->Number %= N;
    return *this;
  }
  explicit operator int() const {
    return Number;
  }
  bool operator==(const Residue<N>& num) const {
    return (Number == num.Number);
  }
  bool operator!=(const Residue<N>& num) const {
    return (Number != num.Number);
  }
  ~Residue() = default;
  friend std::ostream &operator<<<N>(std::ostream &out, Residue<N> &num);
};

template<size_t N>
std::ostream &operator<<(std::ostream &out, Residue<N> &num) {
  out << num.Number;
  return out;
}

template<size_t N>
Residue<N> operator+(const Residue<N>& num1, const Residue<N>& num2) {
  Residue<N> tmp = num1;
  return (tmp += num2);
}
template<size_t N>
Residue<N> operator-(const Residue<N>& num1, const Residue<N>& num2) {
  Residue<N> tmp = num1;
  return (tmp -= num2);
}
template<size_t N>
Residue<N> operator*(const Residue<N>& num1, const Residue<N>& num2) {
  Residue<N> tmp = num1;
  return (tmp *= num2);
}
template<size_t N>
Residue<N> operator/(const Residue<N>& num1, const Residue<N>& num2) {
  Residue<N> tmp = num1;
  return (tmp /= num2);
}


template<size_t M, size_t N = M, typename Field = Rational>
class Matrix {
 public:
  Matrix(const vector<vector<Field>> &matrix1) : matrix(matrix1) {}
  template<typename = typename std::enable_if<N == M>>
  Matrix(): matrix(vector(M, vector(M, Field(0)))) {
    for (size_t i = 0; i < M; ++i) matrix[i][i] = Field(1);
  }
  Matrix(const Field &num) : matrix(vector(M, vector(N, num))) {}
  Matrix(std::initializer_list<std::initializer_list<int>> arr): matrix(vector(M, vector(N, Field()))) {
    size_t row = 0;
    for (auto it : arr) {
      size_t column = 0;
      for (auto itt = it.begin(); itt != it.end(); ++itt) matrix[row][column++] = Field(*itt);
      ++row;
    }
  }
  ~Matrix() = default;
  Matrix<M, N, Field> &operator=(const Matrix<M, N, Field> &matrix1) {
    for (size_t i = 0; i < M; ++i) {
      for (size_t j = 0; j < N; ++j) {
        matrix[i][j] = matrix1[i][j];
      }
    }
    return *this;
  }
  Matrix<M, N, Field> &operator+=(const Matrix<M, N, Field> &matrix1) {
    for (size_t i = 0; i < M; ++i) {
      for (size_t j = 0; j < N; ++j) {
        matrix[i][j] += matrix1[i][j];
      }
    }
    return *this;
  }
  Matrix<M, N, Field> &operator-=(const Matrix<M, N, Field> &matrix1) {
    for (size_t i = 0; i < M; ++i) {
      for (size_t j = 0; j < N; ++j) {
        matrix[i][j] -= matrix1[i][j];
      }
    }
    return *this;
  }
  Matrix<M, N, Field> &operator*=(const Field &num) {
    for (size_t i = 0; i < M; ++i) {
      for (size_t j = 0; j < N; ++j) {
        matrix[i][j] *= num;
      }
    }
    return *this;
  }
  template<typename = typename std::enable_if<N == M>>
  [[nodiscard]] Field det() const {
    std::pair<Matrix<M, N, Field>, int> matrix_det = triangulation();
    Field determinator = (matrix_det.second % 2 ? -1 : 1);
    for (size_t i = 0; i < M; ++i) {
      determinator *= matrix_det.first[i][i];
    }
    return determinator;
  }
  template<typename = typename std::enable_if<N == M>>
  [[nodiscard]] size_t rank() const {
    std::pair<Matrix<M, N, Field>, int> matrix_det = triangulation();
    size_t row = 0;
    for (size_t column = 0; column < N; ++column) {
      if (matrix_det.first[row][column] != 0) ++row;
    }
    return row;
  }
  [[nodiscard]] Matrix<N, M, Field> transposed() const {
    vector<vector<Field>> matrix1(N, vector(M, Field()));
    for (size_t i = 0; i < M; ++i)
      for (size_t j = 0; j < N; ++j)
        matrix1[j][i] = matrix[i][j];
    Matrix<M, M, Field> tmp(matrix1);
    return matrix1;
  }
  template<typename = typename std::enable_if<N == M>>
  Matrix<M, N, Field> &operator*=(const Matrix<M, N, Field> &matrix1) {
    Matrix<M, N, Field> matrix_new = *this;
    *this = matrix_new * matrix1;
    return *this;
  }
  bool operator==(const Matrix<M, N, Field> &matrix1) const {
    for (size_t i = 0; i < M; ++i) {
      for (size_t j = 0; j < N; ++j) {
        if (matrix[i][j] != matrix1[i][j]) return false;
      }
    }
    return true;
  }
  template<typename = typename std::enable_if<N == M>>
  Matrix<M, N, Field> &invert() {
    for (size_t i = 0; i < M; ++i) {
      for (size_t j = 0; j < N; ++j) {
      }
    }
    Matrix<M, M, Field> E;
    for (size_t i = 0; i < M; ++i) {
      size_t row = i;
      while (row < M - 1 && matrix[row][i] == Field(0)) ++row;
      if (row != i) {
        std::swap(matrix[row], matrix[i]);
        std::swap(E[row], E[i]);
      }
      if (matrix[i][i] != Field(0)) {
        Field tmp = matrix[i][i];
        for (size_t j = 0; j < M; ++j) {
          matrix[i][j] /= tmp;
          E[i][j] /= tmp;
        }
        for (size_t j = i + 1; j < M; ++j) {
          Field dividor = matrix[j][i] / matrix[i][i];
          for (size_t z = 0; z < N; ++z) {
            matrix[j][z] -= dividor * matrix[i][z];
            E[j][z] -= dividor * E[i][z];
          }
        }
      } else return *this;
    }
    for (size_t j = M - 1; j >= 1; --j) {
      for (size_t z = 0; z < j; ++z) {
        Field dividor = matrix[z][j];
        matrix[z][j] = Field(0);
        for (size_t k = 0; k < M; ++k) {
          E[z][k] -= dividor * E[j][k];
        }
      }
    }
    *this = E;
    return *this;
  }
  bool operator!=(const Matrix<M, N, Field> &matrix1) const {
    return !(*this == matrix1);
  }
  template<typename = typename std::enable_if<N == M>>
  [[nodiscard]] Field trace() const {
    Field tmp(0);
    for (size_t i = 0; i < N; ++i) tmp += matrix[i][i];
    return tmp;
  }
  [[nodiscard]] vector<Field> getRow(size_t row) const {
    return matrix[row];
  }
  [[nodiscard]] vector<Field> getColumn(size_t column) const {
    vector<Field> tmp(M);
    for (size_t i = 0; i < M; ++i) {
      tmp[i] = matrix[i][column];
    }
    return tmp;
  }
  template<typename = typename std::enable_if<N == M>>
  Matrix<M, N, Field> inverted() {
    Matrix<M, N, Field> tmp = *this;
    return tmp.invert();
  }
  vector<Field> &operator[](size_t ind) {
    return matrix[ind];
  }
  vector<Field> operator[](size_t ind) const {
    return matrix[ind];
  }
  private:
  vector<vector<Field>> matrix;
  [[nodiscard]] std::pair<Matrix<M, N, Field>, size_t> triangulation() const {
    Matrix<M, N, Field> matrix1 = *this;
    size_t amount_of_swaps = 0;
    for (size_t i = 0; i < std::min(N, M); ++i) {
      size_t row = i;
      while (row < M - 1 && matrix1[row][i] == Field(0)) ++row;
      if (row != i) {
        std::swap(matrix1[row], matrix1[i]);
        ++amount_of_swaps;
      }
      if (matrix1[i][i] != 0)
        for (size_t j = i + 1; j < M; ++j) {
          Field dividor = matrix1[j][i] / matrix1[i][i];
          for (size_t z = i; z < N; ++z) {
            matrix1[j][z] -= matrix1[i][z] * dividor;
          }
        }
    }
    return {matrix1, amount_of_swaps};
  }
};

template<size_t M, size_t N, typename Field = Rational>
Matrix<M, N, Field> operator+(const Matrix<M, N, Field> &matrix1, const Matrix<M, N, Field> &matrix2) {
  Matrix<M, N, Field> matrix = matrix1;
  return (matrix += matrix2);
}
template<size_t M, size_t N, typename Field = Rational>
Matrix<M, N, Field> operator-(const Matrix<M, N, Field> &matrix1, const Matrix<M, N, Field> &matrix2) {
  Matrix<M, N, Field> matrix = matrix1;
  return (matrix -= matrix2);
}
template<size_t M, size_t N, size_t K, typename Field = Rational>
Matrix<M, K, Field> operator*(const Matrix<M, N, Field> &matrix1, const Matrix<N, K, Field> &matrix2) {
  Matrix<M, K, Field> tmp(Field(0));
  for (size_t i = 0; i < M; ++i)
    for (size_t j = 0; j < K; ++j) {
      for (size_t z = 0; z < N; ++z) {
        tmp[i][j] += matrix1[i][z] * matrix2[z][j];
      }
    }
  return tmp;
}
template<size_t M, size_t N, typename Field = Rational>
Matrix<M, N, Field> operator*(const Field &num, const Matrix<M, N, Field> &matrix1) {
  Matrix<M, N, Field> matrix = matrix1;
  return (matrix *= num);
}

template<size_t M, typename Field = Rational>
using SquareMatrix = Matrix<M, M, Field>;
