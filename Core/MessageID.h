#ifndef CORE_MESSAGEID_H_
#define CORE_MESSAGEID_H_

#include <vector>
#include <string>

class MessageID {
public:
    using vtype_t = size_t;

    MessageID() = default;
    MessageID(vtype_t topLevelId);
    MessageID(const std::vector<vtype_t> &);
    ~MessageID() = default;

    bool isEmpty() const noexcept;
    bool isTopLevel() const noexcept; // true consists of 1 level
    bool isFirst() const noexcept; // true if last level id is 0
    size_t getLevelsNum() const noexcept;
    void addLevel(vtype_t);
    bool removeLastLevel() noexcept;
    bool getLastLevel(vtype_t &) const noexcept;
    bool setLastLevel(vtype_t) noexcept;

    bool operator==(const MessageID &) const;

    vtype_t operator[](size_t i) const;

    // increment/decrement last level id
    MessageID &operator++() noexcept;
    MessageID &operator--() noexcept;

    std::string toString() const;

private:
    std::vector<vtype_t> data;
};


#endif //CORE_MESSAGEID_H_
