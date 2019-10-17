#pragma once

#include <string>
#include <vector>

class LargeInt
{
public:
  LargeInt(const unsigned long long init_val = 0);

  LargeInt(const std::string& init_string);

  std::string representedValue() const;

  // cast to std::string is same as calling representedvalue
  operator std::string() const;

  size_t getsize()
  {
    return m_shorts.size();
  }
  // return reference to result
  LargeInt& operator++();
  // return copy of previous
  LargeInt operator++(int);
  // return reference to result
  LargeInt& operator--();
  // return copy of previous
  LargeInt operator--(int);

  //just an exponent operator, since it seemed like it would be nice to have
  LargeInt& operator^(const LargeInt& exponent);

  friend bool operator<(const LargeInt& val_1, const LargeInt& val_2);
  friend bool operator>(const LargeInt& val_1, const LargeInt& val_2);
  friend bool operator<=(const LargeInt& val_1, const LargeInt& val_2);
  friend bool operator>=(const LargeInt& val_1, const LargeInt& val_2);
  friend bool operator==(const LargeInt& val_1, const LargeInt& val_2);
  friend bool operator!=(const LargeInt& val_1, const LargeInt& val_2);

  friend LargeInt operator+(const LargeInt& intA, const LargeInt& intB);
  friend LargeInt operator-(const LargeInt& intA, const LargeInt& intB);
  friend LargeInt operator*(const LargeInt& intA, const LargeInt& intB);
  friend LargeInt operator/(const LargeInt& intA, const LargeInt& intB);

  friend LargeInt inefficDiv(const LargeInt& intA, const LargeInt& intB);

  friend std::ostream& operator<<(std::ostream&, const LargeInt& val);

private:
  LargeInt* shrinkPreceedingZeroes();
  std::vector<unsigned short> m_shorts;
};