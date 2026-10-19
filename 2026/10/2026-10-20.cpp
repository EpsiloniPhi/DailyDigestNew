// Daily Quant Digest 2026-10-20
// Algorithms & DS: Graphs and topological sort, "Alien Dictionary"
// Compile: g++ -std=c++17 -O2 -Wall 2026-10-20.cpp -o alien && ./alien

#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <queue>
#include <random>
#include <string>
#include <vector>

using std::string;
using std::vector;

// ---------------------------------------------------------------------------
// Solution: build a graph of "letter u comes before letter v" from adjacent
// words, then run Kahn's algorithm (BFS on in-degrees). A min-heap picks the
// smallest available letter each time, so the answer is the alphabetically
// smallest valid order (deterministic, easy to test).
// Returns "" if no valid order exists.
// Time O(C + 26 log 26) where C = total characters; space O(26^2).
// ---------------------------------------------------------------------------
string alienOrder(const vector<string>& words) {
    std::array<bool, 26> present{};                // letters that appear at all
    std::array<std::array<bool, 26>, 26> edge{};   // edge[u][v]: u before v
    std::array<int, 26> indegree{};

    for (const string& w : words)
        for (char ch : w) present[ch - 'a'] = true;

    // Only ADJACENT pairs are needed: if w1 <= w2 <= w3 then w1 <= w3 follows.
    for (std::size_t i = 0; i + 1 < words.size(); ++i) {
        const string& a = words[i];
        const string& b = words[i + 1];
        // std::mismatch walks both strings and stops at the first difference.
        auto [ia, ib] = std::mismatch(a.begin(), a.end(), b.begin(), b.end());
        if (ia == a.end()) continue;               // a is a prefix of b (or equal): fine
        if (ib == b.end()) return "";              // b is a proper prefix of a: impossible
        int u = *ia - 'a', v = *ib - 'a';
        if (!edge[u][v]) {                         // avoid counting a duplicate edge twice
            edge[u][v] = true;
            ++indegree[v];
        }
    }

    // Min-heap of letters with no remaining prerequisites.
    std::priority_queue<int, vector<int>, std::greater<int>> ready;
    int total = 0;
    for (int c = 0; c < 26; ++c)
        if (present[c]) {
            ++total;
            if (indegree[c] == 0) ready.push(c);
        }

    string order;
    while (!ready.empty()) {
        int u = ready.top();
        ready.pop();
        order.push_back(static_cast<char>('a' + u));
        for (int v = 0; v < 26; ++v)
            if (edge[u][v] && --indegree[v] == 0) ready.push(v);
    }
    // If a cycle exists, its letters never reach in-degree 0.
    return static_cast<int>(order.size()) == total ? order : "";
}

// ---------------------------------------------------------------------------
// Brute force reference: try every permutation of the present letters in
// alphabetical order; return the first one under which the list is sorted.
// ---------------------------------------------------------------------------
bool sortedUnder(const vector<string>& words, const string& order) {
    std::array<int, 26> rank{};
    for (std::size_t k = 0; k < order.size(); ++k) rank[order[k] - 'a'] = static_cast<int>(k);
    auto less = [&](const string& x, const string& y) {
        return std::lexicographical_compare(
            x.begin(), x.end(), y.begin(), y.end(),
            [&](char p, char q) { return rank[p - 'a'] < rank[q - 'a']; });
    };
    for (std::size_t i = 0; i + 1 < words.size(); ++i)
        if (less(words[i + 1], words[i])) return false;
    return true;
}

string bruteForce(const vector<string>& words) {
    string letters;
    for (const string& w : words) letters += w;
    std::sort(letters.begin(), letters.end());
    letters.erase(std::unique(letters.begin(), letters.end()), letters.end());
    do {
        if (sortedUnder(words, letters)) return letters;
    } while (std::next_permutation(letters.begin(), letters.end()));
    return "";
}

int main() {
    struct Case { vector<string> words; string expected; };
    vector<Case> cases = {
        {{"wrt", "wrf", "er", "ett", "rftt"}, "wertf"},   // example 1
        {{"z", "x"}, "zx"},                              // example 2
        {{"z", "x", "z"}, ""},                           // example 3: cycle
        {{"abc", "ab"}, ""},                             // example 4: bad prefix
        {{"ab", "abc"}, "abc"},                          // prefix the right way round
        {{"baa", "abcd", "abca", "cab", "cad"}, "bdac"}, // longer chain
    };
    for (const auto& c : cases) {
        string got = alienOrder(c.words);
        std::cout << "\"" << got << "\"\n";
        assert(got == c.expected);
        assert(bruteForce(c.words) == c.expected);
    }

    // Randomised cross-check: sometimes sort by a hidden alphabet (always
    // valid), sometimes leave the list in random order (often invalid).
    std::mt19937 rng(20261020);
    for (int trial = 0; trial < 20000; ++trial) {
        int L = 1 + rng() % 5;                     // alphabet a..(a+L-1)
        int n = 1 + rng() % 6;
        vector<string> words(n);
        for (auto& w : words) {
            int len = 1 + rng() % 4;
            for (int k = 0; k < len; ++k) w.push_back(static_cast<char>('a' + rng() % L));
        }
        if (rng() % 2) {
            string hidden(L, ' ');
            for (int k = 0; k < L; ++k) hidden[k] = static_cast<char>('a' + k);
            std::shuffle(hidden.begin(), hidden.end(), rng);
            std::array<int, 26> rank{};
            for (int k = 0; k < L; ++k) rank[hidden[k] - 'a'] = k;
            std::sort(words.begin(), words.end(), [&](const string& x, const string& y) {
                return std::lexicographical_compare(
                    x.begin(), x.end(), y.begin(), y.end(),
                    [&](char p, char q) { return rank[p - 'a'] < rank[q - 'a']; });
            });
            assert(alienOrder(words) != "");       // a valid order must be found
        }
        string got = alienOrder(words);
        assert(got == bruteForce(words));
        if (!got.empty()) assert(sortedUnder(words, got));
    }
    std::cout << "All tests passed.\n";
    return 0;
}
