#ifndef VIEWTRIPDIALOG_H
#define VIEWTRIPDIALOG_H

#include <QApplication>
#include <QDialog>
#include <QFont>
#include <QFormLayout>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QScreen>
#include <QScrollArea>
#include <QSpacerItem>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QVariant>
#include <vector>

#include "../Managers/PersonManager.h"
#include "../Models/header.h"
#include "EditTripDialog.h"
#include "ViewExpenseDialog.h"

using namespace std;

class ViewTripDialog : public QDialog {
    Q_OBJECT

   public:
    explicit ViewTripDialog(TRIP &_trip, PERSONMANAGER *personManager, QWidget *parent = nullptr);
    void setPersonManager(PERSONMANAGER *personManager);
    TRIP getUpdatedTrip() const;
    TRIP getOriginalTrip() const;

   private slots:
    void editCurrentTrip();
    void closeView();
    void onHostInfoClicked();
    void onMemberInfoClicked();
    void onViewExpensesClicked();
    void onAddExpenseClicked();

   private:
    void setupUI();
    void displayCurrentTrip();
    void displayPeopleInfo();
    void setupTripInfoSection();
    void setupPeopleSection();
    void setupExpenseSection();
    void updateExpenseDisplay();

    QString formatCurrency(long long amount) const;

    TRIP &trip;
    TRIP originalTrip;

    QLabel *idLabel;
    QLabel *destinationLabel;
    QTextEdit *descriptionDisplay;
    QLabel *startDateLabel;
    QLabel *endDateLabel;
    QLabel *statusLabel;

    QGroupBox *expenseInfoGroupBox;
    QLabel *totalExpenseLabel;
    QPushButton *viewExpensesButton;
    QPushButton *addExpenseButton;
    QListWidget *expenseListWidget;
    QVBoxLayout *expenseListLayout;

    QGroupBox *tripInfoGroupBox;
    QGroupBox *peopleInfoGroupBox;

    QLabel *hostLabel;
    QLabel *hostNameLabel;
    QLabel *hostEmailLabel;
    QLabel *hostPhoneLabel;
    QPushButton *hostInfoButton;

    QLabel *membersCountLabel;
    QListWidget *membersListWidget;
    QPushButton *membersInfoButton;

    QScrollArea *scrollArea;

    QPushButton *editButton;
    QPushButton *closeButton;

    PERSONMANAGER *personManager;
};

#endif