#include "AddTripDialog.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QDateTime>
#include <QFont>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSpacerItem>
#include <QTextEdit>
#include <QVBoxLayout>

#include "Models/header.h"
#include "UI/AddPersonDialog.h"

Q_DECLARE_METATYPE(HOST *)
Q_DECLARE_METATYPE(MEMBER *)

AddTripDialog::AddTripDialog(QWidget *parent) : QDialog(parent), personManager(nullptr) {
    setupUI();
    setWindowTitle("Add New Trip");
    setModal(true);

    setMinimumSize(1300, 800);
    resize(1400, 800);

    if (parent) {
        move(parent->geometry().center() - rect().center());
    }
}

AddTripDialog::AddTripDialog(PERSONMANAGER *personManager, QWidget *parent)
    : QDialog(parent), personManager(personManager) {
    setupUI();
    setWindowTitle("Add New Trip");
    setModal(true);

    setFixedSize(1200, 800);

    if (parent) {
        move(parent->geometry().center() - rect().center());
    }
}

void AddTripDialog::setupUI() {
    QFont labelFont;
    labelFont.setPointSize(13);
    labelFont.setBold(true);

    QFont inputFont;
    inputFont.setPointSize(10);

    destinationLineEdit = new QLineEdit(this);
    destinationLineEdit->setPlaceholderText("Enter destination (e.g., Paris, Tokyo)...");
    destinationLineEdit->setMinimumHeight(35);
    destinationLineEdit->setFont(inputFont);
    destinationLineEdit->setStyleSheet(
        "QLineEdit { "
        "    padding: 8px 12px; "
        "    border: 2px solid #ddd; "
        "    border-radius: 6px; "
        "    font-size: 15px; "
        "}"
        "QLineEdit:focus { "
        "    border-color: #4CAF50; "
        "    background-color: #f9f9f9; "
        "}");

    descriptionTextEdit = new QTextEdit(this);
    descriptionTextEdit->setMaximumHeight(200);
    descriptionTextEdit->setPlaceholderText("Enter trip description...");
    descriptionTextEdit->setFont(inputFont);
    descriptionTextEdit->setStyleSheet(
        "QTextEdit { "
        "    padding: 8px 12px; "
        "    border: 2px solid #ddd; "
        "    border-radius: 6px; "
        "    font-size: 15px; "
        "}"
        "QTextEdit:focus { "
        "    border-color: #4CAF50; "
        "    background-color: #f9f9f9; "
        "}");

    startDateEdit = new QDateEdit(QDate::currentDate(), this);
    startDateEdit->setCalendarPopup(true);
    startDateEdit->setDisplayFormat("dd/MM/yyyy");
    startDateEdit->setMinimumHeight(35);
    startDateEdit->setFont(inputFont);
    startDateEdit->setStyleSheet(
        "QDateEdit { "
        "    padding: 8px 12px; "
        "    border: 2px solid #ddd; "
        "    border-radius: 6px; "
        "    font-size: 15px; "
        "}"
        "QDateEdit:focus { border-color: #4CAF50; }");

    endDateEdit = new QDateEdit(QDate::currentDate().addDays(1), this);
    endDateEdit->setCalendarPopup(true);
    endDateEdit->setDisplayFormat("dd/MM/yyyy");
    endDateEdit->setMinimumHeight(35);
    endDateEdit->setFont(inputFont);
    endDateEdit->setStyleSheet(
        "QDateEdit { "
        "    padding: 8px 12px; "
        "    border: 2px solid #ddd; "
        "    border-radius: 6px; "
        "    font-size: 15px; "
        "}"
        "QDateEdit:focus { border-color: #4CAF50; }");

    statusComboBox = new QComboBox(this);
    statusComboBox->addItem("🗓️ Planned");
    statusComboBox->addItem("🚀 Ongoing");
    statusComboBox->addItem("✅ Completed");
    statusComboBox->addItem("❌ Cancelled");
    statusComboBox->setMinimumHeight(35);
    statusComboBox->setFont(inputFont);
    statusComboBox->setStyleSheet(
        "QComboBox { "
        "    padding: 8px 12px; "
        "    border: 2px solid #ddd; "
        "    border-radius: 6px; "
        "    font-size: 15px; "
        "}"
        "QComboBox:focus { border-color: #4CAF50; }");

    setupPeopleSelection();

    QFormLayout *tripInfoLayout = new QFormLayout();
    tripInfoLayout->setVerticalSpacing(12);
    tripInfoLayout->setHorizontalSpacing(10);
    tripInfoLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QLabel *destLabel = new QLabel("🌍 Destination:");
    QLabel *descLabel = new QLabel("📝 Description:");
    QLabel *startLabel = new QLabel("📅 Start Date:");
    QLabel *endLabel = new QLabel("📅 End Date:");
    QLabel *statusLabel = new QLabel("📊 Status:");

    QString labelStyle =
        "QLabel { "
        "    font-weight: bold; "
        "    font-size: 18px; "
        "    color: #333; "
        "    padding-right: 8px; "
        "}";

    destLabel->setStyleSheet(labelStyle);
    descLabel->setStyleSheet(labelStyle);
    startLabel->setStyleSheet(labelStyle);
    endLabel->setStyleSheet(labelStyle);
    statusLabel->setStyleSheet(labelStyle);

    tripInfoLayout->addRow(destLabel, destinationLineEdit);
    tripInfoLayout->addRow(descLabel, descriptionTextEdit);
    tripInfoLayout->addRow(startLabel, startDateEdit);
    tripInfoLayout->addRow(endLabel, endDateEdit);
    tripInfoLayout->addRow(statusLabel, statusComboBox);

    QGroupBox *tripInfoGroupBox = new QGroupBox("Trip Information");
    tripInfoGroupBox->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 15px; "
        "    color: #2c3e50; "
        "    border: 2px solid #bdc3c7; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 10px; "
        "}"
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 10px; "
        "    padding: 0 5px 0 5px; "
        "}");
    tripInfoGroupBox->setLayout(tripInfoLayout);
    tripInfoGroupBox->setFixedWidth(505);
    tripInfoGroupBox->setMaximumHeight(650);

    okButton = new QPushButton("✅ Add Trip", this);
    cancelButton = new QPushButton("❌ Cancel", this);

    okButton->setMinimumHeight(40);
    okButton->setMinimumWidth(110);
    okButton->setFont(QFont("Arial", 10, QFont::Bold));
    okButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #4CAF50; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 8px; "
        "    padding: 10px 20px; "
        "    font-size: 13px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #45a049; }"
        "QPushButton:pressed { background-color: #3d8b40; }");

    cancelButton->setMinimumHeight(40);
    cancelButton->setMinimumWidth(110);
    cancelButton->setFont(QFont("Arial", 10, QFont::Bold));
    cancelButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #f44336; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 8px; "
        "    padding: 10px 20px; "
        "    font-size: 13px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #da190b; }"
        "QPushButton:pressed { background-color: #b71c1c; }");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addSpacing(10);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();

    QVBoxLayout *leftPanelLayout = new QVBoxLayout();
    leftPanelLayout->addWidget(tripInfoGroupBox);
    leftPanelLayout->addLayout(buttonLayout);
    leftPanelLayout->addStretch();

    QWidget *leftPanelWidget = new QWidget();
    leftPanelWidget->setLayout(leftPanelLayout);
    leftPanelWidget->setFixedWidth(520);

    QVBoxLayout *rightPanelLayout = new QVBoxLayout();
    rightPanelLayout->setContentsMargins(0, 0, 0, 0);
    rightPanelLayout->setSpacing(10);

    hostGroupBox->setMaximumHeight(280);

    membersGroupBox->setMinimumHeight(400);

    rightPanelLayout->addWidget(hostGroupBox, 2);
    rightPanelLayout->addWidget(membersGroupBox, 3);

    QWidget *rightPanelWidget = new QWidget();
    rightPanelWidget->setLayout(rightPanelLayout);
    rightPanelWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout *mainHorizontalLayout = new QHBoxLayout();
    mainHorizontalLayout->setSpacing(20);
    mainHorizontalLayout->addWidget(leftPanelWidget);
    mainHorizontalLayout->addWidget(rightPanelWidget, 1);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    QLabel *titleLabel = new QLabel("Add New Trip", this);
    titleLabel->setStyleSheet(
        "QLabel { "
        "    font-size: 18px; "
        "    font-weight: bold; "
        "    color: #2c3e50; "
        "    padding: 8px 0; "
        "    text-align: center; "
        "}");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setMaximumHeight(40);

    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(mainHorizontalLayout, 1);

    setStyleSheet(
        "QDialog { "
        "    background-color: #fafafa; "
        "    border: 1px solid #ddd; "
        "}");

    connect(okButton, &QPushButton::clicked, this, &AddTripDialog::acceptDialog);
    connect(cancelButton, &QPushButton::clicked, this, &AddTripDialog::rejectDialog);

    okButton->setDefault(true);

    destinationLineEdit->setFocus();
}

void AddTripDialog::setupPeopleSelection() {
    hostGroupBox = new QGroupBox("🏠 Select Host");
    hostGroupBox->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 15px; "
        "    color: #2c3e50; "
        "    border: 2px solid #e67e22; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 10px; "
        "}"
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 10px; "
        "    padding: 0 5px 0 5px; "
        "}");

    QVBoxLayout *hostLayout = new QVBoxLayout(hostGroupBox);
    hostLayout->setSpacing(8);

    selectedHostLabel = new QLabel("❌ No host selected");
    selectedHostLabel->setStyleSheet(
        "QLabel { "
        "    color: #e74c3c; "
        "    font-style: italic; "
        "    font-size: 13px; "
        "    padding: 4px; "
        "}");

    hostSearchLayout = new QHBoxLayout();

    hostSearchBar = new QTextEdit();
    hostSearchBar->setPlaceholderText("Search hosts...");
    hostSearchBar->setMaximumHeight(45);
    hostSearchBar->setStyleSheet(R"(
                                 padding: 8px 12px;
                              )");

    clearHostSearchButton = new QPushButton("Clear");
    clearHostSearchButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #95a5a6; "
        "    color: white; "
        "    border: none; "
        "    padding: 6px 12px; "
        "    border-radius: 4px; "
        "    font-size: 11px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #7f8c8d; }");

    hostSearchLayout->addWidget(hostSearchBar);
    hostSearchLayout->addWidget(clearHostSearchButton);

    hostsListWidget = new QListWidget();
    hostsListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    hostsListWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    hostsListWidget->setStyleSheet(
        "QListWidget { "
        "    border: 2px solid #ddd; "
        "    border-radius: 5px; "
        "    font-size: 13px; "
        "    background-color: white; "
        "}"
        "QListWidget::item { "
        "    padding: 8px; "
        "    border-bottom: 1px solid #eee; "
        "}"
        "QListWidget::item:hover { "
        "    background-color: #ecf0f1; "
        "}"
        "QListWidget::item:selected { "
        "    background-color: #e67e22; "
        "    color: white; "
        "}");

    QPushButton *addHostButton = new QPushButton("ADD NEW HOST");
    addHostButton->setStyleSheet(R"(
        QPushButton {
            width: 100px;
                                 }
                                 )");

    hostLayout->addWidget(selectedHostLabel);
    hostLayout->addLayout(hostSearchLayout);
    hostLayout->addWidget(hostsListWidget, 1);
    hostLayout->addWidget(addHostButton);

    membersGroupBox = new QGroupBox("👥 Select Members");
    membersGroupBox->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 15px; "
        "    color: #2c3e50; "
        "    border: 2px solid #3498db; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 10px; "
        "}"
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 10px; "
        "    padding: 0 5px 0 5px; "
        "}");

    QVBoxLayout *membersLayout = new QVBoxLayout(membersGroupBox);
    membersLayout->setSpacing(10);

    selectedMembersLabel = new QLabel("No members selected");
    selectedMembersLabel->setStyleSheet(
        "QLabel { "
        "    color: #7f8c8d; "
        "    font-style: italic; "
        "    font-size: 13px; "
        "    padding: 4px; "
        "}");

    QHBoxLayout *memberButtonsLayout = new QHBoxLayout();
    selectAllMembersButton = new QPushButton("Select All");
    clearAllMembersButton = new QPushButton("Clear All");

    selectAllMembersButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #3498db; "
        "    color: white; "
        "    border: none; "
        "    padding: 6px 12px; "
        "    border-radius: 4px; "
        "    font-size: 11px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #2980b9; }");

    clearAllMembersButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #95a5a6; "
        "    color: white; "
        "    border: none; "
        "    padding: 6px 12px; "
        "    border-radius: 4px; "
        "    font-size: 11px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #7f8c8d; }");

    memberButtonsLayout->addWidget(selectAllMembersButton);
    memberButtonsLayout->addWidget(clearAllMembersButton);
    memberButtonsLayout->addStretch();

    memberSearchLayout = new QHBoxLayout();

    memberSearchBar = new QTextEdit();
    memberSearchBar->setPlaceholderText("Search members...");
    memberSearchBar->setMaximumHeight(45);
    memberSearchBar->setStyleSheet(R"(
                                   padding: 8px 12px;
                                )");

    clearMemberSearchButton = new QPushButton("Clear");
    clearMemberSearchButton->setStyleSheet(R"(
                                           QPushButton { 
                                               background-color: #95a5a6; 
                                               color: white; 
                                               border: none; 
                                               padding: 6px 12px; 
                                               border-radius: 4px; 
                                               font-size: 11px; 
                                               font-weight: bold; 
                                           }
                                           QPushButton:hover { background-color: #7f8c8d; }
                                           )");

    memberSearchLayout->addWidget(memberSearchBar);
    memberSearchLayout->addWidget(clearMemberSearchButton);

    membersListWidget = new QListWidget();
    membersListWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    membersListWidget->setStyleSheet(
        "QListWidget { "
        "    border: 2px solid #ddd; "
        "    border-radius: 5px; "
        "    font-size: 13px; "
        "    background-color: white; "
        "}"
        "QListWidget::item { "
        "    padding: 8px; "
        "    border-bottom: 1px solid #eee; "
        "}"
        "QListWidget::item:hover { "
        "    background-color: #ecf0f1; "
        "}"
        "QListWidget::item:selected { "
        "    background-color: #3498db; "
        "    color: white; "
        "}");

    QPushButton *addMemberButton = new QPushButton("ADD NEW MEMBER");
    addMemberButton->setStyleSheet(R"(
        width: 100px;
                                   )");

    membersLayout->addWidget(selectedMembersLabel);
    membersLayout->addLayout(memberButtonsLayout);
    membersLayout->addLayout(memberSearchLayout);
    membersLayout->addWidget(membersListWidget, 1);
    membersLayout->addWidget(addMemberButton);

    connect(hostsListWidget, &QListWidget::itemSelectionChanged, this, &AddTripDialog::onHostSelectionChanged);
    connect(hostSearchBar, &QTextEdit::textChanged, this, &AddTripDialog::updateHostList);
    connect(clearHostSearchButton, &QPushButton::clicked, [this]() {
        hostSearchBar->clear();
        updateHostList();
    });

    connect(membersListWidget, &QListWidget::itemChanged, this, &AddTripDialog::onMemberSelectionChanged);
    connect(membersListWidget, &QListWidget::itemDoubleClicked, this, [](QListWidgetItem *item) {
        item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
    });
    connect(selectAllMembersButton, &QPushButton::clicked, this, &AddTripDialog::onSelectAllMembers);
    connect(clearAllMembersButton, &QPushButton::clicked, this, &AddTripDialog::onClearAllMembers);
    connect(memberSearchBar, &QTextEdit::textChanged, this, &AddTripDialog::updateMemberList);
    connect(clearMemberSearchButton, &QPushButton::clicked, [this]() {
        memberSearchBar->clear();
        updateMemberList();
    });
    connect(addHostButton, &QPushButton::clicked, this, &AddTripDialog::onAddHostClicked);
    connect(addMemberButton, &QPushButton::clicked, this, &AddTripDialog::onAddMemberClicked);

    updateHostList();
    updateMemberList();
}

void AddTripDialog::updateMemberList() {
    if (!personManager) {
        QListWidgetItem *disabledItem = new QListWidgetItem("No people available - Please add people first");
        disabledItem->setFlags(Qt::NoItemFlags);
        membersListWidget->addItem(disabledItem);

        membersListWidget->setEnabled(false);
        selectAllMembersButton->setEnabled(false);
        clearAllMembersButton->setEnabled(false);
        return;
    }

    for (int i = 0; i < membersListWidget->count(); ++i) {
        QListWidgetItem *item = membersListWidget->item(i);
        if (item && item->data(Qt::CheckStateRole).toInt() == Qt::Checked) {
            QString memberID = item->data(Qt::UserRole).toString();
            if (!memberID.isEmpty()) {
                selectedMemberIDs.insert(memberID);
            }
        } else if (item) {
            QString memberID = item->data(Qt::UserRole).toString();
            if (!memberID.isEmpty()) {
                selectedMemberIDs.remove(memberID);
            }
        }
    }

    membersListWidget->clear();

    vector<MEMBER> members = personManager->getAllMembers();
    QString memberSearchName = memberSearchBar->toPlainText().trimmed();

    for (const MEMBER &member : members) {
        QString memberInfo = QString("%1 (ID: %2)")
                                 .arg(QString::fromStdString(member.getFullName()))
                                 .arg(QString::fromStdString(member.getID()));

        if (!memberSearchName.isEmpty() && !memberInfo.contains(memberSearchName, Qt::CaseInsensitive)) {
            continue;
        }

        QListWidgetItem *item = new QListWidgetItem(memberInfo);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);

        QString memberID = QString::fromStdString(member.getID());
        item->setData(Qt::UserRole, memberID);

        if (selectedMemberIDs.contains(memberID)) {
            item->setData(Qt::CheckStateRole, Qt::Checked);
        } else {
            item->setData(Qt::CheckStateRole, Qt::Unchecked);
        }

        membersListWidget->addItem(item);
    }

    updateSelectedCounts();
}

void AddTripDialog::updateHostList() {
    if (!personManager) {
        QListWidgetItem *disabledItem = new QListWidgetItem("No people available - Please add people first");
        disabledItem->setFlags(Qt::NoItemFlags);
        hostsListWidget->addItem(disabledItem);
        hostsListWidget->setEnabled(false);
        return;
    }

    QString selectedHostID;
    QListWidgetItem *currentItem = hostsListWidget->currentItem();
    if (currentItem) {
        selectedHostID = currentItem->data(Qt::UserRole).toString();
    }

    hostsListWidget->clear();

    vector<HOST> hosts = personManager->getAllHosts();
    QString hostSearchName = hostSearchBar->toPlainText().trimmed();

    for (const HOST &host : hosts) {
        QString hostInfo = QString("%1 (ID: %2)")
                               .arg(QString::fromStdString(host.getFullName()))
                               .arg(QString::fromStdString(host.getID()));

        if (!hostSearchName.isEmpty() && !hostInfo.contains(hostSearchName, Qt::CaseInsensitive)) {
            continue;
        }

        QListWidgetItem *item = new QListWidgetItem(hostInfo);
        QString hostID = QString::fromStdString(host.getID());
        item->setData(Qt::UserRole, hostID);

        hostsListWidget->addItem(item);

        if (hostID == selectedHostID) {
            hostsListWidget->setCurrentItem(item);
        }
    }

    updateSelectedCounts();
}

void AddTripDialog::onHostSelectionChanged() { updateSelectedCounts(); }

void AddTripDialog::onMemberSelectionChanged() {
    for (int i = 0; i < membersListWidget->count(); ++i) {
        QListWidgetItem *item = membersListWidget->item(i);
        if (item && item->data(Qt::CheckStateRole).toInt() == Qt::Checked) {
            QString memberID = item->data(Qt::UserRole).toString();
            if (!memberID.isEmpty()) {
                selectedMemberIDs.insert(memberID);
            }
        }
    }

    updateSelectedCounts();
}

void AddTripDialog::onSelectAllMembers() {
    for (int i = 0; i < membersListWidget->count(); ++i) {
        QListWidgetItem *item = membersListWidget->item(i);
        if (item && item->flags() & Qt::ItemIsUserCheckable) {
            item->setData(Qt::CheckStateRole, Qt::Checked);
            QString memberID = item->data(Qt::UserRole).toString();
            if (!memberID.isEmpty()) {
                selectedMemberIDs.insert(memberID);
            }
        }
    }
    updateSelectedCounts();
}

void AddTripDialog::onClearAllMembers() {
    for (int i = 0; i < membersListWidget->count(); ++i) {
        QListWidgetItem *item = membersListWidget->item(i);
        if (item && item->flags() & Qt::ItemIsUserCheckable) {
            item->setData(Qt::CheckStateRole, Qt::Unchecked);
            QString memberID = item->data(Qt::UserRole).toString();
            if (!memberID.isEmpty()) {
                selectedMemberIDs.remove(memberID);
            }
        }
    }
    updateSelectedCounts();
}

void AddTripDialog::updateSelectedCounts() {
    QListWidgetItem *selectedHostItem = hostsListWidget->currentItem();
    if (selectedHostItem) {
        QString hostName = selectedHostItem->text();
        selectedHostLabel->setText(QString("✅ Host selected: %1").arg(hostName));
        selectedHostLabel->setStyleSheet("color: #27ae60; font-weight: bold;");
    } else {
        selectedHostLabel->setText("❌ No host selected");
        selectedHostLabel->setStyleSheet("color: #e74c3c; font-style: italic;");
    }

    int totalSelectedCount = selectedMemberIDs.size();
    int displayedSelectedCount = 0;

    for (int i = 0; i < membersListWidget->count(); ++i) {
        QListWidgetItem *item = membersListWidget->item(i);
        if (item && item->data(Qt::CheckStateRole).toInt() == Qt::Checked) {
            displayedSelectedCount++;
        }
    }

    if (totalSelectedCount > 0) {
        QString searchText = memberSearchBar->toPlainText().trimmed();
        if (!searchText.isEmpty() && displayedSelectedCount != totalSelectedCount) {
            selectedMembersLabel->setText(
                QString("✅ %1 of %2 selected member(s) shown").arg(displayedSelectedCount).arg(totalSelectedCount));
        } else {
            selectedMembersLabel->setText(QString("✅ %1 member(s) selected").arg(totalSelectedCount));
        }
        selectedMembersLabel->setStyleSheet("color: #27ae60; font-weight: bold;");
    } else {
        selectedMembersLabel->setText("❌ No members selected");
        selectedMembersLabel->setStyleSheet("color: #e74c3c; font-style: italic;");
    }
}

bool AddTripDialog::validatePeopleSelection() {
    if (personManager && !hostsListWidget->currentItem()) {
        QMessageBox::warning(this, "Host Required",
                             "Please select a host for the trip!\n\n"
                             "Every trip must have a host.");
        return false;
    }

    int selectedMembers = 0;
    for (int i = 0; i < membersListWidget->count(); ++i) {
        QListWidgetItem *item = membersListWidget->item(i);
        if (item && item->data(Qt::CheckStateRole).toInt() == Qt::Checked) {
            selectedMembers++;
        }
    }

    if (personManager && selectedMembers == 0) {
        QMessageBox::warning(this, "Members Required",
                             "Please select at least one member for the trip!\n\n"
                             "A trip needs participants.");
        return false;
    }

    return true;
}

void AddTripDialog::acceptDialog() {
    if (destinationLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error",
                             "Destination cannot be empty!\n\nPlease enter a destination for your trip.");
        destinationLineEdit->setFocus();
        return;
    } else if (destinationLineEdit->text().trimmed().length() < 3 ||
               destinationLineEdit->text().trimmed().length() > 10) {
        QMessageBox::warning(this, "Validation Error",
                             "Destination must be between 3 and 10 characters!\n\nPlease enter a valid destination.");
        destinationLineEdit->setFocus();
        return;
    } else {
        string text = destinationLineEdit->text().trimmed().toUpper().toStdString();
        for (char c : text) {
            if (!isspace(c) && !isalpha(c)) {
                QMessageBox::warning(this, "Validation Error",
                                     "Destination can only contain uppercase letters and digits!\n\n"
                                     "Please enter a valid destination.");
                destinationLineEdit->setFocus();
                return;
            }
        }
    }

    if (descriptionTextEdit->toPlainText().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error",
                             "Description cannot be empty!\n\nPlease enter a description for your trip.");
        descriptionTextEdit->setFocus();
        return;
    } else if (descriptionTextEdit->toPlainText().trimmed().length() < 0 ||
               descriptionTextEdit->toPlainText().trimmed().length() > 200) {
        QMessageBox::warning(
            this, "Validation Error",
            "Description must be between 0 and 200 characters!\n\nPlease enter a valid description for your trip.");
        descriptionTextEdit->setFocus();
        return;
    }

    QDate startDate = startDateEdit->date();
    QDate endDate = endDateEdit->date();

    if (endDate < startDate) {
        QMessageBox::warning(this, "Date Validation Error",
                             "End date cannot be earlier than start date!\n\nPlease check your dates.");
        startDateEdit->setFocus();
        return;
    }

    if (!validatePeopleSelection()) {
        return;
    }

    DATE startDateObj(startDate.day(), startDate.month(), startDate.year());
    DATE endDateObj(endDate.day(), endDate.month(), endDate.year());

    QString statusText = statusComboBox->currentText();
    QString cleanStatus = statusText.mid(2).trimmed();
    STATUS tripStatus = stringToStatus(cleanStatus.toStdString());

    try {
        std::string destination = destinationLineEdit->text().trimmed().toUpper().toStdString();
        std::string description = descriptionTextEdit->toPlainText().trimmed().toStdString();

        vector<EXPENSE> expenses;
        long long totalExpense = 0;
        _tripData = TRIPFACTORY::createTrip(destination, description, startDateObj, endDateObj, tripStatus, expenses,
                                            totalExpense);

        if (personManager) {
            QListWidgetItem *selectedHostItem = hostsListWidget->currentItem();
            if (selectedHostItem) {
                QString hostID = selectedHostItem->data(Qt::UserRole).toString();
                if (!hostID.isEmpty()) {
                    HOST selectedHost = personManager->getHostByID(hostID.toStdString());
                    if (!selectedHost.getID().empty()) {
                        _tripData.setHost(selectedHost);
                        selectedHost.hostTrip(_tripData.getID());
                    }

                    personManager->updateHost(selectedHost, selectedHost);
                }
            }

            for (int i = 0; i < membersListWidget->count(); ++i) {
                QListWidgetItem *item = membersListWidget->item(i);
                if (item && item->data(Qt::CheckStateRole).toInt() == Qt::Checked) {
                    QString memberID = item->data(Qt::UserRole).toString();
                    if (!memberID.isEmpty()) {
                        MEMBER selectedMember = personManager->getMemberByID(memberID.toStdString());
                        if (!selectedMember.getID().empty()) {
                            _tripData.addMember(selectedMember);
                            selectedMember.joinTrip(_tripData.getID());

                            personManager->updateMember(selectedMember, selectedMember);
                        }
                    }
                }
            }
        }

        accept();

    } catch (const std::exception &e) {
        QMessageBox::critical(
            this, "Error",
            QString("Failed to create trip: %1\n\nPlease check your input and try again.").arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Error",
                              "Unknown error occurred while creating trip.\n\nPlease check your input and try again.");
    }
}

void AddTripDialog::rejectDialog() { reject(); }

TRIP AddTripDialog::getTripData() const { return _tripData; }

void AddTripDialog::onAddMemberClicked() {
    AddPersonDialog dialog(personManager, this);
    dialog.setPersonType("Member");
    if (dialog.exec() == QDialog::Accepted) {
        PERSON *newPerson = dialog.getPersonData();
        if (newPerson) {
            personManager->addPerson(*newPerson);
            delete newPerson;
            QMessageBox::information(this, "Success", "Person added successfully!");
        }
        updateMemberList();
    }
}

void AddTripDialog::onAddHostClicked() {
    AddPersonDialog dialog(personManager, this);
    dialog.setPersonType("Host");
    if (dialog.exec() == QDialog::Accepted) {
        PERSON *newPerson = dialog.getPersonData();
        if (newPerson) {
            personManager->addPerson(*newPerson);
            delete newPerson;
            QMessageBox::information(this, "Success", "Person added successfully!");
        }
        updateHostList();
    }
}
