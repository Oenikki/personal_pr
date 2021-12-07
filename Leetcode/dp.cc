#include <vector>
#include <string>
#include <numeric> //std::accumulate
#include <limits> //INT_MAX
#include <queue>
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
    //BFS
    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
        int m = mat.size();
        int n = mat[0].size();
        vector<vector<int>> visited(m, vector<int>(n, 0));
        vector<vector<int>> ans(m, vector<int>(n, 0));
        queue<pair<int, int>> q;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (mat[i][j] == 0) {
                    q.emplace(i, j);
                    visited[i][j] = 1;
                }
            }
        }
        while (!q.empty())  {
            auto [i, j] = q.front();
            q.pop();
            for (size_t d = 0; d < 4; ++d) {
                int new_i = i + dir[d];
                int new_j = j + dir[d + 1];
                if (0 <= new_i && new_i < m &&
                    0 <= new_j && new_j < n &&
                    !visited[new_i][new_j]) {
                    ans[new_i][new_j] =  ans[i][j] + 1;
                    q.emplace(new_i, new_j);
                    visited[new_i][new_j] = 1;
                }
            }
        }
        return ans;
    }
private:
    vector<int> dir {-1, 0, 1, 0, -1};
};
}

namespace hh221 {
class Solution {
public:
    int maximalSquare(vector<vector<char>>& matrix) {
        int m = matrix.size();
        int n = matrix[0].size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        int max_edge = 0;
        auto fmin = [](int a, int b, int c) {
            return std::min(std::min(a, b), c);
        };
        for (size_t i = 1; i < m + 1; ++i) {
            for (size_t j = 1; j < n + 1; ++j) {
                if (matrix[i - 1][j - 1] == '1') {
                    dp[i][j] = fmin(dp[i - 1][j -1], dp[i - 1][j], dp[i][j - 1]) + 1;
                }
                max_edge = std::max(max_edge, dp[i][j]);
            }
        }
        return max_edge * max_edge;
    }
};
}

namespace hh279 {
class Solution {
public:
    int numSquares(int n) {
        //apparently minimum value only related to
        //index i - 1, i - 4, i - 9 ...
        //so f = 1 + min(f(i - 1) + f(1 - 4) + ...)
        vector<int> dp(n + 1, INT_MAX);
        for (size_t i = 1; i <= n; ++i) {
            for (size_t j = 1; j * j <= i; ++j) {
                dp[i] = min(dp[i], dp[i - j * j] + 1);
            }
        }
        return dp[n];
    }
};
}

namespace hh91 {
class Solution{
public:
    int numDecodings(string s) {
        const int n = s.length();
        vector<int> dp(n + 1, 1);
        int prev = s[0] - '0';
        if (!prev) return 0;
        for (int i = 2; i < n + 1; ++i) {
            int cur = s[i - 1] - '0';
            if ((prev == 0 || prev > 2) && !cur) return 0;
            if ((0 < prev && prev < 2) || prev == 2 && cur < 7) {
                if (cur) {
                    dp[i] = dp[i - 2] + dp[i - 1];
                } else {
                    dp[i] = dp[i - 2];
                }
            } else {
                dp[i] = dp[i - 1];
            }
            prev = cur;
        }
    }
};
}

namespace hh139 {
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        const int n = s.length();
        vector<bool> dp(n + 1, false);
        dp[0] = true;
        for (int i = 1; i < n; ++i) {
            for (const auto& word : wordDict) {
                int len = word.length();
                if (i >= len && s.substr(i - len, len) == word) {
                    dp[i] = dp[i] || dp[i - len];
                }
            }
        }
        return dp[n];
    }
};
}

namespace hh300 {
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        const int n = nums.size();
        vector<int> dp(n, 1);
        int max_length = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (nums[i] > nums[j]) {
                    dp[i] = std::max(dp[i], dp[j]  + 1);
                }
            }
            max_length = std::max(max_length, dp[i]);
        }
        return max_length;
    }
};
}

namespace hh1143 {
class Solution {
public:
    int longestCommonSubsequence(string text1, string text2) {
        const int m = text1.length(), n = text2.length();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        for (int i = 1; i < m; ++i) {
            for (int j = 1; j < n; ++j) {
                if (text1[i - 1] == text2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = std::max(dp[i][j - 1], dp[i - 1][j]);
                }
            }
        }
        return dp[m][n];
    }
};
}

namespace hhknapsack {
    int knapsack(vector<int>& weights, vector<int>& values, int N, int W) {
        vector<vector<int>> dp(N + 1, vector<int>(M + 1, 0));
        for (int i = 1; i < N + 1; ++i) {
            int w = weights[i - 1], v = values[i - 1];
            for (int j = 1; j < W + 1; ++j) {
                if (j >= w) {
                    dp[i][j] = std::max(dp[i - 1][j], dp[i - 1][j - w] + v);
                } else {
                    dp[i][j] = dp[i - 1][j]
                }
            }
        }
        return dp[N][W];
    }

    int knapsack2(vector<int>& weights, vector<int>& values, int N, int W) {
        vector<int> dp(W + 1, 0);
        for (int i = 1; i < N + 1; ++i) {
            int w = weights[i - 1], v = values[i - 1];
            for (int j = W; j >= w; --j) {
                dp[j] = max(dp[j], dp[j - w] + v);
            }
        }
        return dp[W];
    }

    //complete knapsack
    int knapsack3(vector<int>& weights, vector<int>& values, int N, int W) {
        vector<int> dp(M + 1, 0);
        for (int i = 1; i < N + 1; ++i) {
            int w = weight[i - 1], v = values[i - 1];
            for (int j = w; j < W + 1; ++j) {
                dp[j] = std::max(dp[j], dp[i][j - w] + v);
            }
        }
        return dp[M];
    }
}

namespace hh416 {
class Solution {
public:
    bool canPartition(vector<int>& nums) {
        int sum = std::accumulate(nums.begin(), nums.end(), 0);
        if (sum & 1) return false;
        sum /= 2;
        vector<vector<bool>> dp(n, vector<bool>(M + 1, false));
        for (int i = 1; i < n; ++i) {
            for(int j = 1; j < sum + 1; ++j) {
                if (j >= nums[i - 1]) {
                    dp[i][j] = dp[i - 1][j] || dp[i - 1][j - nums[i - 1]];
                } else {
                    dp[i][j] = dp[i - 1][j];
                }
            }
        }
        return dp[n- 1][sum];
    }
};
}

namespace hh474 {
class Solution {
public:
    int findMaxForm(vector<string>& strs, int m, int n) {

    }
};
}
