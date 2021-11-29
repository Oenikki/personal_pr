#include <vector>
#include <string>


namespace hh509 {
class Solution {
public:
    int fib(int n) {
        if (n <= 1) return n;
        cached.resize(n + 1, 0);
        return helper(n);
    }
private:
    int helper(int i) {
        if (i <= 1) return i;
        if (cached[i]) return cached[i];
        return cached[i] = helper(i - 1) + helper(i - 2);
    }
private:
    std::vector<int> cached;
};

class Solution2 {
public:
    int fib(int n) {
        if (n < 2) return n;
        int f = 0;
        int g = 1;
        for (int i = 2; i <= n; ++i) {
            g = f + g;
            f = g - f;
        }
        return g;
    }
};
}

namespace hh1137 {
class Solution {
public:
    int tribonacci(int n) {
        if (n <= 1) return n;
        if (n == 2) return 1;
        int f = 0, g = 1, h = 1;
        for (int i = 3; i <= n; ++i) {
            h = f + g + h;
            g = h - f - g;
            f = h - f - g;
        }
        return h;
    }
};
}

namespace hh70 {
class Solution{
public:
    int climbStairs(int n) {
        if (n < 0) return 0;
        if (n <= 2) return n;
        vector<int> dp(n + 1, 1);
        for (int i = 2; i <= n; ++i) {
            dp[i] = dp[i - 1] + dp[i - 2];
        }
        //can left food on right food to climb sky :)
        int f = 1, g = 1;
        for (size_t i = 2; i < n + 1; ++i) {
            g = f + g;
            f = g - f;
        }
        return g; //g <-> dp[n]
    }
};
}

namespace hh198 {
class Solution{
public:
    int rob(vector<int>& nums) {
        //dp[n] = max(dp[n - 1], dp[n - 2] + nums[n - 1]
        const int n = nums.size();
        vector<int> dp(n + 1, 0);
        dp[1] = nums[0];
        for (int i = 2; i < n + 1; ++i) {
            dp[i] = std::max(dp[i - 1], dp[i - 2] + nums[i - 1]);
        }
        return dp[n];
    }
};
}

