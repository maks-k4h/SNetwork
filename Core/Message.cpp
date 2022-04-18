
#include "Message.h"

Message::Message(const MessageID &messageId)
: id {messageId} {}

Message::Message(const MessageID &messageId, const std::string &text)
        : id {messageId}, messText {text} {}

void Message::setNextMessage(Message *message) {
    if (!message)
        return;
    next = message;
    if (id.isEmpty())
        return;
    next->id = id;
    ++next->id;
}

Message *Message::getNextMessage() const noexcept {
    return next;
}

Message *Message::getResponses() const noexcept {
    return responsesBegin;
}

Message *Message::getLastResponse() const noexcept {
    return responsesTail;
}

size_t Message::getResponsesNum() const noexcept {
    return responsesNum;
}

void Message::setText(const std::string &text) {
    messText = text;
}

const std::string &Message::getText() const {
    return messText;
}

std::string Message::getText() {
    return messText;
}

bool Message::isTopLevelMessage() const noexcept {
    return id.isTopLevel();
}

/*
size_t Message::countMessages() const noexcept {
    size_t result {0};
    auto temp = this;
    while (temp) {
        ++result;
        temp = temp->next;
    }
    return result;
}
 */

size_t Message::getLikes() const noexcept {
    return likes;
}

void Message::addLike() noexcept {
    ++likes;
}

void Message::setLikes(size_t n) noexcept {
    likes = n;
}

bool Message::removeLike() noexcept {
    if (likes == 0)
        return false;
    --likes;
    return true;
}

size_t Message::getDislikes() const noexcept {
    return dislikes;
}

void Message::addDislike() noexcept {
    ++dislikes;
}

void Message::setDislikes(size_t n) noexcept {
    dislikes = n;
}

bool Message::removeDislike() noexcept {
    if (dislikes == 0)
        return false;
    --dislikes;
    return true;
}

const MessageID &Message::getId() const noexcept {
    return id;
}

void Message::setId(const MessageID &newId) noexcept {
    id = newId;
    // setting new ids to all responses (and their responses and so on)
    auto temp {responsesBegin};
    size_t count {0};
    while (temp) {
        auto responseId {id};
        responseId.addLevel(count);
        temp->setId(responseId);
        ++count;
        temp = temp->next;
    }
}

void Message::addResponse(Message *newResponse) {
    // changing response's id
    auto newId {id};
    newId.addLevel(getResponsesNum());
    // adding updated response
    newResponse->setId(newId);
    if (responsesBegin) {
        responsesTail->next = newResponse;
        responsesTail = responsesTail->next;
    } else {
        responsesBegin = responsesTail = newResponse;
    }
    ++responsesNum;
}



