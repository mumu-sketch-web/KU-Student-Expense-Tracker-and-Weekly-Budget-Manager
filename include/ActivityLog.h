#pragma once
#include <string>
#include <vector>
#include "Expense.h"


struct ActivityLogEntry {
    std::string action;      // "Added", "Edited", or "Deleted"
    int expenseId;
    std::string category;
    std::string description;
    double amount;
    std::string date;        // the expense's own date, YYYY-MM-DD
    std::string timestamp;   // when the action happened, YYYY-MM-DD HH:MM:SS
};


void logActivity(std::vector<ActivityLogEntry>& log, const std::string& action, const Expense& e);


std::vector<ActivityLogEntry> loadActivityLog();

std::string getNowTimestamp();
