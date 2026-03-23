// 1. mult de matrices normal, por bloques(6 bucles)
// 2. analizar con software especializado en terminos de cache L1, L2, L3, TLB, etc

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cstring>
#include <fstream>

using namespace std;

int main() {
    // Tamaños de matrices para las pruebas
    vector<int> sizes = {100, 1000, 10000};
    const int NUM_RUNS = 10; // Número de ejecuciones para promediar

    // resultados a un archivo
    ofstream results_file("performance_results.txt");
    if (!results_file.is_open()) {
        cerr << "Error al abrir el archivo performance_results.txt" << endl;
        return EXIT_FAILURE;
    }

    // header archivo
    results_file << "Matrix Size,Avg Loop 1 (ns),Avg Loop 2 (ns)" << endl;

    cout << "Iniciando pruebas de rendimiento (" << NUM_RUNS << " ejecuciones por tamaño)..." << endl;

    for (int MAX : sizes) {
        cout << "Procesando matriz de tamaño: " << MAX << "x" << MAX << endl;

        // --- Asignación de Memoria ---
        double** A = new double*[MAX];
        if (MAX > 0) {
            A[0] = new double[MAX * MAX];
            for (int i = 1; i < MAX; i++) {
                A[i] = A[i-1] + MAX;
            }
            memset(A[0], 0.0, MAX * MAX * sizeof(double));
        }

        double* x = new double[MAX];
        double* y = new double[MAX];

        // --- Inicialización de Datos Aleatorios ---
        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_real_distribution<double> distr(0.0, 10.0);

        for (int i = 0; i < MAX; i++) {
            x[i] = distr(eng);
        }

        for (int i = 0; i < MAX; i++) {
            for (int j = 0; j < MAX; j++) {
                A[i][j] = distr(eng);
            }
        }

        cout << "  Datos inicializados." << endl;

        long long total_loop1_duration = 0;
        long long total_loop2_duration = 0;

        // --- ejecutar pruebas = NUM_RUNS ---
        for (int run = 0; run < NUM_RUNS; run++) {
            
            // Reiniciar y para el primer bucle
            memset(y, 0, MAX * sizeof(double));

            // --- Primer Par de Bucles (Cache-Friendly) ---
            auto start = chrono::steady_clock::now();
            for (int i = 0; i < MAX; i++) {
                for (int j = 0; j < MAX; j++) {
                    y[i] += A[i][j] * x[j];
                }
            }
            auto end = chrono::steady_clock::now();
            total_loop1_duration += chrono::duration_cast<chrono::nanoseconds>(end - start).count();

            // Reiniciar y para el segundo bucle
            memset(y, 0, MAX * sizeof(double));

            // --- Segundo Par de Bucles (Cache-Unfriendly) ---
            start = chrono::steady_clock::now();
            for (int j = 0; j < MAX; j++) {
                for (int i = 0; i < MAX; i++) {
                    y[i] += A[i][j] * x[j];
                }
            }
            end = chrono::steady_clock::now();
            total_loop2_duration += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        }

        // --- Calcular Promedios ---
        long long avg_loop1 = total_loop1_duration / NUM_RUNS;
        long long avg_loop2 = total_loop2_duration / NUM_RUNS;

        cout << "  El primer bucle duró en promedio:  " << avg_loop1 << " ns" << endl;
        cout << "  El segundo bucle duró en promedio: " << avg_loop2 << " ns" << endl;

        // Escribir resultados promedio al archivo
        results_file << MAX << "," << avg_loop1 << "," << avg_loop2 << endl;

        // --- Liberar Memoria ---
        if (MAX > 0) {
            delete[] A[0];
        }
        delete[] A;
        delete[] x;
        delete[] y;

        cout << "------------------------------------" << endl;
    }

    results_file.close();
    cout << "Resultados guardados en performance_results.txt" << endl;

    return 0;
}