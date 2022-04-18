
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
    void cleanData();

    // Returns message as a separate node
    bool getMessageById(const MessageID &, Message &m) const;

    // return response's id on success or an empty one on fail
    MessageID addComment(std::string &&, const MessageID &);

    const Message *getAllData() {return data;}
private:
    Message *messageById(const MessageID &) const;
    void deleteMessage(Message *);
    Message *data {nullptr};
};


#endif //SERVER_SERVER_H_
