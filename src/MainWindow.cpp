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
    resize(820, 760);
    setupUI();
    loadFromFile(expenseList, currentBudget, nextId);
    activityLog = loadActivityLog();
    selectedDay = QDate::currentDate();
    budgetInput->setValue(currentBudget.limit);
    recalculateTotal(currentBudget, expenseList);
    refreshDashboard();
    refreshHistory();
    refreshAnalysis();
}

MainWindow::~MainWindow() {
    
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

    QGroupBox* addGroup = new QGroupBox("Add / Edit Expense");
    QGridLayout* addLayout = new QGridLayout(addGroup);
    addLayout->addWidget(new QLabel("Category:"), 0, 0);
    categoryDropdown = new QComboBox();
    for (const auto& cat : getCategories()) categoryDropdown->addItem(QString::fromStdString(cat));
    addLayout->addWidget(categoryDropdown, 0, 1);
    addLayout->addWidget(new QLabel("Description:"), 0, 2);
    descriptionInput = new QLineEdit();
    descriptionInput->setPlaceholderText("(optional)");
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

    addOrUpdateButton = new QPushButton("➕ Add Expense");
    connect(addOrUpdateButton, &QPushButton::clicked, this, &MainWindow::handleAddOrUpdateExpense);
    addLayout->addWidget(addOrUpdateButton, 2, 0, 1, 3);

    cancelEditButton = new QPushButton("✖ Cancel Edit");
    cancelEditButton->setVisible(false);
    connect(cancelEditButton, &QPushButton::clicked, this, &MainWindow::resetExpenseForm);
    addLayout->addWidget(cancelEditButton, 2, 3);

    mainLayout->addWidget(addGroup);

    expenseTable = new QTableWidget();
    expenseTable->setColumnCount(5);
    expenseTable->setHorizontalHeaderLabels({"ID", "Date", "Category", "Description", "Amount"});
    expenseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    expenseTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    expenseTable->setSelectionMode(QAbstractItemView::ExtendedSelection); // multi-select for bulk delete
    expenseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(expenseTable);

    QHBoxLayout* actionRow = new QHBoxLayout();
    editButton = new QPushButton("✏️ Edit Selected");
    connect(editButton, &QPushButton::clicked, this, &MainWindow::handleEditExpense);
    actionRow->addWidget(editButton);

    deleteButton = new QPushButton("🗑️ Delete Selected");
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::handleDeleteExpense);
    actionRow->addWidget(deleteButton);
    mainLayout->addLayout(actionRow);

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
    historyTable->setColumnCount(7);
    historyTable->setHorizontalHeaderLabels({"Timestamp", "Action", "ID", "Date", "Category", "Description", "Amount"});
    historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    historyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(historyTable);

    return tab;
}

QWidget* MainWindow::createAnalysisTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);

    analysisCalendar = new QCalendarWidget();
    connect(analysisCalendar, &QCalendarWidget::currentPageChanged, this, &MainWindow::handleCalendarPageChanged);
    connect(analysisCalendar, &QCalendarWidget::clicked, this, &MainWindow::handleCalendarDateClicked);
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

    selectedDayLabel = new QLabel("Click a date above to see that day's expenses.");
    selectedDayLabel->setStyleSheet("font-weight: bold;");
    layout->addWidget(selectedDayLabel);

    dayExpenseTable = new QTableWidget();
    dayExpenseTable->setColumnCount(3);
    dayExpenseTable->setHorizontalHeaderLabels({"Category", "Description", "Amount"});
    dayExpenseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    dayExpenseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(dayExpenseTable);

    return tab;
}

void MainWindow::handleSetBudget() {
    currentBudget.limit = budgetInput->value();
    refreshDashboard();
    saveToFile(expenseList, currentBudget);
}

void MainWindow::handleAddOrUpdateExpense() {
    std::string desc = descriptionInput->text().trimmed().toStdString();
    std::string cat = categoryDropdown->currentText().toStdString();
    double amt = amountInput->value();
    std::string date = dateInput->date().toString("yyyy-MM-dd").toStdString();

    if (editingExpenseId != -1) {
        editExpense(expenseList, editingExpenseId, cat, desc, amt, date);
        for (const auto& item : expenseList) {
            if (item.id == editingExpenseId) {
                logActivity(activityLog, "Edited", item);
                break;
            }
        }
        resetExpenseForm();
    } else {
        addExpense(expenseList, nextId, cat, desc, amt, date);
        logActivity(activityLog, "Added", expenseList.back());
        descriptionInput->clear();
        amountInput->setValue(1.0);
        dateInput->setDate(QDate::currentDate());
    }

    recalculateTotal(currentBudget, expenseList);
    refreshDashboard();
    refreshHistory();
    refreshAnalysis();
    saveToFile(expenseList, currentBudget);
}

void MainWindow::handleEditExpense() {
    int currentRow = expenseTable->currentRow();
    if (currentRow < 0) return;

    int id = expenseTable->item(currentRow, 0)->text().toInt();
    for (const auto& item : expenseList) {
        if (item.id == id) {
            editingExpenseId = id;
            categoryDropdown->setCurrentText(QString::fromStdString(item.category));
            descriptionInput->setText(QString::fromStdString(item.description));
            amountInput->setValue(item.amount);
            dateInput->setDate(QDate::fromString(QString::fromStdString(item.date), "yyyy-MM-dd"));
            addOrUpdateButton->setText("✔️ Update Expense");
            cancelEditButton->setVisible(true);
            break;
        }
    }
}

void MainWindow::resetExpenseForm() {
    editingExpenseId = -1;
    categoryDropdown->setCurrentIndex(0);
    descriptionInput->clear();
    amountInput->setValue(1.0);
    dateInput->setDate(QDate::currentDate());
    addOrUpdateButton->setText("➕ Add Expense");
    cancelEditButton->setVisible(false);
}

void MainWindow::handleDeleteExpense() {
    QModelIndexList selected = expenseTable->selectionModel()->selectedRows();
    if (selected.isEmpty()) return;

    std::vector<int> idsToDelete;
    for (const auto& index : selected) {
        idsToDelete.push_back(expenseTable->item(index.row(), 0)->text().toInt());
    }

    for (int id : idsToDelete) {
        for (const auto& item : expenseList) {
            if (item.id == id) {
                logActivity(activityLog, "Deleted", item);
                break;
            }
        }
        deleteExpense(expenseList, id);
        if (id == editingExpenseId) resetExpenseForm(); // don't leave the form pointing at a deleted expense
    }

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

void MainWindow::handleCalendarDateClicked(const QDate& date) {
    selectedDay = date;
    refreshDayView(date);
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

    std::vector<ActivityLogEntry> filtered;
    for (const auto& entry : activityLog) {
        if (selectedMonth.isEmpty() || QString::fromStdString(entry.date).startsWith(selectedMonth)) {
            filtered.push_back(entry);
        }
    }
    std::reverse(filtered.begin(), filtered.end()); // newest activity first

    historyTable->setRowCount(0);
    for (int i = 0; i < static_cast<int>(filtered.size()); ++i) {
        historyTable->insertRow(i);
        historyTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(filtered[i].timestamp)));
        historyTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(filtered[i].action)));
        historyTable->setItem(i, 2, new QTableWidgetItem(QString::number(filtered[i].expenseId)));
        historyTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(filtered[i].date)));
        historyTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(filtered[i].category)));
        historyTable->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(filtered[i].description)));
        historyTable->setItem(i, 6, new QTableWidgetItem(QString("Rs. %1").arg(filtered[i].amount, 0, 'f', 2)));
    }
}

void MainWindow::refreshDayView(const QDate& date) {
    if (!date.isValid()) return;
    std::string dateKey = date.toString("yyyy-MM-dd").toStdString();

    std::vector<Expense> dayItems;
    for (const auto& item : expenseList) {
        if (item.date == dateKey) dayItems.push_back(item);
    }

    selectedDayLabel->setText(QString("Expenses on %1:").arg(date.toString("MMMM d, yyyy")));

    dayExpenseTable->setRowCount(0);
    for (int i = 0; i < static_cast<int>(dayItems.size()); ++i) {
        dayExpenseTable->insertRow(i);
        dayExpenseTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(dayItems[i].category)));
        dayExpenseTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(dayItems[i].description)));
        dayExpenseTable->setItem(i, 2, new QTableWidgetItem(QString("Rs. %1").arg(dayItems[i].amount, 0, 'f', 2)));
    }
    if (dayItems.empty()) {
        dayExpenseTable->insertRow(0);
        dayExpenseTable->setItem(0, 0, new QTableWidgetItem("No expenses on this date"));
    }
}

void MainWindow::refreshAnalysis() {
    QDate visibleDate(analysisCalendar->yearShown(), analysisCalendar->monthShown(), 1);
    std::string monthKey = visibleDate.toString("yyyy-MM").toStdString();

    
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

    if (selectedDay.year() == visibleDate.year() && selectedDay.month() == visibleDate.month()) {
        refreshDayView(selectedDay);
    } else {
        selectedDayLabel->setText("Click a date above to see that day's expenses.");
        dayExpenseTable->setRowCount(0);
    }
}


#include "moc_MainWindow.cpp"
