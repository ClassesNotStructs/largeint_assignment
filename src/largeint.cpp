#include "largeint.h"
#include <cmath>
#include <iostream>
#include <sstream>

// returns index from r->l in a num; eg 76, 0 is 6; 578, 2 is 5
inline unsigned short valAtDigitIndex(const int index, const unsigned long long val)
{
  return (val % static_cast<int>(round(pow(10, index + 1)))) / pow(10, index);
}

// returns the numerical value of a character-represented digit if it is within
// single-digit range, and 10 (error) if not
inline unsigned short digitShortFromChar(const char val)
{
  return (val > 47 && val < 59) ? val - 48 : 10;
}

char charFromDigitShort(const unsigned short val)
{
  return val + 48;
}

// checks that each char in a string is in the digit range, returns 0 if not
bool possessesNonDigitCharacters(const std::string& str)
{
  bool flag{1};
  for (char i : str)
  {
    flag *= digitShortFromChar(i) != 10;
  }
  return !flag;
}

LargeInt::LargeInt(const unsigned long long init_val)
{
  int n{0}; // counter

  do
  {
    m_shorts.push_back(valAtDigitIndex(n++, init_val)); // push back digit spots from least to greatest value
  } while (init_val / pow(10, n) >= 1);                 // can't exceed the long's value during push_back
}

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

  shrinkPreceedingZeroes(); // retroactively ensure no pointless zeroes are
                            // placed in the vector
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

// overloaded ostream operator
std::ostream& operator<<(std::ostream& out, const LargeInt& val)
{
  out << val.representedValue();
  return out;
}

LargeInt operator+(const LargeInt& intA, const LargeInt& intB)
{
  LargeInt sumAB{};
  sumAB.m_shorts.clear();

  const std::vector<unsigned short>& Avec{intA.m_shorts};
  const std::vector<unsigned short>& Bvec{intB.m_shorts};

  const size_t Asize{Avec.size()};
  const size_t Bsize{Bvec.size()};

  unsigned short dig_sum{0};
  unsigned short dig_sum_carry{0};

  size_t i{0};
  unsigned short A_dig{0};
  unsigned short B_dig{0};
  while (true)
  {
    if (i >= Asize && i >= Bsize)
    {
      sumAB.m_shorts.push_back(dig_sum_carry);
      break;
    }
    else if (i >= Avec.size())
    {
      A_dig = 0;
      B_dig = Bvec.at(i);
    }
    else if (i >= Bvec.size())
    {
      A_dig = Avec.at(i);
      B_dig = 0;
    }
    else
    {
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
LargeInt operator-(const LargeInt& intA, const LargeInt& intB)
{
  LargeInt tmp_mod{intA};
  if (intA <= intB)
    return 0;

  std::vector<signed int> tmp_buf_vec{};

  // const references with short names for readability
  const std::vector<unsigned short>& vec_A{intA.m_shorts};
  const std::vector<unsigned short>& vec_B{intB.m_shorts};

  const size_t size_A{vec_A.size()};
  const size_t size_B{vec_B.size()};

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

  // if the result of subtracting digits was negative, add 10 and take one from
  // the previous digit
  for (size_t i{0}; i < tmp_buf_vec.size() - 1; i++)
  {
    if (tmp_buf_vec.at(i) < 0)
    {
      tmp_buf_vec.at(i + 1) -= 1;
      tmp_buf_vec.at(i) += 10;
    }
  }

  // if the result of the subtraction was negative (shouldn't be possible
  // because comparison done earlier)
  if (tmp_buf_vec.at(tmp_buf_vec.size() - 1) < 0)
  {
    return LargeInt{0};
  }

  LargeInt difAB{};
  difAB.m_shorts.clear(); // the default constructor could be changed to init to
                          // anything, clear to be safe

  // because tmp_vec_buf uses signed ints for convenience, do checks to ensure
  // values can be made into unsigned shorts without narrowing
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
  difAB.shrinkPreceedingZeroes();
  return difAB;
}

LargeInt operator*(const LargeInt& intA, const LargeInt& intB)
{
  LargeInt final_result{0};

  LargeInt it_result{0};
  LargeInt in_it_result{0};

  for (size_t i{0}; i < intB.m_shorts.size(); i++)
  {
    for (size_t j{0}; j < intA.m_shorts.size(); j++)
    {
      LargeInt tmp{(unsigned short)(intA.m_shorts.at(j) * intB.m_shorts.at(i))};

      // multiply by 10 to power of current index
      for (size_t k{j}; k > 0; k--)
      {
        tmp.m_shorts.insert(tmp.m_shorts.begin(), 1, 0);
      }
      in_it_result = in_it_result + tmp;
    }
    // multiply by 10 to power of current index
    for (size_t k{i}; k > 0; k--)
    {
      in_it_result.m_shorts.insert(in_it_result.m_shorts.begin(), 1, 0);
    }

    final_result = final_result + in_it_result;
    in_it_result = 0;
  }
  return final_result;
}

// this algorithm calculates division inneficiently through a loop,
// so it should only be used when the quotient is small
LargeInt inefficDiv(const LargeInt& intA, const LargeInt& intB)
{
  LargeInt i{0};
  while (((i + 1) * intB) <= intA)
  {
    ++i;
  }
  i.shrinkPreceedingZeroes();
  return i;
}

// performs integral division between two largeints
LargeInt operator/(const LargeInt& intA, const LargeInt& intB)
{
  LargeInt slice{0};
  slice.m_shorts.clear();
  LargeInt returnval{0};

  //iterate through A
  for (size_t i{0}; i < intA.m_shorts.size(); i++)
  {
    //in each iteration, insert the next-rightmost value into slice at 0 index
    slice.m_shorts.insert(slice.m_shorts.begin(), 1, intA.m_shorts.at(intA.m_shorts.size() - i - 1));
    slice.shrinkPreceedingZeroes(); //ensure no stray zeroes remain
    if (intB <= slice)
    {
      LargeInt tmp{inefficDiv(slice, intB)};

      slice = slice - tmp * intB;
      for (size_t k{intA.m_shorts.size() - i - 1}; k > 0; k--)
      {

        tmp.m_shorts.insert(tmp.m_shorts.begin(), 1, 0);
      }
      returnval = returnval + tmp;
    }
  }
  return returnval;
}

// private utility used in the case a string is needed and for <<
std::string LargeInt::representedValue() const
{
  std::string ret_val{};

  // character represenations of each digit are appended to retval
  for (size_t i{m_shorts.size()}; i > 0; i -= 1)
  {
    ret_val += charFromDigitShort(m_shorts.at(i - 1));
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