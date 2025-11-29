#include <stdio.h>
#include <stdlib.h>
#include "estructuras.h"
#include "load_data.h"


int main(int argc, char const *argv[])
{
    sistema sistema;

    inicializar_sistema(&sistema);

    //Nota, imprimir matrices se limito a 5 a propósito pero nada más se cambia en estructuras.c y ya
    //imprimir_matrices(sistema);

    leer_territorios_csv(&sistema,"territorios.csv");

    printf("DEBUG: Despues de leer territorios, hay %d cargados.\n", sistema.numterritorios);

    mostrar_territorios(sistema);
    leer_personas_csv(&sistema,"personas.csv");
    mostrar_persona(sistema);
    return 0;
}