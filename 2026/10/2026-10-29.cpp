// Daily Quant Digest 2026-10-29: Longest Consecutive Sequence (hash set, O(n) average)
// Compile: g++ -std=c++17 -O2 -Wall 2026-10-29.cpp && ./a.out
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>

// Optimal: put every number in a hash set, then start counting only at the
// first number of each run (a number x whose left neighbour x-1 is missing).
// Values are within [-1e9, 1e9], so x - 1 and x + length never overflow an int.
int longestConsecutive(const std::vector<int>& nums) {
    std::unordered_set<int> present(nums.begin(), nums.end());  // duplicates vanish
    int best = 0;
    for (int x : present) {
        if (present.count(x - 1)) continue;   // not the start of a run: skip
        int length = 1;                       // x starts a run: walk right
        while (present.count(x + length)) ++length;
        best = std::max(best, length);
    }
    return best;
}

// Brute force for checking: sort, remove duplicates, then scan once.
int longestBySorting(std::vector<int> nums) {       // by value: we sort our own copy
    if (nums.empty()) return 0;
    std::sort(nums.begin(), nums.end());
    nums.erase(std::unique(nums.begin(), nums.end()), nums.end());
    int best = 1, run = 1;
    for (std::size_t i = 1; i < nums.size(); ++i) {
        if (nums[i] == nums[i - 1] + 1) ++run;
        else run = 1;
        best = std::max(best, run);
    }
    return best;
}

int main() {
    // Examples from the problem statement
    struct Case { std::vector<int> nums; int expected; };
    std::vector<Case> cases = {
        {{100, 4, 200, 1, 3, 2}, 4},
        {{0, 3, 7, 2, 5, 8, 4, 6, 0, 1}, 9},
        {{}, 0},
        {{1, 2, 0, 1}, 3},
        {{-1000000000, 1000000000, 999999999, -999999999}, 2},
    };
    for (const auto& c : cases) {
        int got = longestConsecutive(c.nums);
        std::cout << "answer = " << got << " (expected " << c.expected << ")\n";
        assert(got == c.expected);
        assert(longestBySorting(c.nums) == c.expected);
    }

    // Randomized cross-check against the sorting version
    std::mt19937 rng(20261029);
    for (int trial = 0; trial < 20000; ++trial) {
        int n = rng() % 30;
        int range = 1 + rng() % 40;
        std::vector<int> nums(n);
        for (int& x : nums) x = static_cast<int>(rng() % range) - range / 2;
        assert(longestConsecutive(nums) == longestBySorting(nums));
    }

    // A large test: 1,000,000 numbers, a shuffled run of 0..999,999
    std::vector<int> big(1000000);
    for (int i = 0; i < 1000000; ++i) big[i] = i;
    std::shuffle(big.begin(), big.end(), rng);
    assert(longestConsecutive(big) == 1000000);

    std::cout << "All tests passed.\n";
    return 0;
}
