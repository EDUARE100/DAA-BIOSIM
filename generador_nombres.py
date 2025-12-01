import random

nombres = ["Juan", "Maria", "Pedro", "Ana", "Luis", "Carmen", "Jose", "Laura", "Carlos", "Sofia", "Jorge", "Isabel", "Miguel", "Fernanda", "Jesus", "Adriana", "Roberto", "Veronica", "Alejandro", "Daniela"]
apellidos = ["Garcia", "Hernandez", "Lopez", "Martinez", "Gonzalez", "Rodriguez", "Perez", "Sanchez", "Ramirez", "Flores", "Gomez", "Torres", "Diaz", "Vasquez", "Cruz", "Morales", "Ruiz", "Jimenez", "Reyes", "Castillo"]

num_territorios = 20
personas_por_territorio = 25


for territorio_id in range(num_territorios):
    for x in range(personas_por_territorio):
        # Generar nombre
        nom = random.choice(nombres)
        ape = random.choice(apellidos)
        nombre_completo = f"{nom} {ape}"
        
        id_persona = ((territorio_id) + 1) * 100 + x;
        
        # Generar datos numéricos
        # Grado: float entre 0.0 y 100.0 (simulando conectividad)
        grado = round(random.uniform(0, 25))
        
        # Riesgo: double entre 0.0 y 1.0 (probabilidad)
        riesgo = round(random.uniform(0, 1), 4)
        
        # Imprimir fila CSV
        print(f"{id_persona},{nombre_completo},{territorio_id},{grado},{riesgo}")