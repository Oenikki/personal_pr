#include <iostream>
#include <thread>
#include <iterator>
#include <algorithm>
#include <functional>
#include <numeric> //std::accumulate
#include <vector>
using namespace std;

namespace simple_test1 {
void hello() {
    cout << "hello concurrency world\n";
}

int main()
{
    std::thread t(hello);
    t.join();
    return 0;
}
}

namespace t1 {
class background_task {
public:
    void operator()() {
        do_something();
        do_something_else();
    }

    static void do_something() {}
    static void do_something_else() {}
};

void test() {
    background_task f;
    std::thread my_thread(f);
    std::thread my_thread1{ background_task() };
    std::thread my_thread2([]{
        background_task::do_something();
        background_task::do_something_else();
    });
}

class thread_guard {
public:
    explicit thread_guard(std::thread& t) : t_(t) {}
    ~thread_guard() {
        if (t_.joinable()) {
            t_.join();
        }
    }
    thread_guard(const thread_guard&) = delete;
    thread_guard& operator=(const thread_guard&) = delete;

private:
    std::thread& t_;
};

void test2() {
    int local_state = 0;
    /*
    func my_func(local_state);
    std::thread t(my_func);
    thread_guard g(t);
    */
}

}

namespace t2 {
template<typename Iterator, typename T>
struct accumulate_block {
    void operator()(Iterator first, Iterator last, T& result) {
        result = std::accumulate(first, last, result);
    }
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init) {
    const unsigned long length = std::distance(first, last);
    if (!length) {
        return init;
    }
    const unsigned long min_per_thread = 25;
    const unsigned long max_threads =
        (length + min_per_thread - 1) / min_per_thread;
    const unsigned long hardware_threads = std::thread::hardware_concurrency();
    const unsigned long num_threads =
        std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    const unsigned long block_size = length / num_threads;
    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(num_threads - 1);

    Iterator block_start = first;
    for (unsigned long i = 0; i < num_threads - 1; ++i) {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(accumulate_block<Iterator, T>(), block_start,
                                 block_end, std::ref(results[i]));
        block_start = block_end;
    }
    accumulate_block<Iterator, T>()(
        block_start, last, results[num_threads - 1]);
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    return std::accumulate(results.begin(), results.end(), init);
}

}

int main()
{
    cout << "hello" << endl;
    return 0;
}
