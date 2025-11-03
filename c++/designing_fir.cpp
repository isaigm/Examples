#include <iostream>
#include <cmath>
#include <vector>
#include <Eigen/Dense>
#include <fftw3.h> // La biblioteca clave para la FFT
#include "gwo.hpp" // Tu implementación de GWO

// --- Definición del Problema del Filtro FIR ---

struct FIRFilterProblem : public GWO::Problem<double>
{
private:
    // Parámetros del diseño del filtro
    int filterOrder;
    int fftSize;
    double samplingFreq;
    double passbandFreq;
    double stopbandFreq;

    // Ponderaciones para el error
    double passbandWeight;
    double stopbandWeight;

    // Datos precalculados para la eficiencia
    Eigen::ArrayXd idealResponse;
    Eigen::ArrayXd weights;

    // Plan y buffers de FFTW. Deben ser 'mutable' porque la función fitness() es 'const',
    // pero necesitamos modificar los buffers para realizar el cálculo de la FFT.
    // Esto no cambia el estado lógico del problema, solo son buffers de trabajo.
    mutable double* fft_in;
    mutable fftw_complex* fft_out;
    mutable fftw_plan plan;

public:
    // Constructor: Inicializa los parámetros y precalcula todo lo posible
    FIRFilterProblem(GWO::Setup setup, int order, int fft_points, double fs, double fp, double fsb)
        : GWO::Problem<double>(setup),
        filterOrder(order),
        fftSize(fft_points),
        samplingFreq(fs),
        passbandFreq(fp),
        stopbandFreq(fsb),
        passbandWeight(1.0),
        stopbandWeight(10.0)
    {
        // 1. Asignar memoria alineada para FFTW
        fft_in = (double*)fftw_malloc(sizeof(double) * fftSize);
        fft_out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * (fftSize / 2 + 1));

        // 2. Crear el plan de FFTW una sola vez
        plan = fftw_plan_dft_r2c_1d(fftSize, fft_in, fft_out, FFTW_ESTIMATE);

        // 3. Precalcular la respuesta ideal y las ponderaciones
        idealResponse.resize(fftSize / 2 + 1);
        weights.resize(fftSize / 2 + 1);

        int passband_bin = static_cast<int>(passbandFreq / (samplingFreq / 2.0) * (fftSize / 2));
        int stopband_bin = static_cast<int>(stopbandFreq / (samplingFreq / 2.0) * (fftSize / 2));

        for (int i = 0; i <= fftSize / 2; ++i) {
            if (i <= passband_bin) {
                idealResponse[i] = 1.0; // Banda de paso
                weights[i] = passbandWeight;
            }
            else if (i >= stopband_bin) {
                idealResponse[i] = 0.0; // Banda de rechazo
                weights[i] = stopbandWeight;
            }
            else {
                idealResponse[i] = 0.0; // Banda de transición (no se evalúa el error)
                weights[i] = 0.0;
            }
        }
    }

    // Destructor: Libera la memoria de FFTW
    ~FIRFilterProblem() {
        fftw_destroy_plan(plan);
        fftw_free(fft_in);
        fftw_free(fft_out);
    }

    // La función de coste: el corazón del problema.
    // Debe ser 'const override' para coincidir con la clase base.
    // En la clase FIRFilterProblem, reemplaza la función fitness existente con esta.

    // En la clase FIRFilterProblem, esta es la versión final y corregida de la función.

    // Esta es la versión final y corregida de la función fitness.

    double fitness(const Eigen::ArrayX<double>& pos) const override
    {
        // --------------------------------------------------------------------
        // Parte 1: Reconstrucción del filtro y cálculo de la FFT (sin cambios)
        // --------------------------------------------------------------------
        Eigen::ArrayXd full_coeffs(filterOrder + 1);
        for (int i = 0; i < pos.size(); ++i) {
            full_coeffs[i] = pos[i];
            if (i != filterOrder - i) {
                full_coeffs[filterOrder - i] = pos[i];
            }
        }

        for (int i = 0; i < fftSize; ++i) {
            fft_in[i] = (i < full_coeffs.size()) ? full_coeffs[i] : 0.0;
        }
        fftw_execute(plan);

        // --------------------------------------------------------------------
        // Parte 2: Cálculo del error con lógica de comparación corregida
        // --------------------------------------------------------------------
        double total_error = 0.0;

        double dc_real = fft_out[0][0];
        double dc_magnitude = std::abs(dc_real);
        if (dc_magnitude < 1e-9) {
            dc_magnitude = 1.0;
        }

        const double STOPBAND_THRESHOLD_DB = -80.0;
        const double stopband_threshold_linear = pow(10.0, STOPBAND_THRESHOLD_DB / 20.0);

        const double stopband_weight_penalty = 100.0;

        int passband_bin_end = static_cast<int>(passbandFreq / (samplingFreq / 2.0) * (fftSize / 2));
        int stopband_bin_start = static_cast<int>(stopbandFreq / (samplingFreq / 2.0) * (fftSize / 2));

        for (int i = 0; i <= fftSize / 2; ++i) {
            double real = fft_out[i][0];
            double imag = fft_out[i][1];
            double magnitude = std::sqrt(real * real + imag * imag);

            double error = 0.0;

            // --- Lógica para la Banda de Paso ---
            if (i <= passband_bin_end) {
                // El error es la diferencia con la ganancia de referencia (dc_magnitude)
                error = magnitude - dc_magnitude;
                total_error += passbandWeight * (error * error);
            }
            // --- Lógica para la Banda de Rechazo (CORREGIDA) ---
            else if (i >= stopband_bin_start) {
                // --- ¡CAMBIO CLAVE Y FINAL! ---
                // Calculamos el umbral de magnitud absoluto para ESTE filtro.
                double target_stopband_magnitude = dc_magnitude * stopband_threshold_linear;

                // Comparamos la magnitud absoluta con el umbral absoluto.
                if (magnitude > target_stopband_magnitude) {
                    // La violación se calcula en el dominio lineal absoluto.
                    error = magnitude - target_stopband_magnitude;
                    total_error += stopband_weight_penalty * (error * error);
                }
                // Si está por debajo, el error es 0.
            }
        }

        return total_error;
    }
};


int main()
{
    // --- 1. Definir las especificaciones del filtro paso bajo ---
    const int FILTER_ORDER = 40;
    const int FFT_SIZE = 2048;
    const double SAMPLING_FREQ = 48000.0;
    const double PASSBAND_FREQ = 4000.0;
    const double STOPBAND_FREQ = 6000.0;

    // --- 2. Configurar GWO ---
    const int num_vars_to_optimize = (FILTER_ORDER / 2) + 1;

    GWO::Setup setup{
        .N = (size_t)num_vars_to_optimize,
        .POP_SIZE = 200,
        .maxRange = Eigen::ArrayXd::Constant(num_vars_to_optimize, 0.5),
        .minRange = Eigen::ArrayXd::Constant(num_vars_to_optimize, -0.5)
    };

    // --- 3. Crear y ejecutar el problema de optimización ---
    FIRFilterProblem problem(setup, FILTER_ORDER, FFT_SIZE, SAMPLING_FREQ, PASSBAND_FREQ, STOPBAND_FREQ);

    std::cout << "Optimizando coeficientes de filtro FIR con GWO...\n";
    // El método run devuelve un objeto Wolf<double>
    auto wolf_alfa = problem.run(3000);

    // --- 4. Mostrar los resultados ---
    std::cout << "\n------------------------------------------\n";
    std::cout << "Diseño de Filtro FIR Paso Bajo Completado\n";
    std::cout << "------------------------------------------\n";
    std::cout << "Mejor fitness (error) encontrado: " << wolf_alfa.savedFitness << "\n";

    // CORREGIDO: Acceder a la posición con 'wolf_alfa.pos'
    Eigen::ArrayXd optimal_half = wolf_alfa.pos;
    Eigen::ArrayXd final_coeffs(FILTER_ORDER + 1);

    // Lógica de reconstrucción simplificada y corregida
    for (int i = 0; i < optimal_half.size(); ++i) {
        final_coeffs[i] = optimal_half[i];
        if (i != FILTER_ORDER - i) {
            final_coeffs[FILTER_ORDER - i] = optimal_half[i];
        }
    }

    // Normalizar los coeficientes finales para que la ganancia en la banda de paso sea 1
    final_coeffs /= final_coeffs.sum();

    std::cout << "Coeficientes del filtro FIR optimizados (" << final_coeffs.size() << " en total):\n";
    for (int i = 0; i < final_coeffs.size(); ++i) {
        std::cout << final_coeffs[i] << (i == final_coeffs.size() - 1 ? "" : ", ");
    }
    std::cout << "\n------------------------------------------\n";

    return 0;
}
