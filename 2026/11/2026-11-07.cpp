// Daily Quant Digest 2026-11-07, Algorithms: Decode String with a stack.
// Compile: g++ -std=c++17 -O2 -Wall 2026-11-07.cpp && ./a.out
#include <cassert>
#include <cctype>
#include <iostream>
#include <random>
#include <stack>
#include <string>
#include <utility>
#include <vector>
using namespace std;

// Fast solution: one pass with a stack of "paused jobs".
// Each frame remembers the text built before a '[' and how many times
// the block inside the brackets must be repeated.
string decodeString(const string& s) {
    stack<pair<string, int>> paused;   // (text before '[', repeat count)
    string cur;                         // the text of the level we are in
    int k = 0;                          // the number being read, digit by digit
    for (char c : s) {
        if (isdigit(static_cast<unsigned char>(c))) {
            k = k * 10 + (c - '0');     // "12" -> 1, then 1*10+2 = 12
        } else if (c == '[') {
            paused.push({cur, k});      // pause the outer job
            cur.clear();                // start a fresh inner job
            k = 0;
        } else if (c == ']') {
            auto [before, times] = paused.top();   // resume the outer job
            paused.pop();
            string block;
            block.reserve(cur.size() * times);
            for (int i = 0; i < times; ++i) block += cur;
            cur = std::move(before);
            cur += block;
        } else {
            cur += c;                   // an ordinary letter
        }
    }
    return cur;
}

// Brute force: repeatedly find an innermost "k[letters]" and expand it,
// rebuilding the whole string each time.
string decodeBrute(string s) {
    while (true) {
        size_t close = s.find(']');
        if (close == string::npos) return s;
        size_t open = s.rfind('[', close);          // innermost '[' before it
        size_t start = open;
        while (start > 0 && isdigit(static_cast<unsigned char>(s[start - 1]))) --start;
        int times = stoi(s.substr(start, open - start));
        string inner = s.substr(open + 1, close - open - 1);
        string rep;
        for (int i = 0; i < times; ++i) rep += inner;
        s = s.substr(0, start) + rep + s.substr(close + 1);
    }
}

// Random valid encoded string generator (small, so outputs stay small).
string randomEncoded(mt19937& rng, int depth) {
    uniform_int_distribution<int> pieces(1, 3), kind(0, 2), letter(0, 3), times(1, 4);
    string out;
    int n = pieces(rng);
    for (int i = 0; i < n; ++i) {
        if (depth > 0 && kind(rng) == 0) {
            out += to_string(times(rng));
            out += '[';
            out += randomEncoded(rng, depth - 1);
            out += ']';
        } else {
            out += static_cast<char>('a' + letter(rng));
        }
    }
    return out;
}

int main() {
    struct Case { string in, expected; };
    vector<Case> cases = {
        {"3[a]2[bc]", "aaabcbc"},
        {"3[a2[c]]", "accaccacc"},
        {"2[abc]3[cd]ef", "abcabccdcdcdef"},
        {"abc", "abc"},
        {"10[x]", "xxxxxxxxxx"},
        {"2[a2[b2[c]]]d", "abccbccabccbccd"},
        {"1[ab]", "ab"},
    };
    for (const auto& c : cases) {
        string got = decodeString(c.in);
        cout << c.in << " -> " << got << '\n';
        assert(got == c.expected);
        assert(decodeBrute(c.in) == c.expected);
    }
    // Long repeat count: 300[a] has length 300.
    assert(decodeString("300[a]").size() == 300);

    mt19937 rng(20261107);
    for (int t = 0; t < 100000; ++t) {
        string s = randomEncoded(rng, 3);
        assert(decodeString(s) == decodeBrute(s));
    }
    cout << "All tests passed (7 examples + 100,000 random cross-checks).\n";
    return 0;
}
