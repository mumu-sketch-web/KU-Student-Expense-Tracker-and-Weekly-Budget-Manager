#pragma once
#include <vector>
#include <map>
#include <string>
#include "Expense.h"

// Structure representing the overall student budget
struct Budget {
    double limit = 0.0;
    double totalSpent = 0.0;
};

// Backend function declaration for Aakriti's math
void recalculateTotal(Budget& budget, const std::vector<Expense>& list);

// Monthly analytics helpers. monthKey format: "YYYY-MM"
double getMonthlyTotal(const std::vector<Expense>& list, const std::string& monthKey);
std::map<std::string, double> getCategoryBreakdown(const std::vector<Expense>& list, const std::string& monthKey);

// Returns every distinct "YYYY-MM" present in the expense list, newest first
std::vector<std::string> getAvailableMonths(const std::vector<Expense>& list);
