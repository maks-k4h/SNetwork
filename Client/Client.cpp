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
    } else if (command == "more") {
        currentCommand = Command::MORE_;
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

bool Client::readBool(bool &res, const std::string &m) const {
    std::cout << m << '\n';
    char c {};
    while (std::cout << "> " && std::cin.get(c)) {
        c = tolower(c);
        if (c !='y' && c != 'n') {
            std::cout << "Enter Y or n.\n";
            while (std::cin.get(c) && c != '\n');
        } else {
            break;
        }
    }
    if (c != 'y' && c != 'n')
        return false;
    res = c == 'y';
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
            break;
        }
        case Command::BACK_: {
            doBack();
            break;
        }
        case Command::MORE_: {
            doMore();
            break;
        }
        case Command::QUIT_: {
            printGoodByeMessage();
            currentPage = Page::EMPTY_;
            running = false;
            break;
        }
        case Command::COMMENT_: {
            doComment();
            break;
        }
        case Command::POST_: {
            doPost();
            break;
        }
        case Command::NEXT_: {
            doNext();
            break;
        }
        case Command::PREVIOUS_: {
            doPrevious();
            break;
        }
//        case Command::LIKE_: {
//            doLike();
//            break;
//        }
//        case Command::DISLIKE_: {
//            doDislike();
//            break;
//        }
        default: {
            std::cout << "Processing of this command is not implemented yet!\n";
        }
    }
}

void Client::doBack() {
    if (currentPage == Page::POST_) {
        if (messagesStack.size() < 2) {
            std::cout << "Cannot go back here.\n";
        } else {
            messagesStack.erase(messagesStack.end() - 1);
            renderPostPage();
        }
    } else {
        std::cout << "Cannot go back here!\n";
    }
}

void Client::doMore() {
    if (currentPage != Page::POST_) {
        std::cout << "Cannot get more here!\n";
        return;
    }
    auto newID = (messagesStack.end() - 1)->getId();
    newID.addLevel(0);
    Message message;
    if (server.getMessageById(newID, message)) {
        messagesStack.emplace_back(std::move(message));
        renderPostPage();
    } else {
        std::cout << "No more information to show.\n";
    }
}

void Client::doComment() {
    if (currentPage == Page::POST_) {
        std::cout << "Enter your response!\n";
        std::string comment;
        getline(std::cin, comment);
        bool b;
        if (!readBool(b, "Publish this comment (Y/n)?")) {
            std::cout << "Something went wrong...\n";
        } else {
            if (b) {
                auto resID = server.addComment(std::move(comment),
                                  (messagesStack.end() - 1)->getId());
                if (resID.isEmpty()) {
                    std::cout << "Publishing denied!\n";
                } else {
                    Message commentMessage;
                    if (!server.getMessageById(resID, commentMessage)) {
                        std::cout << "Something went wrong!\n";
                    } else {
                        server.getMessageById(
                                (messagesStack.end() - 1)->getId(),
                                *(messagesStack.end() - 1));
                        messagesStack.emplace_back(std::move(commentMessage));
                        renderPostPage();
                    }

                }
            } else {
                std::cout << "Message discarded.\n";
                renderPostPage();
            }
        }

    } else {
        std::cout << "You cannot leve a comment here!\n";
    }
}

void Client::doPost() {
    std::cout << "Not implemented\n";
}

void Client::doNext() {
    if (messagesStack.empty()) {
        std::cout << "Something went wrong :(\n";
        return;
    }
    if (currentPage != Page::POST_ && currentPage != Page::EMPTY_) {
        std::cout << "You cannot look up the next message here!\n";
        return;
    }

    auto requestedId = (messagesStack.end() - 1)->getId();
    ++requestedId;
    // asking for the next post
    if (!server.getMessageById(requestedId, *(messagesStack.end() - 1))) {
        std::cout << "That is all for now!\n";
    } else {
        renderPostPage();
    }
}

void Client::doPrevious() {
    if (messagesStack.empty()) {
        std::cout << "Something went wrong :(\n";
        return;
    }
    if (currentPage != Page::POST_ && currentPage != Page::EMPTY_) {
        std::cout << "You cannot look up the previous message here!\n";
        currentPage = Page::EMPTY_;
        return;
    }
    auto requestedId = (messagesStack.end() - 1)->getId();
    if (requestedId.isFirst()) {
        std::cout << "No previous messages!\n";
        return;
    }
    --requestedId;
    // asking for the previous post
    if (!server.getMessageById(requestedId, *(messagesStack.end() - 1))) {
        std::cout << "Something went wrong..\n";
    } else {
        renderPostPage();
    }
}

void Client::doLike() {

}

void Client::doDislike() {

}

void Client::printHelloMessage() const {
    std::cout << "Hello there! Type 'help' if you need more info.\n";
}

void Client::printHelpMessage() const {
    std::cout
        << "Command available:\n"
        << "\thelp — get more info whenever you need it\n"
        << "\tback — return back\n"
        << "\tmore — more info on current post\n"
        << "\tquit — end the session\n"
        << "\tcomment — add comment on the current post\n"
        << "\tpost — share your story with others\n"
        << "\tnext — move to the next message\n"
        << "\tprevious — move to the previous message\n"
        << "\tlike — like this message\n"
        << "\tdislike — dislike this message\n";
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



