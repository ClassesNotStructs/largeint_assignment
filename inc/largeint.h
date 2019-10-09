#pragma once

#include <string>
#include <vector>

class LargeInt
{
public:
    LargeInt(const unsigned long long init_val = 0);

    LargeInt(const std::string &init_string);

    std::string representedValue() const;

    //cast to std::string is same as calling representedvalue
    operator std::string() const;

    bool operator<(const LargeInt &val);

    bool operator>(const LargeInt &val);

    bool operator<=(const LargeInt &val);

    bool operator>=(const LargeInt &val);

    bool operator==(const LargeInt &val);

    bool operator!=(const LargeInt &val);

    friend std::ostream &operator<<(std::ostream &, const LargeInt &val);

private:
    std::vector<unsigned short> m_shorts;
};