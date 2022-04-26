
#ifndef CLIENT_CLIENT_H_
#define CLIENT_CLIENT_H_

#include "../Server/Server.h"
#include "../Core/Message.h"

#include <vector>
#include <string>
#include <iostream>

class Client {
public:
    Client() = default;
    Client(const Client &) = default;
    ~Client() = default;

    void run();
private:
    Server server;
    bool running {false};
    std::vector<Message> messagesStack;

    void prepareData();

    enum class Command {
        HELP_,
        BACK_,
        MORE_,
        QUIT_,
        COMMENT_,
        POST_,
        NEXT_,
        PREVIOUS_,
        LIKE_,
        DISLIKE_,
        INVALID_
    };
    Command currentCommand {Command::INVALID_};

    void doBack();
    void doMore();
    void doComment();
    void doPost();
    void doNext();
    void doPrevious();
    void doLike();
    void doDislike();

    bool readCommand();
    bool readBool(bool &res, const std::string & = "Y/n?") const;
    void processCommand();

    void renderPostPage() const;
    void printHelloMessage() const;
    void printHelpMessage() const;
    void printGoodByeMessage() const;
    void printInvalidCommandMessage() const;
    void printSWWMessage() const;

    void printCharLine(char, int n) const;

};


#endif //CLIENT_CLIENT_H_
