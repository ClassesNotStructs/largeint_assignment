#include <iostream>
#include "largeint.h"

int main()
{
    //LargeInt test{100010};
    //std::cout << test.representedValue() << std::endl;
    LargeInt test1{88};
    LargeInt test2{"880"};
    std::cout << (static_cast<LargeInt>(test1 > test2) < test1) << std::endl;
    std::cout << (test2 < test1) << std::endl;
    return 0;
}