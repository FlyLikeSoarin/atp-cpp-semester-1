#include <iostream>
#include <string.h>

long long tot = 0;

void collect(int** &arr, int* indexes, int mas_index, int* sizes, long long pr, int kol_indexes, int arr_size) {
  if (mas_index >= arr_size) {
    tot += pr;
  }
  else {
    for (int i = 0; i < sizes[mas_index]; ++i) {
      bool flag = true;
      for (int j = 0; j < kol_indexes; ++j) {
        if (indexes[j] == i) {
          flag = false;
          break;
        }
      }
      if (flag == true) {
        indexes[kol_indexes] = i;
        collect(arr,
                indexes,
                mas_index + 1,
                sizes,
                pr * arr[mas_index][i],
                kol_indexes + 1,
                arr_size);
      }
    }
  }
}

int main(int argc, char** argv) {
  int** arr = new int*[argc - 1];
  int* sizes = new int[argc - 1];
  for (int i = 1; i < argc; ++i) {
    int size = std::atoi(argv[i]);
    int* array = new int[size];
    for (int j = 0; j < size; ++j) {
      int k; std::cin >> k;
      array[j] = k;
    }
    arr[i - 1] = array;
    sizes[i - 1] = size;
  }
  int* indexes = new int[argc - 1];
  collect(arr, indexes, 0, sizes, 1, 0, argc - 1);
  std::cout << tot << '\n';
  delete[] indexes;
  for (int i = 0; i < argc - 1; ++i) {
    delete[] arr[i];
  }
  delete[] arr;
  delete[] sizes;
}
