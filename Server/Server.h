
#ifndef SERVER_SERVER_H_
#define SERVER_SERVER_H_

#include "../Core/MessageNode.h"

class Server {
public:
    Server() = default;
    Server(const Server &) = default;
    ~Server();

    // loads data from some source
    bool loadData();
    // called by destructor, though can be called manually
    void cleanData();

    bool getMessageById(const MessageID &, Message &m) const;

    // Both return response's id on success or an empty one on fail
    MessageID addComment(std::string &&, const MessageID &);
    MessageID addPost(std::string &&);

    bool addLike(const MessageID &);
    bool addDislike(const MessageID &);

    bool addReport(const MessageID &);

private:
    // nullptr on fail
    MessageNode *messageById(const MessageID &) const;
    void deleteMessage(MessageNode *);
    MessageNode *data {nullptr};
};


#endif //SERVER_SERVER_H_
