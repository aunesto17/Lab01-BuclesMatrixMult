#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <chrono>

using namespace std;

// Function to multiply matrices using block partitioning
vector<vector<int>> blockMatrixMultiply(const vector<vector<int>>& A, const vector<vector<int>>& B, int blockSize) {
    int m = A.size();
    int n = A[0].size();
    int p = B[0].size();

    vector<vector<int>> C(m, vector<int>(p, 0));

    for (int i0 = 0; i0 < m; i0 += blockSize) {
        for (int j0 = 0; j0 < p; j0 += blockSize) {
            for (int k0 = 0; k0 < n; k0 += blockSize) {
                int i_max = min(i0 + blockSize, m);
                int j_max = min(j0 + blockSize, p);
                int k_max = min(k0 + blockSize, n);

                for (int i = i0; i < i_max; i++) {
                    for (int j = j0; j < j_max; j++) {
                        int sum = C[i][j];
                        for (int k = k0; k < k_max; k++) {
                            sum += A[i][k] * B[k][j];
                        }
                        C[i][j] = sum;
                    }
                }
            }
        }
    }

    return C;
}

int getRandom() {
    static std::random_device rd; 
    static std::mt19937 eng(rd()); 
    std::uniform_int_distribution<> distr(0, 99);
    return distr(eng);
}

// Function to generate a random matrix
vector<vector<int>> generateRandomMatrix(int rows, int cols) {
    vector<vector<int>> matrix(rows, vector<int>(cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = getRandom();  // Random integers between 0 and 99
        }
    }
    return matrix;
}

double measureExecutionTime(const vector<vector<int>>& A, const vector<vector<int>>& B, int blockSize) {
    auto start = chrono::steady_clock::now();
    blockMatrixMultiply(A, B, blockSize);
    auto end = chrono::steady_clock::now();
    double cpu_time_used = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    return cpu_time_used;
}

int main() {
    // Define the matrix sizes you want to test (assuming square matrices for simplicity)
    vector<int> matrixSizes = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
    
    // Different block sizes to test (added 32 and 64 as they often fit CPU caches well)
    vector<int> blockSizes = {5, 8, 12, 16, 32, 64};  

    // Print CSV Header
    cout << "MatrixSize,BlockSize,Time_ns\n";

    for (int size : matrixSizes) {
        // Using square matrices (m = n = p = size)
        int m = size;
        int n = size;
        int p = size;

        // Generate the matrices once per size to keep tests fair across block sizes
        vector<vector<int>> A = generateRandomMatrix(m, n);
        vector<vector<int>> B = generateRandomMatrix(n, p);

        for (int blockSize : blockSizes) {
            double executionTime = measureExecutionTime(A, B, blockSize);
            
            // Print results in CSV format: Size, BlockSize, Time
            cout << size << "," << blockSize << "," << executionTime << "\n";
        }
    } 
        
    return 0;
}