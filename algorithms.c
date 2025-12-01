#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void merge(persona *p, int left, int center, int right);
void merge_sort(persona *p, int left, int right);
int ordenamiento_por_riesgo_desc(sistema *s);
int mergesort_ordenar_personas(territorio *t);

int main(void)
{
    if(ordenamiento_por_riesgo_desc(s)) {
        printf("Se ordenó con éxito los territorios.\n");
    } else {
        printf("Hubo problema el ordenamiento por merge sort.\n");
    }

    return 0;
}

int ordenamiento_por_riesgo_desc(sistema *s)
{
    if(!s) {
        return -1;
    }

    for(int i = 0; i < s->numterritorios; i++) {
        if(mergesort_ordenar_personas(s->territorios[i]) == -1) {
            return -1;
        }
    }

    return 1;
}

int mergesort_ordenar_personas(territorio *t)
{
    if(!t) {
        return -1;
    }

    persona *temp = (persona *)malloc(t->M * sizeof(persona));

    memcpy(temp, t->personas, t->M * sizeof(persona));

    merge_sort(temp, 0, t->M - 1);

    return 1;
}

/* Riesgo DESC */
void merge(persona *p, int left, int center, int right)
{
    int n = right - left + 1;
    persona *temp = (persona *)malloc(n * sizeof(persona));
    
    int i = left, j = center + 1, x = 0;
    
    // mezclar
    while(i <= center && j <= right) {
        if(p[i].riesgo_inicial < p[j].riesgo_inicial) {
            temp[x++] = p[i++];
        } else {
            temp[x++] = p[j++];
        }     
    }

    // vaciar
    while(i <= center) {
        temp[x++] = p[i++];
    }
        
    while(j <= right) {
        temp[x++] = p[j++];
    }
               
    for(x = 0, i = left; i <= right; i++, x++) {
        p[i] = temp[x]; 
    }            
}

void merge_sort(persona *p, int left, int right)
{
    if(left >= right) {
        return;
    }

    int center = (left + right) / 2;

    merge_sort(p, left, center);
    merge_sort(p, center + 1, right);
    merge(p, left, center, right);
}