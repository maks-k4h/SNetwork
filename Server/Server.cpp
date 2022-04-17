
#include "Server.h"

#include "Factories/MessageFactory.h"

Server::~Server() {
    cleanData();
}

bool Server::loadData() {
    // generating random data for now
    MessageFactory::setMessagesNum(10);
    MessageFactory::setLikesRange(0, 1000);
    MessageFactory::setResponsesRange(0, 6);
    data = MessageFactory::createOneMessage();
    return data != nullptr;
}

void Server::cleanData() {
    while (data) {
        auto temp = data->nextMessage();
        deleteMessage(data);
        data = temp;
    }
}

void Server::deleteMessage(Message *message) {
    if (!message)
        return;
    auto responses = message->getResponses();
    delete message;
    while (responses) {
        auto temp = responses->nextMessage();
        deleteMessage(responses);
        responses = temp;
    }
}

bool Server::getMessageById(const MessageID& id, Message &m) const {
    if (id.isEmpty())
        return false;
    size_t level = 0;
    auto message = data;
    while (message) {
        while (message && message->getId()[level] < id[level])
            message = message->nextMessage();
        if (!message)
            return false;
        ++level;
        if (level == id.getLevelsNum()) {
            m = *message;
            return true;
        }
        message = message->getResponses();
    }
    return false;
}


