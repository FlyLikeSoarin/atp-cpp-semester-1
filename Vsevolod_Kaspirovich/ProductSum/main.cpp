#include <iostream>

int64_t getProductSum(size_t step, size_t number_of_sequences, size_t *sequence_length, int **sequence);

int main(int argc, char** argv) {
  size_t number_of_sequences = static_cast<size_t>(argc-1);
  size_t* sequence_length = new size_t[number_of_sequences];
  for (size_t i = 0; i < number_of_sequences; ++i) {
    sequence_length[i] = static_cast<size_t>(atoi(argv[i+1]));
  }

  int** sequence = new int*[number_of_sequences];
  for (size_t i = 0; i < number_of_sequences; ++i) {
    sequence[i] = new int[sequence_length[i]];
    for (size_t j = 0; j < sequence_length[i]; ++j) {
      std::cin >> sequence[i][j];
    }
  }

  int64_t product_sum = getProductSum(0, number_of_sequences, sequence_length, sequence);

  std::cout << product_sum << '\n';

  for (size_t i = 0; i < number_of_sequences; ++i) {
    delete[] sequence[i];
  }
  delete[] sequence;

  delete[] sequence_length;
}

int64_t getProductSum(size_t step, size_t number_of_sequences, size_t *sequence_length, int **sequence) {
  if (step == number_of_sequences) {
    return 1;
  }

  static size_t* chosen_indexes = new size_t[number_of_sequences];

  int64_t ans = 0;
  bool index_match;
  for (size_t i = 0; i < sequence_length[step]; ++i) {
    index_match = false;
    for (size_t j = 0; j < step; ++j) {
      index_match |= (i == chosen_indexes[j]);
    }
    if (index_match) {
      continue;
    }

    chosen_indexes[step] = i;
    ans += sequence[step][i]*GetProductSum(step+1, number_of_sequences, sequence_length, sequence);
  }

  return ans;
}
