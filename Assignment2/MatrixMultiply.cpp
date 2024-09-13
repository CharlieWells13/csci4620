#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <fstream>

// Function to perform matrix multiplication and measure time
double measureMatrixMultiplicationTime(std::vector<std::vector<double>>& matrix1, std::vector<std::vector<double>>& matrix2, int iterations) {
    double totalTime = 0.0;
    int n = matrix1.size(); // Assuming matrices are square

    // Perform multiple iterations and average the time
    for (int i = 0; i < iterations; ++i) {
        std::vector<std::vector<double>> result(n, std::vector<double>(n));  // To store the result of the multiplication

        auto start = std::chrono::steady_clock::now();
        for (int row = 0; row < n; ++row) {
            for (int col = 0; col < n; ++col) {
                result[row][col] = 0.0;
                for (int inner = 0; inner < n; ++inner) {
                    result[row][col] += matrix1[row][inner] * matrix2[inner][col];
                }
            }
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        totalTime += elapsed.count();
    }

    return totalTime / iterations;
}

int main() {
    std::ofstream file("matrix_multiplication_performance.csv");
    file << "Matrix Size (n),FLOP/s\n";

    const int iterations = 5; // Decreased iterations for faster execution

    // Test different sizes
    for (int n = 10; n <= 1000; n += 50) { // Process sizes from 1000 to 10000 in intervals of 1000
        // Allocate and initialize matrices
        std::vector<std::vector<double>> matrix1(n, std::vector<double>(n));
        std::vector<std::vector<double>> matrix2(n, std::vector<double>(n));

        std::srand(static_cast<unsigned>(std::time(0)));
        for (int row = 0; row < n; ++row) {
            for (int col = 0; col < n; ++col) {
                matrix1[row][col] = static_cast<double>(std::rand()) / RAND_MAX;
                matrix2[row][col] = static_cast<double>(std::rand()) / RAND_MAX;
            }
        }

        // Measure the computation time
        double timeTaken = measureMatrixMultiplicationTime(matrix1, matrix2, iterations);

        // Calculate the number of floating-point operations (FLOP)
        long long flops = 2 * n * n * n; // Each element requires 2 operations (multiplication and addition), n^3 total operations

        // Calculate performance in FLOP/s
        double flopsPerSecond = flops / timeTaken;

        file << n << "," << flopsPerSecond << "\n";

        std::cout << "Processed size " << n << std::endl;
    }

    file.close();
    std::cout << "Data has been written to matrix_multiplication_performance.csv" << std::endl;
    return 0;
}
