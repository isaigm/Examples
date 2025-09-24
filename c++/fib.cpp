#include <iostream>
#include <vector>
#include <cmath>
#include <functional> // Para std::function
#include <utility>    // Para std::pair
#include <stdexcept>  // Para std::invalid_argument
#include <iomanip>    // Para std::setprecision

// --- Funciones Objetivo para Minimizar ---

// Función original (potencial de Lennard-Jones), no usada en main pero conservada como ejemplo.
double lennardJonesPotential(double x) {
    if (x == 0) return std::numeric_limits<double>::infinity();
    double term6 = std::pow(1.0 / x, 6);
    double term12 = term6 * term6; // Más eficiente que pow(term6, 2)
    return term12 - 2.0 * term6;
}

// La función que se está minimizando en el ejemplo.
double objectiveFunction(double x) {
    if (x == 0) return std::numeric_limits<double>::infinity();
    return x * x + 54.0 / x;
}

// --- Espacio de Nombres para el Algoritmo de Optimización ---

namespace Optimization {

    // Alias para mayor legibilidad. Representa una función de una variable que devuelve un double.
    using Function = std::function<double(double)>;

    /**
     * @brief Genera los primeros 'count' números de la secuencia de Fibonacci.
     * @param count El número de elementos de Fibonacci a generar.
     * @return Un vector de 'long long' con la secuencia.
     */
    std::vector<long long> generateFibonacciSequence(int count) {
        if (count <= 0) return {};
        if (count == 1) return { 1 };

        std::vector<long long> fib = { 1, 1 };
        fib.reserve(count); // Pre-reserva memoria para eficiencia

        while (fib.size() < count) {
            fib.push_back(fib.back() + fib[fib.size() - 2]);
        }
        return fib;
    }

    /**
     * @brief Encuentra el mínimo de una función unimodal usando el método de búsqueda de Fibonacci.
     *
     * @param func La función objetivo a minimizar.
     * @param a El límite inferior del intervalo de búsqueda.
     * @param b El límite superior del intervalo de búsqueda.
     * @param n El número de iteraciones (evaluaciones de la función) a realizar.
     * @param verbose Si es true, imprime los pasos intermedios de la búsqueda.
     * @return Un std::pair<double, double> representando el intervalo final que contiene el mínimo.
     */
    std::pair<double, double> fibonacciSearch(const Function& func, double a, double b, int n, bool verbose = false) {
        if (a >= b) {
            throw std::invalid_argument("El intervalo es inválido: a debe ser menor que b.");
        }
        if (n <= 1) {
            throw std::invalid_argument("El número de iteraciones n debe ser mayor que 1.");
        }

        // Generamos n+1 números de Fibonacci para realizar n-1 reducciones de intervalo.
        const auto fib = generateFibonacciSequence(n + 1);

        // Calculamos los dos primeros puntos de prueba.
        double length = b - a;
        double x1 = a + (static_cast<double>(fib[n - 2]) / fib[n]) * length;
        double x2 = a + (static_cast<double>(fib[n - 1]) / fib[n]) * length;

        double f1 = func(x1);
        double f2 = func(x2);

        if (verbose) {
            std::cout << "Inicio: a=" << a << ", b=" << b << std::endl;
        }

        // Bucle principal para reducir el intervalo.
        for (int k = 1; k < n - 1; ++k) {
            if (verbose) {
                std::cout << "Iter " << k << ": a=" << a << ", x1=" << x1
                    << ", x2=" << x2 << ", b=" << b << std::endl;
            }

            if (f1 < f2) {
                // El mínimo está en [a, x2], descartamos [x2, b]
                b = x2;
                x2 = x1;
                f2 = f1;
                length = b - a;
                x1 = a + (static_cast<double>(fib[n - k - 2]) / fib[n - k]) * length;
                f1 = func(x1);
            }
            else {
                // El mínimo está en [x1, b], descartamos [a, x1]
                a = x1;
                x1 = x2;
                f1 = f2;
                length = b - a;
                x2 = b - (static_cast<double>(fib[n - k - 2]) / fib[n - k]) * length;
                f2 = func(x2);
            }
        }

        // La última iteración se maneja con cuidado para evitar división por cero si F_0 se usara.
        // Comparamos los dos últimos puntos para obtener el intervalo final más pequeño.
        if (f1 < f2) {
            b = x2;
        }
        else {
            a = x1;
        }

        return { a, b };
    }

    /**
     * @brief Sobrecarga de fibonacciSearch que determina el número de iteraciones a partir de una tolerancia.
     *
     * @param func La función objetivo a minimizar.
     * @param a El límite inferior del intervalo de búsqueda.
     * @param b El límite superior del intervalo de búsqueda.
     * @param tolerance La longitud deseada del intervalo final.
     * @param verbose Si es true, imprime los pasos intermedios de la búsqueda.
     * @return Un std::pair<double, double> representando el intervalo final.
     */
    std::pair<double, double> fibonacciSearch(const Function& func, double a, double b, double tolerance, bool verbose = false) {
        if (tolerance <= 0) {
            throw std::invalid_argument("La tolerancia debe ser positiva.");
        }

        const double required_ratio = (b - a) / tolerance;
        int n = 2; // Mínimo número de iteraciones
        long long f_n_plus_1 = 2; // F_3

        // Encontramos el número de iteraciones 'n' tal que F_{n+1} > (b-a)/tolerance
        std::vector<long long> fib = { 1, 1 };
        while (fib.back() <= required_ratio) {
            fib.push_back(fib.back() + fib[fib.size() - 2]);
            n++;
        }

        if (verbose) {
            std::cout << "Tolerancia=" << tolerance << " requiere " << n << " iteraciones." << std::endl;
        }

        return fibonacciSearch(func, a, b, n, verbose);
    }

} // namespace Optimization

int main() {
    // --- Configuración del Problema ---
    // NOTA: El intervalo original [0, 5] es inválido para objectiveFunction
    // porque causa una división por cero en x=0.
    // Usamos un intervalo seguro que no incluye el 0.
    const double a = 0.1;
    const double b = 5.0;
    const int iterations = 20;
    const double tolerance = 1e-6;

    // Configura la precisión de salida para std::cout
    std::cout << std::fixed << std::setprecision(8);

    try {
        // --- Ejemplo 1: Búsqueda con un número fijo de iteraciones ---
        std::cout << "--- Búsqueda de Fibonacci con " << iterations << " iteraciones ---" << std::endl;
        auto result_n = Optimization::fibonacciSearch(lennardJonesPotential, a, b, iterations, true);
        double min_x_n = (result_n.first + result_n.second) / 2.0;

        std::cout << "\nIntervalo final: [" << result_n.first << ", " << result_n.second << "]" << std::endl;
        std::cout << "Ancho del intervalo: " << (result_n.second - result_n.first) << std::endl;
        std::cout << "Mínimo estimado en x = " << min_x_n << std::endl;
        std::cout << "Valor de la función f(x) = " << lennardJonesPotential(min_x_n) << std::endl;

        std::cout << "\n------------------------------------------------------\n" << std::endl;

        // --- Ejemplo 2: Búsqueda basada en tolerancia ---
        std::cout << "--- Búsqueda de Fibonacci con tolerancia " << tolerance << " ---" << std::endl;
        auto result_tol = Optimization::fibonacciSearch(lennardJonesPotential, a, b, tolerance, true);
        double min_x_tol = (result_tol.first + result_tol.second) / 2.0;

        std::cout << "\nIntervalo final: [" << result_tol.first << ", " << result_tol.second << "]" << std::endl;
        std::cout << "Ancho del intervalo: " << (result_tol.second - result_tol.first) << std::endl;
        std::cout << "Mínimo estimado en x = " << min_x_tol << std::endl;
        std::cout << "Valor de la función f(x) = " << lennardJonesPotential(min_x_tol) << std::endl;

    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
