// Daily Quant Digest 2026-11-05
// FR Lesson 10: binary search.
// Problem: in a sorted (non-decreasing) list of trade prices, find the first
// and last position of a target price, or {-1, -1} if it never traded.
// Compile: g++ -std=c++17 -O2 -Wall 2026-11-05.cpp -o bs && ./bs

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

// The one binary search we ever need:
// "pred" is false, false, ..., false, true, true, ..., true over [lo, hi).
// Return the index of the first true, or hi if there is none.
template <typename Pred>
int firstTrue(int lo, int hi, Pred pred) {
    while (lo < hi) {                    // the answer lives in [lo, hi]
        int mid = lo + (hi - lo) / 2;    // no overflow, and mid < hi
        if (pred(mid)) {
            hi = mid;                    // mid might be the first true: keep it
        } else {
            lo = mid + 1;                // mid is false: the answer is to its right
        }
    }
    return lo;                           // lo == hi: the fence post
}

vector<int> searchRange(const vector<int>& prices, int target) {
    const int n = static_cast<int>(prices.size());

    // first index with prices[i] >= target
    int first = firstTrue(0, n, [&](int i) { return prices[i] >= target; });
    if (first == n || prices[first] != target) {
        return {-1, -1};                 // target never traded
    }
    // first index with prices[i] > target, then step back one
    int afterLast = firstTrue(0, n, [&](int i) { return prices[i] > target; });
    return {first, afterLast - 1};
}

// Same answer using the standard library.
vector<int> searchRangeSTL(const vector<int>& prices, int target) {
    auto [lo, hi] = equal_range(prices.begin(), prices.end(), target);
    if (lo == hi) return {-1, -1};
    int first = static_cast<int>(lo - prices.begin());
    int last  = static_cast<int>(hi - prices.begin()) - 1;
    return {first, last};
}

// Brute force for testing: O(n) scan.
vector<int> searchRangeBrute(const vector<int>& prices, int target) {
    int first = -1, last = -1;
    for (int i = 0; i < static_cast<int>(prices.size()); ++i) {
        if (prices[i] == target) {
            if (first == -1) first = i;
            last = i;
        }
    }
    return {first, last};
}

void show(const vector<int>& v) {
    cout << "[" << v[0] << ", " << v[1] << "]\n";
}

int main() {
    // Example 1
    vector<int> p1 = {101, 102, 102, 102, 105, 107, 107, 110};
    show(searchRange(p1, 102));   // [1, 3]
    show(searchRange(p1, 107));   // [5, 6]
    show(searchRange(p1, 104));   // [-1, -1]
    // Example 2: edges
    vector<int> p2 = {};
    show(searchRange(p2, 5));     // [-1, -1]
    vector<int> p3 = {7, 7, 7, 7};
    show(searchRange(p3, 7));     // [0, 3]
    show(searchRange(p3, 8));     // [-1, -1]
    show(searchRange(p3, 6));     // [-1, -1]

    assert((searchRange(p1, 102) == vector<int>{1, 3}));
    assert((searchRange(p1, 107) == vector<int>{5, 6}));
    assert((searchRange(p1, 104) == vector<int>{-1, -1}));
    assert((searchRange(p2, 5) == vector<int>{-1, -1}));
    assert((searchRange(p3, 7) == vector<int>{0, 3}));
    assert((searchRange(p3, 8) == vector<int>{-1, -1}));
    assert((searchRange(p3, 6) == vector<int>{-1, -1}));

    // Randomized cross-check against brute force and the STL version.
    mt19937 rng(20261105);
    for (int t = 0; t < 200000; ++t) {
        int n = rng() % 30;
        int range = 1 + rng() % 10;
        vector<int> a(n);
        for (int& x : a) x = static_cast<int>(rng() % range);
        sort(a.begin(), a.end());
        int target = static_cast<int>(rng() % (range + 2)) - 1;
        auto b = searchRangeBrute(a, target);
        assert(searchRange(a, target) == b);
        assert(searchRangeSTL(a, target) == b);
    }

    // Large values near the int limits.
    vector<int> big = {-2147483647 - 1, -5, 0, 2147483647, 2147483647};
    assert((searchRange(big, 2147483647) == vector<int>{3, 4}));
    assert((searchRange(big, -2147483647 - 1) == vector<int>{0, 0}));

    // Count of steps for one million prices.
    vector<int> million(1000000);
    for (int i = 0; i < 1000000; ++i) million[i] = i / 3;   // each price 3 times
    int steps = 0;
    firstTrue(0, 1000000, [&](int i) { ++steps; return million[i] >= 123456; });
    show(searchRange(million, 123456));                      // [370368, 370370]
    cout << "steps for n = 1,000,000: " << steps << "\n";    // 19 or 20

    cout << "All tests passed.\n";
    return 0;
}
