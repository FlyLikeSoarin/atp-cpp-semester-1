#include <iostream>
#include <string>
#include <algorithm>

int index_dot(std::string& x, int& siz) {
	for (int i = 0; i < siz; ++i) {
		if (x[i] == '.') return i;
	}
	x += '.';
	siz += 1;
	return siz - 1;
}

int if_null(std::string x, int left, int right) {
	if (left >= right) return 0;
	for (int i = left; i < right; ++i) {
		if ('1' <= x[i] && x[i] <= '9') return 1;
	}
	return 0;
}

char compare(std::string& x, std::string& y) {
	int xsiz = x.size();
	int ysiz = y.size();
	int index_dotx = index_dot(x, xsiz);
	int index_doty = index_dot(y, ysiz);

	if (index_dotx > index_doty) return '>';
	if (index_dotx < index_doty) return '<';
	for (int i = 0; i < std::min(xsiz, ysiz); ++i) {
		if (x[i] > y[i]) return '>';
		if (x[i] < y[i]) return '<';
	}

	if (if_null(x, std::min(xsiz, ysiz), xsiz) == 1) return '>';
	if (if_null(y, std::min(xsiz, ysiz), ysiz) == 1) return '<';
	return '=';

}


int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	std::string x, y;
	std::cin >> x >> y;
	
	if (x[0] != '-' && y[0] == '-') std::cout << '>';
	else if (x[0] == '-' && y[0] != '-') std::cout << '<';
	else if (x[0] != '-' && y[0] != '-') std::cout << compare(x, y);
	else {
		char byabs = compare(x, y);
		if (byabs == '>')  std::cout << '<';
		if (byabs == '<')  std::cout << '>';
		else  std::cout << '=';
	}
}