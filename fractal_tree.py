import os
import sys
import pygame as py

from pygame.locals import *
angle = 30


def get_vector(p, q) -> py.math.Vector2:

    return p - q


def draw_tree(surface: py.Surface, s: py.math.Vector2, en: py.math.Vector2, depth):
    if depth >= 1:
        py.draw.line(surface, (0, 0, 0), (s.x, 600 - s.y), (en.x, 600 - en.y))
        v = get_vector(en, s)
        left = v.rotate(angle).normalize() * (v.length() - v.length() * 0.34)
        right = v.rotate(-angle).normalize() * (v.length() - v.length() * 0.34)
        py.draw.line(surface, (0, 0, 0), (en.x, 600 - en.y), (en.x + left.x, 600 - (left.y + en.y)))
        py.draw.line(surface, (0, 0, 0), (en.x, 600 - en.y), (en.x + right.x, 600 - (right.y + en.y)))
        draw_tree(surface, en, py.Vector2(en.x + left.x, left.y + en.y), depth - 1)
        draw_tree(surface, en, py.Vector2(en.x + right.x, right.y + en.y), depth - 1)


os.environ['SDL_VIDEO_CENTERED'] = '1'
py.init()
py.mixer.quit()

window = py.display.set_mode([800, 600])
clock = py.time.Clock()
end = py.math.Vector2(400, 200)
start = py.math.Vector2(400, 10)
canvas = py.Surface((800, 600))
canvas.fill((255, 255, 255))
draw_tree(canvas, start, end, 10)
while True:
    for e in py.event.get():
        if e.type == py.MOUSEBUTTONDOWN:
            if e.button == 4:
                angle += 5
                canvas.fill((255, 255, 255))
                draw_tree(canvas, start, end, 10)
            if e.button == 5:
                angle -= 5
                canvas.fill((255, 255, 255))
                draw_tree(canvas, start, end, 10)
        if e.type == QUIT:
            py.quit()
            sys.exit()
            break

    window.fill((255, 255, 255))
    window.blit(canvas, (0, 0))
    py.display.flip()
    clock.tick(60)
