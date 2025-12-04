#include "estructuras.c"

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
