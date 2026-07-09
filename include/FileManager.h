#pragma once
#include <vector>
#include "Expense.h"
#include "Budget.h"

void saveToFile(const std::vector<Expense>& list, const Budget& budget);
void loadFromFile(std::vector<Expense>& list, Budget& budget, int& nextId);