#ifndef TRIPDIALOG_H
#define TRIPDIALOG_H

#include <QCheckBox>
#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QVariant>

#include "../Managers/PersonManager.h"
#include "../Managers/TripFactory.h"
#include "../Models/header.h"

class AddTripDialog : public QDialog {
    Q_OBJECT

   public:
    explicit AddTripDialog(QWidget *parent = nullptr);
    explicit AddTripDialog(PERSONMANAGER *personManager, QWidget *parent = nullptr);
    TRIP getTripData() const;

   private slots:
    void acceptDialog();
    void rejectDialog();
    void onHostSelectionChanged();
    void onMemberSelectionChanged();
    void onSelectAllMembers();
    void onClearAllMembers();
    void onAddMemberClicked();
    void onAddHostClicked();

   private:
    void setupUI();
    void setupPeopleSelection();
    void updateMemberList();
    void updateHostList();
    void updateSelectedCounts();
    bool validatePeopleSelection();

    QLineEdit *destinationLineEdit;
    QTextEdit *descriptionTextEdit;
    QDateEdit *startDateEdit;
    QDateEdit *endDateEdit;
    QComboBox *statusComboBox;
    QPushButton *okButton;
    QPushButton *cancelButton;

    QGroupBox *hostGroupBox;
    QGroupBox *membersGroupBox;
    QListWidget *hostsListWidget;
    QLabel *selectedHostLabel;

    QHBoxLayout *hostSearchLayout;
    QTextEdit *hostSearchBar;
    QPushButton *clearHostSearchButton;

    QListWidget *membersListWidget;
    QPushButton *selectAllMembersButton;
    QPushButton *clearAllMembersButton;
    QLabel *selectedMembersLabel;

    QHBoxLayout *memberSearchLayout;
    QTextEdit *memberSearchBar;
    QPushButton *clearMemberSearchButton;

    TRIP _tripData;
    PERSONMANAGER *personManager;
    QSet<QString> selectedMemberIDs;
};

#endif
