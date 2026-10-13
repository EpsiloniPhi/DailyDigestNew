// Daily Quant Digest 2026-10-14
// Largest Rectangle in Histogram (monotonic stack), O(n) time, O(n) space.
// Compile: g++ -std=c++17 -O2 -Wall 2026-10-14.cpp -o hist && ./hist

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

// Returns the area of the largest axis-aligned rectangle that fits under the histogram.
long long largestRectangleArea(const std::vector<int>& heights) {
    const int n = static_cast<int>(heights.size());
    std::vector<int> stk;          // indices; their heights are strictly increasing bottom to top
    stk.reserve(n + 1);
    long long best = 0;

    // i == n acts as a sentinel bar of height 0 that flushes everything left on the stack.
    for (int i = 0; i <= n; ++i) {
        const int h = (i == n) ? 0 : heights[i];
        // Every bar taller than (or equal to) h has just found its first smaller bar on the right: i.
        while (!stk.empty() && heights[stk.back()] >= h) {
            const int top = stk.back();
            stk.pop_back();
            // After popping, the new top (if any) is the first bar to the left that is smaller.
            const int left = stk.empty() ? -1 : stk.back();
            const long long width = i - left - 1;
            best = std::max(best, static_cast<long long>(heights[top]) * width);
        }
        stk.push_back(i);
    }
    return best;
}

// O(n^2) reference: fix the left edge, extend right while tracking the running minimum.
long long bruteForce(const std::vector<int>& h) {
    long long best = 0;
    for (std::size_t l = 0; l < h.size(); ++l) {
        int mn = h[l];
        for (std::size_t r = l; r < h.size(); ++r) {
            mn = std::min(mn, h[r]);
            best = std::max(best, static_cast<long long>(mn) * static_cast<long long>(r - l + 1));
        }
    }
    return best;
}

int main() {
    struct Case { std::vector<int> h; long long expected; };
    const std::vector<Case> cases = {
        {{2, 1, 5, 6, 2, 3}, 10},
        {{2, 4}, 4},
        {{1}, 1},
        {{0, 0, 0}, 0},
        {{3, 3, 3, 3}, 12},
        {{1, 2, 3, 4, 5}, 9},
        {{5, 4, 3, 2, 1}, 9},
        {{6, 2, 5, 4, 5, 1, 6}, 12},
        {{10000, 10000, 10000}, 30000},
    };

    for (const auto& [h, expected] : cases) {
        const long long got = largestRectangleArea(h);
        std::cout << "[";
        for (std::size_t i = 0; i < h.size(); ++i) std::cout << (i ? "," : "") << h[i];
        std::cout << "] -> " << got << (got == expected ? "  OK" : "  FAIL") << '\n';
        assert(got == expected);
    }

    // Large input: 100000 bars of height 10000 gives 1e9, and long long avoids any overflow worry.
    std::vector<int> big(100000, 10000);
    std::cout << "100000 x 10000 -> " << largestRectangleArea(big) << '\n';
    assert(largestRectangleArea(big) == 1000000000LL);

    // Randomized cross-check against brute force.
    std::mt19937 rng(20261014);
    std::uniform_int_distribution<int> lenDist(1, 60), hDist(0, 20);
    for (int t = 0; t < 2000; ++t) {
        std::vector<int> h(lenDist(rng));
        for (auto& x : h) x = hDist(rng);
        assert(largestRectangleArea(h) == bruteForce(h));
    }
    std::cout << "2000 random tests vs brute force: all passed\n";
    return 0;
}
