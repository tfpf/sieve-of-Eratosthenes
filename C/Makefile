CPPFLAGS = -Isrc/include
CFLAGS = -std=c11 -O2 -Wall -Wextra -flto
CXXFLAGS = -std=c++11 -O2 -Wall -Wextra -flto
LDLIBS = -lm

Sources = src/sieve_of_eratosthenes.c src/sieve_of_atkin.c
Objects = $(Sources:.c=.o)

.PHONY: all

all: target/sieve target/bench target/tests

target/sieve: $(Objects) src/main.c
	$(LINK.c) $^ -o $@ $(LDLIBS)

target/bench: $(Objects) bench/bench.cc
	$(LINK.cc) $^ -o $@ $(LDLIBS)

target/tests: $(Objects) tests/tests.c
	$(LINK.c) $^ -o $@ $(LDLIBS)
