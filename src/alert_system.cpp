#include "AlertSystem.h"

std::string checkBudgetAlert(const Budget& budget) {
    if (budget.limit <= 0.0) {
        return "Please set a weekly budget limit above!";
    }
    
    double ratio = budget.totalSpent / budget.limit;
    
    if (ratio >= 1.0) {
        return "⚠️ CRITICAL: Budget Exceeded! You have spent 100% or more!";
    } else if (ratio >= 0.80) {
        return "⚡ WARNING: Running Low! Over 80% of budget consumed.";
    } else {
        return "✅ Budget Healthy: You are within your weekly limits.";
    }
}
