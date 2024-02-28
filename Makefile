CFLAGS = -std=c11 -O2 -Wall -Wextra -flto
CXXFLAGS = -std=c++11 -O2 -Wall -Wextra -flto
LDLIBS = -lm

Sources = src/sieve_of_eratosthenes.c src/sieve_of_atkin.c
Objects = $(Sources:.c=.o)
Main = src/main.c
Bench = src/bench.cc

sieve: $(Objects) $(Main)
	$(LINK.c) $^ -o $@ $(LDLIBS)

bench: $(Objects) $(Bench)
	$(LINK.cc) $^ -o $@ $(LDLIBS)
