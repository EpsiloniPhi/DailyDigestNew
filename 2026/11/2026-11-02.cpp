// Daily Quant Digest 2026-11-02
// FR Lesson 7: two pointers. Problem: three trades that net to zero (3Sum).
// Compile: g++ -std=c++17 -O2 -Wall 2026-11-02.cpp -o three_sum && ./three_sum

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <random>
#include <set>
#include <vector>

using std::array;
using std::vector;

// Warm-up: does a SORTED array contain two entries (at different positions)
// that add up to target? Two pointers walking towards each other.
bool hasPairWithSum(const vector<int>& sorted, int target) {
    int lo = 0;
    int hi = static_cast<int>(sorted.size()) - 1;
    while (lo < hi) {
        int sum = sorted[lo] + sorted[hi];
        if (sum == target) return true;
        if (sum < target) ++lo;   // need more: the small end steps up
        else --hi;                // need less: the big end steps down
    }
    return false;
}

// Main solution: all unique triplets {a, b, c} with a + b + c == 0.
// Sort, fix the smallest member, then two pointers on the rest.
// O(n^2) time, O(1) extra space besides the output (we sort our own copy).
vector<array<int, 3>> threeSum(vector<int> nums) {       // by value: we may reorder it
    std::sort(nums.begin(), nums.end());
    const int n = static_cast<int>(nums.size());
    vector<array<int, 3>> result;

    for (int i = 0; i + 2 < n; ++i) {
        if (i > 0 && nums[i] == nums[i - 1]) continue;   // same anchor value: skip
        if (nums[i] > 0) break;                          // smallest is positive: no more zeros

        int lo = i + 1;
        int hi = n - 1;
        while (lo < hi) {
            int sum = nums[i] + nums[lo] + nums[hi];
            if (sum < 0) {
                ++lo;
            } else if (sum > 0) {
                --hi;
            } else {
                result.push_back({nums[i], nums[lo], nums[hi]});
                int leftVal = nums[lo];
                int rightVal = nums[hi];
                while (lo < hi && nums[lo] == leftVal) ++lo;    // jump past copies
                while (lo < hi && nums[hi] == rightVal) --hi;
            }
        }
    }
    return result;
}

// Brute force for testing: try every triple of positions, O(n^3).
vector<array<int, 3>> threeSumBrute(const vector<int>& nums) {
    std::set<array<int, 3>> found;                       // set removes duplicates and sorts
    const int n = static_cast<int>(nums.size());
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            for (int k = j + 1; k < n; ++k)
                if (nums[i] + nums[j] + nums[k] == 0) {
                    array<int, 3> t = {nums[i], nums[j], nums[k]};
                    std::sort(t.begin(), t.end());
                    found.insert(t);
                }
    return vector<array<int, 3>>(found.begin(), found.end());
}

void print(const vector<array<int, 3>>& triples) {
    std::cout << "[";
    for (std::size_t t = 0; t < triples.size(); ++t) {
        const auto& [a, b, c] = triples[t];                 // structured binding
        std::cout << (t ? ", " : "") << "[" << a << "," << b << "," << c << "]";
    }
    std::cout << "]\n";
}

int main() {
    // Examples from the digest.
    vector<int> ex1 = {-1, 0, 1, 2, -1, -4};
    vector<int> ex2 = {0, 1, 1};
    vector<int> ex3 = {0, 0, 0, 0};
    vector<int> ex4 = {-5, -2, -2, 1, 3, 4, 4, 7};
    print(threeSum(ex1));   // [[-1,-1,2], [-1,0,1]]
    print(threeSum(ex2));   // []
    print(threeSum(ex3));   // [[0,0,0]]
    print(threeSum(ex4));   // [[-5,-2,7], [-5,1,4], [-2,-2,4]]

    vector<int> shelf = {1, 3, 4, 6, 8, 11};
    std::cout << std::boolalpha << hasPairWithSum(shelf, 10) << " "
              << hasPairWithSum(shelf, 13) << "\n";            // true false

    // Randomized cross-check against brute force. The fast version already
    // returns triples in sorted order (anchor ascending, then lo ascending).
    std::mt19937 rng(20261102);
    for (int trial = 0; trial < 3000; ++trial) {
        int n = std::uniform_int_distribution<int>(0, 25)(rng);
        int range = std::uniform_int_distribution<int>(1, 12)(rng);
        std::uniform_int_distribution<int> val(-range, range);
        vector<int> a(n);
        for (int& x : a) x = val(rng);
        auto fast = threeSum(a);
        auto slow = threeSumBrute(a);
        assert(fast == slow);
    }
    std::cout << "3000 random tests passed\n";
    return 0;
}
