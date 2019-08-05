#include <atlas/hlr/Export.hpp>

extern "C" {
EXPORTED int add(int a, int b)
{
    return a + b;
}
}
