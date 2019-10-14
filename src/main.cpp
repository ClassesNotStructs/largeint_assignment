#include <iostream>
#include "largeint.h"

int main()
{
    //LargeInt test{100010};
    //std::cout << test.representedValue() << std::endl;
    LargeInt test1{5714875};
    LargeInt test2{"04781924"};
    std::cout << (test2 <= test1) << " " << (test1 >= test2) << std::endl;
    return 0;
}