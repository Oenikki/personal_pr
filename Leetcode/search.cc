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
        const int n = m ? gird[0].size() : 0;
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
                0 <= new_j && new_j < grid[0].size() &&
                ) {
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
        for (size_t j = 0; j < isConnected.size(); ++k) {
            if (isConnected[i][j] == 1 && !visited[k]) {
                dfs(isConnected, k, visited);
            }
        }
    }
};
}

int main() {
    return 0;
}
