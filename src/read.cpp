#include <iostream>
#include "graph.cpp"

int main(){

    Graph graph;
    graph.from_json("graph.json");

    for (const auto& elm : graph.nodes) {
        std::cout << "Node id: " << elm.first << "\n";
        std::cout << "Node info: " << elm.second.information << std::endl;
    }

    std::vector<size_t> all_leaves = graph.find_all_leaves();

    for (const auto& elm : all_leaves) {
        std::cout << graph.nodes.at(elm).information << std::endl;
    }

    std::vector<size_t> leaves_starting_at_x = graph.find_leaves_starting_from(1);

    for (const auto& elm : leaves_starting_at_x) {
        std::cout << graph.nodes.at(elm).information << std::endl;
    }
}