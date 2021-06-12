import random, math
import time
def fitness(x):
    return -x**4 + 3*x**2 - 10*x - x**3

generation      = 0
SIZE_POPULATION = 1000

population = [random.uniform(500, 1000) for _ in range(SIZE_POPULATION)]


while True:
    bests = [(x, fitness(x)) for x in population]
    bests.sort(key = lambda g: g[1], reverse=True)
    print(bests[:2], end='')
    print(" generation {:0}".format(generation))

    next_gen = [bests[i][0] for i in range(10*SIZE_POPULATION//100)] #seleccionando el 10 % de los mejores individuos
    
    for _ in range(90 * SIZE_POPULATION // 100): #agregando 90 % hijos mas para generar la nueva poblacion
        p1 = population[random.randint(0, SIZE_POPULATION/2)] 
        p2 = population[random.randint(0, SIZE_POPULATION/2)]
        if p1 < p2:
            child = p1 
        else:
            child = p2 
        if random.uniform(0, 1) < 0.45:
            child += random.uniform(-1, 1)
        next_gen.append(child)
    generation += 1
    population = next_gen
