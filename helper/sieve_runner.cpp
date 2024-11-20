// Filename: sieve_runner.cpp

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
    int status = system(command.c_str());
    (void)status;
    auto end_time = high_resolution_clock::now();
    duration<double> exec_time = end_time - start_time;
    return exec_time.count();
}

int main() {
    // Ranges for n values
    vector<long long> n_values = {10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

    // Number of threads for OpenMP
    int num_threads = 4; // Change this as per your requirement

    // Number of processes for MPI
    int num_processes = 4; // Change this as per your requirement

    // Open output file
    ofstream result_file("results.txt");
    if (!result_file.is_open()) {
        cerr << "Failed to open results.txt for writing." << endl;
        return -1;
    }

    result_file << "n,Sequential_Time,OpenMP_Time,MPI_Time\n";

    for (auto n : n_values) {
        cout << "Running algorithms for n = " << n << endl;

        // Run Sequential Version
        string seq_command = "./sieve_sequential " + to_string(n);
        double seq_time = execute_command(seq_command);
        cout << "Sequential time: " << seq_time << " seconds" << endl;

        // Run OpenMP Version
        string omp_command = "OMP_NUM_THREADS=" + to_string(num_threads) + " ./sieve_openmp " + to_string(n);
        double omp_time = execute_command(omp_command);
        cout << "OpenMP time (" << num_threads << " threads): " << omp_time << " seconds" << endl;

        // Run MPI Version
        string mpi_command = "mpirun -np " + to_string(num_processes) + " ./sieve_mpi " + to_string(n);
        double mpi_time = execute_command(mpi_command);
        cout << "MPI time (" << num_processes << " processes): " << mpi_time << " seconds" << endl;

        // Write results to file
        result_file << n << "," << seq_time << "," << omp_time << "," << mpi_time << "\n";
    }

    result_file.close();
    cout << "Results stored in results.txt" << endl;

    return 0;
}
