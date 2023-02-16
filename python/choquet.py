import random
import numpy as np    
#este codigo se obtuvo a partir de ver como se calcula la integral en la pagina
#http://www.isc.senshu-u.ac.jp/~thc0456/Efuzzyweb/fm01.html
def subconjuntos(N, inicio, sub):
    yield (sub) 
    for i in range(inicio, N + 1):
        for s in subconjuntos(N, i + 1, sub + [i]):
            yield s

n = 3
criterios = np.random.dirichlet(np.ones(n),size=1).tolist()[0]
print(f"criterios: {criterios}")
print(f"suma: {sum(criterios)}")
criterios = list(enumerate(criterios, 1))

def generar_clave(conjunto):
    clave = "("
    for i in range(len(conjunto)):
        if i == len(conjunto) - 1:
            clave += f'x{conjunto[i]})'
        else:
            clave += f'x{conjunto[i]},'
    return clave

subs = {}
for sub in subconjuntos(n, 1, []):
    if sub == []: continue
    if len(sub) == 1:
        subs[generar_clave(sub)] = criterios[sub[0] - 1][1]
    else:
        #[1, 2, 3]
        total = 0
        for x in sub:
            total += criterios[x - 1][1]
        medida = random.uniform(total, 1)
        subs[generar_clave(sub)] = medida
print(subs)
criterios.sort(reverse=True, key=lambda x: x[1])
resultado = 0
conjunto = []
for i in range(n):
    diff = 0
    if i == n - 1:
       resultado += criterios[i][1]
    else:
        diff = criterios[i][1] - criterios[i + 1][1]
        conjunto.append(criterios[i][0])
        conjunto.sort()
        medida = subs[generar_clave(conjunto)]
        resultado += medida * diff
print(f"integral de choquet: {resultado}")
