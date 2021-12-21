#include <iostream>
#include <string>
 
using std::string;
using std::cin;
using std::cout;
 
struct BigInt {
    string str;
    int sign;
    string intPart;
    string fracPart;
    BigInt(const string s): str(s), sign(Sign(s)), intPart(IntPart(s)), fracPart(FracPart(s)){};
 
    int PosPoint(const string& str) {
        int ind = str.size();
        for (int i = 0; i < str.size(); ++i) {
            if (str[i] == '.') {
                ind = i;
                break;
            }
        }
        return ind;
    }
 
    int Sign(const string& str) {
        return str[0] == '-' ? 1 : 0;
    }
 
    string IntPart(const string& str) {
        int ind = PosPoint(str);
        int sgn = Sign(str);
        return str.substr(sgn, ind - sgn);
    }
 
    string FracPart(const string& str) {
        int ind = PosPoint(str);
        int size = str.size();
        if (ind < size) {
            return str.substr(ind + 1, size - ind - 1);
        } else {
            return "";
        }
    }
};
 
void liningInt(string& str1, string& str2) {
    while (str1.size() < str2.size()) {
        str1 = "0" + str1;
    }
}
 
void liningFrac(string& str1, string& str2) {
    while (str1.size() < str2.size()) {
        str1 += "0";
    }
}
 
bool operator<(BigInt& bigint1, BigInt& bigint2) {
    bool ret = true;
    int sgn1 = bigint1.sign;
    int sgn2 = bigint2.sign;
 
    if (sgn1 == 1 && sgn2 == 0) {
        return true;
    } else if (sgn1 == 0 && sgn2 == 1) {
        return false;
    } else {
        liningInt(bigint1.intPart, bigint2.intPart);
        liningInt(bigint2.intPart, bigint1.intPart);
 
        liningFrac(bigint1.fracPart, bigint2.fracPart);
        liningFrac(bigint2.fracPart, bigint1.fracPart);
 
        if (bigint1.intPart < bigint2.intPart) {
            ret = true;
        } else if (bigint1.intPart > bigint2.intPart) {
            ret = false;
        } else {
            if (bigint1.fracPart < bigint2.fracPart) {
                ret = true;
            } else if (bigint1.fracPart >= bigint2.fracPart) {
                ret = false;
            }
        }
 
        if (bigint1.sign == 0) {
            return ret;
        } else {
            return !ret;
        }
    }
}
 
bool operator>(BigInt& bigint1, BigInt& bigint2) {
    return bigint2 < bigint1;
}
 
bool operator==(BigInt& bigint1, BigInt& bigint2) {
    return !(bigint1 < bigint2) && !(bigint1 > bigint2);
}
 
int main(){
    string str1, str2;
    cin >> str1 >> str2;
    BigInt a(str1), b(str2);
 
    if (a < b) {
        cout << "<";
    } else if (a > b) {
        cout << ">";
    } else {
        cout << "=";
    }
}
