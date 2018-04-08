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

	TListaSimple raiz = buscarRaiz (datos, intervalo, regulaFalsi);

	GREATEST_ASSERT (L_Vacia (raiz) == FALSE);

	struct TElemRaiz elemRaiz;

	L_Elem_Cte(raiz, & elemRaiz);

	GREATEST_ASSERT_IN_RANGE(-2.391638050447, elemRaiz.raiz, 0.0000005);

	L_Vaciar(& raiz);

	// Masa = 0
	datos = cargarVectorDatos ();
	datos.masaParticula = 0;

	intervalo.intervaloMin = -1;
	intervalo.intervaloMax = +1.5;

	raiz = buscarRaiz (datos, intervalo, regulaFalsi);

	GREATEST_ASSERT (L_Vacia (raiz) == FALSE);

	L_Elem_Cte(raiz, & elemRaiz);

	GREATEST_ASSERT_IN_RANGE(0, elemRaiz.raiz, 0.0000005);

	L_Vaciar(& raiz);

	PASS ();

}

// Main
GREATEST_MAIN_DEFS ();

int correrTests () {

	GREATEST_MAIN_BEGIN ();

	RUN_TEST (testCargarDatos);
	RUN_TEST (testBuscarRaiz);

	GREATEST_MAIN_END ();

}
