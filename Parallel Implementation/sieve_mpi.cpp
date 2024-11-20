// Filename: sieve_mpi.cpp

#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <mpi.h>

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2) {
        if (rank == 0) {
            cout << "Usage: mpirun -np <num_processes> ./sieve_mpi <n>" << endl;
        }
        MPI_Finalize();
        return -1;
    }

    long long n = atoll(argv[1]);
    long long sqrt_n = sqrt(n);

    auto start_time = high_resolution_clock::now();

    // Step 1: Process 0 computes small primes up to sqrt(n)
    vector<long long> small_primes;
    if (rank == 0) {
        vector<bool> is_prime_small(sqrt_n + 1, true);
        is_prime_small[0] = is_prime_small[1] = false;

        for (long long p = 2; p * p <= sqrt_n; p++) {
            if (is_prime_small[p]) {
                for (long long i = p * p; i <= sqrt_n; i += p) {
                    is_prime_small[i] = false;
                }
            }
        }

        // Collect small primes
        for (long long p = 2; p <= sqrt_n; p++) {
            if (is_prime_small[p]) {
                small_primes.push_back(p);
            }
        }
    }

    // Broadcast the number of small primes
    long long num_small_primes;
    if (rank == 0) {
        num_small_primes = small_primes.size();
    }
    MPI_Bcast(&num_small_primes, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    // Broadcast the small primes array
    if (rank != 0) {
        small_primes.resize(num_small_primes);
    }
    MPI_Bcast(small_primes.data(), num_small_primes, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    // Step 2: Each process sieves its portion
    long long low = (rank * (n - sqrt_n) / size) + sqrt_n + 1;
    long long high = ((rank + 1) * (n - sqrt_n) / size) + sqrt_n;

    if (low < sqrt_n + 1) low = sqrt_n + 1;
    if (high > n) high = n;

    long long segment_size = high - low + 1;

    vector<char> is_prime(segment_size);
    fill(is_prime.begin(), is_prime.end(), true);

    for (size_t i = 0; i < small_primes.size(); i++) {
        long long p = small_primes[i];
        long long start = max(p * p, ((low + p - 1) / p) * p);
        for (long long j = start; j <= high; j += p) {
            is_prime[j - low] = false;
        }
    }

    // Count primes in the local segment
    long long local_count = 0;
    for (long long i = 0; i < segment_size; i++) {
        if (is_prime[i]) {
            local_count++;
        }
    }

    // Sum up the total number of primes
    long long total_count = 0;
    MPI_Reduce(&local_count, &total_count, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    // Add small primes to total count
    if (rank == 0) {
        total_count += num_small_primes;

        auto end_time = high_resolution_clock::now();
        duration<double> exec_time = end_time - start_time;

        cout << "Total primes up to " << n << ": " << total_count << endl;
        cout << "Execution Time: " << exec_time.count() << " seconds" << endl;
    }

    MPI_Finalize();
    return 0;
}
