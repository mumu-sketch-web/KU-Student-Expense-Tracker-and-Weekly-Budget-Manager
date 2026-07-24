#include "Budget.h"
#include <set>

void recalculateTotal(Budget& budget, const std::vector<Expense>& list) {
    budget.totalSpent = 0.0;
    for (const auto& item : list) {
        budget.totalSpent += item.amount;
    }
}

double getMonthlyTotal(const std::vector<Expense>& list, const std::string& monthKey) {
    double total = 0.0;
    for (const auto& item : list) {
        if (item.date.size() >= 7 && item.date.substr(0, 7) == monthKey) {
            total += item.amount;
        }
    }
    return total;
}

std::map<std::string, double> getCategoryBreakdown(const std::vector<Expense>& list, const std::string& monthKey) {
    std::map<std::string, double> breakdown;
    for (const auto& item : list) {
        if (item.date.size() >= 7 && item.date.substr(0, 7) == monthKey) {
            breakdown[item.category] += item.amount;
        }
    }
    return breakdown;
}

std::vector<std::string> getAvailableMonths(const std::vector<Expense>& list) {
    std::set<std::string> months;
    for (const auto& item : list) {
        if (item.date.size() >= 7) {
            months.insert(item.date.substr(0, 7));
        }
    }
    // Newest first
    return std::vector<std::string>(months.rbegin(), months.rend());
}
