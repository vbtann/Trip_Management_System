#include "EditPersonDialog.h"

EditPersonDialog::EditPersonDialog(PERSON* person, QWidget* parent)
    : QDialog(parent), originalPerson(person), personManager(nullptr), updatedPerson(nullptr) {
    setupUI();
    populateFields();
    setWindowTitle("Edit Person Information");
    setModal(true);
    setMinimumSize(500, 600);
}

EditPersonDialog::EditPersonDialog(PERSON* person, PERSONMANAGER* personManager, QWidget* parent)
    : QDialog(parent), originalPerson(person), personManager(personManager), updatedPerson(nullptr) {
    setupUI();
    populateFields();
    setWindowTitle("Edit Person Information");
    setModal(true);
    setMinimumSize(500, 600);
}

void EditPersonDialog::setupUI() {
    mainLayout = new QVBoxLayout(this);

    titleLabel = new QLabel("Edit Person Information", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin: 10px;");

    setupPersonTypeSelection();
    setupBasicInfoForm();
    setupRoleSpecificForm();

    formLayout = new QFormLayout();
    formLayout->addRow("Person Type:", personTypeCombo);
    formLayout->addRow("ID:", idLineEdit);
    formLayout->addRow("Full Name:", fullNameLineEdit);
    formLayout->addRow("Gender:", genderCombo);
    formLayout->addRow("Date of Birth:", dobDateEdit);
    formLayout->addRow("Email:", emailLineEdit);
    formLayout->addRow("Phone:", phoneLineEdit);
    formLayout->addRow("Address:", addressLineEdit);
    formLayout->addRow("Emergency Contact:", emergencyContactLineEdit);

    buttonLayout = new QHBoxLayout();
    saveButton = new QPushButton("💾 Save Changes", this);
    cancelButton = new QPushButton("❌ Cancel", this);

    buttonLayout->addStretch();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(memberGroupBox);
    mainLayout->addWidget(hostGroupBox);
    mainLayout->addLayout(buttonLayout);

    connect(personTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &EditPersonDialog::onPersonTypeChanged);
    connect(saveButton, &QPushButton::clicked, this, &EditPersonDialog::onSaveClicked);
    connect(cancelButton, &QPushButton::clicked, this, &EditPersonDialog::onCancelClicked);
    connect(fullNameLineEdit, &QLineEdit::textChanged, this, &EditPersonDialog::onFullNameChanged);
    connect(dobDateEdit, &QDateEdit::dateChanged, this, &EditPersonDialog::onDateOfBirthChanged);

    styleComponents();
    updateFormVisibility();
}

void EditPersonDialog::setupPersonTypeSelection() {
    personTypeCombo = new QComboBox(this);
    personTypeCombo->addItem("Member");
    personTypeCombo->addItem("Host");

    personTypeCombo->setEnabled(true);
    personTypeCombo->setStyleSheet("QComboBox:disabled { background-color: #f0f0f0; color: #666; }");
}

void EditPersonDialog::setupBasicInfoForm() {
    idLineEdit = new QLineEdit(this);
    idLineEdit->setReadOnly(true);
    idLineEdit->setStyleSheet("QLineEdit:read-only { background-color: #f0f0f0; color: #666; }");

    fullNameLineEdit = new QLineEdit(this);

    connect(fullNameLineEdit, &QLineEdit::textChanged, this, &EditPersonDialog::onFullNameChanged);

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

void EditPersonDialog::setupRoleSpecificForm() {
    memberGroupBox = new QGroupBox("Member Information", this);
    QFormLayout* memberLayout = new QFormLayout(memberGroupBox);

    interestsTextEdit = new QTextEdit(this);
    interestsTextEdit->setMaximumHeight(80);
    interestsTextEdit->setPlaceholderText("Enter interests separated by commas...");

    totalSpentLineEdit = new QLineEdit(this);
    totalSpentLineEdit->setPlaceholderText("0.0");

    memberLayout->addRow("Interests:", interestsTextEdit);
    memberLayout->addRow("Total Spent:", totalSpentLineEdit);

    hostGroupBox = new QGroupBox("Host Information", this);
    QFormLayout* hostLayout = new QFormLayout(hostGroupBox);
    hostLayout->addRow(new QLabel("Host-specific settings can be managed here."));
}

void EditPersonDialog::styleComponents() {
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

    QString saveButtonStyle =
        "QPushButton { "
        "background-color: #2ecc71; "
        "color: white; "
        "border: none; "
        "padding: 8px 16px; "
        "border-radius: 4px; "
        "font-size: 13px; "
        "font-weight: bold; "
        "}"
        "QPushButton:hover { "
        "background-color: #27ae60; "
        "}"
        "QPushButton:pressed { "
        "background-color: #229954; "
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

    fullNameLineEdit->setStyleSheet(lineEditStyle);
    emailLineEdit->setStyleSheet(lineEditStyle);
    phoneLineEdit->setStyleSheet(lineEditStyle);
    addressLineEdit->setStyleSheet(lineEditStyle);
    emergencyContactLineEdit->setStyleSheet(lineEditStyle);
    totalSpentLineEdit->setStyleSheet(lineEditStyle);

    genderCombo->setStyleSheet(comboBoxStyle);

    saveButton->setStyleSheet(saveButtonStyle);
    cancelButton->setStyleSheet(cancelButtonStyle);
}

void EditPersonDialog::populateFields() {
    if (!originalPerson) return;

    personTypeCombo->setCurrentText(QString::fromStdString(originalPerson->getRole()));

    idLineEdit->setText(QString::fromStdString(originalPerson->getID()));
    fullNameLineEdit->setText(QString::fromStdString(originalPerson->getFullName()));
    genderCombo->setCurrentText(QString::fromStdString(genderToString(originalPerson->getGender())));

    DATE dob = originalPerson->getDateOfBirth();
    dobDateEdit->setDate(QDate(dob.getYear(), dob.getMonth(), dob.getDay()));

    emailLineEdit->setText(QString::fromStdString(originalPerson->getEmail()));
    phoneLineEdit->setText(QString::fromStdString(originalPerson->getPhoneNumber()));
    addressLineEdit->setText(QString::fromStdString(originalPerson->getAddress()));

    if (originalPerson->getRole() == "Member") {
        const MEMBER* member = static_cast<const MEMBER*>(originalPerson);

        emergencyContactLineEdit->setText(QString::fromStdString(member->getEmergencyContact()));

        const vector<string>& interests = member->getInterests();
        QString interestsText;
        for (size_t i = 0; i < interests.size(); ++i) {
            interestsText += QString::fromStdString(interests[i]);
            if (i < interests.size() - 1) {
                interestsText += ", ";
            }
        }
        interestsTextEdit->setPlainText(interestsText);

        totalSpentLineEdit->setText(QString::number(member->getTotalSpent(), 'f', 2));
    } else if (originalPerson->getRole() == "Host") {
        const HOST* host = static_cast<const HOST*>(originalPerson);
        emergencyContactLineEdit->setText(QString::fromStdString(host->getEmergencyContact()));
    }
}

void EditPersonDialog::onPersonTypeChanged() { updateFormVisibility(); }

void EditPersonDialog::updateFormVisibility() {
    bool isMember = (personTypeCombo->currentText() == "Member");
    memberGroupBox->setVisible(isMember);
    hostGroupBox->setVisible(!isMember);
}

void EditPersonDialog::onFullNameChanged() {
    QString text = fullNameLineEdit->text().toUpper();
    fullNameLineEdit->setText(text);

    if (!text.isEmpty()) {
        QDate dobDate = dobDateEdit->date();
        DATE dob(dobDate.day(), dobDate.month(), dobDate.year());

        string newID = PERSONFACTORY::generatePersonID(fullNameLineEdit->text().trimmed().toStdString(), dob);
        idLineEdit->setText(QString::fromStdString(newID));
    }
}

void EditPersonDialog::onDateOfBirthChanged() {
    if (fullNameLineEdit->text().trimmed().isEmpty()) {
        return;
    }

    QDate dobDate = dobDateEdit->date();
    DATE dob(dobDate.day(), dobDate.month(), dobDate.year());

    string newID = PERSONFACTORY::generatePersonID(fullNameLineEdit->text().trimmed().toStdString(), dob);
    idLineEdit->setText(QString::fromStdString(newID));
}

bool EditPersonDialog::validateInput() {
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

    string email = emailLineEdit->text().trimmed().toStdString();
    if (!email.empty() && (email.find("@") == string::npos || email.find(".") == string::npos)) {
        QMessageBox::warning(this, "Validation Error", "Email must be a valid email address!");
        emailLineEdit->setFocus();
        return false;
    }

    string phone = phoneLineEdit->text().trimmed().toStdString();
    if (!phone.empty() && phone.length() != 10) {
        QMessageBox::warning(this, "Validation Error", "Phone number must be exactly 10 digits!");
        phoneLineEdit->setFocus();
        return false;
    }

    QDate dob = dobDateEdit->date();
    if (dob > QDate::currentDate()) {
        QMessageBox::warning(this, "Validation Error", "Date of birth cannot be in the future!");
        dobDateEdit->setFocus();
        return false;
    }

    if (!validateNoDuplicates()) {
        return false;
    }

    return true;
}

bool EditPersonDialog::validateNoDuplicates() {
    if (!personManager) {
        return true;
    }

    string currentFullName = fullNameLineEdit->text().trimmed().toStdString();
    string currentEmail = emailLineEdit->text().trimmed().toStdString();
    string currentPhone = phoneLineEdit->text().trimmed().toStdString();

    const vector<PERSON>& allPeople = personManager->getAllPeople();

    for (const PERSON& person : allPeople) {
        if (originalPerson && person.getID() == originalPerson->getID()) continue;

        if (!currentEmail.empty() && person.getEmail() == currentEmail) {
            QMessageBox::warning(this, "Duplicate Email",
                                 QString("Another person already has this email address!\n\n"
                                         "Existing person: %1 (%2)")
                                     .arg(QString::fromStdString(person.getFullName()))
                                     .arg(QString::fromStdString(person.getRole())));
            emailLineEdit->setFocus();
            return false;
        }

        if (!currentPhone.empty() && person.getPhoneNumber() == currentPhone) {
            QMessageBox::warning(this, "Duplicate Phone Number",
                                 QString("Another person already has this phone number!\n\n"
                                         "Existing person: %1 (%2)")
                                     .arg(QString::fromStdString(person.getFullName()))
                                     .arg(QString::fromStdString(person.getRole())));
            phoneLineEdit->setFocus();
            return false;
        }
    }

    return true;
}

void EditPersonDialog::onSaveClicked() {
    if (!validateInput()) {
        return;
    }

    accept();
}

void EditPersonDialog::onCancelClicked() { reject(); }

PERSON* EditPersonDialog::getUpdatedPerson() const {
    if (!originalPerson) return nullptr;

    string id = idLineEdit->text().trimmed().toStdString();
    string fullName = fullNameLineEdit->text().trimmed().toStdString();
    GENDER gender = (genderCombo->currentText() == "Male") ? GENDER::Male : GENDER::Female;

    QDate dobDate = dobDateEdit->date();
    DATE dob(dobDate.day(), dobDate.month(), dobDate.year());

    if (personTypeCombo->currentText() == "Member") {
        MEMBER* member = new MEMBER(id, fullName, gender, dob);

        member->setEmail(emailLineEdit->text().trimmed().toStdString());
        member->setPhoneNumber(phoneLineEdit->text().trimmed().toStdString());
        member->setAddress(addressLineEdit->text().trimmed().toStdString());
        member->setEmergencyContact(emergencyContactLineEdit->text().trimmed().toStdString());

        QString interestsText = interestsTextEdit->toPlainText().trimmed();
        if (!interestsText.isEmpty()) {
            QStringList interestsList = interestsText.split(",", Qt::SkipEmptyParts);
            for (const QString& interest : interestsList) {
                member->addInterest(interest.trimmed().toStdString());
            }
        }

        bool ok;
        double totalSpent = totalSpentLineEdit->text().toDouble(&ok);
        if (ok && totalSpent > 0) {
            member->addToTotalSpent(totalSpent);
        }

        updatedPerson = member;
    } else {
        HOST* host = new HOST(id, fullName, gender, dob);

        host->setEmail(emailLineEdit->text().trimmed().toStdString());
        host->setPhoneNumber(phoneLineEdit->text().trimmed().toStdString());
        host->setAddress(addressLineEdit->text().trimmed().toStdString());
        host->setEmergencyContact(emergencyContactLineEdit->text().trimmed().toStdString());

        updatedPerson = host;
    }

    return updatedPerson;
}

PERSON* EditPersonDialog::getOriginalPerson() const { return originalPerson; }