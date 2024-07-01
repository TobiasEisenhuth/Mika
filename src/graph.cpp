#include <unordered_map>
#include <fstream>
#include "node.cpp"

struct Graph {
    std::unordered_map<int, Node> nodes;

    void to_json(const std::string &filename) const {
        nlohmann::json j = nodes;
        std::ofstream file(filename);
        file << j.dump(4);
    }

    void from_json(const std::string &filename) {
        std::ifstream file(filename);
        nlohmann::json j;
        file >> j;
        nodes = j.get<std::unordered_map<int, Node>>();
    }
};
