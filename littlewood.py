import numpy
import pygame as py
import sys
from pygame.locals import *

def littlewood(m, n, pl):
    if len(pl) == m + 1:
        yield pl[::-1]
    if n < 0:
        return
    pl.append(1)
    for p in littlewood(m, n - 1, pl):
        yield p
    pl.pop()
    pl.append(-1)
    for p in littlewood(m, n - 1, pl):
        yield p
    pl.pop()

n = 12

WIDTH, HEIGHT = 600, 600
py.init()

py.display.set_caption("Fractal")
window = py.display.set_mode([WIDTH, HEIGHT])
canvas = py.Surface([WIDTH, HEIGHT])
canvas.fill((0, 0, 0))

xsup =  2
xinf = -2
ysup =  2
yinf = -2


for i in range(1, n + 1):
    for p in littlewood(i, i, []):
        pp = numpy.polynomial.Polynomial(p)
        roots = pp.roots()

        for r in roots:
            #primero, no podemos graficar directamente numeros complejos
            #ni siquiera cualquier pareja (x,y). x debe estar en [0, WIDTH] y 
            #y en [0, HEIGHT], segundo sabemos que este fractal esta en el intervalo
            #[-2, 2] en x (la parte real) y en y (la imaginaria) igual. tercero
            # si tenemos el intervalo [0, n] para x y para y [0, m], podemos decir que cada
            # pixel (i, j) tiene coordenadas, dentro de estos intervalos, (i * n / WIDTH, j * m / HEIGHT)
            # por la que para convertir coordenadas (x,y) a coordenadass de pixel simplemente hay que despejar i y j
            #cuarto, pero el fractal esta en [-2, 2] y [-2, 2], entonces estos intervalos hay que convertirlos
            #a intervalos de la forma [0, k] para poder hacer la conversion directa a pixeles,
            #y para eso simplemente sumamos 2 a cada (x, y) con lo que estaran en el intervalo [0, 4], lo cual
            #ya sabemos como graficar
            x = (r.real + xsup) * WIDTH / (xsup - xinf) 
            y = HEIGHT - (r.imag + ysup) * HEIGHT / (ysup - yinf) #-2 se va a mapear 0, por lo hay que restarle HEIGHT para quede abajo y no arriba
            canvas.set_at((int(x), int(y)), (255, 255, 255))
clock = py.time.Clock()
while True:
    for e in py.event.get():
        if e.type == QUIT:
            py.quit()
            sys.exit()
    window.fill((0, 0, 0))
    window.blit(canvas, (0, 0))
    py.display.flip()
    clock.tick(60)
   
