#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Consecutive differences between coprime residues modulo 60: 1, 7, 11, 13,
// 17, 19, 23, 29, 31, 37, 41, 43, 47, 49, 53 and 59.
static short unsigned OFFSETS[] = { 6, 4, 2, 4, 2, 4, 6, 2, 6, 4, 2, 4, 2, 4, 6, 2 };

// Position of the bit indicating the primality of a coprime residue modulo 60
// in a 16-element bitfield. For non-coprime residues, the value is 16.
static short unsigned SHIFTS[] = { 16, 0, 16, 16, 16, 16, 16, 1, 16, 16, 16, 2, 16, 3, 16, 16, 16, 4, 16, 5, 16, 16,
    16, 6, 16, 16, 16, 16, 16, 7, 16, 8, 16, 16, 16, 16, 16, 9, 16, 16, 16, 10, 16, 11, 16, 16, 16, 12, 16, 13, 16, 16,
    16, 14, 16, 16, 16, 16, 16, 15 };

struct SieveOfEratosthenes
{
    size_t limit;
    size_t sieve_len;
    // Each element of this array is a 16-element bitfield which indicates the
    // primality of 16 coprime residues modulo 60. If a bit is cleared, the
    // corresponding number is prime.
    uint16_t *sieve;
};

/******************************************************************************
 * Generate the sieve of Eratosthenes up to and including the given number.
 *
 * @param limit Non-strict upper bound up to which primes should be found.
 *
 * @return Sieve of Eratosthenes. This should be deleted after use.
 *****************************************************************************/
struct SieveOfEratosthenes *
sieve_of_eratosthenes_new(size_t limit)
{
    struct SieveOfEratosthenes *erato = malloc(sizeof *erato);
    if (erato == NULL)
    {
        fprintf(stderr, "Memory error. Fatally low memory.\n");
        exit(EXIT_FAILURE);
    }
    erato->limit = limit;

    // Strict upper bound divisible by 60.
    size_t limit_rounded = limit - limit % 60 + 60;
    if (limit_rounded <= limit)
    {
        fprintf(stderr, "Overflow detected. Argument is too large.\n");
        exit(EXIT_FAILURE);
    }
    size_t limit_rounded_isqrt = sqrtl(limit_rounded);

    // Start with a cleared array, indicating that all numbers are prime. Set
    // the bit for 1, because it is not prime.
    size_t sieve_len = limit / 60 + 1;
    erato->sieve_len = sieve_len;
    uint16_t *sieve = calloc(sieve_len, sizeof *sieve);
    if (sieve == NULL)
    {
        fprintf(stderr, "Memory error. Argument is too large.\n");
        exit(EXIT_FAILURE);
    }
    sieve[0] = 1;
    erato->sieve = sieve;

    // Set the bits for multiples of primes.
    size_t num = 1;
    size_t offsets_idx = 0;
    for (size_t sieve_idx = 0; sieve_idx < sieve_len; ++sieve_idx)
    {
        for (int shift = 0; shift < 16; ++shift)
        {
            if ((sieve[sieve_idx] >> shift & 1) == 0)
            {
                for (size_t multiple = num * num; multiple < limit_rounded; multiple += num)
                {
                    sieve[multiple / 60] |= (uint32_t)1 << SHIFTS[multiple % 60];
                }
            }
            num += OFFSETS[offsets_idx];
            offsets_idx = (offsets_idx + 1) % 16;
            if (num > limit_rounded_isqrt)
            {
                return erato;
            }
        }
    }
    return erato;
}

/******************************************************************************
 * Count the number of prime numbers which have been found.
 *
 * @param erato Sieve of Eratosthenes.
 *
 * @param Number of primes.
 *****************************************************************************/
size_t
sieve_of_eratosthenes_count(struct SieveOfEratosthenes *erato)
{
    size_t num = 1;
    size_t offsets_idx = 0;
    size_t count = 0;
    for (size_t sieve_idx = 0; sieve_idx < erato->sieve_len; ++sieve_idx)
    {
        for (int shift = 0; shift < 16; ++shift)
        {
            if ((erato->sieve[sieve_idx] >> shift & 1) == 0)
            {
                ++count;
            }
            num += OFFSETS[offsets_idx];
            offsets_idx = (offsets_idx + 1) % 16;
            if (num > erato->limit)
            {
                return count;
            }
        }
    }
    return count;
}

/******************************************************************************
 * Release memory.
 *
 * @param erato Sieve of Eratosthenes.
 *****************************************************************************/
void
sieve_of_eratosthenes_delete(struct SieveOfEratosthenes *erato)
{
    free(erato->sieve);
    free(erato);
}

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
    size_t num_of_primes = sieve_of_eratosthenes_count(erato);
    sieve_of_eratosthenes_delete(erato);
    printf("%zu primes (excluding 2, 3 and 5) till %zu\n", num_of_primes, limit);
}
