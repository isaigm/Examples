#include <iostream>
#include <cmath>
int * Dijkstra(int **matrix, int n_vertex, int origin){
    int *distances = new int [n_vertex];
    bool* visited = new bool [n_vertex];
    for(int j = 0; j < n_vertex; j++){
        visited[j] = false;
    }
    for(int i = 0; i < n_vertex; i++){
        if(matrix[origin][i] != -1){
            distances[i] = matrix[origin][i];
        }else distances[i] = int(pow(2, 8*sizeof(int)) - 1);
    }
    visited[origin] = true;
    int current = origin;
    int nvisited = 1;
    while(nvisited < n_vertex){
        int minDistance = int(pow(2, 8*sizeof(int)) - 1);
        for(int i = 0; i < n_vertex; i++){
            if(distances[i] < minDistance && !visited[i]){
                minDistance = distances[i];
                current = i;
            }
        }
        visited[current] = true;
        for(int i = 0; i < n_vertex; i++){
            if(matrix[current][i] != 0 && matrix[current][i] != -1){
                if(distances[i] > matrix[current][i] + distances[current]){
                    distances[i] = matrix[current][i] + distances[current];
                }
            }
        }
        nvisited++;
    }
    delete [] visited;
    return distances;
}
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
    int *result = Dijkstra(matrix, n_vertex, 0);
    for(int i = 0; i < n_vertex; i++){
        std::cout << result[i] << std::endl;
    }
    return 0;
}
