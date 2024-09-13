#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <fstream>

// Function to perform matrix-vector multiplication and measure time
double measureMatrixVectorMultiplicationTime(std::vector<std::vector<double>>& matrix, std::vector<double>& vec, int iterations) {
    double totalTime = 0.0;
    int n = matrix.size(); // Assuming matrix is square

    // Perform multiple iterations and average the time
    for (int i = 0; i < iterations; ++i) {
        std::vector<double> result(n);  // To store the result of the multiplication

        auto start = std::chrono::steady_clock::now();
        for (int row = 0; row < n; ++row) {
            result[row] = 0.0;
            for (int col = 0; col < n; ++col) {
                result[row] += matrix[row][col] * vec[col];
            }
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        totalTime += elapsed.count();
    }

    return totalTime / iterations;
}

int main() {
    std::ofstream file("matrix_vector_performance.csv");
    file << "Matrix Size (n),FLOP/s\n";

    const int iterations = 10; // Decreased iterations for faster execution

    // Test different sizes
    for (int n = 100; n <= 10000; n += 100) { // Process sizes from 100 to 10000 in intervals of 100
        // Allocate and initialize matrix and vector
        std::vector<std::vector<double>> matrix(n, std::vector<double>(n));
        std::vector<double> vec(n);

        std::srand(static_cast<unsigned>(std::time(0)));
        for (int row = 0; row < n; ++row) {
            for (int col = 0; col < n; ++col) {
                matrix[row][col] = static_cast<double>(std::rand()) / RAND_MAX;
            }
            vec[row] = static_cast<double>(std::rand()) / RAND_MAX;
        }

        // Measure the computation time
        double timeTaken = measureMatrixVectorMultiplicationTime(matrix, vec, iterations);

        // Calculate the number of floating-point operations (FLOP)
        long long flops = 2 * n * n; // Each element requires 2 operations (multiplication and addition)

        // Calculate performance in FLOP/s
        double flopsPerSecond = flops / timeTaken;

        file << n << "," << flopsPerSecond << "\n";

        if (n % 1000 == 0) { // Adjust output frequency as needed
            std::cout << "Processed size " << n << std::endl;
        }
    }

    file.close();
    std::cout << "Data has been written to matrix_vector_performance.csv" << std::endl;
    return 0;
}
