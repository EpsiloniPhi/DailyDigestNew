// Daily Quant Digest 2026-10-17
// Distance between two nodes in a binary tree, many queries.
// Lowest common ancestor via binary lifting: O(n log n) preprocessing, O(log n) per query.
// Compile: g++ -std=c++17 -O2 -Wall 2026-10-17.cpp -o lca && ./lca

#include <array>
#include <cassert>
#include <iostream>
#include <queue>
#include <random>
#include <utility>
#include <vector>

class TreeLCA {
public:
    static constexpr int LOG = 17;  // 2^17 = 131072 > 1e5 nodes

    // left[i], right[i] are the children of node i (-1 if missing). Root is `root`.
    TreeLCA(const std::vector<int>& left, const std::vector<int>& right, int root = 0)
        : n_(static_cast<int>(left.size())), depth_(n_, 0), up_(n_) {
        // BFS from the root: sets depth and the direct parent (up_[v][0]).
        // Iterative on purpose: a skewed tree of 1e5 nodes would overflow a recursive DFS.
        std::queue<int> q;
        q.push(root);
        up_[root].fill(root);  // the root's ancestors are all the root itself
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int c : {left[v], right[v]}) {  // range-for over an initializer_list
                if (c == -1) continue;
                depth_[c] = depth_[v] + 1;
                up_[c][0] = v;
                q.push(c);
            }
        }
        // up_[v][j] = 2^j-th ancestor = 2^(j-1)-th ancestor of the 2^(j-1)-th ancestor.
        // Loop j outside so that level j-1 is complete for every node before level j.
        for (int j = 1; j < LOG; ++j)
            for (int v = 0; v < n_; ++v)
                up_[v][j] = up_[up_[v][j - 1]][j - 1];
    }

    int lca(int u, int v) const {
        if (depth_[u] < depth_[v]) std::swap(u, v);  // make u the deeper node
        // Step 1: lift u up by the depth difference, one binary digit at a time.
        int diff = depth_[u] - depth_[v];
        for (int j = 0; diff > 0; ++j, diff >>= 1)
            if (diff & 1) u = up_[u][j];
        if (u == v) return u;  // v was an ancestor of u
        // Step 2: lift both while their ancestors differ, largest jumps first.
        for (int j = LOG - 1; j >= 0; --j) {
            if (up_[u][j] != up_[v][j]) {
                u = up_[u][j];
                v = up_[v][j];
            }
        }
        return up_[u][0];  // now u and v are children of the LCA
    }

    int distance(int u, int v) const {
        return depth_[u] + depth_[v] - 2 * depth_[lca(u, v)];
    }

    int depth(int v) const { return depth_[v]; }

private:
    int n_;
    std::vector<int> depth_;
    std::vector<std::array<int, LOG>> up_;  // contiguous rows of 17 ints per node
};

// Brute force for testing: walk the deeper node up via parent pointers. O(n) per query.
int bruteDistance(const std::vector<int>& parent, const std::vector<int>& depth, int u, int v) {
    int d = 0;
    while (u != v) {
        if (depth[u] >= depth[v]) u = parent[u];
        else v = parent[v];
        ++d;
    }
    return d;
}

int main() {
    /* Example tree:
               0
              / \
             1   2
            / \   \
           3   4   5
              / \
             6   7
    */
    std::vector<int> left  = {1, 3, -1, -1, 6, -1, -1, -1};
    std::vector<int> right = {2, 4,  5, -1, 7, -1, -1, -1};
    TreeLCA t(left, right);

    struct Case { int u, v, lca, dist; };
    std::vector<Case> cases = {
        {6, 7, 4, 2}, {3, 7, 1, 3}, {6, 5, 0, 5}, {4, 4, 4, 0}, {1, 6, 1, 2}, {0, 7, 0, 3}};
    for (const auto& c : cases) {
        int l = t.lca(c.u, c.v), d = t.distance(c.u, c.v);
        std::cout << "lca(" << c.u << "," << c.v << ") = " << l << ", distance = " << d << '\n';
        assert(l == c.lca && d == c.dist);
    }

    // Randomised test against the brute force, including a fully skewed tree (a path).
    std::mt19937 rng(17);
    for (int trial = 0; trial < 200; ++trial) {
        int n = 1 + static_cast<int>(rng() % 300);
        std::vector<int> L(n, -1), R(n, -1), parent(n, 0), dep(n, 0);
        bool path = (trial % 10 == 0);
        for (int v = 1; v < n; ++v) {
            // Attach v as a free child of a random earlier node (or of v-1 for a path).
            while (true) {
                int p = path ? v - 1 : static_cast<int>(rng() % v);
                int& slot = (rng() & 1) ? L[p] : R[p];  // reference into L or R
                if (slot == -1) { slot = v; parent[v] = p; dep[v] = dep[p] + 1; break; }
            }
        }
        TreeLCA tt(L, R);
        for (int q = 0; q < 200; ++q) {
            int u = static_cast<int>(rng() % n), v = static_cast<int>(rng() % n);
            assert(tt.distance(u, v) == bruteDistance(parent, dep, u, v));
        }
    }

    // Stress: a path of 100000 nodes (worst case depth), deepest node vs root.
    int big = 100000;
    std::vector<int> L(big, -1), R(big, -1);
    for (int v = 0; v + 1 < big; ++v) L[v] = v + 1;
    TreeLCA path(L, R);
    assert(path.distance(big - 1, 0) == big - 1);
    assert(path.lca(big - 1, 54321) == 54321);

    std::cout << "All tests passed.\n";
    return 0;
}
