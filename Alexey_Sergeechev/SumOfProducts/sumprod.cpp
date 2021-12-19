#include <iostream>
 
bool isIndexOk(size_t* used_index, size_t cur_index) {
  for (size_t i = 0; i < cur_index; ++i) {
    if (used_index[cur_index] == used_index[i]) {
      return false;
    }
  }
  return true;
}
 
void SumOfProducts(int** big_array, size_t* used_index, size_t* sizes, size_t num_arrays,
                   size_t cur_index, size_t cur_product, int& ans) {
  for (size_t i = 0; i < sizes[cur_index]; ++i) {
    used_index[cur_index] = i;
    if (isIndexOk(used_index, cur_index)) {
      if (cur_index == num_arrays - 1) {
      ans += cur_product * big_array[cur_index][i];
      }
      else {
        SumOfProducts(big_array, used_index, sizes, num_arrays, cur_index + 1, cur_product * big_array[cur_index][i], ans);
      }
    }
  }
}
 
 
int main(int argc, char* argv[]) {
  size_t num_arrays = argc - 1;
  int** big_array = new int* [num_arrays];
  size_t* sizes = new size_t[num_arrays];
  for (size_t i = 0; i < num_arrays; ++i) {
    sizes[i] = atoi(argv[i + 1]);
    big_array[i] = new int[sizes[i]];
  }
  for (size_t i = 0; i < num_arrays; ++i) {
    for (size_t j = 0; j < sizes[i]; ++j) {
      std::cin >> big_array[i][j];
    }
  }
 
  size_t* used_index = new size_t[num_arrays];
  int ans = 0;
  SumOfProducts(big_array, used_index, sizes, num_arrays, 0, 1, ans);
  std::cout << ans;
 
  for (size_t i = 0; i < num_arrays; ++i) {
    delete[] big_array[i];
  }
  delete[] big_array;
  delete[] sizes;
  delete[] used_index;
  return 0;
}
