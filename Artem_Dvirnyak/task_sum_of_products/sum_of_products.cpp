#include <iostream>

long long sum_of_products (int** &arrays, int argc, char* argv[],
                           int current_index, int current_array,
                           int occupied_indexes[]) {
    long long result = 0;
    occupied_indexes[current_array++] = current_index;
    if (current_array == argc-1) {
        result = 1;
        for (int i = 0; i < argc-1; ++i) {
            result *= arrays[i][occupied_indexes[i]];
        }
        return result;
    }
    for (int i = 0; i < atoi(argv[1+current_array]); ++i) {
        bool is_occupied = false;
        for (int j = 0; j < current_array; ++j) {
            if (occupied_indexes[j] == i) {
                is_occupied = true;
                break;
            }
        }
        if (!is_occupied) {
            result += sum_of_products(arrays, argc, argv, i,
                                      current_array, occupied_indexes);
        }
    }
    return result;
}
long long sum_of_products(int** &arrays, int argc, char* argv[]) {
    long long result = 0;
    int* occupied_indexes = new int[argc-1];
    for (int i = 0; i < atoi(argv[1]); ++i) {
        result += sum_of_products(arrays, argc, argv,i,
                                  0, occupied_indexes);
    }
    delete[] occupied_indexes;
    return result;
}
int main(int argc, char* argv[]) {
    int** arrays = new int*[argc-1];
    for (int i = 1; i < argc; ++i) {
        int* temp_arr = new int[atoi(argv[i])];
        arrays[i-1] = temp_arr;
        for (int j = 0; j < atoi(argv[i]); ++j) {
            std::cin >> temp_arr[j];
        }
    }
    std::cout << sum_of_products(arrays, argc, argv) << "\n";
    for (int i = 0; i < argc - 1; ++i) {
        delete[] arrays[i];
    }
    delete[] arrays;
    return 0;
}
