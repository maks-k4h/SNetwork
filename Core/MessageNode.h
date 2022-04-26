
#ifndef CORE_MESSAGENODE_H_
#define CORE_MESSAGENODE_H_

#include "Message.h"

class MessageNode : public Message {
public:
    MessageNode() = default;
    MessageNode(const std::string &);
    explicit MessageNode(const MessageID &);
    explicit MessageNode(const MessageID &, const std::string &);
    MessageNode(const MessageNode &) = default;
    ~MessageNode() override = default;

    void setId(const MessageID &) override;

    // may change id
    void setNextMessage(MessageNode *);
    MessageNode *getNextMessage() const noexcept;


    void addResponse(MessageNode *);
    MessageNode *getResponses() const noexcept;
    MessageNode *getLastResponse() const noexcept;


private:
    MessageNode *next {nullptr};
    MessageNode *responsesBegin {nullptr};
    MessageNode *responsesTail {nullptr};
};


#endif //CORE_MESSAGENODE_H_
