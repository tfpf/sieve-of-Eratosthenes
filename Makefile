SHELL  = /bin/sh
CC     = gcc
CFLAGS = -O2 -std=c11 -Wall -Wextra
LDLIBS =
RM     = rm -f

Sources    = $(wildcard *.c)
Executable = sieve

.PHONY: clean

$(Executable): $(Sources)
	$(CC) $(CFLAGS) -o $(Executable) $(Sources) $(LDLIBS)

clean:
	$(RM) $(Executable)
