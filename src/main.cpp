#include <iostream>
#include "../include/User.h"

int main() {

    User user("Shravik");

    std::cout << "Expense Tracker Started" << std::endl;
    std::cout << user.getName() << std::endl;

    return 0;
}