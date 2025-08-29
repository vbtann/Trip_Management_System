#include "AddExpenseDialog.h"

AddExpenseDialog::AddExpenseDialog(TRIP &trip, QWidget *parent, PERSONMANAGER *personManager)
    : QDialog(parent), personManager(personManager), trip(trip) {
    setWindowTitle("Add New Expense");
    setModal(true);
    resize(450, 400);

    setupUI();
    connectSignals();
}

AddExpenseDialog::~AddExpenseDialog() {}

void AddExpenseDialog::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    QLabel *titleLabel = new QLabel("💰 Add New Expense");
    titleLabel->setStyleSheet(
        "QLabel { "
        "    font-size: 18px; "
        "    font-weight: bold; "
        "    color: #2c3e50; "
        "    padding: 10px 0; "
        "    text-align: center; "
        "}");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    QGroupBox *formGroupBox = new QGroupBox("Expense Details");
    formGroupBox->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 14px; "
        "    color: #27ae60; "
        "    border: 2px solid #a9dfbf; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 10px; "
        "    background-color: #f8fbf8; "
        "}"
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 15px; "
        "    padding: 0 5px 0 5px; "
        "    background-color: #f8fbf8; "
        "}");

    QFormLayout *formLayout = new QFormLayout(formGroupBox);
    formLayout->setSpacing(12);
    formLayout->setContentsMargins(15, 20, 15, 15);

    QString inputStyle =
        "QLineEdit, QDateEdit, QComboBox, QTextEdit { "
        "    border: 2px solid #d5f4e6; "
        "    border-radius: 6px; "
        "    padding: 8px; "
        "    font-size: 13px; "
        "    background-color: #fdfdfe; "
        "    color: #2c3e50; "
        "}"
        "QLineEdit:focus, QDateEdit:focus, QComboBox:focus, QTextEdit:focus { "
        "    border-color: #27ae60; "
        "    background-color: #ffffff; "
        "}"
        "QComboBox::drop-down { "
        "    border: none; "
        "    background-color: #27ae60; "
        "    border-radius: 4px; "
        "    width: 20px; "
        "}"
        "QComboBox::down-arrow { "
        "    image: none; "
        "    border: 3px solid #ffffff; "
        "    border-top-color: transparent; "
        "    border-left-color: transparent; "
        "    border-right-color: transparent; "
        "}";

    dateEdit = new QDateEdit(QDate::currentDate());
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    dateEdit->setStyleSheet(inputStyle);

    QLabel *dateLabel = new QLabel("📅 Date:");
    dateLabel->setStyleSheet("QLabel { font-weight: bold; color: #2c3e50; }");
    formLayout->addRow(dateLabel, dateEdit);

    categoryComboBox = new QComboBox();
    categoryComboBox->addItems({"Food", "Drink", "Gas", "Accommodation", "Others"});
    categoryComboBox->setStyleSheet(inputStyle);

    QLabel *categoryLabel = new QLabel("📂 Category:");
    categoryLabel->setStyleSheet("QLabel { font-weight: bold; color: #2c3e50; }");
    formLayout->addRow(categoryLabel, categoryComboBox);

    amountLineEdit = new QLineEdit();
    amountLineEdit->setPlaceholderText("Enter amount (thousand VND)");
    amountLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]+"), this));
    amountLineEdit->setStyleSheet(inputStyle);

    QLabel *amountLabel = new QLabel("💵 Amount:");
    amountLabel->setStyleSheet("QLabel { font-weight: bold; color: #2c3e50; }");
    formLayout->addRow(amountLabel, amountLineEdit);

    noteTextEdit = new QTextEdit();
    noteTextEdit->setPlaceholderText("Enter expense description/note...");
    noteTextEdit->setMaximumHeight(80);
    noteTextEdit->setStyleSheet(inputStyle);

    QLabel *noteLabel = new QLabel("📝 Note:");
    noteLabel->setStyleSheet("QLabel { font-weight: bold; color: #2c3e50; }");
    formLayout->addRow(noteLabel, noteTextEdit);

    mainLayout->addWidget(formGroupBox);

    QGroupBox *personGroupBox = new QGroupBox("👤 Person in Charge");
    personGroupBox->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 14px; "
        "    color: #27ae60; "
        "    border: 2px solid #a9dfbf; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 10px; "
        "    background-color: #f8fbf8; "
        "}"
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 15px; "
        "    padding: 0 5px 0 5px; "
        "    background-color: #f8fbf8; "
        "}");

    QVBoxLayout *personLayout = new QVBoxLayout(personGroupBox);
    personLayout->setContentsMargins(15, 20, 15, 15);
    personLayout->setSpacing(10);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLineEdit = new QLineEdit();
    searchLineEdit->setPlaceholderText("🔍 Search for people...");
    searchLineEdit->setStyleSheet(inputStyle);

    QPushButton *clearSearchButton = new QPushButton("❌");
    clearSearchButton->setMaximumWidth(35);
    clearSearchButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #e74c3c; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 6px; "
        "    padding: 8px; "
        "    font-weight: bold; "
        "    font-size: 11px; "
        "}"
        "QPushButton:hover { "
        "    background-color: #c0392b; "
        "}"
        "QPushButton:pressed { "
        "    background-color: #a93226; "
        "}");

    searchLayout->addWidget(searchLineEdit);
    searchLayout->addWidget(clearSearchButton);
    personLayout->addLayout(searchLayout);

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setStyleSheet(
        "QScrollArea { "
        "    border: 2px solid #d5f4e6; "
        "    border-radius: 6px; "
        "    background-color: #fdfdfe; "
        "}"
        "QScrollBar:vertical { "
        "    background-color: #ecf0f1; "
        "    width: 12px; "
        "    border-radius: 6px; "
        "}"
        "QScrollBar::handle:vertical { "
        "    background-color: #27ae60; "
        "    border-radius: 6px; "
        "    min-height: 20px; "
        "}"
        "QScrollBar::handle:vertical:hover { "
        "    background-color: #229954; "
        "}");

    scrollArea->setWidgetResizable(true);
    scrollArea->setMaximumHeight(200);
    scrollArea->setMinimumHeight(150);

    peopleListWidget = new QWidget();
    peopleListLayout = new QVBoxLayout(peopleListWidget);
    peopleListLayout->setSpacing(5);
    peopleListLayout->setContentsMargins(10, 10, 10, 10);

    scrollArea->setWidget(peopleListWidget);
    personLayout->addWidget(scrollArea);

    selectedPersonLabel = new QLabel("ℹ️ No person selected");
    selectedPersonLabel->setStyleSheet(
        "QLabel { "
        "    color: #7f8c8d; "
        "    font-style: italic; "
        "    padding: 8px; "
        "    background-color: #ecf0f1; "
        "    border-radius: 6px; "
        "    border: 1px solid #bdc3c7; "
        "    font-size: 12px; "
        "}");
    personLayout->addWidget(selectedPersonLabel);

    QPushButton *refreshButton = new QPushButton("🔄 Refresh List");
    refreshButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #3498db; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 6px; "
        "    padding: 8px 12px; "
        "    font-weight: bold; "
        "    font-size: 12px; "
        "}"
        "QPushButton:hover { "
        "    background-color: #2980b9; "
        "}"
        "QPushButton:pressed { "
        "    background-color: #21618c; "
        "}");
    personLayout->addWidget(refreshButton);

    mainLayout->addWidget(personGroupBox);

    mainLayout->addStretch();

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *addButton = new QPushButton("✅ Add Expense");
    QPushButton *cancelButton = new QPushButton("❌ Cancel");

    addButton->setDefault(true);
    addButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #27ae60; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 8px; "
        "    padding: 12px 24px; "
        "    font-weight: bold; "
        "    font-size: 14px; "
        "    min-width: 120px; "
        "}"
        "QPushButton:hover { "
        "    background-color: #229954; "
        "    transform: scale(1.05); "
        "}"
        "QPushButton:pressed { "
        "    background-color: #1e8449; "
        "}");

    cancelButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #e74c3c; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 8px; "
        "    padding: 12px 24px; "
        "    font-weight: bold; "
        "    font-size: 14px; "
        "    min-width: 120px; "
        "}"
        "QPushButton:hover { "
        "    background-color: #c0392b; "
        "    transform: scale(1.05); "
        "}"
        "QPushButton:pressed { "
        "    background-color: #a93226; "
        "}");

    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addSpacing(10);
    buttonLayout->addWidget(addButton);
    mainLayout->addLayout(buttonLayout);

    this->addButton = addButton;
    this->cancelButton = cancelButton;
    this->refreshButton = refreshButton;
    this->clearSearchButton = clearSearchButton;

    refreshPersonList();

    setStyleSheet("QDialog { background-color: #f8fbf8; }");
    resize(500, 650);
}

void AddExpenseDialog::connectSignals() {
    connect(addButton, &QPushButton::clicked, this, &AddExpenseDialog::acceptDialog);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(refreshButton, &QPushButton::clicked, this, &AddExpenseDialog::refreshPersonList);
    connect(searchLineEdit, &QLineEdit::textChanged, this, &AddExpenseDialog::filterPeopleList);
    connect(clearSearchButton, &QPushButton::clicked, this, &AddExpenseDialog::clearSearch);
}

void AddExpenseDialog::refreshPersonList() {
    QLayoutItem *item;
    while ((item = peopleListLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    personCheckBoxes.clear();

    vector<MEMBER> members = trip.getMembers();
    allMembers = members;

    sort(members.begin(), members.end(),
         [](const MEMBER &a, const MEMBER &b) { return a.getFullName() < b.getFullName(); });

    if (members.empty()) {
        QLabel *noMembersLabel = new QLabel("❌ No members found in this trip");
        noMembersLabel->setStyleSheet(
            "QLabel { "
            "    color: #e74c3c; "
            "    font-style: italic; "
            "    padding: 10px; "
            "    text-align: center; "
            "    background-color: #fdf2f2; "
            "    border: 1px solid #f5b7b1; "
            "    border-radius: 4px; "
            "}");
        noMembersLabel->setAlignment(Qt::AlignCenter);
        peopleListLayout->addWidget(noMembersLabel);
    } else {
        for (const MEMBER &member : members) {
            QCheckBox *checkBox = new QCheckBox();
            QString memberText = QString("👤 %1\n📧 ID: %2")
                                     .arg(QString::fromStdString(member.getFullName()))
                                     .arg(QString::fromStdString(member.getID()));

            checkBox->setText(memberText);
            checkBox->setStyleSheet(
                "QCheckBox { "
                "    font-size: 12px; "
                "    color: #2c3e50; "
                "    padding: 8px; "
                "    background-color: #fdfdfe; "
                "    border: 1px solid #d5f4e6; "
                "    border-radius: 6px; "
                "    spacing: 8px; "
                "}"
                "QCheckBox:hover { "
                "    background-color: #e8f8f5; "
                "    border-color: #a9dfbf; "
                "}"
                "QCheckBox::indicator { "
                "    width: 18px; "
                "    height: 18px; "
                "    border: 2px solid #27ae60; "
                "    border-radius: 4px; "
                "    background-color: white; "
                "}"
                "QCheckBox::indicator:checked { "
                "    background-color: #27ae60; "
                "    border-color: #1e7e34; "
                "}"
                "QCheckBox::indicator:checked:hover { "
                "    background-color: #229954; "
                "}");

            checkBox->setProperty("memberID", QString::fromStdString(member.getID()));

            connect(checkBox, &QCheckBox::toggled, this, &AddExpenseDialog::onPersonCheckBoxToggled);

            peopleListLayout->addWidget(checkBox);
            personCheckBoxes.append(checkBox);
        }
    }

    peopleListLayout->addStretch();

    updateSelectedPersonLabel();
}

void AddExpenseDialog::filterPeopleList() {
    QString searchText = searchLineEdit->text().toLower();

    for (QCheckBox *checkBox : personCheckBoxes) {
        QString memberText = checkBox->text().toLower();
        bool matches = memberText.contains(searchText);
        checkBox->setVisible(matches);
    }
}

void AddExpenseDialog::clearSearch() {
    searchLineEdit->clear();
    filterPeopleList();
}

void AddExpenseDialog::onPersonCheckBoxToggled(bool checked) {
    QCheckBox *senderCheckBox = qobject_cast<QCheckBox *>(sender());
    if (!senderCheckBox) return;

    if (checked) {
        for (QCheckBox *checkBox : personCheckBoxes) {
            if (checkBox != senderCheckBox) {
                checkBox->blockSignals(true);
                checkBox->setChecked(false);
                checkBox->blockSignals(false);
            }
        }
    }

    updateSelectedPersonLabel();
}

void AddExpenseDialog::updateSelectedPersonLabel() {
    QCheckBox *selectedCheckBox = nullptr;
    QString selectedMemberID;

    for (QCheckBox *checkBox : personCheckBoxes) {
        if (checkBox->isChecked()) {
            selectedCheckBox = checkBox;
            selectedMemberID = checkBox->property("memberID").toString();
            break;
        }
    }

    if (selectedCheckBox && !selectedMemberID.isEmpty()) {
        QString memberName = selectedCheckBox->text().split('\n')[0].remove("👤 ");
        selectedPersonLabel->setText(QString("✅ Selected: %1").arg(memberName));
        selectedPersonLabel->setStyleSheet(
            "QLabel { "
            "    color: #27ae60; "
            "    font-weight: bold; "
            "    padding: 8px; "
            "    background-color: #d5f4e6; "
            "    border-radius: 6px; "
            "    border: 1px solid #a9dfbf; "
            "    font-size: 12px; "
            "}");
    } else {
        selectedPersonLabel->setText("ℹ️ No person selected");
        selectedPersonLabel->setStyleSheet(
            "QLabel { "
            "    color: #7f8c8d; "
            "    font-style: italic; "
            "    padding: 8px; "
            "    background-color: #ecf0f1; "
            "    border-radius: 6px; "
            "    border: 1px solid #bdc3c7; "
            "    font-size: 12px; "
            "}");
    }
}

bool AddExpenseDialog::validateInputs() {
    if (amountLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter an amount.");
        amountLineEdit->setFocus();
        return false;
    }

    long long amount = amountLineEdit->text().toLongLong() * 1000;
    if (amount < 1000 || amount % 1000 != 0) {
        QMessageBox::warning(this, "Validation Error", "Invalid amount.");
        amountLineEdit->setFocus();
        return false;
    }

    bool personSelected = false;
    for (QCheckBox *checkBox : personCheckBoxes) {
        if (checkBox->isChecked()) {
            personSelected = true;
            break;
        }
    }

    if (!personSelected) {
        QMessageBox::warning(this, "Validation Error", "Please select a person in charge.");
        return false;
    }

    if (noteTextEdit->toPlainText().length() > 500) {
        QMessageBox::warning(this, "Validation Error", "Note cannot exceed 500 characters.");
        noteTextEdit->setFocus();
        return false;
    }

    return true;
}

void AddExpenseDialog::acceptDialog() {
    if (!validateInputs()) {
        return;
    }

    try {
        QDate selectedDate = dateEdit->date();
        QString category = categoryComboBox->currentText();
        QString amountText = amountLineEdit->text();
        QString note = noteTextEdit->toPlainText();

        QString selectedPersonId;
        for (QCheckBox *checkBox : personCheckBoxes) {
            if (checkBox->isChecked()) {
                selectedPersonId = checkBox->property("memberID").toString();
                break;
            }
        }

        DATE expenseDate(selectedDate.day(), selectedDate.month(), selectedDate.year());
        CATEGORY expenseCategory = stringToCategory(category.toStdString());
        long long amount = amountText.toLongLong() * 1000;

        MEMBER memberInCharge = personManager->getMemberByID(selectedPersonId.toStdString());

        _expenseData = EXPENSE(expenseDate, expenseCategory, amount, note.toStdString(), memberInCharge);

        accept();
    } catch (const exception &e) {
        QMessageBox::critical(this, "Error", QString("Failed to add expense: %1").arg(e.what()));
    }
}

EXPENSE AddExpenseDialog::getExpense() const { return _expenseData; }