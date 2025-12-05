#ifndef estructuras_H
#define estructuras_H

/* >> Constantes << */
#define MAX_Personas_por_territorio 30 //Máximo de personas por territorio
#define MAX_Territorios 20 //Matriz de 20x20, territorios 
//En total serán 500 personas totales repartidas en los 20 territorios
#define MAX_cepas 50 //Número máximo de cepas
#define MAX_semillas 10 //Número de semillas iniciales
extern const char *nombres_estados[];

typedef enum{
    SANO, // 0: Es susceptible a infectarse, sano por el momento
    INFECTADO, // 1: Si está infectado
    RECUPERADO, // 2: Si ya está recuperado
    FALLECIDO // 3: Si ya está fallecido :(. Por el momento si está en ese estado no se eliminará de la BD
}estado;

typedef struct persona{
    int id;
    char nombre[50];
    int territorio_id; //Para tener referencia inicial a la hora de realizar los algoritmos
    int grado_inicial;
    double riesgo_inicial; //Lo puse como double porque al ser tantas personas osea muchos datos, nos evitamos de inconsistencias en el cálculo de probabilidades, aumenta un poco el espacio de memoria en MB pero no es mucho.
    //grado inicial: Número promedio de contactos de una persona
    //riesgo inicial: vulnerabilidad de la personas. Vulnerabilidad individual
    estado estado; // 0 si es no infectado, 1 si si lo es
    int tiempo_contagio; //Dias de contagiado. Tipo de datos especifico para el subproblema 1, ordenamiento de tiempo_infeccion ASC
    int cepa_id;

    float x,y; //Campos para la animacion
}persona;

typedef struct territorio{
    int territorio_id;
    char nombre[15];
    int M; //Número de personas

    persona personas[MAX_Personas_por_territorio]; //Creamos un array interno en el struct de territorio de tipo Persona que tendrá de máximo de personas permidas por territorio 25.
    double grafopersonas[MAX_Personas_por_territorio][MAX_Personas_por_territorio]; //Este es nuestro grafo interno que será la conexión (aristas) de las personas dentro del array de territorios, hacemos de 25x25 la red para las conexiones.
}territorio;

typedef struct cepa{
    int cepa_id;
    char nombre[50];
    double beta; //Tasa de propagación
    double letalidad;
    double gamma_recuperacion; //tasa de recuperación
}cepa;


typedef struct semilla{
    int persona_id; //Pacientes 0
    int t0; //Tiempo iniclal de contagio
    int cepa_id; //Tipo de cepa que tiene
}semilla;

//Esta estructura engloba todas las estructuras, haciendolo de un array de arrays con una matriz(grafo) interno de tipo persona y otra matriz externa(grafo) de tipo territorio para tener las conexiones presentes, el peso entre conexiones será la probabilidad de contagio inicial que se tiene y este tendrá que ir cambiando conforme a los dias en la simulación.
typedef struct sistema{
    territorio territorios[MAX_Territorios]; //Array de territorios
    int numterritorios;
    double grafoterritorios[MAX_Territorios][MAX_Territorios]; //Matriz/grafo externo, me refiero externo porque no está dentro del array como tal

    cepa cepas[MAX_cepas];
    int numcepas; //Numero de cepas. Necesitamos saberlas por si se introducen más cepas posteriormente o para saber cuantas cepas fueron cargadas.

    semilla semillas[MAX_semillas];
    int numsemillas; //Numero de semillas. Necesitamos saberlas por si se introducen más semillas posteriormente o para saber cuantas semillas fueron cargadas.

}sistema;

void inicializar_sistema(sistema *sistema);
void matriz_conexiones_territorios(sistema *sistema);
void matriz_conexiones_personas(sistema *sistema);
void mostrar_persona(sistema *sistema);
void mostrar_territorios(sistema *sistema);
void mostrar_cepas(sistema *sistema);
void mostrar_semillas(sistema *sistema);

void addpersonas(sistema *sistema, int territorio_id, int id, const char *nombre, int grado_inicial, double riesgo_inicial);
int addterritorio(sistema *sistema, const char *nombre);
void conexiones_territorios(sistema *sistema, int u_territorio, int v_territorio, float distancia_peso);
void generar_red_contactos(sistema *sistema);

#endif