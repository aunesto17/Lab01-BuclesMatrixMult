package main

import (
	"fmt"
	"math/rand"
	"testing"
)

// setupData handles the memory allocation and random initialization
func setupData(max int) ([][]float64, []float64, []float64) {
	flatA := make([]float64, max*max)
	A := make([][]float64, max)
	for i := range A {
		A[i] = flatA[i*max : (i+1)*max]
	}

	x := make([]float64, max)
	y := make([]float64, max)

	for i := 0; i < max; i++ {
		x[i] = rand.Float64() * 10.0
		for j := 0; j < max; j++ {
			A[i][j] = rand.Float64() * 10.0
		}
	}

	return A, x, y
}

// BenchmarkMatrixMultiplication groups all our size tests together
func BenchmarkMatrixMultiplication(b *testing.B) {
	sizes := []int{100, 1000, 10000, 50000}

	for _, size := range sizes {

		// --- Primer Par de Bucles (Cache-Friendly) ---
		b.Run(fmt.Sprintf("Friendly_Size_%d", size), func(b *testing.B) {
			A, x, y := setupData(size)

			b.ResetTimer() // Stop the timer while we setup, start it exactly here

			for n := 0; n < b.N; n++ { // b.N is dynamically managed by Go
				for i := range y {
					y[i] = 0 // Reiniciar y
				}
				for i := 0; i < size; i++ {
					for j := 0; j < size; j++ {
						y[i] += A[i][j] * x[j]
					}
				}
			}
		})

		// --- Segundo Par de Bucles (Cache-Unfriendly) ---
		b.Run(fmt.Sprintf("Unfriendly_Size_%d", size), func(b *testing.B) {
			A, x, y := setupData(size)

			b.ResetTimer()

			for n := 0; n < b.N; n++ {
				for i := range y {
					y[i] = 0 // Reiniciar y
				}
				for j := 0; j < size; j++ {
					for i := 0; i < size; i++ {
						y[i] += A[i][j] * x[j]
					}
				}
			}
		})
	}
}
