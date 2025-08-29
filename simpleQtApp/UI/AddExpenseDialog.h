#ifndef ADDEXPENSEDIALOG_H
#define ADDEXPENSEDIALOG_H

#include <QCheckBox>
#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QDialog>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QScrollArea>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QVariant>

#include "../Managers/PersonManager.h"
#include "../Models/header.h"

using namespace std;

class AddExpenseDialog : public QDialog {
    Q_OBJECT

   public:
    explicit AddExpenseDialog(TRIP &trip, QWidget *parent = nullptr, PERSONMANAGER *personManager = nullptr);
    ~AddExpenseDialog();

    EXPENSE getExpense() const;

   private slots:
    void acceptDialog();
    void refreshPersonList();

    void filterPeopleList();
    void clearSearch();
    void onPersonCheckBoxToggled(bool checked);
    void updateSelectedPersonLabel();

   private:
    void setupUI();
    void connectSignals();
    bool validateInputs();

    QDateEdit *dateEdit;
    QComboBox *categoryComboBox;
    QLineEdit *amountLineEdit;
    QTextEdit *noteTextEdit;
    QComboBox *personComboBox;
    QLabel *selectedPersonLabel;

    QPushButton *addButton;
    QPushButton *cancelButton;
    QPushButton *refreshButton;

    QLineEdit *searchLineEdit;
    QWidget *peopleListWidget;
    QVBoxLayout *peopleListLayout;
    QList<QCheckBox *> personCheckBoxes;
    QPushButton *clearSearchButton;
    vector<MEMBER> allMembers;

    EXPENSE _expenseData;
    PERSONMANAGER *personManager;
    TRIP &trip;
};

#endif