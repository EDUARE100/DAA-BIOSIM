# Instrucciones BIOSIM

## Introducción: 
Diseñar un sistema que modele cómo se propaga una enfermedad (virus, bacteria, o mutación genética) a través de una red de individuos y territorios, y que permita predecir, optimizar y contener la propagación usando múltiples paradigmas algorítmicos, cada uno resolviendo un subproblema específico.

## Escenario Base
1. Población de N individuos conectados por una red de contactos
   - Cada territorio tiene un conjunto M de individuos, donde se relacionan por proximidad o contacto con cierta **PROBABILIDAD** de contagio
2. El virus tiene características de **MUTACIÓN**
   - Propagación
   - Letalidad
   - Tiempo de recuperación
3. El sistema deberá resolver 8 subproblemas, cada uno mapeado a un paradigma algoritmico distinto, todos parte del **MISMO MODELO** integral

## Mapa de Subproblemas y paradigmas
- [ ] Fase 1
      
| Fase | Subproblema a resolver | Complejidad de algoritmo involucrado|
| :---: | :--- | :--- |
| 1. Inicicialización y análisis de datos | Leer poblaciones por nombre. Para cada población, listar sus individuos ordenados por: <br> - Riesgo **DESC** <br> - Tiempo_de_infección **ASC** (los No infectados al final <br> - Nombre **ASC**| Cada conjunto de datos deberá ser ordenado usando: 4 algoritmos O(nlogn) o de menor complejidad diferentes |

- [ ] Fase 2
      
| Fase | Subproblema a resolver | Complejidad de algoritmo involucrado|
| :---: | :--- | :--- |
| 2. Detección de brotes |Determinar los primeros focos y zonas conectadas|Algoritmo menor a O(n^3)|

- [ ] Fase 3
      
| Fase | Subproblema a resolver | Complejidad de algoritmo involucrado|
| :---: | :--- | :--- |
| 3. Propagación temporal |Calcular la expansión paso a paso del contagio|Simular contagio acumulativo y recuperación (Almacenar estados por día), algoritmo menor o igual a O(nlogn)|

- [ ] Fase 4
      
| Fase | Subproblema a resolver | Complejidad de algoritmo involucrado|
| :---: | :--- | :--- |
| 4. Minimización del riesgo total |Seleccionar subconjunto mínimo de nodos a aislar (sumando los riesgos individuales de cada nodo sin considerar como se conectan entre si| Algoritmo de complejidad menor a O(n^2)|


- [ ] Fase 5
      
| Fase | Subproblema a resolver | Complejidad de algoritmo involucrado|
| :---: | :--- | :--- |
| 5. Identificación de rutas críticas de contagio |Encontrar el "camino de mayor probabilidad de infección" entre dos nodos (PUNTO A - PUNTO B)| Complejidad a alcanzar O((n+m)logn), donde n = numero de nodos y m = numero de aristas|

- [ ] Fase 6
      
| Fase | Subproblema a resolver | Complejidad de algoritmo involucrado|
| :---: | :--- | :--- |
| 6. Cálculo de rutas óptimas de contención|Hallar la red mínima de vacunación o aislamiento|Algoritmo de complejidad O((n+m)logn)|

- [ ] Fase 7
      
| Fase | Subproblema a resolver | Complejidad de algoritmo involucrado|
| :---: | :--- | :--- |
| 7. Clustering de cepas similares|Agrupar variantes según similitud genética o contagio|Construcción O(nL) Búsqueda o consulta por cepa: O(L), n = numero de variantes, L = longitud del nombre o cadena|

- [ ] Fase 8
      
| Fase | Subproblema a resolver | Complejidad de algoritmo involucrado|
| :---: | :--- | :--- |
| 8. Almacenamiento eficiente y consulta rápida|Acceso a historiales de contagios, información individual, cepas o variantes|Todos son con complejidad O(1). Se puede ocupar el mismo algoritmo aquí para todo|


> [!NOTE]
> # Sugerencias <br>
> **Individuos:** id_nombre, territorio_id, grado_inicial, riesgo_inicial <br>
> **Territorios:** territorio_id, nombre, M <br>
> **Conexiones_territorio:** u_territorio, v_territorio, peso_proximidad (>= 30 filas) <br>
> **Contactos:** cepa_id, nombre, beta, letalidad, gamm_recuperación <br>
> **Semillas:** individuo_id, t0, cepa_id <br>
> ## ENTREGABLES especificados en PDF <br>

> [!WARNING]
> # Datos precargados <br>
> 50 variantes/cepas <br>
> 20 territorios/poblaciones <br>
> Mayor o igual a 30 conexiones entre territorios (aristas inter-territoriales) y 10 pacientes contagiados (semillas iniciales de contagio) <br>

> [!CAUTION]
> # Prohibido <br>
> Usar algoritmos mayores a O(n^3) o factoriales en cualquier funcionalidad <br>

> [!IMPORTANT]
> # Escenarios a Mostrar <br>
> **1. Inicialización y análisis de datos:** lectura de poblaciones, ordenamiento de 
individuos por distintos criterios (riesgo, tiempo de infección, nombre) usando 
varios algoritmos 𝑂(𝑛log 𝑛). <br>
> **2. Detección de brotes:** identificación de los primeros focos y zonas 
conectadas. <br>
> **3. Propagación temporal:** simulación paso a paso del contagio y recuperación 
acumulada. <br>
> **4. Minimización del riesgo total:** selección del subconjunto mínimo de nodos a 
aislar. <br>
> **5. Identificación de rutas críticas:** cálculo del camino de mayor probabilidad de 
infección. <br>
> **6. Rutas óptimas de contención:** construcción de la red mínima de vacunación 
o aislamiento. <br>
> **7. Clustering de cepas similares:** agrupamiento de variantes según similitud 
genética o patrón de contagio. <br>
> **8. Almacenamiento y consulta eficiente:** acceso rápido a historiales, nodos 
clave y variantes mediante hashing o estructuras balanceadas. <br>
