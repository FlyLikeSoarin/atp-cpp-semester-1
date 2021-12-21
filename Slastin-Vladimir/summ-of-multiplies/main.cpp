/**
 *    author:  VladimirSlastin
 *    created: 19.11.2021 - 25.11.2021.
 *    modified: 20.12.2021.
**/
#include <iostream>

int argc_global;

void build_baseArray (int** &baseArray, int* &intArgv, char* argv[], int &maxSize) {
    for (int i = 0; i < argc_global; ++i) {
        char* tmp = argv[i + 1];
        char** end = nullptr;
        intArgv[i] = strtol(tmp, end, 10);
        baseArray[i] = new int[intArgv[i]];
        maxSize = std::max(maxSize, intArgv[i]);
        for (int elem = 0; elem < intArgv[i]; ++elem) {
            std::cin >> baseArray[i][elem];
        }
    }
}
void solve (int** baseArray, int* szArrays, bool* isUsed, long &ans,
            long currentMultiply = 1, int currentLevel = 0) {
    if (currentLevel == argc_global) {
        ans += currentMultiply;
        return;
    }
    for (int i = 0; i < szArrays[currentLevel]; ++i) {
        if (!isUsed[i]) {
            isUsed[i] = true;
            solve(baseArray,
                  szArrays,
                  isUsed,
                  ans,
                  currentMultiply * baseArray[currentLevel][i],
                  currentLevel + 1);
            isUsed[i] = false;
        }
    }
}

void clear_all (int**&baseArray, int*&intArgv, bool*&isUsed) {
    delete[] intArgv;
    for (int i = 0; i < argc_global; ++i) {
        delete[] baseArray[i];
    }
    delete[] baseArray;
    delete[] isUsed;
}

int main (int argc, char* argv[]) {
    int maxSize = -1;
    int** baseArray = new int* [argc];
    int* intArgv = new int[--argc];
    argc_global = argc;
    build_baseArray (baseArray, intArgv, argv, maxSize);
    bool* isUsed = new bool[maxSize];
    for (int i = 0; i < maxSize; ++i) {
        isUsed[i] = false;
    }
    long ans = 0;
    solve(baseArray, intArgv, isUsed, ans);
    std::cout << ans << '\n';
    clear_all(baseArray, intArgv, isUsed);
}