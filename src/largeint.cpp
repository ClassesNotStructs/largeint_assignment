#include "largeint.h"
#include <iostream>
#include <cmath>

unsigned short valAtDigitIndex(const int index, const unsigned long long val)
{
    unsigned short retval = (val % (int)round(pow(10, index + 1))) / pow(10, index);
    std::cout << "numerator: " << (val % (int)round(pow(10, index + 1))) << std::endl;
    int tmp{(int)round(pow(10, index + 1))};
    std::cout << "numerator w/out cast: " << tmp << std::endl;
    std::cout << "denominator: " << pow(10, index) << std::endl;
    std::cout << retval << std::endl;
    return retval;
}

LargeInt::LargeInt(const unsigned long long init_val)
{
    /*
    unsigned int digits{0};
    while (true)
    {
        std::cout << init_val / pow(10, digits) << std::endl;
        if ((init_val / pow(10, digits)) < 1)
        {
            break;
        }
        else
        {
            digits += 1;
        }
    }
    std::cout << digits << std::endl;
    */

    m_shorts.reserve(sizeof(unsigned long long));
    int current_digit_value;
    for (unsigned int exponent{0}; exponent < sizeof(unsigned long long); exponent++)
    {
        m_shorts.push_back(valAtDigitIndex(exponent, init_val));
    }

    std::cout << '\n';
    for (unsigned short val : m_shorts)
        std::cout << val;
    std::cout << std::endl;
}