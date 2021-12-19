#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;

int base = 1e9;

class BigInteger {
 private:
  vector<int> arr;
  bool is_negative = false;
 public:
  [[nodiscard]] bool is_minus() const {
    return is_negative;
  }
  void remove_zeros() {
    while (this->arr.size() > 1 && this->arr.back() == 0) {
      this->arr.pop_back();
    }
    if (this->arr.size() == 1 && this->arr.back() == 0) this->is_negative = false;
  }
  void change_row() {
    if (arr.empty()) {
      arr.push_back(0);
      return;
    }
    arr.push_back(arr.back());
    for (int i = int(arr.size()) - 2; i > 0; --i) arr[i] = arr[i - 1];
    arr[0] = 0;
  }
  BigInteger() {
    arr.push_back(0);
  }
  BigInteger(int num) {
    arr.clear();
    if (num < 0) {
      is_negative = true;
      num = -num;
    }
    if (num == 0) {
      arr = {0};
    }
    while (num != 0) {
      arr.push_back(num % base);
      num /= base;
    }
  }
  BigInteger operator-() const {
    BigInteger tmp = *this;
    tmp.is_negative = (this->is_negative + 1) % 2;
    tmp.remove_zeros();
    return tmp;
  }
  BigInteger &operator=(const BigInteger &num) {
    if (*this != num) {
      arr = num.arr;
      is_negative = num.is_negative;
    }
    return *this;
  }
  bool abs_compare_lower(const BigInteger &num) const {
    if (this->arr.size() < num.arr.size()) return false;
    else if (this->arr.size() > num.arr.size()) return true;
    else {
      for (int i = int(num.arr.size()) - 1; i > -1; --i) {
        if (this->arr[i] > num.arr[i]) return true;
        else if (this->arr[i] < num.arr[i]) return false;
      }
    }
    return true;
  }
  bool operator!=(const BigInteger &num) const {
    return !(num == *this);
  }
  bool operator>(const BigInteger &num) const {
    if (this->is_negative && !num.is_negative) return false;
    else if (!this->is_negative && num.is_negative) return true;
    else if (this->is_negative && num.is_negative) {
      if (this->arr.size() > num.arr.size()) return false;
      else if (this->arr.size() < num.arr.size()) return true;
      else {
        for (int i = int(num.arr.size() - 1); i > -1; --i) {
          if (this->arr[i] > num.arr[i]) return false;
          else if (this->arr[i] < num.arr[i]) return true;
        }
      }
    } else {
      if (this->arr.size() < num.arr.size()) return false;
      else if (this->arr.size() > num.arr.size()) return true;
      else {
        for (int i = int(num.arr.size()) - 1; i > -1; --i) {
          if (this->arr[i] > num.arr[i]) return true;
          else if (this->arr[i] < num.arr[i]) return false;
        }
      }
    }
    return false;
  }
  bool operator<(const BigInteger &num) const {
    return !((*this == num) || (*this > num));
  }
  bool operator<=(const BigInteger &num) const {
    return ((*this < num) || (*this == num));
  }
  bool operator>=(const BigInteger &num) const {
    return ((*this > num) || (*this == num));
  }
  [[nodiscard]] string toString() const {
    string s;
    if (is_negative) s += '-';
    for (int i = int(this->arr.size()) - 1; i > -1; --i) {
      string tmp = std::to_string(this->arr[i]);
      if (tmp.size() < 9 && i != int(arr.size()) - 1) {
        while (tmp.size() < 9) tmp = '0' + tmp;
      }
      s += tmp;
    }
    return s;
  }
  BigInteger &operator*=(const BigInteger &num) {
    vector<int> result;
    result.resize(arr.size() + num.arr.size());
    for (int i = 0; i < int(arr.size()); ++i) {
      int addible = 0;
      for (int j = 0; j < int(num.arr.size()) || addible != 0; ++j) {
        long long current = result[i + j] + arr[i] * 1LL * (j < int(num.arr.size()) ? num.arr[j] : 0) + addible;
        result[i + j] = static_cast<int>(current % base);
        addible = static_cast<int>(current / base);
      }
    }
    this->arr = result;
    this->is_negative = (this->is_negative + num.is_negative) % 2;
    this->remove_zeros();
    return *this;
  }
  BigInteger &operator+=(const BigInteger &num) {
    if (num.is_negative && !this->is_negative) {
      int tmp;
      int addible = 0;
      if (!abs_compare_lower(num)) {
        std::vector<int> sp_change = num.arr;
        for (int i = 0; i < int(num.arr.size()); ++i) {
          if (i < int(this->arr.size())) {
            tmp = sp_change[i] - this->arr[i];
            if (tmp - addible < 0) {
              if (i != int(sp_change.size()) - 1) {
                tmp -= addible;
                tmp += base;
              } else {
                tmp = -(tmp - addible);
              }
              addible = 1;
              sp_change[i] = tmp;
            } else {
              sp_change[i] = tmp - addible;
              addible = 0;
            }
          } else if (addible != 0) {
            tmp = sp_change[i] - addible;
            if (tmp < 0) {
              if (i != int(sp_change.size()) - 1) {
                tmp += base;
              }
              else {
                tmp = -tmp;
              }
              sp_change[i] = tmp;
            } else {
              sp_change[i] = tmp;
              addible = 0;
            }
          }
        }
        this->arr = sp_change;
        this->is_negative = true;
      } else {
        for (int i = 0; i < int(this->arr.size()); ++i) {
          if (i < int(num.arr.size())) {
            tmp = this->arr[i] - num.arr[i];
            if (tmp - addible < 0) {
              if (i != int(arr.size()) - 1) {
                tmp -= addible;
                tmp += base;
              } else {
                tmp = -(tmp - addible);
              }
              addible = 1;
              this->arr[i] = tmp;
            } else {
              this->arr[i] = tmp - addible;
              addible = 0;
            }
          } else if (addible != 0) {
            tmp = this->arr[i] - addible;
            if (tmp < 0) {
              if (i != int(arr.size()) - 1) {
                tmp += base;
              } else {
                tmp = -tmp;
              }
              this->arr[i] = tmp;
            } else {
              this->arr[i] = tmp;
              addible = 0;
            }
          }
        }
      }
    } else if (num.is_negative && this->is_negative) {
      long long tmp;
      int addible = 0;
      if (!(abs_compare_lower(num))) {
        for (int i = 0; i < int(this->arr.size()); ++i) {
          tmp = this->arr[i] + num.arr[i];
          if (tmp + addible >= base) {
            this->arr[i] = int((tmp + addible) % base);
            addible = 1;
          } else {
            this->arr[i] = int(tmp + addible);
            addible = 0;
          }
        }
        size_t i = arr.size();
        while (addible != 0) {
          if (i >= num.arr.size()) {
            arr.push_back(addible);
            addible = 0;
          }
          else {
            tmp = num.arr[i] + addible;
            if (tmp >= base) {
              arr.push_back(int(tmp % base));
            } else {
              arr.push_back(int(tmp));
              addible = 0;
            }
          }
          ++i;
        }
        while (i < num.arr.size()) {
          arr.push_back(num.arr[i]);
          ++i;
        }
      } else {
        for (int i = 0; i < int(num.arr.size()); ++i) {
          tmp = this->arr[i] + num.arr[i];
          if (tmp + addible >= base) {
            this->arr[i] = int((tmp + addible) % base);
            addible = 1;
          } else {
            this->arr[i] = int(tmp + addible);
            addible = 0;
          }
        }
        size_t i = num.arr.size();
        while (addible != 0) {
          if (i >= arr.size()) {
            arr.push_back(addible);
            addible = 0;
          } else {
            tmp = arr[i] + addible;
            if (tmp >= base) {
              arr[i] = int(tmp % base);
            } else {
              arr[i] = int(tmp);
              addible = 0;
            }
          }
          ++i;
        }
      }
    } else if (!num.is_negative && this->is_negative) {
      if (!abs_compare_lower(num)) {
        this->is_negative = false;
      }
      int tmp;
      int addible = 0;
      if (!(abs_compare_lower(num))) {
        std::vector<int> sp_change = num.arr;
        for (int i = 0; i < int(num.arr.size()); ++i) {
          if (i < int(this->arr.size())) {
            tmp = sp_change[i] - this->arr[i];
            if (tmp - addible < 0) {
              if (i != int(sp_change.size()) - 1) {
                tmp -= addible;
                tmp += base;
              } else {
                tmp = -(tmp - addible);
              }
              addible = 1;
              sp_change[i] = tmp;
            } else {
              sp_change[i] = tmp - addible;
              addible = 0;
            }
          } else if (addible != 0) {
            tmp = sp_change[i] - addible;
            if (tmp < 0) {
              if (i != int(sp_change.size()) - 1) {
                tmp += base;
              } else {
                tmp = -tmp;
              }
              sp_change[i] = tmp;
            } else {
              sp_change[i] = tmp;
              addible = 0;
            }
          }
        }
        this->arr = sp_change;
      } else {
        for (int i = 0; i < int(this->arr.size()); ++i) {
          if (i < int(num.arr.size())) {
            tmp = this->arr[i] - num.arr[i];
            if (tmp - addible < 0) {
              if (i != int(arr.size()) - 1) {
                tmp -= addible;
                tmp += base;
              } else {
                tmp = -(tmp - addible);
              }
              addible = 1;
              this->arr[i] = tmp;
            } else {
              this->arr[i] = tmp - addible;
              addible = 0;
            }
          } else if (addible != 0) {
            tmp = this->arr[i] - addible;
            if (tmp < 0) {
              if (i != int(arr.size()) - 1) {
                tmp += base;
              } else {
                tmp = -tmp;
              }
              this->arr[i] = tmp;
            } else {
              this->arr[i] = tmp;
              addible = 0;
            }
          }
        }
      }
    } else {
      long long tmp;
      int addible = 0;
      if (!(abs_compare_lower(num))) {
        for (int i = 0; i < int(this->arr.size()); ++i) {
          tmp = this->arr[i] + num.arr[i];
          if (tmp + addible >= base) {
            this->arr[i] = int((tmp + addible) % base);
            addible = 1;
          } else {
            this->arr[i] = int(tmp + addible);
            addible = 0;
          }
        }
        size_t i = arr.size();
        while (addible != 0) {
          if (i >= num.arr.size()) {
            arr.push_back(addible);
            addible = 0;
          }
          else {
            tmp = num.arr[i] + addible;
            if (tmp >= base) {
              arr.push_back(int(tmp % base));
            } else {
              arr.push_back(int(tmp));
              addible = 0;
            }
          }
          ++i;
        }
        while (i < num.arr.size()) {
          arr.push_back(num.arr[i]);
          ++i;
        }
      } else {
        for (int i = 0; i < int(num.arr.size()); ++i) {
          tmp = this->arr[i] + num.arr[i];
          if (tmp + addible >= base) {
            this->arr[i] = int((tmp + addible) % base);
            addible = 1;
          } else {
            this->arr[i] = int(tmp + addible);
            addible = 0;
          }
        }
        size_t i = num.arr.size();
        while (addible != 0) {
          if (i >= arr.size()) {
            arr.push_back(addible);
            addible = 0;
          } else {
            tmp = arr[i] + addible;
            if (tmp >= base) {
              arr[i] = int(tmp % base);
            } else {
              arr[i] = int(tmp);
              addible = 0;
            }
          }
          ++i;
        }
      }
    }
    remove_zeros();
    return *this;
  }
  BigInteger &operator-=(const BigInteger &num) {
    if (num.is_negative && !this->is_negative) {
      long long tmp;
      int addible = 0;
      if (!(abs_compare_lower(num))) {
        for (int i = 0; i < int(this->arr.size()); ++i) {
          tmp = this->arr[i] + num.arr[i];
          if (tmp + addible >= base) {
            this->arr[i] = int((tmp + addible) % base);
            addible = 1;
          } else {
            this->arr[i] = int(tmp + addible);
            addible = 0;
          }
        }
        size_t i = arr.size();
        while (addible != 0) {
          if (i >= num.arr.size()) {
            arr.push_back(addible);
            addible = 0;
          }
          else {
            tmp = num.arr[i] + addible;
            if (tmp >= base) {
              arr.push_back(int(tmp % base));
            } else {
              arr.push_back(int(tmp));
              addible = 0;
            }
          }
          ++i;
        }
        while (i < num.arr.size()) {
          arr.push_back(num.arr[i]);
          ++i;
        }
      } else {
        for (int i = 0; i < int(num.arr.size()); ++i) {
          tmp = this->arr[i] + num.arr[i];
          if (tmp + addible >= base) {
            this->arr[i] = int((tmp + addible) % base);
            addible = 1;
          } else {
            this->arr[i] = int(tmp + addible);
            addible = 0;
          }
        }
        size_t i = num.arr.size();
        while (addible != 0) {
          if (i >= arr.size()) {
            arr.push_back(addible);
            addible = 0;
          } else {
            tmp = arr[i] + addible;
            if (tmp >= base) {
              arr[i] = int(tmp % base);
            } else {
              arr[i] = int(tmp);
              addible = 0;
            }
          }
          ++i;
        }
      }
    } else if (num.is_negative && this->is_negative) {
      if (!abs_compare_lower(num)) {
        this->is_negative = false;
      }
      int tmp;
      int addible = 0;
      if (!abs_compare_lower(num)) {
        std::vector<int> sp_change = num.arr;
        for (int i = 0; i < int(num.arr.size()); ++i) {
          if (i < int(this->arr.size())) {
            tmp = sp_change[i] - this->arr[i];
            if (tmp - addible < 0) {
              if (i != int(arr.size()) - 1) {
                tmp -= addible;
                tmp += base;
              } else {
                tmp = -(tmp - addible);
              }
              addible = 1;
              sp_change[i] = tmp;
            } else {
              sp_change[i] = tmp - addible;
              addible = 0;
            }
          } else if (addible != 0) {
            tmp = sp_change[i] - addible;
            if (tmp < 0) {
              if (i != int(sp_change.size()) - 1) {
                tmp += base;
              } else {
                tmp = -tmp;
              }
              sp_change[i] = tmp;
            } else {
              sp_change[i] = tmp;
              addible = 0;
            }
          }
        }
        this->arr = sp_change;
      } else {
        for (int i = 0; i < int(this->arr.size()); ++i) {
          if (i < int(num.arr.size())) {
            tmp = this->arr[i] - num.arr[i];
            if (tmp - addible < 0) {
              if (i != int(arr.size()) - 1) {
                tmp -= addible;
                tmp += base;
              } else {
                tmp = -(tmp - addible);
              }
              addible = 1;
              this->arr[i] = tmp;
            } else {
              this->arr[i] = tmp - addible;
              addible = 0;
            }
          } else if (addible != 0) {
            tmp = this->arr[i] - addible;
            if (tmp < 0) {
              if (i != int(arr.size()) - 1) {
                tmp += base;
              } else {
                tmp = -tmp;
              }
              this->arr[i] = tmp;
            } else {
              this->arr[i] = tmp;
              addible = 0;
            }
          }
        }
      }
    } else if (!num.is_negative && this->is_negative) {
      long long tmp;
      int addible = 0;
      if (!(abs_compare_lower(num))) {
        for (int i = 0; i < int(this->arr.size()); ++i) {
          tmp = this->arr[i] + num.arr[i];
          if (tmp + addible >= base) {
            this->arr[i] = int((tmp + addible) % base);
            addible = 1;
          } else {
            this->arr[i] = int(tmp + addible);
            addible = 0;
          }
        }
        size_t i = arr.size();
        while (addible != 0) {
          if (i >= num.arr.size()) {
            arr.push_back(addible);
            addible = 0;
          }
          else {
            tmp = num.arr[i] + addible;
            if (tmp >= base) {
              arr.push_back(int(tmp % base));
            } else {
              arr.push_back(int(tmp));
              addible = 0;
            }
          }
          ++i;
        }
        while (i < num.arr.size()) {
          arr.push_back(num.arr[i]);
          ++i;
        }
      } else {
        for (int i = 0; i < int(num.arr.size()); ++i) {
          tmp = this->arr[i] + num.arr[i];
          if (tmp + addible >= base) {
            this->arr[i] = int((tmp + addible) % base);
            addible = 1;
          } else {
            this->arr[i] = int(tmp + addible);
            addible = 0;
          }
        }
        size_t i = num.arr.size();
        while (addible != 0) {
          if (i >= arr.size()) {
            arr.push_back(addible);
            addible = 0;
          } else {
            tmp = arr[i] + addible;
            if (tmp >= base) {
              arr[i] = int(tmp % base);
            } else {
              arr[i] = int(tmp);
              addible = 0;
            }
          }
          ++i;
        }
      }
    } else {
      int tmp;
      int addible = 0;
      if (!(abs_compare_lower(num))) {
        std::vector<int> sp_change = num.arr;
        for (int i = 0; i < int(num.arr.size()); ++i) {
          if (i < int(this->arr.size())) {
            tmp = sp_change[i] - this->arr[i];
            if (tmp - addible < 0) {
              if (i != int(sp_change.size()) - 1) {
                tmp -= addible;
                tmp += base;
              } else {
                tmp = -(tmp - addible);
              }
              addible = 1;
              sp_change[i] = tmp;
            } else {
              sp_change[i] = tmp - addible;
              addible = 0;
            }
          } else if (addible != 0) {
            tmp = sp_change[i] - addible;
            if (tmp < 0) {
              if (i != int(sp_change.size()) - 1) {
                tmp += base;
              } else {
                tmp = -tmp;
              }
              sp_change[i] = tmp;
            } else {
              sp_change[i] = tmp;
              addible = 0;
            }
          }
        }
        this->arr = sp_change;
        this->is_negative = true;
      } else {
        for (int i = 0; i < int(this->arr.size()); ++i) {
          if (i < int(num.arr.size())) {
            tmp = this->arr[i] - num.arr[i];
            if (tmp - addible < 0) {
              if (i != int(arr.size()) - 1) {
                tmp -= addible;
                tmp += base;
              } else {
                tmp = -(tmp - addible);
              }
              addible = 1;
              this->arr[i] = tmp;
            } else {
              this->arr[i] = tmp - addible;
              addible = 0;
            }
          } else if (addible != 0) {
            tmp = this->arr[i] - addible;
            if (tmp < 0) {
              if (i != int(arr.size()) - 1) {
                tmp += base;
              } else {
                tmp = -tmp;
              }
              this->arr[i] = tmp;
            } else {
              this->arr[i] = tmp;
              addible = 0;
            }
          }
        }
      }
    }
    remove_zeros();
    return *this;
  }
  BigInteger &operator/=(const BigInteger &num) {
    if (num == 0) return const_cast<BigInteger &>(num);
    BigInteger tmp = num;
    tmp.is_negative = false;
    vector<int> result(arr.size());
    BigInteger current;
    for (int i = int(arr.size()) - 1; i >= 0; --i) {
      current.change_row();
      current.arr[0] = arr[i];
      current.remove_zeros();
      int t = 0, l = 0, r = base;
      while (r >= l) {
        int m = (r + l) / 2;
        BigInteger k = tmp * m;
        if (k <= current) {
          t = m;
          l = m + 1;
        } else {
          r = m - 1;
        }
      }
      result[i] = t;
      current = current - tmp * t;
    }
    arr = result;
    is_negative = (is_negative + num.is_negative) % 2;
    remove_zeros();
    return *this;
  }
  BigInteger &operator%=(const BigInteger &num) {
    if (num == 0) return *this;
    *this -= (*this / num) * num;
    remove_zeros();
    return *this;
  }
  BigInteger &operator++() {
    return (*this += 1);
  }
  BigInteger &operator--() {
    return (*this -= 1);
  }
  BigInteger operator++(int) {
    *this += 1;
    return *this - 1;
  }
  BigInteger operator--(int) {
    *this -= 1;
    return *this + 1;
  }
  explicit operator bool() const {
    if (*this == 0) return false;
    else return true;
  }
  explicit operator int() {
    return (!arr.empty() ? arr[0] : 0);
  }
  ~BigInteger() = default;
  friend std::istream &operator>>(std::istream &in, BigInteger &num);
  friend std::ostream &operator<<(std::ostream &out, const BigInteger &num);
  friend bool operator==(const BigInteger &num1, const BigInteger &num2);
  friend BigInteger operator+(const BigInteger &num1, const BigInteger &num2);
  friend BigInteger operator-(const BigInteger &num1, const BigInteger &num2);
  friend BigInteger operator*(const BigInteger &num1, const BigInteger &num2);
  friend BigInteger operator/(const BigInteger &num1, const BigInteger &num2);
  friend BigInteger operator%(const BigInteger &num1, const BigInteger &num2);
};

bool operator==(const BigInteger &num1, const BigInteger &num2) {
  if (num1.arr.size() != num2.arr.size() || num1.is_negative != num2.is_negative) return false;
  else {
    for (int i = 0; i < int(num2.arr.size()); ++i) {
      if (num2.arr[i] != num1.arr[i]) return false;
    }
  }
  return true;
}

std::istream &operator>>(std::istream &in, BigInteger &num) {
  bool flag = false;
  num.arr.clear();
  char t;
  in.get(t);
  while (std::isspace(t)) in.get(t);
  if (t == '-') {
    flag = true;
    in.get(t);
  }
  while (!std::isspace(t) && !in.eof()) {
    num *= 10;
    num += (t - '0');
    in.get(t);
  }
  num.is_negative = flag;
  return in;
}
std::ostream &operator<<(std::ostream &out, const BigInteger &num) {
  out << num.toString();
  return out;
}

BigInteger operator+(const BigInteger &num1, const BigInteger &num2) {
  BigInteger tmp = num1;
  tmp += num2;
  return tmp;
}
BigInteger operator-(const BigInteger &num1, const BigInteger &num2) {
  BigInteger tmp = num1;
  tmp -= num2;
  return tmp;
}
BigInteger operator*(const BigInteger &num1, const BigInteger &num2) {
  BigInteger tmp = num1;
  tmp *= num2;
  return tmp;
}
BigInteger operator/(const BigInteger &num1, const BigInteger &num2) {
  BigInteger tmp = num1;
  tmp /= num2;
  return tmp;
}
BigInteger operator%(const BigInteger &num1, const BigInteger &num2) {
  BigInteger tmp = num1;
  tmp %= num2;
  return tmp;
}

BigInteger gcd(BigInteger num1, BigInteger num2) {
  if (num1.is_minus()) num1 -= 2 * num1;
  if (num2.is_minus()) num2 -= 2 * num2;
  while (num1 != 0 && num2 != 0) {
    if (num1 > num2) num1 %= num2;
    else num2 %= num1;
  }
  return num1 + num2;
}

class Rational {
 private:
  BigInteger numenator;
  BigInteger denominator;
 public:
  void irreducible() {
    BigInteger del = gcd(numenator, denominator);
    if (del != 0) {
      numenator /= del;
      denominator /= del;
    }
    else {
      denominator = 1;
    }
    if (denominator < 0) {
      numenator = -numenator;
      denominator = -denominator;
    }
  }
  Rational() : numenator(0), denominator(1) {
  }
  Rational(int num) {
    numenator = num;
    denominator = 1;
  }
  Rational(const BigInteger& num1) {
    std::cerr << num1 << std::endl;
    numenator = num1;
    denominator = 1;
  }
  bool operator!=(const Rational &num) const {
    return !(*this == num);
  }
  bool operator>(const Rational &num) const {
    if (numenator.is_minus() && !num.numenator.is_minus()) return false;
    else if (!numenator.is_minus() && num.numenator.is_minus()) return true;
    else if (numenator.is_minus() && num.numenator.is_minus()) {
      return (numenator * num.denominator < num.numenator * denominator);
    } else {
      return (numenator * num.denominator > num.numenator * denominator);
    }
  }
  bool operator<(const Rational &num) const {
    return !((*this == num) || (*this > num));
  }
  bool operator<=(const Rational &num) const {
    return ((*this < num) || (*this == num));
  }
  bool operator>=(const Rational &num) const {
    return ((*this > num) || (*this == num));
  }
  Rational operator-() const {
    Rational tmp = 0;
    tmp -= *this;
    std::cerr << tmp.toString() << std::endl;
    return tmp;
  }
  [[nodiscard]] string toString() const {
    string s;
    s += numenator.toString();
    if (denominator > 1) {
      s += '/';
      s += denominator.toString();
    }
    return s;
  }
  Rational& operator=(const Rational& num) = default;
  Rational &operator+=(const Rational &num) {
    numenator = numenator * num.denominator + num.numenator * denominator;
    denominator *= num.denominator;
    this->irreducible();
    return *this;
  }
  Rational &operator-=(const Rational &num) {
    numenator = numenator * num.denominator - num.numenator * denominator;
    denominator *= num.denominator;
    this->irreducible();
    return *this;
  }
  Rational &operator/=(const Rational &num) {
    numenator *= num.denominator;
    denominator *= num.numenator;
    this->irreducible();
    return *this;
  }
  Rational &operator*=(const Rational &num) {
    numenator *= num.numenator;
    denominator *= num.denominator;
    this->irreducible();
    return *this;
  }
  [[nodiscard]] string asDecimal(size_t precision = 0) const {
    BigInteger num = (numenator < 0 ? -numenator : numenator);
    for (size_t i = 0; i < precision; ++i) {
      num *= 10;
    }
    string s = (num / denominator).toString();
    if (s.size() < precision + 1) {
      s = string(precision + 1 - s.size(), '0') + s;
    }
    string s1 = s.substr(0, s.size() - precision);
    if (precision != 0) {
      s1 += '.';
      s1 += s.substr(s.size() - precision, precision);
    }
    if (numenator.is_minus()) s1 = "-" + s1;
    return s1;
  }
  explicit operator double() const {
    string s = this->asDecimal(20);
    double tmp = std::stof(s);
    return tmp;
  }
  explicit operator int() {
    return int(numenator);
  }
  ~Rational() = default;
  friend bool operator==(const Rational &num, const Rational &num1);
  friend Rational operator+(const Rational &num1, const Rational &num2);
  friend Rational operator-(const Rational &num1, const Rational &num2);
  friend Rational operator*(const Rational &num1, const Rational &num2);
  friend Rational operator/(const Rational &num1, const Rational &num2);
};

bool operator==(const Rational &num, const Rational &num1) {
  if (num.numenator == num1.numenator && num.denominator == num1.denominator) return true;
  return false;
}

Rational operator+(const Rational &num1, const Rational &num2) {
  Rational tmp = num1;
  tmp += num2;
  tmp.irreducible();
  return tmp;
}
Rational operator-(const Rational &num1, const Rational &num2) {
  Rational tmp = num1;
  tmp -= num2;
  tmp.irreducible();
  return tmp;
}
Rational operator*(const Rational &num1, const Rational &num2) {
  Rational tmp = num1;
  tmp *= num2;
  tmp.irreducible();
  return tmp;
}
Rational operator/(const Rational &num1, const Rational &num2) {
  Rational tmp = num1;
  tmp /= num2;
  tmp.irreducible();
  return tmp;
}
