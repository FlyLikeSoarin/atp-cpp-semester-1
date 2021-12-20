#include <iostream>
#include <string>
#include <algorithm>


int compare_2(std::string number_1, std::string number_2){
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
    bool after_dot = false;
    for (size_t i = 0; i < number_2.size(); ++i) {
        if (number_1[i] == '.'){
            after_dot = true;
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
            }else{
                std::cout << i << "\n<- 'if' mistake\n";
            }
        }
    }
    for (size_t i = number_2.size(); i < number_1.size(); ++i) {
        if (number_1[i] == '.'){
            after_dot = true;
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
    std::string number_1, number_2;
    std::cin >> number_1 >> number_2;

    int result;
    bool swapped = false;

    if (number_1.size() >= number_2.size()){
        result = compare_2(number_1, number_2);
    }else{
        result = compare_2(number_2, number_1);
        swapped = true;
    }
    if (number_1[0] == number_2[0] && number_1[0] == '-'){
        if (swapped){
            swapped = false;
        } else{
            swapped = true;
        }
    }
//    std::cout << result << "-result " << swapped << "-swapped\n";
    if (result == 1){
        if (!swapped){
            std::cout << "<";
        }else{
            std::cout << ">";
        }
    }else if (result == 2){
        std::cout << "=";
    }else {
        if (!swapped) {
            std::cout << ">";
        } else {
            std::cout << "<";
        }
    }
    return 0;
}
