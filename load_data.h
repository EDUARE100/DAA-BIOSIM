#ifndef CSV_LOAD
#define CSV_LOAD

#include "estructuras.h"
#include "load_data.c"

int leer_territorios_csv(sistema *sistema, const char *filename);
int leer_personas_csv(sistema *sistema, const char *filename);
int leer_conexiones_territorios(sistema *sistema, const char *filename);
int leer_cepas_csv(sistema *sistema, const char *filename);
int leer_semillas_csv(sistema *sistema, const char *filename);

#endif