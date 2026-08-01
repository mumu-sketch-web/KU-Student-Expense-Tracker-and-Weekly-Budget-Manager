#pragma once
#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QCalendarWidget>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>
#include <vector>
#include "Expense.h"
#include "Budget.h"
#include "ActivityLog.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleAddOrUpdateExpense();
    void handleEditExpense();
    void handleDeleteExpense();
    void handleSetBudget();
    void handleHistoryFilterChanged(int index);
    void handleCalendarPageChanged(int year, int month);
    void handleCalendarDateClicked(const QDate& date);

private:
    void setupUI();
    QWidget* createDashboardTab();
    QWidget* createHistoryTab();
    QWidget* createAnalysisTab();

    void refreshDashboard();
    void refreshHistory();
    void refreshAnalysis();
    void refreshDayView(const QDate& date);
    void populateHistoryMonthDropdown();
    void resetExpenseForm();

    std::vector<Expense> expenseList;
    std::vector<ActivityLogEntry> activityLog;
    Budget currentBudget;
    int nextId = 1;
    int editingExpenseId = -1; // -1 = adding a new expense, otherwise the id being edited

    QTabWidget* tabWidget;

    // --- Dashboard tab ---
    QDoubleSpinBox* budgetInput;
    QLabel* totalSpentLabel;
    QLabel* alertStatusLabel;
    QProgressBar* budgetProgressBar;

    QComboBox* categoryDropdown;
    QLineEdit* descriptionInput;
    QDoubleSpinBox* amountInput;
    QDateEdit* dateInput;
    QPushButton* addOrUpdateButton;
    QPushButton* cancelEditButton;

    QTableWidget* expenseTable;
    QPushButton* editButton;
    QPushButton* deleteButton;

    // --- History tab (read-only activity log) ---
    QComboBox* historyMonthFilter;
    QTableWidget* historyTable;

    // --- Monthly Analysis tab ---
    QCalendarWidget* analysisCalendar;
    QLabel* analysisTotalLabel;
    QTableWidget* categoryBreakdownTable;
    QLabel* selectedDayLabel;
    QTableWidget* dayExpenseTable;
    QDate selectedDay;
};
