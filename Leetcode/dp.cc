#include <vector>
#include <string>
#include <numeric> //std::accumulate
#include <limits> //INT_MAX
using namespace std;


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
        /*
        vector<int> dp(n + 1, 0);
        dp[1] = nums[0];
        for (int i = 2; i < n + 1; ++i) {
            dp[i] = std::max(dp[i - 1], dp[i - 2] + nums[i - 1]);
        }
        return dp[n];
        */
        //compress
        int dp1 = 0, dp2 = 0, dp;
        for (int i = 2; i < n + 1; ++i) {
            dp = std::max(dp1, dp2 + nums[i - 1]);
            dp2 = dp1;
            dp1 = dp;
        }
        return dp;
    }
};
}

namespace hh413 {
class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& nums) {
        const int n = nums.size();
        vector<int> dp(n, 0);
        for (size_t i = 2; i < n; ++i) {
            if (nums[i] - nums[i - 1] == nums[i - 1] - nums[i - 2])
                dp[i] = dp[i - 1] + 1;
        }
        return std::accumulate(dp.begin(), dp.end(), 0);
    }
};
}

namespace hh64 {
class Solution {
public:
    int minPathSum(vector<vector<int>>& grid) {
        const int m = grid.size();
        const int n = m ? grid[0].size() : 0;
        /*
        vector<vector<int>> dp(m, vector<int>(n, 0));
        for(size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                if (i == 0 && j == 0) {
                    dp[i][j] = grid[i][j];
                } else if (i == 0) {
                    dp[i][j] = dp[i][j - 1] + grid[i][j];
                } else if (j == 0) {
                    dp[i][j] = dp[i - 1][j] + grid[i][j];
                } else {
                    dp[i][j] = std::min(dp[i - 1][j], dp[i][j - 1]) + grid[i][j];
                }
            }
        }
        return dp[m - 1][n - 1];
        */
        //compress
        vector<int> dp(n, 0);
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                if (i == 0 && j == 0) {
                    dp[j] = 0;
                } else if (i == 0) {
                    dp[j] = dp[j - 1] + grid[i][j];
                } else if (j == 0) {
                    dp[j] = dp[j] + grid[i][j];
                } else {
                    dp[j] = std::min(dp[j - 1], dp[j]) + grid[i][j];
                }
            }
        }
        return dp[n - 1];
    }
};
}

namespace hh542 {
class Solution {
public:
    /**
     * method 1:
        dp twice, once right and down, once left and up;
     * method 2:
        BFS
     */
    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
        const int m = mat.size();
        const int n = mat[0].size();
        vector<vector<int>> dp(m, vector<int>(n, INT_MAX / 2));
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (mat[i][j] == 0) {
                    dp[i][j] = 0;
                } else {
                    if (i > 0) {
                        dp[i][j] = std::min(dp[i][j], dp[i - 1][j] + 1);
                    }
                    if (j > 0) {
                        dp[i][j] = std::min(dp[i][j], dp[i][j - 1] + 1);
                    }
                }
            }
        }

        for (int i = m - 1; i >= 0; --i) {
            for (int j = n - 1; j >= 0; --j) {
                if (mat[i][j]) {
                    if (i < m - 1) {
                        dp[i][j] = std::min(dp[i][j], dp[i + 1][j] + 1);
                    }
                    if (j < n - 1) {
                        dp[i][j] = std::min(dp[i][j], dp[i][j + 1] + 1);
                    }
                }
            }
        }
        return dp;
    }
};

class Solution2 {
public:
    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
        int m = mat.size();
        int n = mat[0].size();
        vector<vector<int>> visited(m, vector<int>(n, 0));

        queue<pair<int, int>> q;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if ()
            }
        }
    }
};
}
