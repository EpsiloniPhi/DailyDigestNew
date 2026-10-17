// Daily Quant Digest 2026-10-18
// Sliding window median with two heaps and lazy deletion.
// Compile: g++ -std=c++17 -O2 -Wall 2026-10-18.cpp -o median && ./median

#include <algorithm>
#include <cassert>
#include <cmath>
#include <climits>
#include <functional>
#include <iostream>
#include <queue>
#include <random>
#include <unordered_map>
#include <vector>

class DualHeap {
public:
    explicit DualHeap(int k) : k_(k) {}

    void insert(int x) {
        if (lo_.empty() || x <= lo_.top()) {
            lo_.push(x);
            ++loSize_;
        } else {
            hi_.push(x);
            ++hiSize_;
        }
        rebalance();
    }

    void erase(int x) {
        ++delayed_[x];                 // remember to throw x away later
        if (x <= lo_.top()) {          // x lives (logically) in the lower half
            --loSize_;
            if (x == lo_.top()) prune(lo_);
        } else {                       // x lives in the upper half
            --hiSize_;
            if (x == hi_.top()) prune(hi_);
        }
        rebalance();
    }

    double median() const {
        if (k_ % 2 == 1) return static_cast<double>(lo_.top());
        // promote to double before adding: INT_MAX + INT_MAX overflows int
        return (static_cast<double>(lo_.top()) + static_cast<double>(hi_.top())) / 2.0;
    }

private:
    // Pop every stale element sitting on top of heap h.
    template <typename Heap>
    void prune(Heap& h) {
        while (!h.empty()) {
            auto it = delayed_.find(h.top());
            if (it == delayed_.end() || it->second == 0) break;
            if (--(it->second) == 0) delayed_.erase(it);
            h.pop();
        }
    }

    // Keep loSize_ == hiSize_ or loSize_ == hiSize_ + 1.
    void rebalance() {
        if (loSize_ > hiSize_ + 1) {
            hi_.push(lo_.top());
            lo_.pop();
            --loSize_;
            ++hiSize_;
            prune(lo_);
        } else if (loSize_ < hiSize_) {
            lo_.push(hi_.top());
            hi_.pop();
            ++loSize_;
            --hiSize_;
            prune(hi_);
        }
    }

    int k_;
    std::priority_queue<int> lo_;                                      // max-heap: smaller half
    std::priority_queue<int, std::vector<int>, std::greater<int>> hi_; // min-heap: larger half
    std::unordered_map<int, int> delayed_;                             // value -> pending deletions
    int loSize_ = 0, hiSize_ = 0;                                      // logical (valid) sizes
};

std::vector<double> medianSlidingWindow(const std::vector<int>& nums, int k) {
    DualHeap dh(k);
    for (int i = 0; i < k; ++i) dh.insert(nums[i]);
    std::vector<double> ans;
    ans.reserve(nums.size() - k + 1);
    ans.push_back(dh.median());
    for (std::size_t i = k; i < nums.size(); ++i) {
        dh.insert(nums[i]);
        dh.erase(nums[i - k]);
        ans.push_back(dh.median());
    }
    return ans;
}

// O(n k log k) reference: sort every window.
std::vector<double> bruteForce(const std::vector<int>& nums, int k) {
    std::vector<double> ans;
    for (std::size_t i = 0; i + k <= nums.size(); ++i) {
        std::vector<int> w(nums.begin() + i, nums.begin() + i + k);
        std::sort(w.begin(), w.end());
        ans.push_back(k % 2 ? static_cast<double>(w[k / 2])
                            : (static_cast<double>(w[k / 2 - 1]) + w[k / 2]) / 2.0);
    }
    return ans;
}

void print(const std::vector<double>& v) {
    std::cout << "[";
    for (std::size_t i = 0; i < v.size(); ++i) std::cout << (i ? ", " : "") << v[i];
    std::cout << "]\n";
}

int main() {
    std::vector<int> a = {1, 3, -1, -3, 5, 3, 6, 7};
    auto r1 = medianSlidingWindow(a, 3);
    print(r1);  // [1, -1, -1, 3, 5, 6]
    assert((r1 == std::vector<double>{1, -1, -1, 3, 5, 6}));

    auto r2 = medianSlidingWindow(a, 4);
    print(r2);  // [0, 1, 1, 4, 5.5]
    assert((r2 == std::vector<double>{0, 1, 1, 4, 5.5}));

    std::vector<int> b = {INT_MAX, INT_MAX};
    auto r3 = medianSlidingWindow(b, 2);
    std::cout.precision(12);
    print(r3);  // [2147483647]
    assert(r3[0] == 2147483647.0);

    std::vector<int> c = {5, 5, 5, 1, 1, 5, 5};
    assert(medianSlidingWindow(c, 2) == bruteForce(c, 2));

    // Randomised check against brute force, heavy on duplicates.
    std::mt19937 rng(42);
    for (int t = 0; t < 3000; ++t) {
        int n = 1 + rng() % 60;
        int k = 1 + rng() % n;
        int range = 1 + rng() % 10;
        std::vector<int> v(n);
        for (int& x : v) x = static_cast<int>(rng() % (2 * range + 1)) - range;
        assert(medianSlidingWindow(v, k) == bruteForce(v, k));
    }
    std::cout << "All tests passed.\n";
    return 0;
}
