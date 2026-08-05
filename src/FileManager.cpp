#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

void saveToFile(const std::vector<Expense>& list, const Budget& budget) {
    std::ofstream out("data/expenses.csv");
    if (!out.is_open()) {
        std::cerr << "Error: Could not open data/expenses.csv for saving." << std::endl;
        return;
    }
    
    
    out << budget.limit << "\n";
    
    
    for (const auto& item : list) {
        out << item.id << "," << item.category << "," << item.description << "," << item.amount << "," << item.date << "\n";
    }
    out.close();
}

void loadFromFile(std::vector<Expense>& list, Budget& budget, int& nextId) {
    std::ifstream in("data/expenses.csv");
    if (!in.is_open()) {
        return; 
    }
    
    list.clear();
    std::string line;
    
   
    if (std::getline(in, line)) {
        try {
            budget.limit = std::stod(line);
        } catch (...) {
            budget.limit = 0.0;
        }
    }
    
    
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string idStr, cat, desc, amtStr, dateStr;
        
        std::getline(ss, idStr, ',');
        std::getline(ss, cat, ',');
        std::getline(ss, desc, ',');
        std::getline(ss, amtStr, ',');
        std::getline(ss, dateStr, ','); // Older rows saved before this feature won't have a date

        try {
            int id = std::stoi(idStr);
            double amt = std::stod(amtStr);
            if (dateStr.empty()) {
                dateStr = "Unknown";
            }
            list.push_back({id, cat, desc, amt, dateStr});
            
            
            if (id >= nextId) {
                nextId = id + 1;
            }
        } catch (...) {
            continue; // Skip malformed lines
        }
    }
    in.close();
}
