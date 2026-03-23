#include <iostream>
#include <vector>
#include <chrono>
#include <random>

using Matrix = std::vector<std::vector<double>>;

Matrix multiply(const Matrix& A, const Matrix& B, int n) {
    Matrix C(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

// Helper to generate a matrix with random values
Matrix generateMatrix(int n) {
    Matrix M(n, std::vector<double>(n));
    std::mt19937 gen(42);
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            M[i][j] = dis(gen);
        }
    }
    return M;
}

int main() {
    std::vector<int> sizes = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
    
    std::cout << "Size,Time_ms" << "\n";
    
    for (int n : sizes) {
        Matrix A = generateMatrix(n);
        Matrix B = generateMatrix(n);
        
        auto start = std::chrono::high_resolution_clock::now();
        Matrix C = multiply(A, B, n);
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << n << "," << duration.count() << "\n";
    }
    
    return 0;
}