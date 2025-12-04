/*
Pequeña documentación acerca de este problema 

Objetivo: Simular el contagio dia a dia, aplicando probabilidades de infección, recuperación y muerte. Almacenar datos por dia y usar algoritmo de complejidad O(nlogn)

Pasos:

1.- Iteraremos por cada dia que el usuario quiera avanzar (paramtro)
2.- Copia de seguridad (array): Para que los cambios sean simultaneos no se puede cambiar el estado de una persona de inmediato, esa misma persona podria contagiar a alguien mas el mismo dia. Se calcula primero todo en un array temporal y al final del dia se actualiza los estados
3.- Fase de contagio: Recorremos los infectados, Buscamos por los vecinos SANOS y aplicamos la formula de propagación

p = Beta(cepa) * Peso de conexión de personas * riesgo_inicial del vecino individual

4.- Fase de evolución (Recuperación/Deceso):
Cada infectado tiene una probabilidad de morir(letalidad) o recuperarse(gamma)

*/