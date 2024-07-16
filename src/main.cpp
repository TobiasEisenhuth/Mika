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

void showMenu() {
    std::cout << '\t' << "Choose either:\n";
    std::cout << "\t    " << (char)HELP << ", Print this help menu.\n";
    std::cout << "\t    " << (char)QUIT << ", Quit prgramm.\n";
    std::cout << "\t    " << (char)LOAD_FILE << ", Load file.\n";
    std::cout << "\t    " << (char)ADD_TASK << ", Add a task.\n";
    std::cout << "\t    " << (char)REVISE_TASK << ", Revise a task.\n";
    std::cout << "\t    " << (char)SPLIT_TASK << ", Split a task.\n";
    std::cout << "\t    " << (char)ERASE_TASK << ", Erase a task.\n";
    std::cout << "\t    " << (char)MOVE_TO << ", Move to a task.\n";
    std::cout << "\t    " << (char)GRAPH << ", Show graph.\n";
    std::cout << std::flush;
}

void queryUser(const std::string &prompt, std::vector<std::string> &result, queryModes mode) {
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

user_options queryUserOption() {
    std::vector<std::string> query;
    queryUser("> ", query, FIRST_LETTER);
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
    queryUser(prompt, query, FIRST_LETTER);
    return query.front() == "y";
}

bool load_file_from_user_input(Graph &graph) {
    if (!state.saved && interogate()) {
        std::cout << "\tSaved!\n";
    }
    std::vector<std::string> query;
    queryUser("Enter file location: > ", query, FIRST_WORD);
    return load_file(query.front(), graph);
}

bool add_task(Graph &graph) {
    std::vector<std::string> query;
    if (!graph.next_id) {
        queryUser("Enter root node information: > ", query, SENTENCE);
        graph.add_root(query.front());
    } else {
        std::vector<size_t> parents;
        queryUser("Enter parent node(s): > ", query, ALL_WORDS);
        size_t count = query.size();
        for (const auto &word : query) {
            size_t parent_id;
            std::stringstream stream(word);
            stream >> parent_id;
            if (graph.nodes.find(parent_id) != graph.nodes.end()) {
                parents.emplace_back(parent_id);
                graph.nodes.at(parent_id).children.emplace_back(graph.next_id);
            }
            else {
                std::cout << "\tSkipping id = " << parent_id << " as it does not exist!\n";
                --count;
            }
        }
        if (count) {
            query.clear();
            queryUser("Enter node information: > ", query, SENTENCE);
            graph.add_node(query.front(), parents);
        } else {
            std::cout << "\tDetached nodes are not supported yet!\n";
            std::cout << "\tMake sure a node has at least one valid parent!\n";
        }

    }
    std::cout << std::flush;
    return true;
}

bool save_to_file(Graph &graph, bool skip_cached_name = false) {
    std::vector<std::string> query;
    if (!state.file_name_cache.empty() && !skip_cached_name) {
        std::string prompt = "\tSave the changes to " + state.file_name_cache + "? (yes/no)\n> ";
        queryUser(prompt, query, FIRST_LETTER);
        if (query.front() == "y") {
            graph.to_json(state.file_name_cache);
            return true;
        }
    }
    query.clear();
    queryUser("Enter file location: > ", query, FIRST_WORD);
    if (std::ifstream(query.front()).good()) {
        std::string new_file_name = query.front();
        query.clear();
        std::string prompt = "\tThe file " + new_file_name + " already exists. Proceed anyway? (yes/no)\n> ";
        queryUser(prompt, query, FIRST_LETTER);
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

void execute(user_options &instruction, Graph &graph) {
    bool success = false;
    switch (instruction)
    {
        case HELP:
            showMenu();
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
            state.saved = !success;
            break;

        case REVISE_TASK:
            std::cout << "\tTask revised!\n";
            success = [](){return true;};
            state.saved = !success;
            break;

        case SPLIT_TASK:
            std::cout << "\tSplit task!\n";
            //bool success = add_task();
            //state.loaded = success;
            break;

        case ERASE_TASK:
            std::cout << "\tErased task!\n";
            //bool success = add_task();
            //state.loaded = success;
            break;

        case MOVE_TO:
            std::cout << "\tMoved to task!\n";
            //bool success = add_task();
            //state.loaded = success;
            break;

        case GRAPH:
            if (state.loaded || !state.saved)
                graph.print_structure();
            else
                std::cout << "\tNo graph to show!\n";
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
    showMenu();

    main_loop:
    while (instruction != QUIT) {
        instruction = queryUserOption();
        execute(instruction, graph);
    }

    end_program:
    std::cout << "\tHave a great Day!" << std::endl;
    return 0;
}