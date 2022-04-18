#ifndef SERVER_MESSAGEID_H_
#define SERVER_MESSAGEID_H_

#include <vector>
#include <string>

class MessageID {
public:
    using vtype_t = size_t;

    MessageID() = default;
    MessageID(vtype_t topLevelId);
    MessageID(std::vector<vtype_t>);
    ~MessageID() = default;

    bool isEmpty() const noexcept;
    bool isTopLevel() const noexcept;
    bool isFirst() const noexcept;
    size_t getLevelsNum() const noexcept;
    void addLevel(vtype_t);
    bool removeLastLevel() noexcept;
    bool removeFirstLever() noexcept;
    bool getLastLevel(vtype_t &) const noexcept;
    bool setLastLevel(vtype_t) noexcept;

    vtype_t operator[](size_t i) const;
    bool operator==(const MessageID &) const;
    MessageID &operator++() noexcept;
    MessageID &operator--() noexcept;

    std::string toString() const;

private:
    std::vector<vtype_t> data;
};


#endif //SERVER_MESSAGEID_H_
