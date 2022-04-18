//
// Created by Maks Konevych on 16.04.2022.
//

#include "MessageFactory.h"

size_t MessageFactory::messNum {0};
std::array<size_t, 2> MessageFactory::likesRange {0, 100};
std::array<size_t, 2> MessageFactory::responsesRange {0, 5};

static const std::vector<std::string> wordsDictionary {
    "ability", "absorb", "accept", "affair",
    "balance", "bear", "best", "blind",
    "carrier", "chain", "charity",
    "deep", "difficulty", "dish",
    "do", "drink", "drive",
    "eat", "education", "elsewhere",
    "face", "far", "figure",
    "gain", "gray", "gun",
    "half", "helpful", "hero",
    "imagine", "Japanese", "joy",
    "keep", "killing", "knife",
    "lady", "laugh", "lots",
    "make", "much", "money",
    "need", "new", "network",
    "odd", "organic", "orange",
    "prefer", "peace", "presence",
    "qualify", "quality", "quick",
    "rope", "red", "reporter",
    "save", "scientific", "skin",
    "take", "try", "today",
    "use", "unusual", "utility",
    "very", "visible", "vessel",
    "we", "walk", "wealthy",
    "young", "yellow", "yard",
    "zoo", "zone"
};

void MessageFactory::setMessagesNum(size_t n) noexcept {
    messNum = n;
}

bool MessageFactory::setLikesRange(size_t a, size_t b) noexcept {
    if (a > b)
        return false;
    MessageFactory::likesRange = {a, b};
    return true;
}

bool MessageFactory::setResponsesRange(size_t a, size_t b) noexcept {
    if (a > b)
        return false;
    responsesRange = {a, b};
    return true;
}

Message *MessageFactory::createOneMessage() {
    auto res = new Message(0, generateBodyText(1 + rand() % 10));
    res->setLikes(likesRange[0] + rand() % (likesRange[1] - likesRange[0] + 1));
    auto responsesNumber = responsesRange[0]
            + rand() % (responsesRange[1] - responsesRange[0] + 1);
    if(responsesRange[1] == 0)
        return res;

    // generating responses
    auto tempResRange = responsesRange;
    responsesRange[0] /= 3; responsesRange[1] /= 3;
    auto tempLikesRange = likesRange;
    likesRange[0] /= 5; likesRange[1] /= 5;
    for (size_t response {0}; response < responsesNumber; ++response)
        res->addResponse(createOneMessage());

    // restoring ranges
    responsesRange = tempResRange;
    likesRange = tempLikesRange;
    return res;
}

Message *MessageFactory::createMessageSet() {
    if (messNum < 1)
        return nullptr;
    auto res = createOneMessage();
    auto curr = res;
    for (size_t i = 1; i < messNum; ++i) {
        curr->setNextMessage(createOneMessage());
        curr = curr->getNextMessage();
        curr->setId(i);
    }
    return res;
}

std::string MessageFactory::generateBodyText(size_t sentences) {
    std::string res;
    while (sentences--) {
        res += generateSentence() + ' ';
    }
    return std::move(res);
}

std::string MessageFactory::generateSentence() {
    std::string sentence = getRandomWord(true);
    auto length = rand() % 10;
    while (length--)
        sentence += ' ' + getRandomWord(false);
    sentence += '.';
    return std::move(sentence);
}

std::string MessageFactory::getRandomWord(bool capital) {
    auto word = wordsDictionary[rand() % wordsDictionary.size()];
    if (capital)
        word[0] = toupper(word[0]);
    return std::move(word);
}
