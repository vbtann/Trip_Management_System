#include "ViewExpenseDialog.h"

ViewExpenseDialog::ViewExpenseDialog(TRIP &trip, PERSONMANAGER *personManager, QWidget *parent)
    : QDialog(parent),
      trip(trip),
      expenses(trip.getAllExpenses()),
      tripName(QString::fromStdString(trip.getDestination())),
      expensesChanged(false),
      personManager(personManager),
      totalAmount(0) {
    setWindowTitle(QString("View Expenses - %1").arg(tripName));
    setModal(true);
    setMinimumSize(900, 700);
    resize(1100, 800);

    setupUI();
    connectSignals();
    populateExpenseTable();
    updateTotalAmount();
    updateButtonStates();

    QRect screenGeometry = QApplication::primaryScreen()->geometry();
    move(screenGeometry.center() - rect().center());
}

ViewExpenseDialog::~ViewExpenseDialog() {}

void ViewExpenseDialog::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QGroupBox *headerGroup = new QGroupBox();
    QVBoxLayout *headerLayout = new QVBoxLayout(headerGroup);

    tripNameLabel = new QLabel(tripName);
    QFont titleFont = tripNameLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    tripNameLabel->setFont(titleFont);
    tripNameLabel->setStyleSheet("QLabel { color: #2c3e50; padding: 5px; }");
    headerLayout->addWidget(tripNameLabel);

    QHBoxLayout *summaryLayout = new QHBoxLayout();

    expenseCountLabel = new QLabel("0 expenses");
    expenseCountLabel->setStyleSheet("QLabel { color: #7f8c8d; font-size: 12px; }");

    totalExpenseLabel = new QLabel("Total: 0 VND");
    QFont totalFont = totalExpenseLabel->font();
    totalFont.setPointSize(14);
    totalFont.setBold(true);
    totalExpenseLabel->setFont(totalFont);
    totalExpenseLabel->setStyleSheet("QLabel { color: #27ae60; }");

    summaryLayout->addWidget(expenseCountLabel);
    summaryLayout->addStretch();
    summaryLayout->addWidget(totalExpenseLabel);

    headerLayout->addLayout(summaryLayout);
    mainLayout->addWidget(headerGroup);

    expenseTable = new QTableWidget();
    expenseTable->setColumnCount(5);
    QStringList headers = {"Date", "Category", "Amount (VND)", "Person in Charge", "Note"};
    expenseTable->setHorizontalHeaderLabels(headers);
    expenseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    expenseTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    expenseTable->setSelectionMode(QAbstractItemView::SingleSelection);
    expenseTable->setAlternatingRowColors(true);
    expenseTable->setSortingEnabled(true);
    expenseTable->verticalHeader()->setVisible(false);

    expenseTable->horizontalHeader()->setStretchLastSection(true);
    expenseTable->setColumnWidth(0, 100);
    expenseTable->setColumnWidth(1, 120);
    expenseTable->setColumnWidth(2, 120);
    expenseTable->setColumnWidth(3, 150);

    expenseTable->setStyleSheet(
        "QTableWidget {"
        "    gridline-color: #bdc3c7;"
        "    background-color: white;"
        "    alternate-background-color: #f8f9fa;"
        "    border-radius: 8px;"
        "}"
        "QTableWidget::item {"
        "    padding: 8px;"
        "    border-bottom: 1px solid #ecf0f1;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: #3498db;"
        "    color: white;"
        "}"
        "QHeaderView::section {"
        "    background-color: #27ae60;"
        "    color: white;"
        "    padding: 10px;"
        "    border: none;"
        "    font-weight: bold;"
        "}");

    mainLayout->addWidget(expenseTable);

    QHBoxLayout *buttonLayout = new QHBoxLayout();

    deleteButton = new QPushButton("🗑️ Delete Selection");
    deleteButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #e74c3c;"
        "    color: white;"
        "    font-weight: bold;"
        "    padding: 10px 20px;"
        "    border: none;"
        "    border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #c0392b;"
        "}"
        "QPushButton:disabled {"
        "    background-color: #bdc3c7;"
        "    color: #7f8c8d;"
        "}");
    deleteButton->setEnabled(false);

    refreshButton = new QPushButton("🔄 Refresh");
    refreshButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #3498db;"
        "    color: white;"
        "    font-weight: bold;"
        "    padding: 10px 20px;"
        "    border: none;"
        "    border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #2980b9;"
        "}");

    closeButton = new QPushButton("❌ Close");
    closeButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #95a5a6;"
        "    color: white;"
        "    font-weight: bold;"
        "    padding: 10px 20px;"
        "    border: none;"
        "    border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #7f8c8d;"
        "}");
    closeButton->setDefault(true);

    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(refreshButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);

    mainLayout->addLayout(buttonLayout);
}

void ViewExpenseDialog::connectSignals() {
    connect(deleteButton, &QPushButton::clicked, this, &ViewExpenseDialog::onDeleteExpense);
    connect(refreshButton, &QPushButton::clicked, this, &ViewExpenseDialog::onRefreshView);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(expenseTable, &QTableWidget::itemSelectionChanged, this, &ViewExpenseDialog::onExpenseSelectionChanged);
}

void ViewExpenseDialog::populateExpenseTable() {
    expenseTable->setRowCount(static_cast<int>(expenses.size()));

    for (size_t i = 0; i < expenses.size(); ++i) {
        const EXPENSE &expense = expenses[i];

        QTableWidgetItem *dateItem = new QTableWidgetItem(formatDate(expense.getDate()));
        expenseTable->setItem(static_cast<int>(i), 0, dateItem);

        QTableWidgetItem *categoryItem = new QTableWidgetItem(formatCategory(expense.getCategory()));
        expenseTable->setItem(static_cast<int>(i), 1, categoryItem);

        QTableWidgetItem *amountItem = new QTableWidgetItem(formatCurrency(expense.getAmount()));
        amountItem->setData(Qt::UserRole, static_cast<qulonglong>(expense.getAmount()));
        expenseTable->setItem(static_cast<int>(i), 2, amountItem);

        QTableWidgetItem *personItem = new QTableWidgetItem(QString::fromStdString(expense.getPIC().getFullName()));
        expenseTable->setItem(static_cast<int>(i), 3, personItem);

        QTableWidgetItem *noteItem = new QTableWidgetItem(QString::fromStdString(expense.getNote()));
        noteItem->setToolTip(QString::fromStdString(expense.getNote()));
        expenseTable->setItem(static_cast<int>(i), 4, noteItem);
    }

    expenseTable->resizeRowsToContents();
}

void ViewExpenseDialog::updateTotalAmount() {
    totalAmount = 0;
    for (const EXPENSE &expense : expenses) {
        totalAmount += expense.getAmount();
    }

    totalExpenseLabel->setText(QString("Total: %1").arg(formatCurrency(totalAmount)));
    expenseCountLabel->setText(QString("%1 expense(s)").arg(expenses.size()));
}

void ViewExpenseDialog::updateButtonStates() {
    bool hasSelection = expenseTable->currentRow() >= 0;
    deleteButton->setEnabled(hasSelection && !expenses.empty());
}

void ViewExpenseDialog::onDeleteExpense() {
    int currentRow = expenseTable->currentRow();
    if (currentRow < 0 || currentRow >= static_cast<int>(expenses.size())) {
        return;
    }

    const EXPENSE &expenseToDelete = expenses[currentRow];

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("⚠️ Confirm Deletion");
    msgBox.setIcon(QMessageBox::Question);
    QString confirmMessage = QString(
                                 "Are you sure you want to delete this expense?\n\n"
                                 "Date: %1\n"
                                 "Category: %2\n"
                                 "Amount: %3\n"
                                 "Person: %4")
                                 .arg(formatDate(expenseToDelete.getDate()))
                                 .arg(formatCategory(expenseToDelete.getCategory()))
                                 .arg(formatCurrency(expenseToDelete.getAmount()))
                                 .arg(QString::fromStdString(expenseToDelete.getPIC().getFullName()));

    msgBox.setText(confirmMessage);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    if (msgBox.exec() == QMessageBox::Yes) {
        try {
            long long expenseAmount = expenseToDelete.getAmount();
            string memberID = expenseToDelete.getPIC().getID();
            string tripID = trip.getID();

            string expenseDateStr = expenseToDelete.getDate().toString();
            CATEGORY expenseCategory = expenseToDelete.getCategory();

            removeExpenseAt(currentRow);
            expensesChanged = true;
            trip.setExpenses(expenses);

            vector<MEMBER> tripMembers = trip.getMembers();
            bool memberFound = false;

            for (size_t i = 0; i < tripMembers.size(); ++i) {
                if (tripMembers[i].getID() == memberID) {
                    memberFound = true;
                    tripMembers[i].removeSpending(tripID, expenseToDelete);
                    personManager->updateMember(tripMembers[i], tripMembers[i]);
                    break;
                }
            }

            if (!memberFound) {
                qDebug() << "Warning: Member with ID" << QString::fromStdString(memberID)
                         << "not found in trip members.";
            }

            trip.setMembers(tripMembers);

            if (personManager) {
                updatePeopleCacheFile(personManager->getAllMembers(), personManager->getAllHosts());
            }

            populateExpenseTable();
            updateTotalAmount();
            updateButtonStates();

            QMessageBox::information(this, "Success", "Expense deleted successfully.");
        } catch (const exception &e) {
            QMessageBox::critical(this, "Error", QString("Failed to delete expense: %1").arg(e.what()));

            expenses = trip.getAllExpenses();
            populateExpenseTable();
            updateTotalAmount();
            updateButtonStates();
        }
    }
}

void ViewExpenseDialog::onRefreshView() {
    populateExpenseTable();
    updateTotalAmount();

    expenseTable->clearSelection();
    expenseTable->setCurrentItem(nullptr);
    updateButtonStates();
}

void ViewExpenseDialog::onExpenseSelectionChanged() { updateButtonStates(); }

void ViewExpenseDialog::removeExpenseAt(int index) {
    if (index >= 0 && index < static_cast<int>(expenses.size())) {
        expenses.erase(expenses.begin() + index);
    }
}

QString ViewExpenseDialog::formatCurrency(long long amount) const {
    QString amountStr = QString::number(amount);
    QString formatted;
    int count = 0;

    for (int i = amountStr.length() - 1; i >= 0; --i) {
        if (count > 0 && count % 3 == 0) {
            formatted.prepend('.');
        }
        formatted.prepend(amountStr[i]);
        count++;
    }

    return formatted + " VND";
}

QString ViewExpenseDialog::formatDate(const DATE &date) const {
    return QString("%1/%2/%3")
        .arg(date.getDay(), 2, 10, QChar('0'))
        .arg(date.getMonth(), 2, 10, QChar('0'))
        .arg(date.getYear());
}

QString ViewExpenseDialog::formatCategory(CATEGORY category) const {
    return QString::fromStdString(categoryToString(category));
}

vector<EXPENSE> ViewExpenseDialog::getUpdatedExpenses() const { return expenses; }

TRIP ViewExpenseDialog::getUpdatedTrip() const { return trip; }

bool ViewExpenseDialog::hasExpensesChanged() const { return expensesChanged; }