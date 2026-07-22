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

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleAddExpense();
    void handleDeleteExpense();
    void handleSetBudget();
    void handleDeleteHistoryExpense();
    void handleHistoryFilterChanged(int index);
    void handleCalendarPageChanged(int year, int month);

private:
    void setupUI();
    QWidget* createDashboardTab();
    QWidget* createHistoryTab();
    QWidget* createAnalysisTab();

    void refreshDashboard();
    void refreshHistory();
    void refreshAnalysis();
    void populateHistoryMonthDropdown();

    std::vector<Expense> expenseList;
    Budget currentBudget;
    int nextId = 1;

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

    QTableWidget* expenseTable;
    QPushButton* deleteButton;

    // --- History tab ---
    QComboBox* historyMonthFilter;
    QTableWidget* historyTable;
    QLabel* historyTotalLabel;
    QPushButton* historyDeleteButton;

    // --- Monthly Analysis tab ---
    QCalendarWidget* analysisCalendar;
    QLabel* analysisTotalLabel;
    QTableWidget* categoryBreakdownTable;
};