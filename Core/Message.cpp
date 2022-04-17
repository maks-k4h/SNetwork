
#include "Message.h"

Message::Message(const MessageID &messageId)
: id {messageId} {}

Message *Message::nextMessage() const noexcept {
    return next;
}

Message *Message::getResponses() const noexcept {
    return responsesBegin;
}

Message *Message::getLastResponse() const noexcept {
    return responsesTail;
}

void Message::setText(const std::string &text) {
    messText = text;
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

size_t Message::getResponsesNum() const noexcept {
   return responsesNum;
}

Message::Message(const MessageID &messageId, const std::string &text)
: id {messageId}, messText {text} {}


size_t Message::getLikes() const noexcept {
    return likes;
}

void Message::addLike() noexcept {
    ++likes;
}

void Message::setLikes(size_t l) noexcept {
    likes = l;
}

bool Message::removeLike() noexcept {
    if (likes == 0)
        return false;
    --likes;
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
        auto responseId = id;
        responseId.addLevel(count);
        temp->setId(responseId);
        ++count;
        temp = temp->next;
    }
}

void Message::addResponse(Message *newResponse) {
    // changing response's id
    auto newId = id;
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

const std::string &Message::getText() const {
    return messText;
}

std::string Message::getText() {
    return messText;
}



