#include <iostream>
#include <string>
#include <vector>


class BigInteger {
public:
	bool num_sign = true;
	BigInteger() = default;
	BigInteger(const int p) : num_sign(p >= 0) {
		int new_n = p;
		if (p == 0) number = std::vector<long long>(1, 0);
		if (0 > new_n) new_n *= -1;
		while (new_n != 0) {
			number.push_back(new_n % basis);
			new_n /= basis;
		}
	}

	void delete_0() {
		while (number.size() > 1 && number.back() == 0) {
			number.pop_back();
		}
	}
	void add_0(size_t n) {
		number.reserve(n);
		int ind = n - number.size();
		for (int i = 0; i < ind; ++i) {
			number.push_back(0);
		}
	}

	friend bool operator==(const BigInteger&, const BigInteger&);
	friend std::ostream& operator<<(std::ostream&, const BigInteger&);
	friend std::istream& operator>>(std::istream&, BigInteger&);

	bool operator!=(const BigInteger& num) {
		return !(*this == num);
	}

	BigInteger& operator=(const BigInteger& a) = default;

	BigInteger operator-() const {
		BigInteger new_n(*this);
		if (new_n != BigInteger(0)) {
			new_n.num_sign = !new_n.num_sign;
		}
		return new_n;
	}

	BigInteger(std::string str) {
		num_sign = true;
		number.clear();
		if (str.length() != 0) {
			if (str[0] == '-') {
				num_sign = false;
				str = str.substr(1);
			}
			for (int i = str.length(); i > 0; i -= 9) {
				if (i < 9) {
					number.push_back(std::stoi(str.substr(0, i)));
				}
				else {
					number.push_back(std::stoi(str.substr(i - 9, 9)));
				}
			}
			this->delete_0();
		}
		else {
			number = std::vector<long long >(1, 0);
		}
		num_sign = !num_sign;
		if (*this != BigInteger(0)) {
			num_sign = !num_sign;
		}
	}

	std::string toString() const {
		std::string str;
		if (num_sign == false) {
			str.push_back('-');
		}
		str += std::to_string(number[static_cast<int>(number.size()) - 1]);
		for (int i = static_cast<int>(number.size()) - 2; i >= 0; --i) {
			if (9 - static_cast<int>(std::to_string(number[i]).length()) <= 0) {
				str += std::to_string(number[i]);
			}
			else {
				std::string z(9 - std::to_string(number[i]).length(), '0');
				str += (z + std::to_string(number[i]));
			}
		}
		return str;
	}

	bool operator>(const BigInteger& num) {
		BigInteger num2 = num;
		BigInteger num1 = *this;
		num1.delete_0();
		num2.delete_0();
		if (!num1.num_sign) {
			if (!num2.num_sign) {
				if ((-num2) > (-num1)) {
					return true;
				}
				return false;
			}
			return false;
		}
		if (num1.num_sign && num2.num_sign == false) {
			return true;
		}
		size_t size1 = num1.number.size(), size2 = num2.number.size();
		if (size1 == size2) {
			for (int i = size2 - 1; i >= 0; --i) {
				if (num1.number[i] != num2.number[i]) {
					if (num1.number[i] > num2.number[i]) {
						return true;
					}
					return false;
				}
			}
		}
		if (size1 > size2) {
			return true;
		}
		return false;
	}

	bool operator<=(const BigInteger& num) {
		if (*this > num) {
			return false;
		}
		return true;
	}
	bool operator<(const BigInteger& num) {
		if ((*this <= num) && (*this != num)) {
			return true;
		}
		return false;
	}
	bool operator>=(const BigInteger& num) {
		if (*this < num) {
			return false;
		}
		return true;
	}

	BigInteger& operator+=(const BigInteger& num) {
		BigInteger new_n = num;
		BigInteger res = *this;
		if (!new_n.num_sign and num_sign) {
			new_n.num_sign = true;
			*this -= new_n;
			return *this;
		}
		if (new_n.num_sign and !num_sign) {
			new_n += *this;
			*this = new_n;
			return *this;
		}
		if (this->number.size() >= new_n.number.size()) {
			res.add_0(this->number.size());
			new_n.add_0(this->number.size());
		}
		else {
			res.add_0(new_n.number.size());
			new_n.add_0(new_n.number.size());
		}

		size_t sum, balance = 0;
		for (size_t k = 0; k <= res.number.size() - 1; ++k) {
			sum = res.number[k] + new_n.number[k] + balance;
			res.number[k] = sum % BigInteger::basis;
			balance = sum / BigInteger::basis;
		}
		if (balance != 0)
		{
			res.number.push_back(balance);
		}
		*this = res;
		return *this;
	}

	BigInteger& operator-=(const BigInteger& num) {
		BigInteger new_n = num;
		if (*this == num) {
			number = std::vector<long long>(1, 0);
			num_sign = true;
			return *this;
		}
		if (new_n.num_sign == false && num_sign == true) {
			new_n.num_sign = true;
			*this += new_n;
			return *this;
		}
		if (new_n.num_sign == true && num_sign == false) {
			new_n -= *this;
			new_n.num_sign = false;
			*this = new_n;
			return *this;
		}
		if (this->number.size() >= new_n.number.size()) {
			this->add_0(this->number.size());
			new_n.add_0(this->number.size());
		}
		else {
			this->add_0(new_n.number.size());
			new_n.add_0(new_n.number.size());
		}
		if (num_sign == true) {
			if (*this > new_n) {
				for (size_t i = 0; i < new_n.number.size(); ++i) {
					this->number[i] -= new_n.number[i];
					if (0 > this->number[i]) {
						this->number[i] += basis;
						this->number[i + 1] -= 1;
					}
				}
				this->delete_0();
				return *this;
			}
			new_n -= *this;
			*this = new_n;
			num_sign = !num_sign;
			return *this;
		}
		new_n.num_sign = !new_n.num_sign;
		num_sign = !num_sign;
		*this -= new_n;
		num_sign = !num_sign;
		return *this;
	}


	BigInteger& operator++() {
		return (*this += 1);
	}
	BigInteger operator++(int) {
		return (*this += 1) - 1;
	}
	BigInteger& operator--() {
		return *this -= 1;
	}
	BigInteger operator--(int) {
		return (*this -= 1) + 1;
	}

	BigInteger& operator*=(const BigInteger& num) {
		BigInteger second = num;
		BigInteger first = *this;
		BigInteger res = 0;

		if (first.number.size() >= 2 * second.number.size()) {
			res.add_0(2 * first.number.size());
		}
		else {
			res.add_0(4 * second.number.size());
		}

		for (size_t i = 0; i <= first.number.size() - 1; ++i)
			for (size_t j = 0; j <= second.number.size() - 1; ++j) {
				long long result = first.number[i] * second.number[j] + res.number[i + j];
				res.number[i + j] = result % basis;
				res.number[i + j + 1] += result / basis;
			}
		res.delete_0();
		res.num_sign = false;
		if (second.num_sign == num_sign) {
			res.num_sign = true;
		}
		*this = res;
		if (-*this == BigInteger(0)) {
			num_sign = true;
		}
		return *this;
	}


	BigInteger& operator/=(const BigInteger& num) {
		BigInteger new_n = num;
		BigInteger res, digits = 0;
		new_n.num_sign = true;
		res.number.resize(number.size());
		int i = static_cast<int>(number.size()) - 1;
		while (i >= 0) {
			digits *= basis;
			digits.number[0] = number[i];
			digits.delete_0();
			size_t x = basis, left = 0, right = basis, middle = (left + right) / 2;
			while (left <= right) {
				if (new_n * BigInteger(middle) > digits) {
					right = middle - 1;
				}
				else {
					left = (x = middle) + 1;
				}
				middle = (left + right) / 2;
			}
			res.number[i] = x;
			digits -= new_n * BigInteger(x);
			--i;
		}
		if (num_sign == num.num_sign) {
			res.num_sign = true;
		}
		else {
			res.num_sign = false;
		}
		res.delete_0();
		if (BigInteger(0) == -res) {
			res.num_sign = true;
		}
		return *this = res;
	}



	BigInteger& operator%=(const BigInteger& num) {
		*this -= (*this / num) * num;
		return *this;
	}

	explicit operator bool() const {
		return !(*this == BigInteger(0));
	}
	friend BigInteger operator/(const BigInteger& num, const BigInteger& down);
	friend BigInteger operator*(const BigInteger& num, const BigInteger& down);
	friend BigInteger operator+(const BigInteger& num, const BigInteger& down);
	friend BigInteger operator-(const BigInteger& num, const BigInteger& down);
	friend BigInteger operator%(const BigInteger& num, const BigInteger& down);
private:
	static const size_t basis = 1000000000;
	std::vector<long long> number;
};


BigInteger operator+(const BigInteger& num, const BigInteger& down) {
	BigInteger other = num;
	other += down;
	return other;
}
BigInteger operator-(const BigInteger& num, const BigInteger& down) {
	BigInteger other = num;
	other -= down;
	return other;
}
BigInteger operator/(const BigInteger& num, const BigInteger& down) {
	BigInteger other = num;
	other /= down;
	return other;
}
BigInteger operator%(const BigInteger& num, const BigInteger& down) {
	BigInteger other = num;
	other %= down;
	return other;
}
BigInteger operator*(const BigInteger& num, const BigInteger& down) {
	BigInteger other = num;
	other *= down;
	return other;
}

std::istream& operator>>(std::istream& in, BigInteger& input) {
	std::string str;
	in >> str;
	input = BigInteger(str);
	return in;
}

std::ostream& operator<<(std::ostream& os, const BigInteger& output) {
	std::string string = output.toString();
	os << string;
	return os;
}

bool operator==(const BigInteger& first, const BigInteger& second) {
	BigInteger num1 = first;
	BigInteger num2 = second;
	if (num1.num_sign != num2.num_sign) {
		return false;
	}
	num1.delete_0();
	num2.delete_0();
	if (num1.number.size() != num2.number.size()) {
		return false;
	}
	for (size_t i = 0; i <= num1.number.size() - 1; ++i) {
		if (num1.number[i] != num2.number[i]) {
			return false;
		}
	}
	return true;
}

class Rational {
public:
	Rational(int num, int down = 1) : numer(num), denom(down) {};
	Rational() = default;

	friend Rational operator+(const Rational& num, const Rational& down);
	friend Rational operator-(const Rational& num, const Rational& down);
	friend Rational operator/(const Rational& num, const Rational& down);
	friend Rational operator*(const Rational& num, const Rational& down);
	void reduce_fraction() {
		BigInteger new_numer = numer;
		BigInteger new_denom = denom;
		while (new_denom) {
			new_numer %= new_denom;
			std::swap(new_numer, new_denom);
		}
		numer /= new_numer;
		denom /= new_numer;
		if (denom.num_sign == numer.num_sign || this->numer == BigInteger(0)) {
			numer.num_sign = true;
		}
		else {
			numer.num_sign = false;
		}
		denom.num_sign = true;
	}
	Rational(const BigInteger& num, const BigInteger& down = 1) : numer(num), denom(down) {};

	explicit operator double() const {
		std::string str = asDecimal(15);
		double main_part = 0, frac_part = 0;
		size_t i = 0;
		while (i < str.size() && str[i] != '.') {
			main_part *= 10;
			main_part += str[i] - '0';
			++i;
		}
		if (i == str.size()) return main_part;
		++i;
		double coef = 0.1;
		while (i < str.size()) {
			frac_part += coef * (str[i] - '0');
			coef *= 0.1;
			++i;
		}
		return main_part + frac_part;
	}

	std::string toString() {
		this->reduce_fraction();
		if (denom == BigInteger(1)) {
			return numer.toString();
		}
		return numer.toString() + '/' + denom.toString();
	}

	std::string asDecimal(size_t point = 0) const {
		BigInteger rank = 1;
		BigInteger frac = (numer / denom);
		std::string result;
		for (size_t i = 0; i < point; ++i) {
			rank *= 10;
		}
		if (frac == BigInteger(0) && !(numer.num_sign == denom.num_sign)) {
			result = "-";
		}
		result += frac.toString();
		if (point == 0) {
			return result;
		}
		result += '.';
		BigInteger new_numer = numer, new_denom = denom;
		new_numer.num_sign = true;
		new_denom.num_sign = true;
		frac.num_sign = true;
		std::string frac_part = (((new_numer * rank) / new_denom) - frac * rank).toString();
		if (frac_part == "0") {
			return result += std::string(point, '0');
		}
		if (static_cast<int>(point) <= static_cast<int>(frac_part.length())) {
			return result += frac_part;
		}
		return result += std::string(point - frac_part.length(), '0') + frac_part;
	}

	bool operator>(const Rational& frac) {
		Rational new_this = *this;
		new_this.reduce_fraction();
		Rational new_frac = frac;
		new_frac.reduce_fraction();
		return new_this.numer * new_frac.denom > new_this.denom * new_frac.numer;
	}

	bool operator==(const Rational& frac) {
		Rational new_this = *this;
		new_this.reduce_fraction();
		Rational new_frac = frac;
		new_frac.reduce_fraction();
		return new_this.numer * new_frac.denom == new_this.denom * new_frac.numer;
	}

	bool operator>=(const
		Rational& frac) {
		return (*this > frac || *this == frac);
	}

	bool operator<(const Rational& frac) {
		return !(*this >= frac);
	}

	bool operator<=(const Rational& frac) {
		return !(*this > frac);
	}

	bool operator!=(const Rational& frac) {
		return !(*this == frac);
	}

	explicit operator bool() const {
		Rational new_r = *this;
		return !(new_r == 0);
	}

	Rational& operator+=(const Rational& frac) {
		numer *= frac.denom;
		numer += denom * frac.numer;
		denom *= frac.denom;
		this->reduce_fraction();
		return *this;
	}

	Rational& operator-=(const Rational& frac) {
		*this += -frac;
		return *this;
	}

	Rational& operator/=(const Rational& frac) {
		numer *= frac.denom;
		denom *= frac.numer;
		this->reduce_fraction();
		return *this;
	}

	Rational& operator*=(const Rational& frac) {
		numer *= frac.numer;
		denom *= frac.denom;
		this->reduce_fraction();
		return *this;
	}
	Rational operator-() const {
		Rational new_this = *this;
		new_this.reduce_fraction();
		new_this.numer *= -1;
		return new_this;
	}

	Rational& operator++() {
		return *this += 1;
	}
	Rational operator++(int) {
		return (*this += 1) - 1;
	}
	Rational& operator--() {
		return *this -= 1;
	}
	Rational operator--(int) {
		return (*this -= 1) + 1;
	}

private:
	BigInteger numer;
	BigInteger denom;
};

Rational operator+(const Rational& num, const Rational& down) {
	Rational other = num;
	other += down;
	return other;
}
Rational operator-(const Rational& num, const Rational& down) {
	Rational other = num;
	other -= down;
	return other;
}
Rational operator/(const Rational& num, const Rational& down) {
	Rational other = num;
	other /= down;
	return other;
}
Rational operator*(const Rational& num, const Rational& down) {
	Rational other = num;
	other *= down;
	return other;
}