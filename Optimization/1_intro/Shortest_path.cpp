#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include <vector>
#include <climits>

int min_distance = INT_MAX;

void dfs(int cur, int dist, int e,
         std::vector<bool>& visited,
         std::vector<std::vector<int>>& distance) {
    if (dist > min_distance) {
        return;
    }
    if (cur == e) {
        min_distance = std::min(min_distance, dist);
        return;
    }
    for (int j = 0; j < distance.size(); ++j) {
        if (distance[cur][j] != -1 && !visited[j]) {
            visited[j] = true;
            dfs(j, distance[cur][j], e, visited, distance);
            visited[j] = false;
        }
    }
    return;
}

int main() {
    std::string file_name("input.txt");
    std::ifstream ifs(file_name, std::ios::binary);
    std::stringstream ss;
    std::string line;
    int e = 0;
    std::cin >> e;
    int vertex = 0, edge = 0;
    int i = 0, j = 0, weight = 0;
    bool first_row = true;
    std::vector<std::vector<int>> distance;
    while (ifs.good() && !ifs.eof()) {
        std::getline(ifs, line, '\n');
        ss.clear();
        ss.str(line);
        if (first_row) {
            first_row = false;
            ss >> vertex >> edge;
            std::cout << "vertex: " << vertex
                      << " edge: " << edge << std::endl;
            assert(vertex > 0 && edge > 0);
            distance = std::vector<std::vector<int>>(vertex, std::vector<int>(vertex, -1));
            for (size_t i = 0; i < vertex; ++i) {
                distance[i][i] = 0;
            }
        } else {
            ss >> i >> j >> weight;
            std::cout << "i: " << i << " j: " << j
                      << " weight: " << weight << std::endl;
            distance[i - 1][j - 1] = weight;
        }
    }


    for (const auto& v :distance) {
        for (const int& e : v) {
            std::cout << e << " ";
        }
        std::cout << std::endl;
    }

    std::vector<bool> visited(distance.size(), false);
    dfs(0, 0, e, visited, distance);
    std::cout << "min distance: " << min_distance << std::endl;
    return 0;
}
