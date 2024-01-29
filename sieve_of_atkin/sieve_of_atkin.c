#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Consecutive differences between coprime residues modulo 60: 1, 7, 11, 13,
// 17, 19, 23, 29, 31, 37, 41, 43, 47, 49, 53 and 59.
static short unsigned OFFSETS[] = {6, 4, 2, 4, 2, 4, 6, 2, 6, 4, 2, 4, 2, 4, 6, 2};

// Position of the bit indicating the primality of a coprime residue modulo 60
// in a 16-element bitfield. For non-coprime residues, the value is 16.
static short unsigned SHIFTS[] = {16, 0, 16, 16, 16, 16, 16, 1, 16, 16, 16, 2, 16, 3, 16, 16, 16, 4, 16, 5, 16, 16, 16, 6, 16, 16, 16, 16, 16, 7, 16, 8, 16, 16, 16, 16, 16, 9, 16, 16, 16, 10, 16, 11, 16, 16, 16, 12, 16, 13, 16, 16, 16, 14, 16, 16, 16, 16, 16, 15};

struct SieveOfAtkin
{
    size_t limit;
    size_t sieve_len;
    // Each element of this array is a 16-element bitfield which indicates the
    // primality of 16 coprime residues modulo 60. If a bit is set, the
    // corresponding number is prime.
    uint16_t *sieve;
};

/******************************************************************************
 * Algorithm 4.1.
 *
 * @param soa Sieve of Atkin.
 * @param delta What each of the prime numbers must be congruent to modulo 60.
 * @param x Starting abcissa.
 * @param y0 Starting ordinate.
 *****************************************************************************/
void sieve_of_atkin_algorithm_4_1(struct SieveOfAtkin *soa, int delta, long long x, long long y0)
{
    long long k0 = (4 * x * x + y0 * y0 - delta) / 60;
    while(k0 < (long long)soa->sieve_len)
    {
        k0 += 2 * x + 15;
        x += 15;
    }
    while(true)
    {
        x -= 15;
        k0 -= 2 * x + 15;
        if(x <= 0)
        {
            return;
        }
        while(k0 < 0)
        {
            k0 += y0 + 15;
            y0 += 30;
        }
        for(long long k = k0, y = y0; k < (long long)soa->sieve_len;)
        {
            soa->sieve[k] ^= 1 << SHIFTS[delta];
            k += y + 15;
            y += 30;
        }
    }
}

/******************************************************************************
 * Algorithm 4.2.
 *
 * @param soa Sieve of Atkin.
 * @param delta What each of the prime numbers must be congruent to modulo 60.
 * @param x Starting abcissa.
 * @param y0 Starting ordinate.
 *****************************************************************************/
void sieve_of_atkin_algorithm_4_2(struct SieveOfAtkin *soa, int delta, long long x, long long y0)
{
    long long k0 = (3 * x * x + y0 * y0 - delta) / 60;
    while(k0 < (long long)soa->sieve_len)
    {
        k0 += x + 5;
        x += 10;
    }
    while(true)
    {
        x -= 10;
        k0 -= x + 5;
        if(x <= 0)
        {
            return;
        }
        while(k0 < 0)
        {
            k0 += y0 + 15;
            y0 += 30;
        }
        for(long long k = k0, y = y0; k < (long long)soa->sieve_len;)
        {
            soa->sieve[k] ^= 1 << SHIFTS[delta];
            k += y + 15;
            y += 30;
        }
    }
}

/******************************************************************************
 * Algorithm 4.3.
 *
 * @param soa Sieve of Atkin.
 * @param delta What each of the prime numbers must be congruent to modulo 60.
 * @param x Starting abcissa.
 * @param y0 Starting ordinate.
 *****************************************************************************/
void sieve_of_atkin_algorithm_4_3(struct SieveOfAtkin *soa, int delta, long long x, long long y0)
{
    long long k0 = (3 * x * x - y0 * y0 - delta) / 60;
    while(true)
    {
        while(k0 >= (long long)soa->sieve_len)
        {
            if(x <= y0)
            {
                return;
            }
            k0 -= y0 + 15;
            y0 += 30;
        }
        for(long long k = k0, y = y0; k >= 0 && y < x;)
        {
            soa->sieve[k] ^= 1 << SHIFTS[delta];
            k -= y + 15;
            y += 30;
        }
        k0 += x + 5;
        x += 10;
    }
}

/******************************************************************************
 * Algorithm 3.1.
 *
 * @param soa Sieve of Atkin.
 * @param delta What each of the prime numbers must be congruent to modulo 60.
 *****************************************************************************/
void sieve_of_atkin_algorithm_3_1(struct SieveOfAtkin *soa, int delta)
{
    for(int f = 1; f <= 15; ++f)
    {
        for(int g = 1; g <= 30; g += 2)
        {
            if(delta == (4 * f * f + g * g) % 60)
            {
                sieve_of_atkin_algorithm_4_1(soa, delta, f, g);
            }
        }
    }
}

/******************************************************************************
 * Algorithm 3.2.
 *
 * @param soa Sieve of Atkin.
 * @param delta What each of the prime numbers must be congruent to modulo 60.
 *****************************************************************************/
void sieve_of_atkin_algorithm_3_2(struct SieveOfAtkin *soa, int delta)
{
    for(int f = 1; f <= 10; f += 2)
    {
        for(int g = 2; g <= 30; g += 2)
        {
            if(delta == (3 * f * f + g * g) % 60)
            {
                sieve_of_atkin_algorithm_4_2(soa, delta, f, g);
            }
        }
    }
}

/******************************************************************************
 * Algorithm 3.3.
 *
 * @param soa Sieve of Atkin.
 * @param delta What each of the prime numbers must be congruent to modulo 60.
 *****************************************************************************/
void sieve_of_atkin_algorithm_3_3(struct SieveOfAtkin *soa, int delta)
{
    for(int f = 1; f <= 10; ++f)
    {
        for(int g = f % 2 + 1; g <= 30; g += 2)
        {
            // We need residues modulo 60, so make negative remainders
            // positive.
            int remainder = (3 * f * f - g * g) % 60;
            remainder = remainder < 0 ? remainder + 60 : remainder;
            if(delta == remainder)
            {
                sieve_of_atkin_algorithm_4_3(soa, delta, f, g);
            }
        }
    }
}

/******************************************************************************
 * Generate the sieve of Atkin up to and including the given number.
 *
 * @param limit Non-strict upper bound up to which primes should be found.
 *
 * @return Sieve of Atkin. This should be deleted after use.
 *****************************************************************************/
struct SieveOfAtkin *sieve_of_atkin_new(size_t limit)
{
    struct SieveOfAtkin *soa = malloc(sizeof *soa);
    if(soa == NULL)
    {
        fprintf(stderr, "Memory error. Fatally low memory.\n");
        exit(EXIT_FAILURE);
    }
    soa->limit = limit;

    // Strict upper bound divisible by 60.
    size_t limit_rounded = limit - limit % 60 + 60;
    if(limit_rounded <= limit)
    {
        fprintf(stderr, "Overflow detected. Argument is too large.\n");
        exit(EXIT_FAILURE);
    }
    size_t limit_rounded_isqrt = sqrtl(limit_rounded);

    // Start with a cleared array, indicating that all numbers are composite.
    size_t sieve_len = limit / 60 + 1;
    soa->sieve_len = sieve_len;
    uint16_t *sieve = calloc(sieve_len, sizeof *sieve);
    if(sieve == NULL)
    {
        fprintf(stderr, "Memory error. Argument is too large.\n");
        exit(EXIT_FAILURE);
    }
    soa->sieve = sieve;

    sieve_of_atkin_algorithm_3_1(soa, 1);
    sieve_of_atkin_algorithm_3_1(soa, 13);
    sieve_of_atkin_algorithm_3_1(soa, 17);
    sieve_of_atkin_algorithm_3_1(soa, 29);
    sieve_of_atkin_algorithm_3_1(soa, 37);
    sieve_of_atkin_algorithm_3_1(soa, 41);
    sieve_of_atkin_algorithm_3_1(soa, 49);
    sieve_of_atkin_algorithm_3_1(soa, 53);
    sieve_of_atkin_algorithm_3_2(soa, 7);
    sieve_of_atkin_algorithm_3_2(soa, 19);
    sieve_of_atkin_algorithm_3_2(soa, 31);
    sieve_of_atkin_algorithm_3_2(soa, 43);
    sieve_of_atkin_algorithm_3_3(soa, 11);
    sieve_of_atkin_algorithm_3_3(soa, 23);
    sieve_of_atkin_algorithm_3_3(soa, 47);
    sieve_of_atkin_algorithm_3_3(soa, 59);

    // Clear the bits for multiples of squares of primes.
    size_t num = 1;
    size_t offsets_idx = 0;
    for(size_t sieve_idx = 0; sieve_idx < sieve_len; ++sieve_idx)
    {
        for(int shift = 0; shift < 16; ++shift)
        {
            if((sieve[sieve_idx] >> shift & 1) == 1)
            {
                size_t num_sqr = num * num;
                for(size_t multiple = num_sqr; multiple < limit_rounded; multiple += num_sqr)
                {
                    sieve[multiple / 60] &= ~((uint32_t)1 << SHIFTS[multiple % 60]);
                }
            }
            num += OFFSETS[offsets_idx];
            offsets_idx = (offsets_idx + 1) % 16;
            if(num > limit_rounded_isqrt)
            {
                return soa;
            }
        }
    }
    return soa;
}

/******************************************************************************
 * Count the number of prime numbers which have been found.
 *
 * @param soa Sieve of Atkin.
 *
 * @param Number of primes.
 *****************************************************************************/
size_t sieve_of_atkin_count(struct SieveOfAtkin *soa)
{
    size_t num = 1;
    size_t offsets_idx = 0;
    size_t count = 0;
    for(size_t sieve_idx = 0; sieve_idx < soa->sieve_len; ++sieve_idx)
    {
        for(int shift = 0; shift < 16; ++shift)
        {
            if((soa->sieve[sieve_idx] >> shift & 1) == 1)
            {
                ++count;
            }
            num += OFFSETS[offsets_idx];
            offsets_idx = (offsets_idx + 1) % 16;
            if(num > soa->limit)
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
 * @param soa Sieve of Atkin.
 *****************************************************************************/
void sieve_of_atkin_delete(struct SieveOfAtkin *soa)
{
    free(soa->sieve);
    free(soa);
}

/******************************************************************************
 * Main function.
 *****************************************************************************/
int main(int const argc, char const *argv[])
{
    size_t limit = 100;
    if(argc > 1)
    {
        char *endptr;
        long long unsigned limit_ = strtoull(argv[1], &endptr, 10);
        if(*endptr != '\0')
        {
            fprintf(stderr, "Argument is unparseable.\n");
            return EXIT_FAILURE;
        }
        limit = limit_;
    }

    struct SieveOfAtkin *soa = sieve_of_atkin_new(limit);
    size_t num_of_primes = sieve_of_atkin_count(soa);
    sieve_of_atkin_delete(soa);
    printf("%zu primes (excluding 2, 3 and 5) till %zu\n", num_of_primes, limit);
}
