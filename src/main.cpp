#include <iostream>
#include "graph.cpp"

struct program_state {
    bool loaded = false;
    std::string file_name_cache;
    bool saved = true;
} state;

enum user_options {
    NONE,
    HELP = 'h',
    QUIT = 'q',
    LOAD_FILE = 'l',
    ADD_TASK = 'a',
    REVISE_TASK = 'r',
    SPLIT_TASK = 's',
    ERASE_TASK = 'e',
    MOVE_TO = 'm',
    GRAPH = 'g',
    CHECK = 'c',
};

user_options str2option(std::string &str) {
    return (user_options) str.front();
}

enum queryModes {
    FIRST_LETTER,
    FIRST_WORD,
    ALL_WORDS,
    SENTENCE,
};

void show_menu() {
    std::cout << '\t' << "Choose either:\n";
    std::cout << "\t    " << (char)HELP << ", [P]RINT this help menu.\n";
    std::cout << "\t    " << (char)QUIT << ", [Q]UIT prgramm.\n";
    std::cout << "\t    " << (char)LOAD_FILE << ", [L]OAD file.\n";
    std::cout << "\t    " << (char)ADD_TASK << ", [A]DD a task.\n";
    std::cout << "\t    " << (char)REVISE_TASK << ", [R]EVISE a task.\n";
    std::cout << "\t    " << (char)SPLIT_TASK << ", [S]PLIT a task.\n";
    std::cout << "\t    " << (char)ERASE_TASK << ", [E]RASE a task.\n";
    std::cout << "\t    " << (char)MOVE_TO << ", [M]OVE to a task.\n";
    std::cout << "\t    " << (char)GRAPH << ", [S]HOW graph.\n";
    std::cout << "\t    " << (char)CHECK << ", [C]HECK task as done.\n";
    std::cout << std::flush;
}

void query_user(const std::string &prompt, std::vector<std::string> &result, queryModes mode) {
    std::cout << prompt;
    std::string user_input;
    std::getline(std::cin, user_input);
    switch (mode)
    {
    case FIRST_LETTER:
        result.emplace_back(user_input.substr(0,1));
        break;

    case FIRST_WORD:
        result.emplace_back(user_input.substr(0, user_input.find(" ")));
        break;

    case ALL_WORDS:
        while(!user_input.empty()) {
            size_t pos = user_input.find(' ');
            if (pos == std::string::npos) {
                result.emplace_back(user_input);
                user_input.clear();
            } else {
                result.emplace_back(user_input.substr(0, pos));
                user_input.erase(0, pos+1);
            }
        }
        break;

    case SENTENCE:
        result.emplace_back(user_input);
        break;

    default:
        break;
    }

}

user_options query_user_option() {
    std::vector<std::string> query;
    query_user("> ", query, FIRST_LETTER);
    return !query.front().empty() ? str2option(query.front()) : NONE;
}

bool load_file(const std::string &filename, Graph &graph) {
    graph.reset();
    bool success = graph.from_json(filename);
    if (success) {
        std::cout << "\tGraph successfully loaded!\n";
        state.file_name_cache = filename;
    } else {
        std::cout << "\tSomething did not work out loading the graph from file, sorry!\n";
    }
    std::cout << std::flush;
    return success;
}

bool interogate() {
    std::vector<std::string> query;
    std::string prompt = "\tDo you want to save the changes? (yes/no)\n> ";
    query_user(prompt, query, FIRST_LETTER);
    return query.front() == "y";
}

bool load_file_from_user_input(Graph &graph) {
    if (!state.saved && interogate()) {
        std::cout << "\tSaved!\n";
    }
    std::vector<std::string> query;
    query_user("Enter file location: > ", query, FIRST_WORD);
    return load_file(query.front(), graph);
}

bool add_task(Graph &graph) {
    std::vector<std::string> query;
    if (!graph.next_id) {
        query_user("Enter root node information: > ", query, SENTENCE);
        graph.add_root(query.front());
    } else {
        std::set<size_t> parents;
        query_user("Enter parent node(s): > ", query, ALL_WORDS);
        size_t count = query.size();
        for (const auto &word : query) {
            size_t parent_id;
            std::stringstream stream(word);
            stream >> parent_id;
            if (stream.fail()) {
                std::cout << "\tNode ID has to be a number!" << std::endl;
                return false;
            }
            if (graph.nodes.find(parent_id) != graph.nodes.end()) {
                parents.emplace(parent_id);
                graph.nodes.at(parent_id).children.insert(graph.next_id);
            }
            else {
                std::cout << "\tSkipping ID = " << parent_id << " as it does not exist!\n";
                --count;
            }
        }
        if (count) {
            query.clear();
            query_user("Enter task information: > ", query, SENTENCE);
            graph.add_node(query.front(), parents);
        } else {
            std::cout << "\tMake sure a task has at least one valid parent!" << std::endl;
            return false;
        }

    }
    std::cout << std::flush;
    return true;
}

bool save_to_file(Graph &graph, bool skip_cached_name = false) {
    std::vector<std::string> query;
    if (!state.file_name_cache.empty() && !skip_cached_name) {
        std::string prompt = "\tSave the changes to " + state.file_name_cache + "? (yes/no)\n> ";
        query_user(prompt, query, FIRST_LETTER);
        if (query.front() == "y") {
            graph.to_json(state.file_name_cache);
            return true;
        }
    }
    query.clear();
    query_user("Enter file location: > ", query, FIRST_WORD);
    if (std::ifstream(query.front()).good()) {
        std::string new_file_name = query.front();
        query.clear();
        std::string prompt = "\tThe file " + new_file_name + " already exists. Proceed anyway? (yes/no)\n> ";
        query_user(prompt, query, FIRST_LETTER);
        if (query.front() == "y") {
            graph.to_json(new_file_name);
            state.file_name_cache = new_file_name;
            return true;
        } else {
            save_to_file(graph, true);
        }
    } else {
        graph.to_json(query.front());
        return true;
    }

    return false;
}

std::string strike_through(const std::string &original_str) {
    std::string strike_through_str = "\u0336";
    for (char c : original_str) {
        strike_through_str += c;
        strike_through_str += "\u0336";
    }
    return strike_through_str;
}

bool mark_as_done(Graph &graph) {
    std::vector<std::string> query;
    query_user("Which task(s) is(are) done? > ", query, ALL_WORDS);
    std::set<size_t> nodes_id;
    for (const auto &word : query)
    {
        size_t id;
        std::stringstream stream(word);
        stream >> id;
        if (!stream.fail()) {
            nodes_id.insert(id);
        } else {
            std::cout << "\tTask ID has to be a number! " << strike_through(word) << "\n";
        }
    }
    if (nodes_id.empty()) {
        std::cout << "\tEmpty input!" << std::endl;
        return false;
    }
    for (const auto &id : nodes_id) {
        if (graph.nodes.find(id) != graph.nodes.end()) {
            std::string original_info = graph.nodes.at(id).information;
            graph.nodes.at(id).information = strike_through(original_info);
        }
    }
    return true;
}

bool revise_task(Graph &graph) {
    std::vector<std::string> query;
    query_user("Enter task ID to be revised: > ", query, FIRST_WORD);
    size_t node_id;
    std::stringstream stream(query.front());
    stream >> node_id;
    if (stream.fail()) {
        std::cout << "\tTask ID has to be a number! " << strike_through(query.front()) << "\n";
        return false;
    }
    if (graph.nodes.find(node_id) != graph.nodes.end()) {
        std::cout << "\tTask in question:\n";
        std::cout << "\t└─ ";
        graph.print_node(node_id);
        query.clear();
        query_user("Enter revised task information: > ", query, SENTENCE);
        graph.nodes.at(node_id).information = query.front();
    } else {
        std::cout << "\tTask with ID = " << node_id << " does not exist!" << std::endl;
        return false;
    }

    return true;
}

bool erase_task(Graph &graph) {
    std::vector<std::string> query;
    query_user("Which task(s) should be erased? > ", query, ALL_WORDS);
    std::set<size_t> nodes_id;
    for (const auto &word : query)
    {
        size_t id;
        std::stringstream stream(word);
        stream >> id;
        if (!stream.fail()) {
            nodes_id.insert(id);
        } else {
            std::cout << "\tTask ID has to be a number! " << strike_through(word) << "\n";
        }
    }
    if (nodes_id.empty()) {
        std::cout << "\tEmpty input!" << std::endl;
        return false;
    }
    bool success = false;
    for (const auto &id : nodes_id) {
        if (graph.nodes.find(id) != graph.nodes.end()) {
            if (graph.nodes.at(id).children.empty()) {
                for (const auto &parent_id : graph.nodes.at(id).parents) {
                    graph.nodes.at(parent_id).children.erase(id);
                }
                graph.nodes.erase(id);
                success = true;
            } else {
                std::cout << "\tCurrently only deletion of leaf tasks is supported! (id=" << id << ")\n";
            }
        } else {
            std::cout << "\tTask with ID = " << id << " does not exist!\n";
        }
    }
    std::cout << std::flush;
    return success;
}

void execute(user_options &instruction, Graph &graph) {
    bool success = false;
    switch (instruction)
    {
        case HELP:
            show_menu();
            break;

        case QUIT:
            if (!state.saved && interogate()) {
                save_to_file(graph);
                std::cout << "\tSaved!\n";
            }
            break;

        case NONE:
            break;

        case LOAD_FILE:
            success = load_file_from_user_input(graph);
            state.loaded = success;
            break;

        case ADD_TASK:
            success = add_task(graph);
            state.saved = !success && state.saved;
            break;

        case REVISE_TASK:
            success = revise_task(graph);
            state.saved = !success && state.saved;
            break;

        case SPLIT_TASK:
            std::cout << "\tSplit task!\n";
            //success = add_task();
            //state.saved = !success && state.saved;
            break;

        case ERASE_TASK:
            success = erase_task(graph);
            state.saved = !success && state.saved;
            break;

        case MOVE_TO:
            std::cout << "\tMoved to task!\n";
            //success = add_task();
            //state.saved = !success && state.saved;
            break;

        case GRAPH:
            if (state.loaded || !state.saved)
                graph.print_structure();
            else
                std::cout << "\tNo graph to show!\n";
            break;

        case CHECK:
            success = mark_as_done(graph);
            state.saved = !success && state.saved;
            break;

        default:
            std::cout << "\tInvalid option!\n";
            break;
    }
    std::cout << std::flush;
}

int main(int argc, char *argv[]) {

    spadework:
    Graph graph;
    if (argc > 1) {
        bool success = load_file(argv[1], graph);
        state.loaded = success;
    }

    fresh_start:
    auto instruction = NONE;
    show_menu();

    main_loop:
    while (instruction != QUIT) {
        instruction = query_user_option();
        execute(instruction, graph);
    }

    end_program:
    std::cout << "\tHave a great Day!" << std::endl;
    return 0;
}