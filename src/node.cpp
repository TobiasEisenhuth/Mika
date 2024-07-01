#include <string>
#include <vector>
#include <nlohmann/json.hpp>

struct Node {
    int id;
    std::string information;
    std::vector<int> parents;
    std::vector<int> children;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Node, id, information, parents, children)
};
