// Daily Quant Digest 2026-10-13-b
// Sliding window: Minimum Window Substring
//
// Given strings s and t, return the shortest contiguous substring of s that
// contains every character of t (with multiplicity). Return "" if none exists.
// If several windows tie on length, return the leftmost one.
//
// Compile: g++ -std=c++17 -O2 -Wall 2026-10-13-b.cpp -o mws && ./mws

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// O(|s| + |t|) time, O(1) extra space (fixed 128-slot table for ASCII).
std::string minWindow(const std::string& s, const std::string& t) {
    if (t.empty() || s.size() < t.size()) return "";

    // need[c] > 0  : the window still lacks need[c] copies of c
    // need[c] == 0 : exactly satisfied
    // need[c] < 0  : the window holds surplus copies of c
    std::array<int, 128> need{};          // value-initialised to all zeros
    for (unsigned char c : t) ++need[c];

    int missing = static_cast<int>(t.size());  // total characters still lacking
    int bestStart = 0;
    int bestLen = -1;                          // -1 means "no window found yet"

    int left = 0;
    for (int right = 0; right < static_cast<int>(s.size()); ++right) {
        unsigned char in = s[right];
        // If this character was still needed, it reduces what is missing.
        if (need[in] > 0) --missing;
        --need[in];

        // Window [left, right] is valid: shrink from the left while it stays valid.
        while (missing == 0) {
            int len = right - left + 1;
            if (bestLen == -1 || len < bestLen) {
                bestLen = len;
                bestStart = left;
            }
            unsigned char out = s[left];
            ++need[out];
            // need[out] just became positive: we removed a required copy.
            if (need[out] > 0) ++missing;
            ++left;
        }
    }
    return bestLen == -1 ? "" : s.substr(bestStart, bestLen);
}

// Brute force for cross-checking: O(n^2 * alphabet).
std::string minWindowBrute(const std::string& s, const std::string& t) {
    if (t.empty()) return "";
    std::array<int, 128> target{};
    for (unsigned char c : t) ++target[c];
    std::string best;
    bool found = false;
    for (std::size_t i = 0; i < s.size(); ++i) {
        std::array<int, 128> have{};
        for (std::size_t j = i; j < s.size(); ++j) {
            ++have[static_cast<unsigned char>(s[j])];
            bool ok = true;
            for (int c = 0; c < 128 && ok; ++c) ok = have[c] >= target[c];
            if (ok) {
                std::size_t len = j - i + 1;
                if (!found || len < best.size()) {
                    best = s.substr(i, len);
                    found = true;
                }
                break;  // extending j further only makes the window longer
            }
        }
    }
    return best;
}

int main() {
    struct Case { std::string s, t, expected; };
    const std::vector<Case> cases = {
        {"ADOBECODEBANC", "ABC", "BANC"},
        {"a", "a", "a"},
        {"a", "aa", ""},
        {"BBAACB", "ABB", "BBA"},
        {"XYZ", "Q", ""},
        {"aaflslflsldkalskaaa", "aaa", "aaa"},
    };
    for (const auto& c : cases) {
        std::string got = minWindow(c.s, c.t);
        std::cout << "s=\"" << c.s << "\" t=\"" << c.t << "\" -> \"" << got
                  << "\"" << (got == c.expected ? "  OK" : "  FAIL") << '\n';
        assert(got == c.expected);
    }

    // Randomised cross-check against the brute force.
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> letter(0, 3), lenS(0, 25), lenT(1, 5);
    for (int iter = 0; iter < 20000; ++iter) {
        std::string s, t;
        int n = lenS(rng), m = lenT(rng);
        for (int i = 0; i < n; ++i) s += static_cast<char>('a' + letter(rng));
        for (int i = 0; i < m; ++i) t += static_cast<char>('a' + letter(rng));
        std::string fast = minWindow(s, t), slow = minWindowBrute(s, t);
        if (fast != slow) {
            std::cout << "MISMATCH s=" << s << " t=" << t << " fast=" << fast
                      << " slow=" << slow << '\n';
            return 1;
        }
    }
    std::cout << "20000 random tests passed\n";
    return 0;
}
