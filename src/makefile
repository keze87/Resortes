CC=LC_ALL=C gcc -g -lm -std=c11 -Wall -pedantic -pedantic-errors

OBJS=main.c main.h greatest.h tests.c tests.h

all: pre-build main-build

pre-build:
	@$(MAKE) --no-print-directory clean
	printf "\n\n"
	cppcheck --language=c --enable=all *.c |& grep -v "is never used."

post-build:
	printf "\n\n"
	./main || echo "ERROR: $$?"

main-build: main

main: $(OBJS)
	printf "\n\n"
	$(CC) -o $@ $(OBJS)
	@$(MAKE) --no-print-directory post-build

clean:
	rm main *.o || true
