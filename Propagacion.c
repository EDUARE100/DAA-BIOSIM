#ifndef Prop_H
#define Prop_H

#include "estructuras.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


//Complejidad O(N) debido a que iteramos sobre numcepas aunque numcepas ya está limitado a 50 desde un principio podria ser O(1) ya que pues no se indica que se puedan agregar más o menos cepas al código

/* Por que es O(n) y no de complejidad mayor?, */

int indice_cepa(sistema *sistema, int cepa_id){
    for (int i = 0; i < sistema->numcepas; i++)
    {
        if(sistema->cepas[i].cepa_id == cepa_id) return 1;
    }
    return -1;
}

void simulacion(sistema *sistema, int dias){
    for (int i = 1; i <= dias; i++)
    {
        int nuevos_infectados = 0;
        int nuevos_recuperados = 0;
        int nuevos_fallecidos = 0;

        //Iteramos por numero de territorios
        for (int j = 0; j < sistema->numterritorios; j++)
        {
            int M = sistema->territorios[j].M;
            if(M==0)continue;

            //Guardamos en copias temporales los cambios del dia sin afectar la iteración actual
            estado *estados_futuros = (estado*)malloc(M * sizeof(estado));
            int *cepas_futuras = (int*)malloc(M * sizeof(int));
            

            //Inicializamos futuro con presente. M está definido como 30 asi que es un O(1) este for
            for (int k = 0; k < M; k++)
            {
                estados_futuros[k] = sistema->territorios[j].personas[k].estado;
                cepas_futuras[k] = sistema->territorios[j].personas[k].cepa_id;
            }
            //Calcular contagios y evolución
            for (int k = 0; k < M; k++)
            {
                persona *individuo = &sistema->territorios[j].personas[k];

                //Si solamente el individuo está infectado pueden contagiar o cambiar de estado
                if (individuo->estado == INFECTADO)
                {
                    int id_cepa = indice_cepa(sistema,individuo->cepa_id);
                    if(id_cepa == -1) continue; //Medida de seguridad para


                    double beta = sistema->cepas[id_cepa].beta; //Tasa de contagio
                    double letalidad = sistema->cepas[id_cepa].letalidad;
                    double gamma = sistema->cepas[id_cepa].gamma_recuperacion;

                    for (int vecino = 0; vecino < M; vecino++)
                    {
                        /*
                            Condiciones a contagiar 
                            1.- Que exista la conexión entre las personas osea que != -1
                            2.- Que el vecino esté SANO
                            3.- Que el vecino siga SANO en el futuro, para no contagiarlo 2 veces el mismo dia
                        
                        */

                        double prob_contagio = sistema->territorios[j].grafopersonas[k][vecino];

                        if (prob_contagio != -1.0 && sistema->territorios[j].personas[vecino].estado == SANO && estados_futuros[vecino] == SANO)
                        {
                            
                            double riesgo_vecino = sistema->territorios[j].personas[vecino].riesgo_inicial;
                            
                            //Formula de contagio
                            double prob_final = beta * prob_contagio * riesgo_vecino;
                            /* 
                                Se rige de la probabilidad final de contagio siendo el producto de la probabilidad beta de la cepa por la probabilidad de contagio que es el peso de la conexión entre cada persona del gráfico estático por el riesgo inicial 
                            */
                            //Asignamos el tope de contagio normalizado para que no rebase el 100%
                            if(prob_final > 1.0) prob_final = 1.0;

                            double prob_aleatoria_contagio = (double)rand() / RAND_MAX;
                            
                            //Si la prob_final que es la formula de contagio se dice que es mayor que la probabilidad aleatoria de contagio entonces se infecta el individuo
                            if (prob_aleatoria_contagio < prob_final)
                            {
                                estados_futuros[vecino] = INFECTADO;
                                cepas_futuras[vecino] = individuo->cepa_id; //Hereda la cepa
                                nuevos_infectados++;
                            }
                            
                            
                        }
                        
                    }
                    
                        individuo->tiempo_contagio++;

                        double prob_aleatoria_recuperacion = (double)rand() / RAND_MAX;

                        if (prob_aleatoria_recuperacion < gamma)
                        {
                            double prob_aleatoria_muerte = (double)rand() / RAND_MAX;

                            if (prob_aleatoria_muerte < letalidad)
                            {
                                estados_futuros[k] = FALLECIDO;
                                nuevos_fallecidos++;
                            }
                            else
                            {
                                estados_futuros[k] = RECUPERADO;
                                nuevos_recuperados++;
                            }
                    }
                }

            } //termino del bucle k
            // Actualización de los cambios al array del sistema real
            for (int q = 0; q < M; q++)
            {
                if (sistema->territorios[j].personas[q].estado != estados_futuros[q])
                {
                    sistema->territorios[j].personas[q].estado = estados_futuros[q];
                    sistema->territorios[j].personas[q].cepa_id = cepas_futuras[q];


                    if (estados_futuros[q] == INFECTADO)
                    {
                        sistema->territorios[j].personas[q].tiempo_contagio = 0;
                    }
                    

                }
                
            }

            free(estados_futuros);
            free(cepas_futuras);
        }
        
        if (nuevos_infectados > 0 ||nuevos_fallecidos > 0)
        {
            printf("Dia %d: +%d Infectados, +%d Recuperados, +%d Muertes\n", i, nuevos_infectados, nuevos_recuperados, nuevos_fallecidos);
        } 
    }
}

#endif