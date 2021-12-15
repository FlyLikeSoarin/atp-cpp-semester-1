#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
class BigInteger {
 public:
  friend class Rational;
  friend std::istream& operator>>(std::istream& in, BigInteger& integer);
  friend std::ostream& operator<<(std::ostream& out, const BigInteger& integer);
  friend bool operator<(const BigInteger& first, const BigInteger& second);
  explicit BigInteger() : integer(1, 0), sign(1) {}
  BigInteger(int numb) : integer(0), sign(1) {
    if (numb < 0) {
      sign = -1;
      numb = -numb;
    }
    do {
      integer.push_back(numb % base);
      numb /= base;
    } while (numb != 0);
  }
  BigInteger& operator+=(const BigInteger& anotherInteger) {
    (getSign() == anotherInteger.getSign()) ? addDigits(anotherInteger) : subtractNumber(anotherInteger);
    return *this;
  }
  BigInteger& operator-=(const BigInteger& anotherInteger) {
    (getSign() == anotherInteger.getSign()) ? subtractNumber(anotherInteger) : addDigits(anotherInteger);
    return *this;
  }
  BigInteger& operator*=(const BigInteger& anotherInteger) {
    sign *= anotherInteger.getSign();
    if (integer.empty()) {
      return *this;
    }
    std::vector<int> newInteger(integer.size() + anotherInteger.size() + 2, 0);
    for (size_t i = 0; i < anotherInteger.size(); ++i) {
      long long debt = 0;
      for (size_t j = 0; j < integer.size(); ++j) {
        debt += static_cast<long long>(integer[j]) * anotherInteger[i] + newInteger[i + j];
        newInteger[i + j] = static_cast<int>(debt % base);
        debt /= base;
      }
      if (debt != 0) {
        newInteger[i + integer.size()] = static_cast<int>(debt);
      }
    }
    integer = newInteger;
    clearNotSignificant();
    return *this;
  }
  BigInteger& operator/=(const BigInteger& anotherInteger) {
    std::vector<int> newInteger(1, 0);
    remainderAndDivision(anotherInteger, newInteger);
    integer = newInteger;
    sign *= anotherInteger.getSign();
    return *this;
  }
  BigInteger& operator%=(const BigInteger& anotherInteger) {
    std::vector<int> newInteger(0);
    remainderAndDivision(anotherInteger, newInteger);
    return *this;
  }
  BigInteger& operator++() {
    if (isZero()) {
      sign = 1;
      integer[0] = 1;
      return *this;
    }
    int debt = ((getSign() == 1) ? 1 : -1);
    pushDebt(integer, debt, 0, base);
    return *this;
  }
  BigInteger& operator--() {
    if (isZero()) {
      sign = -1;
      integer[0] = 1;
    } else {
      int debt = (getSign() == -1) ? 1 : -1;
      pushDebt(integer, debt, 0, base);
    }
    return *this;
  }
  BigInteger operator++(int) {
    BigInteger copyInteger(*this);
    ++(*this);
    return (copyInteger);
  }
  BigInteger operator--(int) {
    BigInteger copyInteger(*this);
    --(*this);
    return (copyInteger);
  }
  BigInteger operator-() {
    BigInteger copyInteger(*this);
    copyInteger.sign *= -1;
    return copyInteger;
  }
  explicit operator bool() const {
    return !isZero();
  }
  [[nodiscard]] std::string toString() const {
    if (integer.empty() || isZero()) {
      return "0";
    }
    std::stringstream answerStream;
    if (getSign() == -1) {
      answerStream << '-';
    }
    for (size_t i = integer.size(); i > 0; --i) {
      if (i != integer.size()) {
        for (int k = base / 10; k > integer[i - 1]; k /= 10) {
          answerStream << 0;
        }
      }
      if (integer[i - 1] != 0) {
        answerStream << integer[i - 1];
      }
    }
    std::string ans = answerStream.str();
    return answerStream.str();
  }

 private:
  std::vector<int> integer;
  int sign;
  int base = 100000000;
  int amountInBlock = 8;
  [[nodiscard]] int modCompare(const BigInteger& anotherInteger) const {
    const int more = 1;
    const int equal = 0;
    const int less = -1;
    if (integer.size() < anotherInteger.size()) {
      return less;
    }
    if (integer.size() > anotherInteger.size()) {
      return more;
    }
    for (size_t i = integer.size(); i > 0; --i) {
      if (integer[i - 1] < anotherInteger[i - 1]) {
        return less;
      }
      if (integer[i - 1] > anotherInteger[i - 1]) {
        return more;
      }
    }
    return equal;
  }
  void clearNotSignificant() {
    for (size_t i = integer.size(); i > 1; --i) {
      if (integer[i - 1] != 0) {
        break;
      }
      integer.pop_back();
    }
  }
  void addDigits(const BigInteger& anotherInteger) {
    int debt = 0;
    size_t index = 0;
    for (; index < anotherInteger.size(); ++index) {
      if (integer.size() == index) {
        integer.push_back(0);
      }
      debt += integer[index] + anotherInteger[index];
      integer[index] = debt % base;
      debt /= base;
    }
    pushDebt(integer, debt, index, base);
  }
  void subtractDigits(const BigInteger& anotherInteger, int deltaSign, long long indexDelta = 0, int numberDelta = 1) {
    if (indexDelta == 0 && numberDelta == 1 && anotherInteger.integer == integer) {
      integer.clear();
      integer.push_back(0);
      return;
    }
    if (deltaSign == 0) {
      deltaSign = 1;
    }
    int debt = 0;
    size_t index = indexDelta;
    while (debt != 0 || index < anotherInteger.size() + indexDelta) {
      if (integer.size() == index) {
        integer.push_back(0);
      }
      debt += deltaSign * integer[index];
      if (index - indexDelta < anotherInteger.size()) {
        debt -= deltaSign * ((anotherInteger[index - indexDelta] % (base / numberDelta)) * numberDelta);
      }
      if (debt % base < 0) {
        integer[index] = base + (debt % base);
        debt = debt / base - 1;
      } else {
        integer[index] = debt % base;
        debt /= base;
      }
      if (index - indexDelta < anotherInteger.size()) {
        debt -= deltaSign * anotherInteger[index - indexDelta] / (base / numberDelta);
      }
      ++index;
    }
    if (deltaSign == -1) {
      for (size_t i = index; i < integer.size(); ++i) {
        integer[i] *= deltaSign;
      }
    }
    clearNotSignificant();
  }
  void subtractNumber(const BigInteger& anotherInteger) {
    bool isModLess = (modCompare(anotherInteger) == -1);
    int deltaSign = isModLess ? -1 : 1;
    sign *= deltaSign;
    subtractDigits(anotherInteger, deltaSign);
  }
  static void pushDebt(std::vector<int>& integerArray, long long debt, size_t index, int base) {
    while (debt != 0) {
      while (integerArray.size() <= index) {
        integerArray.push_back(0);
      }
      debt += integerArray[index];
      integerArray[index] = static_cast<int>(debt % base);
      debt /= base;
      ++index;
    }
  }
  void remainderAndDivision(const BigInteger& anotherInteger, std::vector<int>& newInteger) {
    int compare = modCompare(anotherInteger);
    while (compare == 1) {
      long long indexDelta = static_cast<long long>(integer.size()) - static_cast<long long>(anotherInteger.size());
      long long dividend = integer[integer.size() - 1];
      int numberDelta = 1;
      if (!(indexDelta == 0 && dividend <= anotherInteger[anotherInteger.size() - 1])) {
        if (dividend <= anotherInteger[anotherInteger.size() - 1]) {
          dividend *= base;
          indexDelta -= 1;
        }
        while (((dividend / numberDelta) / 10) > anotherInteger[anotherInteger.size() - 1]) {
          numberDelta *= 10;
        }
      } else {
      }
      subtractDigits(anotherInteger, 1, indexDelta, numberDelta);
      if (!newInteger.empty()) {
        pushDebt(newInteger, numberDelta, indexDelta, base);
      }
      compare = modCompare(anotherInteger);
    }
    if (compare == 0) {
      if (!newInteger.empty()) {
        pushDebt(newInteger, 1, 0, base);
      }
      integer.clear();
      integer.push_back(0);
    }
  }
  int& operator[](size_t index) {
    return integer[index];
  }
  const int& operator[](size_t index) const {
    return integer[index];
  }
  [[nodiscard]] size_t size() const {
    return integer.size();
  }
  [[nodiscard]] bool isZero() const {
    return (integer.size() == 1 && integer[0] == 0);
  }
  [[nodiscard]] int getSign() const {
    return (isZero()) ? 1 : sign;
  }
  void changeSign() {
    sign *= -1;
  }
};

BigInteger operator+(const BigInteger& first, const BigInteger& second) {
  BigInteger copyInteger(first);
  copyInteger += second;
  return copyInteger;
}
BigInteger operator-(const BigInteger& first, const BigInteger& second) {
  BigInteger copyInteger(first);
  copyInteger -= second;
  return copyInteger;
}
BigInteger operator*(const BigInteger& first, const BigInteger& second) {
  BigInteger copyInteger(first);
  copyInteger *= second;
  return copyInteger;
}
BigInteger operator/(const BigInteger& first, const BigInteger& second) {
  BigInteger copyInteger(first);
  copyInteger /= second;
  return copyInteger;
}
BigInteger operator%(const BigInteger& first, const BigInteger& second) {
  BigInteger copyInteger(first);
  copyInteger %= second;
  return copyInteger;
}
bool operator<(const BigInteger& first, const BigInteger& second) {
  if (first.size() == 1 && first.integer[0] == 0 && second.size() == 1 && second[0] == 0) {
    return false;
  }
  if (first.getSign() < second.getSign()) {
    return true;
  }
  if (first.getSign() > second.getSign()) {
    return false;
  }
  return first.modCompare(second) * first.getSign() == -1;
}
bool operator==(const BigInteger& first, const BigInteger& second) {
  return (!(first < second) && !(second < first));
}
bool operator>(const BigInteger& first, const BigInteger& second) {
  return second < first;
}
bool operator<=(const BigInteger& first, const BigInteger& second) {
  return !(second < first);
}
bool operator>=(const BigInteger& first, const BigInteger& second) {
  return !(first < second);
}
bool operator!=(const BigInteger& first, const BigInteger& second) {
  return !(first == second);
}
std::ostream& operator<<(std::ostream& out, const BigInteger& bigInteger) {
  out << bigInteger.toString();
  return out;
}
std::istream& operator>>(std::istream& in, BigInteger& bigInteger) {
  std::string stringInteger;
  in >> stringInteger;
  size_t numberStart = 0;
  bigInteger.sign = 1;
  if (stringInteger[0] == '-') {
    ++numberStart;
    bigInteger.sign = -1;
  }
  size_t delta = (stringInteger.size() - numberStart) % bigInteger.amountInBlock;
  size_t currentIndex = (stringInteger.size() - numberStart) / bigInteger.amountInBlock + (delta != 0 ? 1 : 0);
  bigInteger.integer.resize(currentIndex);
  if (delta != 0) {
    bigInteger.integer[currentIndex - 1] = std::stoi(stringInteger.substr(numberStart, delta));
    numberStart += delta;
    --currentIndex;
  }
  delta = bigInteger.amountInBlock;
  for (; currentIndex > 0; --currentIndex) {
    bigInteger.integer[currentIndex - 1] = std::stoi(stringInteger.substr(numberStart, delta));
    numberStart += delta;
  }
  if (bigInteger.isZero()) {
    bigInteger.sign = 1;
  }
  return in;
}

class Rational {
 public:
  friend std::istream& operator>>(std::istream& in, Rational& rational) {
    BigInteger input;
    in >> input;
    rational = Rational(input);

    return in;
  }
  friend bool operator<(const Rational& first, const Rational& second);
  Rational() : numerator(0), denominator(1), sign(1) {}
  Rational(int intNumerator) : numerator(std::abs(intNumerator)),
      denominator(1), sign(intNumerator < 0 ? -1 : 1) {}
  Rational(const BigInteger& bigNumerator) : numerator(bigNumerator), denominator(1),
                                             sign(bigNumerator.getSign()) {
    if (numerator.getSign() == -1) {
      numerator.changeSign();
    }
  }
  Rational& operator+=(const Rational& anotherRational) {
    (anotherRational.getSign() != getSign()) ? Subtract(anotherRational) : Add(anotherRational);
    return *this;
  }
  Rational& operator-=(const Rational& anotherRational) {
    (anotherRational.getSign() == getSign()) ? Subtract(anotherRational) : Add(anotherRational);
    return *this;
  }
  Rational& operator*=(const Rational& anotherRational) {
    sign *= anotherRational.getSign();
    numerator *= anotherRational.numerator;
    denominator *= anotherRational.denominator;
    ReduceFraction();
    return *this;
  }
  Rational& operator/=(const Rational& anotherRational) {
    if (this->numerator == anotherRational.numerator && this->denominator == anotherRational.denominator) {
      this->numerator = 1;
      this->denominator = 1;
      sign *= anotherRational.getSign();
    } else {
      sign *= anotherRational.getSign();
      numerator *= anotherRational.denominator;
      denominator *= anotherRational.numerator;
      ReduceFraction();
    }
    return *this;
  }
  Rational operator-() const {
    Rational copyRational(*this);
    copyRational.sign *= -1;

    return copyRational;
  }
  [[nodiscard]] std::string toString() const {
    std::string answer = ((getSign() == -1) ? "-" : "") + numerator.toString();
    if (denominator != 1) {
      answer += "/" + denominator.toString();
    }
    return answer;
  }
  std::string asDecimal(size_t precision = 0) {
    std::string answer;
    BigInteger numeratorCopy = numerator;
    BigInteger remainder = numeratorCopy % denominator;
    numeratorCopy /= denominator;
    std::string signDecimal = (getSign() == -1 && (!numeratorCopy.isZero() || !remainder.isZero())) ? "-" : "";
    if (precision == 0) {
      answer = numeratorCopy.toString();
    } else if (precision > 0) {
      size_t differenceInBlocks = denominator.integer.size() - remainder.size();
      int digitsAmountDenominator = GetPowerOfTen(denominator.integer[denominator.integer.size() - 1]);
      int digitsAmountRemainder = GetPowerOfTen(remainder.integer[remainder.integer.size() - 1]);
      int tenPowerRemainder = TenToPower(digitsAmountRemainder);
      int tenPowerDenominator = TenToPower(digitsAmountDenominator);
      if (digitsAmountDenominator < digitsAmountRemainder) {
        digitsAmountDenominator += denominator.amountInBlock;
        --differenceInBlocks;
        tenPowerDenominator *= denominator.base / tenPowerRemainder;
      } else {
        tenPowerDenominator /= tenPowerRemainder;
      }
      MoveDigits(remainder, differenceInBlocks, tenPowerDenominator);
      size_t countLeadingZeros =
          differenceInBlocks * denominator.amountInBlock + digitsAmountDenominator - digitsAmountRemainder - 1;
      int numberToMultiply = 1;
      if (denominator > remainder) {
        numberToMultiply *= 10;
        ++countLeadingZeros;
      }
      if (precision < countLeadingZeros) {
        answer = signDecimal + numeratorCopy.toString() +
            "." + std::string(precision, '0');
      } else {
        precision = precision + 1 - countLeadingZeros;
        size_t blocksNumber = (precision) / remainder.amountInBlock;
        int power = static_cast<int>((precision) % remainder.amountInBlock);
        if (power + 1 == remainder.amountInBlock && numberToMultiply == 10) {
          numberToMultiply = 1;
          ++blocksNumber;
        } else {
          while (power != 0) {
            numberToMultiply *= 10;
            --power;
          }
        }
        MoveDigits(remainder, blocksNumber, numberToMultiply);
        remainder /= denominator;
        int firstBeforeRounding = remainder.integer[remainder.integer.size() - 1];
        size_t blocksBeforeRounding = remainder.integer.size();
        if (remainder[0] >= 5) {
          remainder += 10;
        }
        if (remainder.size() != blocksBeforeRounding ||
            GetPowerOfTen(firstBeforeRounding) < GetPowerOfTen(
                remainder.integer[remainder.integer.size() - 1])) {
          if (countLeadingZeros != 0) {
            ++precision;
            --countLeadingZeros;
          } else {
            numeratorCopy += 1;
          }
        }

        answer = signDecimal +
            numeratorCopy.toString() + "." + std::string(countLeadingZeros, '0');
        if (precision != 0) {
          answer += remainder.toString().substr(0, precision - 1);
        }
      }
    }
    return answer;
  }
  explicit operator double() {
    return std::stod(asDecimal(16));
  }
  [[nodiscard]] int getSign() const {
    if (sign == 1 || numerator.isZero()) {
      return 1;
    }
    return -1;
  }

 private:
  BigInteger numerator;
  BigInteger denominator;
  int sign;
  static BigInteger CountGcd(const BigInteger& first, const BigInteger& second) {
    BigInteger firstCopy = first;
    BigInteger secondCopy = second;
    while (true) {
      firstCopy %= secondCopy;
      if (firstCopy.isZero()) {
        return secondCopy;
      }
      secondCopy %= firstCopy;
      if (secondCopy.isZero()) {
        return firstCopy;
      }
    }
  }
  void ReduceFraction() {
    BigInteger gcd = CountGcd(numerator, denominator);
    numerator /= gcd;
    denominator /= gcd;
  }
  void Add(const Rational& anotherRational) {
    if (this->numerator == anotherRational.numerator && this->denominator == anotherRational.denominator) {
      this->numerator *= 2;
    } else {
      if (numerator.isZero()) {
        sign = 1;
      }
      numerator *= anotherRational.denominator;
      numerator += anotherRational.numerator * denominator;
      denominator *= anotherRational.denominator;
    }
    ReduceFraction();
  }
  void Subtract(const Rational& anotherRational) {
    if (this->numerator == anotherRational.numerator && this->denominator == anotherRational.denominator) {
      this->numerator = 0;
      this->denominator = 1;
      return;
    }
    if (numerator.isZero()) {
      sign = 1;
    }
    numerator *= anotherRational.denominator;
    numerator -= anotherRational.numerator * denominator;
    if (numerator.getSign() == -1) {
      sign *= -1;
      numerator.changeSign();
    }
    denominator *= anotherRational.denominator;
    ReduceFraction();
  }
  static void MoveDigits(BigInteger& bigInteger, size_t blocksNumber, int numberToMultiply) {
    std::vector<int> newInteger(blocksNumber + bigInteger.integer.size(), 0);
    int debt = 0;
    for (size_t i = 0; i < bigInteger.integer.size(); ++i) {
      debt += (bigInteger.integer[i] % (bigInteger.base / numberToMultiply)) * numberToMultiply;
      newInteger[i + blocksNumber] = debt % bigInteger.base;
      debt /= bigInteger.base;
      debt += bigInteger.integer[i] / (bigInteger.base / numberToMultiply);
    }
    if (debt != 0) {
      newInteger.push_back(debt);
    }
    bigInteger.integer = newInteger;
  }
  static int TenToPower(int power) {
    int number = 1;
    while (power != 0) {
      number *= 10;
      --power;
    }
    return number;
  }
  static int GetPowerOfTen(int numberBlock) {
    int k = 1;
    int number = 10;
    while (number <= numberBlock) {
      ++k;
      number *= 10;
    }
    return k;
  }
};

Rational operator+(const Rational& first, const Rational& second) {
  Rational copyInteger(first);
  copyInteger += second;
  return copyInteger;
}
Rational operator-(const Rational& first, const Rational& second) {
  Rational copyInteger(first);
  copyInteger -= second;
  return copyInteger;
}
Rational operator*(const Rational& first, const Rational& second) {
  Rational copyInteger(first);
  copyInteger *= second;
  return copyInteger;
}
Rational operator/(const Rational& first, const Rational& second) {
  Rational copyInteger(first);
  copyInteger /= second;
  return copyInteger;
}
bool operator<(const Rational& first, const Rational& second) {
  if (first.getSign() < second.getSign()) {
    return true;
  }
  if (first.getSign() > second.getSign()) {
    return false;
  }
  if (first.getSign() == -1) {
    return (first.numerator * second.denominator > first.denominator * second.numerator);
  }
  return (first.numerator * second.denominator < first.denominator * second.numerator);
}
bool operator==(const Rational& first, const Rational& second) {
  return (!(first < second) && !(second < first));
}
bool operator>(const Rational& first, const Rational& second) {
  return second < first;
}
bool operator<=(const Rational& first, const Rational& second) {

  return !(second < first);
}
bool operator>=(const Rational& first, const Rational& second) {
  return !(first < second);
}
bool operator!=(const Rational& first, const Rational& second) {
  return !(first == second);
}

template<size_t N, size_t K>
struct IsPrimeHelper {
  static const bool value = N % K != 0 && IsPrimeHelper<N, K - 1>::value;
};
template<size_t N>
struct IsPrimeHelper<N, 1> {
  static const bool value = true;
};
template<size_t N>
struct IsPrime {
  static const bool value = IsPrimeHelper<N, static_cast<unsigned long long>(std::sqrt(N))>::value;
};

template<size_t N>
class Residue {
  static Residue<N> BinaryPower(const Residue<N>& base, unsigned long long p) {
    if (p == 1) {
      return base;
    }
    if (p % 2 == 0) {
      Residue<N> power = BinaryPower(base, p / 2);
      power *= power;
      return power;
    } else {
      Residue<N> power = BinaryPower(base, p - 1);
      power *= base;
      return power;
    }
  }
 public:
  unsigned long long value;

  Residue() = default;
  explicit Residue(int a) : value((a < 0) ? (N - ((-a) % N)) : (a % N)) {
  }
  explicit operator int() {
    return value;
  }
  Residue<N>& operator+=(const Residue<N>& another) {
    (N - another.value <= value) ? value -= N - another.value : value += another.value;
    return *this;
  }
  Residue<N>& operator-=(const Residue<N>& another) {
    (another.value > value) ? value += N - another.value : value -= another.value;
    return *this;
  }
  Residue<N>& operator*=(const Residue<N>& another) {

    if (value == 0 || another.value == 0) {
      value = 0;
      return *this;
    }
    unsigned long long oldValue = value;
    unsigned long long count = another.value - 1;
    unsigned long long k = (N - value) / oldValue;
    while (k < count) {
      count -= k + 1;
      value = oldValue - (N - k * oldValue - value);
      k = (N - value) / oldValue;
    }
    value = (value + count * oldValue) % N;
    return *this;
  }
  Residue<N>& operator/=(const Residue<N>& another) {
    static_assert(IsPrime<N>::value);
    return *this *= GetReverse(another);
  }

  Residue<N> operator-() const {
    return Residue<N>((N - value) % N);
  }

  static Residue<N> GetReverse(const Residue<N>& residue) {
    return (N <= 2) ? residue : BinaryPower(residue, N - 2);
  }

  friend bool operator==(const Residue<N>& first, const Residue<N>& second) {
    return first.value == second.value;
  }
  [[nodiscard]] std::string toString() const {
    std::stringstream answerStream;
    answerStream << value;
    return answerStream.str();
  }
};

template<size_t N>
Residue<N> operator+(const Residue<N>& first, const Residue<N>& second) {
  Residue<N> copyResidue(first);
  copyResidue += second;
  return copyResidue;
}

template<size_t N>
Residue<N> operator-(const Residue<N>& first, const Residue<N>& second) {
  Residue<N> copyResidue(first);
  copyResidue -= second;
  return copyResidue;
}

template<size_t N>
Residue<N> operator*(const Residue<N>& first, const Residue<N>& second) {
  Residue<N> copyResidue(first);
  copyResidue *= second;
  return copyResidue;
}

template<size_t N>
Residue<N> operator/(const Residue<N>& first, const Residue<N>& second) {
  Residue<N> copyResidue(first);
  copyResidue /= second;
  return copyResidue;
}
template<size_t N>
bool operator!=(const Residue<N>& first, const Residue<N>& second) {
  return !(first == second);
}
template<size_t N>
bool operator!=(int first, const Residue<N>& second) {
  return !(Residue<N>(first) == second);
}
template<size_t N>
bool operator!=(const Residue<N>& first, int second) {
  return !(first == Residue<N>(second));
}
template<size_t N>
bool operator==(int first, const Residue<N>& second) {
  return (Residue<N>(first) == second);
}
template<size_t N>
bool operator==(const Residue<N>& first, int second) {
  return (first == Residue<N>(second));
}

template<size_t N, typename Field>
class Row {
 public:
  std::vector<Field> row;
  Row() : row(std::vector<Field>(N, Field(0))) {};
  template<typename T>
  explicit Row(const std::vector<T>& vector): row(std::vector<Field>(N)) {
    for (size_t i = 0; i < N; ++i) {
      row[i] = Field(vector[i]);
    }
  };
  template<typename T>
  Row(const std::initializer_list<T>& vector): row(std::vector<Field>(N)) {
    size_t index = 0;
    for (const T& element: vector) {
      row[index] = Field(element);
      ++index;
    }
  };

  explicit Row(const std::vector<Field>& vector) : row(vector) {};
  Row& operator=(const Row<N, Field>& anotherRow) {
    row = anotherRow.row;
    return *this;
  }
  ~Row() = default;

  Field& operator[](size_t index) {
    return row[index];
  };
  const Field& operator[](size_t index) const {
    return row[index];
  };
  Row& operator+=(const Row<N, Field>& anotherRow) {
    for (size_t i = 0; i < N; ++i) {
      row[i] += anotherRow[i];
    }
    return *this;
  }
  Row& operator-=(const Row<N, Field>& anotherRow) {
    for (size_t i = 0; i < N; ++i) {
      row[i] -= anotherRow[i];
    }
    return *this;
  }

  Row& SubtractKTimes(const Row<N, Field>& anotherRow, const Field& k) {
    for (size_t i = 0; i < N; ++i) {
      row[i] -= k * anotherRow[i];
    }
    return *this;
  }

  Row& operator*=(Field value) {
    for (size_t i = 0; i < N; ++i) {
      row[i] *= value;
    }
    return *this;
  }

  friend bool operator==(const Row<N, Field>& first, const Row<N, Field>& second) {
    return first.row == second.row;
  }
  friend bool operator!=(const Row<N, Field>& first, const Row<N, Field>& second) {
    return first.row != second.row;
  }

};

template<size_t M, size_t N, typename Field=Rational>
class Matrix {
 public:
  friend bool operator==(const Matrix<M, N, Field>& first, const Matrix<M, N, Field>& second) {
    return first.matrix == second.matrix;
  }
  friend bool operator!=(const Matrix<M, N, Field>& first, const Matrix<M, N, Field>& second) {
    return !(first == second);
  }
  template<typename = typename std::enable_if<N == M>>
  Matrix(): matrix(M, Row<N, Field>()) {
    for (size_t i = 0; i < M; ++i) {
      matrix[i][i] = ONE;
    }
  }
  template<typename T>
  explicit Matrix<M, N, Field>(const std::vector<std::vector<T, std::allocator<T>>>& inputVector):
      matrix(std::vector<Row<N, Field>>(M)) {
    for (size_t i = 0; i < M; ++i) {
      matrix[i] = Row<N, Field>(inputVector[i]);
    }
  }
  template<typename T>
  Matrix<M, N, Field>(const std::initializer_list<std::initializer_list<T>>& inputVector):
      matrix(std::vector<Row<N, Field>>(M)) {
    size_t index = 0;
    for (const std::initializer_list<T>& element: inputVector) {
      matrix[index] = Row<N, Field>(element);
      ++index;
    }
  }
  Matrix<M, N, Field>(const Matrix<M, N, Field>& anotherMatrix) : matrix(anotherMatrix.matrix) {}
  Matrix<M, N, Field>& operator=(const Matrix<M, N, Field>& anotherMatrix) {
    matrix = anotherMatrix.matrix;
    return *this;
  }
  ~Matrix() = default;
  Row<N, Field>& operator[](size_t index) {
    return matrix[index];
  }
  const Row<N, Field>& operator[](size_t index) const {
    return matrix[index];
  }

  Matrix& operator+=(const Matrix<M, N, Field>& anotherMatrix) {
    for (size_t i = 0; i < M; ++i) {
      matrix[i] += anotherMatrix[i];
    }
    return *this;
  }

  Matrix& operator-=(const Matrix<M, N, Field>& anotherMatrix) {
    for (size_t i = 0; i < M; ++i) {
      matrix[i] -= anotherMatrix[i];
    }
    return *this;
  }
  Matrix& operator*=(const Field& value) {
    for (size_t i = 0; i < M; ++i) {
      for (size_t j = 0; j < N; ++j) {
        matrix[i][j] *= value;
      }
    }
    return *this;
  }
  template<typename = typename std::enable_if<N == M>>
  Matrix& operator*=(const Matrix<N, N, Field>& anotherMatrix) {
    Matrix<M, M, Field> answer;
    for (size_t i = 0; i < M; ++i) {
      for (size_t j = 0; j < M; ++j) {
        Field sum(0);
        for (size_t k = 0; k < M; ++k) {
          sum += matrix[i][k] * anotherMatrix[k][j];
        }
        answer[i][j] = sum;
      }
    }
    *this = answer;
    return *this;
  }
  Matrix<M, N / 2, Field> GetRightPart() {
    std::vector<std::vector<Field>> answer(M, std::vector<Field>(N / 2));
    for (size_t i = 0; i < M; ++i) {
      for (size_t j = N / 2; j < N; ++j) {
        answer[i][j - N / 2] = matrix[i][j];
      }
    }
    return Matrix<M, N / 2, Field>(answer);
  }
  Matrix<M, N, Field> StraightGaussianStroke() const {
    Matrix<M, N, Field> equivalent(*this);
    size_t deltaColIndex = 0;
    for (size_t i = 0; i < M && i + deltaColIndex < N; ++i) {
      if (equivalent[i][i + deltaColIndex] == ZERO) {
        for (; i + deltaColIndex < N; ++deltaColIndex) {
          for (size_t j = i + 1; j < M; ++j) {
            if (equivalent[j][i + deltaColIndex] != Field(0)) {
              std::swap(equivalent[i], equivalent[j]);
              equivalent[i] *= Field(-1);
              break;
            }
          }
          if (equivalent[i][i + deltaColIndex] != ZERO) {
            break;
          }
        }
        if (i + deltaColIndex >= N) {
          break;
        }
      }
      for (size_t j = i + 1; j < M; ++j) {
        if (equivalent[j][i + deltaColIndex] != ZERO) {
          equivalent[j].SubtractKTimes(equivalent[i],
                                       equivalent[j][i + deltaColIndex] / equivalent[i][i + deltaColIndex]);
        }
      }
    }
    return equivalent;
  }
  Matrix<M, N, Field> ReverseGauss() const {
    Matrix<M, N, Field> equivalent(*this);
    for (size_t i = std::min(N, M); i > 0; --i) {
      if (equivalent[i - 1][i - 1] != ZERO) {
        Field reversedElement = ONE / equivalent[i - 1][i - 1];
        equivalent[i - 1] *= reversedElement;
        for (size_t j = i - 1; j > 0; --j) {
          if (equivalent[j - 1][i - 1] != ZERO) {
            Field delta = equivalent[j - 1][i - 1];
            equivalent[j - 1].SubtractKTimes(equivalent[i - 1], delta);
          }
        }
      }
    }
    return equivalent;
  }
  template<typename = typename std::enable_if<N == M>>
  Field det() const {
    Matrix<M, N, Field> equivalent = StraightGaussianStroke();
    Field answer(ONE);
    for (size_t i = 0; i < N; ++i) {
      answer *= equivalent[i][i];
    }
    return answer;
  }
  Field rank() const {
    Matrix<M, N, Field> equivalent = StraightGaussianStroke();
    Field answer(ZERO);
    size_t deltaIndex = 0;
    for (size_t i = 0; i < M && i + deltaIndex < N; ++i) {
      while (i + deltaIndex + 1 < N && equivalent[i][i + deltaIndex] == 0) {
        ++deltaIndex;
      }
      if (equivalent[i][i + deltaIndex] != 0) {
        answer += ONE;
      }
    }
    return answer;
  }
  Matrix<N, M, Field> transposed() const {
    Matrix<N, M, Field> answer;
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        answer[i][j] = matrix[j][i];
      }
    }
    return answer;
  }
  template<typename = typename std::enable_if<N == M>>
  Field trace() const {
    Field answer(ZERO);
    for (size_t i = 0; i < N; ++i) {
      answer += matrix[i][i];
    }
    return answer;
  }
  template<typename = typename std::enable_if<N == M>>
  Matrix<M, N, Field>& invert() {
    Matrix<M, N + N, Field> equivalent = MergeMatrix(*this, Matrix<N, N, Field>());
    matrix = equivalent.StraightGaussianStroke().ReverseGauss().GetRightPart().matrix;
    return *this;
  }
  Matrix<M, N, Field> inverted() const {
    Matrix<M, N, Field> copyMatrix = *this;
    copyMatrix.invert();
    return copyMatrix;
  }
  std::vector<Field> getRow(unsigned long long index) const {
    std::vector<Field> answer(N);
    for (size_t j = 0; j < N; ++j) {
      answer[j] = matrix[index][j];
    }
    return answer;
  }
  std::vector<Field> getColumn(unsigned long long index) const {
    std::vector<Field> answer(M);
    for (size_t i = 0; i < M; ++i) {
      answer[i] = matrix[i][index];
    }
    return answer;
  }

 private:
  std::vector<Row<N, Field>> matrix;
  Field ZERO = Field(0);
  Field ONE = Field(1);
  template<size_t K>
  static Matrix<M, N + K, Field> MergeMatrix(const Matrix<M, N, Field>& first, const Matrix<M, K, Field> second) {
    std::vector<std::vector<Field>> answer(M);
    for (size_t i = 0; i < M; ++i) {
      answer[i].reserve(N + K);
      std::vector<Field> firstRow = first.getRow(i);
      std::vector<Field> secondRow = second.getRow(i);
      answer[i].insert(answer[i].end(), firstRow.begin(), firstRow.end());
      answer[i].insert(answer[i].end(), secondRow.begin(), secondRow.end());
    }
    return Matrix<M, N + K, Field>(answer);
  }
};

template<size_t M, size_t N, size_t K, typename Field = Rational>
Matrix<M, K, Field> operator*(const Matrix<M, N, Field>& first, const Matrix<N, K, Field>& anotherMatrix) {
  Matrix<M, K, Field> answer;
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < K; ++j) {
      Field sum(0);
      for (size_t k = 0; k < N; ++k) {
        sum += first[i][k] * anotherMatrix[k][j];
      }
      answer[i][j] = sum;
    }
  }
  return answer;
}
template<size_t M, size_t N, typename Field = Rational>
Matrix<M, N, Field> operator*(const Matrix<M, N, Field>& first, const Field& second) {
  Matrix<M, N, Field> answer(first);
  answer *= second;
  return answer;
}
template<size_t M, size_t N, typename Field = Rational>
Matrix<M, N, Field> operator*(const Field& first, const Matrix<M, N, Field>& second) {
  Matrix<M, N, Field> answer(second);
  answer *= first;
  return answer;
}
template<size_t M, size_t N, typename Field=Rational>
Matrix<M, N, Field> operator+(const Matrix<M, N, Field>& first, const Matrix<M, N, Field>& second) {
  Matrix<M, N, Field> copy = first;
  copy += second;
  return copy;
}
template<size_t M, size_t N, typename Field=Rational>
Matrix<M, N, Field> operator-(const Matrix<M, N, Field>& first, const Matrix<M, N, Field>& second) {
  Matrix<M, N, Field> copy = first;
  copy -= second;
  return copy;
}

template<size_t N, typename Field=Rational>
using SquareMatrix = Matrix<N, N, Field>;











/*
87 15 -69 -56 58 -85 52 -59 -72 88 60 -48 -43 -80 6 -91 -44 -72 -47 9
-8 10 26 -10 -24 31 -96 58 81 -73 95 15 82 -37 -10 22 -64 -35 -52 -77
-65 36 -53 -32 -42 69 -42 85 -80 -52 25 -59 57 49 -71 21 59 5 39 -91
32 -29 -57 5 -22 -92 -7 85 74 30 64 -64 -71 21 1 -35 -25 -54 -73 -44
-31 38 74 13 -86 35 94 18 67 40 -60 32 -33 83 -45 -95 -41 -1 -65 -26
-97 73 -57 7 86 45 50 52 73 69 74 -90 33 33 96 -58 -1 -21 -66 45
-75 -83 -30 67 30 59 -66 -46 -54 82 -52 60 -31 1 -58 -17 53 -19 87 3
83 51 53 -30 -68 -96 -71 65 90 -55 -22 51 92 50 -61 -14 -18 80 58 41
-4 -54 -48 -51 1 -93 -28 -83 52 -84 7 69 80 81 -98 1 -93 -45 -95 -45
53 -78 16 10 44 -55 76 83 78 -80 47 45 -28 -79 12 42 -100 77 73 81
-53 92 39 -96 -46 44 -68 87 96 48 60 -55 -8 26 -84 65 -23 -44 98 6
18 48 -27 45 14 61 -38 -38 47 -70 -97 50 -17 13 72 54 -3 92 83 -41
91 -9 94 81 -61 -66 -25 84 -57 55 -55 -99 -51 -71 -56 -41 -74 -90 -84 16
-76 -51 -63 83 21 91 -92 -97 23 66 -61 69 96 8 -10 -75 25 -9 -18 44
63 -80 -21 -86 -40 -89 23 -24 -6 10 -67 49 25 18 49 26 68 -48 55 -95
-92 -62 -68 0 59 81 20 81 -86 88 -66 65 -23 70 98 -80 -61 -74 32 -89
29 83 22 55 -75 34 -92 9 69 79 89 77 81 -74 -87 -95 -95 -92 99 29
-98 47 -30 87 -71 -75 -51 -40 -22 50 52 29 -2 -69 -18 -20 -73 -91 17 -86
-89 -24 -81 79 -65 63 -58 -23 22 -25 -31 70 -44 97 -90 5 9 -84 -19 -63
-36 17 -30 51 78 -1 -85 -9 6 97 16 4 25 4 78 2 -62 -87 80 51
 */


/*
pythonInvert

 Matrix([[-605881510885260481662676940519486053455808/15220577867950275295882895328869189436876329,
 -509018288392179987072869989611739891152142/15220577867950275295882895328869189436876329,
 -289516550396146666324688055269569102324193/15220577867950275295882895328869189436876329,
 295408733047433520526985687197970662925086/15220577867950275295882895328869189436876329,
 -572295727911040280182707273927470873747911/15220577867950275295882895328869189436876329,
 175199493207728765607649428881464410394357/15220577867950275295882895328869189436876329,
 4957765563242404103508093201285571021169/15220577867950275295882895328869189436876329,
 -262443419778638323848102952205326922775307/15220577867950275295882895328869189436876329,
 10244091333825084580064391376084949908302/371233606535372568192265739728516815533569,
 -319339096297308093868900405935227876594735/15220577867950275295882895328869189436876329,
 300487884071755319448576623037016616679/15220577867950275295882895328869189436876329,
 157529029308945247148949053711858500841037/15220577867950275295882895328869189436876329, 101680460289722571765969025493556235370888/15220577867950275295882895328869189436876329, -235579465644591843351006629618475649711591/15220577867950275295882895328869189436876329, -100700971098402283352789835595663424793459/15220577867950275295882895328869189436876329, 284667591361559683662940225122318908553653/15220577867950275295882895328869189436876329, 608307167496874082568417997056968815608768/15220577867950275295882895328869189436876329, -87032571481906131501352430561732748206611/15220577867950275295882895328869189436876329, -344544051162260212955444579103600424701895/15220577867950275295882895328869189436876329, -592327544865886608862100155960973642205389/15220577867950275295882895328869189436876329], [-290275896418701456440887400329576329624816/15220577867950275295882895328869189436876329, -532777613722890571866387801630431593944463/30441155735900550591765790657738378873752658, -224699015029050338052041537178074201608570/15220577867950275295882895328869189436876329, 100434995848792302755586026987323760622197/30441155735900550591765790657738378873752658, -370453438148774547107743217874470764622265/15220577867950275295882895328869189436876329, 421377838830275754765973149319509722499901/30441155735900550591765790657738378873752658, 31644414856037990551061133887105986226702/15220577867950275295882895328869189436876329, -58917729096119297843811259639290622765895/15220577867950275295882895328869189436876329, 10606937134999603513968699147175789583409/742467213070745136384531479457033631067138, -401032510310435570637829340990287797066255/30441155735900550591765790657738378873752658, 59383781467863439520128396043727222683081/30441155735900550591765790657738378873752658, 105839324846631037196335161255172704262107/15220577867950275295882895328869189436876329, 165563860150490405510275938372791370312455/30441155735900550591765790657738378873752658, -187247104086601527844135338545654681727905/15220577867950275295882895328869189436876329, -62311408756568318821489844439300659653382/15220577867950275295882895328869189436876329, 366247970996638031807745961504737482419695/30441155735900550591765790657738378873752658, 623327887470588393968258228811631114305211/30441155735900550591765790657738378873752658, 15437510180203823963836723757139971337193/30441155735900550591765790657738378873752658, -228696824425893495640202966811674646872211/30441155735900550591765790657738378873752658, -824595855360748968840555052156200103071663/30441155735900550591765790657738378873752658], [418059076874348671446459878828680684755050/15220577867950275295882895328869189436876329, 854807343147859345386013591577548092222505/30441155735900550591765790657738378873752658, 213470863644434751555440419680924478204317/15220577867950275295882895328869189436876329, -418843782293573907870998986463816392507345/30441155735900550591765790657738378873752658, 485894799760630507330003372236436509682533/15220577867950275295882895328869189436876329, -304962752279425240096345096203598780547985/30441155735900550591765790657738378873752658, 66591537217147423335905543408224609943802/15220577867950275295882895328869189436876329, 171062742259209601129109519441099539902491/15220577867950275295882895328869189436876329, -14457776810473830947568673659513352670393/742467213070745136384531479457033631067138, 389372681873698180324726165374778811910251/30441155735900550591765790657738378873752658, -113128908310741478848073436185317794326815/30441155735900550591765790657738378873752658, -114880095618979023889988022398941515316665/15220577867950275295882895328869189436876329, -146208353987957376172885504095069731348423/30441155735900550591765790657738378873752658, 86125012522427895051316684513977557348823/15220577867950275295882895328869189436876329, 78619445601108620849697506363256416307030/15220577867950275295882895328869189436876329, -497228387343121905647169144992193237280989/30441155735900550591765790657738378873752658, -826626502075248211220904953366279251329849/30441155735900550591765790657738378873752658, 7671188847262932662723334684337355293449/30441155735900550591765790657738378873752658, 399904239994060635840908312497806796344069/30441155735900550591765790657738378873752658, 1011690411525080331634531515195950038273765/30441155735900550591765790657738378873752658], [-506379601552478589792986403697239744515265/15220577867950275295882895328869189436876329, -784255193977198586206296407845566783270161/30441155735900550591765790657738378873752658, -199444200445278577336020052788633785487993/15220577867950275295882895328869189436876329, 430500544937953359731329477105005487075455/30441155735900550591765790657738378873752658, -416925909829740324328571047531366296978403/15220577867950275295882895328869189436876329, 381448290781605856734384832844163182473891/30441155735900550591765790657738378873752658, 63207995989966135415691227138503727062531/15220577867950275295882895328869189436876329, -203120526256871031831682094145566147376849/15220577867950275295882895328869189436876329, 16061291971873572120821234652868209708027/742467213070745136384531479457033631067138, -453103554816238918748708030172496964224463/30441155735900550591765790657738378873752658, -95852997555993612306775807639273089281741/30441155735900550591765790657738378873752658, 124820337722516784964288569569249164501533/15220577867950275295882895328869189436876329, 189986308792391492260804777326733073915889/30441155735900550591765790657738378873752658, -211554472249964656732509072205402317761339/15220577867950275295882895328869189436876329, -79488429021343216202717395688293314394127/15220577867950275295882895328869189436876329, 369965524764185620885752483948244242899067/30441155735900550591765790657738378873752658, 924103110338914128966759077182577791675741/30441155735900550591765790657738378873752658, -45348347875139368673490096372243628738033/30441155735900550591765790657738378873752658, -516285098785509084342705300517876137811003/30441155735900550591765790657738378873752658, -860337883485544392735051177792597536864207/30441155735900550591765790657738378873752658], [-98331903469667925385474206827501925121735/15220577867950275295882895328869189436876329, -46595642376780240972899512646347281717780/15220577867950275295882895328869189436876329, -112233532210848615425227994701582942538296/15220577867950275295882895328869189436876329, -24553453583660976397623724710684640388961/15220577867950275295882895328869189436876329, -163092020294001378646145669315202949126574/15220577867950275295882895328869189436876329, 201771892480548412183868698222201558595565/15220577867950275295882895328869189436876329, 192077072197726761567932710668371453820524/15220577867950275295882895328869189436876329, -8664029079990996466795643273536617627317/15220577867950275295882895328869189436876329, 3653615395184778891649617014781586999762/371233606535372568192265739728516815533569, -83954560394309690729166084793419860466068/15220577867950275295882895328869189436876329, -21600285524426992121914541729323278215963/15220577867950275295882895328869189436876329, 83364539843057350969876321483596117653720/15220577867950275295882895328869189436876329, 71014730499856440681312573682632224456318/15220577867950275295882895328869189436876329, -183060698925947249742884423183838323197520/15220577867950275295882895328869189436876329, -6918836116349343651794596468924046369149/15220577867950275295882895328869189436876329, 63534044456165972776873408605816840704818/15220577867950275295882895328869189436876329, 131143853591011832007699537965028664056959/15220577867950275295882895328869189436876329, -38972687868978514746056468115200877344420/15220577867950275295882895328869189436876329, -59829618581620131652066854524382190036238/15220577867950275295882895328869189436876329, -185436080052798558629000459942547796003929/15220577867950275295882895328869189436876329], [-122742571498729843752365344320761535191384/15220577867950275295882895328869189436876329, -90214787241949486380667647372707546494852/15220577867950275295882895328869189436876329, -33229700669505728781176750702814889231458/15220577867950275295882895328869189436876329, 63753456230126953856491439065087664388426/15220577867950275295882895328869189436876329, -88437963087569949627039082126011941014837/15220577867950275295882895328869189436876329, -7874085997560817230127371596795024552460/15220577867950275295882895328869189436876329, -40744770595317026777686901248914360228340/15220577867950275295882895328869189436876329, -107502816906469345498749801188193294225762/15220577867950275295882895328869189436876329, 1610558455157094685043131467131319192840/371233606535372568192265739728516815533569, -55190742946642290228527447378464370176292/15220577867950275295882895328869189436876329, 17491751171947529952513233388364439705095/15220577867950275295882895328869189436876329, 34866660562399694776514904586922140219973/15220577867950275295882895328869189436876329, 6647386768007859771065112412083715443445/15220577867950275295882895328869189436876329, -2593557640661972887328065544584592992625/15220577867950275295882895328869189436876329, -36735179395331355254643447323477976897987/15220577867950275295882895328869189436876329, 61011732797220615946718719007591361383723/15220577867950275295882895328869189436876329, 132688948056467365765460077995772569016728/15220577867950275295882895328869189436876329, -49947740729148242416738672884042212163305/15220577867950275295882895328869189436876329, -70059470883513091400153606330118409436142/15220577867950275295882895328869189436876329, -104285642345397401177353820980473739248283/15220577867950275295882895328869189436876329], [-472357512231691427554854571036946522335730/15220577867950275295882895328869189436876329, -416215194521231143214516387496155838671455/15220577867950275295882895328869189436876329, -208204746333079035605767930444972047016829/15220577867950275295882895328869189436876329, 157687856750765496379317095479750282233746/15220577867950275295882895328869189436876329, -411673410049260111548370413238341980335441/15220577867950275295882895328869189436876329, 214422473675765124442356874483063125243064/15220577867950275295882895328869189436876329, 19845150700409796035949122506396779007114/15220577867950275295882895328869189436876329, -218911649701177029028006253860053566110211/15220577867950275295882895328869189436876329, 8208108248235226689487532563669459122613/371233606535372568192265739728516815533569, -192902044987444439566685926354354350338487/15220577867950275295882895328869189436876329, 4237530151551981779259926941122863647092/15220577867950275295882895328869189436876329, 104465687650806326443274970346399131945408/15220577867950275295882895328869189436876329, 89413236908645435013119299597717611851452/15220577867950275295882895328869189436876329, -197716916999301788696858181439487382599938/15220577867950275295882895328869189436876329, -37417626855543506140373336045308098720427/15220577867950275295882895328869189436876329, 207577592468182186318435991514587287041980/15220577867950275295882895328869189436876329, 455941666151614917503425091027546001654566/15220577867950275295882895328869189436876329, -22903134841335331305103083757931717109354/15220577867950275295882895328869189436876329, -248087361516092202851754997269178616547383/15220577867950275295882895328869189436876329, -487225901138459932835668226389988659811251/15220577867950275295882895328869189436876329], [-421635327624550364029558662425467693819216/15220577867950275295882895328869189436876329, -669004701909317592696338251375180924165143/30441155735900550591765790657738378873752658, -253969786004335747018847157593658457503866/15220577867950275295882895328869189436876329, 274076469053303742017893471204494489824725/30441155735900550591765790657738378873752658, -473296540902569154268091849159969578108755/15220577867950275295882895328869189436876329, 438999983879591167381818146128608182007817/30441155735900550591765790657738378873752658, 33084871428071374184724872101504648426688/15220577867950275295882895328869189436876329, -96700869857136450238754561848946662025377/15220577867950275295882895328869189436876329, 12881590346480408903612932296876847069847/742467213070745136384531479457033631067138, -400492188892002913891429815630279966025035/30441155735900550591765790657738378873752658, 75780585405061724063320868168069739902927/30441155735900550591765790657738378873752658, 78666169321779688841930756343127222240102/15220577867950275295882895328869189436876329, 224721002185373348134001940142722198746907/30441155735900550591765790657738378873752658, -179869015366791596840540902595199005647794/15220577867950275295882895328869189436876329, -65753984537390705837669108925659061338402/15220577867950275295882895328869189436876329, 531420741294614916371123487000763499992027/30441155735900550591765790657738378873752658, 773886935737669120482219579792171102179839/30441155735900550591765790657738378873752658, -30094189919057625949094436596066238168293/30441155735900550591765790657738378873752658, -355560595432018802484287374124281222404253/30441155735900550591765790657738378873752658, -1037552720701449748289647474651745032564957/30441155735900550591765790657738378873752658], [164276713620678436089945166139721300947497/15220577867950275295882895328869189436876329, 159289694103344807830776433231160159493629/15220577867950275295882895328869189436876329, 62051780096267534184966130544042649370982/15220577867950275295882895328869189436876329, -58805508366132282197161717346460230982893/15220577867950275295882895328869189436876329, 173169565543611037422421468075349427583723/15220577867950275295882895328869189436876329, 6563329329572287593127463236035334958239/15220577867950275295882895328869189436876329, 41951358757521976673978505696920689624930/15220577867950275295882895328869189436876329, 57374480120111477288043936103633423949981/15220577867950275295882895328869189436876329, -2348707483647705709297232710099118229193/371233606535372568192265739728516815533569, 99668521714329990721711318650913677672406/15220577867950275295882895328869189436876329, 9644983549574387573355471411943223324608/15220577867950275295882895328869189436876329, 6189387712581478212100289569162580978087/15220577867950275295882895328869189436876329, -12163486263576586655011056008066408878520/15220577867950275295882895328869189436876329, 56661998414444329145113868442842517368877/15220577867950275295882895328869189436876329, 70549791417415756597539939411664079638281/15220577867950275295882895328869189436876329, -112674880866419457236228465512406292608370/15220577867950275295882895328869189436876329, -141651865987785714756360480026025672024986/15220577867950275295882895328869189436876329, -3303486779950458215306217425917727984897/15220577867950275295882895328869189436876329, 93584838045791475773136310340646083678857/15220577867950275295882895328869189436876329, 146267036879124791880085661991074549140450/15220577867950275295882895328869189436876329], [-1429663890896117877393969404672232105606439/15220577867950275295882895328869189436876329, -2395797979000990612544818929714331939257677/30441155735900550591765790657738378873752658, -729206784076441197896790639849194013844159/15220577867950275295882895328869189436876329, 1281720572474592741131986787788634028503663/30441155735900550591765790657738378873752658, -1338820075553056361027308350191566925591225/15220577867950275295882895328869189436876329, 899567428788042126324804858549103500162405/30441155735900550591765790657738378873752658, 39973165843996336448597287769573812103047/15220577867950275295882895328869189436876329, -567743399634191876311642291341432385889018/15220577867950275295882895328869189436876329, 46824054064052135798536514632520450481497/742467213070745136384531479457033631067138, -1452124835403392722535722528271767743371913/30441155735900550591765790657738378873752658, 220538441862928223043565334154210555358869/30441155735900550591765790657738378873752658, 297956729681815619424085293133908469997133/15220577867950275295882895328869189436876329, 441514016835362097730717417432357536943125/30441155735900550591765790657738378873752658, -465678658875100207787834946099223461211097/15220577867950275295882895328869189436876329, -258825147349780259425828431049586641847937/15220577867950275295882895328869189436876329, 1432786885842099218670610366203250688883909/30441155735900550591765790657738378873752658, 2606346827930070933653012735322572110864567/30441155735900550591765790657738378873752658, -93618744324290180120122914693644886622433/30441155735900550591765790657738378873752658, -1664505363658982614513503809655591066476657/30441155735900550591765790657738378873752658, -2901316178194518475675255317290348339091329/30441155735900550591765790657738378873752658], [-189689617532074439984527005744026532197762/15220577867950275295882895328869189436876329, -246589049514147932209278244511838250602355/30441155735900550591765790657738378873752658, -47158168417742132869737635214085339695131/15220577867950275295882895328869189436876329, 291538477407013230586312454894679522678753/30441155735900550591765790657738378873752658, -116247173660753703942609483256569014147537/15220577867950275295882895328869189436876329, -44222210303028374846604366710427104569917/30441155735900550591765790657738378873752658, -12172696874333270202983412771264237532925/15220577867950275295882895328869189436876329, -103765622610324251969659841662228277932679/15220577867950275295882895328869189436876329, 4732394122385650624222073403308878025309/742467213070745136384531479457033631067138, -189059082366642344562506645325877039115659/30441155735900550591765790657738378873752658, -70096487842375144301382624665873595542635/30441155735900550591765790657738378873752658, -16090123132134429507986295542574029935964/15220577867950275295882895328869189436876329, -105963350181309304372500119658864912412417/30441155735900550591765790657738378873752658, -77674430978760073249077984247833040028513/15220577867950275295882895328869189436876329, -71311151030412240466470810443275607704029/15220577867950275295882895328869189436876329, 107398492039579646225687058980737428754575/30441155735900550591765790657738378873752658, 354442790816091036695746656195730722565839/30441155735900550591765790657738378873752658, -39716663294582711125783851986127696924563/30441155735900550591765790657738378873752658, -260304374079730505363021134115943614398059/30441155735900550591765790657738378873752658, -180556292864544280611736925380919272446049/30441155735900550591765790657738378873752658], [-712013564054880081627279262579264735851534/15220577867950275295882895328869189436876329, -1145656742118117469679924150431557784194465/30441155735900550591765790657738378873752658, -488730312932049572812472356615522225936696/15220577867950275295882895328869189436876329, 450948184148310606537540370233618499670127/30441155735900550591765790657738378873752658, -778310219923907910435024599998486326743983/15220577867950275295882895328869189436876329, 600152613775051139276680807685429040201599/30441155735900550591765790657738378873752658, 43626988419000393013133769031829632763823/15220577867950275295882895328869189436876329, -199705416135929145007117992366917395732748/15220577867950275295882895328869189436876329, 22879343260623203309904565984042225142329/742467213070745136384531479457033631067138, -769963342860857288104463899285193832231227/30441155735900550591765790657738378873752658, 85388838408014091789364221082949829257897/30441155735900550591765790657738378873752658, 129035747752532940427768161291850087708204/15220577867950275295882895328869189436876329, 226671360572039532386288190805026849429223/30441155735900550591765790657738378873752658, -309504489266225235094105757793341281988310/15220577867950275295882895328869189436876329, -131939868725888436211719795920096577932530/15220577867950275295882895328869189436876329, 860107787099854940350478354982862596511107/30441155735900550591765790657738378873752658, 1356477552471636956839478994335315934522759/30441155735900550591765790657738378873752658, -29288371154454261755832122812173145576115/30441155735900550591765790657738378873752658, -620771342778373235177212661647444220700369/30441155735900550591765790657738378873752658, -1707803795831737145582674654001877694558075/30441155735900550591765790657738378873752658], [-560466205971460429836382471214876355758653/15220577867950275295882895328869189436876329, -448851179314065513142575301859179945093348/15220577867950275295882895328869189436876329, -217189673548145117052413404344670628483370/15220577867950275295882895328869189436876329, 199561509384820651830354403342199364923193/15220577867950275295882895328869189436876329, -498994547109504509731524755394123289537225/15220577867950275295882895328869189436876329, 212192496886685786043173334117202027670961/15220577867950275295882895328869189436876329, 1540911495658282267460206529959477458808/15220577867950275295882895328869189436876329, -221320116695208526991318323868750206649315/15220577867950275295882895328869189436876329, 9608886137214166785572524777320794050575/371233606535372568192265739728516815533569, -251838231663197829471159115834630892058380/15220577867950275295882895328869189436876329, 8635913076939409303043601183666297801782/15220577867950275295882895328869189436876329, 110501583450134860763716318583838308225572/15220577867950275295882895328869189436876329, 106223245841013405111190992424443514769337/15220577867950275295882895328869189436876329, -163864459268542629160124424713913608331717/15220577867950275295882895328869189436876329, -54230574965722913733551809364482365848077/15220577867950275295882895328869189436876329, 245544006367161219752627615587959021607312/15220577867950275295882895328869189436876329, 513338709392302448468203956777302865498948/15220577867950275295882895328869189436876329, -24435242710429025819049458810928871874575/15220577867950275295882895328869189436876329, -329207686009899442165852848752364039825864/15220577867950275295882895328869189436876329, -526749758471594997366046940878103337257001/15220577867950275295882895328869189436876329], [-145623365813769643761085367592648587790539/15220577867950275295882895328869189436876329, -234257506472225831167360753475178840075593/30441155735900550591765790657738378873752658, 4285703515784360398490380823070493794157/15220577867950275295882895328869189436876329, 231570003177546943697876692843975810331549/30441155735900550591765790657738378873752658, -27027653580781711827396183073786131278408/15220577867950275295882895328869189436876329, -74776303106964218662451075821574398294577/30441155735900550591765790657738378873752658, -20706480979215276139883889615869747420909/15220577867950275295882895328869189436876329, -63650511752486266059324511107447742554647/15220577867950275295882895328869189436876329, 4562770556712784483179560157664837486787/742467213070745136384531479457033631067138, -157122394439869232616557157248662272347489/30441155735900550591765790657738378873752658, -72226751844505286013479520067756473789683/30441155735900550591765790657738378873752658, 1371619395947296880921548559724373917387/15220577867950275295882895328869189436876329, -66553181982723574079336423421773361491685/30441155735900550591765790657738378873752658, -58080528000505764529766447062506430043751/15220577867950275295882895328869189436876329, -48676339071913330842715611972923194433331/15220577867950275295882895328869189436876329, 60527104283128534144133226581055655235295/30441155735900550591765790657738378873752658, 206052642198004168460955398577698172339797/30441155735900550591765790657738378873752658, -93800455630620921102785866368583630502207/30441155735900550591765790657738378873752658, -173453515109510495945710890857705647218351/30441155735900550591765790657738378873752658, 37078709233151079864150954541379796530919/30441155735900550591765790657738378873752658], [404001698327530312704860234710350817056185/15220577867950275295882895328869189436876329, 351872611276375757126477984674446048135790/15220577867950275295882895328869189436876329, 211027667988040783811052573755312447588772/15220577867950275295882895328869189436876329, -140730236753157939107247727780350881391797/15220577867950275295882895328869189436876329, 418157182485158467472453338265840833591456/15220577867950275295882895328869189436876329, -211135349009148675617801991487241847770100/15220577867950275295882895328869189436876329, -111934813263481230181055003070852730870488/15220577867950275295882895328869189436876329, 151517619263149951910584586454213030203787/15220577867950275295882895328869189436876329, -8238355590905643665265831338326231958653/371233606535372568192265739728516815533569, 204494817791353423672537850710592317473611/15220577867950275295882895328869189436876329, -36462996542320948991080746022697294155388/15220577867950275295882895328869189436876329, -113721458057828302923103147053742995965735/15220577867950275295882895328869189436876329, -109655284079046571737857938411207257673308/15220577867950275295882895328869189436876329, 197543105888950892293805993121220537290248/15220577867950275295882895328869189436876329, 71756064504388670748002805028341540183124/15220577867950275295882895328869189436876329, -195050629106270319302195152933974945672278/15220577867950275295882895328869189436876329, -409915969119938318885503378622460108258859/15220577867950275295882895328869189436876329, 28200327377339148241479064086836502710182/15220577867950275295882895328869189436876329, 219645696495537120883000773874090027678622/15220577867950275295882895328869189436876329, 498608876616734155021632518096643903563522/15220577867950275295882895328869189436876329], [-1492528140927642996298019615806383996629060/15220577867950275295882895328869189436876329, -1228293191279413667182417578600653837488632/15220577867950275295882895328869189436876329, -763851053346522600135497900375718412614056/15220577867950275295882895328869189436876329, 592564676125684538023749339030047131311382/15220577867950275295882895328869189436876329, -1436102825148584958331386648315170875644101/15220577867950275295882895328869189436876329, 488866101661326672935437582855960431421642/15220577867950275295882895328869189436876329, -310841480518188264360341742900628579028/15220577867950275295882895328869189436876329, -593207085323030689744719061560074986995013/15220577867950275295882895328869189436876329, 24376356813203346512606088293312493884211/371233606535372568192265739728516815533569, -723751022340337729576859680304044814272048/15220577867950275295882895328869189436876329, 118648050395927626949068629496113748102993/15220577867950275295882895328869189436876329, 311681450309701901969547902733026701221562/15220577867950275295882895328869189436876329, 253033837796097445723410451685325174662508/15220577867950275295882895328869189436876329, -514216862189934188276695157132641198017078/15220577867950275295882895328869189436876329, -250544101912006125666178243664957476066455/15220577867950275295882895328869189436876329, 724771923674002341888374099701251187954868/15220577867950275295882895328869189436876329, 1327440193896551838268605015903813798981523/15220577867950275295882895328869189436876329, -49589025503942836791042642998387286073249/15220577867950275295882895328869189436876329, -780770152346180057215983055720595969969035/15220577867950275295882895328869189436876329, -1502006169373335594226281199585755512597049/15220577867950275295882895328869189436876329], [116452537109973466917514675717004212137224/15220577867950275295882895328869189436876329, 244910107801000328708537619479785183076679/30441155735900550591765790657738378873752658, 8293058962654898677428752659842613113992/15220577867950275295882895328869189436876329, -191696885881529100577565596758988701747435/30441155735900550591765790657738378873752658, 54448838507127264669645856970356110101977/15220577867950275295882895328869189436876329, 128999740375337253209753321398131323339263/30441155735900550591765790657738378873752658, 82005809960443497111239982496024573881776/15220577867950275295882895328869189436876329, 95963950475318944996305141918632965857160/15220577867950275295882895328869189436876329, -5201122514286922683362795481016510873509/742467213070745136384531479457033631067138, 48859066734028229259409817452817045418629/30441155735900550591765790657738378873752658, -62157189000482015735694705191876193598163/30441155735900550591765790657738378873752658, -37395648018684350225355436302451993612078/15220577867950275295882895328869189436876329, -9281273349466941305615838744751044692483/30441155735900550591765790657738378873752658, -23666101395926603195631064879836350544034/15220577867950275295882895328869189436876329, 67770391927462142978468266543752458286938/15220577867950275295882895328869189436876329, -141984785295304279586749643683790271167407/30441155735900550591765790657738378873752658, -187390821258614868647106278016602911217087/30441155735900550591765790657738378873752658, -14801592831365413545790759383880005611693/30441155735900550591765790657738378873752658, 243027656915479885212352828619781600298689/30441155735900550591765790657738378873752658, 82308010079539886471723859852541906464675/30441155735900550591765790657738378873752658], [-582049070506638670578265097446302751947021/15220577867950275295882895328869189436876329, -983009040581155507735707542365994747952229/30441155735900550591765790657738378873752658, -256493182854917428265071320792479297722633/15220577867950275295882895328869189436876329, 638737757907979449861030520671394183662389/30441155735900550591765790657738378873752658, -509128113511764752956301304344298584057318/15220577867950275295882895328869189436876329, 222265679114922110239398547442858306886237/30441155735900550591765790657738378873752658, 1262462685799147280812982565882319447949/15220577867950275295882895328869189436876329, -223116131597080453545493079214208324850652/15220577867950275295882895328869189436876329, 18631535224150569166626474081011356000583/742467213070745136384531479457033631067138, -570399031300501350346706278640277443680443/30441155735900550591765790657738378873752658, 74285077414999146722308468231367857802705/30441155735900550591765790657738378873752658, 136783706835629436127027436630946718573301/15220577867950275295882895328869189436876329, 89483950405225711644841142272980571592505/30441155735900550591765790657738378873752658, -144153971715823828429943096643940422340479/15220577867950275295882895328869189436876329, -163888133524721188631802142868884283157400/15220577867950275295882895328869189436876329, 577318173692307319060905421630290984742469/30441155735900550591765790657738378873752658, 995457593857124036204357286499231312196017/30441155735900550591765790657738378873752658, -2584316901351034869216890266279720765157/30441155735900550591765790657738378873752658, -808633853158025024349823113101340052910621/30441155735900550591765790657738378873752658, -1160211736199645825907531421338780050907061/30441155735900550591765790657738378873752658], [667372154634519842320831370539494648546944/15220577867950275295882895328869189436876329, 536036526486217047532134469775985322699226/15220577867950275295882895328869189436876329, 414950032555716046432521887041935498185356/15220577867950275295882895328869189436876329, -261141924935198894927818457518293685505177/15220577867950275295882895328869189436876329, 698482601543494005114703660095834733084358/15220577867950275295882895328869189436876329, -252132550414791927282189321667351420387259/15220577867950275295882895328869189436876329, 8758590919849118688052405846441834429272/15220577867950275295882895328869189436876329, 232157446415000059740747761328182845002455/15220577867950275295882895328869189436876329, -10986248845037272087170783249559531403651/371233606535372568192265739728516815533569, 366544613342420975170511642855588196415918/15220577867950275295882895328869189436876329, -54430914204331695054437452252618253838373/15220577867950275295882895328869189436876329, -124267568787652253946626761356325913400133/15220577867950275295882895328869189436876329, -129636872874947510340577229060068248853360/15220577867950275295882895328869189436876329, 221848200854237152317774777678580197338577/15220577867950275295882895328869189436876329, 144149348915416489422988332583762520075301/15220577867950275295882895328869189436876329, -372630982076415464935998647031430497718459/15220577867950275295882895328869189436876329, -594104057290910716395164252703798764962701/15220577867950275295882895328869189436876329, 18216530956426752243932541854891898074167/15220577867950275295882895328869189436876329, 332907742809087653727755402096112262373759/15220577867950275295882895328869189436876329, 772236315735523627144616660418633668505862/15220577867950275295882895328869189436876329], [351490994828173497309389539913325478972792/15220577867950275295882895328869189436876329, 483655153221225931497734859445337334395433/30441155735900550591765790657738378873752658, 142441654216978313231967883384160908952748/15220577867950275295882895328869189436876329, -346470331858300225667304124050687704265149/30441155735900550591765790657738378873752658, 289366231882521018179378303102825551977727/15220577867950275295882895328869189436876329, -292618967484363984494058606885775745021503/30441155735900550591765790657738378873752658, -107117539457209316130945577356796517714811/15220577867950275295882895328869189436876329, 165545213675244184177755095294130501497793/15220577867950275295882895328869189436876329, -13465896339359213931808674751774300210989/742467213070745136384531479457033631067138, 390977637229627475076821018374798026493177/30441155735900550591765790657738378873752658, 19108796042379448834110344233910601908183/30441155735900550591765790657738378873752658, -137571560281764203906045081365987068518425/15220577867950275295882895328869189436876329, -123585776052951570633492622442144859522003/30441155735900550591765790657738378873752658, 184919064308759024599703679532208073014442/15220577867950275295882895328869189436876329, 35192436076637207113656673936270938681169/15220577867950275295882895328869189436876329, -313068735338862185168221171753979262966891/30441155735900550591765790657738378873752658, -675845310018579016809314334624752854698137/30441155735900550591765790657738378873752658, 20426502538120249539754320967219770437393/30441155735900550591765790657738378873752658, 503384016786560758315548371288320515372405/30441155735900550591765790657738378873752658, 725615387356589851886135522219589532829311/30441155735900550591765790657738378873752658]])

 */