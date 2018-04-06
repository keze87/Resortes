#define  TRUE 0
#define  FALSE 1
#include "tests.h"

int proceso () {

	return TRUE;

}

int main () {

	int aux = correrTests (); // Pruebas Unitarias

	if (aux)
		aux = proceso ();

	return aux;

}
