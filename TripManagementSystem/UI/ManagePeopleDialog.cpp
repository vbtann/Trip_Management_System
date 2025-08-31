#include "ManagePeopleDialog.h"

#include "../Managers/FileManager.h"
#include "../Models/Utility_Functions.cpp"
#include "AddPersonDialog.h"

ManagePeopleDialog::ManagePeopleDialog(PERSONMANAGER *personManager, QWidget *parent)
    : QDialog(parent), personManager(personManager) {
    setWindowTitle("👥 Manage People");
    setModal(true);
    setMinimumSize(900, 600);

    updatePeopleData();

    setupUI();
    styleComponents();
    refreshPersonList();

    QRect screenGeometry = QApplication::primaryScreen()->geometry();
    move(screenGeometry.center() - rect().center());
}

void ManagePeopleDialog::updatePeopleData() {
    try {
        QWidget *parentWidget = this->parentWidget();
        MainWindow *mainWindow = qobject_cast<MainWindow *>(parentWidget);

        if (!mainWindow) {
            QWidget *topLevel = parentWidget;
            while (topLevel && !qobject_cast<MainWindow *>(topLevel)) {
                topLevel = topLevel->parentWidget();
            }
            mainWindow = qobject_cast<MainWindow *>(topLevel);
        }

        if (!mainWindow) {
            return;
        }

        TRIPMANAGER *tripManager = mainWindow->getTripManager();
        if (!tripManager) {
            return;
        }

        vector<TRIP> currentTrips = tripManager->getAllTrips();

        vector<MEMBER> allMembers = personManager->getAllMembers();
        for (MEMBER &member : allMembers) {
            member.clearSpendings();
            member.setTotalSpent(0);

            for (const TRIP &trip : currentTrips) {
                vector<MEMBER> tripMembers = trip.getMembers();
                bool isMemberInTrip = false;

                for (const MEMBER &tripMember : tripMembers) {
                    if (tripMember.getID() == member.getID()) {
                        isMemberInTrip = true;
                        break;
                    }
                }

                if (isMemberInTrip) {
                    vector<EXPENSE> tripExpenses = trip.getAllExpenses();
                    for (const EXPENSE &expense : tripExpenses) {
                        if (expense.getPIC().getID() == member.getID()) {
                            member.addSpending(expense, trip);
                        }
                    }
                }
            }

            MEMBER originalMember = personManager->getMemberByID(member.getID());
            personManager->updateMember(originalMember, member);
        }

        vector<HOST> allHosts = personManager->getAllHosts();
        for (HOST &host : allHosts) {
            host.getHostedTripIDs().clear();

            for (const TRIP &trip : currentTrips) {
                if (trip.getHost().getID() == host.getID()) {
                    host.hostTrip(trip.getID());
                }
            }

            HOST originalHost = personManager->getHostByID(host.getID());
            personManager->updateHost(originalHost, host);
        }

    } catch (const std::exception &e) {
        QMessageBox::warning(
            this, "Data Update Warning",
            QString("Could not fully update people data: %1\n\nDialog will show current data.").arg(e.what()));
    }
}

void ManagePeopleDialog::setupUI() {
    setStyleSheet("QDialog { background-color: #f8fbf8; }");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    QWidget *leftWidget = new QWidget();
    leftWidget->setMinimumWidth(350);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);

    QLabel *titleLabel = new QLabel("👥 People Management Dashboard");
    titleLabel->setStyleSheet(
        "QLabel { "
        "    font-size: 20px; "
        "    font-weight: bold; "
        "    color: #27ae60; "
        "    margin: 10px 0; "
        "    padding: 8px; "
        "}");
    titleLabel->setAlignment(Qt::AlignCenter);

    QGroupBox *statsGroup = new QGroupBox("📊 Statistics");
    statsGroup->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 14px; "
        "    color: #27ae60; "
        "    border: 2px solid #a9dfbf; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 10px; "
        "    background-color: #fdfdfe; "
        "}"
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 15px; "
        "    padding: 0 5px 0 5px; "
        "    background-color: #fdfdfe; "
        "}");

    QGridLayout *statsLayout = new QGridLayout(statsGroup);
    statsLayout->setContentsMargins(15, 20, 15, 15);

    QString statsStyleSheet =
        "QLabel { "
        "    font-size: 13px; "
        "    color: #2c3e50; "
        "    padding: 4px 8px; "
        "    background-color: #e8f8f5; "
        "    border-radius: 4px; "
        "    border: 1px solid #d5f4e6; "
        "}";

    personCountLabel = new QLabel("👥 Total People: 0");
    personCountLabel->setStyleSheet(statsStyleSheet);

    memberCountLabel = new QLabel("👤 Members: 0");
    memberCountLabel->setStyleSheet(statsStyleSheet);

    hostCountLabel = new QLabel("🏠 Hosts: 0");
    hostCountLabel->setStyleSheet(statsStyleSheet);

    statsLayout->addWidget(personCountLabel, 0, 0);
    statsLayout->addWidget(memberCountLabel, 1, 0);
    statsLayout->addWidget(hostCountLabel, 2, 0);

    QLabel *listLabel = new QLabel("📋 People List:");
    listLabel->setStyleSheet(
        "QLabel { "
        "    font-weight: bold; "
        "    margin-top: 10px; "
        "    color: #2c3e50; "
        "    font-size: 14px; "
        "}");

    peopleListWidget = new QListWidget();
    peopleListWidget->setMinimumHeight(300);

    leftLayout->addWidget(titleLabel);
    leftLayout->addWidget(statsGroup);
    leftLayout->addWidget(listLabel);
    leftLayout->addWidget(peopleListWidget);

    QWidget *rightWidget = new QWidget();
    rightWidget->setMinimumWidth(400);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);

    QGroupBox *actionsGroup = new QGroupBox("⚙️ Actions");
    actionsGroup->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 14px; "
        "    color: #27ae60; "
        "    border: 2px solid #a9dfbf; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 10px; "
        "    background-color: #fdfdfe; "
        "}"
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 15px; "
        "    padding: 0 5px 0 5px; "
        "    background-color: #fdfdfe; "
        "}");

    QVBoxLayout *actionsLayout = new QVBoxLayout(actionsGroup);
    actionsLayout->setContentsMargins(15, 20, 15, 15);
    actionsLayout->setSpacing(8);

    addPersonButton = new QPushButton("➕ Add New Person");
    editPersonButton = new QPushButton("✏️ Edit Selected Person");
    deletePersonButton = new QPushButton("🗑️ Delete Selected Person");
    viewPersonButton = new QPushButton("👁️ View Person Details");

    editPersonButton->setEnabled(false);
    deletePersonButton->setEnabled(false);
    viewPersonButton->setEnabled(false);

    actionsLayout->addWidget(addPersonButton);
    actionsLayout->addWidget(editPersonButton);
    actionsLayout->addWidget(deletePersonButton);
    actionsLayout->addWidget(viewPersonButton);
    actionsLayout->addStretch();

    QHBoxLayout *importExportLayout = new QHBoxLayout();

    importPeopleButton = new QPushButton("📥 Import People");
    exportPeopleButton = new QPushButton("📤 Export People");

    importExportLayout->addWidget(importPeopleButton);
    importExportLayout->addWidget(exportPeopleButton);

    actionsLayout->addLayout(importExportLayout);

    QGroupBox *infoGroup = new QGroupBox("ℹ️ Selected Person Information");
    infoGroup->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 14px; "
        "    color: #27ae60; "
        "    border: 2px solid #a9dfbf; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 10px; "
        "    background-color: #fdfdfe; "
        "}"
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 15px; "
        "    padding: 0 5px 0 5px; "
        "    background-color: #fdfdfe; "
        "}");

    QVBoxLayout *infoLayout = new QVBoxLayout(infoGroup);
    infoLayout->setContentsMargins(15, 20, 15, 15);

    selectedPersonInfoLabel = new QLabel("No person selected");
    selectedPersonInfoLabel->setWordWrap(true);
    selectedPersonInfoLabel->setMinimumHeight(200);
    selectedPersonInfoLabel->setStyleSheet(
        "QLabel { "
        "    border: 2px solid #d5f4e6; "
        "    padding: 15px; "
        "    font-size: 13px; "
        "    background-color: #f8fbf8; "
        "    border-radius: 6px; "
        "    color: #2c3e50; "
        "}");

    infoLayout->addWidget(selectedPersonInfoLabel);

    QHBoxLayout *controlLayout = new QHBoxLayout();
    refreshButton = new QPushButton("🔄 Refresh");
    closeButton = new QPushButton("❌ Close");

    controlLayout->addWidget(refreshButton);
    controlLayout->addStretch();
    controlLayout->addWidget(closeButton);

    rightLayout->addWidget(actionsGroup);
    rightLayout->addWidget(infoGroup);
    rightLayout->addStretch();
    rightLayout->addLayout(controlLayout);

    mainLayout->addWidget(leftWidget);
    mainLayout->addWidget(rightWidget);

    connect(addPersonButton, &QPushButton::clicked, this, &ManagePeopleDialog::onAddPersonClicked);
    connect(editPersonButton, &QPushButton::clicked, this, &ManagePeopleDialog::onEditPersonClicked);
    connect(deletePersonButton, &QPushButton::clicked, this, &ManagePeopleDialog::onDeletePersonClicked);
    connect(viewPersonButton, &QPushButton::clicked, this, &ManagePeopleDialog::onViewPersonClicked);
    connect(refreshButton, &QPushButton::clicked, this, &ManagePeopleDialog::onRefreshClicked);
    connect(closeButton, &QPushButton::clicked, this, &ManagePeopleDialog::onCloseClicked);
    connect(peopleListWidget, &QListWidget::itemSelectionChanged, this, &ManagePeopleDialog::onPersonSelectionChanged);
    connect(peopleListWidget, &QListWidget::itemDoubleClicked, this, &ManagePeopleDialog::onViewPersonClicked);
    connect(importPeopleButton, &QPushButton::clicked, this, &ManagePeopleDialog::onImportPeopleClicked);
    connect(exportPeopleButton, &QPushButton::clicked, this, &ManagePeopleDialog::onExportPeopleClicked);
}

void ManagePeopleDialog::styleComponents() {
    peopleListWidget->setStyleSheet(
        "QListWidget { "
        "    border: 2px solid #a9dfbf; "
        "    border-radius: 8px; "
        "    background-color: #fdfdfe; "
        "    selection-background-color: #d5f4e6; "
        "    font-size: 13px; "
        "    outline: none; "
        "}"
        "QListWidget::item { "
        "    padding: 12px 10px; "
        "    border-bottom: 1px solid #e8f8f5; "
        "    color: #2c3e50; "
        "    min-height: 20px; "
        "}"
        "QListWidget::item:selected { "
        "    background-color: #d5f4e6; "
        "    color: #1e7e34; "
        "    font-weight: bold; "
        "}"
        "QListWidget::item:hover { "
        "    background-color: #e8f8f5; "
        "}");

    QString greenButtonStyle =
        "QPushButton { "
        "    background-color: #27ae60; "
        "    color: white; "
        "    border: none; "
        "    padding: 10px 15px; "
        "    border-radius: 6px; "
        "    font-size: 13px; "
        "    font-weight: bold; "
        "    margin: 2px; "
        "}"
        "QPushButton:hover { "
        "    background-color: #229954; "
        "}"
        "QPushButton:pressed { "
        "    background-color: #1e8449; "
        "}"
        "QPushButton:disabled { "
        "    background-color: #bdc3c7; "
        "    color: #7f8c8d; "
        "}";

    QString blueButtonStyle =
        "QPushButton { "
        "    background-color: #3498db; "
        "    color: white; "
        "    border: none; "
        "    padding: 10px 15px; "
        "    border-radius: 6px; "
        "    font-size: 13px; "
        "    font-weight: bold; "
        "    margin: 2px; "
        "}"
        "QPushButton:hover { "
        "    background-color: #2980b9; "
        "}"
        "QPushButton:pressed { "
        "    background-color: #21618c; "
        "}"
        "QPushButton:disabled { "
        "    background-color: #bdc3c7; "
        "    color: #7f8c8d; "
        "}";

    QString redButtonStyle =
        "QPushButton { "
        "    background-color: #e74c3c; "
        "    color: white; "
        "    border: none; "
        "    padding: 10px 15px; "
        "    border-radius: 6px; "
        "    font-size: 13px; "
        "    font-weight: bold; "
        "    margin: 2px; "
        "}"
        "QPushButton:hover { "
        "    background-color: #c0392b; "
        "}"
        "QPushButton:pressed { "
        "    background-color: #a93226; "
        "}"
        "QPushButton:disabled { "
        "    background-color: #bdc3c7; "
        "    color: #7f8c8d; "
        "}";

    QString orangeButtonStyle =
        "QPushButton { "
        "    background-color: #f39c12; "
        "    color: white; "
        "    border: none; "
        "    padding: 10px 15px; "
        "    border-radius: 6px; "
        "    font-size: 13px; "
        "    font-weight: bold; "
        "    margin: 2px; "
        "}"
        "QPushButton:hover { "
        "    background-color: #e67e22; "
        "}"
        "QPushButton:pressed { "
        "    background-color: #d35400; "
        "}";

    QString grayButtonStyle =
        "QPushButton { "
        "    background-color: #95a5a6; "
        "    color: white; "
        "    border: none; "
        "    padding: 10px 15px; "
        "    border-radius: 6px; "
        "    font-size: 13px; "
        "    font-weight: bold; "
        "    margin: 2px; "
        "}"
        "QPushButton:hover { "
        "    background-color: #7f8c8d; "
        "}"
        "QPushButton:pressed { "
        "    background-color: #6c7b7d; "
        "}";

    addPersonButton->setStyleSheet(greenButtonStyle);
    editPersonButton->setStyleSheet(blueButtonStyle);
    viewPersonButton->setStyleSheet(blueButtonStyle);
    deletePersonButton->setStyleSheet(redButtonStyle);
    refreshButton->setStyleSheet(blueButtonStyle);
    closeButton->setStyleSheet(redButtonStyle);
    importPeopleButton->setStyleSheet(blueButtonStyle);
    exportPeopleButton->setStyleSheet(greenButtonStyle);
}

void ManagePeopleDialog::refreshPersonList() {
    peopleListWidget->clear();

    const vector<HOST> &hosts = personManager->getAllHosts();
    const vector<MEMBER> &members = personManager->getAllMembers();

    for (const HOST &host : hosts) {
        QString itemText = QString("%1 - %2 (%3)")
                               .arg(QString::fromStdString(host.getID()))
                               .arg(QString::fromStdString(host.getFullName()))
                               .arg(QString::fromStdString(host.getRole()));

        QListWidgetItem *item = new QListWidgetItem(itemText);
        item->setData(Qt::UserRole, QString::fromStdString(host.getID()));
        peopleListWidget->addItem(item);
    }

    for (const MEMBER &member : members) {
        QString itemText = QString("%1 - %2 - (%3)")
                               .arg(QString::fromStdString(member.getID()))
                               .arg(QString::fromStdString(member.getFullName()))
                               .arg(QString::fromStdString(member.getRole()));
        QListWidgetItem *item = new QListWidgetItem(itemText);
        item->setData(Qt::UserRole, QString::fromStdString(member.getID()));
        peopleListWidget->addItem(item);
    }

    editPersonButton->setEnabled(false);
    deletePersonButton->setEnabled(false);
    viewPersonButton->setEnabled(false);

    updatePersonInfo();
}

void ManagePeopleDialog::updatePersonInfo() {
    size_t totalPeople = personManager->getPersonCount();
    size_t memberCount = personManager->getMemberCount();
    size_t hostCount = personManager->getHostCount();

    personCountLabel->setText(QString("Total People: %1").arg(totalPeople));
    memberCountLabel->setText(QString("Members: %1").arg(memberCount));
    hostCountLabel->setText(QString("Hosts: %1").arg(hostCount));

    QListWidgetItem *currentItem = peopleListWidget->currentItem();
    if (currentItem) {
        QString personId = currentItem->data(Qt::UserRole).toString();
        PERSON *person = personManager->findPersonById(personId.toStdString());

        if (person) {
            QString info = QString(
                               "<b>ID:</b> %1<br>"
                               "<b>Name:</b> %2<br>"
                               "<b>Role:</b> %3<br>"
                               "<b>Gender:</b> %4<br>"
                               "<b>Date of birth:</b> %9<br>"
                               "<b>Age:</b> %5<br>"
                               "<b>Email:</b> %6<br>"
                               "<b>Phone:</b> %7<br>"
                               "<b>Address:</b> %8<br>")
                               .arg(QString::fromStdString(person->getID()))
                               .arg(QString::fromStdString(person->getFullName()))
                               .arg(QString::fromStdString(person->getRole()))
                               .arg(QString::fromStdString(genderToString(person->getGender())))
                               .arg(person->getAge())
                               .arg(QString::fromStdString(person->getEmail()))
                               .arg(QString::fromStdString(person->getPhoneNumber()))
                               .arg(QString::fromStdString(person->getAddress()))
                               .arg(QString::fromStdString(person->getDateOfBirth().toString()));

            selectedPersonInfoLabel->setText(info);
        }
    } else {
        selectedPersonInfoLabel->setText("No person selected");
    }
}

void ManagePeopleDialog::onAddPersonClicked() {
    AddPersonDialog dialog(personManager, this);
    if (dialog.exec() == QDialog::Accepted) {
        PERSON *newPerson = dialog.getPersonData();
        if (newPerson) {
            personManager->addPerson(*newPerson);
            delete newPerson;
            refreshPersonList();
            QMessageBox::information(this, "Success", "Person added successfully!");
        }
    }
}

void ManagePeopleDialog::onEditPersonClicked() {
    QListWidgetItem *currentItem = peopleListWidget->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "No Selection", "Please select a person to edit.");
        return;
    }

    QString personId = currentItem->data(Qt::UserRole).toString();
    PERSON *person = personManager->findPersonById(personId.toStdString());

    if (!person) {
        QMessageBox::warning(this, "Error", "Selected person not found.");
        return;
    }

    EditPersonDialog dialog(person, personManager, this);
    if (dialog.exec() == QDialog::Accepted) {
        PERSON *updatedPerson = dialog.getUpdatedPerson();
        if (updatedPerson) {
            if (personManager->updatePerson(*person, *updatedPerson)) {
                delete updatedPerson;
                refreshPersonList();
                QMessageBox::information(this, "Success", "Person updated successfully!");
            } else {
                QMessageBox::warning(this, "Error", "Failed to update person.");
                delete updatedPerson;
            }
        }
    }
}

void ManagePeopleDialog::onDeletePersonClicked() {
    QListWidgetItem *currentItem = peopleListWidget->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "No Selection", "Please select a person to delete.");
        return;
    }

    QString personId = currentItem->data(Qt::UserRole).toString();
    PERSON *person = personManager->findPersonById(personId.toStdString());

    if (person) {
        int ret = QMessageBox::question(
            this, "Delete Person",
            QString("Are you sure you want to delete %1?").arg(QString::fromStdString(person->getFullName())),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (ret == QMessageBox::Yes) {
            if (personManager->removePerson(personId.toStdString())) {
                refreshPersonList();
                QMessageBox::information(this, "Success", "Person deleted successfully!");
            } else {
                QMessageBox::critical(this, "Error", "Failed to delete person.");
            }
        }
    }
}

void ManagePeopleDialog::onViewPersonClicked() {
    QListWidgetItem *currentItem = peopleListWidget->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "No Selection", "Please select a person to view.");
        return;
    }

    QString personId = currentItem->data(Qt::UserRole).toString();
    PERSON *person = personManager->findPersonById(personId.toStdString());

    if (person) {
        showPersonDetailDialog(person);
    }
}

void ManagePeopleDialog::showPersonDetailDialog(PERSON *person) {
    QDialog *detailDialog = new QDialog(this);
    detailDialog->setWindowTitle(QString("👤 %1 - Details").arg(QString::fromStdString(person->getFullName())));
    detailDialog->setModal(true);
    detailDialog->setMinimumSize(700, 800);
    detailDialog->resize(900, 800);
    detailDialog->setStyleSheet("QDialog { background-color: #f8fbf8; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(detailDialog);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    QLabel *titleLabel = new QLabel(QString("👤 %1").arg(QString::fromStdString(person->getFullName())));
    titleLabel->setStyleSheet(
        "QLabel { "
        "    font-size: 20px; "
        "    font-weight: bold; "
        "    color: #27ae60; "
        "    padding: 10px 0; "
        "}");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFixedHeight(80);
    mainLayout->addWidget(titleLabel);

    QGroupBox *personalGroup = new QGroupBox("ℹ️ Personal Information");
    personalGroup->setFixedHeight(300);
    personalGroup->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 14px; "
        "    color: #27ae60; "
        "    border: 2px solid #a9dfbf; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 10px; "
        "    background-color: #fdfdfe; "
        "}"
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 15px; "
        "    padding: 0 5px 0 5px; "
        "    background-color: #fdfdfe; "
        "}");

    QFormLayout *personalLayout = new QFormLayout(personalGroup);
    personalLayout->setContentsMargins(15, 20, 15, 15);
    personalLayout->setSpacing(8);

    QString labelStyle = "QLabel { font-weight: bold; color: #2c3e50; }";
    QString valueStyle = "QLabel { color: #2c3e50; padding: 4px; background-color: #e8f8f5; border-radius: 4px; }";

    QLabel *idLabel = new QLabel("🆔 ID:");
    idLabel->setStyleSheet(labelStyle);
    QLabel *idValue = new QLabel(QString::fromStdString(person->getID()));
    idValue->setStyleSheet(valueStyle);
    personalLayout->addRow(idLabel, idValue);

    QLabel *nameLabel = new QLabel("👤 Name:");
    nameLabel->setStyleSheet(labelStyle);
    QLabel *nameValue = new QLabel(QString::fromStdString(person->getFullName()));
    nameValue->setStyleSheet(valueStyle);
    personalLayout->addRow(nameLabel, nameValue);

    QLabel *roleLabel = new QLabel("🏷️ Role:");
    roleLabel->setStyleSheet(labelStyle);
    QLabel *roleValue = new QLabel(QString::fromStdString(person->getRole()));
    roleValue->setStyleSheet(valueStyle);
    personalLayout->addRow(roleLabel, roleValue);

    QLabel *genderLabel = new QLabel("⚧ Gender:");
    genderLabel->setStyleSheet(labelStyle);
    QLabel *genderValue = new QLabel(QString::fromStdString(genderToString(person->getGender())));
    genderValue->setStyleSheet(valueStyle);
    personalLayout->addRow(genderLabel, genderValue);

    QLabel *dobLabel = new QLabel("🎂 Date of Birth:");
    dobLabel->setStyleSheet(labelStyle);
    QLabel *dobValue = new QLabel(QString::fromStdString(person->getDateOfBirth().toString()));
    dobValue->setStyleSheet(valueStyle);
    personalLayout->addRow(dobLabel, dobValue);

    QLabel *emailLabel = new QLabel("📧 Email:");
    emailLabel->setStyleSheet(labelStyle);
    QLabel *emailValue = new QLabel(QString::fromStdString(person->getEmail()));
    emailValue->setStyleSheet(valueStyle);
    personalLayout->addRow(emailLabel, emailValue);

    QLabel *phoneLabel = new QLabel("📞 Phone:");
    phoneLabel->setStyleSheet(labelStyle);
    QLabel *phoneValue = new QLabel(QString::fromStdString(person->getPhoneNumber()));
    phoneValue->setStyleSheet(valueStyle);
    personalLayout->addRow(phoneLabel, phoneValue);

    QLabel *addressLabel = new QLabel("🏠 Address:");
    addressLabel->setStyleSheet(labelStyle);
    QLabel *addressValue = new QLabel(QString::fromStdString(person->getAddress()));
    addressValue->setStyleSheet(valueStyle);
    personalLayout->addRow(addressLabel, addressValue);

    mainLayout->addWidget(personalGroup);

    MEMBER *member = dynamic_cast<MEMBER *>(person);
    HOST *host = dynamic_cast<HOST *>(person);

    if (member) {
        QGroupBox *spendingGroup = new QGroupBox("💰 Spending Information");
        spendingGroup->setStyleSheet(personalGroup->styleSheet());

        QVBoxLayout *spendingLayout = new QVBoxLayout(spendingGroup);
        spendingLayout->setContentsMargins(15, 20, 15, 15);
        spendingLayout->setSpacing(10);

        long long totalSpent = member->getTotalSpent();
        QLabel *totalLabel = new QLabel(QString("💵 Total Spent: %1").arg(formatCurrency(totalSpent)));
        totalLabel->setFixedHeight(50);
        totalLabel->setStyleSheet(
            "QLabel { "
            "    font-size: 16px; "
            "    font-weight: bold; "
            "    color: #27ae60; "
            "    padding: 8px; "
            "    background-color: #d5f4e6; "
            "    border-radius: 6px; "
            "    border: 1px solid #a9dfbf; "
            "}");
        spendingLayout->addWidget(totalLabel);

        QTableWidget *spendingTable = new QTableWidget();
        spendingTable->setColumnCount(5);
        QStringList headers = {"Date", "Trip", "Amount", "Category", "Note"};
        spendingTable->setHorizontalHeaderLabels(headers);

        spendingTable->setStyleSheet(
            "QTableWidget { "
            "    gridline-color: #d5f4e6; "
            "    background-color: #fdfdfe; "
            "    border: 2px solid #a9dfbf; "
            "    border-radius: 6px; "
            "    selection-background-color: #d5f4e6; "
            "    font-size: 12px; "
            "    outline: none; "
            "}"
            "QTableWidget::item { "
            "    padding: 15px 10px; "
            "    border-bottom: 1px solid #e8f8f5; "
            "    color: #2c3e50; "
            "    min-height: 30px;"
            "}"
            "QTableWidget::item:selected { "
            "    background-color: #d5f4e6; "
            "    color: #1e7e34; "
            "}"
            "QHeaderView::section { "
            "    background-color: #27ae60; "
            "    color: white; "
            "    font-weight: bold; "
            "    padding: 12px 10px; "
            "    border: 1px solid #229954; "
            "    min-height: 25px;"
            "}");

        vector<pair<string, EXPENSE>> spendings = member->getSpendings();

        spendingTable->setRowCount(spendings.size());

        for (size_t i = 0; i < spendings.size(); ++i) {
            const EXPENSE &expense = spendings[i].second;
            QString tripId = QString::fromStdString(spendings[i].first);

            spendingTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(expense.getDate().toString())));
            spendingTable->setItem(i, 1, new QTableWidgetItem(tripId));
            spendingTable->setItem(i, 2, new QTableWidgetItem(formatCurrency(expense.getAmount())));
            spendingTable->setItem(
                i, 3, new QTableWidgetItem(QString::fromStdString(categoryToString(expense.getCategory()))));
            spendingTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(expense.getNote())));
        }

        spendingTable->setColumnWidth(0, 100);
        spendingTable->setColumnWidth(1, 120);
        spendingTable->setColumnWidth(2, 120);
        spendingTable->setColumnWidth(3, 120);
        spendingTable->horizontalHeader()->setStretchLastSection(true);

        spendingTable->setAlternatingRowColors(true);
        spendingTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        spendingTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        spendingTable->verticalHeader()->setVisible(false);
        spendingTable->setMaximumHeight(500);

        spendingLayout->addWidget(spendingTable);
        mainLayout->addWidget(spendingGroup);

    } else if (host) {
        QGroupBox *tripGroup = new QGroupBox("🏠 Hosted Trips Information");
        tripGroup->setStyleSheet(personalGroup->styleSheet());

        QVBoxLayout *tripLayout = new QVBoxLayout(tripGroup);
        tripLayout->setContentsMargins(15, 20, 15, 15);
        tripLayout->setSpacing(10);

        vector<string> hostedTripIDs = host->getHostedTripIDs();

        QLabel *totalLabel = new QLabel(QString("🎯 Total Hosted Trips: %1").arg(hostedTripIDs.size()));
        totalLabel->setFixedHeight(50);
        totalLabel->setStyleSheet(
            "QLabel { "
            "    font-size: 16px; "
            "    font-weight: bold; "
            "    color: #27ae60; "
            "    padding: 8px; "
            "    background-color: #d5f4e6; "
            "    border-radius: 6px; "
            "    border: 1px solid #a9dfbf; "
            "}");
        tripLayout->addWidget(totalLabel);

        QTableWidget *tripsTable = new QTableWidget();
        tripsTable->setColumnCount(5);
        QStringList headers = {"Trip ID", "Destination", "Start Date", "End Date", "Status"};
        tripsTable->setHorizontalHeaderLabels(headers);

        tripsTable->setStyleSheet(
            "QTableWidget { "
            "    gridline-color: #d5f4e6; "
            "    background-color: #fdfdfe; "
            "    border: 2px solid #a9dfbf; "
            "    border-radius: 6px; "
            "    selection-background-color: #d5f4e6; "
            "    font-size: 12px; "
            "    outline: none; "
            "}"
            "QTableWidget::item { "
            "    padding: 15px 10px; "
            "    border-bottom: 1px solid #e8f8f5; "
            "    color: #2c3e50; "
            "    min-height: 30px;"
            "}"
            "QTableWidget::item:selected { "
            "    background-color: #d5f4e6; "
            "    color: #1e7e34; "
            "}"
            "QHeaderView::section { "
            "    background-color: #27ae60; "
            "    color: white; "
            "    font-weight: bold; "
            "    padding: 12px 10px; "
            "    border: 1px solid #229954; "
            "    min-height: 25px;"
            "}");

        QWidget *parentWidget = this->parentWidget();
        MainWindow *mainWindow = qobject_cast<MainWindow *>(parentWidget);

        if (!mainWindow) {
            QWidget *topLevel = parentWidget;
            while (topLevel && !qobject_cast<MainWindow *>(topLevel)) {
                topLevel = topLevel->parentWidget();
            }
            mainWindow = qobject_cast<MainWindow *>(topLevel);
        }

        vector<TRIP> hostedTrips;
        if (mainWindow && mainWindow->getTripManager()) {
            TRIPMANAGER *tripManager = mainWindow->getTripManager();
            vector<TRIP> allTrips = tripManager->getAllTrips();

            for (const TRIP &trip : allTrips) {
                for (const string &tripId : hostedTripIDs) {
                    if (trip.getID() == tripId) {
                        hostedTrips.push_back(trip);
                        break;
                    }
                }
            }
        }

        tripsTable->setRowCount(hostedTrips.size());

        for (size_t i = 0; i < hostedTrips.size(); ++i) {
            const TRIP &trip = hostedTrips[i];

            tripsTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(trip.getID())));
            tripsTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(trip.getDestination())));
            tripsTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(trip.getStartDate().toString())));
            tripsTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(trip.getEndDate().toString())));
            tripsTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(statusToString(trip.getStatus()))));
        }

        tripsTable->setColumnWidth(0, 120);
        tripsTable->setColumnWidth(1, 150);
        tripsTable->setColumnWidth(2, 120);
        tripsTable->setColumnWidth(3, 120);
        tripsTable->horizontalHeader()->setStretchLastSection(true);

        tripsTable->setAlternatingRowColors(true);
        tripsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        tripsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tripsTable->verticalHeader()->setVisible(false);
        tripsTable->setMaximumHeight(500);

        tripLayout->addWidget(tripsTable);
        mainLayout->addWidget(tripGroup);
    }

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *closeButton = new QPushButton("❌ Close");
    closeButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #95a5a6; "
        "    color: white; "
        "    border: none; "
        "    padding: 10px 20px; "
        "    border-radius: 6px; "
        "    font-size: 13px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { "
        "    background-color: #7f8c8d; "
        "}");

    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    mainLayout->addLayout(buttonLayout);

    connect(closeButton, &QPushButton::clicked, detailDialog, &QDialog::accept);

    detailDialog->move(this->geometry().center() - detailDialog->rect().center());

    detailDialog->exec();
    delete detailDialog;
}

QString ManagePeopleDialog::formatCurrency(long long amount) {
    QString amountStr = QString::number(amount);
    QString formatted;
    int count = 0;

    for (int i = amountStr.length() - 1; i >= 0; --i) {
        if (count > 0 && count % 3 == 0) {
            formatted.prepend('.');
        }
        formatted.prepend(amountStr[i]);
        count++;
    }

    return formatted + " VND";
}

void ManagePeopleDialog::onPersonSelectionChanged() {
    QListWidgetItem *currentItem = peopleListWidget->currentItem();
    bool hasSelection = (currentItem != nullptr);

    editPersonButton->setEnabled(hasSelection);
    deletePersonButton->setEnabled(hasSelection);
    viewPersonButton->setEnabled(hasSelection);

    updatePersonInfo();
}

void ManagePeopleDialog::onRefreshClicked() { refreshPersonList(); }

void ManagePeopleDialog::onCloseClicked() { accept(); }

void ManagePeopleDialog::onImportPeopleClicked() {
    QString filename = QFileDialog::getOpenFileName(this, "Import People from JSON", QDir::homePath(),
                                                    "JSON Files (*.json);;All Files (*.*)");

    if (filename.isEmpty()) {
        return;
    }

    try {
        vector<MEMBER> importedMembers;
        vector<HOST> importedHosts;

        importPeopleInfoFromJson(importedMembers, importedHosts, filename.toStdString());

        if (importedMembers.empty() && importedHosts.empty()) {
            QMessageBox::warning(this, "Import Failed", "No people found in the file or the file format is incorrect.");
            return;
        }

        int importCount = 0;

        for (const MEMBER &member : importedMembers) {
            if (!personManager->findMemberById(member.getID())) {
                personManager->addMember(member);
                importCount++;
            }
        }

        for (const HOST &host : importedHosts) {
            if (!personManager->findHostById(host.getID())) {
                personManager->addHost(host);
                importCount++;
            }
        }

        refreshPersonList();

        QMessageBox::information(this, "Import Successful",
                                 QString("Successfully imported %1 people.").arg(importCount));

    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Import Error", QString("An error occurred during import: %1").arg(e.what()));
    }
}

void ManagePeopleDialog::onExportPeopleClicked() {
    QString filename =
        QFileDialog::getSaveFileName(this, "Export People to JSON", QDir::homePath() + "/people_export.json",
                                     "JSON Files (*.json);;All Files (*.*)");

    if (filename.isEmpty()) {
        return;
    }

    try {
        const vector<MEMBER> &members = personManager->getAllMembers();
        const vector<HOST> &hosts = personManager->getAllHosts();

        if (members.empty() && hosts.empty()) {
            QMessageBox::warning(this, "Export Failed", "There are no people to export.");
            return;
        }

        exportPeopleInfoToJson(members, hosts, filename.toStdString());

        int totalPeople = members.size() + hosts.size();
        QMessageBox::information(
            this, "Export Successful",
            QString("Successfully exported %1 people to %2.").arg(totalPeople).arg(QFileInfo(filename).fileName()));

    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Export Error", QString("An error occurred during export: %1").arg(e.what()));
    }
}