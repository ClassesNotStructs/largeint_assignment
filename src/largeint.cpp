#include "largeint.h"
#include <iostream>
#include <cmath>

//basically will return the single-digit value at an index in a number (measured from r->l, starts at 0)
unsigned short valAtDigitIndex(const int index, const unsigned long long val)
{
    return (val % static_cast<int>(round(pow(10, index + 1)))) / pow(10, index);
}

//simple conversion from character representing a digit to a digit; returns 10 if incorrect range, which is error code as impossible for single digit
unsigned short digitShortFromChar(const char val)
{
    return (val > 47 && val < 59) ? val - 48 : 10;
}

char charFromDigitShort(const unsigned short val)
{
    return val + 48;
}

//checks that each char in a string is in the digit range, returns 0 if not
bool flagNonDigitCharacters(const std::string &str)
{
    bool flag{1};
    for (char i : str)
    {
        flag *= digitShortFromChar(i) != 10;
    }
    return flag;
}

LargeInt::LargeInt(const unsigned long long init_val)
{
    //pushes back each digit, checking whether that digit can exist in init_val beforehand, from least to greatest value
    int n{0};
    do
    {
        m_shorts.push_back(valAtDigitIndex(n, init_val));
        n += 1;
    } while (init_val / pow(10, n) >= 1);
}

LargeInt::LargeInt(const std::string &init_string)
{
    if (!flagNonDigitCharacters(init_string) || (init_string.begin() == init_string.end()))
    {
        m_shorts.clear();
        m_shorts.push_back(0);
        return;
    }

    for (size_t index{init_string.length()}; index > 0; index--)
    {
        m_shorts.push_back(digitShortFromChar(init_string[index - 1]));
        //std::cout << digitShortFromChar(init_string[index]) << std::endl;
    }
}

std::ostream &operator<<(std::ostream &out, const LargeInt &val)
{
    out << val.representedValue();
    return out;
}

LargeInt::operator std::string() const
{
    return representedValue();
}

//appends each digit from least to greatest value
std::string LargeInt::representedValue() const
{
    std::string ret_val{0};
    for (size_t i{m_shorts.size()}; i > 0; i -= 1)
    {
        std::cout << i - 1 << std::endl;
        ret_val.push_back(charFromDigitShort(m_shorts.at(i - 1)));
    }
    return ret_val;
}
