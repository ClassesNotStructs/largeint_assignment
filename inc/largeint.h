#pragma once

#include <string>
#include <vector>

class LargeInt
{
public:
  friend LargeInt reverseSliceFrontalNumber(const LargeInt &val, size_t ind_from_left); //zero gives the 1 digit, 1 the 1 then 10, so on
  friend LargeInt inefficDiv(const LargeInt &intA, const LargeInt &intB);

  LargeInt(const unsigned long long init_val = 0);

  LargeInt(const std::string &init_string);

  std::string representedValue() const;

  // cast to std::string is same as calling representedvalue
  operator std::string() const;

  // return reference to result
  LargeInt &operator++();
  // return copy of previous
  LargeInt operator++(int);
  // return reference to result
  LargeInt &operator--();
  // return copy of previous
  LargeInt operator--(int);

  friend bool operator<(const LargeInt &val_1, const LargeInt &val_2);
  friend bool operator>(const LargeInt &val_1, const LargeInt &val_2);
  friend bool operator<=(const LargeInt &val_1, const LargeInt &val_2);
  friend bool operator>=(const LargeInt &val_1, const LargeInt &val_2);
  friend bool operator==(const LargeInt &val_1, const LargeInt &val_2);
  friend bool operator!=(const LargeInt &val_1, const LargeInt &val_2);

  friend LargeInt operator+(const LargeInt &intA, const LargeInt &intB);
  friend LargeInt operator-(const LargeInt &intA, const LargeInt &intB);
  friend LargeInt operator*(const LargeInt &intA, const LargeInt &intB);
  friend LargeInt operator/(const LargeInt &intA, const LargeInt &intB);

  friend std::ostream &operator<<(std::ostream &, const LargeInt &val);

private:
  LargeInt *shrinkPreceedingZeroes();
  std::vector<unsigned short> m_shorts;
};