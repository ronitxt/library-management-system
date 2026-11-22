#include "Member.h"
#include <utility>

Member::Member(int id, std::string name, std::string email)
    : id_(id), name_(std::move(name)), email_(std::move(email)) {}

int Member::getId() const { return id_; }
const std::string& Member::getName() const { return name_; }
const std::string& Member::getEmail() const { return email_; }
