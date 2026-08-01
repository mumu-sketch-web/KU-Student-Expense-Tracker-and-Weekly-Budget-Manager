#include "ActivityLog.h"
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>

std::string getNowTimestamp() {
    std::time_t t = std::time(nullptr);
    std::tm tmStruct{};
#if defined(_WIN32)
    localtime_s(&tmStruct, &t);
#else
    localtime_r(&t, &tmStruct);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tmStruct, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void logActivity(std::vector<ActivityLogEntry>& log, const std::string& action, const Expense& e) {
    ActivityLogEntry entry{action, e.id, e.category, e.description, e.amount, e.date, getNowTimestamp()};
    log.push_back(entry);

    std::ofstream out("data/history_log.csv", std::ios::app);
    if (!out.is_open()) return;
    out << entry.timestamp << "," << entry.action << "," << entry.expenseId << ","
        << entry.category << "," << entry.description << "," << entry.amount << "," << entry.date << "\n";
}

std::vector<ActivityLogEntry> loadActivityLog() {
    std::vector<ActivityLogEntry> log;
    std::ifstream in("data/history_log.csv");
    if (!in.is_open()) return log;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string ts, action, idStr, cat, desc, amtStr, dateStr;
        std::getline(ss, ts, ',');
        std::getline(ss, action, ',');
        std::getline(ss, idStr, ',');
        std::getline(ss, cat, ',');
        std::getline(ss, desc, ',');
        std::getline(ss, amtStr, ',');
        std::getline(ss, dateStr, ',');
        try {
            ActivityLogEntry entry;
            entry.timestamp = ts;
            entry.action = action;
            entry.expenseId = std::stoi(idStr);
            entry.category = cat;
            entry.description = desc;
            entry.amount = std::stod(amtStr);
            entry.date = dateStr;
            log.push_back(entry);
        } catch (...) {
            continue;
        }
    }
    return log;
}
