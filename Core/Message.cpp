
#include "Message.h"

Message::Message(const std::string &str)
: messText(str), id {0} {}

Message::Message(const MessageID &messageId)
: id {messageId} {}

Message::Message(const MessageID &messageId, const std::string &text)
: id {messageId}, messText {text} {}

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

void Message::setId(const MessageID &newId) {
    id = newId;
}



