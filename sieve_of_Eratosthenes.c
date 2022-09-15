#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * Find a bit in an array.
 *
 * @param composite_array Array to look in.
 * @param position Position of the bit to look for.
 *
 * @return The bit.
 *****************************************************************************/
bool composite_array_get(uint64_t *composite_array, int long long position)
{
    int long long index = position / 64;
    uint64_t mask = (uint64_t)1 << (position % 64);
    return (composite_array[index] & mask) != 0;
}

/******************************************************************************
 * Change a bit in an array.
 *
 * @param composite_array Array to look in.
 * @param position Position of the bit to change.
 * @param new_value Value to set the bit to.
 *****************************************************************************/
void composite_array_set(uint64_t *composite_array, int long long position, bool new_value)
{
    int long long index = position / 64;
    uint64_t mask = (uint64_t)1 << (position % 64);
    if(new_value)
    {
        composite_array[index] |= mask;
    }
    else
    {
        composite_array[index] &= ~mask;
    }
}

/******************************************************************************
 * Main function.
 *****************************************************************************/
int main(int const argc, char const *argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  %s <number>\n", argv[0]);
        fprintf(stderr, "where <number> is an integer greater than 1.\n");
        return EXIT_FAILURE;
    }

    char *endptr;
    errno = 0;
    int long long limit = strtoll(argv[1], &endptr, 10);
    if(*endptr != '\0' || errno == ERANGE || limit <= 1 || limit == LLONG_MAX)
    {
        fprintf(stderr, "Argument '%s' is invalid or out of range.\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Allocate memory for an array. This array will be treated as a string of
    // bits; each bit tells us whether the corresponding number is composite or
    // prime.
    int long long composite_array_size = (limit + 1) / 64;
    if((limit + 1) % 64 != 0)
    {
        ++composite_array_size;
    }
    uint64_t *composite_array = calloc(composite_array_size, sizeof *composite_array);
    if(composite_array == NULL)
    {
        fprintf(stderr, "Could not allocate %lld GiB of memory.\n", limit >> 33);
        return EXIT_FAILURE;
    }
    fprintf(stderr, "Allocated approximately %lld KiB (%lld MiB) to store the sieve.\n", limit >> 13, limit >> 23);

    // Initially, all bits are cleared. By the end, only the bits corresponding
    // to prime numbers remain cleared.
    for(int long long p = 2; p * p <= limit; ++p)
    {
        // If this number is prime, the corresponding bit in the array will be
        // cleared. If so, mark all its multiples as composite.
        if(!composite_array_get(composite_array, p))
        {
            // All multiples below the square of this number have already been
            // processed. Just look at larger multiples.
            for(int long long multiple = p * p; multiple <= limit; multiple += p)
            {
                composite_array_set(composite_array, multiple, true);
            }
        }
    }

    for(int long long p = 0; p <= limit; ++p)
    {
        if(!composite_array_get(composite_array, p))
        {
            printf("%lld\n", p);
        }
    }
    return EXIT_SUCCESS;
}
