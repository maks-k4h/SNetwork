//
// Created by Maks Konevych on 26.04.2022.
//

#include "MessageNode.h"

#include <cmath>

MessageNode::MessageNode(const MessageID &id)
: Message(id), timestamp{time(nullptr)} { }

MessageNode::MessageNode(const std::string &str)
: Message(str), timestamp{time(nullptr)} { }

MessageNode::MessageNode(const MessageID &id, const std::string &str)
: Message(id, str), timestamp{time(nullptr)} { }


void MessageNode::setNextMessage(MessageNode *message) {
    next = message;
    if (!next)
        return;
    if (id.isEmpty())
        return;
    next->id = id;
    ++next->id;
}

MessageNode *MessageNode::getNextMessage() const noexcept {
    return next;
}

void MessageNode::setPreviousMessage(MessageNode *p) {
    previous = p;
}

MessageNode *MessageNode::getPreviousMessage() const noexcept {
    return previous;
}

MessageNode *MessageNode::getResponses() const noexcept {
    return responsesBegin;
}

MessageNode *MessageNode::getLastResponse() const noexcept {
    return responsesTail;
}

void MessageNode::addResponse(MessageNode *newResponse) {
    // changing response's id
    auto newId {id};
    newId.addLevel(getResponsesNum());
    // adding updated response
    newResponse->setId(newId);
    if (responsesBegin) {
        responsesTail->next = newResponse;
        responsesTail->next->previous = responsesTail;
        responsesTail = responsesTail->next;
    } else {
        responsesBegin = responsesTail = newResponse;
    }
    ++responsesNum;
}

void MessageNode::setId(const MessageID &newId) {
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

time_t MessageNode::getTimestamp() const noexcept {
    return timestamp;
}

void MessageNode::addReport() noexcept {
    ++reportsNumber;
}

size_t MessageNode::getReportsNum() const noexcept {
    return reportsNumber;
}

bool MessageNode::setTextSpamScore(double s) noexcept {
    if (s < TSS_MIN || s > TSS_MAX)
        return false;
    tss = s;
    return true;
}

double MessageNode::getTextSpamScore() const noexcept {
    return tss;
}

double MessageNode::calculateRate() const noexcept {
    return tanh(FL_MU * static_cast<double>(reportsNumber)
        / (1 + static_cast<double>(getLikes()))) + tss;
}

bool MessageNode::isSpamMessage() const noexcept {
    return calculateRate() >= FL_H;
}

bool MessageNode::removeResponse(MessageNode *rp) {
    // checking if such a response exist
    auto curr = responsesBegin;
    while (curr && curr != rp)
        curr = curr->next;
    if (!curr)
        return false;

    --responsesNum;
    if (curr->previous) {
        curr->previous->next = curr->next;
    } else {
        responsesBegin = curr->next;
    }

    if (curr->next) {
        curr->next->previous = curr->previous;
    } else {
        responsesTail = curr->previous;
    }

    return true;
}


