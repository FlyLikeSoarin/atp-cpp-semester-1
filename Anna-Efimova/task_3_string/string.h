#include <iostream>
#include <cstring>

class String {
private:
	char* str = nullptr;
	size_t sz;
	size_t memory;
	friend std::ostream& operator<<(std::ostream& out, const String& s);
	friend std::istream& operator>>(std::istream& in, String& s);
	friend String operator+(char c, const String& s);


	void more_memory() {
		String new_one(memory, '0');
		for (size_t i = 0; i < sz; ++i)
			new_one.str[i] = str[i];
		new_one.sz = sz;
		new_one.swap(*this);
	}


public:
	String() : str(new char[5]), sz(0), memory(5) {}
	String(size_t n, char c) : str(new char[2 * n]), sz(n), memory(2 * n) {
		memset(str, c, n);
	}

	String(const char* s) :str(new char[2 * strlen(s)]), sz(strlen(s)), memory(2 * strlen(s)) {
		for (size_t i = 0; i < sz; i++) str[i] = s[i];
	}

	String(const String& s) : str(new char[2 * s.sz]), sz(s.sz), memory(2 * s.sz) {
		memcpy(str, s.str, sz);
	}

	String(char l) :str(new char[2]), sz(1), memory(2) {
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
		else {
			for (size_t i = 0; i < sz; ++i) {
				if (str[i] != s.str[i]) return false;
			}
		}
		return true;
	}

	void push_back(char addit) {
		if (sz >= memory)
			more_memory();
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
			more_memory();
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
		delete[] str;
		sz = 0;
		memory = 10;
		str = nullptr;
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
	while (in.get(c) && c != ' ' && !std::isspace(c)) {
		s.push_back(c);
	}
	return in;
}
