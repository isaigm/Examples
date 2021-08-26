#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
//matrix es la matriz de adyacencia
int * Dijkstra(int **matrix, int n_vertex, int origin, int dest){
    int *distances = new int [n_vertex]; //menores distancias desde el nodo origin hacia el i-esimo nodo
    bool* visited = new bool [n_vertex]; //nodos yo visitados
    int *parents = new int[n_vertex]; //para el i-esimo nodo se tiene el padre de este con parents[i], esto servira para construir la ruta desde un nodo origen hacia un nodo destino
    for(int j = 0; j < n_vertex; j++){
        visited[j] = false;
    }
    for(int i = 0; i < n_vertex; i++){
        if(matrix[origin][i] != -1){ //el -1 significa que hacia el nodo i no se puede ir desde origin, basicamente los nodos que no cumplen eso son los vecinos del nodo origin
            distances[i] = matrix[origin][i];
            parents[i] = origin;
        }else distances[i] = int(pow(2, 8*sizeof(int)) - 1); //distancia infinita hacia los nodos que no son los vecinos
    }
    visited[origin] = true;
    parents[origin] = -1;
    int current = origin;
    int nvisited = 1;
    while(nvisited < n_vertex){
        int minDistance = int(pow(2, 8*sizeof(int)) - 1);
        //se busca el nodo con la menor distancia que no haya sido visitado
        for(int i = 0; i < n_vertex; i++){
            if(distances[i] < minDistance && !visited[i]){
                minDistance = distances[i];
                current = i;
            }
        }
        //se marca como visitado
        visited[current] = true;
        //con este bucle se analizan los vecinos del nodo current
        for(int i = 0; i < n_vertex; i++){
            //primero se comprueba que se pueda acceder al nodo i y que ese nodo no sea el mismo que el nodo current
            if(matrix[current][i] != 0 && matrix[current][i] != -1){
                //se comprueba que la distancia hasta el momento hacia el nodo i sea mayor que la distancia desde el nodo actual junto con la distancia desde el nodo current hasta el nodo i
                if(distances[i] > matrix[current][i] + distances[current]){
                    distances[i] = matrix[current][i] + distances[current]; //si se cumple esto, entonces se ha encontrado una distancia hacia el nodo i mejor
                    parents[i] = current; //por lo tanto se actualiza el padre del nodo i, significa que por el nodo current se obtiene una mejor distancia o ruta hacia el nodo i
                }
            }
        }
        nvisited++;
    }
    std::vector<int> path;
    current = dest;
    while(current != -1) //con este bucle se tendra la ruta optima de origin a dest, pero en orden inverso, por lo tanto hay que revertir path
    {
        path.push_back(current);
        current = parents[current];
    }
    std::reverse(path.begin(), path.end());
    for(const auto &x: path)
    {
        std::cout << x << "\n";
    }
    std::cout << "fin ruta\n";
    delete [] visited;
    delete [] parents;
    return distances;
}
/*
ejemplo de entrada
6
0 100 5 -1 -1 -1
-1 0 -1 -1 -1 -1
-1 -1 0 1 -1 -1
-1 -1 -1 0 50 10
-1 1 -1 -1 0 -1 
-1 10 -1 -1 -1 0    

*/
int main(){
    int n_vertex;
    std::cin >> n_vertex;
    int **matrix;
    matrix = new int *[n_vertex];
    for(int i = 0; i < n_vertex; i++){
        matrix[i] = new int[n_vertex];
    }
    for(int i = 0; i < n_vertex; i++){
        for(int j = 0; j < n_vertex; j++){
            int temp;
            std::cin >> temp;
            matrix[i][j] = temp;
        }
    }
    int *result = Dijkstra(matrix, n_vertex, 0, 2);
    for(int i = 0; i < n_vertex; i++){
        std::cout << result[i] << std::endl;
    }
    return 0;
}
