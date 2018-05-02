CC=LC_ALL=C gcc -g -std=c11 -Wall -pedantic -pedantic-errors

OBJS=CT04.c CT04.h greatest.h tests.c tests.h

all: pre-build main-build

pre-build:
	@$(MAKE) --no-print-directory clean
	# printf "\n\n"
	# cppcheck --language=c --enable=all *.c |& grep -v "is never used."

post-build:
	printf "\n\n"
	./main || echo "ERROR: $$?"

main-build: main

main: $(OBJS)
	printf "\n\n"
	$(CC) -o $@ $(OBJS) -lm
	@$(MAKE) --no-print-directory post-build

clean:
	rm main *.o || true
