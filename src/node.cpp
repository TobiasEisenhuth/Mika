#include <string>
#include <vector>
#include <nlohmann/json.hpp>

typedef std::vector<size_t> Relations;
typedef std::vector<size_t> Bunch;

struct Node {
    size_t id;
    std::string information;
    Relations parents;
    Relations children;

    // Define how to convert Node to/from JSON
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Node, id, information, parents, children)
};