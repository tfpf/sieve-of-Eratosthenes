#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// A number may be prime if it has one of these residues modulo 60. Otherwise,
// it is definitely composite. (Exceptions are 2, 3 and 5, which are handled
// separately.)
static int residues[] = {1, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 49, 53, 59};

// Differences between consecutive residues.
static int offsets[] = {6, 4, 2, 4, 2, 4, 6, 2, 6, 4, 2, 4, 2, 4, 6, 2};

// Indices are residues; values are their positions in the array of residues.
// Values at other indices are -1.
static int shifts[] = {-1, 0, -1, -1, -1, -1, -1, 1, -1, -1, -1, 2, -1, 3, -1, -1, -1, 4, -1, 5, -1, -1, -1, 6, -1, -1, -1, -1, -1, 7, -1, 8, -1, -1, -1, -1, -1, 9, -1, -1, -1, 10, -1, 11, -1, -1, -1, 12, -1, 13, -1, -1, -1, 14, -1, -1, -1, -1, -1, 15};

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
        while((long long unsigned)k0 >= sieve_len)
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
            if(delta == (3 * f * f - g * g) % 60)
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
        switch(delta)
        {
            case 1: case 13: case 17: case 29: case 37: case 41: case 49: case 53:
            sieve_of_atkin_algorithm_3_1(sieve, sieve_len, delta);
            break;

            case 7: case 19: case 31: case 43:
            sieve_of_atkin_algorithm_3_2(sieve, sieve_len, delta);
            break;

            case 11: case 23: case 47: case 59:
            sieve_of_atkin_algorithm_3_3(sieve, sieve_len, delta);
            break;
        }
    }

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
                    int shift = shifts[shifts_idx];
                    if(shift != -1)
                    {
                        sieve[sieve_idx] &= ~((uint16_t)1 << shift);
                    }
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
    for(size_t num = 1, num_div_60 = 0; num <= limit; ++num_div_60)
    {
        for(size_t offsets_idx = 0, num_mod_60 = 1; offsets_idx < 16 && num <= limit; ++offsets_idx)
        {
            if((sieve[num_div_60] >> shifts[num_mod_60] & 1) == 1)
            {
                printf("%zu\n", num);
            }
            num_mod_60 += offsets[offsets_idx];
            num += offsets[offsets_idx];
        }
    }

    return EXIT_SUCCESS;
}
