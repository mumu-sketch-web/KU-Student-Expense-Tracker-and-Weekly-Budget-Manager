#ifndef USER_H
#define USER_H

#include <string>

class User {
private:
    std::string name;

public:
    User(std::string name);
    std::string getName();
};

#endif