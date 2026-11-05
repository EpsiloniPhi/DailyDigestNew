// Daily Quant Digest 2026-11-06
// FR Lesson 11: binary search on the answer.
// Problem: the slowest execution rate that still finishes every order in time.
//
// You must work n block orders with sizes q[0..n-1] (in lots) within H hours.
// Each hour the algo works ONE order and executes up to k lots of it. If the
// order has fewer than k lots left, it finishes that order and stays idle for
// the rest of the hour. Find the smallest integer k that finishes everything
// within H hours. (n <= H is guaranteed, so k = max(q) always works.)
//
// Compile: g++ -std=c++17 -O2 -Wall 2026-11-06.cpp && ./a.out

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

// Hours needed at rate k: each order of size x takes ceil(x / k) hours.
// The total can exceed the int range (1e5 orders x 1e9 hours), so use long long.
long long hoursNeeded(const vector<int>& q, long long k) {
    long long total = 0;
    for (int x : q) {
        total += (x + k - 1) / k;          // ceil(x / k) in integer arithmetic
    }
    return total;
}

// Binary search on the answer: the fence "hoursNeeded(k) <= H" is
// false, ..., false, true, ..., true as k grows. Find the first true.
int minRate(const vector<int>& q, long long H) {
    long long lo = 1;
    long long hi = *max_element(q.begin(), q.end());   // always fast enough
    while (lo < hi) {                       // the answer lives in [lo, hi]
        long long mid = lo + (hi - lo) / 2;
        if (hoursNeeded(q, mid) <= H) {
            hi = mid;                       // mid is fast enough: keep it
        } else {
            lo = mid + 1;                   // too slow: answer is above mid
        }
    }
    return static_cast<int>(lo);
}

// Brute force: try k = 1, 2, 3, ... until it is fast enough.
int minRateBrute(const vector<int>& q, long long H) {
    for (long long k = 1;; ++k) {
        if (hoursNeeded(q, k) <= H) return static_cast<int>(k);
    }
}

int main() {
    // Examples from the digest.
    struct Case { vector<int> q; long long H; int expected; };
    vector<Case> cases = {
        {{3, 6, 7, 11}, 8, 4},
        {{30, 11, 23, 4, 20}, 5, 30},
        {{30, 11, 23, 4, 20}, 6, 23},
        {{9, 12, 5}, 10, 3},
        {{1000000000}, 2, 500000000},
        {{1000000000, 1000000000}, 3, 1000000000},
        {{5}, 1000000000, 1},
    };
    for (const auto& c : cases) {
        int got = minRate(c.q, c.H);
        cout << "H = " << c.H << ", answer k = " << got
             << " (expected " << c.expected << ")\n";
        assert(got == c.expected);
    }

    // Trace of hoursNeeded for the first example.
    vector<int> ex = {3, 6, 7, 11};
    for (int k = 1; k <= 11; ++k) {
        cout << "k = " << k << ": hours = " << hoursNeeded(ex, k) << "\n";
    }

    // Randomized cross-check against brute force.
    mt19937 rng(20261106);
    for (int t = 0; t < 100000; ++t) {
        int n = uniform_int_distribution<int>(1, 8)(rng);
        vector<int> q(n);
        for (int& x : q) x = uniform_int_distribution<int>(1, 60)(rng);
        long long sum = 0;
        for (int x : q) sum += x;
        long long H = uniform_int_distribution<long long>(n, sum + 5)(rng);
        assert(minRate(q, H) == minRateBrute(q, H));
    }

    // Big case: 100,000 orders of 1e9 lots, H = 1e9 hours.
    // hoursNeeded(1) would be 1e14, far beyond int: long long matters.
    vector<int> big(100000, 1000000000);
    int kBig = minRate(big, 1000000000LL);
    cout << "big case: k = " << kBig << ", hours = " << hoursNeeded(big, kBig)
         << ", hours at k-1 = " << hoursNeeded(big, kBig - 1) << "\n";
    assert(hoursNeeded(big, kBig) <= 1000000000LL);
    assert(hoursNeeded(big, kBig - 1) > 1000000000LL);

    cout << "All tests passed.\n";
    return 0;
}
