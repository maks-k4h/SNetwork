//
// Created by Maks Konevych on 16.04.2022.
//

#include "Client.h"
#include <iostream>

Client::Client(Server &s)
: server{s} {}

void Client::run() {
    // preparing data
    updateMessageStack();
    // running the client
    running = true;
    printHelloMessage();
    renderPostPage();
    while (running) {
        readCommand();
        processCommand();
    }
}

void Client::updateMessageStack() {
    while (!messagesStack.empty()) {
        // updating message
        if (!server.getMessageById((messagesStack.end() - 1)->getId(),
                              *(messagesStack.end() - 1))) {
            messagesStack.erase(messagesStack.end() - 1);
        } else break;
    }
    if (messagesStack.empty()) {
        Message message;
        if (server.getFirstPost(message)) {
            messagesStack.emplace_back(std::move(message));
        } else {
            // no messages to show
        }
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
                        << messagesStack[i].getDislikes() << " dislikes, "
                        << messagesStack[i].getResponsesNum() << " responses. "
                        << "ID: " << messagesStack[i].getId().toString()
                        << '\n';
        }
    }
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
    } else if (command == "report") {
        currentCommand = Command::REPORT_;
    } else {
        currentCommand = Command::INVALID_;
    }
    return true;
}

bool Client::readBool(bool &res, const std::string &m) const {
    std::cout << m << '\n';
    char c {};
    while (std::cout << "> " && std::cin.get(c)) {
        for (char temp; std::cin.get(temp) && temp != '\n';);
        c = tolower(c);
        if (c !='y' && c != 'n') {
            std::cout << "Enter Y or n.\n";
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
        case Command::LIKE_: {
            doLike();
            break;
        }
        case Command::DISLIKE_: {
            doDislike();
            break;
        }
        case Command::REPORT_: {
            doReport();
            break;
        }
        default: {
            std::cout << "Processing of this command is not implemented yet!\n";
        }
    }
}

void Client::doBack() {
    if (messagesStack.size() < 2) {
        std::cout << "Cannot go back here.\n";
    } else {
        messagesStack.erase(messagesStack.end() - 1);
        renderPostPage();
    }
}

void Client::doMore() {
    if (messagesStack.empty()) {
        std::cout << "No more information available.\n";
        return;
    }

    Message message;
    if (server.getFirstResponse((messagesStack.end() - 1)->getId(), message)) {
        messagesStack.emplace_back(std::move(message));
        renderPostPage();
    } else {
        std::cout << "No more information to show.\n";
    }
}

void Client::doComment() {
    if (messagesStack.empty()) {
        std::cout << "Nothing to comment!\n";
        return;
    }
    std::cout << "Enter your response!\n";
    std::string comment;
    getline(std::cin, comment);
    if (comment.empty()) {
        std::cout << "It's alright, come back later...\n";
        return;
    }
    bool b;
    if (!readBool(b, "Publish this comment (Y/n)?")) {
        printSWWMessage();
    } else {
        if (b) {
            auto resID = server.addComment(std::move(comment),
                              (messagesStack.end() - 1)->getId());
            if (resID.isEmpty()) {
                std::cout << "Publishing denied!\n";
            } else {
                Message commentMessage;
                if (!server.getMessageById(resID, commentMessage)) {
                    printSWWMessage();
                    updateMessageStack();
                } else {
                    updateMessageStack();
                    messagesStack.emplace_back(std::move(commentMessage));
                    renderPostPage();
                }

            }
        } else {
            std::cout << "Message discarded.\n";
            renderPostPage();
        }
    }
}

void Client::doPost() {
    std::cout << "What's up? Tell us:\n";
    std::string text;
    std::getline(std::cin, text);
    if (text.empty()) {
        std::cout << "Could you be more specific? ;)\n";
        return;
    }
    bool b;
    if (!readBool(b, "Publish it (Y/n)?")) {
        printSWWMessage();
        return;
    }
    if (b) {
        auto ID = server.addPost(std::move(text));
        if (ID.isEmpty()) {
            std::cout << "Sorry, cannot post your message :(\n";
            return;
        } else {
            Message post;
            if (server.getMessageById(ID, post)) {
                messagesStack.clear();
                messagesStack.emplace_back(std::move(post));
            } else {
                printSWWMessage();
            }
        }
    } else {
        std::cout << "Alright then...\n";
    }
    renderPostPage();
}

void Client::doNext() {
    if (messagesStack.empty()) {
        updateMessageStack();
        renderPostPage();
        return;
    }

    if (!server.getNextMessage((messagesStack.end() - 1)->getId(),
                               *(messagesStack.end() - 1))) {
        std::cout << "That is all for now!\n";
    } else {
        renderPostPage();
    }
}

void Client::doPrevious() {
    if (messagesStack.empty()) {
        std::cout << "No posts yet.\n";
        return;
    }

    if (!server.getPreviousMessage((messagesStack.end() - 1)->getId(),
                                   *(messagesStack.end() - 1))) {
        std::cout << "No previous messages!\n";
    } else {
        renderPostPage();
    }
}

void Client::doLike() {
    if (messagesStack.empty()) {
        std::cout << "Nothing to like...\n";
        return;
    }
    if (!server.addLike((messagesStack.end() - 1)->getId()))
        printSWWMessage();
    else {
        updateMessageStack();
        renderPostPage();
    }
}

void Client::doDislike() {
    if (messagesStack.empty()) {
        std::cout << "Nothing to dislike...\n";
        return;
    }
    if (!server.addDislike((messagesStack.end() - 1)->getId()))
        printSWWMessage();
    else {
        printSWWMessage();
        renderPostPage();
    }
}

void Client::doReport() {
    if (messagesStack.empty()) {
        std::cout << "Nothing to report...\n";
        return;
    }
    if (!server.addReport((messagesStack.end() - 1)->getId()))
        printSWWMessage();
    else {
        std::cout << "Message was successfully reported!\n";
        updateMessageStack();
        renderPostPage();
    }
}

void Client::printHelloMessage() const {
    std::cout <<
    "   S S S     S         S   S S S S S   S S S S S  \n" <<
    " S       S   S S       S   S               S      \n" <<
    " S S         S   S     S   S S S S         S      \n" <<
    "       S S   S     S   S   S               S      \n" <<
    " S       S   S       S S   S               S      \n" <<
    "   S S S     S         S   S S S S S       S    o \n";
    std::cout << "\nHi! Type 'help' if you need more info.\n\n";
}

void Client::printHelpMessage() const {
    std::cout
        << "Commands available:\n"
        << "\thelp ??? get more info whenever you need it\n"
        << "\tback ??? return back\n"
        << "\tmore ??? more info on current post\n"
        << "\tquit ??? end the session\n"
        << "\tcomment ??? add your response to the current post\n"
        << "\tpost ??? share your story with others\n"
        << "\tnext ??? move to the next message\n"
        << "\tprevious ??? move to the previous message\n"
        << "\tlike ??? like this message\n"
        << "\treport ??? report spam\n"
        << "\tdislike ??? dislike this message\n";
}

void Client::printInvalidCommandMessage() const {
    std::cout << "No such command. Type 'help' to get info on commands.\n";
}

void Client::printGoodByeMessage() const {
    std::cout << "Hope to see you soon...\n";
}

void Client::printSWWMessage() const {
    std::cout << "Something went wrong :(\n";
}

void Client::printCharLine(char ch, int n) const {
    while (0 < n--)
        std::cout.put(ch);
}





