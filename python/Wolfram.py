import sys, turtle, random as rand
def filled_rectangle(color, t, x, y, l, w):
        t.penup()
        t.setposition(x, y)
        t.pendown()
        t.fillcolor(color)
        t.begin_fill()
        for i in range(2):
            t.right(90)
            t.forward(l)
            t.right(90)
            t.forward(w)
        t.end_fill()
def main():
    if len(sys.argv) > 1:
        rule = int(sys.argv[1])
        bits = []
        def to_binary(number : int):
            for i in range(0, 8):
                bit = number & (0x1 << (7-i))
                if bit != 0:
                    bits.append(1)
                else:
                    bits.append(0)
        to_binary(rule)
        states = {'111' : bits[0], '110': bits[1], '101': bits[2], '100': bits[3], '011': bits[4], '010': bits[5],
              '001': bits[6], '000': bits[7]}
        gen = ""
        limit = 100
        for i in range(0, limit):
            gen += str(rand.randint(0, 1))
        copy = gen[:]
        generations = 0
        t = turtle.Turtle(visible = False)
        t._delay(0)
        t.ht()
        t._tracer(0, 0)
        arr = []
        screen = turtle.Screen()
        screen.title("Wolfram Rules")
        screen.setup(1280, 720)
        while generations < limit:
            arr.append(gen)
            for i in range(1, len(gen)-1):
                result = states[gen[i-1:i+2]]
                aux = list(copy)
                aux[i] = str(result)
                copy = "".join(aux)
            gen = copy[:]
            generations += 1
        generations = 0
        cell_size = 7
        while len(arr) > 0:
           it = arr.pop(0)
           for j in range(0, limit):
               if it[j] == '1':
                   filled_rectangle("white", t, -380+j*cell_size, 336-generations*cell_size, cell_size, cell_size)
               else:
                   filled_rectangle("black", t, -380+j*cell_size, 336-generations*cell_size, cell_size, cell_size)
           generations += 1
        screen.getcanvas().postscript(file="Rule"+str(rule)+".eps")
        screen.exitonclick()
    else:
        raise Exception("Se espera un argumento")
if __name__ == '__main__':
    main()
