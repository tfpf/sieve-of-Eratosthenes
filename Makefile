CFLAGS = -std=c11 -O2 -Wall -Wextra -flto
CXXFLAGS = -std=c++11 -O2 -Wall -Wextra -flto
LDLIBS = -lm

Sources = src/sieve_of_eratosthenes.c src/sieve_of_atkin.c
Objects = $(Sources:.c=.o)

.PHONY: sieve bench

sieve: target/sieve

target/sieve: $(Objects) src/main.c
	$(LINK.c) $^ -o $@ $(LDLIBS)

bench: target/bench

target/bench: $(Objects) bench/bench.cc
	$(LINK.cc) $^ -o $@ $(LDLIBS)
