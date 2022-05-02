import math, numpy as np
import matplotlib.pyplot as plt
#basicamente quiero graficar la grafica que muestra los valores de pi cuando la distancia se generaliza
#a lp = (|x1-x2|^p + |y1-y2|^p) ^ (1 / p), donde si hacemos p = 2, entonces estamos en la distancia euclidea de siempre.
#para calcular pi, consideramos pi = longitud_de_circunferencia / diametro, es decir, si p = 2, se consideraria
#como circunferencia aquellos puntos que satisfagan x^2 + y ^2 = 1, o si p = 4, entonces x^4 + y^4 = 1 y asi
#para lo cual podemos pensar en la simple parametrizacion de la curva
#x = t
#y = (1-t^p) ^ (1/p)
#si hacemos 0 <= t <= 1, entonces la curva se encuentra unicamente en el primer cuadrante, se puede ver facilmente
#que este segmento es simetrico a los restantes, es decir, podemos construir la curva total con este segmento
#por lo cual la longitud de la curva seria 4 veces la longitud del segmento
#para encontrar esa longitud hacemos una integral
#la integral simplemente divide la curva en muchos puntos, y se van uniendo estos uno a uno formando segmentos de longitud dada
#por la distancia lp, estas longitudes se van sumando y listo
#se considera que la curva tiene radio 1, por la tanto su diametro es 2
def get_y(t, p):
    return math.pow(1 - t ** p, 1 / p)

def get_dist(p, x1, y1, x2, y2):
    d = math.fabs(x1 - x2)**p + math.fabs(y1 - y2)**p
    return math.pow(d, 1 / p)

def get_pi(p):
    n = 1000
    s = 0
    last_point = (0, get_y(0, p))
    dt = 1 / n
    for i in range(n + 1):
        xnew = i * dt
        ynew = get_y(xnew, p)
        x2, y2 = last_point
        s += get_dist(p, xnew, ynew, x2, y2)
        last_point = (xnew, ynew)
    return 2 * s

p = np.arange(1, 16, 0.1)
pip = [get_pi(dp) for dp in p]

plt.plot(p, pip)
plt.ylabel('lp distances')
plt.show()
