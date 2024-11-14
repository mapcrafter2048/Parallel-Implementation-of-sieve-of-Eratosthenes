// Filename: sieve_sequential.cpp

#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]) {
    // Check if the upper limit n is provided
    if (argc != 2) {
        cout << "Usage: ./sieve_sequential <n>" << endl;
        return -1;
    }

    long long n = atoll(argv[1]);

    auto start_time = high_resolution_clock::now();

    // Initialize the is_prime array
    vector<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = false;

    long long sqrt_n = sqrt(n);

    // Sieve algorithm
    for (long long p = 2; p <= sqrt_n; p++) {
        if (is_prime[p]) {
            for (long long i = p * p; i <= n; i += p) {
                is_prime[i] = false;
            }
        }
    }

    auto end_time = high_resolution_clock::now();
    duration<double> exec_time = end_time - start_time;

    // Count the number of primes found
    long long prime_count = 0;
    for (long long i = 2; i <= n; i++) {
        if (is_prime[i]) {
            prime_count++;
        }
    }

    cout << "Total primes up to " << n << ": " << prime_count << endl;
    cout << "Execution Time: " << exec_time.count() << " seconds" << endl;

    return 0;
}
