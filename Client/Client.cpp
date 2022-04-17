//
// Created by Maks Konevych on 16.04.2022.
//

#include "Client.h"
#include <iostream>

Client::Client() {
    server.loadData();
}

void Client::run() {
    prepareData();
    running = true;
    printHelloMessage();
    renderPostPage();
    while (running) {
        readCommand();
        processCommand();
    }
}

void Client::prepareData() {
    Message message;
    if (server.getMessageById(0, message)) {
        messagesStack.emplace_back(std::move(message));
    }
}

void Client::renderPostPage() const {
    if (messagesStack.empty()) {
        std::cout << "No messages to show...\n";
        std::cout << "No messages to show...\n";
    } else {
        for (int i = 0; i < messagesStack.size(); ++i) {
            printCharLine('-', 3 + i * 3);
            std::cout << messagesStack[i].getText() << '\n';
            printCharLine(' ', 3 + i * 3);
            std::cout   << messagesStack[i].getLikes() << " likes, "
                        << messagesStack[i].getResponsesNum() << " responses. "
                        << "ID: " << messagesStack[i].getId().toString()
                        << '\n';
        }
    }
}

void Client::renderCommentPage() const {

}

void Client::renderNewPostPage() const{

}

bool Client::readCommand() {
    std::cout << "> ";
    std::string command;
    if (!(std::cin >> command))
        return false;
    for (char ch; std::cin.get(ch) && ch != '\n';);
    if (command == "help") {
        currentCommand = Command::HELP_;
    } else if (command == "back") {
        currentCommand = Command::BACK_;
    } else if (command == "quit") {
        currentCommand = Command::QUIT_;
    } else if (command == "comment") {
        currentCommand = Command::COMMENT_;
    } else if (command == "post") {
        currentCommand = Command::POST_;
    } else if (command == "next") {
        currentCommand = Command::NEXT_;
    } else if (command == "previous") {
        currentCommand = Command::PREVIOUS_;
    } else if (command == "like") {
        currentCommand = Command::LIKE_;
    } else if (command == "dislike") {
        currentCommand = Command::DISLIKE_;
    } else {
        currentCommand = Command::INVALID_;
    }
    return true;
}

void Client::processCommand() {
    switch (currentCommand) {
        case Command::INVALID_: {
            printInvalidCommandMessage();
            break;
        }
        case Command::HELP_: {
            printHelpMessage();
        }
        case Command::BACK_: {
            doBack();
            break;
        }
        case Command::QUIT_: {
            printGoodByeMessage();
            running = false;
            break;
        }
        case Command::COMMENT_: {

            break;
        }
        case Command::POST_: {

            break;
        }
        case Command::NEXT_: {

        }
        case Command::PREVIOUS_: {

        }
        case Command::LIKE_: {

        }
        case Command::DISLIKE_: {

        }
        default: {
            std::cout << "Processing of this command is not implemented yet!\n";
        }
    }
}

void Client::doBack() {
    if (currentPage == Page::POST_) {

    } else if (currentPage == Page::COMMENT_) {

    } else if (currentPage == Page::NEW_POST_) {

    } else {
        std::cout << "Command cannot be processed! :( Sorry... \n";
    }
}

void Client::printHelloMessage() const {
    std::cout << "Hello! Type 'help' if you need more info.\n";
}

void Client::printHelpMessage() const {
    std::cout
        << "Command available:\n"
        << "\thelp — \n"
        << "\tback — \n"
        << "\tquit — \n"
        << "\tcomment — \n"
        << "\tpost — \n"
        << "\tnext — \n"
        << "\tprevious — \n"
        << "\tlike — \n"
        << "\tdislike — \n";
}

void Client::printInvalidCommandMessage() const {
    std::cout << "No such command. Type 'help' to get info on commands.\n";
}

void Client::printGoodByeMessage() const {
    std::cout << "Hope to see you soon...\n";
}

void Client::printCharLine(char ch, int n) const {
    while (0 < n--)
        std::cout.put(ch);
}



