#ifndef biosimlib_H

#define biosimlib_H

/*
    enum estado: Enum para indicar en qué etapa de infección se encuentra el paciente x.
        - Esquema:
            SUSCEPTIBLE -> INFECTADO -> (DOS OPCIONES) -> RECUPERADO
                                                       -> EXTINTO
        > SUSCEPTIBLE: 0.
            - Descripción: Todos somos susceptibles a contagiarnos de algo, es lo más cercano a paciente sano.
        > INFECTADO: 1.
            - Descripción: Paciente infectado de cepa x.
        > RECUPERADO: 2.
            - Descripción: Paciente que se encontraba infectado, pero sanó.
        > EXTINTO: 3.
            - Descripción: Paciente que se encontraba infectado, pero falleció.

*/
enum Estado
{
    SUSCEPTIBLE,
    INFECTADO,
    RECUPERADO,
    EXTINTO
};

/*
    struct NodeAdyacente:
        > prob_contagio: float.
            - Descripción: Depende de persona->estado. (Falta añadir cómo, idea: se multiplica el valor de estado por cierto valor fijo)
*/
typedef struct NodeAdyacente
{
    int Persona_id;
    float prob_contagio; /* Lo hago float porque es medida estadística, la "etiqueta" vendría a ser el peso
    esta será la medida que usaremos para la "propagación" del o los virus, ya una medida dispuesta que haga que por
    probabilidad cambie el campo infectado de 0 que inicia en caso de no ser la semilla a 1 que sería que esté infectado,
    true y false */
    struct NodeAdyacente *next;

} NodeAdyacente;

/*
    struct Persona: Estructura para indicar las personas que se encuentran en cierto territorio.
        > persona_id: int.
            - Descripción: Identificador de persona.
        > nombre[20]: char.
            - Descripción: Nombre de la persona.
        > Territorio_id: int.
            - Descripción: Identificador del territorio al que pertenece la persona.
        > grado_inicial: int.
            - Descripción: ¿?
        > riesgo_inicial: float.
            - Descripción: Valor de riesgo de infección.
        > estado: enum.
            - Descripción: Indica qué tipo de paciente es, es decir, en qué grado de infección se encuentra.
        > *cepa_actual: struct Cepa.
            - Descripción: Depende de estado.
                            - Si INFECTADO/EXTINTO: le corresponde una cepa.
                            - Si SUSCEPTIBLE/RECUPERADO: le corresponde NULL.
*/
typedef struct Persona
{
    int persona_id;
    char nombre[50];
    int Territorio_id;
    int grado_inicial;    /* Esto no se a que se refiere lol, suponga es la medida estádistica ya predispuesta
       a la hora de insertar los datos. ACTUALIZACION. Es la cantidad de enlaces o aristas cuando recien se inserta el nodo */
    float riesgo_inicial;
    enum Estado estado;
    struct Cepa *cepa_actual;

} Persona;

/*
    struct Cepa: Estructura para indicar las cepas que se encuentran del virus x.
        > cepa_id: int.
            - Descripción: Identificador de cepa.
        > nombre[20]: char.
            - Descripción: Nombre de la cepa.
        > beta_propagacion: float
            - Descripción: Tasa de propagación/infección.
        > mu_letalidad: float
            - Descripción: Tasa de defunción.
        > gamma_recuperacion: float
            - Descripción: Tasa de recuperación.
*/
typedef struct Cepa
{
    int cepa_id;
    char nombre[20];
    float beta_propagacion;
    float mu_letalidad;
    float gamma_recuperacion;

} Cepa;

/*
    struct GrafoPersonas:
        > num_Personas: int.
        > *personas: struct Persona.
        > **list_Adyacencia: struct NodeAdyacente.
*/
typedef struct GrafoPersonas
{
    int num_Personas; // Numero total de personas, n / size
    Persona *personas;
    NodeAdyacente **list_Adyacencia; // Será nuestro contenedor que nos conecté al struct de nodos
    
} GrafoPersonas;

/*
    struct Territorio: Estructura para indicar cada territorio de toda la población de x país.
        > territorio_id: int.
        > nombre[15]: char.
        > M: int.
            - Descripción: Indica la población total del territorio x.
        > *grafo_personas: struct GrafoPersonas.
*/
typedef struct Territorio
{
    int territorio_id;
    char nombre[15];
    int M;
    GrafoPersonas *grafo_personas;
    
} Territorio;


#endif