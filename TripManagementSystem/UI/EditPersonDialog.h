#ifndef EDITPERSONDIALOG_H
#define EDITPERSONDIALOG_H

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
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

#include "../Managers/PersonFactory.h"
#include "../Managers/PersonManager.h"
#include "../Models/header.h"

using namespace std;

class EditPersonDialog : public QDialog {
    Q_OBJECT

   public:
    explicit EditPersonDialog(PERSON *person, QWidget *parent = nullptr);
    explicit EditPersonDialog(PERSON *person, PERSONMANAGER *personManager, QWidget *parent = nullptr);
    PERSON *getUpdatedPerson() const;
    PERSON *getOriginalPerson() const;

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
    void populateFields();
    bool validateInput();
    bool validateNoDuplicates();

    QVBoxLayout *mainLayout;
    QFormLayout *formLayout;
    QHBoxLayout *buttonLayout;
    QLabel *titleLabel;

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

    PERSON *originalPerson;
    PERSONMANAGER *personManager;
    mutable PERSON *updatedPerson;
};

#endif