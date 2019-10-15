#include <iostream>
#include "largeint.h"

int main()
{
    //LargeInt test{100010};
    //std::cout << test.representedValue() << std::endl;
    LargeInt test1{5};
    LargeInt test2{"10"};
    std::cout << (test2 * test1) << std::endl;
    return 0;
}