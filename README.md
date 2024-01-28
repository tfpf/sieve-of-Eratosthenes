* A. O. L. Atkin and D. J. Bernstein, "Prime Sieves Using Binary Quadratic Forms", in Mathematics of Computation,
  vol. 73, no. 246, pp. 1023–1030.
* D. J. Bernstein, [primegen 0.97](http://cr.yp.to/primegen.html).

# Sieve of Atkin

A prime sieving algorithm which is faster than a wheel-factorised sieve of Eratosthenes. I have implemented the
technique described by Atkin and Bernstein. While Bernstein has already provided an optimised implementation, I found
the code very hard to understand, and it does not store the list of primes, so I decided to write it myself.

Also included here is an non-optimal implementation of the sieve of Eratosthenes which I wrote a long time ago.

I rewrote this sieve of Atkin implementation in Rust to augment
[my Project Euler solutions](https://github.com/tfpf/project-euler) library.
