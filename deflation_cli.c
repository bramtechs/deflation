// Valid build paremeters
// - DEFLATION_TEST
#include <stdlib.h>

#include "deflation.h"

# ifdef DEFLATION_TEST
void unit_test()
{
}
# endif

int main(int argc, char** argv)
{
#  ifdef DEFLATION_TEST
    unit_test();
#  endif

    return EXIT_SUCCESS;
}
