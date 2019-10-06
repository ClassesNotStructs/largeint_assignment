#pragma once

#include <string>
#include <vector>

class LargeInt
{
public:
    LargeInt(const unsigned long long init_val = 0);

    LargeInt(const std::string &init_string);

    std::string representedValue() const;

    operator std::string() const;

private:
    std::vector<unsigned short> m_shorts;
};