import os
import sys
import pygame as py
from pygame.locals import *
import numpy as np
from typing import List

#http://neuralnetworksanddeeplearning.com/chap2.html
py.font.init()
def sigmoid(x):
    return 1 / (1 + np.exp(-x))

def sigmoid_deriv(x):
    return sigmoid(x) * (1 - sigmoid(x))

def to_np_array(x):
    return np.array(x).reshape((len(x), 1))

class MLP:
    def __init__(self, num_inputs: int, num_hidden: List[int], num_outputs: int):
        self.num_inputs = num_inputs
        self.num_hidden = num_hidden
        self.num_outputs = num_outputs
        self.weights = []
        self.layers = [num_inputs] + self.num_hidden + [num_outputs]
        self.biases = [np.random.randn(self.layers[idx], 1) for idx in range(1, len(self.layers))]
        for idx in range(1, len(self.layers)):
            w = np.random.randn(self.layers[idx], self.layers[idx - 1])
            self.weights.append(w)
       
    def predict(self, inputs):
        curr_inputs = to_np_array(inputs)  #np.array(inputs).reshape((len(inputs), 1))
        for idx in range(len(self.weights)):
            curr_inputs = sigmoid(np.dot(self.weights[idx], curr_inputs) + self.biases[idx])        
        return curr_inputs

    def train(self, training_data, epochs, learning_rate, mini_batch_size):
        for _ in range(epochs):
            np.random.shuffle(training_data)
            for idx in range(0, len(training_data), mini_batch_size):
                mini_batch = training_data[idx: idx + mini_batch_size]
                gradient_bias =  [np.zeros(bias.shape) for bias in self.biases]
                gradient_weights = [np.zeros(weight.shape) for weight in self.weights]
                for x, y in mini_batch:
                    w, b = self.backpropagation(x, y)
                    gradient_bias = [nb+dnb for nb, dnb in zip(gradient_bias, b)]
                    gradient_weights = [nw+dnw for nw, dnw in zip(gradient_weights, w)]
                self.weights = [w1 - (learning_rate/len(mini_batch)) * nw for w1, nw in zip(self.weights, gradient_weights)]
                self.biases = [b1 - (learning_rate/len(mini_batch)) * nb for b1, nb in zip(self.biases, gradient_bias)]
    def backpropagation(self, inputs, target):
        curr_inputs = to_np_array(inputs)
        target = to_np_array(target)
        zl = []
        activations = [curr_inputs]
        errors = [np.zeros(bias.shape) for bias in self.biases]
        for idx in range(len(self.weights)):
            curr_zl = np.dot(self.weights[idx], curr_inputs) + self.biases[idx]
            zl.append(curr_zl)     
            curr_inputs = sigmoid(curr_zl)
            activations.append(curr_inputs)
        errors[-1] = (curr_inputs - target) * sigmoid_deriv(zl[-1])
       
        for idx in range(len(errors) - 2, -1, -1):
            errors[idx] = np.dot(self.weights[idx + 1].transpose(), errors[idx + 1]) * sigmoid_deriv(zl[idx])
        nabla_weights = [np.dot(errors[layer], activations[layer].transpose()) for layer, _ in enumerate(self.weights)]
        
        return (nabla_weights, errors)
    
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
        for idx, neurons_pos in enumerate(self.all_neurons_pos):
           i = 0
           for neuron_pos in neurons_pos:
               py.draw.circle(window, (255, 255, 255), neuron_pos, self.neuron_height, 1)
               if idx > 0:
                   text = self.font.render("%.3f" % self.network.biases[idx - 1][i] , True, (0, 255, 0))
                   window.blit(text, neuron_pos)                   
               i += 1
        for idx in range(len(self.all_neurons_pos) - 1):
           curr_layer = self.all_neurons_pos[idx]
           next_layer = self.all_neurons_pos[idx + 1] 
           for row, curr_neuron in enumerate(curr_layer):
               for col, next_neuron in enumerate(next_layer):
                   v = np.array(next_neuron) - np.array(curr_neuron)
                   dist = np.linalg.norm(v)
                   v /=  dist
                   v = curr_neuron + (dist / 2) * v
                   text = self.font.render("%.3f" % self.network.weights[idx][col, row] , True, (0, 255, 0))
                   py.draw.line(window, (255, 0, 0), curr_neuron, next_neuron)
                   window.blit(text, (v[0], v[1]))
                   
               
        
os.environ['SDL_VIDEO_CENTERED'] = '1'
py.init()
py.mixer.quit()
WIDTH, HEIGHT = 1600, 900
window = py.display.set_mode([WIDTH, HEIGHT])
clock = py.time.Clock()
mlp = MLP(1, [16, 16], 1)
canvas = py.Surface((WIDTH, HEIGHT))
canvas.fill((0, 0, 0))

training_data = []
x = 0
for _ in range(100):
    training_data.append(([x], [np.sin(x)]))
    x += 1 / 100

mlp.train(training_data, 60, 1.55, 10)

print(mlp.predict([0.2]))
print(mlp.predict([0]))
print(mlp.predict([0.3]))
print(mlp.predict([0.5]))
print(mlp.predict([0.7]))
print(mlp.predict([0.4]))
print(mlp.predict([0.9]))


drawer = NetworkPlot(WIDTH, HEIGHT, mlp)
drawer.render(canvas)
while True:
    for e in py.event.get():
        if e.type == QUIT:
            py.quit()
            sys.exit()
    window.fill((0, 0, 0))
    window.blit(canvas, (0, 0))
    py.display.flip()
    clock.tick(60)
