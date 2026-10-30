// Daily Quant Digest 2026-10-31: Sorting (merge sort, std::sort, custom comparators).
// Problem: count the pairs of trades that arrived out of order (inversions).
// Compile: g++ -std=c++17 -O2 -Wall 2026-10-31.cpp && ./a.out
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// Brute force: look at every pair (i, j) with i < j. O(n^2) time, O(1) space.
long long countInversionsBrute(const std::vector<int>& a) {
    long long count = 0;
    const std::size_t n = a.size();
    for (std::size_t i = 0; i < n; ++i)
        for (std::size_t j = i + 1; j < n; ++j)
            if (a[i] > a[j]) ++count;
    return count;
}

// Sort a[lo, hi) with merge sort and return the number of inversions inside it.
// buf is scratch space of the same size as a, shared by every call.
long long sortAndCount(std::vector<int>& a, std::vector<int>& buf,
                       std::size_t lo, std::size_t hi) {
    if (hi - lo <= 1) return 0;                 // 0 or 1 element: sorted, no inversions
    const std::size_t mid = lo + (hi - lo) / 2;

    long long count = sortAndCount(a, buf, lo, mid)   // inversions inside the left half
                    + sortAndCount(a, buf, mid, hi);  // inversions inside the right half

    // Merge the two sorted halves, counting the "cross" inversions.
    std::size_t i = lo, j = mid, k = lo;
    while (i < mid && j < hi) {
        if (a[i] <= a[j]) {
            buf[k++] = a[i++];                  // left value is smaller: no inversion
        } else {
            // a[j] jumps ahead of a[i] and of every value still waiting on the left.
            count += static_cast<long long>(mid - i);
            buf[k++] = a[j++];
        }
    }
    while (i < mid) buf[k++] = a[i++];          // copy whatever is left over
    while (j < hi)  buf[k++] = a[j++];
    std::copy(buf.begin() + lo, buf.begin() + hi, a.begin() + lo);
    return count;
}

// Public entry point: takes the array by value, so the caller's copy is untouched.
long long countInversions(std::vector<int> a) {
    std::vector<int> buf(a.size());
    return sortAndCount(a, buf, 0, a.size());
}

// A small demo of std::sort with a custom comparator.
struct Trade {
    std::string id;
    double price;
    int time;   // seconds after the open
};

int main() {
    // 1) Worked examples from the text.
    struct Case { std::vector<int> a; long long expected; };
    std::vector<Case> cases = {
        {{3, 1, 4, 5, 2}, 4},
        {{2, 4, 1, 3, 5}, 3},
        {{1, 2, 3, 4}, 0},
        {{5, 4, 3, 2, 1}, 10},
        {{2, 2, 1}, 2},          // equal values are NOT an inversion
        {{}, 0},
    };
    for (const auto& c : cases) {
        long long got = countInversions(c.a);
        std::cout << "inversions = " << got << " (expected " << c.expected << ")\n";
        assert(got == c.expected);
        assert(countInversionsBrute(c.a) == c.expected);
    }

    // 2) Randomized cross-check against brute force, and check the sort itself.
    std::mt19937 rng(20261031);
    std::uniform_int_distribution<int> lenDist(0, 40), valDist(-5, 5);
    for (int t = 0; t < 20000; ++t) {
        std::vector<int> a(lenDist(rng));
        for (auto& x : a) x = valDist(rng);
        assert(countInversions(a) == countInversionsBrute(a));

        std::vector<int> mine = a, expected = a, buf(a.size());
        sortAndCount(mine, buf, 0, mine.size());
        std::sort(expected.begin(), expected.end());
        assert(mine == expected);
    }

    // 3) Large reversed input: n(n-1)/2 is about 2 * 10^10, too big for int.
    const int n = 200000;
    std::vector<int> rev(n);
    for (int i = 0; i < n; ++i) rev[i] = n - i;
    assert(countInversions(rev) == static_cast<long long>(n) * (n - 1) / 2);

    // 4) std::sort with a custom comparator: best price first (highest bid),
    //    and at the same price, earliest time first.
    std::vector<Trade> bids = {
        {"B1", 1.1650, 30}, {"B2", 1.1652, 45}, {"B3", 1.1650, 10}, {"B4", 1.1652, 20}};
    std::sort(bids.begin(), bids.end(), [](const Trade& x, const Trade& y) {
        if (x.price != y.price) return x.price > y.price;   // higher price first
        return x.time < y.time;                             // then earlier time
    });
    std::cout << "bid queue:";
    for (const auto& b : bids) std::cout << ' ' << b.id;
    std::cout << '\n';
    assert(bids[0].id == "B4" && bids[1].id == "B2" && bids[2].id == "B3" && bids[3].id == "B1");

    std::cout << "All tests passed.\n";
    return 0;
}
