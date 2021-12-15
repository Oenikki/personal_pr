#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

namespace hh684 {
class UF {
public:
    UF(int n) : id(n) {
        for (size_t i = 0; i < n; ++i) {
            id[i] = i;
        }
    }

    int find(int p) {
        while (p != id[p]) {
            p = id[p];
        }
        return p;
    }

    void connect(int p, int q) {
        id[find(p)] = find(q);
    }

    bool isConnected(int p, int q) {
        return find(p) == find(q);
    }

private:
    vector<int> id;
};

//path compact
class UF2 {
public:
    UF2(int n) : id(n), size(n, 1) {
        for (int i = 0; i < n; ++i) {
            id[i] = i;
        }
    }

    int find(int p) {
        while (p != id[p]) {
            id[p] = id[id[p]];
            p = id[p];
        }
        return p;
    }

    void connect(int p, int q) {
        int p_root = find(p);
        int q_root = find(q);
        if (p_root == q_root) {
            return;
        }
        if (size[p_root] < size[q_root]) {
            id[p_root] = q_root;
            size[q_root] += size[p_root];
        } else {
            id[q_root] = p_root;
            size[p_root] += size[q_root];
        }
    }

    bool isConnect(int p, int q) {
        return find(p) == find(q);
    }


private:
    vector<int> id;
    vector<int> size;
};

class Solution {
public:
    vector<int> findRedundantConnection(vector<vector<int>>& edges) {
        int n = edges.size();
        UF uf(n + 1);
        for (const auto& e : edges) {
            int u = e[0], v = e[1];
            if (uf.isConnected(u, v)) {
                return e;
            }
            uf.connect(u, v);
        }
        return {-1, -1};
    }
};
}

namespace hh146 {
class LRUCache {
public:
    LRUCache(int capacity) {

    }

    int get(int key) {

    }

    void put(int key, int value) {

    }
};
}
