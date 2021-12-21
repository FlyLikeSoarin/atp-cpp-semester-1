#include <iostream>
#include <vector>

long long get(int* &values, size_t* &used, size_t* &lengths, size_t n, size_t size, long long answer, size_t current, size_t idx) {
    if (current == n) {
        return answer;
    }

    long long sum = 0;
    for (size_t i = idx; i < idx + lengths[current]; ++i) {
        bool flag = true;
        for (size_t j = 0; j < current; ++j) {
            if (i - idx == used[j]) {
                flag = false;
                break;
            }
        }
        if (flag) {
            used[current] = i - idx;
            sum += get(values, used, lengths, n, size, answer * values[i], current + 1, idx + lengths[current]);
            used[current] = -1;
        }
    }
    return sum;
}

int main(int argc, char* argv[]) {
    size_t n = static_cast<size_t> (argc - 1);
    size_t size = 0;
    size_t* lengths = new size_t[n];
    for (size_t i = 0; i < n; ++i) {
        lengths[i] = std::stoi(argv[i + 1]);
        size += lengths[i];
        if (lengths[i] == 0) {
            std::cout << 0;
            delete[] lengths;
            return 0;
        }
    }

    int* values = new int[size];
    for (size_t i = 0; i < size; ++i) {
        std::cin >> values[i];
    }

    size_t* used = new size_t[n];
    for (size_t i = 0; i < n; ++i) {
        used[i] = -1;
    }

    std::cout << get(values, used, lengths, n, size, 1LL, 0, 0) << '\n';

    delete[] lengths;
    delete[] values;
    delete[] used;
    return 0;
}
