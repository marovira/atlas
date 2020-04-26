#include <atlas/hlr/export.hpp>

extern "C" {
EXPORTED int add(int a, int b)
{
    return a + b;
}
}
