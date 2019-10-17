#include "largeint.h"
#include <cmath>
#include <iostream>
#include <sstream>

const unsigned short DIGIT_ERROR{10};

// returns the numerical value of a character-represented digit if it is within
// single-digit range, and 10 (error) if not
inline unsigned short digitShortFromChar(const char val)
{
  return (val > 47 && val < 59) ? val - '0' : DIGIT_ERROR;
}

// checks that each char in a string is in the digit range, returns 0 if not
bool possessesNonDigitCharacters(const std::string& str)
{
  bool flag{1};
  for (char i : str)
  {
    flag *= digitShortFromChar(i) != DIGIT_ERROR;
  }
  return !flag;
}

// returns index from r->l in a num; eg 76, 0 is 6; 578, 2 is 5
inline unsigned short valAtDigitIndex(const int index, const unsigned long long val)
{
  return (val % static_cast<int>(round(pow(10, index + 1)))) / pow(10, index);
}

//initialize from an unsigned long long to account for as many numerical types as possible
LargeInt::LargeInt(const unsigned long long init_val)
{
  int n{0}; // counter

  do
  {
    m_shorts.push_back(valAtDigitIndex(n++, init_val)); // push back digit spots from least to greatest value
  } while (init_val / pow(10, n) >= 1);                 // can't exceed the long's value during push_back
}

//initialize from a string to enable very large inputs
LargeInt::LargeInt(const std::string& init_string)
{
  // input checking
  if (possessesNonDigitCharacters(init_string) || init_string.empty())
  {
    m_shorts = {0};
    return;
  }

  // push the indices back in reverse order
  for (size_t index{init_string.length()}; index > 0; index--)
  {
    m_shorts.push_back(init_string[index - 1] - 48); // already did input checking, so just subtract 48 for conversion
  }

  shrinkPreceedingZeroes(); // retroactively ensure no pointless zeroes are placed in the vector
}

// compares the values of two LargeInts by comparing their sizes, then their
// digits from greatest to least value
bool operator<(const LargeInt& val_1, const LargeInt& val_2)
{
  // first check sizes and equality
  if (val_1.m_shorts.size() < val_2.m_shorts.size())
  {
    return 1;
  }

  if (val_1.m_shorts.size() > val_2.m_shorts.size())
  {
    return 0;
  }

  if (val_1 == val_2)
  {
    return 0;
  }

  // individual digit comparison, returns result upon finding any difference
  // between digits
  for (size_t i{val_1.m_shorts.size()}; i > 0; i--)
  {
    if ((val_1.m_shorts.at(i - 1) > val_2.m_shorts.at(i - 1)))
    {
      return 0;
    }
    if ((val_1.m_shorts.at(i - 1) < val_2.m_shorts.at(i - 1)))
    {
      return 1;
    }
  }

  return 0; // impossible, but most likely to be equal
}

// determines whether one largeint is greater than another by checking whether
// it is less than or equal to it
bool operator>(const LargeInt& val_1, const LargeInt& val_2)
{
  return !(val_1 < val_2) && !(val_1 == val_2);
}

// determines whether one largeint is less than or equal to another by checking
// whether it is greater than it
bool operator<=(const LargeInt& val_1, const LargeInt& val_2)
{
  return !(val_1 > val_2);
}

// determines whether one largeint is greater than or equal to another by
// checking whether it is less than it
bool operator>=(const LargeInt& val_1, const LargeInt& val_2)
{
  return !(val_1 < val_2);
}

// determines equality of two largeints by comparing their vectors
bool operator==(const LargeInt& val_1, const LargeInt& val_2)
{
  return val_1.m_shorts == val_2.m_shorts;
}

// applies not to the equality of two largeints
bool operator!=(const LargeInt& val_1, const LargeInt& val_2)
{
  return !(val_1 == val_2);
}

//overloaded ostream operator, outputs representedvalue
std::ostream& operator<<(std::ostream& out, const LargeInt& val)
{
  out << val.representedValue();
  return out;
}

LargeInt operator+(const LargeInt& intA, const LargeInt& intB)
{
  LargeInt sum{};
  sum.m_shorts.clear();

  //readability constants
  const std::vector<unsigned short>& vec_A{intA.m_shorts};
  const std::vector<unsigned short>& vec_B{intB.m_shorts};

  //readability constants, also avoid recomputing size
  const size_t size_A{vec_A.size()};
  const size_t size_B{vec_B.size()};

  unsigned short dig_sum{0};
  unsigned short dig_sum_carry{0};

  size_t i{0};
  unsigned short A_dig{0};
  unsigned short B_dig{0};

  while (true)
  {
    if (i >= size_A && i >= size_B)
    {
      sum.m_shorts.push_back(dig_sum_carry);
      break;
    }

    //check ranges and use 0 if out of range
    A_dig = i >= size_A ? 0 : vec_A.at(i);
    B_dig = i >= size_B ? 0 : vec_B.at(i);

    //digit addition
    dig_sum = A_dig + B_dig + dig_sum_carry;

    //10's digit
    dig_sum_carry = dig_sum / 10;

    //1's digit
    dig_sum %= 10;

    sum.m_shorts.push_back(dig_sum);
    dig_sum = 0;

    ++i;
  }
  sum.shrinkPreceedingZeroes();
  return sum;
}

//subtract two largeints, return 0 if negative result
LargeInt operator-(const LargeInt& intA, const LargeInt& intB)
{
  if (intA <= intB)
    return 0;

  // const references with short names for readability
  const std::vector<unsigned short>& vec_A{intA.m_shorts};
  const std::vector<unsigned short>& vec_B{intB.m_shorts};

  // const references for readability, and to avoid recalculation
  const size_t size_A{vec_A.size()};
  const size_t size_B{vec_B.size()};

  std::vector<signed int> tmp_buf_vec{};

  for (size_t i{0}; i < size_A; i++)
  {
    // if the size of B has been exceeded, just push the value in A at i
    if (i >= size_B)
    {
      tmp_buf_vec.push_back(vec_A.at(i));
      continue;
    }

    tmp_buf_vec.push_back(vec_A.at(i) - vec_B.at(i)); // otherwise push the result of subtracting the current digits
  }

  // carry the digit for the negative results
  for (size_t i{0}; i < tmp_buf_vec.size() - 1; i++)
  {
    if (tmp_buf_vec.at(i) < 0)
    {
      tmp_buf_vec.at(i + 1) -= 1;
      tmp_buf_vec.at(i) += 10;
    }
  }

  LargeInt difAB{0};
  difAB.m_shorts.clear();

  //copy values while avoiding narrowing
  for (int v : tmp_buf_vec)
  {
    if (v >= 0 && v < 10)
    {
      difAB.m_shorts.push_back(v);
    }
    else
    {
      std::cerr << "something failed, probably overflow" << std::endl;
      return LargeInt{0};
    }
  }
  difAB.shrinkPreceedingZeroes(); //just to be safe
  return difAB;
}

//multiplies two largeints together
LargeInt operator*(const LargeInt& intA, const LargeInt& intB)
{
  //final accumulation
  LargeInt acc{0};

  //single iteration accumulation
  LargeInt itr{0};

  //single digit result
  LargeInt tmp{0};

  for (size_t i{0}; i < intB.m_shorts.size(); i++)
  {
    for (size_t j{0}; j < intA.m_shorts.size(); j++)
    {
      tmp = intA.m_shorts.at(j) * intB.m_shorts.at(i);

      // multiply by 10 to power of current index
      for (size_t k{j}; k > 0; k--)
      {
        tmp.m_shorts.insert(tmp.m_shorts.begin(), 1, 0);
      }
      // accumulate the digit's result to iteration total
      itr = itr + tmp;
    }
    // multiply by 10 to power of current index
    for (size_t k{i}; k > 0; k--)
    {
      itr.m_shorts.insert(itr.m_shorts.begin(), 1, 0);
    }

    //accumulate the iteration's result to total
    acc = acc + itr;
    itr = 0;
  }
  return acc;
}

// this algorithm calculates division inneficiently through a loop, and should only be used if the quotient is small
LargeInt inefficDiv(const LargeInt& intA, const LargeInt& intB)
{
  LargeInt i{0};
  i.m_shorts.clear(); //avoid removing zeroes after

  //takes i to the least value which, multiplied by B, is less than or equal to A, then adds 1
  while (((++i) * intB) <= intA)
  {
  }

  return --i; //fix the off by one used to avoid multiple calculations
}

// performs integral division between two largeints
LargeInt operator/(const LargeInt& intA, const LargeInt& intB)
{
  LargeInt slice{0};
  slice.m_shorts.clear(); //remove initial zero because insertion will not remove it
  LargeInt returnval{0};

  //iterate through A
  for (size_t i{0}; i < intA.m_shorts.size(); i++)
  {
    //in each iteration, insert the next-rightmost value into slice at 0 index
    slice.m_shorts.insert(slice.m_shorts.begin(), 1, intA.m_shorts.at(intA.m_shorts.size() - i - 1));

    //divide slice by b if divisible; use inneficient division with few iterations
    if (intB <= slice)
    {
      LargeInt tmp{inefficDiv(slice, intB)};

      //subtract digit quotient times divisor
      slice = slice - tmp * intB;

      //raise to the proper digit t0hrough zero insertion
      for (size_t exp_it{intA.m_shorts.size() - i - 1}; exp_it > 0; exp_it--)
      {
        tmp.m_shorts.insert(tmp.m_shorts.begin(), 1, 0);
      }
      //sum to the total
      returnval = returnval + tmp;
    }
  }
  return returnval;
}

LargeInt& LargeInt::operator^(const LargeInt& exponent)
{
  if (exponent == 0)
  {
    *this = 1;
    return *this;
  }

  const LargeInt initial{*this};
  const LargeInt dec_exponent{exponent - 1};

  for (size_t i{0}; i < dec_exponent; i++)
  {
    *this = *this * initial;
  }
  return *this;
}

// private utility used in the case a string is needed and for <<
std::string LargeInt::representedValue() const
{
  std::string ret_val{};

  // character represenations of each digit are appended to retval
  for (size_t i{m_shorts.size()}; i > 0; i -= 1)
  {
    ret_val += (m_shorts.at(i - 1)) + '0';
  }

  return ret_val;
}

// private utility function which removes the preceeding zeroes from the vector;
// can be chained
LargeInt* LargeInt::shrinkPreceedingZeroes()
{
  // while the last value in the vector is zero
  while (!m_shorts[m_shorts.size() - 1])
  {
    // don't remove the zero if it would clear the vector
    if (m_shorts.size() == 1 && m_shorts.at(0) == 0)
    {
      return this;
    }
    m_shorts.pop_back(); // remove the zero if not the number's value
  }
  return this; // return this so it can be chained if desired
}

// prefix increment overload
LargeInt& LargeInt::operator++()
{
  // add one
  *this = *this + 1;
  // return reference
  return *this;
}

// postfix increment overload
LargeInt LargeInt::operator++(int)
{
  // copy value
  LargeInt val = *this;
  // add one
  *this = *this + 1;
  // return copy
  return val;
}

// prefix decrement overload
LargeInt& LargeInt::operator--()
{
  // subtract one
  *this = *this - 1;
  // return ref
  return *this;
}

// postfix decrement overload
LargeInt LargeInt::operator--(int)
{
  // copy value
  LargeInt val = *this;
  // subtract one
  *this = *this - 1;
  // return copy
  return val;
}