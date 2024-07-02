#include <iostream>
#include "graph.cpp"

struct program_state {
    bool loaded = false;
    bool saved = true;
} state;

enum user_options {
    NONE,
    HELP = 'h',
    QUIT = 'q',
    LOAD_FILE = 'l',
    ADD_TASK = 'a',
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
};

void showMenu() {
    std::cout << '\t' << "Choose either:\n";
    std::cout << "\t    " << (char)HELP << ", Print this help menu.\n";
    std::cout << "\t    " << (char)QUIT << ", Quit prgramm.\n";
    std::cout << "\t    " << (char)LOAD_FILE << ", Load file.\n";
    std::cout << "\t    " << (char)ADD_TASK << ", Add a task.\n";
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
    bool success = graph.from_json(filename);
    if (success) {
        std::cout << "\tGraph successfully loaded!\n";
    } else {
        std::cout << "\tSomething did not work out loading the graph from file, sorry!\n";
    }
    std::cout << std::flush;
    return success;
}

bool load_file_from_user_input(Graph &graph) {
    std::vector<std::string> query;
    queryUser("Enter file location: > ", query, FIRST_WORD);
    return load_file(query.front(), graph);
}

bool interogate() {
    std::vector<std::string> query;
    std::string prompt = "\tDo you want to save the changes? (yes/no)\n> ";
    queryUser(prompt, query, FIRST_LETTER);
    return query.front() == "y";
}

void execute(user_options &instruction, Graph &graph) {
    bool success = false;
    switch (instruction)
    {
        case HELP:
            showMenu();
            break;

        case QUIT:
            if (!state.saved && interogate())
                std::cout << "\tSaved!\n";
            break;

        case NONE:
            break;

        case LOAD_FILE:
            success = load_file_from_user_input(graph);
            state.loaded = success;
            break;

        case ADD_TASK:
            std::cout << "\tAdded task!\n";
            success = [](){return true;};
            state.saved = !success;
            break;

        case SPLIT_TASK:
            graph.test();
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
            std::cout << "\tLook at this graph!\n";
            graph.print_structure();
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