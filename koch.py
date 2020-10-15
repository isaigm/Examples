import os
import sys
import pygame as py

from pygame.locals import *

angle = 27


def get_vector(p, q) -> py.math.Vector2:
    return p - q


def draw_fractal(surface: py.Surface, s: py.math.Vector2, en: py.math.Vector2, depth):
    if depth >= 1:
        v = get_vector(en, s).normalize() * (get_vector(en, s).length() / 3)
        p1 = s + v
        p2 = p1 + v
        r1 = v.rotate(60)
        py.draw.line(surface, (0, 0, 0), (s.x, 600 - s.y), (p1.x, 600 - p1.y))
        py.draw.line(surface, (255, 255, 255), (p1.x, 600 - p1.y), (p2.x, 600 - p2.y), 5)
        py.draw.line(surface, (0, 0, 0), (p2.x, 600 - p2.y), (en.x, 600 - en.y))
        py.draw.line(surface, (0, 0, 0), (round(p1.x), round(600-p1.y)), (round(p1.x+r1.x), round(600 - p1.y-r1.y)))
        py.draw.line(surface, (0, 0, 0), (round(p2.x), round(600 - p2.y)), (round(p1.x+r1.x), round(600 - p1.y-r1.y)))
        draw_fractal(surface, s, p1, depth - 1)
        draw_fractal(surface, p1, r1 + p1, depth - 1)
        draw_fractal(surface, r1 + p1, p2, depth - 1)
        draw_fractal(surface, p2, en, depth - 1)


os.environ['SDL_VIDEO_CENTERED'] = '1'
py.init()
py.mixer.quit()
depth = 5
window = py.display.set_mode([800, 600])
clock = py.time.Clock()
end = py.math.Vector2(780, 200)
start = py.math.Vector2(20, 200)
canvas = py.Surface((800, 600))
canvas.fill((255, 255, 255))
draw_fractal(canvas, start, end, depth)
while True:
    for e in py.event.get():
        if e.type == QUIT:
            py.quit()
            sys.exit()
    window.fill((255, 255, 255))
    window.blit(canvas, (0, 0))
    py.display.flip()
    clock.tick(60)
