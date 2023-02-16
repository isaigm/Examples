import random, math
N = 50

class individual:
    def __init__(self):
        temp = [i for i in range(N)]
        random.shuffle(temp)
        self.queens = temp
    
def not_attacked(queens, q1, q2):
    flag = True
    flag = flag and (queens[q1] != queens[q2])
    flag = flag and (queens[q1] - q1) != (queens[q2] - q2)
    flag = flag and (queens[q1] + q1) != (queens[q2] + q2)
    return flag

def queens_not_attacked(queens):
    ans = 0
    for i in range(N):
        flag = False
        for j in range(N):
            if i != j:
                if not not_attacked(queens, i, j):
                    flag = True
                    break
        if not flag:
            ans += 1
    return ans

def print_board(queens):
    board = [['0' for i in range(N)] for _ in range(N)]
    print('-' * (N * 2 - 1))
    for i in range(N):
        board[queens[i]][i] = '*'
    for i in range(N):
        for j in range(N):
            print(board[i][j], end=' ')
        print()
    print('-' * (N * 2 - 1))


generation      = 0
SIZE_POPULATION = 200
population = [individual() for _ in range(SIZE_POPULATION)]


while True:
    population.sort(key = lambda g: queens_not_attacked(g.queens), reverse=True)

    if queens_not_attacked(population[0].queens) == N:
        print(generation)
        for i, r in enumerate(population[0].queens):
            print(i, r)
        break


    next_gen = [population[i] for i in range(10*SIZE_POPULATION//100)] #seleccionando el 10 % de los mejores individuos
    
    for _ in range(90 * SIZE_POPULATION // 100): #agregando 90 % hijos mas para generar la nueva poblacion
        p1 = population[random.randint(0, SIZE_POPULATION/2)] 
        p2 = population[random.randint(0, SIZE_POPULATION/2)]
      
        child = individual() #seleccion
        i1 = random.randint(1, N - 2) #dos indices aleatorios distintos
        i2 = random.randint(1, N - 2)
        while i1 == i2:
            i1 = random.randint(1, N - 2)
            i2 = random.randint(1, N - 2)
        if i1 > i2:
            i1, i2 = i2, i1
        visited = []
        missing = []
        for i in range(i1, i2 + 1): #p1[i1, i2] a child
            child.queens[i] = p1.queens[i]
            visited.append(p1.queens[i])
        for i in range(N):
            if p2.queens[i] not in visited:
                missing.append(p2.queens[i])
        k = 0
       
        for i in range(i1):
            child.queens[i] = missing[k]
            k += 1
        for i in range(i2 + 1, N):
            child.queens[i] = missing[k]
            k += 1
        if random.uniform(0, 1) > 0.20:
            q1 = random.randint(0, N - 1)
            q2 = random.randint(0, N - 1)
            child.queens[q1], child.queens[q2] = child.queens[q2], child.queens[q1]
        next_gen.append(child)
    
    generation += 1
    population = next_gen
