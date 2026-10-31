// Daily Quant Digest 2026-11-01 (Revision day): hash maps + sorting + counting.
// Problem: group words that are anagrams of each other, and count the pairs (i < j)
// of words that are anagrams.
// Compile: g++ -std=c++17 -O2 -Wall 2026-11-01.cpp && ./a.out
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

// Two words are anagrams if they use the same letters the same number of times.
// Brute force check for one pair: sort copies of both and compare.
bool isAnagram(std::string a, std::string b) {      // by value: we sort our own copies
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    return a == b;
}

// Brute force: test every pair. O(n^2 * L log L) time.
long long countPairsBrute(const std::vector<std::string>& words) {
    long long pairs = 0;
    for (std::size_t i = 0; i < words.size(); ++i)
        for (std::size_t j = i + 1; j < words.size(); ++j)
            if (isAnagram(words[i], words[j])) ++pairs;
    return pairs;
}

// The "fingerprint" of a word: its letters in sorted order.
// All anagrams share one fingerprint, and non-anagrams never do.
std::string fingerprint(const std::string& w) {
    std::string key = w;                             // copy, then sort the copy
    std::sort(key.begin(), key.end());
    return key;
}

// Group the words by fingerprint. Groups are returned in a fixed order
// (each group sorted, groups sorted by their first word) so output is predictable.
std::vector<std::vector<std::string>> groupAnagrams(const std::vector<std::string>& words) {
    std::unordered_map<std::string, std::vector<std::string>> buckets;
    for (const std::string& w : words)
        buckets[fingerprint(w)].push_back(w);       // [] creates an empty bucket if new

    std::vector<std::vector<std::string>> groups;
    groups.reserve(buckets.size());
    for (auto& [key, group] : buckets) {             // structured binding (C++17)
        std::sort(group.begin(), group.end());
        groups.push_back(std::move(group));          // move: no copy of the strings
    }
    std::sort(groups.begin(), groups.end(),
              [](const std::vector<std::string>& x, const std::vector<std::string>& y) {
                  return x.front() < y.front();      // strict "<", never "<="
              });
    return groups;
}

// Count anagram pairs in one pass: when a word arrives, it pairs with every
// earlier word that has the same fingerprint. O(n * L log L) time, O(n * L) space.
long long countPairs(const std::vector<std::string>& words) {
    std::unordered_map<std::string, long long> seen;
    long long pairs = 0;
    for (const std::string& w : words) {
        long long& c = seen[fingerprint(w)];         // reference into the map
        pairs += c;                                  // pair with each earlier match
        ++c;                                         // then count this word too
    }
    return pairs;
}

int main() {
    // 1) Worked examples from the text.
    std::vector<std::string> ex1 = {"rates", "fx", "stare", "vol", "bond", "tears", "xf", "lov"};
    assert(countPairs(ex1) == 5);
    assert(countPairsBrute(ex1) == 5);
    for (const auto& g : groupAnagrams(ex1)) {
        std::cout << "{";
        for (std::size_t i = 0; i < g.size(); ++i) std::cout << (i ? ", " : "") << g[i];
        std::cout << "}\n";
    }
    std::cout << "pairs = " << countPairs(ex1) << "\n";

    std::vector<std::string> ex2 = {"aa", "aa", "aa", "aa"};   // C(4,2) = 6
    assert(countPairs(ex2) == 6);
    std::vector<std::string> ex3 = {"ab", "abb", "bab", "ba"}; // ab/ba, abb/bab -> 2
    assert(countPairs(ex3) == 2);
    assert(countPairs({}) == 0);

    // 2) Randomized cross-check against brute force (small alphabet = many matches).
    std::mt19937 rng(20261101);
    std::uniform_int_distribution<int> nDist(0, 30), lenDist(0, 4), chDist(0, 2);
    for (int t = 0; t < 20000; ++t) {
        std::vector<std::string> w(nDist(rng));
        for (auto& s : w) {
            int len = lenDist(rng);
            for (int k = 0; k < len; ++k) s.push_back(static_cast<char>('a' + chDist(rng)));
        }
        assert(countPairs(w) == countPairsBrute(w));
        // Group sizes must add up to n, and sum of C(size, 2) must equal the pair count.
        long long total = 0, viaGroups = 0;
        for (const auto& g : groupAnagrams(w)) {
            long long s = static_cast<long long>(g.size());
            total += s;
            viaGroups += s * (s - 1) / 2;
        }
        assert(total == static_cast<long long>(w.size()));
        assert(viaGroups == countPairs(w));
    }

    // 3) Large input: 100,000 copies of one word gives C(100000, 2), too big for int.
    std::vector<std::string> big(100000, "spread");
    assert(countPairs(big) == 100000LL * 99999 / 2);
    std::cout << "big pairs = " << countPairs(big) << "\n";

    std::cout << "All tests passed.\n";
    return 0;
}
