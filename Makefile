CFLAGS = -std=c11 -O2 -Wall -Wextra -flto
LDLIBS = -lm

Sources = $(wildcard src/*.c)
Objects = $(Sources:.c=.o)

sieve: $(Objects)
	$(LINK.c) $^ -o $@ $(LDLIBS)
