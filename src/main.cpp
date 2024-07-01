#include <iostream>
#include "graph.cpp"

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

void showMenu() {
    std::cout << '\t' << "Choose either:\n";
    std::cout << "\t    " << (char)HELP << ", Print this help menu.\n";
    std::cout << "\t    " << (char)QUIT << ", Quit prgramm.\n";
    std::cout << "\t    " << (char)LOAD_FILE << "Load file.";
    std::cout << "\t    " << (char)ADD_TASK << ", Add a task.\n";
    std::cout << "\t    " << (char)SPLIT_TASK << ", Split a task.\n";
    std::cout << "\t    " << (char)ERASE_TASK << ", Erase a task.\n";
    std::cout << "\t    " << (char)MOVE_TO << ", Move to a task.\n";
    std::cout << "\t    " << (char)GRAPH << ", Show graph.\n";
    std::cout << std::flush;
}

user_options queryUser() {
    std::cout << "> ";
    std::string user_input;
    std::getline(std::cin, user_input);
    return !user_input.empty() ? (user_options) user_input.front() : NONE;
}

void execute(user_options& instruction) {
    switch (instruction)
    {
        case HELP:
            showMenu();
            break;

        case QUIT:
        case NONE:
            break;

        case LOAD_FILE:
            std::cout << "\tFile loaded!" << std::endl;
            break;

        case ADD_TASK:
            std::cout << "\tAdded task!" << std::endl;
            break;

        case SPLIT_TASK:
            std::cout << "\tSplit task!" << std::endl;
            break;

        case ERASE_TASK:
            std::cout << "\tErased task!" << std::endl;
            break;

        case MOVE_TO:
            std::cout << "\tMoved to task!" << std::endl;
            break;

        case GRAPH:
            std::cout << "\tLook at this graph!" << std::endl;
            break;

        default:
            std::cout << "\tInvalid option!" << std::endl;
            break;
    }
}

int main(int argc, char **argv) {

    fresh_start:
    auto instruction = NONE;
    showMenu();

    main_loop:
    while (instruction != QUIT) {
        instruction = queryUser();
        execute(instruction);
    }

    end_program:
    std::cout << "\tHave a great Day!" << std::endl;
    return 0;
}