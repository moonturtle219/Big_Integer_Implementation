#include "BigInt.h"

int main()
{
    BigInt a(768583582);
    BigInt b(1034);
    BigInt c = a * b;
    std::cout << a << " + " << b << " = " << c << std::endl;
    return 0;
}