#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * Main function.
 *****************************************************************************/
int main(int const argc, char const *argv[])
{
    // Non-strict upper bound till which primes will be calculated.
    size_t limit = 100;
    if(argc > 1)
    {
        char *endptr;
        long long unsigned limit_ = strtoull(argv[1], &endptr, 10);
        if(*endptr != '\0' || limit_ > SIZE_MAX / 2)
        {
            fprintf(stderr, "Argument 1 is unparseable or too large.\n");
            return EXIT_FAILURE;
        }
        limit = limit_;
    }
    // Strict upper bound till which primes will be calculated.
    size_t limit_rounded = limit - limit % 60 + 60;

    return EXIT_SUCCESS;
}
