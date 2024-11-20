// Filename: sieve_openmp.cpp

#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]) {
    // Check if the upper limit n is provided
    if (argc != 2) {
        cout << "Usage: ./sieve_openmp <n>" << endl;
        return -1;
    }

    long long n = atoll(argv[1]);

    auto start_time = high_resolution_clock::now();

    long long sqrt_n = sqrt(n);

    // Step 1: Precompute small primes up to sqrt(n)
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
    vector<long long> small_primes;
    for (long long p = 2; p <= sqrt_n; p++) {
        if (is_prime_small[p]) {
            small_primes.push_back(p);
        }
    }

    // Step 2: Segmented sieve for the range [sqrt_n + 1, n]
    long long segment_size = 100000; // Adjust segment size as needed
    long long count = 0;

    #pragma omp parallel
    {
        vector<char> is_prime(segment_size);
        #pragma omp for reduction(+:count)
        for (long long low = sqrt_n + 1; low <= n; low += segment_size) {
            fill(is_prime.begin(), is_prime.end(), true);

            long long high = min(low + segment_size - 1, n);

            for (size_t i = 0; i < small_primes.size(); i++) {
                long long p = small_primes[i];
                long long start = max(p * p, ((low + p - 1) / p) * p);
                for (long long j = start; j <= high; j += p) {
                    is_prime[j - low] = false;
                }
            }

            // Count primes in the current segment
            for (long long i = low; i <= high; i++) {
                if (is_prime[i - low]) {
                    count++;
                }
            }
        }
    }

    // Add primes up to sqrt_n
    long long small_prime_count = 0;
    for (long long p = 2; p <= sqrt_n; p++) {
        if (is_prime_small[p]) {
            small_prime_count++;
        }
    }

    long long total_primes = small_prime_count + count;

    auto end_time = high_resolution_clock::now();
    duration<double> exec_time = end_time - start_time;

    cout << "Total primes up to " << n << ": " << total_primes << endl;
    cout << "Execution Time: " << exec_time.count() << " seconds" << endl;

    return 0;
}
