#define  TRUE 0
#define  FALSE 1

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

struct TVectorDatos {

	float longitudNatural;
	float masaParticula;
	int constElastica;
	int distEntreExtremosFijos;

};

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

int L_Vacia (TListaSimple Ls);
void L_Elem_Cte (TListaSimple Ls, void * pE);
int L_Mover_Cte (TListaSimple * pLs, TMovimiento_Ls M);
void L_Vaciar (TListaSimple * pLs);

int proceso ();
struct TVectorDatos cargarVectorDatos ();
TListaSimple buscarRaiz (struct TVectorDatos datos, struct TIntervalos intervalo,
						int (* metodo)(struct retornoMetodo *,double,double,struct TVectorDatos));
int regulaFalsi (struct retornoMetodo * retornoMetodo, double intervaloMin, double intervaloMax, struct TVectorDatos datos);
double funcion (struct TVectorDatos d, double y);
