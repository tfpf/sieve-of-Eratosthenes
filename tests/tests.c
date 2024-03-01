#include <assert.h>
#include <stddef.h>

#include "sieve_of_eratosthenes_atkin.h"

int
main(void)
{
    // Each count of prime numbers excludes 2, 3 and 5, since both sieves start
    // finding primes numbers from 7 onwards.
    size_t limits_primes[][2] = { { 10000000, 664576 }, { 100000000, 5761452 }, { 1000000000, 50847531 } };
    for (size_t i = 0; i < sizeof limits_primes / sizeof *limits_primes; ++i)
    {
        size_t limit = limits_primes[i][0];
        size_t primes = limits_primes[i][1];

        struct SieveOfEratosthenes *erato = sieve_of_eratosthenes_new(limit);
        size_t erato_count = sieve_of_eratosthenes_count(erato);
        sieve_of_eratosthenes_delete(erato);
        assert(erato_count == primes);

        struct SieveOfAtkin *atkin = sieve_of_atkin_new(limit);
        size_t atkin_count = sieve_of_atkin_count(atkin);
        sieve_of_atkin_delete(atkin);
        assert(atkin_count == primes);
    }
}
