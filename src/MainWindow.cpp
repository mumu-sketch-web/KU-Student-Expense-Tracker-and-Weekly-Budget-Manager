#include "MainWindow.h"
#include "Category.h"
#include "AlertSystem.h"
#include "FileManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QTextCharFormat>
#include <QDate>
#include <QColor>
#include <QFont>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("KU Student Expense Tracker");
    resize(820, 680);
    setupUI();
    loadFromFile(expenseList, currentBudget, nextId);
    budgetInput->setValue(currentBudget.limit);
    recalculateTotal(currentBudget, expenseList);
    refreshDashboard();
    refreshHistory();
    refreshAnalysis();
}

MainWindow::~MainWindow() {
    // Keeps the compiler happy when closing the window
}

void MainWindow::setupUI() {
    tabWidget = new QTabWidget(this);
    tabWidget->addTab(createDashboardTab(), "Dashboard");
    tabWidget->addTab(createHistoryTab(), "History");
    tabWidget->addTab(createAnalysisTab(), "Monthly Analysis");
    setCentralWidget(tabWidget);
}

QWidget* MainWindow::createDashboardTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(tab);

    QGroupBox* budgetGroup = new QGroupBox("Weekly Budget Tracker");
    QVBoxLayout* budgetLayout = new QVBoxLayout(budgetGroup);
    QHBoxLayout* limitLayout = new QHBoxLayout();
    limitLayout->addWidget(new QLabel("Set Weekly Limit (NPR):"));
    budgetInput = new QDoubleSpinBox();
    budgetInput->setRange(0, 1000000);
    budgetInput->setPrefix("Rs. ");
    limitLayout->addWidget(budgetInput);
    QPushButton* setBudgetBtn = new QPushButton("Update Limit");
    connect(setBudgetBtn, &QPushButton::clicked, this, &MainWindow::handleSetBudget);
    limitLayout->addWidget(setBudgetBtn);
    budgetLayout->addLayout(limitLayout);

    totalSpentLabel = new QLabel("Total Spent: Rs. 0.00");
    totalSpentLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    budgetLayout->addWidget(totalSpentLabel);
    budgetProgressBar = new QProgressBar();
    budgetLayout->addWidget(budgetProgressBar);
    alertStatusLabel = new QLabel("Status: Healthy");
    alertStatusLabel->setStyleSheet("font-weight: bold;");
    budgetLayout->addWidget(alertStatusLabel);
    mainLayout->addWidget(budgetGroup);

    QGroupBox* addGroup = new QGroupBox("Add New Expense");
    QGridLayout* addLayout = new QGridLayout(addGroup);
    addLayout->addWidget(new QLabel("Category:"), 0, 0);
    categoryDropdown = new QComboBox();
    for (const auto& cat : getCategories()) categoryDropdown->addItem(QString::fromStdString(cat));
    addLayout->addWidget(categoryDropdown, 0, 1);
    addLayout->addWidget(new QLabel("Description:"), 0, 2);
    descriptionInput = new QLineEdit();
    addLayout->addWidget(descriptionInput, 0, 3);

    addLayout->addWidget(new QLabel("Amount:"), 1, 0);
    amountInput = new QDoubleSpinBox();
    amountInput->setRange(1, 100000);
    amountInput->setPrefix("Rs. ");
    addLayout->addWidget(amountInput, 1, 1);

    addLayout->addWidget(new QLabel("Date:"), 1, 2);
    dateInput = new QDateEdit(QDate::currentDate());
    dateInput->setCalendarPopup(true);
    dateInput->setDisplayFormat("yyyy-MM-dd");
    dateInput->setMaximumDate(QDate::currentDate()); // expenses can't be logged in the future
    addLayout->addWidget(dateInput, 1, 3);

    QPushButton* addBtn = new QPushButton("➕ Add Expense");
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::handleAddExpense);
    addLayout->addWidget(addBtn, 2, 0, 1, 4);
    mainLayout->addWidget(addGroup);

    expenseTable = new QTableWidget();
    expenseTable->setColumnCount(5);
    expenseTable->setHorizontalHeaderLabels({"ID", "Date", "Category", "Description", "Amount"});
    expenseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    expenseTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    expenseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(expenseTable);

    deleteButton = new QPushButton("🗑️ Delete Selected Expense");
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::handleDeleteExpense);
    mainLayout->addWidget(deleteButton);

    return tab;
}

QWidget* MainWindow::createHistoryTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);

    QHBoxLayout* filterLayout = new QHBoxLayout();
    filterLayout->addWidget(new QLabel("Filter by Month:"));
    historyMonthFilter = new QComboBox();
    connect(historyMonthFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::handleHistoryFilterChanged);
    filterLayout->addWidget(historyMonthFilter);
    filterLayout->addStretch();
    layout->addLayout(filterLayout);

    historyTable = new QTableWidget();
    historyTable->setColumnCount(5);
    historyTable->setHorizontalHeaderLabels({"ID", "Date", "Category", "Description", "Amount"});
    historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    historyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    historyTable->setSortingEnabled(true);
    layout->addWidget(historyTable);

    historyTotalLabel = new QLabel("Total: Rs. 0.00");
    historyTotalLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    layout->addWidget(historyTotalLabel);

    historyDeleteButton = new QPushButton("🗑️ Delete Selected");
    connect(historyDeleteButton, &QPushButton::clicked, this, &MainWindow::handleDeleteHistoryExpense);
    layout->addWidget(historyDeleteButton);

    return tab;
}

QWidget* MainWindow::createAnalysisTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);

    analysisCalendar = new QCalendarWidget();
    connect(analysisCalendar, &QCalendarWidget::currentPageChanged, this, &MainWindow::handleCalendarPageChanged);
    layout->addWidget(analysisCalendar);

    analysisTotalLabel = new QLabel("Total Spent This Month: Rs. 0.00");
    analysisTotalLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
    layout->addWidget(analysisTotalLabel);

    layout->addWidget(new QLabel("Spending by Category:"));
    categoryBreakdownTable = new QTableWidget();
    categoryBreakdownTable->setColumnCount(2);
    categoryBreakdownTable->setHorizontalHeaderLabels({"Category", "Amount"});
    categoryBreakdownTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    categoryBreakdownTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(categoryBreakdownTable);

    return tab;
}

void MainWindow::handleSetBudget() {
    currentBudget.limit = budgetInput->value();
    refreshDashboard();
    saveToFile(expenseList, currentBudget);
}

void MainWindow::handleAddExpense() {
    if (descriptionInput->text().trimmed().isEmpty()) return;
    addExpense(expenseList, nextId,
               categoryDropdown->currentText().toStdString(),
               descriptionInput->text().toStdString(),
               amountInput->value(),
               dateInput->date().toString("yyyy-MM-dd").toStdString());
    descriptionInput->clear();
    amountInput->setValue(1.0);
    dateInput->setDate(QDate::currentDate());
    recalculateTotal(currentBudget, expenseList);
    refreshDashboard();
    refreshHistory();
    refreshAnalysis();
    saveToFile(expenseList, currentBudget);
}

void MainWindow::handleDeleteExpense() {
    int currentRow = expenseTable->currentRow();
    if (currentRow < 0) return;
    deleteExpense(expenseList, expenseTable->item(currentRow, 0)->text().toInt());
    recalculateTotal(currentBudget, expenseList);
    refreshDashboard();
    refreshHistory();
    refreshAnalysis();
    saveToFile(expenseList, currentBudget);
}

void MainWindow::handleDeleteHistoryExpense() {
    int currentRow = historyTable->currentRow();
    if (currentRow < 0) return;
    deleteExpense(expenseList, historyTable->item(currentRow, 0)->text().toInt());
    recalculateTotal(currentBudget, expenseList);
    refreshDashboard();
    refreshHistory();
    refreshAnalysis();
    saveToFile(expenseList, currentBudget);
}

void MainWindow::handleHistoryFilterChanged(int index) {
    Q_UNUSED(index);
    refreshHistory();
}

void MainWindow::handleCalendarPageChanged(int year, int month) {
    Q_UNUSED(year);
    Q_UNUSED(month);
    refreshAnalysis();
}

void MainWindow::refreshDashboard() {
    totalSpentLabel->setText(QString("Total Spent: Rs. %1").arg(currentBudget.totalSpent, 0, 'f', 2));
    int percentage = currentBudget.limit > 0 ? static_cast<int>((currentBudget.totalSpent / currentBudget.limit) * 100) : 0;
    budgetProgressBar->setValue(std::min(percentage, 100));
    alertStatusLabel->setText(QString::fromStdString(checkBudgetAlert(currentBudget)));

    // Show most recently dated expenses first
    std::vector<Expense> sorted = expenseList;
    std::sort(sorted.begin(), sorted.end(), [](const Expense& a, const Expense& b) {
        return a.date > b.date;
    });

    expenseTable->setRowCount(0);
    for (int i = 0; i < static_cast<int>(sorted.size()); ++i) {
        expenseTable->insertRow(i);
        expenseTable->setItem(i, 0, new QTableWidgetItem(QString::number(sorted[i].id)));
        expenseTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(sorted[i].date)));
        expenseTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(sorted[i].category)));
        expenseTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(sorted[i].description)));
        expenseTable->setItem(i, 4, new QTableWidgetItem(QString("Rs. %1").arg(sorted[i].amount, 0, 'f', 2)));
    }
}

void MainWindow::populateHistoryMonthDropdown() {
    QString previousSelection = historyMonthFilter->currentData().toString();

    historyMonthFilter->blockSignals(true);
    historyMonthFilter->clear();
    historyMonthFilter->addItem("All Time", "");

    for (const auto& monthKey : getAvailableMonths(expenseList)) {
        QDate d = QDate::fromString(QString::fromStdString(monthKey + "-01"), "yyyy-MM-dd");
        QString label = d.isValid() ? d.toString("MMMM yyyy") : QString::fromStdString(monthKey);
        historyMonthFilter->addItem(label, QString::fromStdString(monthKey));
    }

    int idx = historyMonthFilter->findData(previousSelection);
    historyMonthFilter->setCurrentIndex(idx >= 0 ? idx : 0);
    historyMonthFilter->blockSignals(false);
}

void MainWindow::refreshHistory() {
    populateHistoryMonthDropdown();
    QString selectedMonth = historyMonthFilter->currentData().toString();

    std::vector<Expense> filtered;
    for (const auto& item : expenseList) {
        if (selectedMonth.isEmpty() || QString::fromStdString(item.date).startsWith(selectedMonth)) {
            filtered.push_back(item);
        }
    }
    std::sort(filtered.begin(), filtered.end(), [](const Expense& a, const Expense& b) {
        return a.date > b.date;
    });

    historyTable->setSortingEnabled(false);
    historyTable->setRowCount(0);
    double total = 0.0;
    for (int i = 0; i < static_cast<int>(filtered.size()); ++i) {
        historyTable->insertRow(i);
        historyTable->setItem(i, 0, new QTableWidgetItem(QString::number(filtered[i].id)));
        historyTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(filtered[i].date)));
        historyTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(filtered[i].category)));
        historyTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(filtered[i].description)));
        historyTable->setItem(i, 4, new QTableWidgetItem(QString("Rs. %1").arg(filtered[i].amount, 0, 'f', 2)));
        total += filtered[i].amount;
    }
    historyTable->setSortingEnabled(true);
    historyTotalLabel->setText(QString("Total: Rs. %1").arg(total, 0, 'f', 2));
}

void MainWindow::refreshAnalysis() {
    QDate visibleDate(analysisCalendar->yearShown(), analysisCalendar->monthShown(), 1);
    std::string monthKey = visibleDate.toString("yyyy-MM").toStdString();

    // Clear old highlighting, then highlight every day in the visible month that has an expense
    QTextCharFormat defaultFormat;
    analysisCalendar->setDateTextFormat(QDate(), defaultFormat);

    QTextCharFormat highlightFormat;
    highlightFormat.setBackground(QColor(255, 214, 153));
    highlightFormat.setFontWeight(QFont::Bold);

    for (const auto& item : expenseList) {
        QDate d = QDate::fromString(QString::fromStdString(item.date), "yyyy-MM-dd");
        if (d.isValid() && d.year() == visibleDate.year() && d.month() == visibleDate.month()) {
            analysisCalendar->setDateTextFormat(d, highlightFormat);
        }
    }

    double total = getMonthlyTotal(expenseList, monthKey);
    analysisTotalLabel->setText(QString("Total Spent in %1: Rs. %2")
        .arg(visibleDate.toString("MMMM yyyy"))
        .arg(total, 0, 'f', 2));

    auto breakdown = getCategoryBreakdown(expenseList, monthKey);
    categoryBreakdownTable->setRowCount(0);
    int row = 0;
    for (const auto& entry : breakdown) {
        categoryBreakdownTable->insertRow(row);
        categoryBreakdownTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(entry.first)));
        categoryBreakdownTable->setItem(row, 1, new QTableWidgetItem(QString("Rs. %1").arg(entry.second, 0, 'f', 2)));
        row++;
    }
    if (breakdown.empty()) {
        categoryBreakdownTable->insertRow(0);
        categoryBreakdownTable->setItem(0, 0, new QTableWidgetItem("No expenses this month"));
        categoryBreakdownTable->setItem(0, 1, new QTableWidgetItem(""));
    }
}

// Crucial fix: explicitly compile the meta-object code right here
#include "moc_MainWindow.cpp"
