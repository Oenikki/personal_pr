#include <iostream>
#include <vector>
#include <cstdlib>
#include <unordered_map>
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
struct DLinkedNode {
    int key, value;
    DLinkedNode *prev;
    DLinkedNode *next;
    DLinkedNode() : key(0), value(0), prev(nullptr), next(nullptr) {}
    DLinkedNode(int key, int value)
        : key(key), value(value),
          prev(nullptr), next(nullptr) {}
};

class LRUCache {
public:
    LRUCache(int capacity) : capacity(capacity) {
        head = new DLinkedNode();
        tail = new DLinkedNode();
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        if (!cache.count(key)) {
            return -1;
        }
        auto node = cache.at(key);
        moveToHead(node);
        return node->value;
    }

    void put(int key, int value) {
        if (cache.count(key)) {
            auto node = cache.at(key);
            node->value = value;
            moveToHead(node);
        } else {
            auto new_node = new DLinkedNode(key, value);
            cache.emplace(key, new_node);
            addToHead(new_node);
            ++size;
            if (size > capacity) {
                auto remove_node = removeTail();
                cache.erase(remove_node->key);
                delete remove_node;
                --size;
            }

        }
    }

    void moveToHead(DLinkedNode *node) {
        removeNode(node);
        addToHead(node);
    }

    void removeNode(DLinkedNode *node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void addToHead(DLinkedNode *node) {
        node->next = head->next;
        head->next->prev = node;
        node->prev = head;
        head->next = node;
    }

    DLinkedNode *removeTail() {
        auto remove_node = tail->prev;
        removeNode(remove_node);
        return remove_node;
    }
private:
    unordered_map<int, DLinkedNode*> cache;
    DLinkedNode *head;
    DLinkedNode *tail;
    int size = 0;
    int capacity;
};
}
