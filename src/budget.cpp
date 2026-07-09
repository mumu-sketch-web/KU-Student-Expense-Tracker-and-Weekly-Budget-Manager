#include "Budget.h"

void recalculateTotal(Budget& budget, const std::vector<Expense>& list) {
    budget.totalSpent = 0.0;
    for (const auto& item : list) {
        budget.totalSpent += item.amount;
    }
}