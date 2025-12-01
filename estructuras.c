#ifndef estructuras_H
#define estructuras_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_Personas_por_territorio 30 //Máximo de personas por territorio
#define MAX_Territorios 20 //Matriz de 20x20, territorios 
//En total serán 500 personas totales repartidas en los 20 territorios
#define MAX_cepas 50 //Número máximo de cepas
#define MAX_semillas 10 //Número de semillas iniciales

typedef enum{
    SANO, // 0: Es susceptible a infectarse, sano por el momento
    INFECTADO, // 1: Si está infectado
    RECUPERADO, // 2: Si ya está recuperado
    FALLECIDO // 3: Si ya está fallecido :(. Por el momento si está en ese estado no se eliminará de la BD
}estado;

const char *nombres_estados[] = {
    "Sano",
    "Infectado",
    "Recuperado",
    "Fallecido"
};

typedef struct persona{
    int id;
    char nombre[50];
    int territorio_id; //Para tener referencia inicial a la hora de realizar los algoritmos
    int grado_inicial;
    double riesgo_inicial; //Lo puse como double porque al ser tantas personas osea muchos datos, nos evitamos de inconsistencias en el cálculo de probabilidades, aumenta un poco el espacio de memoria en MB pero no es mucho.
    //gradp inicial: Número promedio de contactos de una persona
    //riesgo inicial: vulnerabilidad de la personas. Vulnerabilidad individual
    estado estado; // 0 si es no infectado, 1 si si lo es
    int tiempo_contagio; //Dias de contagiado. Tipo de datos especifico para el subproblema 1, ordenamiento de tiempo_infeccion ASC
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

void inicializar_sistema(sistema *sistema){
    sistema->numterritorios = 0;
    sistema->numcepas = 0;
    sistema->numsemillas = 0;

    //Inicialización grafo de territorios
    for (int i = 0; i < MAX_Territorios; i++)
    {
        for (int j = 0; j < MAX_Territorios; j++)
        {
            sistema->grafoterritorios[i][j] = 0;
        }
    }
    //Inicializar cada territorio
    for (int t = 0; t < MAX_Territorios; t++)
    {   
        sistema->territorios[t].territorio_id = t;
        strcpy(sistema->territorios[t].nombre, ""); //Asignamos los strings de nombre a vacíos
        sistema->territorios[t].M = 0; //Número de personas en el territorio
        //Inicializacipon del grafo de personas
        for (int p1 = 0; p1 < MAX_Personas_por_territorio; p1++)
        {
            for (int p2 = 0; p2 < MAX_Personas_por_territorio; p2++)
            {
                sistema->territorios[t].grafopersonas[p1][p2] = 0;
            }
        }
    }
    
    
}

// Solo muestra las conexiones
void imprimir_matrices(sistema *sistema){
    
    printf("\nMatriz de territorios 20x20\n");
    
    for (int i = 0; i < MAX_Territorios; i++)
    {
        for (int j = 0; j < MAX_Territorios; j++)
        {
            printf("%2.f ",sistema->grafoterritorios[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    //Por ahora limite la impresión a solo 5 territorios
    for (int t = 0; t < 5; t++){
        printf("Grafo de Personas por territorio (ID territorio): [%d]\n",t);   
        for (int p1 = 0; p1 < MAX_Personas_por_territorio; p1++)
        {
            for (int p2 = 0; p2 < MAX_Personas_por_territorio; p2++)
            {
                printf("%2.f ",sistema->territorios[t].grafopersonas[p1][p2]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

void mostrar_persona(sistema *sistema){
    if (sistema->numterritorios > MAX_Territorios)
    {
        printf("Territorio no existente");
        return;
    }
    
    for (int i = 0; i < sistema->numterritorios; i++)
    {
        printf("Territorio: %-15s [ID: %d] [Personas: %d]\n",sistema->territorios[i].nombre, sistema->territorios[i].territorio_id, sistema->territorios[i].M);

        if (sistema->territorios[i].M == 0)
        {
            printf("Sin personas registradas en el territorio\n\n");
        }else{
            for (int j = 0; j < sistema->territorios[i].M; j++)
            {
                printf("[ID: %d] %-20s[Grado: %2d] [Riesgo_init: %.3f] [Estado: %s]\n",sistema->territorios[i].personas[j].id, sistema->territorios[i].personas[j].nombre, sistema->territorios[i].personas[j].grado_inicial, sistema->territorios[i].personas[j].riesgo_inicial, nombres_estados[sistema->territorios[i].personas[j].estado]);
            }
            printf("\n");
        }
        
    }
    printf("\n");
}


void addpersonas(sistema *sistema, int territorio_id, int id, const char *nombre, int grado_inicial, double riesgo_inicial){
    //Validamos si existe el territorio
    if (territorio_id >= sistema->numterritorios || territorio_id < 0)
    {
        printf("Territorio no existente");
        return;
    }
    //Validación que no esté lleno el array basicamente, si se quieren insertar personas despues pues se sube la cantidad de personas estaticamente en el define
    //M es el numPersonas de ese territorio en especifico por eso le indicamos el indice del territorio al que pertenece por el territorio_id
    if (sistema->territorios[territorio_id].M >= MAX_Personas_por_territorio)
    {
        printf("Error. No puede insertar más personas. Capacidad máxima alcanzada (25 max)");
        return;
    }
    int indice = sistema->territorios[territorio_id].M; //Guardamos el indice de la persona a ingresar en el territorio que se especifico
    sistema->territorios[territorio_id].personas[indice].id = id;
    strcpy(sistema->territorios[territorio_id].personas[indice].nombre, nombre);
    sistema->territorios[territorio_id].personas[indice].grado_inicial = grado_inicial;
    sistema->territorios[territorio_id].personas[indice].riesgo_inicial = riesgo_inicial;
    sistema->territorios[territorio_id].personas[indice].estado = SANO;

    sistema->territorios[territorio_id].M++; //Se incrementa M osea numPersona por cada inserción que se haga en el indice del territorio específico, esto evitando que se acumulen todos las personas en un mismo contador.
}

void mostrar_territorios(sistema *sistema){
    //Validamos si hay territorios existentes
    if (sistema->numterritorios == 0)
    {
        printf("No hay territorios a imprimir");
        return;
    }

    printf("----------------Territorios agregados-----------\n");
    
    if (sistema->numterritorios == 0)
    {
        printf("No hay territorios agregados");
        return;
    }
    printf("--  #  Nombre Territorio     ID      M  \n");
    for (int i = 0; i < sistema->numterritorios; i++)
    {
        printf("-- [%2d] %-16s (ID: %2d, Personas: %2d)\n",i,sistema->territorios[i].nombre,sistema->territorios[i].territorio_id ,sistema->territorios[i].M);
    }
    printf("------------------------------------------------\n");
}

int addterritorio(sistema *sistema, const char *nombre){
    if (sistema->numterritorios >= MAX_Territorios)
    {
        printf("No se pueden agregar más territorios al máximo permitido MAX: %d\n", MAX_Territorios);
        return -1;
    }

    if (nombre == NULL || strlen(nombre) == 0)
    {
        printf("El nombre del territorio no puede estar vacío");
        return -1; //Retornarmos -1 en todos los casos para evitar poner 0 y pensar que se añadió algún dato no requerido
    }
    
    if (strlen(nombre) >= 15)
    {
        printf("Nombre muy largo para este campo (Máximo de 14 caracteres)");
        return -1;
    }
    

    int indice = sistema->numterritorios; //Hacemos que nuestro indice del array sea lo mismo que el valor de num de territorios

    sistema->territorios[indice].territorio_id = indice; //Asignamos el valor del indice actual de nuestro array al indice que tuvimos al declarar la variable indice
    strcpy(sistema->territorios[indice].nombre, nombre); //Copiamos el nombre que le pasariamos por parametro desde el main y se lo asignamos al nombre de la posicion osea del indice actual
    sistema->territorios[indice].M = 0; //Se empieza sin personas desde un inicio, estó se irá actualizando

    sistema->numterritorios++; //Esto es lo que hará que se actualice el indice y no se nos sobreescriba en la misma posición una y otra vez

    return indice; //Retornamos el índice actual
    
}

#endif