#include <iostream>
#include <cstring>
class String {
 private:
  char* str = nullptr;
  size_t size = 0;
  size_t bufferSize = 32;
  void increaseSize();
  void decreaseSize();
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
  size_t find(const String S) const {

    for (size_t i = 0; i < size - S.size; ++i) {
      int flag = 1;
      for (size_t j = 0; j < S.size; ++j) {
        if (str[i + j] != S.str[j]) {
          flag = 0;
          break;
        }
      }
      if (flag == 1) {
        return i;
      }

    }
    return size;
  }

  size_t rfind(const String S) const {
    for (size_t i = size - S.size + 1; i >= 1; --i) {
      int flag = 1;
      for (size_t j = 0; j < S.size; ++j) {
        if (str[i - 1 + j] != S.str[j]) {
          flag = 0;
          break;
        }
      }
      if (flag == 1) {
        return i - 1;
      }
    }
    return size;
  }

  String substr(size_t start, size_t count) const {
    String S;
    S.size = count;
    S.bufferSize = count * 2 + 3;
    S.str = new char[bufferSize];
    for (size_t i = 0; i < count; ++i) {
      S.str[i] = str[i + start];
    }
    return S;
  }

  String() {
    str = new char[bufferSize];
  };
  String(const size_t n, const char c) : size(n), bufferSize(2 * n) {
    str = new char[bufferSize];
    memset(str, c, n);
  }
  String(const String& S) : str(new char[S.bufferSize]), size(S.size), bufferSize(S.bufferSize) {
    memcpy(str, S.str, S.size);
  }

  String(const char *S) : size(strlen(S)), bufferSize(strlen(S) * 2) {
    str = new char[bufferSize];
    for (size_t i = 0; i < size; ++i) {
      str[i] = S[i];
    }
  }
  String(const char c) : str(new char[2]), size(1), bufferSize(5) {
    str[0] = c;
  }

  String& operator+=(const String& other) {
    size_t len = size;
    size += other.size;
    if (size > bufferSize) {
      increaseSize();
    }
    for (size_t i = len; i < size; ++i) {
      str[i] = other.str[i - len];
    }
    return *this;
  }
  bool operator==(const String& other) {
    if (size != other.size) return false;
    else {
      for (size_t i = 0; i < size; ++i) {
        if (str[i] != other.str[i]) {
          return false;
        }
      }
      return true;
    }
  }
  ~String() {
    delete[] str;
  };
  void swap(String& S) {
    std::swap(size, S.size);
    std::swap(str, S.str);
    std::swap(bufferSize, S.bufferSize);
  }
  String& operator=(const String& S) {
    String copy(S);
    swap(copy);
    return *this;
  }
  const char& operator[](size_t index) const {
    return str[index];
  }
  char& operator[](size_t index) {
    return str[index];
  }
};

void String::increaseSize() {
  bufferSize = 2 * bufferSize + 3;
  char *copy = new char[bufferSize];
  memcpy(copy, str, size);
  delete[] str;
  str = copy;
}

void String::decreaseSize() {
  bufferSize = bufferSize / 2;
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
  while (size > 0) {
    pop_back();
  }
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
