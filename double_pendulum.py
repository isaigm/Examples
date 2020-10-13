import sys
import os
import pygame as py
from math import *

from pygame.locals import *

os.environ['SDL_VIDEO_CENTERED'] = '1'
py.init()
py.mixer.quit()
l1 = 100
l2 = 100
m1 = 40
m2 = 40
o1 = 0
o2 = 0
t1 = radians(90)
t2 = radians(30)
g = 1
py.display.set_caption("Double pendulum simulation")
window = py.display.set_mode([800, 600])
clock = py.time.Clock()
dt = 1
canvas = py.Surface((800, 600))
canvas.fill((255, 255, 255))

base_x = 400
base_y = 300
while True:
    for e in py.event.get():
        if e.type == QUIT:
            py.quit()
            sys.exit()
    n1 = -g * (2*m1 + m2) * sin(t1)
    n2 = -m2 * g * sin(t1 - 2*t2)
    n3 = -2*sin(t1-t2) * m2 * (o2**2 * l2 + o1**2 * l1 * cos(t1-t2))
    d = 2*m1 + m2 - m2 * cos(2*t1 - 2*t2)
    oo1 = o1
    o1 = o1 + dt*(n1 + n2 + n3) / (l1 * d)

    n1 = 2*sin(t1-t2)
    n2 = oo1**2 * l1 * (m1 + m2) + g*(m1 + m2) * cos(t1)
    n3 = o2**2 * l2 * m2 * cos(t1-t2)

    o2 = o2 + dt * n1 * (n2 + n3) / (l2 * d)
    t1 = t1 + dt * o1
    t2 = t2 + dt * o2
    x1 = round(l1 * sin(t1))
    y1 = round(l1 * cos(t1))
    x2 = x1 + round(l2 * sin(t2))
    y2 = y1 + round(l2 * cos(t2))
    final_coords_1 = (base_x + x1, base_y + y1)
    final_coords_2 = (base_x + x2, base_y + y2)
    window.fill((255, 255, 255))
    window.blit(canvas, (0, 0))
    py.draw.circle(window, (255, 0, 0), (base_x, base_y), 5)
    py.draw.circle(window, (0, 0, 0), final_coords_1, 10)
    py.draw.circle(window, (0, 0, 0), final_coords_2, 10)
    py.draw.line(window, (0, 0, 0), (base_x, base_y), final_coords_1)
    py.draw.line(window, (0, 0, 0), final_coords_1, final_coords_2)
    py.draw.circle(canvas, (0, 0, 0), final_coords_2, 2)
    py.display.flip()
    clock.tick(60)
