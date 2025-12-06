import numpy as np
import time


def objective(x, y):
    """Función objetivo de Himmelblau."""
    return (x**2 + y - 11)**2 + (x + y**2 - 7)**2

def gradient(f, point, delta=1e-5):
    """Calcula el gradiente."""
    n = len(point)
    grad = np.zeros(n)
    for i in range(n):
        point_plus = point.copy()
        point_minus = point.copy()
        point_plus[i] += delta
        point_minus[i] -= delta
        grad[i] = (f(*point_plus) - f(*point_minus)) / (2 * delta)
    return grad

def hessian_matrix(f, point, delta=1e-5):
    """Calcula la matriz Hessiana numérica."""
    n = len(point)
    hess = np.zeros((n, n))
    for j in range(n):
        point_plus = point.copy()
        point_plus[j] += delta
        point_minus = point.copy()
        point_minus[j] -= delta
        grad_plus = gradient(f, point_plus, delta)
        grad_minus = gradient(f, point_minus, delta)
        hess[:, j] = (grad_plus - grad_minus) / (2 * delta)
    return hess


def marquardt_method(start_point, epsilon, M):
  
    # Paso 1: Inicialización
    # Elegir punto inicial, M (max iteraciones) y epsilon.
    x_k = np.array(start_point, dtype=float)
    n = len(x_k)
    I = np.eye(n) # Matriz identidad
    
    # Hacer k = 0 y lambda = 10^4 (un valor grande)
    k = 0
    lam = 10000.0 
    
    print(f"{'Iter (k)':<10} {'Punto Actual':<25} {'f(x)':<12} {'||grad||':<12} {'Lambda':<10}")
    print("-" * 75)

    while True:
        # Paso 2: Calcular Gradiente
        grad_k = gradient(objective, x_k)
        norm_grad = np.linalg.norm(grad_k)
        f_k = objective(*x_k)
        
        # Imprimir estado actual
        print(f"{k:<10} {str(np.round(x_k, 4)):<25} {f_k:<12.4f} {norm_grad:<12.4f} {lam:<10.5g}")

        # Paso 3: Criterio de Terminación
        # IF ||grad|| <= epsilon OR k >= M THEN Terminar
        if norm_grad <= epsilon:
            print(f"\nTerminación: Norma del gradiente ({norm_grad:.6f}) <= epsilon")
            break
        if k >= M:
            print(f"\nTerminación: Se alcanzó el número máximo de iteraciones M={M}")
            break

        # Calcular Hessiana (necesaria para el Paso 4)
        H_k = hessian_matrix(objective, x_k)
        
        # Bucle interno para ajustar Lambda (Manejo del GOTO Paso 4 y Paso 7)
        step_accepted = False
        
        while not step_accepted:
            # Paso 4: Calcular dirección s y nuevo punto
            # s = - [H + lambda * I]^-1 * grad
            # x(k+1) = x(k) + s
            
            # Matriz a invertir: (H + lambda * I)
            matrix_A = H_k + lam * I
            
            try:
                # Resolvemos el sistema lineal A * s = -grad
                s = np.linalg.solve(matrix_A, -grad_k)
            except np.linalg.LinAlgError:
                print("Error: Matriz singular, aumentando lambda forzosamente.")
                lam *= 2
                continue

            x_new = x_k + s
            f_new = objective(*x_new)
            
            # Paso 5: Evaluación de descenso
            # ¿Es f(x_new) < f(x_k)?
            if f_new < f_k:
                # SI ES ASÍ: GOTO Paso 6
                # Paso 6: Reducir lambda, actualizar k y volver al Paso 2
                lam = 0.5 * lam
                x_k = x_new
                k += 1
                step_accepted = True # Sale del bucle interno, vuelve al `while True` principal (Paso 2)
            else:
                # ELSE: GOTO Paso 7
                # Paso 7: Aumentar lambda y GOTO Paso 4 (repetir cálculo de s)
                lam = 2 * lam
                # El bucle `while not step_accepted` repite el proceso (Paso 4) con la nueva lambda

    return x_k


if __name__ == "__main__":
    start_point = np.array([0.0, 0.0], dtype='float')
    M = 100          # Número máximo de iteraciones
    epsilon = 0.001   # Parámetro de terminación

    print("--- Ejecución del Método de Marquardt ---")
    start_time = time.time()
    
    minimum_point = marquardt_method(start_point, epsilon, M)
    
    end_time = time.time()
    
    print("\n--- Resultado Final ---")
    print(f"Punto óptimo encontrado: {minimum_point}")
    print(f"Valor de la función: {objective(*minimum_point)}")
    print(f"Tiempo de ejecución: {end_time - start_time:.4f} segundos")
