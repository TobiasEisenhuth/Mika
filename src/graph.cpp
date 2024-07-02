#include <iostream>
#include <unordered_map>
#include <stack>
#include <fstream>
#include "node.cpp"

struct Graph {
    std::unordered_map<size_t, Node> nodes;
    size_t next_id = -1; // ID counter

    void to_json(const std::string &filename) const {
        nlohmann::json j = nodes;
        std::ofstream file(filename);
        file << j.dump(4); // Pretty print with an indent of 4 spaces
    }

    void update_next_id() {
        for (const auto &pair : nodes) {
            if (pair.first >= next_id) {
                next_id = pair.first + 1;
            }
        }
    }

    bool from_json(const std::string &filename) {
        std::ifstream file(filename);
        bool is_good = file.good();
        if (is_good) {
            nlohmann::json j;
            try {
                file >> j;
            } catch(nlohmann::json_abi_v3_11_2::detail::parse_error err) {
                std::cerr << err.what() << '\n';
                return false;
            } catch(nlohmann::json_abi_v3_11_2::detail::type_error err) {
                std::cerr << err.what() << '\n';
                return false;
            }
            nodes = j.get<std::unordered_map<size_t, Node>>();
            update_next_id();
        }
        return is_good;
    }

    std::vector<size_t> find_leaves_from(size_t start_node_id) const {
        std::vector<size_t> leaves;
        if (nodes.find(start_node_id) == nodes.end()) {
            return leaves;
        }

        std::stack<size_t> stack;
        stack.push(start_node_id);

        while (!stack.empty()) {
            size_t node_id = stack.top();
            stack.pop();
            const Node &node = nodes.at(node_id);

            if (node.children.empty()) {
                leaves.push_back(node_id);
            } else {
                for (size_t child_id : node.children) {
                    stack.push(child_id);
                }
            }
        }

        return leaves;
    }

    size_t add_root(const std::string &information) {
        size_t id = next_id++;
        nodes[id] = {id, information, {}, {}};
        return id;
    }

    size_t add_node(const std::string &information, std::vector<size_t>& parents, std::vector<size_t>& children) {
        size_t id = next_id++;
        nodes[id] = {id, information, {}, {}};
        return id;
    }

    void remove_node(size_t id) {
        if (nodes.find(id) == nodes.end()) {
            throw std::runtime_error("Node not found.");
        }
        // Remove references from parents and children
        for (size_t parent_id : nodes[id].parents) {
            auto &siblings = nodes[parent_id].children;
            siblings.erase(std::remove(siblings.begin(), siblings.end(), id), siblings.end());
        }
        for (size_t child_id : nodes[id].children) {
            auto &parents = nodes[child_id].parents;
            parents.erase(std::remove(parents.begin(), parents.end(), id), parents.end());
        }
        nodes.erase(id);
    }

    void switch_parent(size_t node_id, size_t old_parent_id, size_t new_parent_id) {
        if (nodes.find(node_id) == nodes.end() || nodes.find(old_parent_id) == nodes.end() || nodes.find(new_parent_id) == nodes.end()) {
            throw std::runtime_error("Node or parent not found.");
        }
        // Remove old parent
        auto &old_siblings = nodes[old_parent_id].children;
        old_siblings.erase(std::remove(old_siblings.begin(), old_siblings.end(), node_id), old_siblings.end());
        nodes[node_id].parents.erase(std::remove(nodes[node_id].parents.begin(), nodes[node_id].parents.end(), old_parent_id), nodes[node_id].parents.end());
        // Add new parent
        nodes[node_id].parents.push_back(new_parent_id);
        nodes[new_parent_id].children.push_back(node_id);
    }

    void add_subtree(const Graph &subtree, size_t attach_to) {
        if (nodes.find(attach_to) == nodes.end()) {
            throw std::runtime_error("Node to attach to not found.");
        }
        for (const auto &pair : subtree.nodes) {
            size_t node_id = next_id++;
            Node new_node = pair.second;
            new_node.id = node_id;
            for (size_t &parent_id : new_node.parents) {
                parent_id = next_id++;
            }
            for (size_t &child_id : new_node.children) {
                child_id = next_id++;
            }
            nodes[node_id] = new_node;
        }
        // Connect the root of the subtree to the node to attach to
        for (const auto &pair : subtree.nodes) {
            if (pair.second.parents.empty()) {
                nodes[attach_to].children.push_back(pair.second.id);
                nodes[pair.second.id].parents.push_back(attach_to);
                break;
            }
        }
    }

    void print_tree(size_t node_id, const std::string &prefix = "", bool is_last = true) const {
        if (nodes.find(node_id) == nodes.end()) {
            throw std::runtime_error("Node not found.");
        }

        std::cout << prefix;

        std::cout << (is_last ? "└─ " : "├─ ");

        std::cout << nodes.at(node_id).id << std::endl;

        const auto &children = nodes.at(node_id).children;
        for (size_t i = 0; i < children.size(); ++i) {
            print_tree(children[i], prefix + (is_last ? "   " : "│  "), i == children.size() - 1);
        }
    }   

    void print_structure() const {
        // Assuming the root node is the one with no parents
        size_t root_id = -1;
        for (const auto &pair : nodes) {
            if (pair.second.parents.empty()) {
                root_id = pair.first;
                break;
            }
        }
        if (root_id == static_cast<size_t>(-1)) {
            throw std::runtime_error("Root node not found.");
        }

        print_tree(root_id);
    }
};
