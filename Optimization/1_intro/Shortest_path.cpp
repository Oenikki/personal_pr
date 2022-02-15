#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include <vector>
#include <climits>

int min_distance = INT_MAX;

namespace dfs_method {
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
            dfs(j, dist + distance[cur][j], e, visited, distance);
            visited[j] = false;
        }
    }
    return;
}
}

namespace floyd_method {
void floyd(std::vector<std::vector<int>>& distance) {
    const int virtex = distance.size();
    for (int k = 0; k < virtex; ++k) {
        for (int i = 0; i < virtex; ++i) {
            for (int j = 0; j < virtex; ++j) {
                if (distance[i][k] + distance[k][j] < distance[i][j]) {
                    distance[i][j] = distance[i][k] + distance[k][j];
                }
            }
        }
    }
}
}

namespace dijkstra_method {
void dijkstra(const std::vector<std::vector<int>>& distance,
              std::vector<int>& dist,
              std::vector<bool>& visited) {
    int min_distance;
    int s;
    for (int i = 0; i < distance.size(); ++i) {
        min_distance = INT_MAX;
        for (int j = 0; j < distance.size(); ++j) {
            if (!visited[j] && dist[j] < min_distance) {
                min_distance = dist[j];
                s = j;
            }
        }
        visited[s] = true;
        for (int k = 0; k < distance.size(); ++k) {
            if (distance[s][k] != -1) {
                if (dist[k] > dist[s] + distance[s][k]) {
                    dist[k] = dist[s] + distance[s][k];
                }
            }
        }
    }
}
}

int main() {
    std::string file_name("input.txt");
    std::ifstream ifs(file_name, std::ios::binary);
    std::stringstream ss;
    std::string line;
    int end = 0;
    std::cin >> end;
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

    int flag;
    std::cin >> flag;
    switch (flag) {
        case 1: {
            std::vector<bool> visited(distance.size(), false);
            dfs_method::dfs(0, 0, end, visited, distance);
        }
        break;
        case 2: {
            floyd_method::floyd(distance);
        }
        break;
        case 3: {
            std::vector<int> dist;
            dist.resize(distance.size());
            for (int i = 0; i < distance.size(); ++i) {
                dist[i] = distance[0][i];
            }
            std::vector<bool> visited(distance.size(), false);
            visited[0] = true;
            dijkstra_method::dijkstra(distance, dist, visited);
        }
    }
    std::cout << "min distance: " << min_distance << std::endl;
    return 0;
}
