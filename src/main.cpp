#include "largeint.h"
#include <iostream>

int main() {
  // LargeInt test{100010};
  // std::cout << test.representedValue() << std::endl;
  LargeInt test1{5};
  LargeInt test2{11};
  std::cout << (test1 * test2) << std::endl;
  return 0;
}