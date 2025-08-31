#include "AddPersonDialog.h"

#include <QGridLayout>
#include <QMessageBox>

AddPersonDialog::AddPersonDialog(QWidget *parent) : QDialog(parent) {
    setupUI();
    setWindowTitle("Add New Person");

    setModal(true);
    setMinimumSize(450, 400);
}

AddPersonDialog::AddPersonDialog(PERSONMANAGER *personManager, QWidget *parent)
    : QDialog(parent), personManager(personManager) {
    setupUI();
    setWindowTitle("Add New Person");
    setModal(true);
    setMinimumSize(450, 400);
}

void AddPersonDialog::setupUI() {
    setupPersonTypeSelection();
    setupBasicInfoForm();
    setupRoleSpecificForm();
    styleComponents();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Add New Person", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "QLabel { font-size: 18px; font-weight: bold; "
        "color: #2c3e50; margin: 10px; }");

    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("Role:", personTypeCombo);
    formLayout->addRow("ID:", idLineEdit);
    formLayout->addRow("Full Name:", fullNameLineEdit);
    formLayout->addRow("Gender:", genderCombo);
    formLayout->addRow("Date of Birth:", dobDateEdit);
    formLayout->addRow("Email:", emailLineEdit);
    formLayout->addRow("Phone:", phoneLineEdit);
    formLayout->addRow("Address:", addressLineEdit);
    formLayout->addRow("Emergency Contact:", emergencyContactLineEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    saveButton = new QPushButton("Save", this);
    cancelButton = new QPushButton("Cancel", this);
    buttonLayout->addStretch();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);

    QString buttonStyle =
        "QPushButton { "
        "background-color: #3498db; "
        "color: white; "
        "border: none; "
        "padding: 8px 16px; "
        "border-radius: 4px; "
        "font-size: 13px; "
        "font-weight: bold; "
        "}"
        "QPushButton:hover { "
        "background-color: #2980b9; "
        "}"
        "QPushButton:pressed { "
        "background-color: #21618c; "
        "}";

    QString cancelButtonStyle =
        "QPushButton { "
        "background-color: #e74c3c; "
        "color: white; "
        "border: none; "
        "padding: 8px 16px; "
        "border-radius: 4px; "
        "font-size: 13px; "
        "font-weight: bold; "
        "}"
        "QPushButton:hover { "
        "background-color: #c0392b; "
        "}"
        "QPushButton:pressed { "
        "background-color: #a93226; "
        "}";

    saveButton->setStyleSheet(buttonStyle);
    cancelButton->setStyleSheet(cancelButtonStyle);

    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(memberGroupBox);
    mainLayout->addWidget(hostGroupBox);
    mainLayout->addLayout(buttonLayout);

    connect(personTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &AddPersonDialog::onPersonTypeChanged);
    connect(saveButton, &QPushButton::clicked, this, &AddPersonDialog::onSaveClicked);
    connect(cancelButton, &QPushButton::clicked, this, &AddPersonDialog::onCancelClicked);

    connect(fullNameLineEdit, &QLineEdit::textChanged, this, &AddPersonDialog::onFullNameChanged);
    connect(dobDateEdit, &QDateEdit::dateChanged, this, &AddPersonDialog::onDateOfBirthChanged);

    updateFormVisibility();
}

void AddPersonDialog::setupPersonTypeSelection() {
    personTypeCombo = new QComboBox(this);
    personTypeCombo->addItem("Member");
    personTypeCombo->addItem("Host");
}

void AddPersonDialog::setupBasicInfoForm() {
    idLineEdit = new QLineEdit(this);
    idLineEdit->setReadOnly(true);

    fullNameLineEdit = new QLineEdit(this);

    genderCombo = new QComboBox(this);
    genderCombo->addItem("Male");
    genderCombo->addItem("Female");

    dobDateEdit = new QDateEdit(QDate::currentDate().addYears(-18), this);
    dobDateEdit->setCalendarPopup(true);
    dobDateEdit->setMaximumDate(QDate::currentDate());

    emailLineEdit = new QLineEdit(this);
    phoneLineEdit = new QLineEdit(this);
    addressLineEdit = new QLineEdit(this);
    emergencyContactLineEdit = new QLineEdit(this);
}

void AddPersonDialog::setupRoleSpecificForm() {
    memberGroupBox = new QGroupBox("Member Information", this);
    QFormLayout *memberLayout = new QFormLayout(memberGroupBox);

    interestsTextEdit = new QTextEdit(this);
    interestsTextEdit->setMaximumHeight(80);
    interestsTextEdit->setPlaceholderText("Enter interests separated by commas...");

    totalSpentLineEdit = new QLineEdit(this);
    totalSpentLineEdit->setText("0.0");
    totalSpentLineEdit->setPlaceholderText("0.0");

    memberLayout->addRow("Interests:", interestsTextEdit);
    memberLayout->addRow("Total Spent:", totalSpentLineEdit);

    hostGroupBox = new QGroupBox("Host Information", this);
    QFormLayout *hostLayout = new QFormLayout(hostGroupBox);
    hostLayout->addRow(new QLabel("Additional host information can be added here."));
}

void AddPersonDialog::styleComponents() {
    QString lineEditStyle =
        "QLineEdit { "
        "border: 2px solid #bdc3c7; "
        "border-radius: 5px; "
        "padding: 5px; "
        "font-size: 13px; "
        "}"
        "QLineEdit:focus { "
        "border-color: #3498db; "
        "}";

    QString comboBoxStyle =
        "QComboBox { "
        "border: 2px solid #bdc3c7; "
        "border-radius: 5px; "
        "padding: 5px; "
        "font-size: 13px; "
        "}"
        "QComboBox:focus { "
        "border-color: #3498db; "
        "}";

    idLineEdit->setStyleSheet(lineEditStyle);
    fullNameLineEdit->setStyleSheet(lineEditStyle);
    emailLineEdit->setStyleSheet(lineEditStyle);
    phoneLineEdit->setStyleSheet(lineEditStyle);
    addressLineEdit->setStyleSheet(lineEditStyle);
    emergencyContactLineEdit->setStyleSheet(lineEditStyle);
    totalSpentLineEdit->setStyleSheet(lineEditStyle);

    personTypeCombo->setStyleSheet(comboBoxStyle);
    genderCombo->setStyleSheet(comboBoxStyle);
}
void AddPersonDialog::onPersonTypeChanged() { updateFormVisibility(); }

void AddPersonDialog::updateFormVisibility() {
    bool isMember = (personTypeCombo->currentText() == "Member");
    memberGroupBox->setVisible(isMember);
    hostGroupBox->setVisible(!isMember);
}

bool AddPersonDialog::validateInput() {
    if (fullNameLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Full name cannot be empty!");
        fullNameLineEdit->setFocus();
        return false;
    }

    if (personTypeCombo->currentText() == "Member") {
        bool ok;
        totalSpentLineEdit->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Validation Error", "Total spent must be a valid number!");
            totalSpentLineEdit->setFocus();
            return false;
        }
    }

    if (phoneLineEdit->text().trimmed().toStdString().length() != 10) {
        QMessageBox::warning(this, "Validation Error", "Phone number must be exactly 10 digits!");
        phoneLineEdit->setFocus();
        return false;
    }

    string email = emailLineEdit->text().trimmed().toStdString();
    if (email.find("@") == string::npos || email.find(".") == string::npos) {
        QMessageBox::warning(this, "Validation Error", "Email must be a valid Gmail address!");
        emailLineEdit->setFocus();
        return false;
    } else if (emailLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Email cannot be empty!");
        emailLineEdit->setFocus();
        return false;
    }

    QDate dob = dobDateEdit->date();
    if (dob > QDate::currentDate()) {
        QMessageBox::warning(this, "Validation Error", "Date of birth cannot be in the future!");
        dobDateEdit->setFocus();
        return false;
    } else if (dob.year() < 1925) {
        QMessageBox::warning(this, "Validation Error", "Person cannot be born before 1925!");
        dobDateEdit->setFocus();
        return false;
    } else if (dob.year() > QDate::currentDate().year() - 18) {
        QMessageBox::warning(this, "Validation Error", "Person must be at least 18 years old!");
        dobDateEdit->setFocus();
        return false;
    }

    if (!validateNoDuplicate()) {
        return false;
    }

    return true;
}

bool AddPersonDialog::validateNoDuplicate() {
    if (!personManager) {
        return true;
    }

    string currentFullName = fullNameLineEdit->text().trimmed().toUpper().toStdString();
    string currentRole = personTypeCombo->currentText().toStdString();
    string currentEmail = emailLineEdit->text().trimmed().toStdString();
    string currentPhoneNumber = phoneLineEdit->text().trimmed().toStdString();
    QDate dob = dobDateEdit->date();
    DATE currentDateOfBirth(dob.day(), dob.month(), dob.year());

    vector<PERSON> people = personManager->getAllPeople();

    for (const PERSON &person : people) {
        bool samePhoneNumber = (person.getPhoneNumber() == currentPhoneNumber);
        bool sameEmail = (person.getEmail() == currentEmail);

        if (samePhoneNumber && !currentPhoneNumber.empty()) {
            QMessageBox::warning(this, "Duplicate phone number", "A person with this phone number already existed!");
            phoneLineEdit->setFocus();
            return false;
        }
        if (sameEmail && !currentEmail.empty()) {
            QMessageBox::warning(this, "Duplicate email", "A person with this email already existed!");
            emailLineEdit->setFocus();
            return false;
        }
    }
    return true;
}

void AddPersonDialog::onSaveClicked() {
    if (validateInput()) {
        accept();
    }
}

void AddPersonDialog::onCancelClicked() {
    int ret = QMessageBox::question(this, "Cancel", "Are you sure you want to cancel? All data will be lost.",
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        reject();
    }
}

void AddPersonDialog::onFullNameChanged() {
    QString text = fullNameLineEdit->text().toUpper();
    if (!text.isEmpty()) {
        QDate dob = dobDateEdit->date();
        DATE dobObj(dob.day(), dob.month(), dob.year());

        string newID = PERSONFACTORY::generatePersonID(text.toStdString(), dobObj);

        idLineEdit->setText(QString::fromStdString(newID));
    }
}

void AddPersonDialog::onDateOfBirthChanged() {
    QString fullName = fullNameLineEdit->text().trimmed();
    if (!fullName.isEmpty()) {
        QDate dob = dobDateEdit->date();
        DATE dobObj(dob.day(), dob.month(), dob.year());

        string newID = PERSONFACTORY::generatePersonID(fullName.toStdString(), dobObj);

        idLineEdit->setText(QString::fromStdString(newID));
    }
}

PERSON *AddPersonDialog::getPersonData() const {
    string id = idLineEdit->text().trimmed().toStdString();
    string fullName = fullNameLineEdit->text().trimmed().toStdString();
    fullName = toUpper(fullName);

    GENDER gender = (genderCombo->currentText() == "Male") ? GENDER::Male : GENDER::Female;

    QDate dobDate = dobDateEdit->date();
    DATE dob(dobDate.day(), dobDate.month(), dobDate.year());

    if (personTypeCombo->currentText() == "Member") {
        MEMBER *member = new MEMBER(id, fullName, gender, dob);

        member->setEmergencyContact(emergencyContactLineEdit->text().trimmed().toStdString());
        member->setEmail(emailLineEdit->text().trimmed().toStdString());
        member->setPhoneNumber(phoneLineEdit->text().trimmed().toStdString());
        member->setAddress(addressLineEdit->text().trimmed().toStdString());

        QString interestsText = interestsTextEdit->toPlainText().trimmed();
        QStringList interestsList = interestsText.split(",", Qt::SkipEmptyParts);
        for (const QString &interest : interestsList) {
            member->addInterest(interest.trimmed().toStdString());
        }

        double totalSpent = totalSpentLineEdit->text().toDouble();
        member->addToTotalSpent(totalSpent);

        return member;
    } else {
        HOST *host = new HOST(id, fullName, gender, dob);

        host->setEmergencyContact(emergencyContactLineEdit->text().trimmed().toStdString());
        host->setEmail(emailLineEdit->text().trimmed().toStdString());
        host->setPhoneNumber(phoneLineEdit->text().trimmed().toStdString());
        host->setAddress(addressLineEdit->text().trimmed().toStdString());

        return host;
    }
}

void AddPersonDialog::setPersonType(const string &_type) {
    if (_type == "Host") {
        this->personTypeCombo->setCurrentIndex(1);
    } else {
        this->personTypeCombo->setCurrentIndex(0);
    }
}
