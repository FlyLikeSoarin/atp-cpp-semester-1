﻿#include <iostream>
#include <cstring>

class String {

public:
	String() : sz(0), str(new char[0]), memory(1) {}
	String(size_t n, char c) : sz(n), str(new char[n]), memory(n) {
		memset(str, c, n);
	}

	String(const char* s) : sz(strlen(s)), str(new char[strlen(s)]), memory(strlen(s)) {
		memcpy(str, s, sz);
	}

	String(const String& s) : sz(s.sz), str(new char[s.memory]), memory(s.memory) {
		memcpy(str, s.str, sz);
	}

	String(char l) : sz(1), str(new char[1]), memory(1) {
		str[0] = l;
	}

	size_t length() const {
		return sz;
	}

	void swap(String& s) {
		std::swap(str, s.str);
		std::swap(sz, s.sz);
		std::swap(memory, s.memory);
	}

	String& operator=(String s) {
		swap(s);
		return *this;
	}

	String& operator+=(char addit) {
		push_back(addit);
		return *this;
	}

	bool operator==(const String& s) {
		if (sz != s.sz) return false;
		for (size_t i = 0; i < sz; ++i) {
			if (str[i] != s.str[i]) return false;
		}
		return true;
	}

	void push_back(char addit) {
		if (sz >= memory)
			increase_memory();
		str[sz] = addit;
		sz++;
	}

	String operator+(char addit) const {
		String res(*this);
		res += addit;
		return res;
	}

	String& operator+=(const String& s) {
		while (memory <= s.sz + sz)
			increase_memory();
		for (size_t i = sz; i < sz + s.sz; ++i) {
			str[i] = s.str[i - sz];
		}
		sz += s.sz;
		return *this;
	}

	String operator+(const String& s) const {
		String res(*this);
		res += s;
		return res;
	}

	void pop_back() {
		if (sz)
			sz--;
	}

	char& front() {
		return str[0];
	}

	const char& front() const {
		return str[0];
	}

	char& back() {
		return str[sz - 1];
	}

	const char& back() const {
		return str[sz - 1];
	}

	char& operator[](size_t ind) { return str[ind]; }

	char operator[](size_t ind) const { return str[ind]; }

	void clear() {
		sz = 0;
	}

	bool empty() {
		return sz == 0;
	}

	String substr(size_t begin, size_t count) const {
		String s(count, '0');
		for (size_t i = begin; i < begin + count; ++i) s[i - begin] = str[i];
		return s;
	}

	size_t find(const String& s) const {
		size_t leng = s.sz;
		for (size_t i = 0; i <= sz - leng; ++i) {
			if (this->substr(i, leng) == s) return i;
		}
		return sz;
	}

	size_t rfind(const String& s) const {
		size_t leng = s.sz;
		size_t res = s.sz;
		for (size_t i = 0; i <= sz - leng; ++i) {
			if (this->substr(i, leng) == s) res = i;
		}
		return res;
	}

	~String() {
		delete[] str;
	}

private:
	size_t sz;
	char* str;
	size_t memory;
	friend std::ostream& operator<<(std::ostream& out, const String& s);
	friend std::istream& operator>>(std::istream& in, String& s);
	friend String operator+(char c, const String& s);


	void increase_memory() {
		char* new_one = new char[memory * 2];
		memory *= 2;
		memcpy(new_one, str, sz);
		//delete[] str;
		str = new_one;
	}
};

String operator+(char l, const String& s) {
	return String(1, l) + s;
}

void swap(String& left, String& right) {
	left.swap(right);
}

std::ostream& operator<<(std::ostream& out, const String& s) {
	for (size_t i = 0; i < s.sz; ++i) {
		out << s.str[i];
	}
	return out;
}

std::istream& operator>>(std::istream& in, String& s) {
	if (s.sz != 0)
		s.clear();
	char c = '1';
	while (in.get(c) && c != ' ' && !(isspace(c))) {
		s.push_back(c);
	}
	return in;
}
