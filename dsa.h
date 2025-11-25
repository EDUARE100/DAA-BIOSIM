#ifndef dsa_H

#define dsa_H

#include <glib.h>
#include "biosimlib.h"

/*
    Si tu archivo de código fuente se llama mi_proyecto.c, el comando de compilación se vería así:
    gcc mi_proyecto.c -o mi_proyecto $(pkg-config --cflags --libs glib-2.0)
    Donde:
    > $(pkg-config --cflags glib-2.0): Le dice al compilador dónde buscar los archivos de header (.h).
    > $(pkg-config --libs glib-2.0): Le dice al enlazador dónde buscar las librerías binarias (.so o .a) y qué librerías enlazar.
*/

/* Funciones */
// Territorios
struct Territorio *crear_territorio(int id, char *nombre, int m, struct GrafoPersonas *grafo);
int insertar_cola_territorios(GQueue *cola, struct Territorio *territorio);

#endif