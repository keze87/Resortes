#include "greatest.h"
#include "main.h"

TEST testPass (void) {

	PASS ();

}

// Main
GREATEST_MAIN_DEFS ();

int correrTests () {

	GREATEST_MAIN_BEGIN ();

	RUN_TEST (testPass);

	GREATEST_MAIN_END ();

}
