#include <stdio.h>
#include <time.h>
#include <assert.h>
#define N 1000
#define MAT_GPU 1 << 2
#define MAT_CPU 1 << 3

__device__ void set_element(int *matrix, int value, int cols, int x, int y)
{
    matrix[x + cols * y] = value;
}
__device__ int get_element(int *matrix, int cols, int x, int y)
{
    return matrix[x + cols * y];
}

__global__ void add(int *a, int *b, int *c, int rows, int cols)
{
    //----------                                      dimension
    int x = blockIdx.x * blockDim.x + threadIdx.x; //[0 0 0 0 0] [0 0 0 0 0] <- bloque
    int y = blockIdx.y * blockDim.y + threadIdx.y; // |-> threadIdx.x
    if (x < cols && y < rows)
    {
        int a_e = get_element(a, cols, x, y);
        int b_e = get_element(b, cols, x, y);
        set_element(c, a_e + b_e, cols, x, y);
    }
}
__global__ void mult(int *a, int *b, int *c, int n)
{
    int i = blockIdx.y * blockDim.y + threadIdx.y;
    int j = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n && j < n)
    {
        int res = 0;
        for (int k = 0; k < n; k++)
        {
            res += a[k + i * n] * b[j + k * n];
        }
        c[j + i * n] = res;
    }
}
int *allocate_matrix(int kind, int rows, int cols)
{
    int *matrix = NULL;
    if (kind == MAT_GPU)
    {
        cudaMalloc((void **)&matrix, sizeof(int) * rows * cols);
    }
    else if (kind == MAT_CPU)
    {
        matrix = (int *)malloc(sizeof(int) * rows * cols);
    }
    return matrix;
}
void random_fill_matrix(int *matrix, int rows, int cols)
{
    for (int idx = 0; idx < rows * cols; idx++)
    {
        matrix[idx] = rand() % 10;
    }
}
void cpu_mul(int *a, int *b, int *c, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int res = 0;

            for (int k = 0; k < n; k++)
            {
                res += a[k + i * n] * b[j + k * n];
            }
            c[j + i * n] = res;
        }
    }
}
void print_matrix(int *m, int rows, int cols)
{
    for (int i = 0; i < rows; i++)

    {
        for (int j = 0; j < cols; j++)

        {
            printf("%d ", m[j + i * cols]);
        }
        printf("\n");
    }
    printf("-----------------------------------\n");
}
int main()
{
    srand(time(nullptr));
    int *a_host_matrix = allocate_matrix(MAT_CPU, N, N);
    int *b_host_matrix = allocate_matrix(MAT_CPU, N, N);
    int *result_host_matrix = allocate_matrix(MAT_CPU, N, N);

    int *a_device_matrix = allocate_matrix(MAT_GPU, N, N);
    int *b_device_matrix = allocate_matrix(MAT_GPU, N, N);
    int *result_device_matrix = allocate_matrix(MAT_GPU, N, N);

    random_fill_matrix(a_host_matrix, N, N);
    random_fill_matrix(b_host_matrix, N, N);
    //print_matrix(a_host_matrix, N, N);
    //print_matrix(b_host_matrix, N, N);
    //cpu_mul(a_host_matrix, b_host_matrix, result_host_matrix, N);
    //print_matrix(result_host_matrix, N, N);
    cudaMemcpy((void *)a_device_matrix, (void *)a_host_matrix, sizeof(int) * N * N, cudaMemcpyHostToDevice);
    cudaMemcpy((void *)b_device_matrix, (void *)b_host_matrix, sizeof(int) * N * N, cudaMemcpyHostToDevice);
    dim3 bdim(int(ceil(N / 32.0f)), int(ceil(N / 32.0f))); //la estructura de los hilos es en forma de grid, donde cada celda es una matriz de 32x32
    dim3 tdim(32, 32);
    clock_t start = clock();
    mult<<<bdim, tdim>>>(a_device_matrix, b_device_matrix, result_device_matrix, N);
    cudaDeviceSynchronize();
    cudaMemcpy((void *)result_host_matrix, (void *)result_device_matrix, sizeof(int) * N * N, cudaMemcpyDeviceToHost);
    clock_t end = clock();
    printf("tiempo de ejecucion: %f\n", double(end - start) / CLOCKS_PER_SEC);
    //print_matrix(result_host_matrix, N, N);
    return 0;
}
