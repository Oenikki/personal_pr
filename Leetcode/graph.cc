#include <vector>
#include <queue>

using namespace std;

namespace hh785 {
class Solution {
public:
    bool isBipartite(vector<vector<int>>& graph) {
        int n = graph.size();
        vector<int> colors(n, 0); //0: no color, 1: red, 2: black
        queue<int> q;
        for (int i = 0; i < n; ++i) {
            if (!colors[i]) {
                q.push(i);
                colors[i] = 1;
            }
            while (!q.empty()) {
                auto v = q.front();
                q.pop();
                for (const auto& adj : graph[v]) {
                    if (!colors[adj]) {
                        q.push(adj);
                        colors[adj] = colors[v] == 1 ? 2 : 1;
                    } else if (colors[adj] == colors[v]) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
};
}

namespace hh210 {
class Solution {
public:
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        vector<vector<int>> graph(numCourses, vector<int>());
        vector<int> indegree(numCourses, 0);
        vector<int> res;
        for (const auto& prerequisite : prerequisites) {
            graph[prerequisite[1]].push_back(prerequisite[0]);
            ++indegree[prerequisite[0]];
        }
        queue<int> q;
        for (int i = 0; i < indegree.size(); ++i) {
            if (!indegree[i]) {
                q.push(i);
            }
        }
        while (!q.empty()) {
            int u = q.front();
            res.push_back(u);
            q.pop();
            for (auto v : graph[u]) {
                --indegree[v];
                if (!indegree[v]) {
                    q.push(v);
                }
            }
        }
        for (int i = 0; i < indegree.size(); ++i) {
            if (indegree[i]) {
                return {};
            }
        }
        return res;
    }
};
}
