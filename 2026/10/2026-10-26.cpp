// Daily Quant Digest 2026-10-26: Russian Doll Envelopes (LIS in two dimensions)
// Compile: g++ -std=c++17 -O2 -Wall 2026-10-26.cpp -o dolls && ./dolls
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

// Optimal: sort by width ascending, and for equal widths by height DESCENDING.
// Then the answer is the longest strictly increasing subsequence of the heights.
// tails[k] = smallest possible last height of an increasing chain of length k+1.
int maxEnvelopes(vector<vector<int>> envelopes) {      // by value: we sort our own copy
    sort(envelopes.begin(), envelopes.end(),
         [](const vector<int>& a, const vector<int>& b) {
             if (a[0] != b[0]) return a[0] < b[0];      // narrower first
             return a[1] > b[1];                         // same width: taller first
         });

    vector<int> tails;
    tails.reserve(envelopes.size());
    for (const auto& e : envelopes) {
        int h = e[1];
        // first tail that is >= h (strict increase, so equal heights cannot extend)
        auto it = lower_bound(tails.begin(), tails.end(), h);
        if (it == tails.end()) tails.push_back(h);      // h extends the longest chain
        else *it = h;                                    // h is a cheaper ending for that length
    }
    return static_cast<int>(tails.size());
}

// Brute force O(n^2) DP for cross-checking: best[i] = longest chain ending at envelope i.
int bruteForce(vector<vector<int>> env) {
    sort(env.begin(), env.end());
    int n = static_cast<int>(env.size()), ans = 0;
    vector<int> best(n, 1);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j)
            if (env[j][0] < env[i][0] && env[j][1] < env[i][1])
                best[i] = max(best[i], best[j] + 1);
        ans = max(ans, best[i]);
    }
    return ans;
}

int main() {
    // Example 1: [2,3] -> [5,4] -> [6,7]
    vector<vector<int>> ex1 = {{5, 4}, {6, 4}, {6, 7}, {2, 3}};
    // Example 2: identical envelopes cannot nest
    vector<vector<int>> ex2 = {{1, 1}, {1, 1}, {1, 1}};
    // Example 3: the equal-width trap; the answer is 2, not 4
    vector<vector<int>> ex3 = {{1, 1}, {2, 2}, {2, 3}, {2, 4}};

    cout << "Example 1: " << maxEnvelopes(ex1) << " (expected 3)\n";
    cout << "Example 2: " << maxEnvelopes(ex2) << " (expected 1)\n";
    cout << "Example 3: " << maxEnvelopes(ex3) << " (expected 2)\n";
    assert(maxEnvelopes(ex1) == 3);
    assert(maxEnvelopes(ex2) == 1);
    assert(maxEnvelopes(ex3) == 2);

    // Randomized cross-check against the O(n^2) brute force.
    mt19937 rng(20261026);
    for (int trial = 0; trial < 3000; ++trial) {
        int n = static_cast<int>(rng() % 40) + 1;
        int range = static_cast<int>(rng() % 10) + 1;    // small range forces many ties
        vector<vector<int>> env(n);
        for (auto& e : env)
            e = {static_cast<int>(rng() % range) + 1, static_cast<int>(rng() % range) + 1};
        assert(maxEnvelopes(env) == bruteForce(env));
    }
    cout << "3000 random tests passed\n";

    // Large test: 100,000 envelopes on a diagonal, answer must be 100,000.
    vector<vector<int>> big;
    for (int i = 100000; i >= 1; --i) big.push_back({i, i});
    cout << "Large diagonal: " << maxEnvelopes(big) << " (expected 100000)\n";
    return 0;
}
