// Daily Quant Digest 2026-10-15
// Binary search on the answer: Split Array Largest Sum
// Compile: g++ -std=c++17 -O2 -Wall 2026-10-15.cpp -o split && ./split

#include <algorithm>
#include <cassert>
#include <climits>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

using namespace std;

// Returns the minimum possible value of the largest piece sum when nums is
// split into exactly k non-empty contiguous pieces (1 <= k <= nums.size()).
long long splitArray(const vector<long long>& nums, int k) {
    // Feasibility test: can we split into at most k pieces, each with sum <= cap?
    // Greedy: extend the current piece while it fits, otherwise start a new one.
    auto feasible = [&nums, k](long long cap) {
        int pieces = 1;
        long long running = 0;
        for (long long x : nums) {
            if (running + x > cap) {   // x does not fit: close the piece
                ++pieces;
                running = x;           // x <= cap is guaranteed by lo below
                if (pieces > k) return false;
            } else {
                running += x;
            }
        }
        return true;
    };

    // The answer lies in [max element, total sum].
    long long lo = *max_element(nums.begin(), nums.end());
    long long hi = accumulate(nums.begin(), nums.end(), 0LL);

    // Find the smallest cap in [lo, hi] with feasible(cap) == true.
    // Invariant: feasible(hi) is true; every value below lo is infeasible.
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;  // rounds down, so the loop shrinks
        if (feasible(mid)) {
            hi = mid;       // mid works; the answer is mid or smaller
        } else {
            lo = mid + 1;   // mid fails; the answer is strictly larger
        }
    }
    return lo;
}

// O(k * n^2) dynamic programming reference, used only to test the fast version.
long long splitArrayDP(const vector<long long>& nums, int k) {
    int n = static_cast<int>(nums.size());
    vector<long long> prefix(n + 1, 0);
    for (int i = 0; i < n; ++i) prefix[i + 1] = prefix[i] + nums[i];

    const long long INF = LLONG_MAX;
    // dp[j][i] = best largest-sum for the first i elements split into j pieces
    vector<vector<long long>> dp(k + 1, vector<long long>(n + 1, INF));
    dp[0][0] = 0;
    for (int j = 1; j <= k; ++j) {
        for (int i = j; i <= n; ++i) {
            for (int p = j - 1; p < i; ++p) {  // last piece is nums[p..i-1]
                if (dp[j - 1][p] == INF) continue;
                long long cand = max(dp[j - 1][p], prefix[i] - prefix[p]);
                dp[j][i] = min(dp[j][i], cand);
            }
        }
    }
    return dp[k][n];
}

int main() {
    struct Test {
        vector<long long> nums;
        int k;
        long long expected;
    };
    vector<Test> tests = {
        {{7, 2, 5, 10, 8}, 2, 18},
        {{1, 2, 3, 4, 5}, 2, 9},
        {{1, 4, 4}, 3, 4},
        {{5}, 1, 5},
        {{2, 3, 1, 2, 4, 3}, 5, 4},
        {{1000000000, 1000000000, 1000000000}, 1, 3000000000LL},  // needs 64-bit
    };

    for (const auto& t : tests) {
        long long got = splitArray(t.nums, t.k);
        cout << "k=" << t.k << " -> " << got << " (expected " << t.expected << ")"
             << (got == t.expected ? "  OK" : "  FAIL") << '\n';
        assert(got == t.expected);
    }

    // Randomized cross-check against the DP on small inputs.
    mt19937 rng(20261015);
    for (int iter = 0; iter < 2000; ++iter) {
        int n = uniform_int_distribution<int>(1, 12)(rng);
        int k = uniform_int_distribution<int>(1, n)(rng);
        vector<long long> nums(n);
        for (auto& x : nums) x = uniform_int_distribution<int>(0, 50)(rng);
        if (*max_element(nums.begin(), nums.end()) == 0) nums[0] = 1;
        long long a = splitArray(nums, k), b = splitArrayDP(nums, k);
        if (a != b) {
            cout << "Mismatch at iter " << iter << ": " << a << " vs " << b << '\n';
            return 1;
        }
    }
    cout << "Random cross-check vs DP: 2000/2000 passed\n";
    return 0;
}
