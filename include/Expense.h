#pragma once
#include <string>
#include <vector>

// Structure representing a single expense item
struct Expense {
    int id;
    std::string category;
    std::string description;
    double amount;
    std::string date; // format: YYYY-MM-DD
};

// Backend function declarations for Shreya's logic
void addExpense(std::vector<Expense>& list, int& nextId, const std::string& cat, const std::string& desc, double amt, const std::string& date);
void deleteExpense(std::vector<Expense>& list, int id);
bool editExpense(std::vector<Expense>& list, int id, const std::string& cat, const std::string& desc, double amt, const std::string& date);

// Returns today's date as YYYY-MM-DD (pure C++, no Qt dependency)
std::string getTodayDateString();
