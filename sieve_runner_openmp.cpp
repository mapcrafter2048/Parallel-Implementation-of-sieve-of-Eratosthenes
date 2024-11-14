// Filename: sieve_runner_openmp.cpp

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

// Function to execute a command and return its output
double execute_command(const string &command) {
    auto start_time = high_resolution_clock::now();
    int status = system(command.c_str());  // Capture the return status to avoid warnings
    (void)status;  // Explicitly ignore the status to prevent unused variable warnings
    auto end_time = high_resolution_clock::now();
    duration<double> exec_time = end_time - start_time;
    return exec_time.count();
}

int main() {
    // Set the value of n
    long long n = 100000000;  // Adjust this as needed

    // Range for number of threads
    vector<int> num_threads_list;
    for (int i = 1; i <= 24; i++) {
        num_threads_list.push_back(i);
    }

    // Range for segment sizes
    vector<long long> segment_sizes = {1000,10000, 100000, 1000000, 10000000};  // Adjust as needed

    // Open output file
    ofstream result_file("results_openmp_segmented.csv");
    if (!result_file.is_open()) {
        cerr << "Failed to open results_openmp_segmented.csv for writing." << endl;
        return -1;
    }

    // Write header
    result_file << "n,num_threads,segment_size,Execution_Time\n";

    // Run experiments
    for (auto num_threads : num_threads_list) {
        for (auto segment_size : segment_sizes) {
            cout << "Running OpenMP Version with " << num_threads << " threads and segment size " << segment_size << "..." << endl;
            string omp_command = "OMP_NUM_THREADS=" + to_string(num_threads) + " ./sieve_openmp_segmented " + to_string(n) + " " + to_string(segment_size) + " > /dev/null";
            double omp_time = execute_command(omp_command);
            cout << "Execution time: " << omp_time << " seconds" << endl;

            // Write results to file
            result_file << n << "," << num_threads << "," << segment_size << "," << omp_time << "\n";
        }
    }

    result_file.close();
    cout << "Results stored in results_openmp_segmented.csv" << endl;

    return 0;
}
