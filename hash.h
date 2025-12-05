#ifndef HASH_H
#define HASH_H

#include "estructuras.h"
#include "hash.h"
#include <stdlib.h>
#include <stdio.h>

#define TAMANO_HASH 1009 // Número primo para reducir colisiones

// Nodo genérico para la tabla hash (almacena el puntero al dato real)
typedef struct NodoHash {
    int id;             // La clave (Key)
    void *dato;         // Puntero genérico (a Persona*, Cepa*, etc.)
    char tipo;          // 'P' = Persona, 'C' = Cepa
    struct NodoHash *siguiente; 
} NodoHash;

// La Tabla Hash (Array de punteros a Nodos)
typedef struct {
    NodoHash *buckets[TAMANO_HASH];
} TablaHash;

void inicializar_hash(TablaHash *t); 
int funcion_hash(int id);
void insertar_hash(TablaHash *t, int id, void *dato, char tipo);
void* buscar_hash(TablaHash *t, int id, char tipo);
void consultar_hash(TablaHash *tabla);


#endif