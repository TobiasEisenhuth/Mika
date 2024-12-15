#include <string>
#include <set>
#include <nlohmann/json.hpp>
#include <ctime>

typedef std::set<size_t> Relations;

enum node_urgency {
    URGENT,
    RELAXED,
};

enum node_importance {
    IPORTANT,
    NEGLIGIBLE,
};

struct node_properties {
    node_urgency urgency;
    node_importance importantce;
    double priority_score = 0.;
    time_t deadline = time(NULL);
};


struct Node {
    size_t id;
    std::string information;
    Relations parents;
    Relations children;
    node_properties properties;

    // Define how to convert Node to/from JSON
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Node, id, information, parents, children)
};