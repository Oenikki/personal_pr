#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>

int main() {
    std::string file_name("input.txt");
    std::ifstream ifs(file_name, std::ios::binary);
    std::stringstream ss;
    std::string line;
    int vertex = 0, edge = 0;
    int i = 0, j = 0, weight = 0;
    bool first_row = true;
    std::vector<vector<int>> distance;
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
            distance.resize(vertex);
            for (auto& v : distance) {
                v.resize(edge);
            }
        } else {
            ss >> i >> j >> weight;
            std::cout << "i: " << i << " j: " << j
                      << " weight: " << weight << std::endl;
            distance[i - 1][j - 1] =
        }
    }
    return 0;
}
