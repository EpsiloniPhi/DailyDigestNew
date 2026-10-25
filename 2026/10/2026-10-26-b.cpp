// Daily Quant Digest 2026-10-26-b
// Foundations Restart, Algorithms Lesson 1: Big-O, counting steps.
// Problem: best single trade (buy once, sell once later) on a price series.
// Compile: g++ -std=c++17 -O2 -Wall 2026-10-26-b.cpp -o best_trade && ./best_trade

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

// Brute force: try every (buy day, sell day) pair. About n*n/2 steps: O(n^2).
// 'steps' is passed by reference so the caller can see how much work was done.
long long bestTradeBrute(const std::vector<long long>& prices, long long& steps) {
    long long best = 0;                       // doing nothing earns 0
    const int n = static_cast<int>(prices.size());
    for (int buy = 0; buy < n; ++buy) {
        for (int sell = buy + 1; sell < n; ++sell) {
            ++steps;
            best = std::max(best, prices[sell] - prices[buy]);
        }
    }
    return best;
}

// One pass: remember the cheapest price seen so far. About n steps: O(n).
long long bestTradeFast(const std::vector<long long>& prices, long long& steps) {
    if (prices.empty()) return 0;
    long long cheapest = prices[0];           // best buy price among days seen so far
    long long best = 0;
    for (long long p : prices) {              // range-for: p is a copy of each price
        ++steps;
        best = std::max(best, p - cheapest);  // sell today, having bought at the cheapest day
        cheapest = std::min(cheapest, p);     // today may become the new cheapest day
    }
    return best;
}

int main() {
    // 1) The examples from the problem statement.
    struct Case { std::vector<long long> prices; long long expected; };
    const std::vector<Case> cases = {
        {{148, 146, 151, 149, 155, 150}, 9},
        {{155, 153, 150, 149}, 0},
        {{150}, 0},
        {{3, 8, 1, 6}, 5},
    };
    for (const auto& c : cases) {
        long long s1 = 0, s2 = 0;
        long long a = bestTradeBrute(c.prices, s1);
        long long b = bestTradeFast(c.prices, s2);
        std::cout << "brute=" << a << " fast=" << b << " expected=" << c.expected << '\n';
        assert(a == c.expected && b == c.expected);
    }

    // 2) Randomized cross-check: the fast answer must equal the brute force answer.
    std::mt19937 rng(2026);
    std::uniform_int_distribution<int> lenDist(0, 60);
    std::uniform_int_distribution<long long> priceDist(1, 1000);
    for (int trial = 0; trial < 20000; ++trial) {
        std::vector<long long> prices(lenDist(rng));
        for (auto& p : prices) p = priceDist(rng);  // auto&: a reference, so we write into the vector
        long long s1 = 0, s2 = 0;
        assert(bestTradeBrute(prices, s1) == bestTradeFast(prices, s2));
    }
    std::cout << "20000 random tests passed\n";

    // 3) Count the steps as n grows.
    std::cout << "n, brute steps, fast steps\n";
    for (int n : {10, 100, 1000, 10000}) {
        std::vector<long long> prices(n);
        for (auto& p : prices) p = priceDist(rng);
        long long s1 = 0, s2 = 0;
        bestTradeBrute(prices, s1);
        bestTradeFast(prices, s2);
        std::cout << n << ", " << s1 << ", " << s2 << '\n';
    }
    return 0;
}
