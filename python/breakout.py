import sys
import os
import pygame as py
import math
import random
import numpy
from pygame.locals import *
os.environ['SDL_VIDEO_CENTERED'] = '1'
py.init()
py.mixer.quit()
py.display.set_caption("Breakout")
WIDTH = 640
HEIGHT = 480
RANGE = 15
B_WIDTH = 100
B_HEIGHT = 35
P_WIDTH = 100
P_HEIGHT = 20
START_X = 7
START_Y = 10
BSTART_X = (WIDTH - 10) / 2
BSTART_Y = HEIGHT / 2 + 30
colors = {0: (255, 0, 0), 1: (255, 128, 0), 2: (253, 233, 3), 3: (0, 255, 0), 4: (0, 0, 255),
          5: (87, 35, 100)}
window = py.display.set_mode([WIDTH, HEIGHT])
clock = py.time.Clock()

class Paddle:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.width = P_WIDTH
        self.height = P_HEIGHT


class Brick:
    def __init__(self, x, y, color):
        self.x = x
        self.y = y
        self.color = color
        self.is_visible = True
        self.width = B_WIDTH
        self.height = B_HEIGHT


class Ball:
    def __init__(self, x, y, r):
        self.x = x
        self.y = y
        self.r = r
        self.angle = None
        self.velx = 350
        self.vely = -350
        self.on_screen = False

    def update(self, dt):
        self.x += math.cos(math.radians(self.angle)) * self.velx * dt
        self.y += math.sin(math.radians(self.angle)) * self.vely * dt
        if self.y <= 0:
            self.y = BSTART_Y
            self.x = BSTART_X
            self.on_screen = False
        if self.x + self.r > WIDTH:
            if 270 + RANGE <= self.angle <= 360 - RANGE:
                self.angle = numpy.random.uniform(180 + RANGE, 270 - RANGE)
            else:
                self.angle = numpy.random.uniform(90 + RANGE, 180 - RANGE)
            self.x = WIDTH - self.r
        if self.x < 0:
            if 90 + RANGE <= self.angle <= 180 - RANGE:
                self.angle = numpy.random.uniform(RANGE, 90 - RANGE)
            else:
                self.angle = numpy.random.uniform(270 + RANGE, 360 - RANGE)
            self.x = 0

    def detect_collision(self, object):
        cx = self.x 
        cy = self.y
        if self.x < object.x:
            cx = object.x
        elif self.x > object.x + object.width:
            cx = object.x + object.width
        if self.y < object.y:
            cy = object.y
        elif self.y > object.y + object.height:
            cy = object.y + object.height
        diffx = cx - self.x 
        diffy = cy - self.y
        return math.sqrt(diffx**2 + diffy**2) <= self.r

paddle = Paddle((WIDTH - P_WIDTH) / 2, HEIGHT - P_HEIGHT - 5)
ball = Ball(BSTART_X, BSTART_Y, 10)
bricks = []
for i in range(6):
    for j in range(6):
        bricks.append(Brick(START_X + j * (B_WIDTH + 5), START_Y + i * (B_HEIGHT + 5), colors[i]))
dt = clock.tick(60)
start_tick = py.time.get_ticks()
while True:
    seconds = (py.time.get_ticks() - start_tick) / 1000
    if seconds > 1 and not ball.on_screen:
        ball.on_screen = True
        ball.angle = random.uniform(231, 309)
    for e in py.event.get():
        if e.type == QUIT:
            py.quit()
            sys.exit()
    keys = py.key.get_pressed()
    if keys[py.K_LEFT]:
        paddle.x -= 70 * dt / 100
    if keys[py.K_RIGHT]:
        paddle.x += 70 * dt / 100
    window.fill((255, 255, 255))
    if ball.on_screen:
        ball.update(dt / 1000)
        if ball.detect_collision(paddle):
            print(ball.x, ball.y)
            ball.angle = random.uniform(30, 150)
    for brick in bricks:
        if ball.on_screen and ball.detect_collision(brick) and brick.is_visible:
            brick.is_visible = False
            ball.angle = random.uniform(231, 309)
            break
    for brick in bricks:
        if brick.is_visible:
            py.draw.rect(window, brick.color, (brick.x, brick.y, B_WIDTH, B_HEIGHT))
    if ball.on_screen:
        py.draw.circle(window, (0, 0, 0), (ball.x, ball.y), ball.r)
    py.draw.rect(window, (0, 0, 0), (paddle.x, paddle.y, P_WIDTH, P_HEIGHT))
    py.display.flip()
    dt = clock.tick(60)
