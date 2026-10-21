// Daily Quant Digest 2026-10-22
// Union-Find: Number of Islands II (online island counting)
// Compile: g++ -std=c++17 -O2 -Wall 2026-10-22.cpp && ./a.out
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <numeric>
#include <queue>
#include <random>
#include <utility>
#include <vector>

using std::vector;

// Disjoint Set Union with path halving and union by size.
class DSU {
public:
    explicit DSU(int n) : parent_(n), size_(n, 1) {
        std::iota(parent_.begin(), parent_.end(), 0);  // every cell is its own boss
    }

    int find(int x) {
        while (parent_[x] != x) {
            parent_[x] = parent_[parent_[x]];  // path halving: skip a level
            x = parent_[x];
        }
        return x;
    }

    // Returns true if a and b were in different sets (a real merge happened).
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (size_[a] < size_[b]) std::swap(a, b);  // attach smaller tree under larger
        parent_[b] = a;
        size_[a] += size_[b];
        return true;
    }

private:
    vector<int> parent_;
    vector<int> size_;
};

vector<int> numIslands2(int m, int n, const vector<std::array<int, 2>>& positions) {
    DSU dsu(m * n);
    vector<char> land(m * n, 0);
    vector<int> answer;
    answer.reserve(positions.size());
    int islands = 0;

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    for (const auto& [r, c] : positions) {
        const int id = r * n + c;
        if (land[id]) {                 // duplicate position: nothing changes
            answer.push_back(islands);
            continue;
        }
        land[id] = 1;
        ++islands;                      // a brand new one-cell island
        for (int k = 0; k < 4; ++k) {
            const int nr = r + dr[k], nc = c + dc[k];
            if (nr < 0 || nr >= m || nc < 0 || nc >= n) continue;
            const int nid = nr * n + nc;
            if (land[nid] && dsu.unite(id, nid)) --islands;  // two islands became one
        }
        answer.push_back(islands);
    }
    return answer;
}

// ---------------- testing ----------------

// Brute force: after each addition, count islands from scratch with BFS.
vector<int> bruteForce(int m, int n, const vector<std::array<int, 2>>& positions) {
    vector<vector<int>> grid(m, vector<int>(n, 0));
    vector<int> out;
    for (const auto& [r, c] : positions) {
        grid[r][c] = 1;
        vector<vector<int>> seen(m, vector<int>(n, 0));
        int count = 0;
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j) {
                if (!grid[i][j] || seen[i][j]) continue;
                ++count;
                std::queue<std::pair<int, int>> q;
                q.push({i, j});
                seen[i][j] = 1;
                while (!q.empty()) {
                    auto [x, y] = q.front();
                    q.pop();
                    const int dx[4] = {-1, 1, 0, 0}, dy[4] = {0, 0, -1, 1};
                    for (int k = 0; k < 4; ++k) {
                        int a = x + dx[k], b = y + dy[k];
                        if (a >= 0 && a < m && b >= 0 && b < n && grid[a][b] && !seen[a][b]) {
                            seen[a][b] = 1;
                            q.push({a, b});
                        }
                    }
                }
            }
        out.push_back(count);
    }
    return out;
}

void print(const vector<int>& v) {
    std::cout << "[";
    for (size_t i = 0; i < v.size(); ++i) std::cout << (i ? "," : "") << v[i];
    std::cout << "]\n";
}

int main() {
    // Example 1
    auto a1 = numIslands2(3, 3, {{0, 0}, {0, 1}, {1, 2}, {2, 1}});
    print(a1);
    assert((a1 == vector<int>{1, 1, 2, 3}));

    // Example 2
    auto a2 = numIslands2(1, 1, {{0, 0}});
    print(a2);
    assert((a2 == vector<int>{1}));

    // Example 3: a bridge cell merges three islands at once, plus a duplicate
    auto a3 = numIslands2(3, 3, {{0, 1}, {1, 0}, {1, 2}, {2, 1}, {1, 1}, {1, 1}});
    print(a3);
    assert((a3 == vector<int>{1, 2, 3, 4, 1, 1}));

    // Randomized cross-check against the brute force
    std::mt19937 rng(20261022);
    for (int t = 0; t < 2000; ++t) {
        int m = 1 + rng() % 7, n = 1 + rng() % 7;
        int k = 1 + rng() % (2 * m * n);
        vector<std::array<int, 2>> pos(k);
        for (auto& p : pos) p = {int(rng() % m), int(rng() % n)};
        assert(numIslands2(m, n, pos) == bruteForce(m, n, pos));
    }

    // Large test: full 100 x 100 grid filled in random order ends with 1 island
    {
        int m = 100, n = 100;
        vector<std::array<int, 2>> pos;
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j) pos.push_back({i, j});
        std::shuffle(pos.begin(), pos.end(), rng);
        auto res = numIslands2(m, n, pos);
        assert(res.back() == 1);
    }
    std::cout << "All tests passed.\n";
    return 0;
}
