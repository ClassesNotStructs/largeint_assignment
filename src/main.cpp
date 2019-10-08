#include <iostream>
#include "largeint.h"

int main()
{
    //LargeInt test{100010};
    //std::cout << test.representedValue() << std::endl;
    std::string brh = "100010";
    LargeInt test1{brh};
    std::cout << test1.representedValue() << std::endl;
    return 0;
}