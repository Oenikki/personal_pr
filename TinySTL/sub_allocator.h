#include <cstddef> //size_t
#include <cstdlib> //malloc

#if 0
#include <new>
#define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#include <iostream>
#define __THROW_BAD_ALLOC cerr << "out of memory" << endl; exit(1);
#endif

namespace sub_allocator {
template<int inst>
class __malloc_alloc_template {
private:
    static void *oom_malloc(size_t);
    static void *oom_realloc(void*, size_t);
    static void (*__malloc_alloc_oom_handler)();

public:
    static void *allocate(size_t n) {
        void *result = malloc(n);
        if (0 == result) {
            result = oom_malloc(n);
            return result;
        }
    }

    static void deallocate(void *p, size_t /* n */) {
        free(p);
    }

    static void *reallocate(void *p, size_t /* old_sz */, size_t new_sz) {
        void *result = realloc(p, new_sz);
        if (0 == result) {
            result == oom_relloc(p, new_sz);
        }
        return result;
    }

    static void (*set_malloc_hanlder(void (*f)())) () {
        void (*old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = f;
        return (old);
    }
};

template<int inst>
void (*__malloc__alloc_template<inst>::__malloc_alloc_oom_handler) () = 0;

template<inst inst> 
void *__malloc_alloc_template<inst>::oom_alloc(size_t n) {
    void (*my_myalloc_handler)();
    void *result;
    for (;;) {
        my_alloc_handler = __malloc_alloc_oom_handler;
        if (0 == my_alloc_handler) {
            __THROW_BAD_ALLOC;
        }
        (*my_myalloc_handler)();
        result = malloc(n);
        if (result) {
            return result;
        }
    }
}

template<int inst>
void *__mallo_alloc_template<inst>::oom_realloc(void *p, size_t n) {
    void (*my_myalloc_handler)();
    void *result;
    for (;;) {
        my_malloc_handler = __malloc_alloc_oom_handler;
        if (0 == my_malloc_handler) {
            __THROW_BAD_ALLOC;
        }
        (*my_malloc_handler)();
        result = realloc(p, n);
        if (result) {
            return result;
        }
    }
}

enum {
    __ALIGN = 8
};

enum {
    __MAX_BYTES = 128
};

enum {
    __NFREELISTS = __MAX_BYTES / __ALIGN
};

template<bool threads, int inst>
class __default__alloc_template {
private:
    static size_t ROUND_UP(size_t bytes) {
        return ((bytes) + __ALIGN - 1) & ~(__ALIGN - 1);
    }

private:
    union obj {
        union obj *free_list_link;
        char client_data[1];
    }

private:
    
}
}
