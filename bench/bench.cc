#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>

#include "sieve_of_eratosthenes_atkin.h"

/******************************************************************************
 * Run a benchmark.
 *
 * @param sieve_new Function which constructs the sieve.
 * @param sieve_delete Function which deallocates the sieve.
 * @param message String to display before starting the benchmark.
 *****************************************************************************/
template <typename C, typename D>
void
bench(C sieve_new, D sieve_delete, char const *message)
{
    std::cout << message << '\n';
    int constexpr iterations = 10;
    for (unsigned twopower = 16; twopower <= 30; ++twopower)
    {
        std::size_t limit = 1UL << twopower;
        auto begin = std::chrono::steady_clock::now();
        for (int _ = 0; _ < iterations; ++_)
        {
            auto sieve = sieve_new(limit);
            sieve_delete(sieve);
        }
        auto end = std::chrono::steady_clock::now();
        auto total_delay = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        auto average_delay = total_delay / iterations;
        std::cout << "1 << " << twopower << ' ' << std::setw(6) << average_delay << " ms\n";
    }
    std::cout << '\n';
}

/******************************************************************************
 * Main function.
 *****************************************************************************/
int
main(void)
{
    // bench(sieve_of_eratosthenes_new, sieve_of_eratosthenes_delete, "Eratosthenes");
    bench(sieve_of_atkin_new, sieve_of_atkin_delete, "Atkin");
}
