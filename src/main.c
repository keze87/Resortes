#include "tests.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#define TRUE 0
#define FALSE 1
#define FRACASO -32000
#define MAXITERACIONES 1000
#define TAMMATRIZX 10
#define TAMMATRIZY 22
#define MAXRAICES 10

void imprimirEnunciado (char enunciado) {

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

/*
 *
 * Manejo de lista
 *
 */

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

	for (pNodo = pLs->Primero; (pNodo); pNodo = Siguiente) {
		Siguiente = pNodo->Siguiente;
		free (pNodo->Elem);
		free (pNodo);
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

	memcpy (pE, Ls.Corriente->Elem, Ls.TamanioDato);

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
 * L_BORRAR_CTE
 * Pre: Ls creada y no vacia.
 * Post: Se elimino el elemento Corriente, El nuevo elemento es el Siguiente o
 * el Anterior si el Corriente era el ultimo elemento.
 */
void L_Borrar_Cte (TListaSimple * pLs) {

	TNodoListaSimple * pNodo = pLs->Corriente;

	if (pLs->Corriente == pLs->Primero)
	{
		pLs->Primero = pLs->Primero->Siguiente;
		pLs->Corriente = pLs->Primero;
		if (pLs->Corriente != NULL)
			pLs->Corriente->Anterior = NULL;
	} else {
		if (pLs->Corriente->Siguiente) {
			pNodo = pLs->Corriente->Siguiente;
			memcpy (pLs->Corriente->Elem, pNodo->Elem, pLs->TamanioDato);
			pLs->Corriente->Siguiente = pNodo->Siguiente;
			if (pLs->Corriente->Siguiente != NULL)
				pLs->Corriente->Siguiente->Anterior = pLs->Corriente;
		} else {
			/* Si es el último queda en el Anterior al borrado */
			pLs->Corriente->Anterior->Siguiente = NULL;
			pLs->Corriente = pLs->Corriente->Anterior;
		}
	}

	free (pNodo->Elem);
	free (pNodo);

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

	TNodoListaSimple * pNodo = (TNodoListaSimple *) malloc (sizeof(TNodoListaSimple));

	if (!pNodo)
		return FALSE; // No hay memoria disponible

	pNodo->Elem = malloc(pLs->TamanioDato);

	if (!pNodo->Elem) {
		free (pNodo);
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

typedef enum {

	NewtonRaphson,
	RegulaFalsi,
	PuntoFijo

} EMetodos;

struct TRetornoMetodo {

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

	aux.longitudNatural = 2.05;
	aux.masaParticula = 1.02;
	aux.constElastica = 10;
	aux.distEntreExtremosFijos = 1;

	return aux;

}

struct TElemRaiz {

	int k;
	double intervaloMin;
	double intervaloMax;
	double funcIntervaloMin;
	double funcIntervaloMax;
	double raiz;
	double errorAbs;
	double errorRel;
	float lambda;
	float rho;

};

struct TRaiz {

	TListaSimple iteraciones;
	double raiz;
	double errorAbs;
	int k;

};

double funcion (struct TVectorDatos d, double y) {

	return -2 * d.constElastica * y * (1 - d.longitudNatural/sqrt(pow(y,2) + pow(d.distEntreExtremosFijos,2))) \
			- d.masaParticula * 9.81;

}

void buscarIntervalosDeRaices (struct TVectorDatos datos, TListaSimple * intervalos) {

	L_Crear (intervalos, sizeof(struct TIntervalos));

	struct TIntervalos aux;

	float y = -10.1 * datos.longitudNatural;

	// TODO: funcion = 0?
	while (y < 10.1 * datos.longitudNatural) {
		if (funcion (datos, y) * funcion (datos, y + 0.5) < 0) {
			aux.intervaloMin = y;
			aux.intervaloMax = y + 0.5;

			L_Insertar_Cte (intervalos, L_Siguiente, & aux);
		}

		y += 0.5;
	}

}

void aproximarLambdaYP (TListaSimple iteraciones, struct TRetornoMetodo ultimaIteracion, float * lambda, float * rho) {

	double xKMas1;
	double xK;
	double xKMenos1;
	double xKMenos2;

	xKMas1 = ultimaIteracion.raiz;

	struct TElemRaiz elemAux;
	L_Elem_Cte (iteraciones, & elemAux);
	xK = elemAux.raiz;

	L_Mover_Cte (& iteraciones, L_Anterior);
	L_Elem_Cte (iteraciones, & elemAux);
	xKMenos1 = elemAux.raiz;

	L_Mover_Cte (& iteraciones, L_Anterior);
	L_Elem_Cte (iteraciones, & elemAux);
	xKMenos2 = elemAux.raiz;

	double deltaXKMas1 = fabs (xKMas1 - xK);
	double deltaXK = fabs (xK - xKMenos1);
	double deltaXKMenos1 = fabs (xKMenos1 - xKMenos2);

	* rho = log (deltaXKMas1 / deltaXK) / log (deltaXK / deltaXKMenos1);

	* lambda = deltaXKMas1 / pow (deltaXK, * rho);

}

struct TRaiz buscarRaizDentroDeIntervaloMetodoDeConv (struct TVectorDatos datos, struct TIntervalos intervalo,
						int (* metodo)(struct TRetornoMetodo *,double,double,struct TVectorDatos)) {

	struct TRaiz tabla;

	return tabla;

}

struct TRaiz buscarRaizDentroDeIntervaloMetodoArranque (struct TVectorDatos datos, struct TIntervalos intervalo,
						int (* metodo)(struct TRetornoMetodo *,double,double,struct TVectorDatos)) {

	struct TRaiz tabla;

	L_Crear (& tabla.iteraciones, sizeof(struct TElemRaiz));

	struct TElemRaiz elemIteracionK;

	struct TRetornoMetodo retornoMetodo;

	elemIteracionK.k = 0;
	elemIteracionK.intervaloMin = intervalo.intervaloMin;
	elemIteracionK.intervaloMax = intervalo.intervaloMax;
	elemIteracionK.funcIntervaloMin = funcion (datos, intervalo.intervaloMin);
	elemIteracionK.funcIntervaloMax = funcion (datos, intervalo.intervaloMax);

	int aux = metodo (& retornoMetodo, intervalo.intervaloMin, intervalo.intervaloMax, datos);

	while (aux == TRUE && elemIteracionK.k < MAXITERACIONES) {
		elemIteracionK.raiz = retornoMetodo.raiz;
		elemIteracionK.errorAbs = retornoMetodo.error;

		if (retornoMetodo.raiz != 0)
			elemIteracionK.errorRel = fabs (retornoMetodo.error / retornoMetodo.raiz);
		else
			elemIteracionK.errorRel = FRACASO;

		if (elemIteracionK.k <= 2) {
			elemIteracionK.lambda = FRACASO;
			elemIteracionK.rho = FRACASO;
		} else
			aproximarLambdaYP (tabla.iteraciones, retornoMetodo, & elemIteracionK.lambda, & elemIteracionK.rho);

		L_Insertar_Cte (& tabla.iteraciones, L_Siguiente, & elemIteracionK);

		elemIteracionK.k++;
		elemIteracionK.intervaloMin = retornoMetodo.intervaloMin;
		elemIteracionK.intervaloMax = retornoMetodo.intervaloMax;
		elemIteracionK.funcIntervaloMin = funcion (datos, retornoMetodo.intervaloMin);
		elemIteracionK.funcIntervaloMax = funcion (datos, retornoMetodo.intervaloMax);

		aux = metodo (& retornoMetodo, elemIteracionK.intervaloMin, elemIteracionK.intervaloMax, datos);
	}

	tabla.raiz = elemIteracionK.raiz;
	tabla.errorAbs = elemIteracionK.errorAbs;
	tabla.k = elemIteracionK.k;

	return tabla;

}

int regulaFalsi (struct TRetornoMetodo * retornoMetodo, double intervaloMin, double intervaloMax, struct TVectorDatos datos) {

	double puntoMedio;

	//TODO: definir error
	//TODO: corta por MAXITERACIONES
	if ((intervaloMin >= intervaloMax) || (funcion (datos, intervaloMin) * funcion (datos, intervaloMax) >= 0) \
		|| (fabs (intervaloMin - intervaloMax) < 5E-25))
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

// TODO
int newtonRaphson (struct TRetornoMetodo * retornoMetodo, double intervaloMin, double intervaloMax, struct TVectorDatos datos) {

	return FRACASO;

}

int puntoFijo (struct TRetornoMetodo * retornoMetodo, double intervaloMin, double intervaloMax, struct TVectorDatos datos) {

	return FRACASO;

}

void buscarTodasRaices (TListaSimple * raices, struct TVectorDatos datos, EMetodos metodo) {

	L_Crear (raices, sizeof(struct TRaiz));

	TListaSimple intervalosDeRaices;
	buscarIntervalosDeRaices (datos, & intervalosDeRaices);

	int aux = L_Mover_Cte (& intervalosDeRaices, L_Primero);

	while (aux == TRUE) {
		struct TIntervalos elem;
		L_Elem_Cte (intervalosDeRaices, & elem);

		struct TRaiz raiz;
		switch (metodo) {
			case RegulaFalsi:
				raiz = buscarRaizDentroDeIntervaloMetodoArranque (datos, elem, regulaFalsi);
				break;
			case NewtonRaphson:
				raiz = buscarRaizDentroDeIntervaloMetodoDeConv (datos, elem, newtonRaphson);
				break;
			case PuntoFijo:
				raiz = buscarRaizDentroDeIntervaloMetodoDeConv (datos, elem, puntoFijo);
				break;
		}

		L_Insertar_Cte (raices, L_Siguiente, & raiz);

		aux = L_Mover_Cte (& intervalosDeRaices, L_Siguiente);
	}

	L_Vaciar (& intervalosDeRaices);

}

void filtrarRaices (TListaSimple * raices, char opcion) {

	int aux;
	struct TRaiz raiz;

	switch (opcion) {
		case 1:
			aux = L_Mover_Cte (raices, L_Primero);

			while (aux == TRUE) {
				L_Elem_Cte (* raices, & raiz);

				if (raiz.raiz <= 0) {
					L_Vaciar (& raiz.iteraciones);
					L_Borrar_Cte (raices);
				} else
					aux = L_Mover_Cte (raices, L_Siguiente);
			}

			break;
	}

}

void limpiarRaices (TListaSimple * raices) {

	int aux = L_Mover_Cte (raices, L_Primero);

	while (aux == TRUE) {
		struct TRaiz raiz;
		L_Elem_Cte (* raices, & raiz);
		L_Vaciar (& raiz.iteraciones);

		aux = L_Mover_Cte (raices, L_Siguiente);
	}

	L_Vaciar (raices);

}

/*
 *
 * Imprimir a pantalla
 *
 */

void cargarMatrizRedondeadaCompleta (char * matriz[TAMMATRIZX][TAMMATRIZY], struct TRaiz raiz) {

	int aux = L_Mover_Cte (& raiz.iteraciones, L_Primero);
	int y = 1;

	while (aux == TRUE) {
		struct TElemRaiz iteracion;
		L_Elem_Cte (raiz.iteraciones, & iteracion);

		snprintf (matriz[0][y], 29, "%d", iteracion.k);
		snprintf (matriz[1][y], 29, "%f", iteracion.intervaloMin);
		snprintf (matriz[2][y], 29, "%f", iteracion.intervaloMax);
		snprintf (matriz[3][y], 29, "%.1e", iteracion.funcIntervaloMin);
		snprintf (matriz[4][y], 29, "%.1e", iteracion.funcIntervaloMax);
		snprintf (matriz[5][y], 29, "%.16f", iteracion.raiz); // TODO: 16 digitos significativos .16f -> string -> contar numeros != 0
		snprintf (matriz[6][y], 29, "%.1e", iteracion.errorAbs);
		snprintf (matriz[7][y], 29, "%.1e", iteracion.errorRel);
		snprintf (matriz[8][y], 29, "%f", iteracion.lambda);
		snprintf (matriz[9][y], 29, "%f", iteracion.rho);

		aux = L_Mover_Cte (& raiz.iteraciones, L_Siguiente);
		y++;
	}

 }


void cargarMatrizRedondeadaIncompleta (char * matriz[TAMMATRIZX][TAMMATRIZY], struct TRaiz raiz) {

}

// Llena una matriz (para imprimir) con los títulos y los números redondeados
void cargarMatrizRedondeada (char * matriz[TAMMATRIZX][TAMMATRIZY], struct TRaiz raiz) {

	// Pido memoria para los títulos
	for (size_t i = 0; i < 10; i++)
		matriz[i][0] = malloc (sizeof(char) * 5);

	// Pido memoria para los datos
	for (size_t i = 0; i < TAMMATRIZX; i++)
		for (size_t j = 0; j < TAMMATRIZY; j++)
			matriz[i][j] = malloc (sizeof(char) * 30);

	// titulos
	strcpy (matriz[0][0], "k");
	strcpy (matriz[1][0], "aK");
	strcpy (matriz[2][0], "bK");
	strcpy (matriz[3][0], "f(aK)");
	strcpy (matriz[4][0], "f(bK)");
	strcpy (matriz[5][0], "rK+1");
	strcpy (matriz[6][0], "Delta rK+1");
	strcpy (matriz[7][0], "Delta r/r");
	strcpy (matriz[8][0], "Lambda");
	strcpy (matriz[9][0], "Rho");

	if (raiz.k < TAMMATRIZY - 1)
		cargarMatrizRedondeadaCompleta (matriz, raiz);
	else
		cargarMatrizRedondeadaIncompleta (matriz, raiz);

}

// Busca el largo del elemento mas largo de cada columna
void calcularAnchoColumnas (int anchos[TAMMATRIZX], char * matriz[TAMMATRIZX][TAMMATRIZY], int k) {

	for (int i = 0; i < TAMMATRIZX; i++)
		anchos[i] = 0;

	for (int i = 0; i < TAMMATRIZX; i++) {
		for (int j = 0; j < TAMMATRIZY && j <= k; j++) {
			if (strlen (matriz[i][j]) > anchos[i])
				anchos[i] = strlen (matriz[i][j]);
		}
	}

}

// Imprime línea entre filas
void imprimirLineaSeparadora (int anchos[TAMMATRIZX]) {

	int i = 0;
	int anchoTotal = 0;

	for (int j = 0; j < TAMMATRIZX; j++)
		anchoTotal = anchoTotal + anchos[j];

	anchoTotal = anchoTotal + TAMMATRIZX * 3; // Tamaño separador

	printf ("\n");

	while (i <= anchoTotal) {
		printf ("-");

		i++;
	}

	printf ("\n");

}

// Imprime espacios y un separador
void imprimirSeparador (int anchoElemento, int anchoColumna) {

	int aux = anchoElemento;

	while (aux < anchoColumna) {
		printf (" ");

		aux++;
	}

	printf (" | ");

}

// Imprime matriz de strings agregando separadores
void imprimirMatriz (char * matriz[TAMMATRIZX][TAMMATRIZY], int k) {

	int anchos[TAMMATRIZX];

	calcularAnchoColumnas (anchos, matriz, k);

	for (int j = 0; j < TAMMATRIZY && j <= k; j++) {
		for (int i = 0; i < TAMMATRIZX; i++) {
			printf ("%s", matriz[i][j]);

			if (i != TAMMATRIZX - 1)
				imprimirSeparador (strlen (matriz[i][j]), anchos[i]);
		}

		if (j != TAMMATRIZY - 1 && j <= k - 1)
			imprimirLineaSeparadora (anchos);
	}

	printf("\n");

}

// Calcula datos y manda a imprimir
void imprimirRaicesMetodoArranque (TListaSimple raices) {

	int aux = L_Mover_Cte (& raices, L_Primero);

	while (aux == TRUE) {
		char * matrizRedondeada[TAMMATRIZX][TAMMATRIZY];

		struct TRaiz raiz;
		L_Elem_Cte (raices, & raiz);

		cargarMatrizRedondeada (matrizRedondeada, raiz);

		imprimirMatriz (matrizRedondeada, raiz.k);

		//liberarMemoriaMatriz (matrizRedondeada);

		aux = L_Mover_Cte (& raices, L_Siguiente);
	}

}

void buscarPuntosDeEquilibrio (struct TVectorDatos datos, char opcion) {

	TListaSimple raices;

	switch (opcion) {
		case 1:
			datos.masaParticula = 0;

			buscarTodasRaices (& raices, datos, RegulaFalsi);
			filtrarRaices (& raices, opcion);
			imprimirRaicesMetodoArranque (raices);
			limpiarRaices (& raices);

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
