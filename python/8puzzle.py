class Nodo:
    def __init__(self, mat, h, nivel, fz, cz, padre):
        self.nivel = nivel
        self.f_n = h + nivel
        self.mat = mat
        self.fz = fz
        self.cz = cz
        self.padre = padre
def main():
    n = 3
    estado_inicial = [[1, 8, 2], [0, 4, 3], [7, 6, 5]]
    objetivo = [[1, 2, 3], [4, 5, 6], [7, 8, 0]]
    fz = 1
    cz = 0
    def imp(tablero):
        for fila in tablero:
            for col in fila:
                print(col, end = ' ')
            print()
        print()
    def sonIguales(actual, obj):
        for i in range(0, n):
            for j in range(0, n):
                if actual[i][j] != obj[i][j]:
                    return False
        return True
    def distanciaManhattan(actual, obj):
        suma = 0
        for i in range(0, n):
            for j in range(0, n):
                if actual[i][j] != obj[i][j]:
                    i2, j2 = 0, 0
                    while i2 < n and (actual[i][j] != obj[i2][j2]):
                        j2 += 1
                        if j2 >= n:
                            i2 += 1
                            j2 = 0
                    suma += abs(i2 - i) + abs(j2 - j)
        return suma
    def expandir(nodo_padre, fz, cz):
        hijos = []
        copiar = lambda matriz : [x[:] for x in matriz]
        if fz + 1 < n:
            mat = copiar(nodo_padre.mat)
            mat[fz][cz] = mat[fz+1][cz]
            mat[fz+1][cz] = 0
            hijos.append(Nodo(mat, distanciaManhattan(mat, objetivo), nodo_padre.nivel+1, fz+1, cz, nodo_padre))
        if fz - 1 >= 0:
            mat = copiar(nodo_padre.mat)
            mat[fz][cz] = mat[fz-1][cz]
            mat[fz-1][cz] = 0
            hijos.append(Nodo(mat, distanciaManhattan(mat, objetivo), nodo_padre.nivel+1, fz-1, cz, nodo_padre))
        if cz + 1 < n:
            mat = copiar(nodo_padre.mat)
            mat[fz][cz] = mat[fz][cz+1]
            mat[fz][cz+1] = 0
            hijos.append(Nodo(mat, distanciaManhattan(mat, objetivo), nodo_padre.nivel+1, fz, cz+1, nodo_padre))
        if cz - 1 >= 0:
            mat = copiar(nodo_padre.mat)
            mat[fz][cz] = mat[fz][cz-1]
            mat[fz][cz-1] = 0
            hijos.append(Nodo(mat, distanciaManhattan(mat, objetivo), nodo_padre.nivel+1, fz, cz-1, nodo_padre))
        return hijos
    def elegir(nodos):
        index = 0
        elegido = nodos[index]
        for i in range(1, len(nodos)):
            if elegido.f_n > nodos[i].f_n:
                elegido = nodos[i]
                index = i
        return nodos[index]
    def esta(nodos, nodo):
        for n in nodos:
            if sonIguales(n.mat, nodo.mat):
                return True
        return False
    def eliminarCoincidencias(nodos, nodo):
        nueva = []
        for n in nodos:
            if not sonIguales(n.mat, nodo.mat):
                nueva.append(n)
        del nodos
        return nueva
    def buscar(nodos, nodo):
        for n in nodos:
            if sonIguales(n.mat, nodo.mat):
                return n
        return None
    def recorrer(nodo):
        if nodo != None:
            recorrer(nodo.padre)
            imp(nodo.mat)
    nodo_inicial = Nodo(estado_inicial, distanciaManhattan(estado_inicial, objetivo), 0, fz, cz, None)
    open_list = []
    closed_list = []
    open_list.append(nodo_inicial)
    while len(open_list) > 0:
        elegido = open_list[0]
        closed_list.append(elegido)
        if sonIguales(elegido.mat, objetivo):
            print("Solucionado")
            break
        hijos = expandir(elegido, elegido.fz, elegido.cz)
        for h in hijos:
           open_list.append(h)
        del open_list[0]
        open_list.sort(key = lambda x : x.f_n, reverse = False)
    recorrer(elegido)
if __name__ == "__main__":
    main()
