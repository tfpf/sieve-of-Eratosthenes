* A. O. L. Atkin and D. J. Bernstein, "Prime Sieves Using Binary Quadratic Forms", in Mathematics of Computation,
  vol. 73, no. 246, pp. 1023–1030.
* D. J. Bernstein, [primegen 0.97](http://cr.yp.to/primegen.html).

# Sieve of Atkin

A fast prime generator. I have implemented the technique described by Atkin and Bernstein (as opposed to brute-forcing
quadratic congruences). While Bernstein has already provided an optimised implementation, I found the code very hard to
understand, and it does not store the list of primes, so I decided to write it myself.

Also included here is a wheel-factorised sieve of Eratosthenes implementation which proceeds mostly along the same
lines as the sieve of Atkin.

## Implementation Notes

Both sieves start generating primes from 7 onwards. The first three primes (i.e. 2, 3 and 5) must be handled
separately.

## Performance

The sieve of Atkin is faster than the sieve of Eratosthenes. Tabulated below is the time taken (to two significant
figures) to construct the sieves up to _n_ on my machine. Note that the time taken to iterate over the primes (after constructing the sieves) will, in theory, be identical because they store the list of primes in the same manner.

|_n_|Eratosthenes|Atkin|
|-:|-:|-:|
|2<sup>16</sup>|3 ms|3 ms|
|2<sup>17</sup>|3 ms|3 ms|
|2<sup>18</sup>|4 ms|3 ms|
|2<sup>19</sup>|5 ms|4 ms|
|2<sup>20</sup>|7 ms|4 ms|
|2<sup>21</sup>|11 ms|5 ms|
|2<sup>22</sup>|18 ms|6 ms|
|2<sup>23</sup>|35 ms|9 ms|
|2<sup>24</sup>|71 ms|15 ms|
|2<sup>25</sup>|140 ms|28 ms|
|2<sup>26</sup>|300 ms|61 ms|
|2<sup>27</sup>|680 ms|210 ms|
|2<sup>28</sup>|1800 ms|610 ms|
|2<sup>29</sup>|3900 ms|1600 ms|
|2<sup>30</sup>|8700 ms|3200 ms|

## Related

I rewrote this sieve of Atkin implementation in Rust to augment
[my Project Euler solutions](https://github.com/tfpf/project-euler) library.
