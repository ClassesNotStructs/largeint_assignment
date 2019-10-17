#include "largeint.h"
#include <iostream>

int main()
{
  // LargeInt test{100010};
  // std::cout << test.representedValue() << std::endl;
  LargeInt test1{"1123409"};
  LargeInt test2{"1824"};
  LargeInt test3{"1234"};
  //std::cout << ((test1 * test2 / test2) == test1) << std::endl;

  LargeInt val{(test1 - test2)};
  std::cout << val;
  return 0;
}