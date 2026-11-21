/*
 * Course     : Analysis of Algorithms (Assignment 2)
 * Student    : Mohammad Talha
 * SAP ID     : 64932
 * Department : Software Engineering
 * Repository : https://github.com/talha64932/AoA_Assignment2_MohammadTalha
 *
 * Build : g++ -std=c++17 -O2 src/sorting_analysis.cpp -o sorting_analysis
 * Run   : ./sorting_analysis
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <string>
#include <utility>

// 1. Bubble Sort with early-termination check
void sortBubble(std::vector<int>& nums) {
    const size_t total = nums.size();
    for (size_t pass = 0; pass + 1 < total; ++pass) {
        bool swappedAny = false;
        for (size_t idx = 0; idx + 1 < total - pass; ++idx) {
            if (nums[idx] > nums[idx + 1]) {
                std::swap(nums[idx], nums[idx + 1]);
                swappedAny = true;
            }
        }
        if (!swappedAny) {
            break;
        }
    }
}

// 2. Selection Sort (in-place minimum scan)
void sortSelection(std::vector<int>& nums) {
    const size_t total = nums.size();
    for (size_t pos = 0; pos + 1 < total; ++pos) {
        size_t bestIdx = pos;
        for (size_t scan = pos + 1; scan < total; ++scan) {
            if (nums[scan] < nums[bestIdx]) {
                bestIdx = scan;
            }
        }
        if (bestIdx != pos) {
            std::swap(nums[pos], nums[bestIdx]);
        }
    }
}

// 3. Insertion Sort (shift-based in-place insertion)
void sortInsertion(std::vector<int>& nums) {
    const size_t total = nums.size();
    for (size_t i = 1; i < total; ++i) {
        int current = nums[i];
        size_t j = i;
        while (j > 0 && nums[j - 1] > current) {
            nums[j] = nums[j - 1];
            --j;
        }
        nums[j] = current;
    }
}

// 4. Merge Sort (top-down using a single shared auxiliary buffer)
void mergeRange(std::vector<int>& nums, std::vector<int>& scratch, int lo, int mid, int hi) {
    int leftPtr = lo;
    int rightPtr = mid + 1;
    int outPtr = lo;

    while (leftPtr <= mid && rightPtr <= hi) {
        if (nums[leftPtr] <= nums[rightPtr]) {
            scratch[outPtr++] = nums[leftPtr++];
        } else {
            scratch[outPtr++] = nums[rightPtr++];
        }
    }
    while (leftPtr <= mid) {
        scratch[outPtr++] = nums[leftPtr++];
    }
    while (rightPtr <= hi) {
        scratch[outPtr++] = nums[rightPtr++];
    }
    for (int k = lo; k <= hi; ++k) {
        nums[k] = scratch[k];
    }
}

void mergeDivide(std::vector<int>& nums, std::vector<int>& scratch, int lo, int hi) {
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    mergeDivide(nums, scratch, lo, mid);
    mergeDivide(nums, scratch, mid + 1, hi);
    mergeRange(nums, scratch, lo, mid, hi);
}

void sortMerge(std::vector<int>& nums) {
    if (nums.size() <= 1) return;
    std::vector<int> scratch(nums.size());
    mergeDivide(nums, scratch, 0, static_cast<int>(nums.size()) - 1);
}

// Helper: verify ascending order
bool checkAscending(const std::vector<int>& nums) {
    for (size_t i = 1; i < nums.size(); ++i) {
        if (nums[i - 1] > nums[i]) return false;
    }
    return true;
}

// Helper: build ascending or descending sequence 1..N
std::vector<int> createRange(int count, bool descending) {
    std::vector<int> seq(count);
    for (int i = 0; i < count; ++i) {
        seq[i] = descending ? (count - i) : (i + 1);
    }
    return seq;
}

struct Dataset {
    std::string label;
    std::string orderType;
    std::vector<int> values;
};

struct AlgorithmSpec {
    std::string name;
    void (*sortFn)(std::vector<int>&);
};

double measureAverageMicros(void (*sortFn)(std::vector<int>&),
                            const std::vector<int>& source,
                            int trials) {
    // Warmup execution to stabilize CPU cache and branch prediction
    {
        std::vector<int> warmup = source;
        sortFn(warmup);
        if (!checkAscending(warmup)) {
            std::cerr << "Error: Warmup verification failed!\n";
            std::exit(1);
        }
    }

    double accumulated = 0.0;
    for (int t = 0; t < trials; ++t) {
        std::vector<int> working = source;
        auto start = std::chrono::steady_clock::now();
        sortFn(working);
        auto stop = std::chrono::steady_clock::now();

        accumulated += std::chrono::duration<double, std::micro>(stop - start).count();

        if (!checkAscending(working)) {
            std::cerr << "Error: Sorting verification failed!\n";
            std::exit(1);
        }
    }
    return accumulated / static_cast<double>(trials);
}

int main() {
    constexpr int NUM_TRIALS = 10;

    // Given sorted datasets Arr1..Arr4
    std::vector<int> Arr1 = {1, 2, 3, 4, 5};
    std::vector<int> Arr2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> Arr3 = createRange(50, false);
    std::vector<int> Arr4 = createRange(100, false);

    // Build complete test suite (Given Sorted + Reverse Worst-Case)
    std::vector<Dataset> datasets = {
        {"Arr1", "Given",   Arr1},
        {"Arr2", "Given",   Arr2},
        {"Arr3", "Given",   Arr3},
        {"Arr4", "Given",   Arr4},
        {"Rev1", "Reverse", createRange(5, true)},
        {"Rev2", "Reverse", createRange(10, true)},
        {"Rev3", "Reverse", createRange(50, true)},
        {"Rev4", "Reverse", createRange(100, true)}
    };

    std::vector<AlgorithmSpec> algorithms = {
        {"Bubble Sort",    sortBubble},
        {"Selection Sort", sortSelection},
        {"Insertion Sort", sortInsertion},
        {"Merge Sort",     sortMerge}
    };

    std::cout << "Algorithm,OrderType,Dataset,InputSize,AvgTime_us\n";
    for (const auto& algo : algorithms) {
        for (const auto& ds : datasets) {
            double avgUs = measureAverageMicros(algo.sortFn, ds.values, NUM_TRIALS);
            std::cout << algo.name << ","
                      << ds.orderType << ","
                      << ds.label << ","
                      << ds.values.size() << ","
                      << std::fixed << std::setprecision(3) << avgUs << "\n";
        }
    }

    return 0;
}
