#include <iostream>

long long result;

int maximum(int a, int b) {
	if (a > b)
		return a;
	else
		return b;
}

void get_sum(int index, long long arr_prod, bool* if_been_used, int argc, int** arr, int* arr_siz) {
	if (argc == index) {
		result += arr_prod;
		return;
	}
	for (int i = 0; i < arr_siz[index]; ++i) {
		if (if_been_used[i]==0) {
			if_been_used[i] = true;
			get_sum(index + 1, arr_prod * arr[index][i], if_been_used, argc, arr, arr_siz);
			if_been_used[i] = false;
		}

	}
}

int str2int(const char* string) {
	int answer = 0;
	for (int p = 0; string[p]; ++p) {
		answer *= 10;
		answer += string[p] - '0';
	}
	return answer;
}


int main(int argc, char** argv) {
	argc--;
	int** arr = new int* [argc];
	int* arr_siz = new int[argc];
	int size_max = 0;

	for (int i = 0; i < argc; ++i) {
		arr_siz[i] = str2int(argv[i + 1]);
		arr[i] = new int[arr_siz[i]];
		size_max = maximum(arr_siz[i], size_max);
	}

	bool* if_been_used = new bool[size_max];
	for (int i = 0; i < size_max; ++i) {
		if_been_used[i] = false;
	}

	for (int p = 0; p < argc; ++p) {
		for (int i = 0; i < arr_siz[p]; ++i) {
			std::cin >> arr[p][i];
		}
	}

	result = 0;
	get_sum(0, 1, if_been_used, argc, arr, arr_siz);
	std::cout << result;
	delete[] arr_siz;
	delete[] if_been_used;
	for (int i = 0; i < argc; ++i) {
		delete[] arr[i];
	}
	delete[] arr;
	return 0;
}
