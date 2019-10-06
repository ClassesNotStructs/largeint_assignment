#include <iostream>
#include "largeint.h"

int main()
{
    LargeInt test{100010};
    std::cout << test.representedValue() << std::endl;
    LargeInt test1{"100010"};
    std::cout << test1.representedValue() << std::endl;
    return 0;
}