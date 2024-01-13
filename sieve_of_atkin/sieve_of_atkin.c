#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// A number may be prime if it has one of these residues modulo 60. Otherwise,
// it is definitely composite. (Exceptions are 2, 3 and 5, which are handled
// separately.)
static short residues[] = {1, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 49, 53, 59};

// Differences between consecutive residues.
static short unsigned offsets[] = {6, 4, 2, 4, 2, 4, 6, 2, 6, 4, 2, 4, 2, 4, 6, 2};

// Indices are residues; values are their positions in the array of residues.
// Values at other indices are 16.
static short unsigned shifts[] = {16, 0, 16, 16, 16, 16, 16, 1, 16, 16, 16, 2, 16, 3, 16, 16, 16, 4, 16, 5, 16, 16, 16, 6, 16, 16, 16, 16, 16, 7, 16, 8, 16, 16, 16, 16, 16, 9, 16, 16, 16, 10, 16, 11, 16, 16, 16, 12, 16, 13, 16, 16, 16, 14, 16, 16, 16, 16, 16, 15};

// Indices are residues; values indicate the algorithms to apply. (e.g. 1
// indicates that algorithms 3.1 and 4.1 should be applied; 0 indicates that
// no algorithms should be applied.)
static short algorithm[] = {0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 3, 0, 1, 0, 0, 0, 1, 0, 2, 0, 0, 0, 3, 0, 0, 0, 0, 0, 1, 0, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 2, 0, 0, 0, 3, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3};

// Indices are residues; values are arrays with which to begin the search.
static short triplets[][24][3] = {
    {},
    {{2, 15, 4}, {3, 5, 1}, {3, 25, 11}, {5, 9, 3}, {5, 21, 9}, {7, 15, 7}, {8, 15, 8}, {10, 9, 8}, {10, 21, 14}, {12, 5, 10}, {12, 25, 20}, {13, 15, 15}, {15, 1, 15}, {15, 11, 17}, {15, 19, 21}, {15, 29, 29}},
    {},
    {},
    {},
    {},
    {},
    {{1, 2, 0}, {1, 8, 1}, {1, 22, 8}, {1, 28, 13}, {3, 10, 2}, {3, 20, 7}, {7, 10, 4}, {7, 20, 9}, {9, 2, 4}, {9, 8, 5}, {9, 22, 12}, {9, 28, 17}},
    {},
    {},
    {},
    {{2, 1, 0}, {2, 11, -2}, {2, 19, -6}, {2, 29, -14}, {3, 4, 0}, {3, 14, -3}, {3, 16, -4}, {3, 26, -11}, {5, 2, 1}, {5, 8, 0}, {5, 22, -7}, {5, 28, -12}, {7, 4, 2}, {7, 14, -1}, {7, 16, -2}, {7, 26, -9}, {8, 1, 3}, {8, 11, 1}, {8, 19, -3}, {8, 29, -11}, {10, 7, 4}, {10, 13, 2}, {10, 17, 0}, {10, 23, -4}},
    {},
    {{1, 3, 0}, {1, 27, 12}, {4, 3, 1}, {4, 27, 13}, {6, 7, 3}, {6, 13, 5}, {6, 17, 7}, {6, 23, 11}, {9, 7, 6}, {9, 13, 8}, {9, 17, 10}, {9, 23, 14}, {11, 3, 8}, {11, 27, 20}, {14, 3, 13}, {14, 27, 25}},
    {},
    {},
    {},
    {{2, 1, 0}, {2, 11, 2}, {2, 19, 6}, {2, 29, 14}, {7, 1, 3}, {7, 11, 5}, {7, 19, 9}, {7, 29, 17}, {8, 1, 4}, {8, 11, 6}, {8, 19, 10}, {8, 29, 18}, {13, 1, 11}, {13, 11, 13}, {13, 19, 17}, {13, 29, 25}},
    {},
    {{1, 4, 0}, {1, 14, 3}, {1, 16, 4}, {1, 26, 11}, {5, 2, 1}, {5, 8, 2}, {5, 22, 9}, {5, 28, 14}, {9, 4, 4}, {9, 14, 7}, {9, 16, 8}, {9, 26, 15}},
    {},
    {},
    {},
    {{1, 10, -2}, {1, 20, -7}, {2, 7, -1}, {2, 13, -3}, {2, 17, -5}, {2, 23, -9}, {3, 2, 0}, {3, 8, -1}, {3, 22, -8}, {3, 28, -13}, {4, 5, 0}, {4, 25, -10}, {6, 5, 1}, {6, 25, -9}, {7, 2, 2}, {7, 8, 1}, {7, 22, -6}, {7, 28, -11}, {8, 7, 2}, {8, 13, 0}, {8, 17, -2}, {8, 23, -6}, {9, 10, 2}, {9, 20, -3}},
    {},
    {},
    {},
    {},
    {},
    {{1, 5, 0}, {1, 25, 10}, {4, 5, 1}, {4, 25, 11}, {5, 7, 2}, {5, 13, 4}, {5, 17, 6}, {5, 23, 10}, {10, 7, 7}, {10, 13, 9}, {10, 17, 11}, {10, 23, 15}, {11, 5, 8}, {11, 25, 18}, {14, 5, 13}, {14, 25, 23}},
    {},
    {{3, 2, 0}, {3, 8, 1}, {3, 22, 8}, {3, 28, 13}, {5, 4, 1}, {5, 14, 4}, {5, 16, 5}, {5, 26, 12}, {7, 2, 2}, {7, 8, 3}, {7, 22, 10}, {7, 28, 15}},
    {},
    {},
    {},
    {},
    {},
    {{2, 9, 1}, {2, 21, 7}, {3, 1, 0}, {3, 11, 2}, {3, 19, 6}, {3, 29, 14}, {7, 9, 4}, {7, 21, 10}, {8, 9, 5}, {8, 21, 11}, {12, 1, 9}, {12, 11, 11}, {12, 19, 15}, {12, 29, 23}, {13, 9, 12}, {13, 21, 18}},
    {},
    {},
    {},
    {{2, 5, 0}, {2, 25, 10}, {5, 1, 1}, {5, 11, 3}, {5, 19, 7}, {5, 29, 15}, {7, 5, 3}, {7, 25, 13}, {8, 5, 4}, {8, 25, 14}, {10, 1, 6}, {10, 11, 8}, {10, 19, 12}, {10, 29, 20}, {13, 5, 11}, {13, 25, 21}},
    {},
    {{1, 10, 1}, {1, 20, 6}, {3, 4, 0}, {3, 14, 3}, {3, 16, 4}, {3, 26, 11}, {7, 4, 2}, {7, 14, 5}, {7, 16, 6}, {7, 26, 13}, {9, 10, 5}, {9, 20, 10}},
    {},
    {},
    {},
    {{1, 4, -1}, {1, 14, -4}, {1, 16, -5}, {1, 26, -12}, {2, 5, -1}, {2, 25, -11}, {3, 10, -2}, {3, 20, -7}, {4, 1, 0}, {4, 11, -2}, {4, 19, -6}, {4, 29, -14}, {6, 1, 1}, {6, 11, -1}, {6, 19, -5}, {6, 29, -13}, {7, 10, 0}, {7, 20, -5}, {8, 5, 2}, {8, 25, -8}, {9, 4, 3}, {9, 14, 0}, {9, 16, -1}, {9, 26, -8}},
    {},
    {{1, 15, 3}, {4, 15, 4}, {5, 3, 1}, {5, 27, 13}, {6, 5, 2}, {6, 25, 12}, {9, 5, 5}, {9, 25, 15}, {10, 3, 6}, {10, 27, 18}, {11, 15, 11}, {14, 15, 16}, {15, 7, 15}, {15, 13, 17}, {15, 17, 19}, {15, 23, 23}},
    {},
    {},
    {},
    {{1, 7, 0}, {1, 13, 2}, {1, 17, 4}, {1, 23, 8}, {4, 7, 1}, {4, 13, 3}, {4, 17, 5}, {4, 23, 9}, {11, 7, 8}, {11, 13, 10}, {11, 17, 12}, {11, 23, 16}, {14, 7, 13}, {14, 13, 15}, {14, 17, 17}, {14, 23, 21}},
    {},
    {},
    {},
    {},
    {},
    {{1, 2, -1}, {1, 8, -2}, {1, 22, -9}, {1, 28, -14}, {4, 7, -1}, {4, 13, -3}, {4, 17, -5}, {4, 23, -9}, {5, 4, 0}, {5, 14, -3}, {5, 16, -4}, {5, 26, -11}, {6, 7, 0}, {6, 13, -2}, {6, 17, -4}, {6, 23, -8}, {9, 2, 3}, {9, 8, 2}, {9, 22, -5}, {9, 28, -10}, {10, 1, 4}, {10, 11, 2}, {10, 19, -2}, {10, 29, -10}},
};

/******************************************************************************
 * Algorithm 4.1.
 *
 * @param sieve Sieve of Atkin.
 * @param sieve_len Its length.
 * @param delta What each of the prime numbers must be congruent to modulo 60.
 * @param x Starting abcissa.
 * @param y0 Starting ordinate.
 * @param k0 Starting index into `sieve`.
 *****************************************************************************/
void sieve_of_atkin_algorithm_4_1(uint16_t *sieve, size_t sieve_len, int delta, long long x, long long y0, long long k0)
{
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
 * @param k0 Starting index into `sieve`.
 *****************************************************************************/
void sieve_of_atkin_algorithm_4_2(uint16_t *sieve, size_t sieve_len, int delta, long long x, long long y0, long long k0)
{
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
 * @param k0 Starting index into `sieve`.
 *****************************************************************************/
void sieve_of_atkin_algorithm_4_3(uint16_t *sieve, size_t sieve_len, int delta, long long x, long long y0, long long k0)
{
    while(true)
    {
        // It can be negative here, so check its sign before casting it to an
        // unsigned type. If it is negative, it is automatically less than the
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
    for(size_t i = 0; i < 16; ++i)
    {
        sieve_of_atkin_algorithm_4_1(sieve, sieve_len, delta, triplets[delta][i][0], triplets[delta][i][1], triplets[delta][i][2]);
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
    for(size_t i = 0; i < 12; ++i)
    {
        sieve_of_atkin_algorithm_4_2(sieve, sieve_len, delta, triplets[delta][i][0], triplets[delta][i][1], triplets[delta][i][2]);
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
    for(size_t i = 0; i < 24; ++i)
    {
        sieve_of_atkin_algorithm_4_3(sieve, sieve_len, delta, triplets[delta][i][0], triplets[delta][i][1], triplets[delta][i][2]);
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
                    short shift = shifts[shifts_idx];
                    // A marginal speedup is obtained by avoiding a branch
                    // here. Instead of checking whether the value is 16, just
                    // use it with a number wider than 16 bits (thereby
                    // avoiding undefined behaviour).
                    sieve[sieve_idx] &= ~(1UL << shift);
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
