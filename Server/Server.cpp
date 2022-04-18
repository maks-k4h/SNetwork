
#include "Server.h"

#include "Factories/MessageFactory.h"

Server::~Server() {
    cleanData();
}

bool Server::loadData() {
    // generating random data for now
    MessageFactory::setMessagesNum(3);
    MessageFactory::setLikesRange(0, 1000);
    MessageFactory::setResponsesRange(0, 6);
    data = MessageFactory::createMessageSet();
    return data != nullptr;
}

void Server::cleanData() {
    while (data) {
        auto temp = data->getNextMessage();
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
        auto temp = responses->getNextMessage();
        deleteMessage(responses);
        responses = temp;
    }
}

bool Server::getMessageById(const MessageID& id, Message &m) const {
    if (id.isEmpty())
        return false;
    auto message = messageById(id);
    if (message)
        m = *message;
    return nullptr != message;
}

MessageID Server::addComment(std::string &&text, const MessageID &id) {
    auto message = messageById(id);
    if (!message) // no message with such id
        return {};
    auto response = new Message(0, text);
    message->addResponse(response);
    return response->getId();
}

MessageID Server::addPost(std::string &&text) {
    if (!data)
        return {};
    auto curr = data;
    while (curr->getNextMessage())
        curr = curr->getNextMessage();
    curr->setNextMessage(new Message(0, text));
    return curr->getNextMessage()->getId();
}

bool Server::addLike(const MessageID &id) {
    auto message = messageById(id);
    if (!message)
        return false;
    message->addLike();
    return true;
}

bool Server::addDislike(const MessageID &id) {
    auto message = messageById(id);
    if (!message)
        return false;
    message->addDislike();
    return true;
}

Message *Server::messageById(const MessageID &id) const {
    size_t level = 0;
    auto message = data;
    while (message) {
        while (message && message->getId()[level] < id[level])
            message = message->getNextMessage();
        if (!message)
            break;
        ++level;
        if (level == id.getLevelsNum()) {
            return message;
        }
        message = message->getResponses();
    }
    return nullptr;
}




