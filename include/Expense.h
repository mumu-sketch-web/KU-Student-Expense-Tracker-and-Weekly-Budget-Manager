#pragma once
#include <string>
#include <vector>


struct Expense {
    int id;
    std::string category;
    std::string description;
    double amount;
    std::string date; // format: YYYY-MM-DD
};


void addExpense(std::vector<Expense>& list, int& nextId, const std::string& cat, const std::string& desc, double amt, const std::string& date);
void deleteExpense(std::vector<Expense>& list, int id);
bool editExpense(std::vector<Expense>& list, int id, const std::string& cat, const std::string& desc, double amt, const std::string& date);


std::string getTodayDateString();
