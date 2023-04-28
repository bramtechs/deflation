// Valid build paremeters
// - DEFLATION_TEST
#include <stdlib.h>

#define DEFLATION_IMPLEMENTATION
#include "deflation.h"

#ifdef DEFLATION_TEST
void unit_test()
{
}
#endif

int main(int argc, char** argv)
{
#ifdef DEFLATION_TEST
    unit_test();
#endif

    if (argc < 3) {
        FATAL("Not enough arguments given.");
    }

    const char* input_folder = argv[argc - 2];
    const char* output_file = argv[argc - 1];
    if (deflate_folder(input_folder, output_file)) {
        INFO("Deflated folder %s to %s.", input_folder, output_file);
        return EXIT_SUCCESS;
    } else {
        FATAL("Could not deflate folder %s!", input_folder);
    }
}
