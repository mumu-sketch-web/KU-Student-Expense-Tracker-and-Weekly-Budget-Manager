#pragma once
#include <string>
#include <vector>

// Simple list of available categories for student expenses
inline std::vector<std::string> getCategories() {
    return {
        "Food & Dining",
        "Books & Supplies",
        "Transportation",
        "Hostel / Rent",
        "Entertainment",
        "Other"
    };
}