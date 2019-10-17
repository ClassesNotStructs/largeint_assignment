#include "largeint.h"
#include <iostream>

int main()
{
  LargeInt test1{"112"};
  LargeInt test2{"21273849213847128374812374812348723842144"};

  LargeInt val{(test1 * test2)};
  std::cout << val;
  return 0;
}