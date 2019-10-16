#include "largeint.h"
#include <iostream>

int main()
{
  // LargeInt test{100010};
  // std::cout << test.representedValue() << std::endl;
  LargeInt test1{"456"};
  LargeInt test2{"68"};
  //std::cout << ((test1 * test2 / test2) == test1) << std::endl;
  std::cout << (test1 < test2);
  return 0;
}