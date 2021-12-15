#include <iostream>
#include <vector>
#include <string>
#include <sstream>
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
      int  digitsAmountDenominator = GetPowerOfTen(denominator.integer[denominator.integer.size() - 1]);
      int digitsAmountRemainder = GetPowerOfTen(remainder.integer[remainder.integer.size() - 1]);
      int tenPowerRemainder = TenToPower(digitsAmountRemainder);
      int tenPowerDenominator = TenToPower(digitsAmountDenominator);
      if (digitsAmountDenominator < digitsAmountRemainder){
        digitsAmountDenominator += denominator.amountInBlock;
        --differenceInBlocks;
        tenPowerDenominator *= denominator.base / tenPowerRemainder;
      }
      else{
        tenPowerDenominator /= tenPowerRemainder;
      }
      MoveDigits(remainder, differenceInBlocks, tenPowerDenominator);
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
        MoveDigits(remainder, blocksNumber, numberToMultiply);
        remainder /= denominator;
        int firstBeforeRounding = remainder.integer[remainder.integer.size() - 1];
        size_t blocksBeforeRounding = remainder.integer.size();
        if (remainder[0] >= 5){
          remainder += 10;
        }
        if (remainder.size() != blocksBeforeRounding ||
            GetPowerOfTen(firstBeforeRounding) < GetPowerOfTen(
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
      if (numerator.isZero()){
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
    ReduceFraction();
  }
  static void MoveDigits(BigInteger& bigInteger, size_t blocksNumber, int numberToMultiply){
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
  static int TenToPower(int power){
    int number = 1;
    while (power != 0){
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


