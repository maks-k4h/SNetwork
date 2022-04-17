
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

    const Message *getAllData() {return data;}
private:
    void deleteMessage(Message *);
    Message *data {nullptr};
};


#endif //SERVER_SERVER_H_
