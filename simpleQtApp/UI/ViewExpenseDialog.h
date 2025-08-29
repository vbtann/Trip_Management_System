#ifndef VIEWEXPENSEDIALOG_H
#define VIEWEXPENSEDIALOG_H

#include <QApplication>
#include <QDialog>
#include <QFont>
#include <QFormLayout>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QScreen>
#include <QStyle>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

#include "../Managers/FileManager.h"
#include "../Models/header.h"
#include "../UI/AddExpenseDialog.h"

class ViewExpenseDialog : public QDialog {
    Q_OBJECT

   public:
    explicit ViewExpenseDialog(TRIP &trip, PERSONMANAGER *personManager, QWidget *parent = nullptr);

    ~ViewExpenseDialog();

    vector<EXPENSE> getUpdatedExpenses() const;
    TRIP getUpdatedTrip() const;

    bool hasExpensesChanged() const;

   private slots:
    void onDeleteExpense();
    void onRefreshView();
    void onExpenseSelectionChanged();

   private:
    void setupUI();
    void connectSignals();
    void populateExpenseTable();
    void updateTotalAmount();
    void updateButtonStates();

    void removeExpenseAt(int index);
    QString formatCurrency(long long amount) const;
    QString formatDate(const DATE &date) const;
    QString formatCategory(CATEGORY category) const;

    QTableWidget *expenseTable;
    QLabel *tripNameLabel;
    QLabel *totalExpenseLabel;
    QLabel *expenseCountLabel;
    QPushButton *deleteButton;
    QPushButton *refreshButton;
    QPushButton *closeButton;

    TRIP &trip;
    vector<EXPENSE> expenses;
    QString tripName;
    bool expensesChanged;
    long long totalAmount;

    PERSONMANAGER *personManager;
};

#endif