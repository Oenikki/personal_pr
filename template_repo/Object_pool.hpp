/**
 * main structure:
 * (1) free memory block constructed by linked list like. (free list)
 * (2) multi memory block space: chunk, meanwhile, chunks are organized by linked list. (chunk list)
 */

#pragma once
#include <cstddef>
#include <algorithm>
#include <new>

#ifndef MYLIB_NUM_NODES_PER_CHUNK
#define MYLIB_NUM_NODES_PER_CHUNK 128
#endif // MYLIB_NUM_NODES_PER_CHUNK

namespace mylib {

class ObjectPool {
private:
    typedef unsigned char counter_type;
public:
    ObjectPool(size_t _node_size)
        : chunk_data_head(nullptr),
          free_node_head(nullptr),
          node_size(_node_size),
          free_node_size(free_node_offset + std::max(sizeof(free_node*), node_size)),
          chunk_size(chunk_data_size + free_node_size * num_nodes_per_chunk) {}
    ~ObjectPool() {
        while (chunk_data_head) {
            chunk_data *chunk = chunk_data_head;
            chunk_data_head = chunk_data_head->next;
            operator delete (chunk);
        }
    }

    void *allocate() throw (std::bad_alloc);
    void deallocate(void *ptr);
    size_t recycle();

private:
    struct chunk_data {
        counter_type free_node_count;
        chunk_data *next;
    };

    struct free_node {
        counter_type bias; //index of free memory in chunk
        free_node *next;
    };

private:
    static const counter_type num_nodes_per_chunk = MYLIB_NUM_NODES_PER_CHUNK;
    static const size_t chunk_data_size = sizeof(chunk_data);
    static const size_t free_node_offset = offsetof(free_node, next);
    chunk_data *chunk_data_head;
    free_node *free_node_head;
    const size_t node_size;
    const size_t free_node_size;
    const size_t chunk_size;
};

class ObjectPoolArray {
public:
    ObjectPoolArray(size_t _sz, size_t _step)
        : sz(_sz),
          array(static_cast<ObjectPool*>(operator new (sizeof(ObjectPool) * _sz))) {
        for (size_t i = 0; i < sz; ++i) {
            new (array + i) ObjectPool(i * _step + _step);
        }
    }

    ~ObjectPoolArray() {
        for (size_t i = 0; i < sz; ++i) {
            (array + i)->~ObjectPool();
        }
        operator delete (array);
    }

    size_t size() const { return sz; }
    ObjectPool& operator[] (size_t n) { return array[n]; }
private:
    size_t sz;
    ObjectPool *array;
};

}


