#include <iostream>
#include <cstring>


class String {
 public:
  void clear_string() {
    if (size == 1) return;
    size >>= 1;
    char* tmp = new char[size];
    memcpy(tmp, str, size);
    delete[] str;
    str = tmp;
  }
  String(const char* s) : size(1){
    while (size <= strlen(s)) {
      size <<= 1;
    }
    delete[] str;
    str = new char[size];
    memcpy(str, s, strlen(s));
    capacity = strlen(s) + 1;
    str[capacity - 1] = '\0';
  }
  String(const String& s) {
    delete[] str;
    while (size <= s.size) size <<= 1;
    str = new char[size];
    memcpy(str, s.str, s.capacity);
    capacity = s.capacity;
  }
  String(size_t n, char c) {
    capacity = n;
    while (size <= n) {
      size <<= 1;
    }
    delete[] str;
    str = new char[size];
    memset(str, c, n);
    str[capacity] = '\0';
    ++capacity;
  }
  String(): size(1), capacity(0) {
    delete[] str;
    str = new char[size];
    str[0] = '\0';
    capacity = 1;
  }
  ~String() {
    delete[] str;
  }
  [[nodiscard]] size_t length() const {
    return capacity - 1;
  }
  void push_back(char symbol) {
    if (size == capacity) {
      char *tmp = new char[size << 1];
      memcpy(tmp, str, size);
      tmp[size - 1] = symbol;
      tmp[size] = '\0';
      str = tmp;
      size <<= 1;
      ++capacity;
    }
    else {
      str[capacity - 1] = symbol;
      str[capacity] = '\0';
      ++capacity;
    }
  }
  void pop_back() {
    --capacity;
    str[capacity - 1] = '\0';
    if (capacity == size / 4) {
      clear_string();
    }
  }
  char& front() {
    return str[0];
  }
  [[nodiscard]] char front() const {
    return str[0];
  }
  char& back() {
    return str[capacity - 2];
  }
  [[nodiscard]] char back() const {
    return str[capacity - 2];
  }
  [[nodiscard]] size_t find(const String& s) const {
    if (s.capacity > capacity) return length();
    char* c = strstr(str, s.str);
    if (c == nullptr) return length();
    return (c - str);
  }
  [[nodiscard]] size_t rfind(const String& s) const {
    if (s.capacity > capacity) return length();
    char* c = strstr(str, s.str);
    if (c == nullptr) return length();
    while (true) {
      size_t d = (c - str + 1);
      if (strstr(str + d, s.str) == nullptr) {
        if (c == nullptr) return length();
        return (c - str);
      }
      c = strstr(str + d, s.str);
    }
  }
  [[nodiscard]] String substr(size_t l, size_t count) const {
    String s;
    s.size = 1;
    s.capacity = 1;
    s.str = new char[size];
    s.str[0] = '\0';
    for (size_t i = l; i < l + count; ++i) {
      s.push_back(str[i]);
    }
    return s;
  }
  [[nodiscard]] bool empty() const {
    return (length() == 0);
  }
  void clear() {
    size = 1;
    capacity = 1;
    delete[] str;
    str = new char[size];
    str[0] = '\0';
  }
  char& operator[] (const size_t index) {
    return str[index];
  }
  char operator[](const size_t ind) const {
    return str[ind];
  }
  String& operator+= (const String& s) {
    while (size <= s.capacity + capacity - 1) {
      size <<= 1;
    }
    size_t strol = capacity + s.capacity - 1;
    char* tmp = new char[size];
    memcpy(tmp, str, capacity - 1);
    for (size_t i = capacity - 1; i < capacity + s.capacity - 1; ++i) {
      tmp[i] = s.str[i - capacity + 1];
    }
    str = tmp;
    capacity = strol;
    return *this;
  }
  String& operator+= (char symbol) {
    push_back(symbol);
    return *this;
  }
  bool operator==(const String& s1) const {
    if (s1.capacity == capacity) {
      for (size_t i = 0; i < capacity - 1; ++i) {
        if (s1.str[i] != str[i]) return false;
      }
      return true;
    }
    return false;
  }
  String& operator= (const String& s) {
    if (this != &s) {
      while (size < s.size) size <<= 1;
      delete[] str;
      str = new char[size];
      memcpy(str, s.str, s.capacity);
      capacity = s.capacity;
    }
    return *this;
  };
  String operator+ (char symbol);
  friend std::ostream& operator << (std::ostream &out, const String& s);
  friend std::istream& operator >> (std::istream &in, String& s);
 private:
  size_t size = 1;
  size_t capacity = 0;
  char* str = new char[size];
};

std::ostream& operator << (std::ostream &out, const String& s) {
  for (size_t i = 0; i < s.length(); ++i) {
    out << s.str[i];
  }
  return out;
}
std::istream& operator >> (std::istream& in, String &s) {
  char t;
  s.clear();
  while (in.get(t) && !std::isspace(t) && !std::iscntrl(t)) {
    s.push_back(t);
  }
  return in;
}
String operator+ (String& s1, String& s2) {
  String tmp = s1;
  tmp += s2;
  return tmp;
}
String operator+ (char symbol, String& s) {
  String tmp = String(1, symbol);
  return (tmp + s);
}
String String::operator+ (char symbol) {
  String tmp = String(1, symbol);
  return (*this + tmp);
}
