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
    if (isZero()) {
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
    if (integer.size() != anotherInteger.size()){
      return (integer.size() < anotherInteger.size())? less : more;
    }
    for (size_t i = integer.size(); i > 0; --i) {
      if (integer[i - 1] != anotherInteger[i - 1]){
        return (integer[i - 1] < anotherInteger[i - 1])? less : more;
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
    return (integer.size() <= 1 && integer[0] == 0);
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
  if (first.isZero() && second.isZero()) {
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
  if (bigInteger.isZero()){
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
  Rational(int intNumerator) : numerator(std::abs(intNumerator)), denominator(1), sign(intNumerator < 0 ? -1 : 1) {}
  Rational(const BigInteger& bigNumerator) : numerator(bigNumerator), denominator(1),
                                             sign(bigNumerator.getSign()) {
    if (numerator.getSign() == -1) {
      numerator.changeSign();
    }
  }
  Rational& operator+=(const Rational& anotherRational) {
    (anotherRational.getSign() != getSign()) ? subtract(anotherRational) : add(anotherRational);
    return *this;
  }
  Rational& operator-=(const Rational& anotherRational) {
    (anotherRational.getSign() == getSign()) ? subtract(anotherRational) : add(anotherRational);
    return *this;
  }
  Rational& operator*=(const Rational& anotherRational) {
    sign *= anotherRational.getSign();
    numerator *= anotherRational.numerator;
    denominator *= anotherRational.denominator;
    reduceFraction();
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
      reduceFraction();
    }
    return *this;
  }
  Rational operator-() const{
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
  std::string asDecimal(size_t precision = 0){
    std::string answer;
    BigInteger numeratorCopy = numerator;
    BigInteger remainder = numeratorCopy % denominator;
    numeratorCopy /= denominator;
    std::string signDecimal = (getSign() == -1 && (!numeratorCopy.isZero() || !remainder.isZero()))? "-" : "";
    if (precision == 0){
      answer = numeratorCopy.toString();
    }
    else if (precision > 0){
      size_t differenceInBlocks = denominator.integer.size() - remainder.size();
      int  digitsAmountDenominator = getPowerOfTen(denominator.integer[denominator.integer.size() - 1]);
      int digitsAmountRemainder = getPowerOfTen(remainder.integer[remainder.integer.size() - 1]);
      int tenPowerRemainder = tenToPower(digitsAmountRemainder);
      int tenPowerDenominator = tenToPower(digitsAmountDenominator);
      if (digitsAmountDenominator < digitsAmountRemainder){
        digitsAmountDenominator += denominator.amountInBlock;
        --differenceInBlocks;
        tenPowerDenominator *= denominator.base / tenPowerRemainder;
      }
      else{
        tenPowerDenominator /= tenPowerRemainder;
      }
      moveDigits(remainder, differenceInBlocks, tenPowerDenominator);
      size_t countLeadingZeros = differenceInBlocks * denominator.amountInBlock + digitsAmountDenominator - digitsAmountRemainder - 1;
      int numberToMultiply = 1;
      if (denominator > remainder){
        numberToMultiply *= 10;
        ++countLeadingZeros;
      }
      if (precision < countLeadingZeros){
        answer = signDecimal+ numeratorCopy.toString() +
            "." + std::string(precision, '0');
      }
      else{
        precision = precision + 1 - countLeadingZeros;
        size_t blocksNumber = (precision) / remainder.amountInBlock;
        int power = static_cast<int>((precision) % remainder.amountInBlock);
        if (power + 1 == remainder.amountInBlock && numberToMultiply == 10){
          numberToMultiply = 1;
          ++blocksNumber;
        }
        else{
          while (power != 0){
            numberToMultiply *= 10;
            --power;
          }
        }
        moveDigits(remainder, blocksNumber, numberToMultiply);
        remainder /= denominator;
        int firstBeforeRounding = remainder.integer[remainder.integer.size() - 1];
        size_t blocksBeforeRounding = remainder.integer.size();
        if (remainder[0] >= 5){
          remainder += 10;
        }
        if (remainder.size() != blocksBeforeRounding ||
            getPowerOfTen(firstBeforeRounding) < getPowerOfTen(
                remainder.integer[remainder.integer.size() - 1])) {
          if (countLeadingZeros != 0){
            ++precision;
            --countLeadingZeros;
          }
          else{
            numeratorCopy += 1;
          }
        }

        answer = signDecimal +
            numeratorCopy.toString() + "." + std::string(countLeadingZeros, '0');
        if (precision != 0){
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
  static BigInteger countGcd(const BigInteger& first, const BigInteger& second) {
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
  void reduceFraction() {
    BigInteger gcd = countGcd(numerator, denominator);
    numerator /= gcd;
    denominator /= gcd;
  }
  void add(const Rational& anotherRational) {
    if (this->denominator == anotherRational.denominator) {
      this->numerator += anotherRational.numerator;
    } else {
      if (numerator.isZero()){
        sign = 1;
      }
      numerator *= anotherRational.denominator;
      numerator += anotherRational.numerator * denominator;
      denominator *= anotherRational.denominator;
    }
    reduceFraction();
  }
  void subtract(const Rational& anotherRational) {
    if (this->numerator == anotherRational.numerator && this->denominator == anotherRational.denominator) {
      this->numerator = 0;
      this->denominator = 1;
      return;
    }
    if (numerator.isZero()){
      sign = 1;
    }
    numerator *= anotherRational.denominator;
    numerator -= anotherRational.numerator * denominator;
    if (numerator.getSign() == -1) {
      sign *= -1;
      numerator.changeSign();
    }
    denominator *= anotherRational.denominator;
    reduceFraction();
  }
  static void moveDigits(BigInteger& bigInteger, size_t blocksNumber, int numberToMultiply){
    std::vector<int> newInteger(blocksNumber + bigInteger.integer.size(), 0);
    int debt = 0;
    for (size_t i = 0; i < bigInteger.integer.size(); ++i){
      debt += (bigInteger.integer[i] % (bigInteger.base / numberToMultiply)) * numberToMultiply;
      newInteger[i + blocksNumber] = debt % bigInteger.base;
      debt /= bigInteger.base ;
      debt += bigInteger.integer[i] / (bigInteger.base / numberToMultiply);
    }
    if (debt != 0){
      newInteger.push_back(debt);
    }
    bigInteger.integer = newInteger;
  }
  static int tenToPower(int power){
    int number = 1;
    while (power != 0){
      number *= 10;
      --power;
    }
    return number;
  }
  static int getPowerOfTen(int numberBlock) {
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
    return *this *= getReverse(another);
  }

  Residue<N> operator-() const {
    return Residue<N>((N - value) % N);
  }

  static Residue<N> getReverse(const Residue<N>& residue) {
    return (N <= 2) ? residue : binaryPower(residue, N - 2);
  }

  friend bool operator==(const Residue<N>& first, const Residue<N>& second) {
    return first.value == second.value;
  }
  [[nodiscard]] std::string toString() const {
    std::stringstream answerStream;
    answerStream << value;
    return answerStream.str();
  }
 private:
  static Residue<N> binaryPower(const Residue<N>& base, unsigned long long p) {
    if (p == 1) {
      return base;
    }
    if (p % 2 == 0) {
      Residue<N> power = binaryPower(base, p / 2);
      power *= power;
      return power;
    } else {
      Residue<N> power = binaryPower(base, p - 1);
      power *= base;
      return power;
    }
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
  Row& subtractKTimes(const Row<N, Field>& anotherRow, const Field& k) {
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
  Matrix<M, N / 2, Field> getRightPart() {
    std::vector<std::vector<Field>> answer(M, std::vector<Field>(N / 2));
    for (size_t i = 0; i < M; ++i) {
      for (size_t j = N / 2; j < N; ++j) {
        answer[i][j - N / 2] = matrix[i][j];
      }
    }
    return Matrix<M, N / 2, Field>(answer);
  }
  Matrix<M, N, Field> straightGaussianStroke() const {
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
          equivalent[j].subtractKTimes(equivalent[i],
                                       equivalent[j][i + deltaColIndex] / equivalent[i][i + deltaColIndex]);
        }
      }
    }
    return equivalent;
  }
  Matrix<M, N, Field> reverseGauss() const {
    Matrix<M, N, Field> equivalent(*this);
    for (size_t i = std::min(N, M); i > 0; --i) {
      if (equivalent[i - 1][i - 1] != ZERO) {
        Field reversedElement = ONE / equivalent[i - 1][i - 1];
        equivalent[i - 1] *= reversedElement;
        for (size_t j = i - 1; j > 0; --j) {
          if (equivalent[j - 1][i - 1] != ZERO) {
            Field delta = equivalent[j - 1][i - 1];
            equivalent[j - 1].subtractKTimes(equivalent[i - 1], delta);
          }
        }
      }
    }
    return equivalent;
  }
  template<typename = typename std::enable_if<N == M>>
  Field det() const {
    Matrix<M, N, Field> equivalent = straightGaussianStroke();
    Field answer(ONE);
    for (size_t i = 0; i < N; ++i) {
      answer *= equivalent[i][i];
    }
    return answer;
  }
  Field rank() const {
    Matrix<M, N, Field> equivalent = straightGaussianStroke();
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
    Matrix<M, N + N, Field> equivalent = mergeMatrix(*this, Matrix<N, N, Field>());
    matrix = equivalent.straightGaussianStroke().reverseGauss().getRightPart().matrix;
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
  static Matrix<M, N + K, Field> mergeMatrix(const Matrix<M, N, Field>& first, const Matrix<M, K, Field> second) {
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

