import numpy as np

def f(x, y):

    return (x**2 + y - 11)**2 + (x + y**2 - 7)**2

def exploratory_move(base_point: np.array, delta: np.array):
   
    x = base_point.copy()
    n_vars = base_point.shape[0]
    for i in range(n_vars):
        f_current = f(*x)
        x_plus = x.copy()
        x_plus[i] += delta[i]
        f_plus = f(*x_plus)
        x_minus = x.copy()
        x_minus[i] -= delta[i]
        f_minus = f(*x_minus)
        
        f_min = min(f_current, f_plus, f_minus)
        
        if f_min == f_plus:
            x = x_plus
        elif f_min == f_minus:
            x = x_minus

    if not np.array_equal(x, base_point):
        return True, x  
    else:
        return False, base_point 

def hooke_jeeves(start_point: list, initial_delta: list, alpha: float, eps: float):

    xk = np.array(start_point, dtype=float)
    delta = np.array(initial_delta, dtype=float)
    x_previous = xk.copy()
    
    history = [xk.copy()]
    k = 0
    
    while True:
        success, new_x = exploratory_move(xk, delta)
        
        if success:
            x_previous = xk.copy()
            xk = new_x.copy()
            history.append(xk.copy())
            
            xp = xk + (xk - x_previous)
            
            _, new_x_from_pattern = exploratory_move(xp, delta)
            
            if f(*new_x_from_pattern) < f(*xk):
                xk = new_x_from_pattern.copy()
                history.append(xk.copy())
            
        else:
            if np.linalg.norm(delta) < eps:
                print(f"\nConvergence reached at iteration {k}.")
                return xk, f(*xk), history
            else:
                delta = delta / alpha
        
        k += 1
        print(f"Iteration {k}: xk = {xk}, f(xk) = {f(*xk):.4f}, ||Δ|| = {np.linalg.norm(delta):.4f}")

initial_point = [0.0, 0.0]
initial_deltas = [0.5, 0.5]
alpha_factor = 2.0 
epsilon = 1e-4      

optimal_point, optimal_value, point_history = hooke_jeeves(
    start_point=initial_point,
    initial_delta=initial_deltas,
    alpha=alpha_factor,
    eps=epsilon
)

print("\n--- Final Results ---")
print(f"Optimal point found: {optimal_point}")
print(f"Optimal function value: {optimal_value}")
