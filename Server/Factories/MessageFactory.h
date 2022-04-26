
#ifndef SERVER_FACTORIES_MESSAGEFACTORY_H_
#define SERVER_FACTORIES_MESSAGEFACTORY_H_

#include "../../Core/MessageNode.h"

#include <tuple>
#include <cstdlib>
#include <array>
#include <vector>

class MessageFactory {
public:
    MessageFactory() = delete;

    static void setMessagesNum(size_t) noexcept;
    static bool setLikesRange(size_t a, size_t b) noexcept;
    static bool setResponsesRange(size_t a, size_t b) noexcept;

    static MessageNode *createOneMessage();
    static MessageNode *createMessageSet();

    static std::string generateBodyText(size_t sentences = 5);
    static std::string generateSentence();
    static std::string getRandomWord(bool capital = false);

private:

    static size_t messNum;
    static std::array<size_t, 2> likesRange;
    static std::array<size_t, 2> responsesRange;
};


#endif //SERVER_FACTORIES_MESSAGEFACTORY_H_
