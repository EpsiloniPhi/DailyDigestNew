// Daily Quant Digest 2026-10-16
// Linked lists: Reverse Nodes in k-Group
// Compile: g++ -std=c++17 -O2 -Wall 2026-10-16.cpp -o kgroup && ./kgroup

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    explicit ListNode(int v, ListNode* n = nullptr) : val(v), next(n) {}
};

// Reverses the nodes of the list k at a time and returns the new head.
// A final group with fewer than k nodes is left in its original order.
// Only next pointers are rewired: O(n) time, O(1) extra space.
ListNode* reverseKGroup(ListNode* head, int k) {
    if (head == nullptr || k <= 1) return head;

    ListNode dummy(0, head);    // sentinel on the stack: no special case for the head
    ListNode* groupPrev = &dummy;  // node just before the current group

    while (true) {
        // 1. Find the k-th node of this group (or discover there are fewer than k left).
        ListNode* kth = groupPrev;
        for (int i = 0; i < k && kth != nullptr; ++i) kth = kth->next;
        if (kth == nullptr) break;   // short tail: leave it alone

        ListNode* groupStart = groupPrev->next;  // becomes the group's tail after reversal
        ListNode* groupNext = kth->next;         // first node after the group

        // 2. Reverse the k nodes in place. Seeding prev with groupNext means the
        //    old first node ends up pointing to the rest of the list automatically.
        ListNode* prev = groupNext;
        ListNode* cur = groupStart;
        while (cur != groupNext) {
            ListNode* nxt = cur->next;
            cur->next = prev;
            prev = cur;
            cur = nxt;
        }

        // 3. Stitch: the node before the group now points at the old k-th node.
        groupPrev->next = kth;
        groupPrev = groupStart;
    }
    return dummy.next;
}

// ---------- helpers for testing ----------

ListNode* build(const vector<int>& v) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    for (int x : v) {
        tail->next = new ListNode(x);
        tail = tail->next;
    }
    return dummy.next;
}

vector<int> toVector(const ListNode* head) {
    vector<int> out;
    for (const ListNode* p = head; p != nullptr; p = p->next) out.push_back(p->val);
    return out;
}

void destroy(ListNode* head) {
    while (head != nullptr) {
        ListNode* nxt = head->next;
        delete head;
        head = nxt;
    }
}

// Reference answer computed on a plain vector with std::reverse.
vector<int> reference(vector<int> v, int k) {
    if (k <= 1) return v;
    for (size_t i = 0; i + k <= v.size(); i += k)
        reverse(v.begin() + i, v.begin() + i + k);
    return v;
}

string show(const vector<int>& v) {
    string s = "[";
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) s += ",";
        s += to_string(v[i]);
    }
    return s + "]";
}

int main() {
    struct Case { vector<int> in; int k; vector<int> expected; };
    vector<Case> cases = {
        {{1, 2, 3, 4, 5}, 2, {2, 1, 4, 3, 5}},
        {{1, 2, 3, 4, 5}, 3, {3, 2, 1, 4, 5}},
        {{1, 2, 3, 4, 5}, 1, {1, 2, 3, 4, 5}},
        {{1, 2, 3, 4, 5}, 5, {5, 4, 3, 2, 1}},
        {{1, 2, 3, 4, 5, 6}, 3, {3, 2, 1, 6, 5, 4}},
        {{7}, 1, {7}},
    };

    for (const auto& c : cases) {
        ListNode* head = build(c.in);
        head = reverseKGroup(head, c.k);
        vector<int> got = toVector(head);
        cout << show(c.in) << ", k=" << c.k << " -> " << show(got) << "\n";
        assert(got == c.expected);
        destroy(head);
    }

    // Randomised check against the vector reference.
    mt19937 rng(42);
    for (int trial = 0; trial < 2000; ++trial) {
        int n = rng() % 40 + 1;
        int k = rng() % n + 1;
        vector<int> v(n);
        for (int& x : v) x = rng() % 1000;
        ListNode* head = reverseKGroup(build(v), k);
        assert(toVector(head) == reference(v, k));
        destroy(head);
    }
    cout << "All tests passed.\n";
    return 0;
}
