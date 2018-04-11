#include "greatest.h"
#include "main.h"

#define NP 97429

TEST testCargarDatos (void) {

	struct TVectorDatos aux = cargarVectorDatos ();

	GREATEST_ASSERT_IN_RANGE ((float) 2*100000/NP, aux.longitudNatural, 0.01);
	GREATEST_ASSERT_IN_RANGE ((float) 100000/NP, aux.masaParticula, 0.01);
	GREATEST_ASSERT_EQ (1, aux.distEntreExtremosFijos);
	GREATEST_ASSERT_EQ (10, aux.constElastica);

	PASS();

}

TEST testBuscarRaiz (void) {

	// Masa = m0
	struct TVectorDatos datos = cargarVectorDatos ();

	struct TIntervalos intervalo;
	intervalo.intervaloMin = -3;
	intervalo.intervaloMax = 0;

	TListaSimple raiz = buscarRaizDentroDeIntervalo (datos, intervalo, regulaFalsi);
	GREATEST_ASSERT (L_Vacia (raiz) == FALSE);

	struct TElemRaiz elemRaiz;
	L_Elem_Cte (raiz, & elemRaiz); // Solo me fijo en el resultado
	GREATEST_ASSERT_IN_RANGE (-2.391638050447, elemRaiz.raiz, 0.0000005);

	L_Vaciar (& raiz);

	// Masa = 0
	datos = cargarVectorDatos ();
	datos.masaParticula = 0;

	intervalo.intervaloMin = -1;
	intervalo.intervaloMax = +1.5;

	raiz = buscarRaizDentroDeIntervalo (datos, intervalo, regulaFalsi);
	GREATEST_ASSERT (L_Vacia (raiz) == FALSE);

	L_Elem_Cte (raiz, & elemRaiz); // Solo me fijo en el resultado
	GREATEST_ASSERT_IN_RANGE (0, elemRaiz.raiz, 0.0000005);

	L_Vaciar (& raiz);
	PASS ();

}

TEST testBuscarRaices (void) {

	//void buscarTodasRaices (TListaSimple * raices, struct TVectorDatos datos, EMetodos metodo)

	printf ("m=m0\n");
	struct TVectorDatos datos = cargarVectorDatos ();
	TListaSimple raices;
	buscarTodasRaices (& raices, datos, RegulaFalsi); // TODO: otros metodos?

	GREATEST_ASSERT (L_Vacia (raices) == FALSE);

	struct TRaiz raiz;

	GREATEST_ASSERT (L_Mover_Cte (& raices, L_Primero) == TRUE);
	L_Elem_Cte (raices, & raiz); printf ("Raiz = %f +- %f\n", raiz.raiz, raiz.errorAbs);
	GREATEST_ASSERT (L_Mover_Cte (& raices, L_Siguiente) == FALSE); // Una sola raiz

	L_Vaciar (& raices);

	printf ("m=0\n");
	datos.masaParticula = 0;

	buscarTodasRaices (& raices, datos, RegulaFalsi); // TODO: otros metodos?

	GREATEST_ASSERT (L_Vacia (raices) == FALSE);

	GREATEST_ASSERT (L_Mover_Cte (& raices, L_Primero) == TRUE);
	L_Elem_Cte (raices, & raiz); printf ("Raiz = %f +- %f\n", raiz.raiz, raiz.errorAbs);
	GREATEST_ASSERT (L_Mover_Cte (& raices, L_Siguiente) == TRUE);
	L_Elem_Cte (raices, & raiz); printf ("Raiz = %f +- %f\n", raiz.raiz, raiz.errorAbs);
	GREATEST_ASSERT (L_Mover_Cte (& raices, L_Siguiente) == TRUE);
	L_Elem_Cte (raices, & raiz); printf ("Raiz = %f +- %f\n", raiz.raiz, raiz.errorAbs);
	GREATEST_ASSERT (L_Mover_Cte (& raices, L_Siguiente) == FALSE); // 3 raices

	L_Vaciar (& raices);
	PASS ();

}

// Main
GREATEST_MAIN_DEFS ();

int correrTests () {

	GREATEST_MAIN_BEGIN ();

	RUN_TEST (testCargarDatos);
	RUN_TEST (testBuscarRaiz);
	RUN_TEST (testBuscarRaices);

	GREATEST_MAIN_END ();

}
