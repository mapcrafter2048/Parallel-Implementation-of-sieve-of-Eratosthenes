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
    auto start_time = steady_clock::now();
    int status = system(command.c_str());  // Capture the return status to avoid warnings
    (void)status;  // Explicitly ignore the status to prevent unused variable warnings
    auto end_time = steady_clock::now();
    duration<double> exec_time = end_time - start_time;
    return exec_time.count();
}

int main() {
    // Ranges for n values
    vector<long long> n_values = {10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};  // You can adjust this as needed

    // Range for number of threads/processes
    vector<int> num_threads_list;
    for (int i = 1; i <= 24; i++) {
        num_threads_list.push_back(i);
    }

    // Open output files
    ofstream omp_result_file("results_openmp.csv");
    ofstream mpi_result_file("results_mpi.csv");
    ofstream seq_result_file("results_sequential.csv");

    if (!omp_result_file.is_open() || !mpi_result_file.is_open()) {
        cerr << "Failed to open results files for writing." << endl;
        return -1;
    }

    // Write headers
    omp_result_file << "n,num_threads,Execution_Time\n";
    mpi_result_file << "n,num_processes,Execution_Time\n";
    seq_result_file << "n,Execution_Time\n";
    

    for (auto n : n_values) {
        cout << "Running algorithms for n = " << n << endl;

        // Sequential Version
        cout << "Running Sequential Version..." << endl;
        string seq_command = "./sieve_sequential " + to_string(n);
        double seq_time = execute_command(seq_command);
        cout << "Sequential time: " << seq_time << " seconds" << endl;
        seq_result_file << n << "," << seq_time << "\n";

        // OpenMP Version with varying number of threads
        for (auto num_threads : num_threads_list) {
            cout << "Running OpenMP Version with " << num_threads << " threads..." << endl;
            string omp_command = "OMP_NUM_THREADS=" + to_string(num_threads) + " ./sieve_openmp " + to_string(n);
            double omp_time = execute_command(omp_command);
            cout << "OpenMP time (" << num_threads << " threads): " << omp_time << " seconds" << endl;

            // Write results to OpenMP results file
            omp_result_file << n << "," << num_threads << "," << omp_time << "\n";
        }

        // MPI Version with varying number of processes
        for (auto num_processes : num_threads_list) {
            cout << "Running MPI Version with " << num_processes << " processes..." << endl;
            string mpi_command = "mpirun -np " + to_string(num_processes) + " ./sieve_mpi " + to_string(n);
            double mpi_time = execute_command(mpi_command);
            cout << "MPI time (" << num_processes << " processes): " << mpi_time << " seconds" << endl;

            // Write results to MPI results file
            mpi_result_file << n << "," << num_processes << "," << mpi_time << "\n";
        }
    }

    seq_result_file.close();
    omp_result_file.close();
    mpi_result_file.close();
    cout << "Results stored in results_openmp.csv and results_mpi.csv" << endl;

    return 0;
}
