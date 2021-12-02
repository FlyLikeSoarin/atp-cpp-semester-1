/**
 *    author:  VladimirSlastin
 *    created: 19.11.2021 - 25.11.2021.
**/
#include <iostream>
void solve (int** baseArray,
			long* szArrays,
			bool* isUsed,
			long long& ans,
			long long currentMultiply,
			int currentLevel,
			int levels) {
	if (currentLevel == levels) {
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
				  currentLevel + 1,
				  levels);
			isUsed[i] = false;
		}
	}
}
int main (int argc, char* argv[]) {
	long maxSize = -1;
	long* intArgv = new long[argc - 1];
	for (int i = 1; i < argc; ++i) {
		char* tmp = argv[i];
		long a;
		char** end = nullptr;
		a = strtol(tmp, end, 10);
		intArgv[i - 1] = a;
		maxSize = std::max(maxSize, intArgv[i - 1]);
	}
	int** baseArray = new int* [argc - 1];
	for (int i = 0; i < argc - 1; ++i) {
		baseArray[i] = new int[intArgv[i]];
	}
	for (int arr = 0; arr < argc - 1; ++arr) {
		for (int elem = 0; elem < intArgv[arr]; ++elem) {
			std:: cin >> baseArray[arr][elem];
		}
	}
	bool* isUsed = new bool[maxSize];
	for (int i = 0; i < maxSize; ++i) {
		isUsed[i] = false;
	}
	long long ans = 0;
	long long currentMultiply = 1;
	int currentLevel = 0;
	solve(baseArray, intArgv, isUsed, ans, currentMultiply, currentLevel, argc - 1);
	std::cout << ans << '\n';
	delete[] intArgv;
	for (int i = 0; i < argc - 1; ++i) {
		delete[] baseArray[i];
	}
	delete[] baseArray;
	delete[] isUsed;
	return 0;
}