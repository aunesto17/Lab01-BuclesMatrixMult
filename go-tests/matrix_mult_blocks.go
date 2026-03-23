package main

import (
	"fmt"
	"math/rand"
	"time"
)

// Function to multiply matrices using block partitioning
func blockMatrixMultiply(A, B [][]int, blockSize int) [][]int {
	m := len(A)
	n := len(A[0])
	p := len(B[0])

	C := make([][]int, m)
	for i := range C {
		C[i] = make([]int, p)
	}

	for i0 := 0; i0 < m; i0 += blockSize {
		for j0 := 0; j0 < p; j0 += blockSize {
			for k0 := 0; k0 < n; k0 += blockSize {
				iMax := minInt(i0+blockSize, m)
				jMax := minInt(j0+blockSize, p)
				kMax := minInt(k0+blockSize, n)

				for i := i0; i < iMax; i++ {
					for j := j0; j < jMax; j++ {
						sum := C[i][j]
						for k := k0; k < kMax; k++ {
							sum += A[i][k] * B[k][j]
						}
						C[i][j] = sum
					}
				}
			}
		}
	}

	return C
}

// Helper to find the minimum of two integers
func minInt(a, b int) int {
	if a < b {
		return a
	}
	return b
}

// Function to generate a random matrix
func generateRandomMatrix(rows, cols int) [][]int {
	matrix := make([][]int, rows)
	for i := range matrix {
		matrix[i] = make([]int, cols)
		for j := range matrix[i] {
			matrix[i][j] = rand.Intn(100) // Random integers between 0 and 99
		}
	}
	return matrix
}

func main() {
	// Fixed seed to ensure consistent benchmarking
	rand.Seed(42)

	// Define the matrix sizes and block sizes to test
	matrixSizes := []int{100, 200, 300, 400, 500, 600, 700, 800, 900, 1000}
	blockSizes := []int{5, 8, 12, 16, 32, 64}

	// Print CSV Header
	fmt.Println("MatrixSize,BlockSize,Time_ns")

	for _, size := range matrixSizes {
		m, n, p := size, size, size

		// Generate the matrices once per size to keep tests fair across block sizes
		A := generateRandomMatrix(m, n)
		B := generateRandomMatrix(n, p)

		for _, blockSize := range blockSizes {
			start := time.Now()
			_ = blockMatrixMultiply(A, B, blockSize)

			// Measure in nanoseconds to match the C++ output
			duration := time.Since(start).Nanoseconds()

			fmt.Printf("%d,%d,%d\n", size, blockSize, duration)
		}
	}
}
