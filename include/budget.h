#pragma once
#include <vector>
#include "Expense.h"

// Structure representing the overall student budget
struct Budget {
    double limit = 0.0;
    double totalSpent = 0.0;
};

// Backend function declaration for Aakriti's math
void recalculateTotal(Budget& budget, const std::vector<Expense>& list);