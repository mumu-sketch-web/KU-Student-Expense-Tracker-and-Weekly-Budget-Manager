#pragma once
#include <vector>
#include <map>
#include <string>
#include "Expense.h"


struct Budget {
    double limit = 0.0;
    double totalSpent = 0.0;
};


void recalculateTotal(Budget& budget, const std::vector<Expense>& list);


double getMonthlyTotal(const std::vector<Expense>& list, const std::string& monthKey);
std::map<std::string, double> getCategoryBreakdown(const std::vector<Expense>& list, const std::string& monthKey);


std::vector<std::string> getAvailableMonths(const std::vector<Expense>& list);
