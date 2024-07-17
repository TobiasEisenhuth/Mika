#include <string>
#include <set>
#include <nlohmann/json.hpp>

typedef std::set<size_t> Relations;

struct Node {
    size_t id;
    std::string information;
    Relations parents;
    Relations children;

    // Define how to convert Node to/from JSON
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Node, id, information, parents, children)
};