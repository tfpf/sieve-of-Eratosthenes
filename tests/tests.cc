#include <cassert>
#include <cstddef>
#include <thread>

#include "sieve_of_eratosthenes_atkin.h"

// Each count of prime numbers excludes 2, 3 and 5, since both sieves start
// finding primes numbers from 7 onwards.
static std::size_t counts_limits[][2] = {
    { 11145332, 201268277 },
    { 14817689, 272068078 },
    { 18739219, 348738693 },
    { 20409369, 381656751 },
    { 21423606, 401730367 },
    { 21984274, 412847637 },
    { 22879491, 430626488 },
    { 28077424, 534558099 },
    { 36669946, 708491638 },
    { 40759133, 792066697 },
    { 41876892, 814981682 },
    { 43807217, 854635735 },
    { 46429840, 908671296 },
    { 46994786, 920313910 },
    { 48028264, 941665889 },
};

/******************************************************************************
 * Test the implementation of the sieve of Eratosthenes.
 *****************************************************************************/
void
erato_work(void)
{
    for (auto const &[count, limit] : counts_limits)
    {
        SieveOfEratosthenes *erato = sieve_of_eratosthenes_new(limit);
        std::size_t erato_count = sieve_of_eratosthenes_count(erato);
        sieve_of_eratosthenes_delete(erato);
        assert(erato_count == count);
    }
}

/******************************************************************************
 * Test the implementation of the sieve of Atkin.
 *****************************************************************************/
void
atkin_work(void)
{
    for (auto const &[count, limit] : counts_limits)
    {
        SieveOfAtkin *atkin = sieve_of_atkin_new(limit);
        std::size_t atkin_count = sieve_of_atkin_count(atkin);
        sieve_of_atkin_delete(atkin);
        assert(atkin_count == count);
    }
}

/******************************************************************************
 * Main function.
 *****************************************************************************/
int
main(void)
{
    std::thread erato_worker(erato_work);
    std::thread atkin_worker(atkin_work);
    erato_worker.join();
    atkin_worker.join();
}
