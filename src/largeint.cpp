#include "largeint.h"
#include <iostream>
#include <cmath>

//make the side effect thing better - ensure that neither operand is mutable
bool LargeInt::operator<(const LargeInt &val)
{
    //avoids resizing as a side effect
    std::vector<unsigned short> modifiable{val.m_shorts};

    if (m_shorts.size() < val.m_shorts.size())
    {
        m_shorts.resize(val.m_shorts.size());
    }

    else if (val.m_shorts.size() < m_shorts.size())
    {
        modifiable.resize(m_shorts.size());
    }

    /*
    std::cout << 'n' << '\n';
    for (unsigned short cur : m_shorts)
    {
        std::cout << cur << '\n';
    }
    std::cout << '-' << '\n';
    for (unsigned short cur : val.m_shorts)
    {
        std::cout << cur << '\n';
    }
    std::cout << '-' << '\n';
    for (unsigned short cur : modifiable)
    {
        std::cout << cur << '\n';
    }
    */

    for (size_t index{m_shorts.size()}; index > 0; index--)
    {
        if (m_shorts[index - 1] < modifiable[index - 1])
        {
            return true;
        }
        else if (m_shorts[index - 1] > modifiable[index - 1])
        {
            return false;
        }
    }
    return false;
}

bool LargeInt::operator>(const LargeInt &val)
{
    //avoids resizing as a side effect
    std::vector<unsigned short> modifiable{val.m_shorts};

    if (m_shorts.size() < val.m_shorts.size())
    {
        m_shorts.resize(val.m_shorts.size());
    }

    else if (val.m_shorts.size() < m_shorts.size())
    {
        modifiable.resize(m_shorts.size());
    }

    /*
    std::cout << 'n' << '\n';
    for (unsigned short cur : m_shorts)
    {
        std::cout << cur << '\n';
    }
    std::cout << '-' << '\n';
    for (unsigned short cur : val.m_shorts)
    {
        std::cout << cur << '\n';
    }
    std::cout << '-' << '\n';
    for (unsigned short cur : modifiable)
    {
        std::cout << cur << '\n';
    }
    */

    for (size_t index{m_shorts.size()}; index > 0; index--)
    {
        if (m_shorts[index - 1] > modifiable[index - 1])
        {
            return true;
        }
        else if (m_shorts[index - 1] < modifiable[index - 1])
        {
            return false;
        }
    }
    return false;
}
bool LargeInt::operator<=(const LargeInt &val)
{
    return !(*this > val);
}
bool LargeInt::operator>=(const LargeInt &val)
{
    return !(*this < val);
}
bool LargeInt::operator==(const LargeInt &val)
{
    return !(*this < val) && !(*this > val);
}
bool LargeInt::operator!=(const LargeInt &val)
{
    return !(*this == val);
}

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

LargeInt operator+(const LargeInt &intA, const LargeInt &intB)
{
    LargeInt sumAB{};
    sumAB.m_shorts.clear();

    const std::vector<unsigned short> &Avec{intA.m_shorts};
    const std::vector<unsigned short> &Bvec{intB.m_shorts};

    const size_t Asize{Avec.size()};
    const size_t Bsize{Bvec.size()};

    std::cout << Asize << '\n'
              << Bsize << '\n'
              << std::endl;

    unsigned short dig_sum{0};
    unsigned short dig_sum_carry{0};
    /*
    for (size_t i{0}; i < max; i++)
    {
        std::cout << i << std::endl;
        std::cout << Avec.at(i) << '\n'
                  << Bvec.at(i) << '\n'
                  << sumAB.m_shorts.at(i) << '\n';
        dig_sum = Avec.at(i) + Bvec.at(i) + dig_sum_carry;
        dig_sum_carry = dig_sum / 10;
        dig_sum %= 10;
        sumAB.m_shorts.at(i) = dig_sum;
        dig_sum = 0;
    }
    */
    size_t i{0};
    unsigned short A_dig{0};
    unsigned short B_dig{0};
    while (true)
    {
        std::cout << "iteration " << i << std::endl;
        if (i >= Asize && i >= Bsize)
        {
            sumAB.m_shorts.push_back(dig_sum_carry);
            break;
        }
        else if (i >= Avec.size())
        {
            A_dig = 0;
            B_dig = Bvec.at(i);
            std::cout << "na" << '\n'
                      << Bvec.at(i) << ", " << B_dig << std::endl;
        }
        else if (i >= Bvec.size())
        {
            A_dig = Avec.at(i);
            B_dig = 0;
            std::cout << Avec.at(i) << ", " << A_dig << '\n'
                      << "na" << std::endl;
        }
        else
        {
            A_dig = Avec.at(i);
            std::cout << Avec.at(i) << ", " << A_dig << std::endl;
            B_dig = Bvec.at(i);
            std::cout << Bvec.at(i) << ", " << B_dig << std::endl;
        }
        std::cout << "A" << A_dig << "\nB" << B_dig << "\nC" << dig_sum_carry << std::endl;
        dig_sum = A_dig + B_dig + dig_sum_carry;
        dig_sum_carry = dig_sum / 10;
        dig_sum %= 10;
        std::cout << dig_sum << std::endl;
        sumAB.m_shorts.push_back(dig_sum);
        dig_sum = 0;
        ++i;
    }
    return sumAB;
}

//consider using a stack to implement the walking up the vector for subtraction from zero
LargeInt operator-(const LargeInt &intA, const LargeInt &intB)
{
    LargeInt tmp_mod{intA};
    if (tmp_mod.operator<=(intB))
        return 0;

    LargeInt sumAB{};
    sumAB.m_shorts.clear();

    const std::vector<unsigned short> &Avec{intA.m_shorts};
    const std::vector<unsigned short> &Bvec{intB.m_shorts};

    const size_t Asize{Avec.size()};
    const size_t Bsize{Bvec.size()};

    std::cout << Asize << '\n'
              << Bsize << '\n'
              << std::endl;

    short tmp_buf{0};
    short reduction_buffer{0};

    unsigned short A_dig{0};
    unsigned short B_dig{0};

    size_t i{0};
    while (true)
    {
        std::cout << "iteration " << i << std::endl;
        if (i >= Asize && i >= Bsize)
        {
            if (A_dig - B_dig < 0)
            {
                if (tmp_buf - Bvec[i + 1] >= 0)
                {
                    sumAB.m_shorts.at(i + 1) = tmp_buf - Bvec[i + 1];
                }
                else
                {
                    return LargeInt{0};
                }
            }
            break;
        }
        else if (i >= Bvec.size())
        {
            sumAB.m_shorts.push_back(Avec.at(i));
            ++i;
            continue;
        }
        else if (i >= Avec.size())
        {
            return LargeInt{0};
        }
        else
        {
            A_dig = Avec.at(i);
            std::cout << Avec.at(i) << ", " << A_dig << std::endl;
            B_dig = Bvec.at(i);
            std::cout << Bvec.at(i) << ", " << B_dig << std::endl;
        }
        std::cout << "A" << A_dig << "\nB" << B_dig << "\nC" << std::endl;

        tmp_buf = A_dig - B_dig /* + dig_sum_carry*/;

        if (tmp_buf >= 0)
        {
            sumAB.m_shorts.push_back(tmp_buf);
        }

        else
        {
            std::vector<signed int> tmp_vector{};

            signed short j{i};
            while (tmp_buf < 0)
            {
                if (j >= Avec.size())
                {
                    return LargeInt{0};
                }

                sumAB.m_shorts.push_back(tmp_buf + 10);
                tmp_buf = Avec.at(j + 1) - 1;
                /*
                if (tmp_buf >= 0)
                {
                    if (tmp_buf - Bvec.at(j + 1) >= 0)
                    {
                        sumAB.m_shorts.push_back(tmp_buf);
                        ++i;
                        break;
                    }
                    tmp_buf -= BVec.at(j + 1);
                }
                */
                j += 1;
            }
        }

        ++i;
    }
    return sumAB;
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
