#pragma once
#include <string>
#include "Budget.h"

// Returns a warning string depending on how much of the budget is spent
std::string checkBudgetAlert(const Budget& budget);
