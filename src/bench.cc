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
 * Run a benchmark.
 *
 * @param sieve_new Function which constructs the sieve.
 * @param sieve_delete Function which deallocates the sieve.
 *****************************************************************************/
template <typename C, typename D>
void
bench(C sieve_new, D sieve_delete, char const *message)
{
    std::cout << message << '\n';
    int constexpr iterations = 1024;
    for (unsigned twopower = 10; twopower < 20; ++twopower)
    {
        std::size_t limit = 1UL << twopower;
        auto begin = std::chrono::steady_clock::now();
        for (int _ = 0; _ < iterations; ++_)
        {
            auto sieve = sieve_new(limit);
            sieve_delete(sieve);
        }
        auto end = std::chrono::steady_clock::now();
        auto total_delay = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        auto average_delay = total_delay / iterations;
        std::cout << twopower << ' ' << std::setw(10) << average_delay << " µs\n";
    }
    std::cout << '\n';
}

/******************************************************************************
 * Main function.
 *****************************************************************************/
int
main(void)
{
    bench(sieve_of_eratosthenes_new, sieve_of_eratosthenes_delete, "Eratosthenes");
    bench(sieve_of_atkin_new, sieve_of_atkin_delete, "Atkin");
}
