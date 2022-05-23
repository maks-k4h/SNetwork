
#include "Server.h"
#include "Factories/MessageFactory.h"

#include <queue>

Server::Server() {
    loadData();
}

Server::~Server() {
    cleanData();
}

bool Server::loadData() {
    srand(time(nullptr));
    // generating random data for now
    MessageFactory::setMessagesNum(5);
    MessageFactory::setLikesRange(100, 1000);
    MessageFactory::setResponsesRange(0, 10);
    MessageFactory::setSentencesRange(1, 3);
    data = MessageFactory::createMessageSet();
    if (!data)
        return false;

    // adding generated messages into unrated buffer
    std::queue<MessageID> queue;
    queue.push(data->getId());
    while (!queue.empty()) { // BFS
        auto currMessage = messageById(queue.front());
        queue.pop();

        unrated.push_back(currMessage->getId());

        if (currMessage->getNextMessage())
            queue.push(currMessage->getNextMessage()->getId());
        if (currMessage->getResponses())
            queue.push((currMessage->getResponses()->getId()));
    }

    return true;
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

bool Server::getFirstPost(Message &m) const noexcept{
    if (!data)
        return false;
    m = static_cast<Message>(*data);
    return true;
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

    unrated.push_back(response->getId());
    return response->getId();
}

MessageID Server::addPost(std::string &&text) {
    if (!data) {
        data = new MessageNode({0}, text);
        unrated.push_back(data->getId());
        return data->getId();
    }
    auto curr = data;
    while (curr->getNextMessage())
        curr = curr->getNextMessage();
    curr->setNextMessage(new MessageNode(text));
    curr->getNextMessage()->setPreviousMessage(curr);
    curr = curr->getNextMessage();

    unrated.push_back(curr->getId());
    return curr->getId();
}

bool Server::addLike(const MessageID &id) {
    auto message = messageById(id);
    if (!message)
        return false;
    message->addLike();
    processSpam(message);
    return true;
}

bool Server::addDislike(const MessageID &id) {
    auto message = messageById(id);
    if (!message)
        return false;
    message->addDislike();
    processSpam(message);
    return true;
}

bool Server::addReport(const MessageID &id) {
    auto message = messageById(id);
    if (!message)
        return false;
    message->addReport();
    processSpam(message);
    return true;
}
#include <iostream>
MessageNode *Server::messageById(const MessageID &id) const {
    size_t level = 0;
    auto message = data;
    while (message) {
        while (message && message->getId()[level] != id[level])
            message = message->getNextMessage();
        if (!message)
            break;
        ++level;
        if (level == id.getLevelsNum())
            return message;
        message = message->getResponses();
    }
    return nullptr;
}

void Server::processSpam(MessageNode *m) {
    if (m->isSpamMessage()) {
        // deleting the message
        if (!m->isTopLevelMessage()) { // response
            auto parentID = m->getId();
            parentID.removeLastLevel();
            auto parent = messageById(parentID);
            if (!parent) {
                // LOG THE ERROR
                return;
            }
            parent->removeResponse(m);
        } else { // post
            if (m->getPreviousMessage()) {
                m->getPreviousMessage()->
                    setNextMessage(m->getNextMessage());
            }
            if (m->getNextMessage()) {
                m->getNextMessage()->
                    setPreviousMessage(m->getPreviousMessage());
            }
        }
        deleteMessage(m);
    }
}

const std::list<MessageID> &Server::getUnrated() const {
    return unrated;
}

bool Server::setRate(const MessageID &id, double rate) {
    auto r = std::find(unrated.begin(), unrated.end(), id);
    if (r == unrated.end())
        return false;
    unrated.erase(r);
    auto message = messageById(id);
    if (message && !message->setTextSpamScore(rate))
        return false;

    // if no message it's alright, message could be deleted previously
    return true;
}




