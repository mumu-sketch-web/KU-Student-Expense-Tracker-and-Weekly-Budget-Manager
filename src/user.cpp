#include "../include/User.h"

User::User(std::string name) {
    this->name = name;
}

std::string User::getName() {
    return name;
}