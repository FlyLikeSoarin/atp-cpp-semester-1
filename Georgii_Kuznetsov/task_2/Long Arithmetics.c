#include <cstring>
#include <stdio.h>

int compare_2(const char* number_1, const char* number_2){
    if (number_1[0] == '-'){
        if (number_2[0] != '-'){
            return 1;
        }
    }if (number_2[0] == '-') {
        if (number_1[0] != '-'){
            return 3;
        }
    }
    int cpr = 2;
    int after_dot = 0;
    for (size_t i = 0; i < strlen(number_2); ++i) {
        if (number_1[i] == '.'){
            after_dot = 1;
        }
        if (number_1[i] != number_2[i]){
            if (number_1[i] == '.'){
                return 1;
            }else if (number_2[i] == '.'){
                return 3;
            }else if (number_1[i] > number_2[i]){
                if (cpr == 2){
                    cpr = 3;
                }
            }else if (number_2[i] > number_1[i]){
                if (cpr == 2){
                    cpr = 1;
                }
            }
        }
    }
    for (size_t i = strlen(number_2); i < strlen(number_1); ++i) {
        if (number_1[i] == '.'){
            after_dot = 1;
        }
        else if (number_1[i] != '0'){
            if (!after_dot){
                cpr = 3;
            }else{
                if (cpr != 1){
                    cpr = 3;
                }
            }
        }
    }

    return cpr;

}

int main() {
    char* number_1;
    char* number_2;
//    scanf_s(number_1);
//    scanf_s(number_2);
    scanf_s("%s", &number_1);
    scanf_s("%s", &number_2);

    int result;
    int swapped = 0;

    printf("%s", number_1);

    if (strlen(number_1) >= strlen(number_2)){
        result = compare_2(number_1, number_2);
    }else{
        result = compare_2(number_2, number_1);
        swapped = 1;
    }
    if (number_1[0] == number_2[0] && number_1[0] == '-'){
        if (swapped){
            swapped = 0;
        } else{
            swapped = 1;
        }
    }
    if (result == 1){
        if (!swapped){
            printf("<");
        }else{
            printf(">");
        }
    }else if (result == 2){
        printf("=");
    }else {
        if (!swapped) {
            printf(">");
        } else {
            printf("<");
        }
    }
    printf("%s", number_2);
    return 0;
}
