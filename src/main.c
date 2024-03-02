#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// I have placed the function prototypes in a header file, but you might want
// to simply copy and paste the sieve functions in the file where you'd like to
// use them.
#include "sieve_of_eratosthenes_atkin.h"

/******************************************************************************
 * Main function.
 *****************************************************************************/
int
main(int const argc, char const *argv[])
{
    size_t limit = 100;
    if (argc > 1)
    {
        char *endptr;
        long long unsigned limit_ = strtoull(argv[1], &endptr, 10);
        if (*endptr != '\0' || limit_ > SIZE_MAX)
        {
            fprintf(stderr, "Argument is unparseable.\n");
            return EXIT_FAILURE;
        }
        limit = limit_;
    }

    struct SieveOfEratosthenes *erato = sieve_of_eratosthenes_new(limit);
    size_t erato_primes = sieve_of_eratosthenes_count(erato);
    sieve_of_eratosthenes_delete(erato);
    printf("Found %zu primes (excluding 2, 3 and 5) till %zu using Eratosthenes.\n", erato_primes, limit);

    struct SieveOfAtkin *atkin = sieve_of_atkin_new(limit);
    size_t atkin_primes = sieve_of_atkin_count(atkin);
    sieve_of_atkin_delete(atkin);
    printf("Found %zu primes (excluding 2, 3 and 5) till %zu using Atkin.\n", atkin_primes, limit);
}
