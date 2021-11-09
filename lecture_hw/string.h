#include <iostream>
#include <cstring>

class String {
 private:
  const size_t MIN_CAPACITY = 100;
  size_t capacity;
  char* charArr;
  size_t stringLength;
  void swap(String& anotherString) {
    char* tmp = charArr;
    charArr = anotherString.charArr;
    anotherString.charArr = tmp;
    size_t tmpLength = stringLength;
    stringLength = anotherString.stringLength;
    anotherString.stringLength = tmpLength;
    size_t tmpCapacity = capacity;
    capacity = anotherString.capacity;
    anotherString.capacity = tmpCapacity;
  }

  void resize(size_t newCapacity) {
    capacity = newCapacity;
    String copyString(*this);
    swap(copyString);
  }

  [[nodiscard]] size_t commonFind(const String& anotherString, bool reverse) const {
    if (anotherString.length() > stringLength) {
      return stringLength;
    }
    for (size_t i = 0; i < stringLength - anotherString.length() + 1; ++i) {
      bool isEqual = true;
      for (size_t j = 0; j < anotherString.length(); ++j) {
        if ((!reverse && anotherString[j] != charArr[i + j]) ||
            (reverse && anotherString[anotherString.length() - 1 - j] != charArr[stringLength - 1 - i - j])) {
          isEqual = false;
          break;
        }
      }
      if (isEqual) {
        return i;
      }
    }
    return stringLength;
  }

 public:
  String(const char* charArray) : capacity((strlen(charArray) != 0)? strlen(charArray) * 2 : MIN_CAPACITY), charArr(new char[capacity]), stringLength(strlen(charArray)){
    for (size_t i = 0; i < stringLength; ++i) {
      charArr[i] = charArray[i];
    }
  }
  explicit String(size_t n, char c) : capacity((n != 0)? n * 2 : MIN_CAPACITY), charArr(new char[capacity]), stringLength(n) {
    for (size_t i = 0; i < n; ++i) {
      charArr[i] = c;
    }
  }
  explicit String() : capacity(MIN_CAPACITY), charArr(new char[capacity]), stringLength(0) {}
  String(const String& anotherString) : capacity(anotherString.capacity != 0? anotherString.capacity : MIN_CAPACITY), charArr(new char[capacity]), stringLength(anotherString.length()){
    for (size_t i = 0; i < stringLength; ++i) {
      charArr[i] = anotherString[i];
    }
  }
  ~String() {
    delete[] charArr;
  }

  const char& operator[](const size_t index) const {
    return charArr[index];
  }
  char& operator[](size_t index) {
    return charArr[index];
  }

  void push_back(char element) {
    if (stringLength + 1 >= capacity) {
      resize(capacity * 2);
    }
    charArr[stringLength] = element;
    ++stringLength;
  }
  void pop_back() {
    --stringLength;
  }
  void clear() {
    stringLength = 0;
  }

  [[nodiscard]] size_t length() const {
    return stringLength;
  }
  [[nodiscard]] bool empty() const {
    return stringLength == 0;
  }

  char& front() {
    return charArr[0];
  }
  [[nodiscard]] const char& front() const {
    return charArr[0];
  }
  char& back() {
    return charArr[stringLength - 1];
  }
  [[nodiscard]] const char& back() const {
    return charArr[stringLength - 1];
  }
  [[nodiscard]] size_t find(const String& anotherString) const {
    return commonFind(anotherString, false);
  }
  [[nodiscard]] size_t rfind(const String& anotherString) const {
    return commonFind(anotherString, true);
  }
  [[nodiscard]] String substr(size_t start, int count) const {
    String subString(count, 'o');
    for (size_t i = start; i < start + count; ++i) {
      subString[i] = charArr[i];
    }
    return subString;
  }

  String& operator=(const String& anotherString) {
    String copyString(anotherString);
    swap(copyString);
    return *this;
  }


  String& operator+=(char element) {
    push_back(element);
    return *this;
  }
  String& operator+=(const String& anotherString) {
    if (stringLength + anotherString.length() >= capacity) {
      resize((stringLength + anotherString.length()) * 2);
    }
    for (size_t i = 0; i < anotherString.length(); ++i) {
      push_back(anotherString[i]);
    }
    return *this;
  }

  friend std::istream& operator>>(std::istream& in, String& string);
  friend std::ostream& operator<<(std::ostream& out, const String& string);

};

String operator+(const String& string, const char element) {
  String copyString = string;
  copyString += element;
  return copyString;
}

String operator+(const char element, const String& string) {
  String copyString(1, element);
  copyString += string;
  return copyString;
}

String operator+(const String& string1, const String& string2) {
  String copyString(string1);
  copyString += string2;
  return copyString;
}

bool operator==(const String& s1, const String& s2) {
  if (s1.length() != s2.length()) {
    return false;
  }
  for (size_t i = 0; i < s1.length(); ++i) {
    if (s1[i] != s2[i]) {
      return false;
    }
  }
  return true;
}

std::ostream& operator<<(std::ostream& out, const String& string) {
  for (size_t i = 0; i < string.stringLength; ++i) {
    out << string.charArr[i];
  }
  return out;
}

std::istream& operator>>(std::istream& in, String& string) {
  string.clear();
  char inputChar;
  while (!in.eof() && std::isspace(in.peek())) {
    in.get(inputChar);
  }
  while (!in.eof() && !std::isspace(in.peek())) {
    in.get(inputChar);
    string.push_back(inputChar);
  }
  return in;
}
