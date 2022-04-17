#ifndef SERVER_MESSAGEID_H_
#define SERVER_MESSAGEID_H_

#include <vector>
#include <string>

class MessageID {
    using vtype_t = size_t;
    std::vector<vtype_t> data;
public:
    MessageID() = default;
    MessageID(vtype_t topLevelId);
    MessageID(std::vector<vtype_t>);
    ~MessageID() = default;

    bool isEmpty() const noexcept;
    bool isTopLevel() const noexcept;
    size_t getLevelsNum() const noexcept;
    void addLevel(vtype_t);
    bool removeLastLevel() noexcept;
    bool removeFirstLever() noexcept;

    vtype_t operator[](size_t i) const;
    bool operator==(const MessageID &) const;

    std::string toString() const;
};


#endif //SERVER_MESSAGEID_H_
