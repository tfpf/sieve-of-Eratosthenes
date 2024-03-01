// I don't know if C++ data types are identical to C data types of the same
// names. However, as long as the same compiler is used to compile both C and
// C++ programs, there should be no issue. Hence, try to be compatible with
// both.
#ifndef __cplusplus
#include <stddef.h>

struct SieveOfEratosthenes;
struct SieveOfEratosthenes *sieve_of_eratosthenes_new(size_t);
size_t sieve_of_eratosthenes_count(struct SieveOfEratosthenes *);
void sieve_of_eratosthenes_delete(struct SieveOfEratosthenes *);

struct SieveOfAtkin;
struct SieveOfAtkin *sieve_of_atkin_new(size_t);
size_t sieve_of_atkin_count(struct SieveOfAtkin *);
void sieve_of_atkin_delete(struct SieveOfAtkin *);
#else
#include <cstddef>

extern "C"
{
    struct SieveOfEratosthenes;
    SieveOfEratosthenes *sieve_of_eratosthenes_new(std::size_t);
    std::size_t sieve_of_eratosthenes_count(SieveOfEratosthenes *);
    void sieve_of_eratosthenes_delete(SieveOfEratosthenes *);

    struct SieveOfAtkin;
    SieveOfAtkin *sieve_of_atkin_new(std::size_t);
    std::size_t sieve_of_atkin_count(SieveOfAtkin *);
    void sieve_of_atkin_delete(SieveOfAtkin *);
}
#endif
