// Daily Quant Digest 2026-10-28, FR Lesson 3 (Algorithms): arrays, scanning, in-place updates.
// Problem: Next Permutation. Rearrange the numbers into the next larger
// arrangement in dictionary order; if there is none, wrap around to the smallest.
// Compile: g++ -std=c++17 -O2 -Wall 2026-10-28.cpp
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

// Brute force: list every arrangement in dictionary order, find ours, take the next.
// O(n! * n) time and memory. Only usable for tiny n (n <= 8 or so).
std::vector<int> nextPermutationBrute(const std::vector<int>& a) {
    std::vector<int> sorted = a;
    std::sort(sorted.begin(), sorted.end());
    std::vector<std::vector<int>> all;               // every distinct arrangement, in order
    do {
        all.push_back(sorted);
    } while (std::next_permutation(sorted.begin(), sorted.end()));
    for (std::size_t k = 0; k < all.size(); ++k) {
        if (all[k] == a) return all[(k + 1) % all.size()];  // last one wraps to first
    }
    return a;  // never reached
}

// The real solution: O(n) time, O(1) extra space, edits the caller's vector.
void nextPermutation(std::vector<int>& a) {
    const int n = static_cast<int>(a.size());
    // Step 1: scan from the right while the numbers are going UP (reading right to left).
    // Stop at the first number that is smaller than its right neighbour: the pivot.
    int i = n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) --i;
    if (i >= 0) {
        // Step 2: in the tail a[i+1..n-1] (which is non-increasing), find the
        // rightmost number that is strictly bigger than the pivot.
        int j = n - 1;
        while (a[j] <= a[i]) --j;
        // Step 3: swap them. The pivot position now holds the next bigger value.
        std::swap(a[i], a[j]);
    }
    // Step 4: the tail is still non-increasing; reverse it to make it as small as possible.
    // If there was no pivot (i == -1) this reverses the whole array: wrap to the smallest.
    std::reverse(a.begin() + (i + 1), a.end());
}

void print(const std::vector<int>& a) {
    std::cout << "[";
    for (std::size_t k = 0; k < a.size(); ++k) std::cout << (k ? " " : "") << a[k];
    std::cout << "]";
}

int main() {
    struct Case { std::vector<int> input; std::vector<int> expected; };
    const std::vector<Case> cases = {
        {{1, 3, 5, 4, 2}, {1, 4, 2, 3, 5}},
        {{1, 2, 3}, {1, 3, 2}},
        {{3, 2, 1}, {1, 2, 3}},
        {{1, 1, 5}, {1, 5, 1}},
        {{2, 3, 3, 1}, {3, 1, 2, 3}},
        {{7}, {7}},
    };
    for (const auto& c : cases) {
        std::vector<int> a = c.input;   // copy, because nextPermutation edits it
        nextPermutation(a);
        print(c.input); std::cout << " -> "; print(a); std::cout << "\n";
        assert(a == c.expected);
        assert(nextPermutationBrute(c.input) == c.expected);
    }

    // Walk through all 4! = 24 arrangements of {1,2,3,4}: after 24 steps we are back home.
    std::vector<int> w = {1, 2, 3, 4};
    int steps = 0;
    do { nextPermutation(w); ++steps; } while (w != std::vector<int>{1, 2, 3, 4});
    std::cout << "Cycle length for 4 distinct numbers: " << steps << "\n";
    assert(steps == 24);

    // Randomized cross-check against the brute force and against std::next_permutation.
    std::mt19937 rng(20261028);
    for (int t = 0; t < 50000; ++t) {
        int n = 1 + static_cast<int>(rng() % 7);         // 1..7 numbers
        std::vector<int> a(n);
        for (int& x : a) x = static_cast<int>(rng() % 4); // small values: many duplicates
        std::vector<int> mine = a;
        nextPermutation(mine);
        std::vector<int> lib = a;
        std::next_permutation(lib.begin(), lib.end());   // also wraps to sorted at the end
        assert(mine == lib);
        if (t < 5000) assert(mine == nextPermutationBrute(a));
    }
    // Large input: O(n) is instant even for n = 100,000.
    std::vector<int> big(100000);
    for (int k = 0; k < 100000; ++k) big[k] = 100000 - k;  // fully decreasing: worst case
    nextPermutation(big);
    assert(std::is_sorted(big.begin(), big.end()));
    std::cout << "All tests passed (6 examples, 50,000 random checks, n = 100,000).\n";
    return 0;
}
