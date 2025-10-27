import numpy as np

def f(x, y):
    return (x**2 + y - 11)**2 + (x + y**2 - 7)**2

def create_initial_simplex(x0, alpha, N):
    x0 = np.array(x0, dtype=float)
    sqrt_N_plus_1 = np.sqrt(N + 1)
    delta1 = ((sqrt_N_plus_1 + N - 1) / (N * np.sqrt(2))) * alpha
    delta2 = (((sqrt_N_plus_1 - 1) / (N * np.sqrt(2)))) * alpha
    vertices = [x0]
    for i in range(N):
        xi = np.copy(x0)
        for j in range(N):
            if i == j:
                xi[j] += delta1
            else:
                xi[j] += delta2
        vertices.append(xi)
    return vertices

def simplex_search_method(f, initial_simplex, gamma, beta, eps, max_iter=1000):
    xs = [np.array(v) for v in initial_simplex]
    N = len(xs[0])
    
    for iteration in range(max_iter):
        xs.sort(key=lambda x: f(*x))
        xl, xg, xh = xs[0], xs[1], xs[-1]
        xc = np.sum(xs[:-1], axis=0) / N

        xr = 2 * xc - xh
        xnew = xr

        if f(*xr) < f(*xl):
            xnew = (1 + gamma) * xc - gamma * xh
        elif f(*xr) >= f(*xh):
            xnew = (1 - beta) * xc + beta * xh
        elif f(*xg) < f(*xr) < f(*xh):
            xnew = (1 + beta) * xc - beta * xh
        
        xs[-1] = xnew

        f_values = np.array([f(*x) for x in xs])
        f_centroid = f(*xc)
        print(f"xl = {xl} xg = {xg} xh = {xh} xnew = {xnew} f(xh) = {f(*xh):.3f}")
        Q = np.sqrt(np.sum((f_values - f_centroid)**2) / (N + 1))
        
        if Q <= eps:
            print(f"Convergencia alcanzada despues de {iteration + 1} iteraciones.")
            break
    else:
        print(f"Maximas iteraciones  ({max_iter}) alcanzadas sin convergencia.")
        
    xs.sort(key=lambda x: f(*x))
    return xs[0], f(*xs[0])

if __name__ == "__main__":
    N = 2                
    x0 = [0.0, 0.0]       
    alpha = 1.0
    gamma = 1.5           
    beta = 0.5            
    epsilon = 0.0001      

    initial_simplex = create_initial_simplex(x0, alpha, N)
    print("Simplex generado")
    for i, vertex in enumerate(initial_simplex):
        print(f"  Vertice {i+1}: {vertex}")
    print("-" * 30)

    best_point, min_value = simplex_search_method(f, initial_simplex, gamma, beta, epsilon)

    print("-" * 30)
    print(f"Optimo encontrado: {best_point}")
    print(f"Valor minimo: {min_value}")

    
