#include "greatest.h"
#include "main.h"

#define NP 97429

TEST testPass (void) {

	PASS ();

}

TEST testCargarDatos (void) {

	struct vectorDatos aux = cargarVectorDatos ();

	GREATEST_ASSERT_IN_RANGE (aux.longitudNatural, 2*100000/NP, 0.001);
	GREATEST_ASSERT_IN_RANGE (aux.masaParticula, 100000/NP, 0.001);
	GREATEST_ASSERT_EQ (aux.distEntreExtremosFijos, 1);
	GREATEST_ASSERT_EQ (aux.constElastica, 10);

	PASS();

}

// Main
GREATEST_MAIN_DEFS ();

int correrTests () {

	GREATEST_MAIN_BEGIN ();

	RUN_TEST (testPass);

	GREATEST_MAIN_END ();

}
