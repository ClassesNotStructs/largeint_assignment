# largeint_assignment

Description:
This is a simple C++ class able to store decimal numbers with a length of up to the maximum value of an unsigned long long using a vector. 

The LargeInt class is quite simple to use, as most operators (+, -, *, /) are overloaded intuitively. 

Include the header at inc/largeint.h to use it.

Usage:
A LargeInt can be initialized from a string containing your number in decimal (no decimal point), or from an unsigned long long.
If no number is provided, the value defaults to zero. 

The stream insertion operator, comparison operators, prefix and postfix operators, and arithmetic operators are all overloaded.

Tips and Misc:
I would not recommend exponentiation for particularly large exponents unless needed. If you use it, it is called using the binary xor operator (^)

A - B will equal 0 if B > A

This type uses a vector to store unsigned shorts for each decimal digit, which is relatively inneficient compared to, say, representation through binary. This wasn't really a design choice, I just made this for a class and it these were requirements.
As such, don't use this library for academic purposes.

The maximum size for the largeint is the maximum number with a number of digits equal to the maximum value of a size_t type. If you need numbers bigger than this, definitely use a better library, as it's sure to be faster as well.
