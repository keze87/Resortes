#define  TRUE 0
#define  FALSE 1

#include "tests.h"
#include <stdio.h>

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

struct vectorDatos {

	float longitudNatural;
	float masaParticula;
	int constElastica;
	int distEntreExtremosFijos;

};

struct vectorDatos cargarVectorDatos () {

	struct vectorDatos aux;

	aux.longitudNatural = 2.05;
	aux.masaParticula = 1.02;
	aux.constElastica = 10;
	aux.distEntreExtremosFijos = 1;

	return aux;

}

int proceso () {

	imprimirEnunciado (1);

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
