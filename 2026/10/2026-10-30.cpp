// Daily Quant Digest 2026-10-30: Prefix sums.
// Problem: longest stretch of consecutive days whose total P&L is exactly k.
// Compile: g++ -std=c++17 -O2 -Wall 2026-10-30.cpp && ./a.out
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>

// Warm-up: build the prefix array. prefix[i] = a[0] + ... + a[i-1],
// so prefix[0] = 0 and the total of days l..r (inclusive) is prefix[r+1] - prefix[l].
std::vector<long long> buildPrefix(const std::vector<long long>& a) {
    std::vector<long long> prefix(a.size() + 1, 0);
    for (std::size_t i = 0; i < a.size(); ++i) {
        prefix[i + 1] = prefix[i] + a[i];
    }
    return prefix;
}

long long rangeSum(const std::vector<long long>& prefix, std::size_t l, std::size_t r) {
    return prefix[r + 1] - prefix[l];  // O(1) per query
}

// Brute force: try every start, extend the end, keep a running total. O(n^2).
int longestBrute(const std::vector<long long>& a, long long k) {
    int best = 0;
    const int n = static_cast<int>(a.size());
    for (int i = 0; i < n; ++i) {
        long long sum = 0;
        for (int j = i; j < n; ++j) {
            sum += a[j];
            if (sum == k) best = std::max(best, j - i + 1);
        }
    }
    return best;
}

// Optimal: one pass with a running total and a hash map that remembers the
// FIRST day each running total was seen. O(n) average time, O(n) space.
int longestWithSumK(const std::vector<long long>& a, long long k) {
    std::unordered_map<long long, int> firstSeen;
    firstSeen.reserve(a.size() * 2 + 1);
    firstSeen[0] = -1;  // the empty start: total 0 "before day 0"

    long long running = 0;
    int best = 0;
    for (int i = 0; i < static_cast<int>(a.size()); ++i) {
        running += a[i];

        // A stretch ending today with total k needs an earlier running total of running - k.
        auto it = firstSeen.find(running - k);
        if (it != firstSeen.end()) {
            best = std::max(best, i - it->second);
        }

        // Keep only the first time we see this total: the earliest start gives the longest stretch.
        firstSeen.emplace(running, i);  // emplace does nothing if the key is already there
    }
    return best;
}

int main() {
    // Warm-up check of range sums.
    std::vector<long long> pnl = {2, -1, -1, 3, -3, 1};
    auto prefix = buildPrefix(pnl);
    assert(rangeSum(prefix, 0, 5) == 1);
    assert(rangeSum(prefix, 1, 3) == 1);
    assert(rangeSum(prefix, 3, 4) == 0);

    struct Case { std::vector<long long> a; long long k; int expected; };
    std::vector<Case> cases = {
        {{2, -1, -1, 3, -3, 1}, 0, 5},
        {{1, -1, 5, -2, 3}, 3, 4},
        {{4, 2}, 1, 0},
        {{-2, -1, 2, 1}, 1, 2},
        {{0, 0, 0}, 0, 3},
    };
    for (const auto& c : cases) {
        int got = longestWithSumK(c.a, c.k);
        std::cout << "answer = " << got << " (expected " << c.expected << ")\n";
        assert(got == c.expected);
        assert(longestBrute(c.a, c.k) == c.expected);
    }

    // Randomized cross-check against brute force.
    std::mt19937 rng(20261030);
    std::uniform_int_distribution<int> lenDist(0, 30), valDist(-4, 4), kDist(-6, 6);
    for (int t = 0; t < 20000; ++t) {
        std::vector<long long> a(lenDist(rng));
        for (auto& x : a) x = valDist(rng);
        long long k = kDist(rng);
        assert(longestWithSumK(a, k) == longestBrute(a, k));
    }

    // Large values: sums need long long.
    std::vector<long long> big(200000, 1000000000LL);
    assert(longestWithSumK(big, 200000LL * 1000000000LL) == 200000);

    std::cout << "All tests passed.\n";
    return 0;
}
