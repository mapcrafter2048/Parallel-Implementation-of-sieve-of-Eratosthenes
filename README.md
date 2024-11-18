# Parallel Implementation of Sieve of Eratosthenes

## **Overview**

This repository contains the parallel implementation of the Sieve of Eratosthenes algorithm using **OpenMP** and **MPI**. The Sieve of Eratosthenes is a classical algorithm for finding all prime numbers up to a given number \( n \). The project demonstrates sequential and parallel implementations, evaluates their performance, and explores the impact of different configurations such as segment size and number of threads.

---

## **Algorithm Formulation**

The Sieve of Eratosthenes works by iteratively marking the multiples of each prime starting from 2, resulting in an efficient determination of all primes up to \( n \).

### **Steps:**
1. Create an array `is_prime[2..n]` initialized to `true`.
2. Iterate over each integer \( p \) starting from 2:
   - If `is_prime[p]` is `true`, mark all multiples of \( p \) as `false` starting from \( p^2 \).
3. After completion, all numbers marked `true` in the array are prime.

---

## **Parallelization**

The sequential implementation is efficient but does not fully utilize modern multi-core processors. Parallelization strategies include:
- **Using OpenMP:** Employing multithreading on shared memory systems to process segments in parallel.
- **Using MPI:** Leveraging distributed memory systems to divide work among multiple processes.

### **Parallel Algorithm (Segmented Sieve)**
1. **Precompute Small Primes:**
   - Compute all primes up to \( \sqrt{n} \) using the sequential sieve.
2. **Divide the Range:**
   - Divide the range \([ \sqrt{n} + 1, n ]\) into smaller segments.
3. **Parallel Processing:**
   - Each thread/process handles a segment and marks non-prime numbers using the precomputed small primes.
4. **Collect Results:**
   - Combine results from all threads/processes to produce the final list of primes.

---

## **Mathematical Proof**

### **Sequential Complexity**

The sequential Sieve of Eratosthenes has a time complexity of:
\[
O(n \log \log n)
\]

#### **Proof:**
1. The outer loop iterates over all primes \( p \leq \sqrt{n} \).
2. The inner loop marks multiples of \( p \), starting from \( p^2 \) to \( n \).
3. The number of operations is approximately proportional to:
\[
\sum_{p \text{ prime} \leq \sqrt{n}} \left( \frac{n}{p} - p \right) \approx n \sum_{p \text{ prime} \leq \sqrt{n}} \frac{1}{p}
\]
4. Using results from number theory, the sum of reciprocals of primes up to \( x \) is approximated as:
\[
\sum_{p \leq x} \frac{1}{p} \approx \log \log x
\]
5. For large \( n \), this simplifies to:
\[
O(n \log \log n)
\]

### **Parallel Complexity**

The parallel complexity depends on the number of threads/processes \( P \) and the workload distribution:
\[
T_{\text{parallel}} = O\left( \frac{n}{P} \log \log n \right)
\]
#### **Analysis:**
- **Precomputation of Small Primes:**
  - Sequential step: \( O(\sqrt{n} \log \log n) \).
- **Segmented Sieving:**
  - Each thread/process handles \( \frac{n}{P} \) numbers.
  - Total time is reduced by a factor of \( P \).

---

## **Implementation**

### **1. Sequential Implementation**
The sequential algorithm computes all primes up to \( n \) efficiently in \( O(n \log \log n) \). This implementation serves as a baseline for performance comparisons.

### **2. OpenMP Implementation**
Uses multithreading to divide the sieving work into segments, where each thread processes a segment independently.
