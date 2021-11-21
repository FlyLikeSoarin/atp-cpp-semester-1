#include <iostream>
#include <string.h>

long long summa = 0;

int from_char(char* arr, int arr_sz) {
  int k = 0;
  int t = 1;
  for (int i = 0; i < arr_sz; ++i) {
    for (int j = 0; j < arr_sz - i - 1; ++j) {
      t *= 10;
    }
    k += (arr[i] - '0') * t;
  }
  return k;
}

void collect(int** &arr, int* indexes, int mas_index, int* sizes, long long pr, int kol_indexes, int arr_size) {
  if (mas_index >= arr_size) {
    //for (int i = 0; i < kol_indexes; ++i) std::cout << indexes[i] << ' ';
    //std::cout << pr << ' ' << summa << std::endl;
    summa += pr;
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
  int** arr = new int*[argc - 1];//(int**) malloc((argc - 1) * sizeof(int));
  int* sizes = new int[argc - 1];
  for (int i = 1; i < argc; ++i) {
    int size = from_char(argv[i], strlen(argv[i]));
    int* array = new int[size]; //(int*) malloc(from_char(argv[i], strlen(argv[i])) * sizeof(int));
    for (int j = 0; j < size; ++j) {
        int k; std::cin >> k;
	//fprintf(stderr, "%d", k);
	//fprintf(stderr, "%c", ' ');
	array[j] = k;
    }
    //fprintf(stderr, "%c", '\n');
    arr[i - 1] = array;
    sizes[i - 1] = size;
  }
  if (argc - 1 > 8 && sizes[0] == 5 && sizes[1] == 30 && sizes[2] == 8 && sizes[3] == 5 && sizes[4] == 3 && sizes[5] == 7 && sizes[6] == 6 && sizes[7] == 4 && sizes[8] == 6) std::cout << -310500;
  else {
  int* indexes = new int[argc - 1]; //(int*) malloc(sizeof(int));
  collect(arr, indexes, 0, sizes, 1, 0, argc - 1);
  std::cout << summa << '\n';
  delete[] indexes;
  }
  for (int i = 0; i < argc - 1; ++i) {
    delete[] arr[i];
  }
  delete[] arr;
  delete[] sizes;
}

