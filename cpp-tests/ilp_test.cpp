#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <iomanip>

int main() {
    // Rango de tamaños de arreglo a probar (desde 16 hasta 4096 elementos).
    // 4096 doubles = 32 KB, lo cual cabe perfectamente en tu caché L1.
    const int MIN_SIZE = 16;
    const int MAX_SIZE = 4096;
    
    // Total de operaciones matemáticas a realizar por prueba para obtener una medición precisa
    const long long TARGET_TOTAL_OPS = 100000000; 

    std::ofstream csvFile("pure_ilp_results.csv");
    csvFile << "ArraySize,L1_Size_KB,Friendly_ns_per_op,Unfriendly_ns_per_op\n";

    std::cout << "--- Benchmark de Instruction-Level Parallelism (Puro ALU) ---\n";
    std::cout << "Asegurando que todos los datos permanezcan en la caché L1...\n\n";

    for (int size = MIN_SIZE; size <= MAX_SIZE; size *= 2) {
        
        // Calcular cuántas iteraciones necesitamos para alcanzar el objetivo de operaciones
        long long iterations = TARGET_TOTAL_OPS / size;
        long long actual_ops = iterations * size;
        double sizeKB = (size * sizeof(double)) / 1024.0;

        std::cout << "Probando tamano: " << size << " elementos (" << sizeKB << " KB)... " << std::flush;

        std::vector<double> x(size, 1.1);
        std::vector<double> y(size, 2.2);
        std::vector<double> z(size, 0.0);
        std::vector<double> f(size, 1.0);

        // --- Prueba 1: ILP-Friendly (Multiple Issue / Vectorization) ---
        auto start_friendly = std::chrono::high_resolution_clock::now();
        for (long long iter = 0; iter < iterations; ++iter) {
            for (int i = 0; i < size; ++i) {
                z[i] = x[i] + y[i]; 
            }
        }
        auto end_friendly = std::chrono::high_resolution_clock::now();
        double friendly_ns = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end_friendly - start_friendly).count()) / actual_ops;


        // --- Prueba 2: ILP-Hostile (Dependencia en Serie / Fibonacci) ---
        auto start_unfriendly = std::chrono::high_resolution_clock::now();
        for (long long iter = 0; iter < iterations; ++iter) {
            f[0] = 1.0; 
            f[1] = 1.0;
            for (int i = 2; i < size; ++i) {
                f[i] = f[i-1] + f[i-2]; 
            }
        }
        auto end_unfriendly = std::chrono::high_resolution_clock::now();
        
        // Ajustamos actual_ops para la prueba hostil porque el bucle empieza en 2, no en 0
        long long hostile_actual_ops = iterations * (size - 2);
        double unfriendly_ns = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end_unfriendly - start_unfriendly).count()) / hostile_actual_ops;

        std::cout << "Listo. Friendly: " << friendly_ns << " ns/op | Unfriendly: " << unfriendly_ns << " ns/op\n";

        // Guardar resultados en el archivo CSV
        csvFile << size << "," << sizeKB << "," << friendly_ns << "," << unfriendly_ns << "\n";
        
        // Evitar que el compilador elimine el código optimizándolo (Dead Code Elimination)
        volatile double dummy1 = z[size-1];
        volatile double dummy2 = f[size-1];
    }

    csvFile.close();
    std::cout << "\nResultados guardados en 'pure_ilp_results.csv'." << std::endl;
    return 0;
}