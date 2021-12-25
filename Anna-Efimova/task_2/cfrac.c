#include <stdio.h>
#include <string.h>

#define maxlen 10000

int index_dot(const char* x) {
	int i;
    for(i = 0; x[i]; i++)
        if(x[i] == '.')
            return i;
    return i;    
}

int if_null(const char* x, int left, int right) {
	if (left >= right) return 0;
	for (int i = left; i < right; ++i) {
		if ('1' <= x[i] && x[i] <= '9') return 1;
	}
	return 0;
}

int min(int left, int right)
{
    return left < right ? left : right;
}

char compare(const char* x, const char* y) {
	int xsiz = strlen(x);
	int ysiz = strlen(y);
	int index_dotx = index_dot(x);
	int index_doty = index_dot(y);

	if (index_dotx > index_doty) return '>';
	if (index_dotx < index_doty) return '<';
	for (int i = 0; i < min(xsiz, ysiz); ++i) {
		if (x[i] > y[i]) return '>';
		if (x[i] < y[i]) return '<';
	}

	if (if_null(x, min(xsiz, ysiz), xsiz) == 1) return '>';
	if (if_null(y, min(xsiz, ysiz), ysiz) == 1) return '<';
	return '=';

}


int main() {
	char x[maxlen], y[maxlen];
    scanf("%s",x);
    scanf("%s",y);
	
	if (x[0] != '-' && y[0] == '-') printf("%c", '>');
	else if (x[0] == '-' && y[0] != '-') printf("%c", '<');
	else if (x[0] != '-' && y[0] != '-') printf("%c",compare(x, y));
	else {
		char byabs = compare(x, y);
		if (byabs == '>')  printf("%c", '<');
		else if (byabs == '<')  printf("%c", '>');
		else  printf("%c", '=');
	}
	return 0;
}
