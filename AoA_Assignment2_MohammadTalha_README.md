# Analysis of Algorithms — Assignment 2: Empirical Analysis of Sorting Algorithms

**Student Name:** Mohammad Talha  
**SAP ID:** 64932  
**Department:** Software Engineering  
**Course:** Analysis of Algorithms  
**GitHub Repository:** https://github.com/talha64932/AoA_Assignment2_MohammadTalha  

---

## Overview

This repository contains a C++17 empirical benchmark of four foundational sorting algorithms written entirely from scratch (no `std::sort`):

1. **Bubble Sort** (with early-exit optimization)
2. **Selection Sort**
3. **Insertion Sort**
4. **Merge Sort** (top-down using a single pre-allocated auxiliary buffer)

All four algorithms are evaluated on the four specified ascending datasets (`Arr1`–`Arr4` of sizes `N = 5, 10, 50, 100`) as well as reverse-sorted worst-case datasets of the same sizes. After an initial warmup run, each configuration is timed across **10 independent trials** using `std::chrono::steady_clock` on fresh array copies, and every sorted output is programmatically verified.

---

## Repository Structure

```text
AoA_Assignment2_MohammadTalha/
├── src/
│   └── sorting_analysis.cpp                # C++17 implementations & timing benchmark
├── results/
│   ├── timings_summary.csv                 # Mean execution times across 10 trials
│   ├── time_vs_size_charts.png             # Time vs. Input Size (N) scaling graphs
│   └── terminal_output.png                 # Build and execution output capture
├── report/
│   └── 64932_MohammadTalha_Assignment2.pdf # Complete assignment report
├── .gitignore
└── README.md
```

---

## Build & Run

Requires a C++17 compiler (`g++` or `clang++`):

```bash
g++ -std=c++17 -O2 src/sorting_analysis.cpp -o sorting_analysis
./sorting_analysis
./sorting_analysis > results/timings_summary.csv
```

Windows (MinGW):
```powershell
g++ -std=c++17 -O2 src\sorting_analysis.cpp -o sorting_analysis.exe
.\sorting_analysis.exe
```
