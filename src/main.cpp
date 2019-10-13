#include <iostream>
#include "largeint.h"

int main()
{
    //LargeInt test{100010};
    //std::cout << test.representedValue() << std::endl;
    LargeInt test1{55};
    LargeInt test2{"47"};
    std::cout << (test1 - test2) << std::endl;
    return 0;
}