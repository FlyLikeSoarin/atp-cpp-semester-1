#include <iostream>
#include <cstring>
class String {
public:
  String() {}
 
  String(const char* s) : size(strlen(s)), str(new char[size]), capacity(size) {
    memcpy(str, s, size);
  }
 
  String(size_t n, char c) : size(n), str(new char[n]), capacity(n) {
    memset(str, c, n);
  }
 
  String(const String& s) : size(s.size), str(new char[s.size]), capacity(s.size) {
    memcpy(str, s.str, size);
  }
 
  String(char c) :size(1), str(new char[1]), capacity(1) {
    str[0] = c;
  }
 
  ~String() {
    delete[] str;
  }
 
  String& operator=(const String& s) {
    String copy = s;
    swap(copy);
    return *this;
  }
 
  char& operator[](size_t index) {
    return str[index];
  }
 
  const char& operator[](size_t index) const {
    return str[index];
  }
 
  size_t length() const {
    return size;
  }
 
  void push_back(char c) {
    AddChar(c);
  }
 
  void pop_back() {
    if (4 * size <= capacity) {
      String copy;
      copy.size = size;
      copy.capacity = capacity / 2;
      copy.str = new char[copy.capacity];
      memcpy(copy.str, str, size);
      swap(copy);
    }
    --size;
  }
 
  char& front() {
    return str[0];
  }
 
  const char& front() const {
    return str[0];
  }
 
  char& back() {
    return str[size - 1];
  }
 
  const char& back() const {
    return str[size - 1];
  }
 
  String& operator+=(char c) {
    AddChar(c);
    return *this;
  }
 
  String& operator+=(const String& s) {
    if (size + s.size > capacity) {
      String copy;
      copy.size = size + s.size;
      copy.capacity = capacity + s.capacity;
      copy.str = new char[copy.capacity];
      memcpy(copy.str, str, size);
      memcpy(copy.str + size, s.str, s.size);
      swap(copy);
    }
    else {
      memcpy(str + size, s.str, s.size);
      size += s.size;
    }
    return *this;
  }
 
  size_t find(const String& substr) const {
    return CommonFinder(substr);
  }
 
  size_t rfind(const String& substr) const {
    return CommonFinder(substr, true);
  }
 
  String substr(size_t start, size_t count) const {
    String copy;
    copy.capacity = copy.size = count;
    copy.str = new char[count];
    if (start + count <= size) {
      memcpy(copy.str, str + start, count);
    }
    else {
      memcpy(copy.str, str + start, size - start);
    }
    return copy;
  }
 
  bool empty() const {
    return size == 0;
  }
 
  void clear() {
    size = 0;
  }
 
 
private:
  size_t size = 0;
  char* str = nullptr;
  size_t capacity = 0;
  void swap(String& copy) {
    std::swap(copy.str, str);
    std::swap(copy.size, size);
    std::swap(copy.capacity, capacity);
  }
 
  String& AddChar(char c)  {
    if (size == capacity) {
      String copy;
      copy.size = size;
      copy.capacity = capacity * 2;
      if (copy.capacity == 0) {
        ++copy.capacity;
      }
      copy.str = new char[copy.capacity];
      memcpy(copy.str, str, size);
      swap(copy);
    }
    str[size++] = c;
  }
 
  size_t CommonFinder(const String& substr, bool is_reversed = false) const {
    for (size_t i = 0; i <= size - substr.size; ++i) {
      bool flag = true;
      for (size_t j = 0; j < substr.size; ++j) {
        if (str[is_reversed ? size - substr.size - i + j : i + j] != substr[j]) {
          flag = false;
          break;
        }
      }
      if (flag) {
        return i;
      }
    }
    return size;
  }
};
 
String operator+(const String& first, const String& second) {
  String copy = first;
  copy += second;
  return copy;
}
 
bool operator==(const String& first, const String& second) {
  if (first.length() != second.length())
    return false;
  for (size_t i = 0; i < first.length(); ++i)
    if (first[i] != second[i])
      return false;
  return true;
}
 
std::ostream& operator<<(std::ostream& out, const String& s) {
  for (size_t i = 0; i < s.length(); ++i) {
    out << s[i];
  }
  return out;
}
 
std::istream& operator>>(std::istream& in, String& s) {
  s.clear();
  char c;
  while (in.get(c) && !in.eof() && c != ' ' && c != '\n') {
    s.push_back(c);
  }
  return in;
}
