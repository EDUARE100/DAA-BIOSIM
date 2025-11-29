#include "estructuras.c"

void inicializar_sistema(sistema *sistema);
void imprimir_matrices(sistema sistema);
void mostrar_persona(sistema sistema);
void addpersonas(sistema *sistema, int territorio_id, int id, const char *nombre, double grado_inicial, double riesgo_inicial);
void mostrar_territorios(sistema sistema);
int addterritorio(sistema *sistema, const char *nombre);