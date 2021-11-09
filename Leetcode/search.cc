#include <vector>
#include <stack>
#include <utility> //pair
#include <iostream>
#include <cstdlib> //size_t
#include <algorithm> //std::max

using namespace std;

namespace hh695 {
class Solution {
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        const int m = grid.size();
        const int n = m ? grid[0].size() : 0;
        int local_area = 0;
        int area = 0;
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                if (!grid[i][j]) {
                    continue;
                }
                grid[i][j] = 0;
                local_area = 1;
                stack<pair<int, int>> island_coord;
                island_coord.push({i, j});
                while (!island_coord.empty()) {
                    auto cur_coord = island_coord.top();
                    island_coord.pop();
                    for (size_t d = 0; d < 4; ++d) {
                        int new_i = cur_coord.first + dir[d];
                        int new_j = cur_coord.second + dir[d + 1];
                        if (0 <= new_i && new_i < m &&
                            0 <= new_j && new_j < n &&
                            grid[new_i][new_j]) {
                            grid[new_i][new_j] = 0;
                            ++local_area;
                            island_coord.push({new_i, new_j});
                        }
                    }
                }
                area = std::max(area, local_area);
            }
        }
        return area;
    }

private:
    vector<int> dir {-1, 0, 1, 0, -1};
};

class Solution1 {
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        const int m = grid.size();
        const int n = m ? grid[0].size() : 0;
        int area = 0;
        for (size_t i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                if (grid[i][j]) {
                    area = std::max(area, dfs(grid, i, j));
                }
            }
        }
        return area;
    }

private:
    int dfs(vector<vector<int>>& grid, int i, int j) {
        if (grid[i][j] == 0) {
            return 0;
        }
        grid[i][j] = 0;
        int area = 1;
        for (size_t d = 0; d < 4; ++d) {
            int new_i = i + dir[d];
            int new_j = j + dir[d + 1];
            if (0 <= new_i && new_i < grid.size() &&
                0 <= new_j && new_j < grid[0].size()) {
                area += dfs(grid, new_i, new_j);
            }
        }
        return area;
    }
private:
    vector<int> dir {-1, 0, 1, 0, -1};
};
}

namespace hh547 {
class Solution {
public:
    int findCircleNum(vector<vector<int>>& isConnected) {
        int num_province = 0;
        vector<bool> visited(isConnected.size(), false);
        for (size_t i = 0; i < isConnected.size(); ++i) {
            if (!visited[i]) {
                dfs(isConnected, i, visited);
                ++num_province;
            }
        }
        return num_province;
    }
private:
    void dfs(vector<vector<int>>& isConnected, int i, vector<bool>& visited) {
        visited[i] = true;
        for (size_t j = 0; j < isConnected.size(); ++j) {
            if (isConnected[i][j] == 1 && !visited[j]) {
                dfs(isConnected, j, visited);
            }
        }
    }
};
}

namespace hh417 {
class Solution {
public:
    vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
        const int m = height.size();
        const int n = m ? height[0].size() : 0;
        vector<vector<int>> ans;
        vector<vector<bool>> can_reach_pacific(m, vector<bool>(n, false));
        vector<vector<bool>> can_reach_atlantic(n, vector<bool>(n, false));
        for (int i = 0; i < m; ++i) {
            dfs(heights, can_reach_pacific, i, 0);
            dfs(heights, can_reach_atlantic, i, n - 1);
        }

        for (int i = 0; i < n; ++i) {
            dfs(heights, can_reach_pacific, 0, i);
            dfs(heights, can_reach_atlantic, m - 1, i);
        }

        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (can_reach_atlantic[i][j] && can_reach_pacific[i][j]) {
                    ans.push_back({i, j});
                }
            }
        }
        return ans;
    }
private:
    void dfs(vector<vector<int>>& heights, vector<vector<bool>>& rpa, int i, int j) {
        if (rpa[i][j]) {
            return;
        }
        rpa[i][j] = true;
        for (size_t d = 0; d < 4; ++d) {
            int new_i = i + dir[d];
            int new_j = j + dir[d + 1];
            if (0 <= new_i && new_i < heights.size() &&
                0 <= new_j && new_j < heights[0].size() &&
                heights[new_i][new_j] >= heights[i][j]) {
                dfs(height, rpa, new_i, new_j);
            }
        }
    }
private:
    vector<int> dir {-1, 0, 1, 0, -1};
};
}

int main() {
    return 0;
}
