#include "Expense.h"
#include <ctime>
#include <sstream>
#include <iomanip>

void addExpense(std::vector<Expense>& list, int& nextId, const std::string& cat, const std::string& desc, double amt, const std::string& date) {
    // Create a new expense item and push it into the global vector
    Expense newItem;
    newItem.id = nextId++;
    newItem.category = cat;
    newItem.description = desc;
    newItem.amount = amt;
    newItem.date = date.empty() ? getTodayDateString() : date;

    list.push_back(newItem);
}

void deleteExpense(std::vector<Expense>& list, int id) {
    // Loop through and remove the item that matches the target ID
    for (auto it = list.begin(); it != list.end(); ++it) {
        if (it->id == id) {
            list.erase(it);
            break; // Stop searching once deleted
        }
    }
}

std::string getTodayDateString() {
    std::time_t t = std::time(nullptr);
    std::tm tmStruct{};
#if defined(_WIN32)
    localtime_s(&tmStruct, &t);
#else
    localtime_r(&t, &tmStruct);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tmStruct, "%Y-%m-%d");
    return oss.str();
}
