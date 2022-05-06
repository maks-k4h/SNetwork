
#ifndef SERVER_SERVER_H_
#define SERVER_SERVER_H_

#include "../Core/MessageNode.h"

#include <list>

class Server {
public:
    Server();
    Server(const Server &) = default;
    ~Server();

    // loads data from some source
    bool loadData();
    // called by destructor, though can be called manually
    void cleanData();

    bool getMessageById(const MessageID &, Message &m) const;
    bool getNextMessage(const MessageID &, Message &m) const;
    bool getPreviousMessage(const MessageID &, Message &m) const;
    bool getFirstResponse(const MessageID &, Message &m) const;

    // Both return response's id on success or an empty one on fail
    MessageID addComment(std::string &&, const MessageID &);
    MessageID addPost(std::string &&);

    bool addLike(const MessageID &);
    bool addDislike(const MessageID &);

    bool addReport(const MessageID &);

    const std::list<MessageID> &getUnrated() const;
    bool setRate(const MessageID &, double);

private:
    // nullptr on fail
    MessageNode *messageById(const MessageID &) const;
    void deleteMessage(MessageNode *);

    void processSpam(MessageNode *);
    MessageNode *data {nullptr};

    std::list<MessageID> unrated;
};


#endif //SERVER_SERVER_H_
