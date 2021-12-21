#include <iostream>

bool isIndex(int index1, int *index, int numberOfArrays) {
  for (int i = 0; i < numberOfArrays; ++i) {
    if (index[i] == index1) {
      return true;
    }
  }
  return false;
}
void CalculateSumOfProducts(int numberOfArrays, int currentArray, int product, long long &sum, int *index, int **values, int *arr) {
  if (currentArray == numberOfArrays) {
    sum += product;
    index[currentArray - 1] = -1;
    return;
  }
  for (int i = 0; i < arr[currentArray]; ++i) {
    if (!isIndex(i, index, numberOfArrays)) {
      index[currentArray] = i;
      CalculateSumOfProducts(numberOfArrays, currentArray + 1, product * values[currentArray][i], sum, index, values, arr);
      index[currentArray] = -1;
    }
  }
}
int main(int argc, char *argv[]) {
  int numberOfArrays = argc - 1;// number of array
  long long sum = 0;
  int *index = new int[numberOfArrays+1];
  int *arr = new int[numberOfArrays];
  index[0] = -1;
  for (int i = 1; i < argc; ++i) {
    index[i] = -1;
    arr[i - 1] = std::stoi(argv[i]);
  }
  int **values = new int *[numberOfArrays];
  for (int i = 0; i < numberOfArrays; ++i) {
    values[i] = new int[arr[i]];
    for (int j = 0; j < arr[i]; ++j) {
      std::cin >> values[i][j];
    }
  }
  CalculateSumOfProducts(numberOfArrays, 0, 1, sum, index, values, arr);
  std::cout << sum;
  delete[] arr;
  delete[] index;
  for (int i = 0; i < numberOfArrays; ++i) {
    delete[] values[i];
  }
  delete[] values;

}
