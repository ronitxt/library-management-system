#pragma once
#include <string>

class Member {
public:
    Member() = default;
    Member(int id, std::string name, std::string email);

    int getId() const;
    const std::string& getName() const;
    const std::string& getEmail() const;

private:
    int id_ = 0;
    std::string name_;
    std::string email_;
};
