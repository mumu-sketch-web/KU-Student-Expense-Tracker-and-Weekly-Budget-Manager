#pragma once
#include <string>
#include <vector>
#include "Expense.h"

// One line in the "what changed" log shown in the History tab
struct ActivityLogEntry {
    std::string action;      // "Added", "Edited", or "Deleted"
    int expenseId;
    std::string category;
    std::string description;
    double amount;
    std::string date;        // the expense's own date, YYYY-MM-DD
    std::string timestamp;   // when the action happened, YYYY-MM-DD HH:MM:SS
};

// Appends one entry to data/history_log.csv AND to the in-memory vector
void logActivity(std::vector<ActivityLogEntry>& log, const std::string& action, const Expense& e);

// Loads the full log from data/history_log.csv (empty if the file doesn't exist yet)
std::vector<ActivityLogEntry> loadActivityLog();

std::string getNowTimestamp();
