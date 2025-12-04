#ifndef Prop_H
#define Prop_H

#include "estructuras.h"


//Complejidad O(N) debido a que iteramos sobre numcepas aunque numcepas ya está limitado a 50 desde un principio podria ser O(1) ya que pues no se indica que se puedan agregar más o menos cepas al código
int indice_cepa(sistema *sistema, int cepa_id){
    for (int i = 0; i < sistema->numcepas; i++)
    {
        if(sistema->cepas[i].cepa_id == cepa_id) return 1;
    }
    return -1;
}

void simulacion(sistema *sistema, int dias){
    for (int i = 0; i < dias; i++)
    {
        int nuevos_infectados = 0;
        int nuevos_recuperados = 0;
        int nuevos_fallecidos = 0;

        //Iteramos por numero de territorios
        for (int j = 0; j < sistema->numterritorios; j++)
        {
            int M = sistema->territorios[j].M;
            if(M==0)continue;

            //Guardamos los cambios del dia sin afectar la iteración actual
            estado *estados_futuros = (estado*)malloc(M * sizeof(estado));
            int *cepas_futuras = (int*)malloc(M * sizeof(int));
            

            //Inicializamos futuro con presente. M está definido como 30 asi que es un O(1) este for
            for (int k = 0; k < M; k++)
            {
                estados_futuros[k] = sistema->territorios[j].personas[k].estado;
                cepas_futuras[k] = sistema->territorios[j].personas[k].cepa_id;
            }
            

        }
        
    }
    
}

#endif