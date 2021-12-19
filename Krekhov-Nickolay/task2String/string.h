#include <iostream>
#include <cstring>
class String {

 public:
  size_t length() const;
  void push_back(char c);
  void pop_back();
  char& back();
  char& front();
  const char& back() const;
  const char& front() const;
  bool empty() const;
  void clear();

  size_t find(const String& string) const {
    return findHelper(string, false);
  }
  size_t rfind(const String& string) const {
    return findHelper(string, true);
  }

  String substr(size_t start, size_t count) const {
    String string;
    string.size = count;
    string.bufferSize = count * 2 + 3;
    string.str = new char[bufferSize];
    for (size_t i = 0; i < count; ++i) {
      string.str[i] = str[i + start];
    }
    return string;
  }

  //------------------------------CONSTRUTORS && DESTRUCTOR----------------------------------------------------

  String() {
    str = new char[bufferSize];
  };

  String(const size_t n, const char c) : size(n), bufferSize(n) {
    str = new char[bufferSize];
    memset(str, c, n);
  }

  String(const String& string) : str(new char[string.bufferSize]), size(string.size), bufferSize(string.bufferSize) {
    memcpy(str, string.str, string.size);
  }

  String(const char* string) : size(strlen(string)), bufferSize(strlen(string)) {
    str = new char[bufferSize];
    for (size_t i = 0; i < size; ++i) {
      str[i] = string[i];
    }
  }

  String(const char c) : str(new char[2]), size(1), bufferSize(5) {
    str[0] = c;
  }

  ~String() {
    delete[] str;
  }
  //----------------------------------------------------------------------------------

  //-------------------------OPERATORS---------------------------------------------


  String& operator+=(const String& other) {
    size_t len = size;
    size += other.size;
    while (size > bufferSize) {
      increaseSize();
    }
    for (size_t i = len; i < size; ++i) {
      str[i] = other.str[i - len];
    }
    return *this;
  }
  bool operator==(const String& other) {
    if (size != other.size) return false;
    for (size_t i = 0; i < size; ++i) {
      if (str[i] != other.str[i]) {
        return false;
      }
    }
    return true;
  }

  String& operator=(const String& string) {
    String copy(string);
    swap(copy);
    return *this;
  }
  const char& operator[](size_t index) const {
    return str[index];
  }
  char& operator[](size_t index) {
    return str[index];
  }

 private:

  //-----------------------------------MAIN FIELDS------------------------------------------------
  char* str = nullptr;
  size_t size = 0;
  size_t bufferSize = 32;

  //--------------------------------------------MEMORY---------------------------------------------
  void increaseSize();
  void decreaseSize();
  //--------------------------------------HELP FUCTIONS-------------------------------------------

  void swap(String& string) {
    std::swap(size, string.size);
    std::swap(str, string.str);
    std::swap(bufferSize, string.bufferSize);
  }

  size_t findHelper(const String& string, bool reversed) const {
    for (size_t i = (reversed ? size - string.size + 1 : 0); (reversed ? i >= 1 : i < size - string.size);
         (reversed ? --i : ++i)) {
      int flag = 1;
      for (size_t j = 0; j < string.size; ++j) {
        if (str[i + j - reversed] != string.str[j]) {
          flag = 0;
          break;
        }
      }
      if (flag == 1) {
        return i - reversed;
      }
    }
    return size;
  }

};

void String::increaseSize() {
  bufferSize = 2 * bufferSize + 3;
  char* copy = new char[bufferSize];
  memcpy(copy, str, size);
  delete[] str;
  str = copy;
}

void String::decreaseSize() {
  bufferSize = bufferSize / 2;
  char* copy = new char[bufferSize];
  memcpy(copy, str, size);
  delete[] str;
  str = copy;
}

size_t String::length() const {
  return size;
}
void String::push_back(char c) {
  ++size;
  if (size > bufferSize) {
    increaseSize();
  }
  str[size - 1] = c;
}
void String::pop_back() {
  if (bufferSize > size * 4) {
    decreaseSize();
  }
  if (size > 0) {
    --size;
  }
}

char& String::back() {
  return str[size - 1];
}
const char& String::back() const {
  return str[size - 1];
}

char& String::front() {
  return str[0];
}
const char& String::front() const {
  return str[0];
}

bool String::empty() const {
  if (size > 0) return false;
  else return true;
}
void String::clear() {
  size = 0;
  bufferSize = 3;
  delete str[];
  str = new char[bufferSize];
}

String operator+(const String& arg1, const String& arg2) {
  String copy = arg1;
  copy += arg2;
  return copy;
}
std::ostream& operator<<(std::ostream& out, const String& S) {
  for (size_t i = 0; i < S.length(); ++i) {
    out << S[i];
  }
  return out;
}
std::istream& operator>>(std::istream& in, String& S) {
  S.clear();
  while (isspace(in.peek())) in.get();
  for (char c; !isspace(in.peek()) && in >> c;) {
    S.push_back(c);
  }
  return in;
}
