// Daily Quant Digest 2026-10-21
// Algorithms & DS: Shortest paths (Dijkstra), "Number of Ways to Arrive at Destination"
// Compile: g++ -std=c++17 -O2 -Wall 2026-10-21.cpp -o ways && ./ways

#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <random>
#include <utility>
#include <vector>

using std::vector;
using ll = long long;

constexpr ll MOD = 1'000'000'007LL;
constexpr ll INF = std::numeric_limits<ll>::max();

// ---------------------------------------------------------------------------
// Solution: Dijkstra from node 0, carrying a second array ways[v] =
// number of shortest paths from 0 to v (mod 1e9+7).
//   * Found a strictly shorter route to v  -> reset ways[v] = ways[u].
//   * Found another route of equal length  -> ways[v] += ways[u].
// Because every edge has positive weight, when u is popped with its final
// distance, every node that can precede u on a shortest path has already
// been popped, so ways[u] is final before u passes it on.
// Time O((n + m) log m), space O(n + m).
// ---------------------------------------------------------------------------
int countPaths(int n, const vector<std::array<int, 3>>& roads) {
    // Adjacency list: adj[u] holds (neighbour, travel time) pairs.
    vector<vector<std::pair<int, int>>> adj(n);
    for (const auto& r : roads) {
        adj[r[0]].push_back({r[1], r[2]});
        adj[r[1]].push_back({r[0], r[2]});   // roads are two-way
    }

    vector<ll> dist(n, INF);
    vector<ll> ways(n, 0);
    dist[0] = 0;
    ways[0] = 1;

    // Min-heap of (distance, node). std::greater flips the default max-heap.
    using Item = std::pair<ll, int>;
    std::priority_queue<Item, vector<Item>, std::greater<Item>> pq;
    pq.push({0, 0});

    while (!pq.empty()) {
        auto [d, u] = pq.top();   // structured binding (C++17)
        pq.pop();
        if (d > dist[u]) continue;  // stale entry: u was already settled cheaper

        for (const auto& [v, w] : adj[u]) {
            ll nd = d + w;
            if (nd < dist[v]) {            // strictly better route
                dist[v] = nd;
                ways[v] = ways[u];
                pq.push({nd, v});
            } else if (nd == dist[v]) {    // a tie: one more family of routes
                ways[v] = (ways[v] + ways[u]) % MOD;
            }
        }
    }
    return static_cast<int>(ways[n - 1]);
}

// ---------------------------------------------------------------------------
// Brute force for testing: enumerate every simple path from 0 to n-1 by DFS,
// keep the minimum total time and how many paths achieve it. Exponential,
// so only for tiny graphs. (A shortest path with positive weights is always
// simple, so simple paths are enough.)
// ---------------------------------------------------------------------------
int bruteForce(int n, const vector<std::array<int, 3>>& roads) {
    vector<vector<std::pair<int, int>>> adj(n);
    for (const auto& r : roads) {
        adj[r[0]].push_back({r[1], r[2]});
        adj[r[1]].push_back({r[0], r[2]});
    }
    ll best = INF;
    ll count = 0;
    vector<bool> onPath(n, false);

    std::function<void(int, ll)> dfs = [&](int u, ll t) {
        if (t > best) return;               // prune: already too slow
        if (u == n - 1) {
            if (t < best) { best = t; count = 1; }
            else if (t == best) ++count;
            return;
        }
        onPath[u] = true;
        for (const auto& [v, w] : adj[u])
            if (!onPath[v]) dfs(v, t + w);
        onPath[u] = false;
    };
    dfs(0, 0);
    return static_cast<int>(count % MOD);
}

int main() {
    // Example 1 (four shortest routes of length 7).
    vector<std::array<int, 3>> ex1 = {
        {0, 6, 7}, {0, 1, 2}, {1, 2, 3}, {1, 3, 3}, {6, 3, 3},
        {3, 5, 1}, {6, 5, 1}, {2, 5, 1}, {0, 4, 5}, {4, 6, 2}};
    int a1 = countPaths(7, ex1);
    std::cout << "Example 1: " << a1 << " (expected 4)\n";
    assert(a1 == 4);

    // Example 2: a single road.
    int a2 = countPaths(2, {{1, 0, 10}});
    std::cout << "Example 2: " << a2 << " (expected 1)\n";
    assert(a2 == 1);

    // Example 3: a ladder of 30 "diamonds", each with two equal routes.
    // The count is 2^30 mod 1e9+7, which tests the modulo.
    {
        const int k = 30;
        int n = 3 * k + 1;
        vector<std::array<int, 3>> r;
        for (int i = 0; i < k; ++i) {
            int s = 3 * i, top = s + 1, bot = s + 2, e = s + 3;
            r.push_back({s, top, 1});
            r.push_back({top, e, 1});
            r.push_back({s, bot, 1});
            r.push_back({bot, e, 1});
        }
        ll expect = 1;
        for (int i = 0; i < k; ++i) expect = expect * 2 % MOD;
        int a3 = countPaths(n, r);
        std::cout << "Example 3 (2^30 mod p): " << a3 << " (expected " << expect << ")\n";
        assert(a3 == expect);
    }

    // Randomized cross-check against brute force on small connected graphs.
    // Small weights (1..3) create lots of ties, the hard case.
    std::mt19937 rng(20261021);
    int tests = 0;
    for (int iter = 0; iter < 3000; ++iter) {
        int n = 2 + rng() % 8;                         // 2..9 nodes
        vector<std::array<int, 3>> r;
        vector<vector<bool>> used(n, vector<bool>(n, false));
        auto addEdge = [&](int u, int v) {
            if (u == v || used[u][v]) return;
            used[u][v] = used[v][u] = true;
            r.push_back({u, v, 1 + static_cast<int>(rng() % 3)});
        };
        for (int v = 1; v < n; ++v) addEdge(v, rng() % v);   // spanning tree
        int extra = rng() % (n * 2);
        for (int e = 0; e < extra; ++e) addEdge(rng() % n, rng() % n);

        int fast = countPaths(n, r);
        int slow = bruteForce(n, r);
        if (fast != slow) {
            std::cout << "MISMATCH on iteration " << iter << ": " << fast
                      << " vs " << slow << "\n";
            return 1;
        }
        ++tests;
    }
    std::cout << "Random cross-check passed on " << tests << " graphs\n";
    return 0;
}
