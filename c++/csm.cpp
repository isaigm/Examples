#include <iostream>
#include <cmath>
double f(double x)
{
    return x * x + 54.0f / x;
}
double f_prime(double x, double delta)
{
    return (f(x + delta) - f(x - delta)) / (2 * delta);
}
double compute_x_test(double x1, double x2, double delta)
{
    double f1 = f(x1);
    double f2 = f(x2);
    double f1_prime = f_prime(x1, delta);
    double f2_prime = f_prime(x2, delta);
    double z = 3 * (f1 - f2) / (x2 - x1) + f1_prime + f2_prime;
    double w = (x2 - x1) / std::abs(x2 - x1) * std::sqrt(z * z - f1_prime * f2_prime);
    double mu = (f2_prime + w - z) / (f2_prime - f1_prime + 2 * w);
    if (mu < 0) return x2;
    if (mu >= 0 && mu <= 1) return x2 - mu * (x2 - x1);
    return x1;
}
void csm(double starting_point, double eps1, double eps2, double delta, double delta_prime)
{
    double xk_1 = starting_point;
    int iter = 0;
    if (f_prime(xk_1, delta_prime) > 0)
    {
        delta *= -1;
    }
    int k = 0;
    double xk = starting_point;
    double f_prime_k{}, f_prime_k_1{};
    std::cout << "----------------------\n";
    std::cout << "PASO 2\n";
    do
    {
        xk_1 = xk;
        xk = xk_1 + std::pow(2, k) * delta;
        f_prime_k = f_prime(xk, delta_prime);
        f_prime_k_1 = f_prime(xk_1, delta_prime);
        std::printf("xk_1=%f x_k=%f f'(xk_1)=%f f'(xk)=%f\n", xk_1, xk, f_prime_k_1, f_prime_k);
       
        k++;
        iter++;
    } while (f_prime_k * f_prime_k_1 > 0);
    double x1 = xk_1;
    double x2 = xk;
    double x_test = compute_x_test(x1, x2, delta_prime);
    std::cout << "----------------------\n";
    std::cout << "PASO 5\n";
    while (f(x_test) > f(x1))
    {
        std::printf("x_test=%f x1=%f f(x_test)=%f f(x1)=%f\n", x_test, x1, f(x_test), f(x1));
        x_test = x_test - 0.5 * (x_test - x1);
        iter++;
    }
    std::cout << "----------------------\n";
    std::cout << "PASO 6\n";
    while (std::abs(f_prime(x_test, delta_prime)) > eps1 && std::abs((x_test - x1) / x_test) > eps2)
    {
        if (f_prime(x_test, delta_prime) * f_prime(x1, delta_prime) < 0)
        {
            x2 = x_test;
        }
        else
        {
            x1 = x_test;
        }
        x_test = compute_x_test(x1, x2, delta_prime);
        std::printf("x1=%f x2=%f x_test=%f f'(x_test)=%f\n", x1, x2, x_test, f_prime(x_test, delta_prime));

        iter++;
    }
  
}
int main()
{
    csm(1, 0.001, 0.001, 0.5, 0.0001);

	return 0;
}
