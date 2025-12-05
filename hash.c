#include "hash.h"

// Inicialización de la Tabla
void inicializar_hash(TablaHash *t) {
    for(int i = 0; i < TAMANO_HASH; i++) {
        t->buckets[i] = NULL;
    }
}

// Función Hash O(1)
int funcion_hash(int id) {
    return abs(id) % TAMANO_HASH;
}

// 1. Insertar (Almacenamiento O(1))
void insertar_hash(TablaHash *t, int id, void *dato, char tipo) {
    int index = funcion_hash(id);
    
    NodoHash *nuevo = (NodoHash*)malloc(sizeof(NodoHash));
    nuevo->id = id;
    nuevo->dato = dato;
    nuevo->tipo = tipo;
    
    // Inserción al inicio del bucket (costo O(1))
    nuevo->siguiente = t->buckets[index];
    t->buckets[index] = nuevo;
}

// 2. Buscar (Consulta O(1) en promedio)
void* buscar_hash(TablaHash *t, int id, char tipo) {
    int index = funcion_hash(id);
    NodoHash *actual = t->buckets[index];

    // Recorre solo la lista de colisiones (que suele ser muy corta)
    while (actual != NULL) {
        if (actual->id == id && actual->tipo == tipo) {
            return actual->dato; // Retorna el puntero a la Persona/Cepa
        }
        actual = actual->siguiente;
    }
    return NULL; 
}

void consultar_hash(TablaHash *tabla) {
    int id_buscar;
    printf("\n--- CONSULTA RAPIDA O(1) ---\n");
    printf("Ingrese ID de Persona o Cepa: ");
    scanf(" %d", &id_buscar);

    // 1. Búsqueda de Persona (O(1))
    persona *p_encontrada = (persona*)buscar_hash(tabla, id_buscar, 'P');
    
    if (p_encontrada != NULL) {
        printf("\n[PERSONA ENCONTRADA - O(1) Acceso]\n");
        printf("ID: %d | Nombre: %s\n", p_encontrada->id, p_encontrada->nombre);
        printf("Estado: %s | Dias Enfermo: %d | Cepa ID: %d\n", 
               p_encontrada->estado == 1 ? "INFECTADO" : "SANO/REC", 
               p_encontrada->tiempo_contagio, p_encontrada->cepa_id);
    } 
    else {
        // 2. Búsqueda de Cepa (O(1))
        cepa *c_encontrada = (cepa*)buscar_hash(tabla, id_buscar, 'C');
        if (c_encontrada != NULL) {
            printf("\n[CEPA ENCONTRADA - O(1) Acceso]\n");
            printf("ID: %d | Nombre: %s\n", c_encontrada->cepa_id, c_encontrada->nombre);
            printf("Beta: %.2f | Letalidad: %.2f | Gamma: %.2f\n", 
                   c_encontrada->beta, c_encontrada->letalidad, c_encontrada->gamma_recuperacion);
        } else {
            printf("\nID no encontrado en la base de datos O(1).\n");
        }
    }
}