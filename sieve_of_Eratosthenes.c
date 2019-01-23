#include<inttypes.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// main
int main(int const argc, char const **argv)
{
	// check arguments
	// if(argc != 2)
	// {
	// 	printf("usage:\n");
	// 	printf("\t./sieve.out <largest number>\n");
	// 	return 1;
	// }

	// largest number and its square root
	uint64_t limit, root_lim;
	limit = 98589673;
	root_lim = sqrt(limit);

	// allocate space for a binary array
	uint8_t *prime;
	prime = malloc((limit + 1) * sizeof *prime);
	if(prime == NULL)
	{
		printf("Memory error!\n");
		return 2;
	}
	memset(prime, 1, limit + 1); // set all array elements to 1

	uint64_t index, count; // counter

	// optimization: all multiples of 2 (except 2 itself) are composite
	for(index = 0; index <= limit; index += 2)
	{
		prime[index] = 0;
	}
	prime[1] = 0;
	prime[2] = 1;

	// remaining numbers
	for(index = 3; index <= root_lim; index += 2)
	{
		if(prime[index])
		{
			for(count = index * index; count <= limit; count += index)
			{
				prime[count] = 0;
			}
		}
	}

	// write the numbers to a file
	printf("writing data to \'primes.txt\'\n");
	FILE *result;
	result = fopen("primes.txt", "w");
	if(result == NULL)
	{
		printf("Could not open file for writing.\n");
		return 2;
	}
	for(index = 0; index <= limit; index++)
	{
		printf("%" PRIu64 " / %" PRIu64 "\r", index, limit);
		fflush(stdout);
		if(prime[index])
		{
			fprintf(result, "%26" PRIu64 "\n", index);
		}
	}
	fclose(result);

	return 0;
}
