#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#include <iostream>
#include <cstring>

class String {
public:
    String() = default;

    String(const char* string) {
        size = strlen(string);
        str = new char[size];
        buffer = size;
        memcpy(str,string, size);
    }

    String(size_t n, const char& c) {
        size = n; 
        str = new char[size];
        buffer = size; 
        memset(str, c, n);
    }

    String(const String& string) {
        size = string.size;
        str = new char[size];
        buffer = size;
        memcpy(str, string.str, size);
    }

    ~String() {
        delete[] str;
    }
    
    String& operator=(const String& string) {
        if (this == &string) {
            return *this;
        } else {
            size = string.length();
            buffer = size;
            str = new char[size];
            memcpy(str, string.str, size);
            return *this;
        }
    }

    String& operator=(const char* string) {
        size = strlen(string);
        buffer = size;
        str = new char[size];
        memcpy(str, string, size);
        return *this;
    }

    friend String operator+(const String& str1, const String& str2);
    friend String operator+(const String& str, const char& c);
    friend String operator+(const char& c, const String& str);

    String& operator+=(const char& c) {
        push_back(c);
        return *this;
    }
    
    String& operator+=(const String& string) {
        for (size_t i = 0; i < string.size; ++i) {
            push_back(string[i]);
        }
        return *this;
    }

    void push_back(const char& c) {
         if (size >= buffer) {
            char* was = str;
            buffer *= 2;
            str = new char[buffer];
            for (size_t i = 0; i < size; ++i) {
                str[i] = was[i];
            }
            delete[] was;
        }
        str[size] = c;
        size++;
    }

    void pop_back() {
        size--;
    }

    size_t length() const {
        return size;
    }

    friend bool operator==(const String& str1, const String& str2);
    friend bool operator==(const char& c, const String& str);
    friend bool operator==(const String& str, const char& c);

    const char& operator[](size_t idx) const {
        return str[idx];
    }
    char& operator[](size_t idx) {
        return str[idx];
    }

    char& front() {
        return str[0];
    }
    const char& front() const {
        return str[0];
    }

    size_t find(const String& substring) const {
        if (substring.size == 0) return size;
        for (size_t i = 0; i < size - substring.size; ++i) {
            size_t pr = 1;
            for (size_t j = i; j < i + substring.size; ++j) {
                   if (str[j] != substring.str[j - i]) {
                       pr = 0;
                       break;
                   }
            }
            if (pr) return i;
        }
        return size;
    }
    
    size_t rfind(const String& substring) const {
        if (substring.size == 0) return size;
        size_t k = 0;
        for (size_t i = size - substring.size; i >= k; --i) {
            size_t pr = 1;
            for (size_t j = i; j < i + substring.size; ++j) {
                if (str[j] != substring.str[j - i]) {
                    pr = 0;
                    break;
                }
            }
            if (pr) return i;
        }
        return size;
    }

    String substr(size_t start, size_t count) const {
        String ret;
        for (size_t i = start; i < start + count; ++i) {
            ret.push_back(str[i]);
        }
        return ret;
    }

    char& back() {
        return str[size - 1];
    }
    const char& back() const {
        return str[size - 1];
    }
    
    bool empty() const {
        return (size == 0 ? true : false);
    }

    void clear() {
        size = 0;
        buffer = 1;
        delete[] str;
        str = new char[0];
    }

    friend std::ostream& operator<<(std::ostream& out, const String& s);
    friend std::istream& operator>>(std::istream& in, String& s);

private:
    char* str = new char[0];
    size_t size = 0;
    size_t buffer = 1;
};

String operator+(const String& str1, const String& str2) {
    String ret = str1;
    ret += str2;
    return ret;
}

String operator+(const String& str, const char& c) {
    String ret = str;
    ret.push_back(c);
    return ret;
}

String operator+(const char& c, const String& str) {
    String ret;
    ret.push_back(c);
    ret += str;
    return ret;
}

bool operator==(const char& c, const String& str) {
    if (str.size != 1) return false;
    if (str.str[0] != c) return false;
    return true;
}

bool operator==(const String& str1, const String& str2) {
    if (str1.size != str2.size) return false;
    else {
        for (size_t i = 0; i < str1.size; ++i) {
            if (str1[i] != str2[i]) return false;
        }
        return true;
    }
}

bool operator==(const String& str, const char& c) {
    return (c == str);
}

std::ostream& operator<<(std::ostream& out, const String& s) {
    for (size_t i = 0; i < s.size; ++i) {
        out << s.str[i];
    }
    return out;
}

std::istream& operator>>(std::istream& in, String& s) {
    char c;
    while (in.get(c)) {
        if (isspace(c)) break;
        s.push_back(c);
    }
    return in;
}

#endif // STRING_H_INCLUDED
      
