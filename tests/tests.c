#include <assert.h>
#include <stddef.h>

struct SieveOfEratosthenes;
struct SieveOfEratosthenes *sieve_of_eratosthenes_new(size_t limit);
size_t sieve_of_eratosthenes_count(struct SieveOfEratosthenes *erato);
void sieve_of_eratosthenes_delete(struct SieveOfEratosthenes *erato);

struct SieveOfAtkin;
struct SieveOfAtkin *sieve_of_atkin_new(size_t limit);
size_t sieve_of_atkin_count(struct SieveOfAtkin *atkin);
void sieve_of_atkin_delete(struct SieveOfAtkin *atkin);

int
main(void)
{
    struct SieveOfEratosthenes *erato = sieve_of_eratosthenes_new(1000000000);
    size_t erato_count = sieve_of_eratosthenes_count(erato);
    sieve_of_eratosthenes_delete(erato);
    assert(erato_count == 50847531);

    struct SieveOfAtkin *atkin = sieve_of_atkin_new(1000000000);
    size_t atkin_count = sieve_of_atkin_count(atkin);
    sieve_of_atkin_delete(atkin);
    assert(atkin_count == 50847531);
}
