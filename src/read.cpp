#include <iostream>
#include "graph.cpp"

int main(){

    Graph graph;
    graph.from_json("graph.json");

    for (const auto& elm : graph.nodes)
    {
        std::cout << "Node id: " << elm.first << "\n";
        std::cout << "Node info: " << elm.second.information << std::endl;
    }
}