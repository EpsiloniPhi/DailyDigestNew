// Daily Quant Digest 2026-11-03
// FR Lesson 8: sliding window.
// Warm-up: best total P&L over any k consecutive days (fixed-size window).
// Problem: longest stretch of a feed with no repeated symbol
//          (classic "longest substring without repeating characters").
// Compile: g++ -std=c++17 -O2 -Wall 2026-11-03.cpp -o window && ./window
#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using std::array;
using std::string;
using std::vector;

// Warm-up: the largest sum of k consecutive entries. Requires 1 <= k <= pnl.size().
long long bestKDaySum(const vector<int>& pnl, int k) {
    long long window = 0;
    for (int i = 0; i < k; ++i) window += pnl[i];       // first window: days 0..k-1
    long long best = window;
    for (int right = k; right < (int)pnl.size(); ++right) {
        window += pnl[right];          // the new day walks in on the right
        window -= pnl[right - k];      // the oldest day walks out on the left
        best = std::max(best, window);
    }
    return best;
}

// Version 1: the general "grow right, shrink left" template with a count table.
int longestUniqueShrink(const string& s) {
    array<int, 256> count{};           // count[c] = copies of byte c inside the window
    int best = 0, left = 0;
    for (int right = 0; right < (int)s.size(); ++right) {
        unsigned char c = s[right];
        ++count[c];                                        // grow: take s[right] in
        while (count[c] > 1) {                             // window broken: c twice
            --count[(unsigned char)s[left]];               // shrink: drop s[left]
            ++left;
        }
        best = std::max(best, right - left + 1);           // window is valid here
    }
    return best;
}

// Version 2: jump the left edge directly, using where each symbol was last seen.
int longestUniqueJump(const string& s) {
    array<int, 256> last;
    last.fill(-1);                     // -1 = "not seen yet"
    int best = 0, left = 0;
    for (int right = 0; right < (int)s.size(); ++right) {
        unsigned char c = s[right];
        if (last[c] >= left)           // c is already inside the window
            left = last[c] + 1;        // jump just past its old copy (never backwards)
        last[c] = right;
        best = std::max(best, right - left + 1);
    }
    return best;
}

// Brute force checker: try every start, extend until a repeat. O(n^2) with a seen table.
int longestUniqueBrute(const string& s) {
    int n = (int)s.size(), best = 0;
    for (int i = 0; i < n; ++i) {
        array<bool, 256> seen{};
        for (int j = i; j < n; ++j) {
            unsigned char c = s[j];
            if (seen[c]) break;
            seen[c] = true;
            best = std::max(best, j - i + 1);
        }
    }
    return best;
}

long long bestKDayBrute(const vector<int>& pnl, int k) {
    long long best = LLONG_MIN;
    for (int i = 0; i + k <= (int)pnl.size(); ++i) {
        long long sum = 0;
        for (int j = i; j < i + k; ++j) sum += pnl[j];
        best = std::max(best, sum);
    }
    return best;
}

int main() {
    // Warm-up example from the text.
    vector<int> pnl = {4, -2, 7, 1, -5, 6, 3, -1};
    std::cout << "best 3-day P&L: " << bestKDaySum(pnl, 3) << "\n";   // expect 9
    assert(bestKDaySum(pnl, 3) == 9);

    // Examples from the text.
    struct Case { string s; int expected; };
    vector<Case> cases = {
        {"abcabcbb", 3}, {"bbbbb", 1}, {"pwwkew", 3}, {"", 0},
        {"abba", 2}, {"ELHTLEHK", 5}, {"dvdf", 3}, {" ", 1}
    };
    for (const auto& [s, expected] : cases) {
        int a = longestUniqueShrink(s), b = longestUniqueJump(s);
        std::cout << "\"" << s << "\" -> " << a << " / " << b
                  << " (expected " << expected << ")\n";
        assert(a == expected && b == expected);
    }

    // Randomized cross-check against brute force.
    std::mt19937 rng(20261103);
    for (int t = 0; t < 20000; ++t) {
        int n = rng() % 30;
        int alphabet = 1 + rng() % 8;
        string s;
        for (int i = 0; i < n; ++i) s += char('a' + rng() % alphabet);
        int brute = longestUniqueBrute(s);
        assert(longestUniqueShrink(s) == brute);
        assert(longestUniqueJump(s) == brute);

        int m = 1 + rng() % 20, k = 1 + rng() % m;
        vector<int> v(m);
        for (int& x : v) x = (int)(rng() % 201) - 100;
        assert(bestKDaySum(v, k) == bestKDayBrute(v, k));
    }
    // Bytes above 127 (e.g. UTF-8) must not crash: unsigned char keeps the index in 0..255.
    string bytes = "\xC3\xA9\xC3\xA8";
    assert(longestUniqueJump(bytes) == longestUniqueBrute(bytes));
    std::cout << "All random tests passed.\n";
    return 0;
}
