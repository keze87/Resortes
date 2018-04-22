#define  TRUE 0
#define  FALSE 1

//typedef int make_iso_compilers_happy;

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
	float p;

};

struct TVectorDatos {

	float longitudNatural;
	float masaParticula;
	int constElastica;
	int distEntreExtremosFijos;

};

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

struct TIntervalos {

	double intervaloMin;
 	double intervaloMax;

};

typedef enum {

	L_Primero,
	L_Siguiente,
	L_Anterior

} TMovimiento_Ls;

typedef struct TNodoListaSimple {

	void* Elem;
	struct TNodoListaSimple * Siguiente, * Anterior;

} TNodoListaSimple;

typedef struct {

	TNodoListaSimple * Primero, * Corriente;
	int TamanioDato;

} TListaSimple;

struct TRaiz {

	TListaSimple iteraciones;
	double raiz;
	double errorAbs;
	int k;

};

int L_Vacia (TListaSimple Ls);
void L_Elem_Cte (TListaSimple Ls, void * pE);
int L_Mover_Cte (TListaSimple * pLs, TMovimiento_Ls M);
void L_Vaciar (TListaSimple * pLs);

int proceso ();
double funcion (struct TVectorDatos d, double y);
struct TVectorDatos cargarVectorDatos ();
struct TRaiz buscarRaizDentroDeIntervaloMetodoArranque (struct TVectorDatos datos, struct TIntervalos intervalo,
						int (* metodo)(double *,double *,double *,double *,struct TVectorDatos));
struct TRaiz buscarRaizDentroDeIntervaloMetodoDeConv (struct TVectorDatos datos, struct TIntervalos intervalo,
						int (* metodo)(double *,double *,struct TVectorDatos));
int regulaFalsi (double * raiz, double * errorAbs, double * intervaloMin, double * intervaloMax, struct TVectorDatos datos);
int newtonRaphson (double * semilla, double * errorAbs, struct TVectorDatos datos);
int puntoFijo (double * semilla, double * errorAbs, struct TVectorDatos datos);
void buscarIntervalosDeRaices (struct TVectorDatos datos, TListaSimple * intervalos);
void buscarTodasRaices (TListaSimple * raices, struct TVectorDatos datos, EMetodos metodo, TListaSimple intervalosDeRaices);
void limpiarRaices (TListaSimple * raices);
