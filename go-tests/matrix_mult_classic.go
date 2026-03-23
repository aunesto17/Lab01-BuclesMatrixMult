package main

import (
	"fmt"
	"math/rand"
	"time"
)

func multiply(A, B [][]float64, n int) [][]float64 {
	C := make([][]float64, n)
	for i := range C {
		C[i] = make([]float64, n)
	}

	for i := 0; i < n; i++ {
		for j := 0; j < n; j++ {
			for k := 0; k < n; k++ {
				C[i][j] += A[i][k] * B[k][j]
			}
		}
	}
	return C
}

// Helper to generate a matrix with random values
func generateMatrix(n int) [][]float64 {
	M := make([][]float64, n)
	rand.Seed(42)
	for i := range M {
		M[i] = make([]float64, n)
		for j := range M[i] {
			M[i][j] = rand.Float64()
		}
	}
	return M
}

func main() {
	sizes := []int{100, 200, 300, 400, 500, 600, 700, 800, 900, 1000}

	fmt.Println("Size,Time_ms")

	for _, n := range sizes {
		A := generateMatrix(n)
		B := generateMatrix(n)

		start := time.Now()
		_ = multiply(A, B, n)
		duration := time.Since(start)

		fmt.Printf("%d,%d\n", n, duration.Milliseconds())
	}
}
