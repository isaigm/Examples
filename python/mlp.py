import os
import sys
import pygame as py
from pygame.locals import *

import numpy as np
from typing import List

def sigmoid(x):
    return 1 / (1 + np.exp(-x))

def sigmoid_deriv(x):
    return np.multiply(sigmoid(x), (1 - sigmoid(x)))

class MLP:
    def __init__(self, num_inputs: int, num_hidden: List[int], num_outputs: int):
        self.num_inputs = num_inputs
        self.num_hidden = num_hidden
        self.num_outputs = num_outputs
        self.weights = []
        self.layers = [num_inputs] + self.num_hidden + [num_outputs]
        self.biases = [np.random.random((self.layers[idx], 1)) for idx in range(1, len(self.layers))]
        for idx in range(1, len(self.layers)):
            dim = (self.layers[idx], self.layers[idx - 1])
            w = np.random.random(dim)
            self.weights.append(np.matrix(w))
       
    def predict(self, inputs):
        curr_inputs = np.array(inputs).reshape((len(inputs), 1))
        for idx in range(len(self.weights)):
            curr_inputs = sigmoid(np.dot(self.weights[idx], curr_inputs) + self.biases[idx])        
        return curr_inputs

    def feed_forward(self, inputs):
        curr_inputs = inputs
        zl = []
        for idx in range(len(self.weights)):
            curr_zl = np.dot(self.weights[idx], curr_inputs) + self.biases[idx]
            zl.append(curr_zl)     
            curr_inputs = sigmoid(curr_zl)        
        return zl
    
    def train(self, inputs, targets):
       pass
       
class NetworkPlot:
    def __init__(self, width, height, network: MLP):
        self.width = width
        self.height = height
        self.network = network
        self.layer_width = width / len(network.layers)
        self.sep_between_neurons = 60
        self.neuron_height = 35
        self.all_neurons_pos = []
        self.font = py.font.SysFont("Chandas", 20)
        curr_x = self.layer_width / 2
        for idx in range(len(self.network.layers)):
            curr_pos = []
            neurons = self.network.layers[idx]
            margin = (self.height - self.sep_between_neurons * (neurons - 1) - neurons * self.neuron_height) / 2
            curr_y = margin
            for _ in range(neurons):
                curr_pos.append((curr_x, curr_y))
                curr_y += self.sep_between_neurons + self.neuron_height
            self.all_neurons_pos.append(curr_pos)
            curr_x += self.layer_width 
    
    def render(self, window: py.Surface):
        for idx in range(len(self.all_neurons_pos)):
           i = 0
           for neuron_pos in self.all_neurons_pos[idx]:
               py.draw.circle(window, (255, 255, 255), neuron_pos, self.neuron_height, 1)
               if idx > 0:
                   text = self.font.render("%.3f" % self.network.biases[idx - 1][i] , True, (0, 255, 0))
                   window.blit(text, neuron_pos)                   
               i += 1
        for idx in range(len(self.all_neurons_pos) - 1):
           curr_layer = self.all_neurons_pos[idx]
           next_layer = self.all_neurons_pos[idx + 1] 
           row = 0
           for curr_neuron in curr_layer:
               col = 0
               for next_neuron in next_layer:
                   v = np.array(next_neuron) - np.array(curr_neuron)
                   dist = np.linalg.norm(v)
                   v /=  dist
                   v = curr_neuron + (dist / 2) * v
                   text = self.font.render("%.3f" % self.network.weights[idx][col, row] , True, (0, 255, 0))
                   py.draw.line(window, (255, 0, 0), curr_neuron, next_neuron)
                   window.blit(text, (v[0], v[1]))
                   col += 1
               row += 1   
        
os.environ['SDL_VIDEO_CENTERED'] = '1'
py.init()
py.mixer.quit()
WIDTH, HEIGHT = 1280, 720
window = py.display.set_mode([WIDTH, HEIGHT])
clock = py.time.Clock()
mlp = MLP(1, [6, 1, 2], 1)
drawer = NetworkPlot(WIDTH, HEIGHT, mlp)
while True:
    for e in py.event.get():
        if e.type == QUIT:
            py.quit()
            sys.exit()
    window.fill((0, 0, 0))
    drawer.render(window)
    py.display.flip()
    clock.tick(60)
