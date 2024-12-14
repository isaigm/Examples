import pygame as py
import sys
import numpy as np
import pandas as pd
from sklearn.naive_bayes import GaussianNB
from pygame.locals import *
py.init()

np.random.seed(100)
WIDTH, HEIGHT = 800, 600

class point:
    def __init__(self, x, y, category, r=5):
        self.x = x
        self.y = y
        self.category = category
        self.r = r

    def draw(self, window):
        if self.category == 1:
            color = (255, 0, 0)
        else:
            color = (0, 255, 255)
        py.draw.circle(window, color, (self.x, self.y), self.r)

def get_random_point():
    return np.random.random(2) * (WIDTH, HEIGHT)

def gen_data(n):
    p1 = get_random_point()
    p2 = get_random_point()
    df = pd.DataFrame(columns=["feature_1", "feature_2", "category"])
    m = (p1[1] - p2[1]) / (p1[0] - p2[0]) 
    linear_func = lambda x: m * (x - p1[0]) + p1[1]
    points = []
    for _ in range(n):
        p = np.array(get_random_point(), dtype='int')
        y = linear_func(p[0])
        if y - p[1] > 0:
            points.append(point(p[0], p[1], 1))
            df.loc[len(df)] = {"feature_1": p[0], "feature_2": p[1], "category": 1}

        else:
            points.append(point(p[0], p[1], 0))
            df.loc[len(df)] = {"feature_1": p[0], "feature_2": p[1], "category": 0}

    return points, df

def gnb(df: pd.DataFrame, x):
    likelihood = lambda x, mean, std: np.exp(-(x - mean)**2 / (2*std**2)) / (std * np.sqrt(np.pi * 2))
    max_arg = -np.inf
    class_predicted = None
    categories = df.iloc[:,-1].unique()
    probs = np.zeros(len(categories), dtype='float64')
    columns = df.columns.values[:-1]
    for category in categories: 
        cla = df[df.iloc[:,-1] == category]
        p = len(cla) / len(df)
        for i, col in enumerate(columns):
            col_mean = cla[col].mean()
            col_std  = max(cla[col].std(), 1e-9)
            p *= likelihood(x[i], col_mean, col_std)
        if p > max_arg:
            max_arg = p
            class_predicted = category
        probs[category] = p
    probs /= sum(probs)
    return class_predicted, probs
        

points, df = gen_data(100)
g = GaussianNB()
g.fit(df.drop(["category"], axis="columns").to_numpy(), df["category"].astype('int'))

py.display.set_caption("Random Walk")
window = py.display.set_mode([WIDTH, HEIGHT])


clock = py.time.Clock()
while True:
    for e in py.event.get():
        if e.type == QUIT:
            py.quit()
            sys.exit()
        if e.type == MOUSEBUTTONDOWN:
            f1, f2 = py.mouse.get_pos()
            _, p1 = gnb(df, [f1, f2])
            p2 = g.predict_proba([[f1, f2]])[0]
         
            print(p1, p2)
            points.append(point(f1, f2, _))
    window.fill((0, 0, 0))
    for p in points:
        p.draw(window)
    py.display.flip()    
    clock.tick(60)
