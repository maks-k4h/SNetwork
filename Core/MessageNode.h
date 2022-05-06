
#ifndef CORE_MESSAGENODE_H_
#define CORE_MESSAGENODE_H_

#include "Message.h"

#include <ctime>

constexpr double FL_MU {2};
constexpr double FL_H {0.98};
constexpr double TSS_MAX {1}; // text spam score max
constexpr double TSS_MIN {0}; // text spam score min

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

    void setPreviousMessage(MessageNode *);
    MessageNode *getPreviousMessage() const noexcept;


    void addResponse(MessageNode *);
    MessageNode *getResponses() const noexcept;
    MessageNode *getLastResponse() const noexcept;

    time_t getTimestamp() const noexcept;

    void addReport() noexcept;
    size_t getReportsNum() const noexcept;

    bool setTextSpamScore(double) noexcept;
    double getTextSpamScore() const noexcept;

    double calculateRate() const noexcept;
    bool isSpamMessage() const noexcept;

private:
    MessageNode *next {nullptr};
    MessageNode *previous {nullptr};
    MessageNode *responsesBegin {nullptr};
    MessageNode *responsesTail {nullptr};

    time_t timestamp {0};
    size_t reportsNumber {0};
    double tss {0}; // text spam score

};


#endif //CORE_MESSAGENODE_H_
