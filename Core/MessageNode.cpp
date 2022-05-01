//
// Created by Maks Konevych on 26.04.2022.
//

#include "MessageNode.h"

MessageNode::MessageNode(const MessageID &id)
: Message(id), timestamp{time(nullptr)} { }

MessageNode::MessageNode(const std::string &str)
: Message(str), timestamp{time(nullptr)} { }

MessageNode::MessageNode(const MessageID &id, const std::string &str)
: Message(id, str), timestamp{time(nullptr)} { }


void MessageNode::setNextMessage(MessageNode *message) {
    if (!message)
        return;
    next = message;
    if (id.isEmpty())
        return;
    next->id = id;
    ++next->id;
}

MessageNode *MessageNode::getNextMessage() const noexcept {
    return next;
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


