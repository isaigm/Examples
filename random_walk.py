import random
import pygame as py
import sys
from pygame.locals import *

WIDTH, HEIGHT = 800, 600
py.init()

py.display.set_caption("Random Walk")
window = py.display.set_mode([WIDTH, HEIGHT])
canvas = py.Surface([WIDTH, HEIGHT])
canvas.fill((255, 255, 255))
RECT_SIZE = 10
clock = py.time.Clock()
x, y = round(WIDTH / 2), round(HEIGHT / 2)
while True:
    for e in py.event.get():
        if e.type == QUIT:
            py.quit()
            sys.exit()
    window.fill((255, 255, 255))
    py.draw.rect(canvas, (0, 0, 0), (x, y, RECT_SIZE, RECT_SIZE), 2)
    window.blit(canvas, (0, 0))
    py.display.flip()
    n = random.randint(0, 4)
    if n == 0:
        x = (x - RECT_SIZE) % WIDTH
    elif n == 1:
        x = (x + RECT_SIZE) % WIDTH
    elif n == 2:
        y = (y - RECT_SIZE) % HEIGHT
    elif n == 3:
        y = (y + RECT_SIZE) % HEIGHT
    clock.tick(60)
