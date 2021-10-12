//leetcode 455 & 135
#include <iostream>
#include <vector>
#include <algorithm> //std::max
#include <numeric> //std::accumulate

namespace hh145 {
class Solution {
public:
    int findContentChildren(std::vector<int>& g, std::vector<int>& s) {
        std::sort(g.begin(), g.end());
        std::sort(s.begin(), s.end());
        int num_child = 0, num_cookies = 0;
        while (num_child < g.size() && num_cookies < s.size()) {
            if (g[num_child] <= s[num_cookies++]) {
                num_child++;
            }
        }
        return num_child;
    }
};

}

namespace hh455 {
class Solution {
public:
    int candy(std::vector<int>& ratings) {
        std::vector<int> candy(ratings.size(), 1);
        if (ratings.size() < 2) {
            return ratings.size();
        }
        //traverse ratings from left to right
        //if next ratings is greater than the former, plus one
        for (size_t i = 1; i < ratings.size(); ++i) {
            if (ratings[i] > ratings[i - 1]) {
                candy[i] = candy[i - 1] + 1;
            }
        }
        //traverse ratings from right to left
        for (size_t i = ratings.size() - 1; i > 0; --i) {
            if (ratings[i - 1] > ratings[i]) {
                candy[i - 1] = std::max(candy[i - 1], candy[i] + 1);
            }
        }
        return std::accumulate(candy.begin(), candy.end(), 0);
    }
};
}

namespace hh435 {
class Solution {
    int eraseOverlapIntervals(std::vector<std::vector<int>>& intervals) {
        if (intervals.empty()) {
            return 0;
        }
        std::sort(intervals.begin(), intervals.end(),
            [](std::vector<int>& interval1, std::vector<int>& interval2) {
            return interval1[1] < interval2[1];
        });
        int num_erase = 0;
        int prev = intervals[0][1];
        for (size_t i = 1; i < intervals.size(); ++i) {
            if (intervals[i][0] < prev) {
                ++num_erase;
            } else {
                prev = intervals[i][1];
            }
        }
        return num_erase;
    }
};
}

int main() {
    std::vector<int> g;
    std::vector<int> s;
    std::cout << "enter children's desire: " << std::endl;
    int val;
    while (std::cin >> val) {
        g.push_back(val);
        if (std::cin.get() == '\n') break;
    }
    std::cout << "enter cookies's size: " << std::endl;
    int sz;
    while (std::cin >> sz) {
        s.push_back(sz);
        if (std::cin.get() == '\n') break;
    }
    std::cout << "result:" << hh145::Solution().findContentChildren(g, s);
    return 0;
}
