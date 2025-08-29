#ifndef ADDPERSONDIALOG_H
#define ADDPERSONDIALOG_H

#include <QComboBox>
#include <QDateEdit>
#include <QDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QTextEdit>
#include <QVBoxLayout>

#include "../Managers/PersonFactory.h"
#include "../Managers/PersonManager.h"
#include "../Models/header.h"

using namespace std;

class AddPersonDialog : public QDialog {
    Q_OBJECT

   public:
    explicit AddPersonDialog(QWidget *parent = nullptr);
    explicit AddPersonDialog(PERSONMANAGER *personManager, QWidget *parent = nullptr);
    PERSON *getPersonData() const;
    void setPersonType(const string &_type);

   private slots:
    void onPersonTypeChanged();
    void onSaveClicked();
    void onCancelClicked();
    void onFullNameChanged();
    void onDateOfBirthChanged();

   private:
    void setupUI();
    void setupPersonTypeSelection();
    void setupBasicInfoForm();
    void setupRoleSpecificForm();
    void styleComponents();
    void updateFormVisibility();
    bool validateInput();
    bool validateNoDuplicate();

    QComboBox *personTypeCombo;
    QLineEdit *idLineEdit;
    QLineEdit *fullNameLineEdit;
    QComboBox *genderCombo;
    QDateEdit *dobDateEdit;
    QLineEdit *emailLineEdit;
    QLineEdit *phoneLineEdit;
    QLineEdit *addressLineEdit;
    QLineEdit *emergencyContactLineEdit;

    QGroupBox *memberGroupBox;
    QTextEdit *interestsTextEdit;
    QLineEdit *totalSpentLineEdit;

    QGroupBox *hostGroupBox;

    QPushButton *saveButton;
    QPushButton *cancelButton;

    PERSONMANAGER *personManager;
};

#endif
