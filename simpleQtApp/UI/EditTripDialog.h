#ifndef EDITTRIPDIALOG_H
#define EDITTRIPDIALOG_H

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
#include <QTextEdit>
#include <QVBoxLayout>
#include <QVariant>

#include "../Managers/PersonManager.h"
#include "../Managers/TripFactory.h"
#include "../Models/header.h"
#include "../UI/AddExpenseDialog.h"

class EditTripDialog : public QDialog {
    Q_OBJECT

   public:
    explicit EditTripDialog(TRIP &trip, QWidget *parent = nullptr);

    TRIP getUpdatedTrip() const;
    TRIP getOriginalTrip() const;

    void setPersonManager(PERSONMANAGER *manager);

   private slots:
    void acceptChanges();
    void rejectChanges();
    void onDestinationChanged();
    void onStartDateChanged();
    void onHostSelectionChanged();
    void onMembersSelectionChanged();
    void onSelectAllMembers();
    void onClearAllMembers();

   private:
    void setupUI();
    void setupPeopleSelection();
    void setupPeopleSelectionWidget();
    void populateFields();
    void populatePeopleSelection();
    void styleComponents();
    void updateMembersList();
    void updateSelectedCount();
    bool validatePeopleSelection();

    TRIP originalTrip;
    TRIP editedTrip;

    PERSONMANAGER *personManager;

    QLineEdit *tripIDLineEdit;
    QLineEdit *destinationLineEdit;
    QTextEdit *descriptionTextEdit;
    QDateEdit *startDateEdit;
    QDateEdit *endDateEdit;
    QComboBox *statusComboBox;

    QGroupBox *hostGroupBox;
    QComboBox *hostComboBox;
    QLabel *selectedHostLabel;

    QGroupBox *membersGroupBox;
    QListWidget *membersListWidget;
    QLabel *selectedMembersLabel;
    QPushButton *selectAllMembersButton;
    QPushButton *clearAllMembersButton;

    QPushButton *saveButton;
    QPushButton *cancelButton;
    QPushButton *resetButton;
};

#endif