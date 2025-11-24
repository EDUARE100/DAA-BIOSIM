#include "dsa.h"
#include "biosimlib.h"
#include <stdio.h>
#include <glib.h>
#include <string.h>
// https://docs.gtk.org/glib/index.html
// https://developer.ibm.com/tutorials/l-glib/

/*
    Nota: ignorar la declaración e inicializacion de lista_territorios ahí, nomás era para guiarme jeje :P
    (1) Se crea el puntero lista_territorios
    (2) Se crea un territorio: pondremos new_territorio
    (3) Se agregan los datos de este territorio
*/
GList *lista_territorios = NULL;

/*
    Func insertar_lista_territorios: Inserta territorios al final de la lista_territorios
*/
void insertar_lista_territorios(GList *lista_territorios, struct Territorio *territorio)
{
    if(!territorio) {
        return NULL;
    }

    lista_territorios = g_list_append(lista_territorios, territorio);

    return;
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