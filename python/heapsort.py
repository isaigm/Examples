import random
#la idea de heapsort es construir un arbol binario que cumpla que la raiz sea mayor que sus hijos
#y a su vez estos hijos tambien cumplan con tal propiedad de tal forma que nos aseguramos de
#que el mayor elemento se encuentra siempre en la raiz del arbol
#este arbol cumple con que es un arbol completo, excepto en el ultimo nivel

#este arbol no es necesario construirlo mediante nodos, ya que se puede usar un arreglo para su
#representacion

#asi pues los hijos del nodo i son 2*i + 1 izquierdo y 2*i + 2 derecho y su padre seria (i - 1) // 2
#el metodo __push agrega un elemento al final en el heap y si resulta que es mayor que su padre, entonces
#se intercambia y si esto vuelve a suceder con el padre, entonces se repite hasta que se llegue a la raiz o
#hasta que el nodo ya este bien posicionado, esto nos asegura de que siempre tendremos en la posicion 0 del arreglo
#el elemento mayor, por lo que la complejidad de obtener el maximo es O(1)

#el metodo __pop regresa el maximo elemento y reestructura el arbol para seguir teniendo el maximo en la posicion 0
#primero coloca el ultimo elemento al pricipio del arreglo
#busca en los hijos cual es el mayor de ellos, compara ese hijo con el nodo padre, si es mayor, entonces
#se intercambian y se repite esto con el nodo hijo como nuevo padre
class heapsort:
    def __init__(self, array):
        self.array = array
        self.heap = []
        for n in array:
            self.__push(n)

    def __push(self, n): 
        idx = len(self.heap)
        self.heap.append(n)
        while True:
            parent = (idx - 1) // 2
            if parent < 0:
                break
            if self.heap[parent] < self.heap[idx]:
                self.heap[parent], self.heap[idx] = self.heap[idx], self.heap[parent]
            else:
                break
            
            idx = parent
            
            
    def __pop(self):
        if len(self.heap) > 0:
            n = self.heap[0]
            self.heap[0] = self.heap[-1]
            self.heap.pop()
            idx = 0
            while True:
                left = 2 * idx + 1
                right = 2 * idx + 2
                last = idx
                if left < len(self.heap) and right < len(self.heap):
                    if self.heap[left] < self.heap[right]:
                        next_child = right
                    else:
                        next_child = left
                    if self.heap[next_child] > self.heap[idx]:
                        self.heap[next_child], self.heap[idx] = self.heap[idx], self.heap[next_child]
                        idx = next_child
                elif left < len(self.heap):
                    if self.heap[left] > self.heap[idx]:
                        self.heap[left], self.heap[idx] = self.heap[idx], self.heap[left]
                        idx = left
                elif right < len(self.heap):
                    if self.heap[right] > self.heap[idx]:
                        self.heap[right], self.heap[idx] = self.heap[idx], self.heap[right]
                        idx = right
                else:
                    break
                if last == idx:
                    break
          
            return n
        return None

    def sort(self): #O(n*log(n))
        for i in range(len(self.array)):
            self.array[i] = self.__pop()
            

hs = heapsort([random.randint(0, 100) for _ in range(100)])
hs.sort()
print(hs.array)
