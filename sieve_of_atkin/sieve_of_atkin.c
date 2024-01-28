#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Consecutive differences between the 16 coprime residues modulo 60: 1, 7, 11,
// 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 49, 53 and 59.
static short unsigned offsets[] = {6, 4, 2, 4, 2, 4, 6, 2, 6, 4, 2, 4, 2, 4, 6, 2};

// If an index is a coprime residue modulo 60, the corresponding value is its
// position in {1, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 49, 53, 59}.
// Otherwise, the value is 16.
static short unsigned shifts[] = {16, 0, 16, 16, 16, 16, 16, 1, 16, 16, 16, 2, 16, 3, 16, 16, 16, 4, 16, 5, 16, 16, 16, 6, 16, 16, 16, 16, 16, 7, 16, 8, 16, 16, 16, 16, 16, 9, 16, 16, 16, 10, 16, 11, 16, 16, 16, 12, 16, 13, 16, 16, 16, 14, 16, 16, 16, 16, 16, 15};

// If an index is a coprime residue modulo 60, the corresponding value
// indicates the algorithms to apply: 1 for algorithms 3.1 and 4.1, 2 for 3.2
// and 4.2, and 3 for 3.3 and 4.3. Otherwise, the value is 0, indicating a
// no-op.
static short algorithm[] = {0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 3, 0, 1, 0, 0, 0, 1, 0, 2, 0, 0, 0, 3, 0, 0, 0, 0, 0, 1, 0, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 2, 0, 0, 0, 3, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3};

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
 * @param sieve Sieve of Atkin.
 * @param sieve_len Its length.
 * @param delta What each of the prime numbers must be congruent to modulo 60.
 * @param x Starting abcissa.
 * @param y0 Starting ordinate.
 *****************************************************************************/
void sieve_of_atkin_algorithm_4_1(uint16_t *sieve, size_t sieve_len, int delta, long long x, long long y0)
{
    long long k0 = (4 * x * x + y0 * y0 - delta) / 60;
    while((long long unsigned)k0 < sieve_len)
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
        for(size_t k = k0, y = y0; k < sieve_len;)
        {
            sieve[k] ^= (uint16_t)1 << shifts[delta];
            k += y + 15;
            y += 30;
        }
    }
}

/******************************************************************************
 * Algorithm 4.2.
 *
 * @param sieve Sieve of Atkin.
 * @param sieve_len Its length.
 * @param delta What each of the prime numbers must be congruent to modulo 60.
 * @param x Starting abcissa.
 * @param y0 Starting ordinate.
 *****************************************************************************/
void sieve_of_atkin_algorithm_4_2(uint16_t *sieve, size_t sieve_len, int delta, long long x, long long y0)
{
    long long k0 = (3 * x * x + y0 * y0 - delta) / 60;
    while((long long unsigned)k0 < sieve_len)
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
        for(size_t k = k0, y = y0; k < sieve_len;)
        {
            sieve[k] ^= (uint16_t)1 << shifts[delta];
            k += y + 15;
            y += 30;
        }
    }
}

/******************************************************************************
 * Algorithm 4.3.
 *
 * @param sieve Sieve of Atkin.
 * @param sieve_len Its length.
 * @param delta What each of the prime numbers must be congruent to modulo 60.
 * @param x Starting abcissa.
 * @param y0 Starting ordinate.
 *****************************************************************************/
void sieve_of_atkin_algorithm_4_3(uint16_t *sieve, size_t sieve_len, int delta, long long x, long long y0)
{
    long long k0 = (3 * x * x - y0 * y0 - delta) / 60;
    while(true)
    {
        // `k0` can be negative here, so check its sign before doing an
        // unsigned cast. If it is negative, it is automatically less than the
        // sieve length.
        while(k0 > 0 && (long long unsigned)k0 >= sieve_len)
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
            sieve[k] ^= (uint16_t)1 << shifts[delta];
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
 * @param sieve Sieve of Atkin.
 * @param sieve_len Its length.
 * @param delta What each of the prime numbers must be congruent to modulo 60.
 *****************************************************************************/
void sieve_of_atkin_algorithm_3_1(uint16_t *sieve, size_t sieve_len, int delta)
{
    for(int f = 1; f <= 15; ++f)
    {
        for(int g = 1; g <= 30; ++g)
        {
            if(delta == (4 * f * f + g * g) % 60)
            {
                sieve_of_atkin_algorithm_4_1(sieve, sieve_len, delta, f, g);
            }
        }
    }
}

/******************************************************************************
 * Algorithm 3.2.
 *
 * @param sieve Sieve of Atkin.
 * @param sieve_len Its length.
 * @param delta What each of the prime numbers must be congruent to modulo 60.
 *****************************************************************************/
void sieve_of_atkin_algorithm_3_2(uint16_t *sieve, size_t sieve_len, int delta)
{
    for(int f = 1; f <= 10; ++f)
    {
        for(int g = 1; g <= 30; ++g)
        {
            if(delta == (3 * f * f + g * g) % 60)
            {
                sieve_of_atkin_algorithm_4_2(sieve, sieve_len, delta, f, g);
            }
        }
    }
}

/******************************************************************************
 * Algorithm 3.3.
 *
 * @param sieve Sieve of Atkin.
 * @param sieve_len Its length.
 * @param delta What each of the prime numbers must be congruent to modulo 60.
 *****************************************************************************/
void sieve_of_atkin_algorithm_3_3(uint16_t *sieve, size_t sieve_len, int delta)
{
    for(int f = 1; f <= 10; ++f)
    {
        for(int g = 1; g <= 30; ++g)
        {
            // We need residues modulo 60, so make negative remainders
            // positive.
            int remainder = (3 * f * f - g * g) % 60;
            remainder = remainder < 0 ? remainder + 60 : remainder;
            if(delta == remainder)
            {
                sieve_of_atkin_algorithm_4_3(sieve, sieve_len, delta, f, g);
            }
        }
    }
}

/******************************************************************************
 * Generate the sieve of Atkin.
 *
 * @param limit Non-strict upper bound up to which primes will be found.
 *
 * @return Sieve of Atkin.
 *****************************************************************************/
uint16_t *sieve_of_atkin(size_t limit)
{
    // Strict upper bound till which primes will be calculated.
    size_t limit_rounded = limit - limit % 60 + 60;
    if(limit_rounded <= limit)
    {
        fprintf(stderr, "Overflow detected. Argument is too large.\n");
        exit(EXIT_FAILURE);
    }

    size_t sieve_len = limit / 60 + 1;
    uint16_t *sieve = calloc(sieve_len, sizeof *sieve);
    for(int delta = 0; delta < 60; ++delta)
    {
        switch(algorithm[delta])
        {
            case 1:
            sieve_of_atkin_algorithm_3_1(sieve, sieve_len, delta);
            break;

            case 2:
            sieve_of_atkin_algorithm_3_2(sieve, sieve_len, delta);
            break;

            case 3:
            sieve_of_atkin_algorithm_3_3(sieve, sieve_len, delta);
            break;
        }
    }

    // Exclude squares of prime numbers and their multiples.
    for(size_t num = 1, num_div_60 = 0; num < limit_rounded; ++num_div_60)
    {
        for(size_t offsets_idx = 0, num_mod_60 = 1; offsets_idx < 16; ++offsets_idx)
        {
            if((sieve[num_div_60] >> shifts[num_mod_60] & 1) == 1)
            {
                for(size_t composite = num * num; composite < limit_rounded; composite += num * num)
                {
                    size_t sieve_idx = composite / 60;
                    size_t shifts_idx = composite % 60;
                    // A marginal speedup is obtained by avoiding a branch
                    // here. Instead of checking whether the value is 16, just
                    // use it with a number wider than 16 bits (thereby
                    // avoiding undefined behaviour).
                    sieve[sieve_idx] &= ~(1UL << shifts[shifts_idx]);
                }
            }
            num_mod_60 += offsets[offsets_idx];
            num += offsets[offsets_idx];
        }
    }

    return sieve;
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

    uint16_t *sieve = sieve_of_atkin(limit);
    size_t count = 0;
    for(size_t num = 1, num_div_60 = 0; num <= limit; ++num_div_60)
    {
        for(size_t offsets_idx = 0, num_mod_60 = 1; offsets_idx < 16 && num <= limit; ++offsets_idx)
        {
            if((sieve[num_div_60] >> shifts[num_mod_60] & 1) == 1)
            {
                ++count;
            }
            num_mod_60 += offsets[offsets_idx];
            num += offsets[offsets_idx];
        }
    }
    printf("%zu\n", count);

    return EXIT_SUCCESS;
}
