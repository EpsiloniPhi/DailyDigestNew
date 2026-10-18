// Daily Quant Digest 2026-10-19
// Algorithms & DS: Intervals and sweep line, "The Skyline Problem"
// Compile: g++ -std=c++17 -O2 -Wall 2026-10-19.cpp -o skyline && ./skyline

#include <algorithm>
#include <cassert>
#include <iostream>
#include <queue>
#include <random>
#include <set>
#include <utility>
#include <vector>

using std::vector;
using Building = std::vector<long long>;          // {left, right, height}
using Point = std::pair<long long, long long>;    // {x, height}

// ---------------------------------------------------------------------------
// Solution 1: sweep line with a multiset of active heights. O(n log n).
// ---------------------------------------------------------------------------
vector<Point> getSkyline(const vector<Building>& buildings) {
    // Each building produces two events: +h at its left edge, -h at its right edge.
    vector<Point> events;
    events.reserve(2 * buildings.size());
    for (const auto& b : buildings) {
        events.emplace_back(b[0], b[2]);    // start: positive height
        events.emplace_back(b[1], -b[2]);   // end:   negative height
    }
    // Sort by x only. All events at the same x are applied as a batch below,
    // so we do not need any clever tie-breaking rule.
    std::sort(events.begin(), events.end(),
              [](const Point& a, const Point& b) { return a.first < b.first; });

    std::multiset<long long> active{0};     // ground level is always "active"
    vector<Point> result;
    long long prevMax = 0;

    std::size_t i = 0;
    while (i < events.size()) {
        const long long x = events[i].first;
        // Apply every event that happens at this x.
        while (i < events.size() && events[i].first == x) {
            const long long h = events[i].second;
            if (h > 0) {
                active.insert(h);
            } else {
                // erase ONE copy: erase(value) would remove all equal heights
                active.erase(active.find(-h));
            }
            ++i;
        }
        const long long curMax = *active.rbegin();   // largest element
        if (curMax != prevMax) {
            result.emplace_back(x, curMax);
            prevMax = curMax;
        }
    }
    return result;
}

// ---------------------------------------------------------------------------
// Solution 2: sweep over sorted x with a max-heap and lazy deletion. O(n log n).
// Heap stores {height, right edge}; tops whose right edge <= x are stale.
// ---------------------------------------------------------------------------
vector<Point> getSkylineHeap(vector<Building> buildings) {
    std::sort(buildings.begin(), buildings.end());   // by left edge
    vector<long long> xs;
    for (const auto& b : buildings) { xs.push_back(b[0]); xs.push_back(b[1]); }
    std::sort(xs.begin(), xs.end());
    xs.erase(std::unique(xs.begin(), xs.end()), xs.end());

    std::priority_queue<Point> heap;   // max-heap on height
    vector<Point> result;
    std::size_t j = 0;
    long long prevMax = 0;
    for (long long x : xs) {
        while (j < buildings.size() && buildings[j][0] == x) {
            heap.emplace(buildings[j][2], buildings[j][1]);
            ++j;
        }
        while (!heap.empty() && heap.top().second <= x) heap.pop();   // lazy delete
        const long long curMax = heap.empty() ? 0 : heap.top().first;
        if (curMax != prevMax) {
            result.emplace_back(x, curMax);
            prevMax = curMax;
        }
    }
    return result;
}

// ---------------------------------------------------------------------------
// Brute force reference: height on each elementary interval [xs[k], xs[k+1]).
// O(n^2), used only for testing.
// ---------------------------------------------------------------------------
vector<Point> bruteForce(const vector<Building>& buildings) {
    vector<long long> xs;
    for (const auto& b : buildings) { xs.push_back(b[0]); xs.push_back(b[1]); }
    std::sort(xs.begin(), xs.end());
    xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
    vector<Point> result;
    long long prev = 0;
    for (long long x : xs) {
        long long h = 0;
        for (const auto& b : buildings)
            if (b[0] <= x && x < b[1]) h = std::max(h, b[2]);
        if (h != prev) { result.emplace_back(x, h); prev = h; }
    }
    return result;
}

void print(const vector<Point>& pts) {
    std::cout << "[";
    for (std::size_t k = 0; k < pts.size(); ++k)
        std::cout << (k ? "," : "") << "[" << pts[k].first << "," << pts[k].second << "]";
    std::cout << "]\n";
}

int main() {
    // Example 1
    vector<Building> ex1 = {{2, 9, 10}, {3, 7, 15}, {5, 12, 12}, {15, 20, 10}, {19, 24, 8}};
    vector<Point> want1 = {{2, 10}, {3, 15}, {7, 12}, {12, 0}, {15, 10}, {20, 8}, {24, 0}};
    auto got1 = getSkyline(ex1);
    print(got1);
    assert(got1 == want1 && getSkylineHeap(ex1) == want1);

    // Example 2: touching buildings of equal height merge into one segment
    vector<Building> ex2 = {{0, 2, 3}, {2, 5, 3}};
    vector<Point> want2 = {{0, 3}, {5, 0}};
    auto got2 = getSkyline(ex2);
    print(got2);
    assert(got2 == want2 && getSkylineHeap(ex2) == want2);

    // Example 3: identical buildings, and one hidden inside another
    vector<Building> ex3 = {{1, 5, 4}, {1, 5, 4}, {2, 3, 2}};
    vector<Point> want3 = {{1, 4}, {5, 0}};
    auto got3 = getSkyline(ex3);
    print(got3);
    assert(got3 == want3 && getSkylineHeap(ex3) == want3);

    // Example 4: a taller building starts exactly where a shorter one ends
    vector<Building> ex4 = {{1, 3, 2}, {3, 6, 5}, {6, 8, 5}};
    vector<Point> want4 = {{1, 2}, {3, 5}, {8, 0}};
    auto got4 = getSkyline(ex4);
    print(got4);
    assert(got4 == want4 && getSkylineHeap(ex4) == want4);

    // Randomised cross-check against brute force (lots of ties on purpose)
    std::mt19937 rng(20261019);
    for (int trial = 0; trial < 20000; ++trial) {
        int n = 1 + rng() % 8;
        vector<Building> bs;
        for (int k = 0; k < n; ++k) {
            long long l = rng() % 10, w = 1 + rng() % 5, h = 1 + rng() % 4;
            bs.push_back({l, l + w, h});
        }
        auto ref = bruteForce(bs);
        assert(getSkyline(bs) == ref);
        assert(getSkylineHeap(bs) == ref);
    }
    std::cout << "All tests passed.\n";
    return 0;
}
