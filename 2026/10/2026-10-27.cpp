// Daily Quant Digest 2026-10-27
// Foundations Restart, Algorithms Lesson 2: C++ refresher (vector, string,
// references, functions, loops).
// Problem: Reverse Words in a String.
//   Given a string s of words separated by one or more spaces (possibly with
//   leading/trailing spaces), return the words in reverse order, separated by
//   exactly one space, with no leading or trailing spaces.
// Three versions:
//   1) reverseWordsPrepend : glue each word onto the FRONT of the answer, O(n^2)
//   2) reverseWordsSplit   : split into a vector<string>, join backwards, O(n)
//   3) reverseWordsInPlace : reverse all, reverse each word, squeeze spaces,
//                            O(n) time and O(1) extra space
// Compile: g++ -std=c++17 -O2 -Wall 2026-10-27.cpp

#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// Version 1: the slow but obvious way. Each time we find a word we build
// word + " " + answer, which copies the whole answer again: O(n^2) characters.
std::string reverseWordsPrepend(const std::string& s) {
    std::string answer;
    const int n = static_cast<int>(s.size());
    int i = 0;
    while (i < n) {
        while (i < n && s[i] == ' ') ++i;          // skip spaces
        if (i == n) break;
        int j = i;
        while (j < n && s[j] != ' ') ++j;          // s[i..j-1] is one word
        std::string word = s.substr(i, j - i);
        if (answer.empty()) answer = word;
        else answer = word + " " + answer;         // copies all of answer
        i = j;
    }
    return answer;
}

// Version 2: split into a vector of words, then join them from the back.
std::vector<std::string> splitWords(const std::string& s) {
    std::vector<std::string> words;
    std::string current;
    for (char c : s) {                             // range-for over characters
        if (c == ' ') {
            if (!current.empty()) {
                words.push_back(current);
                current.clear();
            }
        } else {
            current.push_back(c);
        }
    }
    if (!current.empty()) words.push_back(current); // last word has no space after
    return words;
}

std::string reverseWordsSplit(const std::string& s) {
    std::vector<std::string> words = splitWords(s);
    std::string answer;
    for (int k = static_cast<int>(words.size()) - 1; k >= 0; --k) {
        answer += words[k];                         // += appends, no full copy
        if (k > 0) answer += ' ';
    }
    return answer;
}

// Version 3: work inside the caller's own string (passed by reference).
void reverseWordsInPlace(std::string& s) {
    std::reverse(s.begin(), s.end());               // "ab cd" -> "dc ba"
    const int n = static_cast<int>(s.size());
    int write = 0;                                  // next free slot in the answer
    int read = 0;
    while (read < n) {
        if (s[read] == ' ') { ++read; continue; }   // skip spaces
        if (write > 0) s[write++] = ' ';            // one space between words
        int start = write;                          // where this word will begin
        while (read < n && s[read] != ' ') s[write++] = s[read++];
        std::reverse(s.begin() + start, s.begin() + write); // fix the letters
    }
    s.resize(write);                                // chop off leftover characters
}

int main() {
    // 1) The examples from the problem statement.
    struct Case { std::string input; std::string expected; };
    const std::vector<Case> cases = {
        {"buy EURUSD at 1.16", "1.16 at EURUSD buy"},
        {"   sell   500   USDJPY  ", "USDJPY 500 sell"},
        {"hedge", "hedge"},
        {"     ", ""},
        {"a b  c", "c b a"},
    };
    for (const auto& c : cases) {
        std::string a = reverseWordsPrepend(c.input);
        std::string b = reverseWordsSplit(c.input);
        std::string d = c.input;                    // copy, because version 3 edits it
        reverseWordsInPlace(d);
        std::cout << "[" << c.input << "] -> [" << d << "]\n";
        assert(a == c.expected && b == c.expected && d == c.expected);
    }

    // 2) Randomized cross-check: all three versions must agree.
    std::mt19937 rng(2027);
    std::uniform_int_distribution<int> lenDist(0, 40);
    std::uniform_int_distribution<int> charDist(0, 3); // 0,1 -> space; 2,3 -> letter
    for (int trial = 0; trial < 50000; ++trial) {
        std::string s(lenDist(rng), ' ');
        for (char& ch : s) {
            int r = charDist(rng);
            ch = (r <= 1) ? ' ' : static_cast<char>('a' + r);
        }
        std::string a = reverseWordsPrepend(s);
        std::string b = reverseWordsSplit(s);
        std::string d = s;
        reverseWordsInPlace(d);
        assert(a == b && b == d);
    }
    std::cout << "50000 random tests passed\n";

    // 3) Timing: n = 200,000 characters of short words.
    std::string big;
    while (big.size() < 200000) big += "fx ";
    auto time = [](auto&& f) {                      // a small lambda for timing
        auto t0 = std::chrono::steady_clock::now();
        f();
        auto t1 = std::chrono::steady_clock::now();
        return std::chrono::duration<double, std::milli>(t1 - t0).count();
    };
    std::string r1, r2, r3 = big;
    double ms1 = time([&] { r1 = reverseWordsPrepend(big); });
    double ms2 = time([&] { r2 = reverseWordsSplit(big); });
    double ms3 = time([&] { reverseWordsInPlace(r3); });
    assert(r1 == r2 && r2 == r3);
    std::cout << "n=" << big.size() << "  prepend " << ms1 << " ms, split "
              << ms2 << " ms, in-place " << ms3 << " ms\n";
    return 0;
}
