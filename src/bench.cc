#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>

extern "C"
{
    struct SieveOfEratosthenes;
    struct SieveOfEratosthenes *sieve_of_eratosthenes_new(std::size_t limit);
    std::size_t sieve_of_eratosthenes_count(struct SieveOfEratosthenes *erato);
    void sieve_of_eratosthenes_delete(struct SieveOfEratosthenes *erato);

    struct SieveOfAtkin;
    struct SieveOfAtkin *sieve_of_atkin_new(std::size_t limit);
    std::size_t sieve_of_atkin_count(struct SieveOfAtkin *atkin);
    void sieve_of_atkin_delete(struct SieveOfAtkin *atkin);
}

/******************************************************************************
 * Main function.
 *****************************************************************************/
int
main(void)
{
    int constexpr iterations = 16384;

    for (unsigned twopower = 10; twopower < 20; ++twopower)
    {
        std::size_t limit = 1UL << twopower;
        auto begin = std::chrono::steady_clock::now();
        for (int _ = 0; _ < iterations; ++_)
        {
            struct SieveOfEratosthenes *erato = sieve_of_eratosthenes_new(limit);
            sieve_of_eratosthenes_delete(erato);
        }
        auto end = std::chrono::steady_clock::now();
        auto average_delay = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / iterations;
        std::cout << twopower << ' ' << std::setw(4) << average_delay << " µs\n";
    }
}
