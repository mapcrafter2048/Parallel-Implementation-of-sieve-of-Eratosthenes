# Parallel Implementation of the Sieve of Eratosthenes

This repository contains a parallel implementation of the Sieve of Eratosthenes, a well-known algorithm to find all prime numbers up to a given number ${n}$. The project explores sequential and parallel implementations using OpenMP and MPI, and evaluates their performance.

## Overview

### What is the Sieve of Eratosthenes?

The Sieve of Eratosthenes is an efficient algorithm for finding all prime numbers up to a given integer ${n}$ . It works by iteratively marking the multiples of each prime, starting from 2.

### Why Parallelize?

As ${n}$ grows large, the computational cost of the sieve increases. Parallelizing the algorithm enables us to leverage modern multi-core and distributed computing systems to process large datasets more efficiently.

---

## Sequential Algorithm

### Steps

1. **Initialization**: Create a Boolean array `is_prime[2..n]` initialized to `true`.
2. **Marking Multiples**: For each integer ${p}$ starting from 2:
    - If ${p}$ is prime, mark all multiples of ${p}$ as not prime.
3. **Stop Condition**: Stop at ${ p^2 > n }$.

### Time Complexity

The time complexity of the sequential sieve is:
${O(n \log \log n)}$

### Explanation

- For each prime ${p}$, multiples ${ p^2, p^2 + p, ...}$ are marked.
- The total number of operations is proportional to:

${\sum_{p \leq n} \frac{n}{p}}$

- Using results from number theory:

${\sum_{p \text{ prime} \leq x} \frac{1}{p} \approx \log \log x + C}$

- Therefore:

${\text{Time Complexity} = O(n \log \log n)}$

---

## Parallel Algorithm

The parallel Sieve of Eratosthenes uses the following steps:

### Steps

1. **Precompute Small Primes**: Compute all primes up to ${\sqrt{n}}$ sequentially. These primes are used to mark multiples in parallel.
2. **Divide the Range**: Split the range ${[ \sqrt{n} + 1, n ]}$ into segments.
3. **Parallel Marking**: Each thread or process marks multiples of small primes in its assigned segment.
4. **Combine Results**: Aggregate results from all segments.

### Mathematical Formulation

1. **Precomputation**: Using the sequential sieve, compute small primes up to ${\sqrt{n} }$:

${T_{\text{small}} = O(\sqrt{n} \log \log \sqrt{n})}$

1. **Workload Distribution**: Divide the range ${[ \sqrt{n} + 1, n ]}$ into ${p}$ segments, where ${p}$ is the number of processors. Each segment size is approximately:
    
    
    ${S = \frac{n - \sqrt{n}}{P}}$
    

1. **Parallel Time Complexity**: Assuming perfect load balancing and negligible communication overhead, the parallel time complexity is:
    
    
    ${T_{\text{parallel}} = T_{\text{small}} + \frac{T_{\text{marking}}}{P}}$
    

Substituting ${ T_{\text{marking}}}$ = ${O(n \log \log n)}$:

${T_{\text{parallel}} = O(\sqrt{n} \log \log \sqrt{n}) + O\left(\frac{n \log \log n}{P}\right)}$

---

## Speedup and Efficiency

### Speedup

Speedup is defined as:

${S(P) = \frac{T_{\text{sequential}}}{T_{\text{parallel}}}}$

In an ideal scenario:

${S(P) \approx P}$

### Efficiency

Efficiency is given by:

${E(P) = \frac{S(P)}{P}}$

Efficiency decreases with increasing ${p}$ due to overheads, communication, and synchronization costs.

---

## Proof of Parallel Time Complexity

### Assumptions

1. **Uniform Workload**: The range ${[ \sqrt{n} + 1, n ]}$ is evenly divided among ${p}$ processors.
2. **Negligible Overheads**: Communication and synchronization overheads are small compared to computation.

### Sequential Part

The time to compute small primes up to ${ \sqrt{n}}$  is:

${T_{\text{small}} = O(\sqrt{n} \log \log \sqrt{n})}$

### Parallel Part

Each processor handles a segment of size ${ \frac{n}{P} }$, and the time complexity per processor is:

${T_{\text{processor}} = O\left(\frac{n}{P} \log \log n\right)}$

The total time is:

${T_{\text{parallel}} = T_{\text{small}} + T_{\text{processor}}}$

---

## Experimental Results

### Dataset

The algorithm was tested for ${ n }$ values ranging from ${ 10^1 }$ to ${10^9 }$, using both OpenMP (shared memory) and MPI (distributed memory) implementations.

### Results (Example)

| ${n}$ | Sequential Time (s) | OpenMP Time (s) | MPI Time (s) |
| --- | --- | --- | --- |
| ${10^3}$  | 0.004 | 0.041 | 0.575 |
| ${10^6}$  | 0.012 | 0.036 | 0.599 |
| ${10^9}$ | 9.065 | 0.704 | 7.091 |

### Observations

- **OpenMP**:
    - Shows significant speedup on shared memory systems.
    - Minimal communication overhead.
- **MPI**:
    - Higher overhead on single machines.
    - Effective for distributed systems.

---

## Visualizations

1. **Speedup vs. Number of Threads (OpenMP)**

${S(P) = \frac{T_{\text{sequential}}}{T_{\text{parallel}}}}$

1. **3D Plot**: Execution Time vs. Segment Size and Number of Threads

---

## Usage

### Prerequisites

- **OpenMP**: Ensure your compiler supports OpenMP.
- **MPI**: Install an MPI implementation (e.g., MPICH, OpenMPI).

### Compilation

### OpenMP

```bash
g++ -O2 -fopenmp -o sieve_openmp sieve_openmp_segmented.cpp
```

### MPI

```bash
mpic++ -O2 -o sieve_mpi sieve_mpi.cpp
```

# Detailed Proof of the Parallel Time Complexity of the Sieve of Eratosthenes

## Introduction

The Sieve of Eratosthenes is an efficient algorithm for finding all prime numbers up to a given limit ${n}$. When dealing with large ${n}$, parallelizing the algorithm can significantly reduce computation time. This proof aims to derive the time complexity of the parallel implementation of the Sieve of Eratosthenes, considering both the parallelizable and non-parallelizable parts of the algorithm.

---

## Sequential Time Complexity

### Algorithm Overview

1. **Initialization**: Create an array `is_prime[2..n]`, initialized to `true`.
2. **Sieving Process**:
    - For each integer ${p}$ from ${2}$ to ${n}$:
        - If `is_prime[p]` is `true`:
            - Mark all multiples of ${p}$ starting from ${p^2}$ up to ${n}$ as `false`.

---

### Time Complexity Analysis

- **Outer Loop**: Iterates over all primes ${p \leq n}$.
- **Inner Loop**: For each prime ${p}$, marks multiples of ${p}$ from ${p^2}$ to ${n}$.

The total number of operations is approximately proportional to:

${\sum_{p \, \text{prime} \, \leq n} \left( \frac{n - p^2}{p} + 1 \right) \approx n \sum_{p \, \text{prime} \, \leq n} \frac{1}{p}}$

Using the fact that the sum of reciprocals of primes up to ${x}$ is approximately ${\log \log x}$, the time complexity is:

${O(n \log \log n)}$

---

## Parallel Sieve of Eratosthenes:

## Algorithm Steps:

1. **Precompute Small Primes (Sequential Part)**:
    - Compute all primes up to ${\sqrt{n}}$ using the sequential sieve.
2. **Divide the Range (Parallel Part)**:
    - Divide the range ${[n + 1, n]}$ into ${P}$ segments, where ${P}$ is the number of processors.
3. **Parallel Sieving**:
    - Each processor sieves its assigned segment using the list of small primes.
4. **Collect Results**:
    - Combine the results from all processors (if necessary).

---

## Time Complexity Analysis:

### 1. Precomputing Small Primes

- Work Done: Sieving up to ${\sqrt{n}}$.
- Time Complexity: ${T_\text{small} = O(\sqrt{n} \log \log \sqrt{n})}$

Note: This part is sequential and cannot be parallelized.

### 2. Parallel Sieving of Segments

- **Total Work**: For the entire range ${[n + 1, n]}$, similar to the sequential case, the total work is:${W_\text{total} = O(n \log \log n)}$
- **Work per Processor**: The range is evenly divided among ${P}$ processors. Each processor handles approximately: ${\frac{n}{P}}$ numbers. The time complexity per processor is: ${T_\text{segment} = O\left(\frac{n}{P} \cdot \log \log n\right)}$
- **Total Parallel Time**: ${T_\text{parallel} = T_\text{small} + T_\text{segment}}$

### 3. Overall Parallel Time Complexity

- Combining both sequential and parallel parts: ${T_\text{total} = O\left(\sqrt{n} \log \log \sqrt{n} + \frac{n}{P} \log \log n\right)}$
- For large ${n}$, the term ${\sqrt{n} \log \log \sqrt{n}}$ becomes negligible compared to ${\frac{n}{P} \log \log n}$. Therefore:${T_\text{total} \approx O\left(\frac{n}{P} \log \log n\right)}$

---

## Detailed Proof

### Assumptions

1. **Uniform Workload Distribution**: The range is evenly divided among processors.
2. **Negligible Overheads**: Communication and synchronization overheads are minimal.
3. **Sufficiently Large** ${n}$: Allows us to approximate and neglect lower-order terms.

---

### Sequential Part Analysis

Sieving up to ${\sqrt{n}}$:

- Number of operations:

${O(\sqrt{n} \log \log \sqrt{n})}$

- Justification:
    - Similar to the full sieve but limited to ${\sqrt{n}}$.
    - The sum ${\sum_{p \leq \sqrt{n}} \frac{1}{p}}$ approximates ${\log \log \sqrt{n}}$.

---

### Parallel Part Analysis

**Work per Processor**:

- Each processor sieves a segment of size: ${s \approx \frac{n}{P}}$
- For each small prime ${p}$, the number of multiples in a segment is: ${\frac{s}{p}}$
- Total Operations per Processor: ${T_\text{processor} = \sum_{p \leq \sqrt{n}} \frac{s}{p} = s \sum_{p \leq \sqrt{n}} \frac{1}{p}}$
- Substituting ${s \approx \frac{n}{P}}$: ${T_\text{processor} = \frac{n}{P} \cdot \log \log \sqrt{n}}$

---

### Total Time Including Overheads

Overheads include:

- **Communication Time** ${T_\text{comm}}$: In shared-memory systems, this is negligible. In distributed systems, it includes broadcasting the small primes and gathering results.
- **Synchronization Time** ${T_\text{sync}}$: Minimal if processors work independently, but may include barriers at the start and end.

Adjusted Total Time:

${T_\text{total} = T_\text{small} + T_\text{comm} + T_\text{sync} + T_\text{processor}}$

Since ${T_\text{comm}}$ and ${T_\text{sync}}$ are generally much smaller than ${T_\text{processor}}$, they can be considered negligible.

---

## Speedup and Efficiency

### Speedup ${S}$

**Definition**:

${S = \frac{T_\text{sequential}}{T_\text{parallel}}}$

**Approximate Speedup**:

${S \approx \frac{n \log \log n}{\frac{n}{P} \log \log n} = P}$

**Interpretation**:

- Ideal linear speedup, suggesting the parallel algorithm is ${P}$ times faster than the sequential one.
- In practice, speedup is less due to overheads.

---

### Efficiency ${E}$

**Definition**:

${E = \frac{S}{P} = \frac{T_\text{sequential}}{P \cdot T_\text{parallel}}}$

**Approximate Efficiency**:

${E \approx 1}$

**Interpretation**:

- High efficiency indicates processors are effectively utilized.
- Efficiency decreases if overheads become significant.

---

## Amdahl's Law Considerations

Amdahl's Law provides the theoretical limit of speedup for parallel computing, considering the fraction of the algorithm that is sequential.

**Formula**:

${S_\text{max} = \frac{1}{(1 - \alpha) + \frac{\alpha}{P}}}$

Where:

- ${\alpha}$ is the parallelizable fraction of the algorithm.
- ${(1 - \alpha)}$ is the sequential fraction.

---

**Applying to the Sieve**:

- **Sequential Fraction**: Computing small primes up to ${\sqrt{n}}$.
    
    Fraction:
    
    ${f_\text{seq} = \frac{T_\text{small}}{T_\text{sequential}} = \frac{\sqrt{n} \log \log \sqrt{n}}{n \log \log n}}$
    
    For large ${n}$, ${f_\text{seq}}$ approaches ${0}$.
    
- **Parallel Fraction**:
    
    ${\alpha = 1 - f_\text{seq} \approx 1}$
    
- **Maximum Speedup**:
    
    ${S_\text{max} \approx P}$
    

---

## Factors Affecting Practical Time Complexity

1. **Overheads**:
    - **Communication Overhead**:
        - Shared Memory: Minimal.
        - Distributed Memory: Increases with ${P}$ and the amount of data exchanged.
    - **Synchronization Overhead**:
        - Barrier synchronization can cause delays if processors finish at different times.
2. **Load Balancing**:
    - Equal Segments: Assumed equal workload, but in practice, some segments may have more work due to the distribution of primes.
    - Dynamic Load Balancing: Can redistribute work among processors.
3. **Memory Bandwidth and Cache Effects**:
    - Memory Contention: Multiple processors accessing memory simultaneously can saturate bandwidth.
    - Cache Performance: Proper segmentation improves cache utilization.

---

## Conclusion

**Parallel Time Complexity**:

${T_\text{parallel} = O\left(\frac{n}{P} \log \log n\right)}$

**Speedup and Efficiency**:

- Near-linear speedup is achievable for large ${n}$ and reasonable ${P}$.
- Efficiency remains high when overheads are minimized.

**Practical Considerations**:

- Overheads and non-parallelizable portions limit the maximum achievable speedup.
- Proper implementation and optimization are crucial for realizing theoretical improvements.
