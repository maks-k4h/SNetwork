
#include "Server.h"

#include "Factories/MessageFactory.h"

Server::~Server() {
    cleanData();
}

bool Server::loadData() {
    srand(time(nullptr));
    // generating random data for now
    MessageFactory::setMessagesNum(3);
    MessageFactory::setLikesRange(0, 1000);
    MessageFactory::setResponsesRange(0, 6);
    MessageFactory::setSentencesRange(1, 3);
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

void Server::deleteMessage(MessageNode *message) {
    if (!message)
        return;
    auto responses = message->getResponses();
    while (responses) {
        auto temp = responses->getNextMessage();
        deleteMessage(responses);
        responses = temp;
    }
    delete message;
}

bool Server::getMessageById(const MessageID& id, Message &m) const {
    auto message = messageById(id);
    if (message) {
        m = static_cast<Message>(*message);
        return true;
    }
    return false;
}

bool Server::getNextMessage(const MessageID &id, Message &m) const {
    auto message = messageById(id);
    if (!message || message->getNextMessage() == nullptr)
        return false;
    m = static_cast<Message>(*message->getNextMessage());
    return true;
}

bool Server::getPreviousMessage(const MessageID &id, Message &m) const {
    auto message = messageById(id);
    if (!message || message->getPreviousMessage() == nullptr)
        return false;
    m = static_cast<Message>(*message->getPreviousMessage());
    return true;
}

bool Server::getFirstResponse(const MessageID &id, Message &m) const {
    auto message = messageById(id);
    if (!message || message->getResponses() == nullptr)
        return false;
    m = static_cast<Message>(*message->getResponses());
    return true;
}

MessageID Server::addComment(std::string &&text, const MessageID &id) {
    auto message = messageById(id);
    if (!message) // no message with such id
        return {};
    auto response = new MessageNode(text);
    message->addResponse(response); // id gets updated here
    return response->getId();
}

MessageID Server::addPost(std::string &&text) {
    if (!data)
        return {};
    auto curr = data;
    while (curr->getNextMessage())
        curr = curr->getNextMessage();
    curr->setNextMessage(new MessageNode(text));
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

bool Server::addReport(const MessageID &id) {
    auto message = messageById(id);
    if (!message)
        return false;
    message->addReport();
    return true;
}

MessageNode *Server::messageById(const MessageID &id) const {
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




