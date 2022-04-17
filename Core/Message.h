
#ifndef CORE_MESSAGE_H_
#define CORE_MESSAGE_H_

#include "MessageID.h"

#include <string>
#include <cstdlib>

class Message {
public:
    Message() = default;
    explicit Message(const MessageID &);
    explicit Message(const MessageID &, const std::string &);
    Message(const Message &) = default;
    ~Message() = default;

    Message *nextMessage() const noexcept;
    Message *getResponses() const noexcept;
    Message *getLastResponse() const noexcept;

    void setText(const std::string &);
    const std::string &getText() const;
    std::string getText();

    size_t getResponsesNum() const noexcept;

    void addResponse(Message *);

    // sets new responses' ids too
    void setId(const MessageID&) noexcept;
    const MessageID& getId() const noexcept;

    size_t getLikes() const noexcept;
    void addLike() noexcept;
    void setLikes(size_t) noexcept;
    bool removeLike() noexcept;

    bool isTopLevelMessage() const noexcept;
private:
    Message *next {nullptr};

    MessageID id;
    std::string messText;
    size_t likes {0};
    size_t responsesNum {0};
    Message *responsesBegin {nullptr};
    Message *responsesTail {nullptr};
};


#endif //CORE_MESSAGE_H_
