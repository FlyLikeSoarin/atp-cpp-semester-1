#include <stdio.h>
#include <string.h>
short commaIndex (const char* str) {
    short i;
    for (i = 0; str[i]; i++)
        if (str[i] == '.')
            break;
    return i;
} // ready
char naturalPart (const char* str, short l, short r) {
    if (l < r) {
        for (short i = l; i < r; i++)
            if (!('1' > str[i] || str[i] > '9'))
                return 1;
    }
    return 0;
} // ready
short min (short l, short r) {
    return l < r ? l : r;
} // ready
char compare_abs (const char* a, const char* b) {
    char tmp = '=';
    short n = (short)strlen(a);
    short m = (short)strlen(b);
    if (commaIndex(a) == commaIndex(b)) {
        int flag = 0;
        for (short i = 0; i < min(n, m); i++) {
            if (a[i] != b[i]) {
                if (a[i] > b[i])
                    tmp = '>';
                else
                    tmp = '<';
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            if (naturalPart(a, min(n, m), n))
                tmp = '>';
            if (naturalPart(b, min(n, m), m))
                tmp = '<';
        }
    } else {
        if (commaIndex(a) < commaIndex(b))
            tmp = '<';
        else
            tmp = '>';
    }
    return tmp;
} // ready
char solve (const char* a, const char* b) {
    char ans = '>';
    if (a[0] != b[0] && (a[0] == '-' || b[0] == '-')) {
        if (a[0] == '-' && b[0] != '-')
            ans = '<';
        else
            ans = '>';
    } else {
        ans = compare_abs(a, b);
        if (a[0] == '-') {
            if (ans != '=') {
                if (ans == '>')
                    ans = '<';
                else
                    ans = '>';
            }
        }
    }
    return ans;
} // ready
int main () {
    char a[9000];
    char b[9000];
    scanf("%s", a);
    scanf("%s", b);
    printf("%c", solve(a, b));
}