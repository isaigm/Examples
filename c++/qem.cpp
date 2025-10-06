#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <stdexcept> 
#include <utility>  

struct Point {
    double x;
    double y; 
};

double f(double x) {
    return x * x + 54.0 / x;
}

double quadraticEstimation(const double startPoint, const double delta, const double xeps, const double yeps) {
    double x1 = startPoint;
    double x2 = x1 + delta;

    double f1 = f(x1);
    double f2 = f(x2);

    double x3;
    if (f1 > f2) {
        x3 = x1 + 2 * delta;
    }
    else {
        x3 = x1 - delta;
    }
    double f3 = f(x3);

    int iter = 0;

    while (true) {
        std::vector<Point> current_points = { {x1, f1}, {x2, f2}, {x3, f3} };
        //Encontrando el mínimo del vector current_points
        std::sort(current_points.begin(), current_points.end(), [](const Point& a, const Point& b) {
            return a.x < b.x;
            });
        x1 = current_points[0].x; f1 = current_points[0].y;
        x2 = current_points[1].x; f2 = current_points[1].y;
        x3 = current_points[2].x; f3 = current_points[2].y;

        double fmin = f1;
        double xmin = x1;
        if (f2 < fmin) { fmin = f2; xmin = x2; }
        if (f3 < fmin) { fmin = f3; xmin = x3; }

        double a1 = (f2 - f1) / (x2 - x1);
        double a2 = (1.0 / (x3 - x2)) * (((f3 - f1) / (x3 - x1)) - a1);

        if (a2 <= 0) {
            std::cerr << "Error: a2 es no positivo (" << a2 << "). La parábola no es cóncava hacia arriba. Terminando." << std::endl;
            return xmin;
        }

        double x_bar = 0.5 * (x1 + x2) - (a1 / (2 * a2));
        double f_bar = f(x_bar);
        std::printf("x1=%f, x2=%f, x3=%f, x_bar=%f, f_bar=%f, f_1=%f, f_2 =%f, f_3=%f\n", x1, x2, x3, x_bar, f_bar, f1, f2, f3);
        std::cout << xmin << "\n";
        if (std::abs(fmin - f_bar) <= yeps && std::abs(xmin - x_bar) <= xeps) {
            std::cout << "Convergencia alcanzada en la iteracion " << iter << std::endl;
            return x_bar;
        }

        std::vector<Point> all_points = { {x1, f1}, {x2, f2}, {x3, f3}, {x_bar, f_bar} };
        //Esto se hace para poder encerrar el mejor punto si es que es posible
        std::sort(all_points.begin(), all_points.end(), [](const Point& a, const Point& b) {
            return a.x < b.x;
            });

        int min_idx = 0;
        for (int i = 1; i < 4; ++i) {
            if (all_points[i].y < all_points[min_idx].y) {
                min_idx = i;
            }
        }

        std::vector<Point> next_points;
        if (min_idx > 0 && min_idx < 3) {
            next_points.push_back(all_points[min_idx - 1]);
            next_points.push_back(all_points[min_idx]);
            next_points.push_back(all_points[min_idx + 1]);
        }
        else {
            std::sort(all_points.begin(), all_points.end(), [](const Point& a, const Point& b) {
                return a.y < b.y;
                });
            next_points.assign(all_points.begin(), all_points.begin() + 3);
        }
        x1 = next_points[0].x; f1 = next_points[0].y;
        x2 = next_points[1].x; f2 = next_points[1].y;
        x3 = next_points[2].x; f3 = next_points[2].y;

        iter++;
    }

    std::vector<Point> final_points = { {x1, f1}, {x2, f2}, {x3, f3} };
    std::sort(final_points.begin(), final_points.end(), [](const Point& a, const Point& b) { return a.y < b.y; });
    return final_points[0].x;
}

int main() {
    double startPoint = 1;
    double delta = 0.1;
    double xeps = 0.001;
    double yeps = 0.001;

    try {
        double minimum_x = quadraticEstimation(startPoint, delta, xeps, yeps);
        std::cout << "El punto minimo encontrado es x = " << minimum_x << std::endl;
        std::cout << "El valor de la funcion en ese punto es f(x) = " << f(minimum_x) << std::endl;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Una excepcion ocurrio: " << e.what() << std::endl;
    }

    // El mínimo analítico de f(x) = x^2 + 54/x es en x = 3, donde f(3) = 27.
    // Esto sirve para verificar nuestro resultado.
    std::cout << "\nVerificacion analitica:" << std::endl;
    std::cout << "Minimo teorico en x=3, f(3) = " << 3 * 3 + 54.0 / 3 << std::endl;


    return 0;
}
