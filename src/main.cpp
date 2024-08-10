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
    LOAD_FILE = 'l',
    ADD_TASK = 'a',
    SPLIT_TASK = 's',
    FORK_TASK = 'f',
    REVISE_TASK = 'r',
    ERASE_TASK = 'e',
    MOVE_TASK = 'm',
    CHECK = 'c',
    GRAPH = 'g',
    QUIT = 'q',
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

std::string strike_through(const std::string &original_str) {
    std::string strike_through_str = "\u0336";
    for (char c : original_str) {
        strike_through_str += c;
        strike_through_str += "\u0336";
    }
    return strike_through_str;
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

bool query_unique_id(const std::string &prompt, std::set<size_t> &nodes_id) {
    std::vector<std::string> query;
    query_user(prompt, query, ALL_WORDS);
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

bool interogate() {
    std::vector<std::string> query;
    std::string prompt = "\tDo you want to save the changes? (yes/no)\n> ";
    query_user(prompt, query, FIRST_LETTER);
    return query.front() == "y";
}

void show_menu() {
    std::cout << '\t' << "Choose either:\n";
    std::cout << "\t    [" << (char)HELP << "]elp menu\n";
    std::cout << "\t    [" << (char)LOAD_FILE << "]oad file\n";
    std::cout << "\t    [" << (char)ADD_TASK << "]dd task\n";
    std::cout << "\t    [" << (char)SPLIT_TASK << "]plit task\n";
    std::cout << "\t    [" << (char)FORK_TASK << "]ork task\n";
    std::cout << "\t    [" << (char)REVISE_TASK << "]evise task\n";
    std::cout << "\t    [" << (char)ERASE_TASK << "]rase task\n";
    std::cout << "\t    [" << (char)MOVE_TASK << "]ove task\n";
    std::cout << "\t    [" << (char)CHECK << "]heck task as done\n";
    std::cout << "\t    [" << (char)GRAPH << "]raph\n";
    std::cout << "\t    [" << (char)QUIT << "]uit prgramm\n";
    std::cout << std::flush;
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

bool load_file_from_user_input(Graph &graph) {
    if (!state.saved && interogate()) {
        std::cout << "\tSaved!\n";
    }
    std::vector<std::string> query;
    query_user("Enter file location: > ", query, FIRST_WORD);
    return load_file(query.front(), graph);
}

bool add_task(Graph &graph, std::set<std::string> parent_id = {}) {
    std::vector<std::string> query;
    if (!graph.next_id) {
        query_user("Enter root node information: > ", query, SENTENCE);
        graph.add_root(query.front());
    } else {
        std::set<size_t> parents;
        if (parent_id.empty()) {
            query_user("Enter parent node(s): > ", query, ALL_WORDS);
        } else {
            query.insert(query.begin(), parent_id.begin(), parent_id.end());
        }
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

bool split_task(Graph &graph) {
    std::set<size_t> nodes_id;
    bool success = query_unique_id("Enter task ID to be split: > ", nodes_id);
    size_t skipped_id_count = 0;
    std::vector<std::string> query;
    for (const auto &id : nodes_id) {
        if (graph.nodes.find(id) != graph.nodes.end()) {
            std::cout << "\tTask in splitting:\n";
            std::cout << "\t└─ ";
            graph.print_node(id);
            std::cout << "Part 1 - ";
            query.clear();
            query_user("Enter task information: > ", query, SENTENCE);
            graph.nodes.at(id).information = query.front();
            auto original_childern = graph.nodes.at(id).children;
            graph.nodes.at(id).children.clear();
            std::cout << "Part 2 - ";
            add_task(graph, {std::to_string(id)});
            graph.nodes.at(graph.most_recently_added_id).children = original_childern;
        } else {
            std::cout << "\tSkipping ID = " << id << " as it does not exist!" << std::endl;
            ++skipped_id_count;
        }
    }
    return nodes_id.size() == skipped_id_count ? false : success;
}

bool fork_task(Graph &graph) {
    return false;
}

bool revise_task(Graph &graph) {
    std::set<size_t> nodes_id;
    bool success = query_unique_id("Enter task ID to be revised: > ", nodes_id);
    size_t skipped_id_count = 0;
    std::vector<std::string> query;
    for (const auto &id : nodes_id) {
        if (graph.nodes.find(id) != graph.nodes.end()) {
            std::cout << "\tTask in revision:\n";
            std::cout << "\t└─ ";
            graph.print_node(id);
            query.clear();
            query_user("Enter revised task information: > ", query, SENTENCE);
            graph.nodes.at(id).information = query.front();
        } else {
            std::cout << "\tSkipping ID = " << id << " as it does not exist!" << std::endl;
            ++skipped_id_count;
        }
    }
    return nodes_id.size() == skipped_id_count ? false : success;
}

bool erase_task(Graph &graph) {
    std::set<size_t> nodes_id;
    bool success = query_unique_id("Which task(s) should be erased? > ", nodes_id);
    size_t skipped_id_count = 0;
    for (const auto &id : nodes_id) {
        if (graph.nodes.find(id) != graph.nodes.end()) {
            if (graph.nodes.at(id).children.empty()) {
                for (const auto &parent_id : graph.nodes.at(id).parents) {
                    graph.nodes.at(parent_id).children.erase(id);
                }
                graph.nodes.erase(id);
            } else {
                std::cout << "\tCurrently only deletion of leaf tasks are supported! (id=" << id << ")" << std::endl;
                ++skipped_id_count;
            }
        } else {
            std::cout << "\tSkipping ID = " << id << " as it does not exist!" << std::endl;
            ++skipped_id_count;
        }
    }
    return nodes_id.size() == skipped_id_count ? false : success;
}

bool move_task(Graph &graph) {
    return false;
}

bool mark_as_done(Graph &graph) {
    std::set<size_t> nodes_id;
    bool success = query_unique_id("Which task(s) is(are) done? > ", nodes_id);
    size_t skipped_id_count = 0;
    for (const auto &id : nodes_id) {
        if (graph.nodes.find(id) != graph.nodes.end()) {
            std::string original_info = graph.nodes.at(id).information;
            graph.nodes.at(id).information = strike_through(original_info);
        } else {
            std::cout << "\tSkipping ID = " << id << " as it does not exist!" << std::endl;
            ++skipped_id_count;
        }
    }
    return nodes_id.size() == skipped_id_count ? false : success;
}

void execute(user_options &instruction, Graph &graph) {
    bool success = false;
    switch (instruction)
    {
        case HELP:
            show_menu();
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

        case SPLIT_TASK:
            success = split_task(graph);
            state.saved = !success && state.saved;
            break;

        case FORK_TASK:
            std::cout << "\tFork task!\n";
            //success = add_task();
            //state.saved = !success && state.saved;
            break;

        case REVISE_TASK:
            success = revise_task(graph);
            state.saved = !success && state.saved;
            break;

        case ERASE_TASK:
            success = erase_task(graph);
            state.saved = !success && state.saved;
            break;

        case MOVE_TASK:
            std::cout << "\tMoved task!\n";
            //success = add_task();
            //state.saved = !success && state.saved;
            break;

        case CHECK:
            success = mark_as_done(graph);
            state.saved = !success && state.saved;
            break;

        case GRAPH:
            if (state.loaded || !state.saved)
                graph.print_structure();
            else
                std::cout << "\tNo graph to show!\n";
            break;

        case QUIT:
            if (!state.saved && interogate()) {
                save_to_file(graph);
                std::cout << "\tSaved!\n";
            }
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