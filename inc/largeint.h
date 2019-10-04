#pragma once

#include <string>
#include <vector>

class LargeInt
{
public:
    LargeInt(const unsigned long long init_val = 0);

    LargeInt(const std::string &init_string);

private:
    std::vector<unsigned short> m_shorts;
};