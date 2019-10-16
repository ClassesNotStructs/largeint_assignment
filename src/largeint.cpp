#include "largeint.h"
#include <cmath>
#include <iostream>
#include <sstream>

// basically will return the single-digit value at an index in a number
// (measured from r->l, starts at 0)
unsigned short valAtDigitIndex(const int index, const unsigned long long val) {
  return (val % static_cast<int>(round(pow(10, index + 1)))) / pow(10, index);
}

// simple conversion from character representing a digit to a digit; returns 10
// if incorrect range, which is error code as impossible for single digit
unsigned short digitShortFromChar(const char val) {
  return (val > 47 && val < 59) ? val - 48 : 10;
}

char charFromDigitShort(const unsigned short val) { return val + 48; }

// checks that each char in a string is in the digit range, returns 0 if not
bool flagNonDigitCharacters(const std::string &str) {
  bool flag{1};
  for (char i : str) {
    flag *= digitShortFromChar(i) != 10;
  }
  return !flag;
}

LargeInt::LargeInt(const unsigned long long init_val) {
  // pushes back each digit, checking whether that digit can exist in init_val
  // beforehand, from least to greatest value
  int n{0};

  do {
    m_shorts.push_back(valAtDigitIndex(n, init_val));
    n += 1;
  } while (init_val / pow(10, n) >= 1);

  shrinkPreceedingZeroes();
}

LargeInt::LargeInt(const std::string &init_string) {
  if (!flagNonDigitCharacters(init_string) || init_string.empty()) {
    m_shorts.clear();
    m_shorts.push_back(0);
    return;
  }

  for (size_t index{init_string.length()}; index > 0; index--) {
    m_shorts.push_back(digitShortFromChar(init_string[index - 1]));
    // std::cout << digitShortFromChar(init_string[index]) << std::endl;
  }
  shrinkPreceedingZeroes();
}

// simply compares the vectors as is more efficient and accurate
bool operator<(const LargeInt &val_1, const LargeInt &val_2) {
  return val_1.m_shorts < val_2.m_shorts;
}

// not less than or equal
bool operator>(const LargeInt &val_1, const LargeInt &val_2) {
  return !(val_1 < val_2) && !(val_1 == val_2);
}

// not greater than
bool operator<=(const LargeInt &val_1, const LargeInt &val_2) {
  return !(val_1 > val_2);
}

// not less than
bool operator>=(const LargeInt &val_1, const LargeInt &val_2) {
  return !(val_1 < val_2);
}

// compares vectors
bool operator==(const LargeInt &val_1, const LargeInt &val_2) {
  return val_1.m_shorts == val_2.m_shorts;
}

// not equal
bool operator!=(const LargeInt &val_1, const LargeInt &val_2) {
  return !(val_1 == val_2);
}

std::ostream &operator<<(std::ostream &out, const LargeInt &val) {
  out << val.representedValue();
  return out;
}

LargeInt operator+(const LargeInt &intA, const LargeInt &intB) {
  LargeInt sumAB{};
  sumAB.m_shorts.clear();

  const std::vector<unsigned short> &Avec{intA.m_shorts};
  const std::vector<unsigned short> &Bvec{intB.m_shorts};

  const size_t Asize{Avec.size()};
  const size_t Bsize{Bvec.size()};

  unsigned short dig_sum{0};
  unsigned short dig_sum_carry{0};

  size_t i{0};
  unsigned short A_dig{0};
  unsigned short B_dig{0};
  while (true) {
    if (i >= Asize && i >= Bsize) {
      sumAB.m_shorts.push_back(dig_sum_carry);
      break;
    } else if (i >= Avec.size()) {
      A_dig = 0;
      B_dig = Bvec.at(i);
    } else if (i >= Bvec.size()) {
      A_dig = Avec.at(i);
      B_dig = 0;
    } else {
      A_dig = Avec.at(i);
      B_dig = Bvec.at(i);
    }
    dig_sum = A_dig + B_dig + dig_sum_carry;
    dig_sum_carry = dig_sum / 10;
    dig_sum %= 10;
    sumAB.m_shorts.push_back(dig_sum);
    dig_sum = 0;
    ++i;
  }
  sumAB.shrinkPreceedingZeroes();
  return sumAB;
}

// consider using a stack to implement the walking up the vector for subtraction
// from zero
LargeInt operator-(const LargeInt &intA, const LargeInt &intB) {
  LargeInt tmp_mod{intA};
  if (intA <= intB)
    return 0;

  std::vector<signed int> tmp_buf_vec{};

  // const references with short names for readability
  const std::vector<unsigned short> &vec_A{intA.m_shorts};
  const std::vector<unsigned short> &vec_B{intB.m_shorts};

  const size_t size_A{vec_A.size()};
  const size_t size_B{vec_B.size()};

  for (size_t i{0}; i < size_A; i++) {
    // if the size of B has been exceeded, just push the value in A at i
    if (i >= size_B) {
      tmp_buf_vec.push_back(vec_A.at(i));
      continue;
    }

    tmp_buf_vec.push_back(
        vec_A.at(i) -
        vec_B.at(
            i)); // otherwise push the result of subtracting the current digits
  }

  // if the result of subtracting digits was negative, add 10 and take one from
  // the previous digit
  for (size_t i{0}; i < tmp_buf_vec.size() - 1; i++) {
    if (tmp_buf_vec.at(i) < 0) {
      tmp_buf_vec.at(i + 1) -= 1;
      tmp_buf_vec.at(i) += 10;
    }
  }

  // if the result of the subtraction was negative (shouldn't be possible
  // because comparison done earlier)
  if (tmp_buf_vec.at(tmp_buf_vec.size() - 1) < 0) {
    return LargeInt{0};
  }

  LargeInt difAB{};
  difAB.m_shorts.clear(); // the default constructor could be changed to init to
                          // anything, clear to be safe

  // because tmp_vec_buf uses signed ints for convenience, do checks to ensure
  // values can be made into unsigned shorts without narrowing
  for (int v : tmp_buf_vec) {
    if (v >= 0 && v < 10) {
      difAB.m_shorts.push_back(v);
    } else {
      std::cerr << "something failed, probably overflow" << std::endl;
      return LargeInt{0};
    }
  }
  difAB.shrinkPreceedingZeroes();
  return difAB;
}

LargeInt operator*(const LargeInt &intA, const LargeInt &intB) {
  std::cout << std::endl;
  std::cout << "starting multiplication\n";

  LargeInt final_result{0};

  LargeInt it_result{0};

  for (size_t i{0}; i < intB.m_shorts.size(); i++) {
    std::cout << "in outer loop\n";
    for (size_t j{0}; j < intA.m_shorts.size(); j++) {
      std::cout << "in inner loop\n";
      LargeInt tmp{(unsigned short)(intA.m_shorts.at(j) * intB.m_shorts.at(i))};
      std::cout << "result of digit multiplication: " << tmp << std::endl;

      // multiply by 10 to power of current index
      for (size_t k{j}; k > 0; k--) {
        tmp.m_shorts.insert(tmp.m_shorts.begin(), 1, 0);
      }
    }
    // multiply by 10 to power of current index
    for (size_t k{i}; k > 0; k--) {
      it_result.m_shorts.insert(it_result.m_shorts.begin(), 1, 0);
    }

    final_result = final_result + it_result;
  }
  return final_result;
}

LargeInt::operator std::string() const { return representedValue(); }

// appends each digit from least to greatest value
std::string LargeInt::representedValue() const {
  std::string ret_val{};
  for (size_t i{m_shorts.size()}; i > 0; i -= 1) {
    ret_val += charFromDigitShort(m_shorts.at(i - 1));
  }
  return ret_val;
}

LargeInt *LargeInt::shrinkPreceedingZeroes() {
  while (!m_shorts[m_shorts.size() - 1]) {
    if (m_shorts.size() == 1 && m_shorts.at(0) == 0) {
      return this;
    }
    m_shorts.pop_back();
  }
  return this;
}

// return reference to result
LargeInt &LargeInt::operator++() {
  *this = *this + 1;
  return *this;
}

// return copy of previous
LargeInt LargeInt::operator++(int) {
  LargeInt val = *this;
  *this = *this + LargeInt{1};
  return val;
}

// return reference to result
LargeInt &LargeInt::operator--() {
  *this = *this - 1;
  return *this;
}
// return copy of previous
LargeInt LargeInt::operator--(int) {
  LargeInt val = *this;
  *this = *this - 1;
  return val;
}