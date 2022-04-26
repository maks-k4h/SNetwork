
#ifndef CORE_MESSAGE_H_
#define CORE_MESSAGE_H_

#include "MessageID.h"

#include <string>
#include <cstdlib>

class Message {
public:
    Message() = default;
    Message(const std::string &);
    explicit Message(const MessageID &);
    explicit Message(const MessageID &, const std::string &);
    Message(const Message &) = default;
    virtual ~Message() = default;

    // may change id
    size_t getResponsesNum() const noexcept;

    void setText(const std::string &);
    const std::string &getText() const;
    std::string getText();

    // sets new ids of responses too
    virtual void setId(const MessageID&);
    const MessageID& getId() const noexcept;

    size_t getLikes() const noexcept;
    void setLikes(size_t) noexcept;
    void addLike() noexcept;
    bool removeLike() noexcept;

    size_t getDislikes() const noexcept;
    void setDislikes(size_t) noexcept;
    void addDislike() noexcept;
    bool removeDislike() noexcept;

    bool isTopLevelMessage() const noexcept; // ~ has one-level id

protected:
    MessageID id;
    size_t responsesNum {0};

private:
    std::string messText;
    size_t likes {0};
    size_t dislikes {0};
};


#endif //CORE_MESSAGE_H_
