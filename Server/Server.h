
#ifndef SERVER_SERVER_H_
#define SERVER_SERVER_H_

#include "../Core/Message.h"

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

    const Message *getAllData() {return data;}
private:
    // nullptr on fail
    Message *messageById(const MessageID &) const;
    void deleteMessage(Message *);
    Message *data {nullptr};
};


#endif //SERVER_SERVER_H_
