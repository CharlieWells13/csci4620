#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <algorithm>  

// Function to measure computation time and return the median time
double measureTwoNormComputationTime(std::vector<double>& vec, int iterations, double outlierFraction = 0.1) {
    std::vector<double> times;  // Vector to store individual measurements
    times.reserve(iterations);  // Reserve space to avoid reallocations

    // Perform multiple iterations and store the times
    for (int i = 0; i < iterations; ++i) {
        auto start = std::chrono::steady_clock::now();
        double sum = 0.0;
        for (double value : vec) {
            sum += value * value;
        }
        double norm = std::sqrt(sum);
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        times.push_back(elapsed.count());  // Store the elapsed time
    }

    // Sort the times
    std::sort(times.begin(), times.end());

    // Remove outliers
    int numOutliers = static_cast<int>(iterations * outlierFraction);
    if (numOutliers > 0) {
        times.erase(times.begin(), times.begin() + numOutliers);              // Remove low outliers
        times.erase(times.end() - numOutliers, times.end());                  // Remove high outliers
    }

    // Calculate the median
    double medianTime;
    int adjustedIterations = times.size();  // Size after removing outliers
    if (adjustedIterations % 2 == 0) {
        // If even number of measurements, average the two middle values
        medianTime = (times[adjustedIterations / 2 - 1] + times[adjustedIterations / 2]) / 2.0;
    } else {
        // If odd number of measurements, take the middle value
        medianTime = times[adjustedIterations / 2];
    }

    return medianTime;
}

int main() {
    std::ofstream file("two_norm_performance.csv");
    file << "n,FLOP/s\n";

    const int iterations = 100;  // Increase the number of iterations for better accuracy

    // Test different sizes
    for (int n = 10; n <= 10000; n += 10) {
        // Allocate and initialize vector
        std::vector<double> vec(n);
        std::srand(static_cast<unsigned>(std::time(0)));
        for (int i = 0; i < n; ++i) {
            vec[i] = static_cast<double>(std::rand()) / RAND_MAX;
        }

        // Measure the computation time
        double timeTaken = measureTwoNormComputationTime(vec, iterations);

        // Calculate the number of floating-point operations (FLOP)
        long long flops = 2 * n + (n - 1);

        // Calculate performance in FLOP/s
        double flopsPerSecond = flops / timeTaken;

        file << n << "," << flopsPerSecond << "\n";

        if (n % 1000 == 0) {
            std::cout << "Processed n = " << n << std::endl;
        }
    }

    file.close();
    std::cout << "Data has been written to two_norm_performance.csv" << std::endl;
    return 0;
}


