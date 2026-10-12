// Daily Quant Digest 2026-10-13
// Problem: count the contiguous windows of daily P&L whose total is exactly K
// (LeetCode 560, "Subarray Sum Equals K").
// Compile: g++ -std=c++17 -O2 -Wall 2026-10-13.cpp -o sol && ./sol

#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>

// Optimal O(n) solution: prefix sums + hash map of prefix-sum frequencies.
long long countWindows(const std::vector<int>& pnl, long long k) {
    std::unordered_map<long long, long long> seen;  // prefix sum -> how many times seen
    seen.reserve(pnl.size() * 2 + 1);
    seen[0] = 1;                                     // the empty prefix

    long long prefix = 0;
    long long count = 0;
    for (int x : pnl) {
        prefix += x;
        auto it = seen.find(prefix - k);             // earlier prefixes P[i] with P[j]-P[i]=k
        if (it != seen.end()) count += it->second;
        ++seen[prefix];                              // record current prefix AFTER the lookup
    }
    return count;
}

// O(n^2) brute force, used only to cross-check the fast version.
long long countWindowsBrute(const std::vector<int>& pnl, long long k) {
    long long count = 0;
    const int n = static_cast<int>(pnl.size());
    for (int i = 0; i < n; ++i) {
        long long sum = 0;
        for (int j = i; j < n; ++j) {
            sum += pnl[j];
            if (sum == k) ++count;
        }
    }
    return count;
}

int main() {
    struct Test {
        std::vector<int> pnl;
        long long k;
        long long expected;
    };
    const std::vector<Test> tests = {
        {{1, 1, 1}, 2, 2},
        {{1, 2, 3}, 3, 2},
        {{3, -1, 2, -2, 4, -3}, 2, 4},
        {{0, 0, 0}, 0, 6},
        {{5}, 5, 1},
        {{-1, -1, 1}, 0, 1},
    };

    bool ok = true;
    for (const auto& t : tests) {
        const long long got = countWindows(t.pnl, t.k);
        std::cout << "k=" << t.k << " -> " << got << " (expected " << t.expected << ")"
                  << (got == t.expected ? "" : "  MISMATCH") << '\n';
        ok = ok && (got == t.expected);
    }

    // Randomised cross-check against brute force.
    std::mt19937 rng(20261013);
    std::uniform_int_distribution<int> len(0, 60), val(-5, 5), kd(-8, 8);
    for (int trial = 0; trial < 2000; ++trial) {
        std::vector<int> a(len(rng));
        for (auto& x : a) x = val(rng);
        const long long k = kd(rng);
        if (countWindows(a, k) != countWindowsBrute(a, k)) {
            std::cout << "Random test failed on trial " << trial << '\n';
            ok = false;
            break;
        }
    }

    std::cout << (ok ? "All tests passed." : "Some tests FAILED.") << '\n';
    return ok ? 0 : 1;
}
