
#include "MessageID.h"

MessageID::MessageID(MessageID::vtype_t topLevelId)
: data(1, topLevelId) {}

MessageID::MessageID(std::vector<vtype_t> vec)
: data{std::move(vec)} {}

bool MessageID::isEmpty() const noexcept {
    return data.empty();
}

bool MessageID::isTopLevel() const noexcept {
    return data.size() == 1;
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

bool MessageID::removeFirstLever() noexcept {
    if (data.empty())
        return false;
    data.erase(data.begin());
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


