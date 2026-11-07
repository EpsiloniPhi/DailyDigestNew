// Daily Quant Digest 2026-11-08 (Revision Day 2)
// Shortest run of trading days whose total volume reaches a target.
// Two solutions: sliding window O(n), and binary search on the answer O(n log n).
// Compile: g++ -std=c++17 -O2 -Wall 2026-11-08.cpp
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

// Solution 1: sliding window (two pointers moving the same way).
// Returns the length of the shortest window with sum >= target, or 0 if none.
int shortestRunWindow(const vector<int>& vol, long long target) {
    const int n = static_cast<int>(vol.size());
    int best = n + 1;          // "infinity": longer than any real window
    long long sum = 0;         // sum of vol[left .. right]
    int left = 0;
    for (int right = 0; right < n; ++right) {
        sum += vol[right];                     // grow: take one more day
        while (sum >= target) {                // window is good
            best = min(best, right - left + 1);// record it
            sum -= vol[left];                  // shrink: drop the oldest day
            ++left;
        }
    }
    return best == n + 1 ? 0 : best;
}

// Solution 2: binary search on the answer, using prefix sums.
// Does some window of length len reach the target?
bool someWindowReaches(const vector<long long>& prefix, int len, long long target) {
    const int n = static_cast<int>(prefix.size()) - 1;
    for (int start = 0; start + len <= n; ++start) {
        if (prefix[start + len] - prefix[start] >= target) return true;
    }
    return false;
}

int shortestRunBinarySearch(const vector<int>& vol, long long target) {
    const int n = static_cast<int>(vol.size());
    vector<long long> prefix(n + 1, 0);        // prefix[i] = vol[0] + ... + vol[i-1]
    for (int i = 0; i < n; ++i) prefix[i + 1] = prefix[i] + vol[i];

    if (prefix[n] < target) return 0;          // even all days together fall short

    // Red/green fence on len = 1..n: find the first len that is "green".
    int lo = 1, hi = n;                        // the answer is in [lo, hi]
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (someWindowReaches(prefix, mid, target)) hi = mid;  // mid works, try shorter
        else lo = mid + 1;                                     // mid fails, go longer
    }
    return lo;
}

// Brute force for testing: try every start and every end. O(n^2).
int shortestRunBrute(const vector<int>& vol, long long target) {
    const int n = static_cast<int>(vol.size());
    int best = 0;
    for (int i = 0; i < n; ++i) {
        long long sum = 0;
        for (int j = i; j < n; ++j) {
            sum += vol[j];
            if (sum >= target) {
                int len = j - i + 1;
                if (best == 0 || len < best) best = len;
                break;                         // longer windows from i cannot be shorter
            }
        }
    }
    return best;
}

int main() {
    struct Case { vector<int> vol; long long target; int expected; };
    const vector<Case> cases = {
        {{2, 3, 1, 2, 4, 3}, 7, 2},
        {{1, 4, 4}, 4, 1},
        {{1, 1, 1, 1, 1, 1, 1, 1}, 11, 0},
        {{5, 1, 3, 5, 10, 7, 4, 9, 2, 8}, 15, 2},
        {{10000, 10000, 10000}, 30000, 3},
    };
    for (const auto& c : cases) {
        int a = shortestRunWindow(c.vol, c.target);
        int b = shortestRunBinarySearch(c.vol, c.target);
        cout << "target " << c.target << ": window " << a << ", binary search " << b
             << " (expected " << c.expected << ")\n";
        assert(a == c.expected && b == c.expected);
    }

    // Randomized cross-check against brute force.
    mt19937 rng(20261108);
    for (int iter = 0; iter < 20000; ++iter) {
        int n = uniform_int_distribution<int>(1, 30)(rng);
        int maxV = uniform_int_distribution<int>(1, 20)(rng);
        vector<int> vol(n);
        for (int& v : vol) v = uniform_int_distribution<int>(1, maxV)(rng);
        long long target = uniform_int_distribution<long long>(1, 300)(rng);
        int expected = shortestRunBrute(vol, target);
        assert(shortestRunWindow(vol, target) == expected);
        assert(shortestRunBinarySearch(vol, target) == expected);
    }

    // Large case: overflow check with 100,000 days of volume 10,000 (total 1e9).
    vector<int> big(100000, 10000);
    assert(shortestRunWindow(big, 1000000000LL) == 100000);
    assert(shortestRunBinarySearch(big, 1000000000LL) == 100000);

    cout << "All tests passed.\n";
    return 0;
}
