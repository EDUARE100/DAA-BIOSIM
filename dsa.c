#include "dsa.h"
#include "biosimlib.h"
#include <glib.h>
#include <string.h>
// https://docs.gtk.org/glib/index.html
// https://developer.ibm.com/tutorials/l-glib/
/*
    Correr el archivo en cmd:
        > con un archivo main momentáneo
        > gcc -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -pthread dsa.c biosimlib.c main.c -o dsa -lglib-2.0
*/

/*
    > Crear cola se crea de la siguiente manera:
        - GQueue *cola_territorios = g_queue_new();
    > Sacar el tamaño de la cola de la sig. manera:
        - int cola_territorios_length = g_queue_get_length(cola_territorios);
*/

/*
    insertar_cola_territorios(GQueue *cola, struct Territorio *territorio): int.
        - Descripción: Inserta territorio en la cola de territorios, devuelve -1 si no se logró insertar, 1 si se insertó con éxito.
        - Elementos:
            > *cola: Cola de territorios, se recibe por parámetro.
            > *territorio: struct Territorio, se recibe por parámetro.
        - Nota:
            > Se utiliza la librería glib y la cola que se encuentra en esta.
            > Se inserta el territorio al final de la cola (tail).
*/
int insertar_cola_territorios(GQueue *cola, struct Territorio *territorio)
{
    if(!territorio) {
        return -1;
    }

    g_queue_push_tail(cola, territorio);

    return 1;
}

/*
    Func crear_territorio: Crea los territorios que se insertarán en la cola doble.
*/
struct Territorio *crear_territorio(int id, char *nombre, int m, struct GrafoPersonas *grafo)
{
    if(!nombre || !grafo) {
        return NULL;
    }

    struct Territorio *new_territorio = g_malloc(sizeof(struct Territorio));
    if(!new_territorio) {
        return NULL;
    }

    /* Llenado de datos */
    new_territorio->territorio_id = id;
    strcpy(new_territorio->nombre, nombre);
    new_territorio->M = m;
    new_territorio->grafo_personas = grafo;

    return new_territorio;
}