#include <iostream>
#include "graph.cpp"

int main(){

    Node parent{0, "I am the WORK", {}, {1,2}};
    Node child1{1, "I am one Child of 0", {0}, {}};
    Node child2{2, "I am one Child of 0", {0}, {}};
    Graph graph;
    graph.nodes.emplace(parent.id, parent);
    graph.nodes.emplace(child1.id, child1);
    graph.nodes.emplace(child2.id, child2);

    graph.to_json("graph.json");
}