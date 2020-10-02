import numpy as np, math, pygame as py
from pygame.locals import *
py.init()
py.mixer.quit()
g = 9.8
l = 150
omega = 0
theta = math.radians(180)
py.display.set_caption("Simple pendulum simulation")
window = py.display.set_mode([800, 600])
clock = py.time.Clock()
dt = clock.tick(60) / 100
base_x = 400
base_y = 300
while True:
    for e in py.event.get():
        if e.type == QUIT:
            py.quit()
            break
    omega = omega - (g / l) * dt * math.sin(theta)
    theta = theta + dt * omega
    x = round(l * math.sin(theta)) 
    y = round(l * math.cos(theta))
    window.fill((255, 255, 255))
    py.draw.circle(window, (0, 0, 0), (400 + x, 300 + y), 15)
    py.draw.circle(window, (255, 0, 0), (base_x, base_y), 5)
    py.draw.line(window, (0, 0, 0), (base_x, base_y), (400 + x, 300 + y), 2)
    py.display.flip()
    dt = clock.tick(60) / 100
