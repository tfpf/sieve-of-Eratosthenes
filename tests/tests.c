#include <assert.h>

#include "sieve_of_eratosthenes_atkin.h"

int
main(void)
{
    struct SieveOfEratosthenes *erato = sieve_of_eratosthenes_new(1000000000);
    size_t erato_count = sieve_of_eratosthenes_count(erato);
    sieve_of_eratosthenes_delete(erato);
    // The count excludes 2, 3 and 5.
    assert(erato_count == 50847531);

    struct SieveOfAtkin *atkin = sieve_of_atkin_new(1000000000);
    size_t atkin_count = sieve_of_atkin_count(atkin);
    sieve_of_atkin_delete(atkin);
    // Ditto.
    assert(atkin_count == 50847531);
}
