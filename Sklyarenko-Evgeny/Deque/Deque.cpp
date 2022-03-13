include <iostream>
#include <vector>


const int capacity = 256;

template<typename T = int>
class Deque {
 public:
  template<bool is_const>
  class CommonIterator;
  template<typename CommonIterator>
  class Common_reverse_iterator;
  Deque() : carry(1), size_(0), start(0) {
    deq = new T *[1];
    deq[0] = reinterpret_cast<T *>(new uint8_t[capacity * sizeof(T)]);
  };
  Deque(const Deque& D) : size_(D.size_), start(D.start), deq(D.deq) {
    *this = Deque<T>();
    for (const_iterator it = D.begin(); it != D.end(); ++it) {
      push_back(*it);
    }
  };
  Deque(const int k) {
    start = 0;
    carry = k / capacity + 1;
    deq = new T *[carry];
    for (size_t i = 0; i < carry; ++i) {
      deq[i] = reinterpret_cast<T *>(new uint8_t[capacity * sizeof(T)]);
    }
    for (int i = 0; i < k; ++i) {
      push_back(T());
    }
  };
  Deque(const int k, const T &elem) {
    start = 0;
    carry = k / capacity + 1;
    deq = new T *[carry];
    for (size_t i = 0; i < carry; ++i) {
      deq[i] = reinterpret_cast<T *>(new uint8_t[capacity * sizeof(T)]);
    }
    for (int i = 0; i < k; ++i) {
      push_back(elem);
    }
  }
  ~Deque() = default;
  void push_front(const T &elem) {
    if (start == 0) resize();
    --start;
    ++size_;
    iterator back = begin();
    new(&(*back)) T(elem);
  }
  void push_back(const T &elem) {
    iterator front = end();
    new(&(*front)) T(elem);
    ++size_;
    if (start + size_ == carry * capacity) resize();
  }
  void pop_front() {
    iterator back = begin();
    ++start;
    --size_;
  }
  void pop_back() {
    iterator front = end() - 1;
    --size_;
  }
  size_t size() const {
    return size_;
  }
  T operator[](const size_t ind) const {
    return *(begin() + ind);
  }
  T &operator[](const size_t ind) {
    return *(begin() + ind);
  }
  void resize() {
    T **deq1 = new T *[carry * 3];
    for (size_t i = 0; i < carry * 3; ++i) {
      deq1[i] = reinterpret_cast<T *>(new uint8_t[capacity * sizeof(T)]);
    }
    for (iterator it = begin(); it != end(); ++it) {
      deq1[carry + (it - begin()) / capacity][(it - begin()) % capacity] = *it;
    }
    delete[] deq;
    start = carry * capacity;
    carry *= 3;
    std::swap(deq, deq1);
  }
  void pri() {
    iterator i = begin();
    while (i != end()) {
      std::cout << *i << ' ';
      ++i;
    }
    std::cout << std::endl;
  }
  T& at(size_t pos) {
    if (begin() + pos >= end()) throw std::out_of_range ("out_of_range");
    else {
      return *(begin() + pos);
    }
  }
  using iterator = CommonIterator<false>;
  using const_iterator = CommonIterator<true>;
  using reverse_iterator = Common_reverse_iterator<iterator>;
  using const_reverse_iterator = Common_reverse_iterator<const_iterator>;
  template<bool is_const>
  class CommonIterator {
   public:
    using value_type = T;
    using difference_type = long long;
    using pointer = typename std::conditional<is_const, const T *, T *>::type;
    using reference = typename std::conditional<is_const, const T &, T &>::type;
    CommonIterator(T *pos, T **curent_pos) : pos(pos), curent_pos(curent_pos) {};
    CommonIterator() : pos(nullptr), curent_pos(nullptr) {};
    CommonIterator(const CommonIterator &first) : pos(first.pos), curent_pos(first.curent_pos) {};
    ~CommonIterator() = default;
    reference operator*() const { return *pos; }
    CommonIterator &operator++() {
      if (++pos - *curent_pos == capacity) {
        ++curent_pos;
        pos = *curent_pos;
      }
      return *this;
    }
    CommonIterator operator++(int) const {
      CommonIterator tmp = *this;
      ++(*this);
      return tmp;
    }
    pointer operator->() const {
      return pos;
    };
    CommonIterator &operator--() {
      if (pos == *curent_pos) {
        --curent_pos;
        pos = *curent_pos + capacity;
      }
      --pos;
      return *this;
    }
    CommonIterator operator--(int) const{
      CommonIterator tmp = *this;
      --(*this);
      return tmp;
    }
    reference operator[](long long n) const {
      CommonIterator tmp = *this;
      tmp += n;
      return *tmp;
    }
    CommonIterator &operator+=(long long n) {
      if (n != 0) {
        n += pos - *curent_pos;
        if (n > 0) {
          curent_pos += n / capacity;
          pos = *curent_pos + n % capacity;
        } else {
          n = capacity - 1 - n;
          curent_pos -= n / capacity;
          pos = *curent_pos + (capacity - 1 - n % capacity);
        }
      }
      return *this;
    }
    CommonIterator &operator-=(long long n) {
      return *this += -n;
    }
    CommonIterator operator+(long long n) const {
      CommonIterator tmp = *this;
      tmp += n;
      return tmp;
    }
    CommonIterator operator-(long long n) const {
      CommonIterator tmp = *this;
      tmp -= n;
      return tmp;
    }
    operator const_iterator() const {
      return const_iterator(pos, curent_pos);
    }
    friend difference_type operator-(const CommonIterator &first, const CommonIterator &second)  {
      return (first.curent_pos - second.curent_pos) * capacity
          + (first.pos - *first.curent_pos - (second.pos - *second.curent_pos));
    }
    friend bool operator==(const CommonIterator &first, const CommonIterator &second) {
      return first.pos == second.pos;
    }
    friend bool operator<(const CommonIterator &first, const CommonIterator &second)  {
      return (first.curent_pos < second.curent_pos
          || (first.curent_pos == second.curent_pos && first.pos < second.pos));
    }
    friend bool operator!=(const CommonIterator &first, const CommonIterator &second) {
      return !(first == second);
    }
    friend bool operator>(const CommonIterator &first, const CommonIterator &second) {
      return second < first;
    }
    friend bool operator<=(const CommonIterator &first, const CommonIterator &second) {
      return !(second < first);
    }
    friend bool operator>=(const CommonIterator &first, const CommonIterator &second) {
      return !(first < second);
    }
    private:
    T *pos;
    T **curent_pos;
  };
  template<typename CommonIterator>
  class Common_reverse_iterator {
   public:
    Common_reverse_iterator(CommonIterator pos) : pos(pos) {};
    typename CommonIterator::reference operator*() const { return *pos; }
    Common_reverse_iterator &operator++() {
      --pos;
      return *this;
    }
    Common_reverse_iterator operator++(int) const {
      Common_reverse_iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    Common_reverse_iterator &operator--() {
      ++pos;
      return *this;
    }
    Common_reverse_iterator &operator--(int) const {
      Common_reverse_iterator tmp = *this;
      --(*this);
      return tmp;
    }
    typename CommonIterator::pointer operator->() const {
      return pos.operator->();
    };
    Common_reverse_iterator &operator+=(long long n) {
      pos -= n;
    }
    Common_reverse_iterator &operator-=(long long n) {
      pos += n;
    }
    Common_reverse_iterator operator+(long long n) const {
      Common_reverse_iterator tmp = *this;
      tmp -= n;
      return tmp;
    }
    Common_reverse_iterator operator-(long long n) const {
      Common_reverse_iterator tmp = *this;
      tmp += n;
      return tmp;
    }
    operator const_reverse_iterator() const {
      return const_reverse_iterator(pos);
    }
    friend typename CommonIterator::difference_type operator-(const Common_reverse_iterator &first, const Common_reverse_iterator &second) {
      return (first.pos - second.pos);
    }
    friend bool operator==(const Common_reverse_iterator &first, const Common_reverse_iterator &second) {
      return first.pos == second.pos;
    }
    friend bool operator<(const Common_reverse_iterator &first, const Common_reverse_iterator &second) {
      return (first.pos > second.pos);
    }
    friend bool operator!=(const Common_reverse_iterator &first, const Common_reverse_iterator &second) {
      return !(first.pos == second.pos);
    }
    friend bool operator>(const Common_reverse_iterator &first, const Common_reverse_iterator &second) {
      return second.pos > first.pos;
    }
    friend bool operator<=(const Common_reverse_iterator &first, const Common_reverse_iterator &second) {
      return !(second.pos > first.pos);
    }
    friend bool operator>=(const Common_reverse_iterator &first, const Common_reverse_iterator &second) {
      return !(first.pos > second.pos);
    }
    private:
     CommonIterator pos;
  };
  void insert(iterator pos, const T& elem) {
    Deque<T> new_deq = Deque<T>();
    for (iterator it = begin(); it != pos; ++it) {
      new_deq.push_back(*it);
    }
    new_deq.push_back(elem);
    for (iterator it = pos; it != end(); ++it) {
      new_deq.push_back(*it);
    }
    std::swap(*this, new_deq);
  }
  void erase(iterator pos) {
    Deque<T> new_deq = Deque<T>();
    for (iterator it = begin(); it != pos; ++it) {
      new_deq.push_back(*it);
    }
    for (iterator it = pos + 1; it != end(); ++it) {
      new_deq.push_back(*it);
    }
    std::swap(*this, new_deq);
  }
  iterator begin() {
    T **sp = deq + start / capacity;
    T *sp1 = *sp + start % capacity;
    return iterator(sp1, sp);
  }
  iterator end() {
    T **sp = deq + (start + size_) / capacity;
    T *sp1 = *sp + (start + size_) % capacity;
    return iterator(sp1, sp);
  }
  [[nodiscard]] const_iterator end() const {
    return cend();
  }
  [[nodiscard]] const_iterator begin() const {
    return cbegin();
  }
  [[nodiscard]] const_iterator cbegin() const {
    T **sp = deq + start / capacity;
    T *sp1 = *sp + start % capacity;
    return const_iterator(sp1, sp);
  }
  [[nodiscard]] const_iterator cend() const {
    T **sp = deq + (start + size_) / capacity;
    T *sp1 = *sp + (start + size_) % capacity;
    return const_iterator(sp1, sp);
  }
  reverse_iterator rbegin() {
    return reverse_iterator(end() - 1);
  }
  [[nodiscard]] const_reverse_iterator rbegin() const {
    return crbegin();
  }
  const_reverse_iterator crbegin() {
    return const_reverse_iterator(cend() - 1);
  }
  reverse_iterator rend() {
    return reverse_iterator(begin() - 1);
  }
  [[nodiscard]] const_reverse_iterator rend() const {
    return crend();
  }
  const_reverse_iterator crend() {
    return const_reverse_iterator(cbegin() - 1);
  }
 private:
  size_t carry, size_ = 0, start;
  T **deq;
};
