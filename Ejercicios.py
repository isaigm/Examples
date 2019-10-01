#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Sep 17 13:48:03 2019
@author: isai
"""
import math, bitstring, numpy as np
import matplotlib.pyplot as plt,  matplotlib.patches as patches
import pygame
from pygame.locals import * 
pygame.init()
pygame.mixer.quit()

def Ej1():
    print(2**16 - 1)
    print('1' * 16)
def Ej2():
    for number in range(0, 19):
        print("Numero {0} -> bits necesarios {1}".format(number, math.ceil(math.log2(number+1))))
def Ej3():
    a = bitstring.BitArray(bin = '01111111')
    b = bitstring.BitArray(bin = '00000001')
    print((a+b).bin)
    a = bitstring.BitArray(bin = '01010101')
    b = bitstring.BitArray(bin = '10101010')
    print((a+b).bin)
    a = bitstring.BitArray(bin = '00000011')
    b = bitstring.BitArray(bin = '00000001')
    print((a+b).bin)
def Ej7(array):
    media = 0
    for i in array:
        media += i
    return media/len(array)
def Ej8():
    nums = [1, 30, 20, 10, 20]
    varianza = 0
    media = Ej7(nums)
    for i in nums:
        varianza += (i - media)**2
    varianza /= len(nums)
    print("Varianza {0:.2f}".format(varianza))
def Ej9(radio):
    return math.pi * radio**2
def Ej10(producto_precio):
    return 0.16 * producto_precio
#Ej11 no, tiene que ser finito, pi tiene infinitos decimales, no terminaria
#Ej12 recursivamente?
def Ej15():
    a = 2 + 3*(6/2)
    b = (4 + 6) / (2 + 3)
    c = 4**5 / 2**5
    d = 4**6 / 2**6
    e = (-3)**2
    f = -(3**2)
"""
Ej16
a) 0.125
b) 0.125
c) 0.125
d) 0.125
e) 2.0
f) 2.0
g) 2.5
h) 2.5
i) 300.0
j) 2000.0
k) 2001.0
l) 2.5
"""
"""
Ej17
True
True
True
False
True
"""
"""
Ej18
a)si
b)no
c)no
d)no
e)si
f)si
g)no
h)no
i)no
j)no
k)no
l)si
"""
#Ej19 100
def Ej20(x):
    result = x**4 + x**3 + 2*x**2 - x
    return result
def Ej21(x):
    result = x**4 + x**3 + 0.5*x**2 - x
    return result
"""
Ej22
2
4
4
8
16
8
5
2.5
-1.5
-3.5
-42.875
"""
"""
Ej23
a) a = 'b'
b) 'bb'
c) 'ba'
d) 'bbbbb'
e) 'bbbb'
"""
"""
Ej24
a) 'aaa/*/*/*/*/*abcabc+'
b) '<<<<abcba>>>><<<<abcba>>>>'
c) '1010101010=---==---==---==---='
d) '1212.333e⁻76767676'
"""
"""Ej25
a) expr = '%'*4 + './'*3 + '<->'*2
b) sub = 3*'(@)'
   sub2 = '='*6
   expr = sub + sub2 + sub + sub2
c) sub = 'asdf'
   expr = sub*3 + '=-'*7 + '?'*6 + sub*2
d) expr = ’........*****---*****---.'*2
"""
"""
Ej26
True
False
True
True
False
True
True
"""
def Ej27():
    res = math.floor(math.fabs(-3.2))
    print('{0:.1f}'.format(res))
def Ej28():
    res = '{0:.3f}'.format(5011/10000)
    print(res)
"""Ej29
'2.11.2'
23
12.30
23
'5'
'5.1'
"""
"""Ej30
"""
def Ej31(lado = 3):
    print("Perimetro = {0:.2f}".format(lado*4))
    print("Area = {0:.2f}".format(lado**2))
def Ej32(base = 3, altura = 5):
    print("Area {0:.2f}".format(base*altura/2))
def Ej33(ancho = 4, altura = 6):
    print("Area {0:.2f}".format(ancho*altura))
def Ej34():
    lado = int(input("Lado del cuadrado: "))
    Ej31(lado)
def Ej35():
    ancho = int(input("Ancho del rectangulo: "))
    alto = int(input("Alto del rectangulo: "))
    Ej32(ancho, alto)
    print("Area {0:.2f}".format(2*(ancho+alto)))
def Ej36():
    ancho = int(input("Base del triangulo: "))
    alto = int(input("Altura del triangulo: "))
    Ej32(ancho, alto)
def Ej37():
    print("Ingrese lados del triangulo separados por espacio: ")
    lados = list(map(int, input().split()))
    s = sum(lados)/2
    res = math.sqrt(s*(s-lados[0])*(s-lados[1])*(s-lados[2]))
    print("Area = {0:.6f}".format(res))
    print("Perimetro = {0:.3f}".format(s*2))
def Ej38():
    a = int(input("Lado a: "))
    b = int(input("Lado b: "))
    theta = float(input("Angulo: "))
    theta = theta*math.pi/180
    area = a*b*math.sin(theta)/2
    print("Area = {0:.6f}".format(area))
def Ej39():
    c = int(input("Capital: "))
    x = float(input("Tasa de interes: "))
    n = int(input("Años: "))
    gan = c * (1 + x/100)**n
    print("Ganancia -> {0:.3f}".format(gan))
def Ej40():
    nombre = input("Nombre: ")
    print((nombre + ' ')*1000)
"""
Ej41
1
1 2
12
1, 2
1 2
"""
#Ej42 manda a upper antes
#Ej43 Sintaxis invalida
def Ej45():
    radio = float(input("Radio: "))
    print("Area {0:.2f}".format(math.pi*radio**2))
    print("Perimetro {0:.2f}".format(2*math.pi*radio))
def Ej46():
    fig, ax = plt.subplots(1)
    rect = patches.Rectangle((.100, .100), .800, .800, linewidth=1, edgecolor='r', facecolor='none')
    ax.set_aspect('equal', adjustable='datalim')
    ax.add_patch(rect)
    plt.plot([.100, .900], [.100, .900], linewidth=1)
    plt.plot([.900, .100], [.100, .900], linewidth=1)
    plt.show()
def Ej47():
    fig, ax = plt.subplots()
    ax.set_aspect('equal', adjustable='datalim')
    ax.add_patch(plt.Circle((0.5, 0.5), 1, color='r', alpha=0.5, fill=False))
    ax.add_patch(plt.Circle((0.5, 0.5), 0.5, color='r', alpha=0.5, fill=False))
    ax.add_patch(plt.Circle((0.5, 0.5), 0.1, color='r', alpha=0.5))
    plt.plot([.5, .5], [-0.50, 1.50], linewidth=1)
    plt.plot([-.5, 1.5], [.5, .5], linewidth=1)
    ax.plot()
    plt.show()
    
def Ej48():
    fig, ax = plt.subplots()
    ax.set_aspect('equal', adjustable='datalim')
    ax.add_patch(plt.Circle((0.5, 0.5), 1, color='r', alpha=0.5, fill=False))
    ax.add_patch(plt.Circle((0.5, 0.5), 0.5, color='r', alpha=0.5, fill=False))
    ax.add_patch(plt.Circle((0.5, 0.5), 0.1, color='r', alpha=0.5))
    plt.plot([.5, .5], [-0.50, 1.50], linewidth=1)
    plt.plot([-.5, 1.5], [.5, .5], linewidth=1)
    plt.text(0.5, 1, "a", fontsize=20)
    plt.text(1.0, 0.5, "b", fontsize=20)
    plt.text(0.5, 0, "c", fontsize=20)
    plt.text(0.0, 0.50, "d", fontsize=20)
    ax.plot()
    plt.show()
    
def Ej49():
    fig, ax = plt.subplots()
    ax.set_aspect('equal', adjustable='datalim')
    ax.add_patch(plt.Circle((0.5, 0.5), 1, color='b', alpha=0.5, fill=False))
    ax.add_patch(plt.Circle((0.5, 0.5), 0.5, color='g', alpha=0.5, fill=False))
    ax.add_patch(plt.Circle((0.5, 0.5), 0.1, color='r', alpha=0.5))
    plt.plot([.5, .5], [-0.50, 1.50], linewidth=1)
    plt.plot([-.5, 1.5], [.5, .5], linewidth=1)
    plt.text(0.5, 1, "a", fontsize=20)
    plt.text(1.0, 0.5, "b", fontsize=20)
    plt.text(0.5, 0, "c", fontsize=20)
    plt.text(0.0, 0.50, "d", fontsize=20)
    ax.plot()
    plt.show()   
    
def Ej50(): 
    # Pie chart, where the slices will be ordered and plotted counter-clockwise:
    suspensos = float(input("Suspensos: "))
    sobre = float(input("Sobresalientes: "))
    ap = float(input("Aprobados: "))
    n = float(input("Notables: "))
    labels = 'suspensos', 'sobresaliente', 'aprobados', 'notables'
    sizes = [suspensos, sobre, ap, n]
    explode = (0, 0.0, 0, 0)  # only "explode" the 2nd slice (i.e. 'Hogs')
    fig1, ax1 = plt.subplots()
    ax1.pie(sizes, explode=explode, labels=labels, autopct='%1.1f%%',
        shadow=True, startangle=90)
    ax1.axis('equal')  # Equal aspect ratio ensures that pie is drawn as a circle.

    plt.show()
    
def Ej51():
    suspensos = float(input("Suspensos: "))
    sobre = float(input("Sobresalientes: "))
    ap = float(input("Aprobados: "))
    n = float(input("Notables: "))
    total = suspensos + sobre + ap + n 
    labels = 'suspensos', 'sobresaliente', 'aprobados', 'notables'
    sizes = [suspensos*100/total, sobre*100/total, ap*100/total, n*100/total]
    explode = (0, 0.0, 0, 0)  # only "explode" the 2nd slice (i.e. 'Hogs')
    fig1, ax1 = plt.subplots()
    ax1.pie(sizes, explode=explode, labels=labels, autopct='%1.1f%%',
        shadow=True, startangle=90)
    ax1.axis('equal')  # Equal aspect ratio ensures that pie is drawn as a circle.

    plt.show()
"""
def Ej52():
    x = [1,2,3,4]
    num_bins = 4
    n, bins, patches = plt.hist(x, num_bins, facecolor='blue', alpha=0.5)
    plt.show()
"""
    
#eEj53 Error de sintaxis
    
#Ej54 Variables no declaradas
    
#Ej55 a=0 es asignación, no devuelve booleano
   
#Ej56 falta else

#Ej57 solo entra en a) 
    
def Ej58():
    numero = float(input("numero: "))
    if(numero<0):
        print("negativo")
        
def Ej59():
    numero = float(input("numero: "))
    if(numero>=0):
        print("positivo")

def Ej60():
    p1 = int(input("edad 1:"))
    p2 = int(input("edad 2:"))
    if(p1>p2):
        print("persona 1 es mayor")
    elif(p2>p1):
        print("persona 2 es mayor")
    else:
        print("son de la misma edad")
        
def Ej61():
    char = input("caracter: ") 
    if(char == '(' or char == ')'):
        print("es parentesis")
        
# Ej62 
#a)   a, m
#b)   360, 90, 180, 270
        
#Ej 63  primer saludo 
        
def Ej64():
    numero = int(input("Numero: "))
    if(numero & 1 == 0):
        print("es par")
    else:
        print("impar")

def Ej65():
    numero = int(input("Numero: "))
    k = numero//2
    if(k & 1 == 1):
        print("es el doble de un impar")
        
def ej66():
    num = int(input("numero 1: "))
    num2 = int(input("numero 2: "))
    
    if( num**2 == num2 ):
        print("el segundo es el cuadrado exacto del primero")
    elif(num2 < num**2):
        print("el segundo es menor que el cuadrado del primero")
    else: 
        print("El segundo es mayor que el cuadrado del primero")
    
def E67():
    c = int(input("Capital: "))
    x = float(input("Tasa de interes: "))
    n = int(input("Años: "))
    if(x>0):
        gan = c * (1 + x/100)**n
        print("Ganancia -> {0:.3f}".format(gan))

def Ej68():
    amount = int(input("Cantidad: "))
    billetes = [500, 200, 100, 50, 20, 10, 5, 2, 1]
    resto = 0
    subcantidad = 0
    for billete in billetes:
        subcantidad = amount//billete
        resto = amount % billete
        if resto == 0:
            print("{0} billetes de {1}".format(subcantidad, billete))
            break
        elif amount > billete:
            print("{0} billetes de {1}".format (subcantidad, billete))
            amount = resto
        else:
            continue
def Ej72():
    char = input("Introduce un caracter: ")
    if char >= 'a' and char <= 'z':
        print("Es MINUSCULA")
    elif char >= 'A' and char <= 'Z':
        print("Es MAYUSCULA")
def Ej73():
    char = input("Introduce un caracter: ")
    if char >= 'a' and char <= 'z':
        print("Es MINUSCULA")
    elif char >= 'A' and char <= 'Z':
        print("Es MAYUSCULA")
    else:
        print("No es una letra")
def Ej74():
    char = input("Introduce un caracter: ")
    if char >= 'a' and char <= 'z':
        print("Es MINUSCULA")
    elif char >= 'A' and char <= 'Z':
        print("Es MAYUSCULA")
    else:
        if char == 'ñ':
            print("ñ MINUSCULA")
        elif char == 'Ñ':
            print("ñ MAYUSCULA")
        else:
            print("No es una letra")
#Ej76 error de dominio, la funcion sqrt espera un argumento >= 0
#Ej78 deberia mostrar error porque si los numeros son iguales, la variable maximo no llega a declararse
def Ej80():
    numeros = [1, 10, -1, 41, 41]
    mayor = numeros[0]
    for it in numeros:
        if mayor < it:
            mayor = it
    print("El mayor es " + str(mayor))
def Ej81():
    palabras = ["hola", "mundo", "como", "es", "aan"]
    mayor = palabras[0]
    for it in palabras:
        if mayor < it:
            mayor = it
    print("El mayor es " + mayor)
def Ej83():
    numeros = [2, 6, 4, 1, 10]
    distancia = abs(numeros[0] - numeros[1])
    cercano = numeros[1]
    for i in range(2, len(numeros)):
        if distancia > abs(numeros[0] - numeros[i]):
            distancia = abs(numeros[0] - numeros[i])
            cercano = numeros[i]
    print("El numero mas cercano es " + str(cercano))
def Ej84():
    distancia = lambda arr1, arr2: math.sqrt((arr1[0]-arr2[0])**2 + (arr1[1]-arr2[1])**2)
    puntos = ([1, 2], [-1, 4], [1, 1.5], [3, 4], [-10, 10])
    dist = distancia(puntos[0], puntos[1])
    cercano = puntos[1]
    for i in range(2, len(puntos)):
        if dist > distancia(puntos[0], puntos[i]):
            dist = distancia(puntos[0], puntos[i])
            cercano = puntos[i]
    print(cercano)
"""
Ej85
a) dia entre 0 y 15, dia entre 15 y 32, dia mayor
b) mes entre 1 y 3, mes es 4 o 5 0 6, si mes no es < 7 o no es > 9, si no es ni 10 ni 11 ni 12
"""
def Ej87():
    cal = 8.5
    cal_cuali = ""
    if cal <= 5:
        cal_cuali = "suspenso"
    elif cal > 5 and cal < 7:
        cal_cuali = "aprobado"
    elif cal >= 7 and cal < 8.5:
        cal_cuali = "notable"
    elif cal >= 8.5 and cal < 10:
        cal_cuali = "sobresaliente"
    else:
        cal_cuali = "matricula de honor"
    print(cal_cuali)
def Ej88():
    vocal_min = ['a', 'e', 'i', 'o', 'u']
    vocal_may = [vocal.upper() for vocal in vocal_min]
    char = 'A'
    if char in vocal_min:
        print("vocal minuscula")
    elif char in vocal_may:
        print("vocal mayuscula")
#ej89 se esta dividiendo entre 0
#ej90 no, no estan declaradas a, b, c
#Ej91 14, 18
"""
Ej95 0\n 1\n 2\n 3\n Hecho\n
Ej96 0\n 2\n 4\n 6\n 8\n Hecho\n
Ej97 3\n 5\n 7\n 9\n Hecho\n
Ej100 no entra en el bucle; si es negativo, entra en el bucle
"""
def Ej103():
    for multiplo in range(1, 151//6):
        print(multiplo*6)
def Ej104():
    n = int(input("Valor de n: "))
    m = int(input("Valor de m: "))
    for multiplo in range(n, (m*n+1), n):
        print(multiplo)
def Ej105():
    for pot in range(0, 30+1):
        print(2**pot)
def Ej107():
    n = int(input("Inf: "))
    m = int(input("Sup: "))
    res = 0
    for num in range(n, m+1):
        res += num
    print("Resultad " + str(res))
def Ej108():
    n = int(input("Inf: "))
    m = int(input("Sup: "))
    if n > m:
        print("No se puede calcular")
    else:
        res = 0
        for num in range(n, m+1):
            res += num
            print("Resultad " + str(res))
def Ej109():
    n = int(input("Valor de n: "))
    product = 1
    for i in range(1, n + 1):
        product *= i
    print("{0}! = {1}".format(n, product))
def Ej110():
    n = int(input("Valor de n: "))
    m = int(input("Valor de m: "))
    if n < m:
        raise ValueError("n debe ser mayor que m")
    def Factorial(k = 1):
        product = 1
        for i in range(1, k + 1):
            product *= i
        return product
    combinations = Factorial(n)//(Factorial(n-m)*Factorial(m))
    print("Combinaciones posibles " + str(combinations))
#Ej111 es correcto
def Ej112():
    flag = True
    while flag:
        num = int(input("Introduce un numero entre 1 y 10"))
        if num >= 1 and num <= 10:
            flag = False
def Ej113():
    flag = True
    while flag:
        texto = input("Introduce un texto: ")
        if any([letra.isupper() for letra in texto]):
            print("No letras mayusculas")
        else:
            flag = False
def Ej116():
    vector1 = []
    vector2 = []
    def capturar(arr):
        for i in range(0, 3):
            componente = int(input("Introduce componente {0}: ".format(i+1)))
            arr.append(componente)
    def producto_punto(vector1, vector2):
        dot_product = sum([i*j for i, j in zip(vector1, vector2)])
        return dot_product
    def longitud(vector):
        return math.sqrt(vector[0]**2 + vector[1]**2 + vector[2]**2)
    flag = True
    while flag:
        print("1) Introducir el primer vector\n2) Introducir el segundo vector\n3) Calcular la suma")
        print("4) Calcular la diferencia\n5) Producto escalar\n6) Producto vectorial\n7) Calcular angulo")
        print("8) Longitud\n9) Finalizar")
        choice = int(input())
        if choice == 1:
            capturar(vector1)
        elif choice == 2:
            capturar(vector2)
        elif choice == 3:
            print("La suma es {0}".format(sum(vector1) + sum(vector2)))
        elif choice == 4:
            print("La diferencia es {0}".format(sum(vector1) - sum(vector2)))
        elif choice == 5:
            dot_product = producto_punto(vector1, vector2)
            print("El producto punto es {0}".format(dot_product))
        elif choice == 6:
            cross = [vector1[1]*vector2[2] - vector1[2]*vector2[1], vector1[2]*vector2[0] - vector1[0]*vector2[2], vector1[0]*vector2[1] - vector1[1]*vector2[0]]
            print("Producto cruz")
            print(cross)
        elif choice == 7:
            arg = producto_punto(vector1, vector2) / (longitud(vector1)*longitud(vector2))
            ang = (180/math.pi) * math.acos(arg)
            print("Angulo {0:.3f}".format(ang))
        elif choice == 8:
            vec = int(input("Cual vector?: "))
            if vec == 1:
                print("Longitud {0:.3f}".format(longitud(vector1)))
            elif vec == 2:
                print("Longitud {0:.3f}".format(longitud(vector2)))
        elif choice == 9:
            flag = False
        else:
            print("Opcion incorrecta")
def Ej117():
    num = int(input("Dame un numero: "))
    for i in range(1, 11):
        print("{0} x {1} = {2}".format(num, i, num*i))
def Ej119():
    num = int(input("Dame un numero: "))
    for r in range(2, 101):
        print("{0}^(1/{1}) = {2:.3f}".format(num, r, num**(1/r)))
def Ej120():
    for par in range(0, 201, 2):
        print(par)
def Ej121():
    for par in range(200, -1, -2):
        print(par)
def Ej122():
    num = int(input("Dame un numero: "))
    for par in range(2, num, 2):
        print(par)
def Ej124():
    n = int(input("Dame un numero: "))
    m = int(input("Dame otro numero: "))
    suma = 0
    for i in range(n, m + 1):
        suma += i**2
    print("Suma de cuadrados %" % suma) 
def Ej129(a, b):
    r = a % b
    while r != 0:
        a = b
        b = r
        r = a % b
    return b
def Ej130(a, b, c):
    return Ej129(Ej129(a, b), c)
def Ej133():
    n =  int(input("Introduce un numero > 0"))
    div = 0
    for i in range(2, int(math.sqrt(n)) + 1):
        if n % i == 0:
            div += 1
        
    if div == 0:
        print("{0} es numero primo".format(n))
def Ej135():
    numeros = []
    flag = True
    while flag:
        n = int(input("Introduce un numero: "))
        if n < 0:
            flag = False
            print("Numero maximo: {0}".format(max(numeros)))
        else:
            numeros.append(n)
def Ej139():
    x = np.linspace(-2, 2, 100)
    y = 1/(x+1)
    plt.plot([-1], [0], marker='o', markersize=3, color="red")
    plt.plot(x, y)
    plt.show()
    
def Ej140():
    a = 1
    b = -2
    c = -3
    z1 = -10
    z2 = 10
    x = np.linspace(z1, z2, 400)
    y = a*x**2 + b*x + c
    plt.plot(x, y)
    print(np.min(y))
    print(np.max(y))
    print(np.roots([a, b, c]))
    plt.axhline(0, color='black')
    plt.axvline(0, color='black')
    plt.show()
    
def Ej142():
    screen = pygame.display.set_mode((800, 600))
    flag=True
    x1 = 400
    y1 = 300
    velx1 = 0.1
    vely1 = 0
    m1 = 20
    x2 = x1 #+ 100
    y2 = y1 #+ 100
    velx2 = -0.1
    vely2 = 0
    m2 = 20
    i = 0
    while(flag):
        for event in pygame.event.get():
            if event.type==pygame.QUIT:
                flag=False     
        if i < 10000:
            r3 = math.sqrt((x2- x1)**2 + (y2-y1)**2)**3      
            try:
                a_x1 = m2*(x2 - x1)/r3
                a_y1 = m2*(y2-y1)/r3
                a_x2 = m1*(x1-x2)/r3
                a_y2 = m1*(y1-y2)/r3
                velx1 += a_x1
                vely1 += a_y1
                velx2 += a_x2
                vely2 += a_y2
                x1 += velx1
                y1 += vely1
                x2 += velx2
                y2 += vely2
                i += 1
            except Exception as e:
                print(e)
                print('Cuerpos en misma pos')
                i = 10001
            
        screen.fill((255, 255, 255))
        pygame.draw.circle(screen, (255,0,100),(int(x1), 600-int(y1)), m1)
        pygame.draw.circle(screen, (100,0,255),(int(x2), 600-int(y2)), m2)
        
        pygame.display.flip()
    pygame.quit()
    
    
def Ej143():
    screen = pygame.display.set_mode((800, 600))
    flag=True
    x1 = 400
    y1 = 300
    velx1 = 0.1
    vely1 = 0
    m1 = 20
    x2 = x1 + 100
    y2 = y1 + 100
    velx2 = -0.1
    vely2 = 0
    m2 = 20
    i = 0
    pygame.display.set_caption('Simulación gravitacional')
    while(flag):
        for event in pygame.event.get():
            if event.type==pygame.QUIT:
                flag=False  
        r3 = math.sqrt((x2- x1)**2 + (y2-y1)**2)**3      
        try:
            a_x1 = m2*(x2 - x1)/r3
            a_y1 = m2*(y2-y1)/r3
            a_x2 = m1*(x1-x2)/r3
            a_y2 = m1*(y1-y2)/r3
            velx1 += a_x1
            vely1 += a_y1
            velx2 += a_x2
            vely2 += a_y2 
            x1 += velx1
            y1 += vely1
            x2 += velx2
            y2 += vely2
        except Exception as e:
            print(e)
            print('Cuerpos en misma pos')
           
            
        screen.fill((255, 255, 255))
        pygame.draw.circle(screen, (255,0,100),(int(x1), 600-int(y1)), m1)
        pygame.draw.circle(screen, (100,0,255),(int(x2), 600-int(y2)), m2)
        
        pygame.display.flip()
    pygame.quit()
    
def Ej145():
    screen = pygame.display.set_mode((1280, 680))
    pygame.display.set_caption('Simulación nave')
    class Entity(pygame.sprite.Sprite):
        def __init__(self, ruta, pos, size):
            pygame.sprite.Sprite.__init__(self)
            self.img = pygame.image.load(ruta).convert_alpha()
            self.sc = pygame.transform.scale(self.img, size)
            self.rect = self.sc.get_rect()
            self.rect.x = pos[0]
            self.rect.y = pos[1]
            
            
    quitar = True
    nave = Entity('/home/xubuntu/Downloads/nave.png',(600, 50), (90, 90))
    plataforma = Entity('/home/xubuntu/Downloads/plataforma.png',(0, 600), (500, 50))
    g = -.001
    vy = 0
    while quitar:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                quitar = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_LEFT:
                    nave.rect.x -= 40
                elif event.key == pygame.K_RIGHT:
                    nave.rect.x += 40    
                elif event.key == pygame.K_UP:
                    nave.rect.y -= 40
                elif event.key == pygame.K_DOWN:
                    nave.rect.y += 40

            
        screen.fill((0, 0, 0))
        screen.blit(nave.sc, nave.rect)
        screen.blit(plataforma.sc, plataforma.rect)
        pygame.display.flip()
    pygame.quit()
        
    
    
"""
Ej152 
/n
octal
        uno
"""
#Ej153 print('\\\\')
#Ej154 emite un pitido
def Ej156():
    DNI = int(input('Ingrese numero del DNI: '))
    final = 'TRWAGMYFPDXBNJZSQVHLCKE'
    index = DNI % 23
    result = str(DNI) + final[index]
    print(result)
#Ej157 no funciona
#Ej158 si funciona
if __name__ == "__main__":
    Ej1()
    Ej2()
    Ej3()
    Ej8()
    print("{0:.3f}".format(Ej20(1.1)))
    print("{0:.3f}".format(Ej21(10)))
    Ej27()
    Ej28()
    Ej31(4)
    Ej32()
    #Ej37()
    #Ej38()
    #Ej39()
    #Ej40()
    #Ej45()
    #Ej46()
    #Ej47()
    #Ej48()
    #Ej49()
    #Ej50()
    #Ej51()
    #Ej58()
    #Ej60()
    #Ej61()
    #Ej64()
    #Ej65()
    #ej66()
    #E67()
    #Ej68()
    #Ej72()
    #Ej73()
    #Ej74()
    #Ej80()
    #Ej81()
    #Ej83()
    #Ej84()
    #Ej87()
    #Ej88()
    #Ej103()
    #Ej104()
    #Ej105()
    #Ej107()
    #Ej109()
    #Ej110()
    #Ej113()
    #Ej116()
    #Ej117()
    #Ej119()
    #Ej120()
    #Ej121()
    #Ej129(124, 18)
    print("MCD = {0}".format(Ej130(18, 99, 48)))
    #Ej133()
    #Ej135()
    #Ej139()
    #Ej140()
    #Ej142()
    #Ej143()
    #Ej145()
    Ej156()
                    
