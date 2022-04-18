
#include "MessageID.h"

MessageID::MessageID(MessageID::vtype_t topLevelId)
: data{topLevelId} {}

MessageID::MessageID(const std::vector<vtype_t> &vec)
: data{vec} {}

bool MessageID::isEmpty() const noexcept {
    return data.empty();
}

bool MessageID::isTopLevel() const noexcept {
    return data.size() == 1;
}

bool MessageID::isFirst() const noexcept {
    return 0 == *(data.end() - 1);
}

size_t MessageID::getLevelsNum() const noexcept {
    return data.size();
}

void MessageID::addLevel(MessageID::vtype_t idOnLevel) {
    data.push_back(idOnLevel);
}

bool MessageID::removeLastLevel() noexcept {
    if (data.empty())
        return false;
    data.erase(data.end() - 1);
    return true;
}

bool MessageID::getLastLevel(MessageID::vtype_t &n) const noexcept {
    if (isEmpty())
        return false;
    n = *(data.end() - 1);
    return true;
}

bool MessageID::setLastLevel(MessageID::vtype_t n) noexcept {
    if(isEmpty())
        return false;
    *(data.end() - 1) = n;
    return true;
}

std::string MessageID::toString() const {
    std::string res;
    for (size_t i {0}; i < data.size(); ++i) {
        if (i != 0)
            res += '-';
        res += std::to_string(data[i]);
    }
    return res;
}

MessageID::vtype_t MessageID::operator[](size_t i) const {
    return data[i];
}

bool MessageID::operator==(const MessageID &that) const {
    return data == that.data;
}

MessageID &MessageID::operator++() noexcept {
    if (!isEmpty()) {
        ++*(data.end() - 1);
    }
    return *this;
}

MessageID &MessageID::operator--() noexcept {
    if (!isEmpty()) {
        --*(data.end() - 1);
    }
    return *this;
}




