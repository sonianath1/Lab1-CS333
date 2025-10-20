CC = gcc
DEBUG = -g
DEFINES =
CFLAGS = $(DEBUG) -Wall -Wshadow -Wunreachable-code -Wredundant-decls \
	 -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes \
	 -Wdeclaration-after-statement -Wextra -Wunsafe-loop-optimizations $(DEFINES) \
	 -Wuninitialized -Werror -Wno-return-local-addr

PROG1 = cae-xor
PROG2 = mystat
PROGS = $(PROG1) $(PROG2)

all: $(PROGS)

$(PROG1): $(PROG1).o
	$(CC) $(CFLAGS) -o $(PROG1) $(PROG1).o

$(PROG1).o: $(PROG1).c
	$(CC) $(CFLAGS) -c $(PROG1).c

$(PROG2): $(PROG2).o
	$(CC) $(CFLAGS) -o $(PROG2) $(PROG2).o

$(PROG2).o: $(PROG2).c
	$(CC) $(CFLAGS) -c $(PROG2).c

clean cls:
	rm -f $(PROGS) *.o *~ \#*

tar:
	tar cvfa Lab-1-${LOGNAME}.tar.gz *.[ch] [mM]akefile
