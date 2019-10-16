#include "largeint.h"
#include <iostream>

int main()
{
  // LargeInt test{100010};
  // std::cout << test.representedValue() << std::endl;
  LargeInt test1{"37871847239847819277847886"};
  LargeInt test2{"1"};
  std::cout << ((test1 * test2 / test2) == test1) << std::endl;
  return 0;
}