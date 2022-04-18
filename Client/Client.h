
#ifndef CLIENT_CLIENT_H_
#define CLIENT_CLIENT_H_

#include "../Server/Server.h"
#include "../Core/Message.h"

#include <vector>
#include <string>
#include <iostream>

class Client {
public:
    Client();
    void run();

private:
    Server server;

    std::vector<Message> messagesStack;
    void prepareData();

    bool running {false};

    enum class Page {
        POST_,
        COMMENT_,
        NEW_POST_,
        EMPTY_
    };
    Page currentPage {Page::POST_};

    void renderPostPage() const;
    void renderCommentPage() const;
    void renderNewPostPage() const;

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

    bool readCommand();
    bool readBool(bool &res, const std::string & = "Y/n?") const;
    void processCommand();

    void doBack();
    void doMore();
    void doComment();
    void doPost();
    void doNext();
    void doPrevious();
    void doLike();
    void doDislike();

    void printHelloMessage() const;
    void printHelpMessage() const;
    void printGoodByeMessage() const;
    void printInvalidCommandMessage() const;

    void printCharLine(char, int n) const;

};


#endif //CLIENT_CLIENT_H_
