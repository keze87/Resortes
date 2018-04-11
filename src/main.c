#include "tests.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#define  TRUE 0
#define  FALSE 1
#define  FRACASO -32000
#define  MAXITERACIONES 100

#define MAXRAICES 10

void imprimirEnunciado (short enunciado) {

	switch (enunciado) {

		case 1:
			printf ("\n1) Encontrar los puntos de equilibrio del sistema sin tener en cuenta el efecto de la gravedad.\n");
			printf ("   Hallar solamente el punto de equilibrio positivo con una tolerancia de 16 dígitos significativos.\n\n");
			break;

		case 2:
			printf ("\n2) Repetir el procedimiento realizado en 1) para el caso m=0.3*m0.\n");
			printf ("   Encontrar todos los puntos de equilibrio del sistema.\n\n");
			break;

		case 3:
			printf ("\n3) Para el método de Newton-Raphson aplicado al ítem 2) encontrar el máximo intervalo de convergencia de cada raíz.\n\n");
			break;

		case 4:
			printf ("\n4) Encontrar todos los puntos de equilibrio del sistema para los casos aplicando el método de Newton-Raphson:\n");
			printf ("   a) m=0.6*m0 b) m=0.9*m0 c) m=1.2*m0 d) m=1.5*m0\n\n");
			break;

		default :
			printf ("Error\n");

	}

}

typedef enum {

	NewtonRaphson,
	RegulaFalsi,
	PuntoFijo

} EMetodos;

struct retornoMetodo {

	double raiz;
	double error;
	double intervaloMin;
	double intervaloMax;

};

struct TVectorDatos {

	float longitudNatural;
	float masaParticula;
	int constElastica;
	int distEntreExtremosFijos;

};

struct TIntervalos {

	double intervaloMin;
 	double intervaloMax;

};

struct TVectorDatos cargarVectorDatos () {

	struct TVectorDatos aux;

	aux.longitudNatural	= 2.05;
	aux.masaParticula	= 1.02;
	aux.constElastica	= 10;
	aux.distEntreExtremosFijos = 1;

	return aux;

}

/*
 *
 * Manejo de lista
 *
 */

 struct TElemRaiz {

 	int k;
 	double intervaloMin;
 	double intervaloMax;
 	double funcIntervaloMin;
 	double funcIntervaloMax;
 	double raiz;
 	double errorAbs;
 	double errorRel;
 	double lambda;
 	double p;

 };

// https://github.com/fiuba-7541/elemental ?
/*
 * Movimientos que va a manejar la estructura. Son de conocimiento público,
 * pero sólo deberían usarse para el manejo puntual de esta estructura.
 */
typedef enum {

	L_Primero,
	L_Siguiente,
	L_Anterior

} TMovimiento_Ls;

/*
 * Estructura auxiliar de la lista simple. Es privada y no debe usarse bajo
 * ningún concepto en la aplicación.
 */
typedef struct TNodoListaSimple {

	void * Elem;
	struct TNodoListaSimple * Siguiente, * Anterior;

} TNodoListaSimple;

/**
 * Estructura cabecera, este es el tipo que se deberá instanciar, aunque
 * nunca acceder a sus campos.
 */
typedef struct {

	TNodoListaSimple * Primero, * Corriente;
	int TamanioDato;

} TListaSimple;

/**
 * L_CREAR
 * Pre: Ls no fue creada.
 * Post: Ls creada y vacia
 */
void L_Crear (TListaSimple * pLs, int TamanioDato) {

	pLs->Corriente = NULL;
	pLs->Primero = NULL;
	pLs->TamanioDato = TamanioDato;

}

/**
 * L_VACIAR
 * Pre: Ls creada.
 * Post: Ls vacia.
 */
void L_Vaciar (TListaSimple * pLs) {

	TNodoListaSimple * pNodo, * Siguiente;

	for(pNodo = pLs->Primero; (pNodo); pNodo = Siguiente) {

		Siguiente = pNodo->Siguiente;

		free(pNodo->Elem);
		free(pNodo);

	}

	pLs->Primero = pLs->Corriente = NULL;

}

/**
 * L_VACIA
 * Pre: Ls creada.
 * Post: Si Ls tiene elementos devuelve FALSE sino TRUE.
 */
int L_Vacia (TListaSimple Ls) {

	if (Ls.Primero == NULL)
		return TRUE;

	return FALSE;

}

/**
 * L_ELEM_CTE
 * Pre: Ls creada y no vacia.
 * Post: Se devuelve en E el elemento Corriente de la lista.
 */
void L_Elem_Cte (TListaSimple Ls, void * pE) {

	memcpy(pE, Ls.Corriente->Elem, Ls.TamanioDato);

}

/**
 * L_MOVER_CTE
 * Pre: Ls creada y no vacia.
 * Post: Si Ls esta vacia, devuelve FALSE. Sino:
 * Si M = L_Primero, el nuevo elemento Corriente es el Primero. Devuelve TRUE
 * Si M = L_Siguiente, el nuevo elemento Corriente es el Siguiente al
 * Corriente. Si estaba en el ultimo elemento, devuelve FALSE, sino TRUE.
 * Si M = L_Anterior, el nuevo elemento Corriente es el Anterior al
 * Corriente. Si estaba en el primer elemento, devuelve FALSE, sino TRUE.
 */
int L_Mover_Cte (TListaSimple * pLs, TMovimiento_Ls M) {

	switch (M) {

		case L_Primero:
			pLs->Corriente = pLs->Primero;
			break;

		case L_Siguiente:
			if (pLs->Corriente->Siguiente == NULL)
				return FALSE;
			else
				pLs->Corriente = pLs->Corriente->Siguiente;
			break;

		case L_Anterior:
			if (pLs->Corriente->Anterior == NULL)
				return FALSE;
			else
				pLs->Corriente = pLs->Corriente->Anterior;
			break;

	}

	return TRUE;

}

/**
 * L_INSERTAR_CTE
 * Pre: Ls creada.
 * Post: E se agrego a la lista y es el actual elemento Corriente.
 * Si M=L_Primero: se inserto como Primero de la lista.
 * Si M=L_Siguiente: se inserto despues del elemento Corriente.
 * Si M=L_Anterior: se inserto antes del elemento Corriente.
 * Si pudo insertar el elemento devuelve TRUE, sino FALSE.
 */
int L_Insertar_Cte (TListaSimple * pLs, TMovimiento_Ls M, void * pE) {

	TNodoListaSimple * pNodo = (TNodoListaSimple *) malloc(sizeof(TNodoListaSimple));

	if (!pNodo)
		return FALSE; // No hay memoria disponible

	pNodo->Elem = malloc(pLs->TamanioDato);

	if (!pNodo->Elem) {

		free(pNodo);

		return FALSE;

	}

	memcpy(pNodo->Elem, pE, pLs->TamanioDato);

	if ((pLs->Primero == NULL) || (M == L_Primero) || ((M == L_Anterior) && (pLs->Primero == pLs->Corriente))) {

		/*Si está vacía o hay que insertar en el Primero o
		hay que insertar en el Anterior y el actual es el Primero */
		pNodo->Siguiente = pLs->Primero;
		if (pLs->Primero != NULL)
			pLs->Primero->Anterior = pNodo;
		pLs->Primero = pLs->Corriente = pNodo;

	} else {

		// Siempre inserto como siguiente, el nodo nuevo, porque es más fácil
		pNodo->Siguiente = pLs->Corriente->Siguiente;
		pNodo->Anterior = pLs->Corriente;
		if (pLs->Corriente->Siguiente != NULL)
			pLs->Corriente->Siguiente->Anterior = pNodo;
		pLs->Corriente->Siguiente = pNodo;

		if (M == L_Anterior) {

			// Pero cuando el movimiento es Anterior, entonces swapeo los elementos
			void * tmp = pNodo->Elem;
			pNodo->Elem = pLs->Corriente->Elem;
			pLs->Corriente->Elem = tmp;

		}

	}

	pLs->Corriente = pNodo;

	return TRUE;

}

double funcion (struct TVectorDatos d, double y) {

	return -2 * d.constElastica * y * (1 - d.longitudNatural/sqrt(pow(y,2) + pow(d.distEntreExtremosFijos,2))) \
			- d.masaParticula * 9.81;

}

void buscarIntervalosDeRaices (struct TVectorDatos datos, TListaSimple * intervalos) {

	L_Crear(intervalos, sizeof(struct TIntervalos));

	struct TIntervalos aux;

	float y = -10.1 * datos.longitudNatural;

	// TODO: funcion = 0?
	while (y < 10.1 * datos.longitudNatural) {

		if (funcion (datos, y) * funcion (datos, y + 0.5) < 0) {

			aux.intervaloMin = y;
			aux.intervaloMax = y + 0.5;

			L_Insertar_Cte(intervalos, L_Siguiente, & aux);

		}

		y += 0.5;

	}

}

struct TRaiz {

	TListaSimple tabla;
	double raiz;
	double errorAbs;

};

struct TRaiz buscarRaizDentroDeIntervalo (struct TVectorDatos datos, struct TIntervalos intervalo,
						int (* metodo)(struct retornoMetodo *,double,double,struct TVectorDatos)) {

	struct TRaiz retorno;

	L_Crear (& retorno.tabla, sizeof(struct TElemRaiz));

	struct TElemRaiz elemIteracionK;

	struct retornoMetodo retornoMetodo;

	elemIteracionK.k = 0;
	elemIteracionK.intervaloMin = intervalo.intervaloMin;
	elemIteracionK.intervaloMax = intervalo.intervaloMax;
	elemIteracionK.funcIntervaloMin = funcion (datos, intervalo.intervaloMin);
	elemIteracionK.funcIntervaloMax = funcion (datos, intervalo.intervaloMax);

	int aux = metodo (& retornoMetodo, intervalo.intervaloMin, intervalo.intervaloMax, datos);

	while (aux == TRUE && elemIteracionK.k < MAXITERACIONES ) {

		elemIteracionK.raiz = retornoMetodo.raiz;
		elemIteracionK.errorAbs = retornoMetodo.error;

		if (retornoMetodo.raiz != 0)
			elemIteracionK.errorRel = fabs(retornoMetodo.error / retornoMetodo.raiz);
		else
			elemIteracionK.errorRel = FRACASO;

		if (elemIteracionK.k < 2) {
			elemIteracionK.lambda = FRACASO;
			elemIteracionK.p = FRACASO;
		} else {
			//TODO

			/* buscarLambdaYP(retorno,retornoMetodo,lamda,p)
			retorno sin &
			itero hacia atras, 3 veces? */

			elemIteracionK.lambda = 3;
			elemIteracionK.p = 1;
		}

		L_Insertar_Cte (& retorno.tabla, L_Siguiente, & elemIteracionK);

		elemIteracionK.k++;
		elemIteracionK.intervaloMin = retornoMetodo.intervaloMin;
		elemIteracionK.intervaloMax = retornoMetodo.intervaloMax;
		elemIteracionK.funcIntervaloMin = funcion (datos, retornoMetodo.intervaloMin);
		elemIteracionK.funcIntervaloMax = funcion (datos, retornoMetodo.intervaloMax);

		aux = metodo (& retornoMetodo, elemIteracionK.intervaloMin, elemIteracionK.intervaloMax, datos);

	}

	retorno.raiz = elemIteracionK.raiz;
	retorno.errorAbs = elemIteracionK.errorAbs;

	return retorno;

}

int regulaFalsi (struct retornoMetodo * retornoMetodo, double intervaloMin, double intervaloMax, struct TVectorDatos datos) {

	double puntoMedio;

	//TODO: definir error
	//TODO: corta por MAXITERACIONES
	if ( ! ((intervaloMin < intervaloMax) && (funcion (datos, intervaloMin) * funcion (datos, intervaloMax) < 0)) \
		|| (fabs (intervaloMin - intervaloMax) < 5E-15))
			return FALSE;

	puntoMedio = intervaloMin - funcion (datos, intervaloMin) * (intervaloMax - intervaloMin) / \
											(funcion (datos, intervaloMax) - funcion (datos, intervaloMin));

	if (funcion (datos, intervaloMin) * funcion (datos, puntoMedio) < 0) {
		retornoMetodo->intervaloMin = intervaloMin;
		retornoMetodo->intervaloMax = puntoMedio;
	} else {
		retornoMetodo->intervaloMin = puntoMedio;
		retornoMetodo->intervaloMax = intervaloMax;
	}

	retornoMetodo->error = fabs (retornoMetodo->intervaloMin - retornoMetodo->intervaloMax);
	retornoMetodo->raiz = puntoMedio;

	return TRUE;

}

void buscarTodasRaices (TListaSimple * raices, struct TVectorDatos datos, EMetodos metodo) {

	L_Crear (raices, sizeof(struct TRaiz));

	TListaSimple intervalosDeRaices;
	buscarIntervalosDeRaices (datos, & intervalosDeRaices);

	int aux = L_Mover_Cte (& intervalosDeRaices, L_Primero);

	while (aux == TRUE) {

		struct TIntervalos elem;
		L_Elem_Cte (intervalosDeRaices, & elem);

		//TODO: switch metodos
		struct TRaiz raiz = buscarRaizDentroDeIntervalo (datos, elem, regulaFalsi);

		L_Insertar_Cte (raices, L_Siguiente, & raiz);

		aux = L_Mover_Cte (& intervalosDeRaices, L_Siguiente);

	}

	L_Vaciar (& intervalosDeRaices);

}

void buscarPuntosDeEquilibrio (struct TVectorDatos datos, char opcion) {

	TListaSimple raices;

	switch (opcion) {

		case 1:
			datos.masaParticula = 0;
			buscarTodasRaices (& raices, datos, RegulaFalsi);
			break;

	}

	//buscarRaices (& raices, datos);
	//filtrarRaices(raices,opcion)
	//imprimirTabla(raices)
	//LimpiarLista
	//repetir con PF y NR

}

int proceso () {

	struct TVectorDatos datos = cargarVectorDatos ();

	imprimirEnunciado (1);
	buscarPuntosDeEquilibrio (datos, 1);

	//imprimirEnunciado (2);

	//imprimirEnunciado (3);

	//imprimirEnunciado (4);

	return TRUE;

}

int main () {

	int aux = correrTests (); // Pruebas Unitarias

	if (aux == TRUE)
		aux = proceso ();

	return aux;

}
