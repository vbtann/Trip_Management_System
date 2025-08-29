#include "ViewTripDialog.h"

ViewTripDialog::ViewTripDialog(TRIP &_trip, PERSONMANAGER *personManager, QWidget *parent)
    : QDialog(parent), trip(_trip), originalTrip(_trip), personManager(personManager) {
    setWindowTitle("View Trip Details - " + QString::fromStdString(trip.getDestination()));
    setWindowIcon(QIcon(":/icons/view.png"));
    setModal(true);

    setMinimumSize(1200, 700);
    resize(1500, 900);

    setupUI();
    displayCurrentTrip();
    displayPeopleInfo();

    QRect screenGeometry = QApplication::primaryScreen()->geometry();
    move(screenGeometry.center() - rect().center());
}

void ViewTripDialog::setPersonManager(PERSONMANAGER *manager) {
    personManager = manager;

    if (personManager) {
        displayPeopleInfo();
    }
}

void ViewTripDialog::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(15);

    QLabel *titleLabel = new QLabel("Trip Details", this);
    titleLabel->setStyleSheet(
        "QLabel { "
        "    font-size: 20px; "
        "    font-weight: bold; "
        "    color: #2c3e50; "
        "    padding: 10px 0; "
        "    text-align: center; "
        "}");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setMaximumHeight(50);
    mainLayout->addWidget(titleLabel);

    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(20);

    setupTripInfoSection();
    setupExpenseSection();
    QWidget *leftPanelWidget = new QWidget();
    QVBoxLayout *leftPanelLayout = new QVBoxLayout(leftPanelWidget);
    leftPanelLayout->setSpacing(10);
    leftPanelLayout->setContentsMargins(0, 0, 0, 0);
    leftPanelLayout->addWidget(tripInfoGroupBox, 1);
    leftPanelLayout->addWidget(expenseInfoGroupBox, 1);
    leftPanelWidget->setFixedWidth(700);

    setupPeopleSection();
    QWidget *rightPanelWidget = new QWidget();
    rightPanelWidget->setLayout(new QVBoxLayout());
    rightPanelWidget->layout()->addWidget(peopleInfoGroupBox);
    rightPanelWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    contentLayout->addWidget(leftPanelWidget);
    contentLayout->addWidget(rightPanelWidget, 1);

    mainLayout->addLayout(contentLayout, 1);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);

    editButton = new QPushButton("✏️ Edit Trip", this);
    closeButton = new QPushButton("❌ Close", this);

    editButton->setMinimumHeight(40);
    editButton->setMinimumWidth(120);
    editButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #3498db; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 8px; "
        "    padding: 10px 20px; "
        "    font-size: 13px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #2980b9; }"
        "QPushButton:pressed { background-color: #21618c; }");

    closeButton->setMinimumHeight(40);
    closeButton->setMinimumWidth(120);
    closeButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #e74c3c; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 8px; "
        "    padding: 10px 20px; "
        "    font-size: 13px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #c0392b; }"
        "QPushButton:pressed { background-color: #a93226; }");

    buttonLayout->addStretch();
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(closeButton);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);

    connect(editButton, &QPushButton::clicked, this, &ViewTripDialog::editCurrentTrip);
    connect(closeButton, &QPushButton::clicked, this, &ViewTripDialog::closeView);
    connect(hostInfoButton, &QPushButton::clicked, this, &ViewTripDialog::onHostInfoClicked);
    connect(membersInfoButton, &QPushButton::clicked, this, &ViewTripDialog::onMemberInfoClicked);
    connect(membersListWidget, &QListWidget::itemDoubleClicked, this, &ViewTripDialog::onMemberInfoClicked);
    connect(viewExpensesButton, &QPushButton::clicked, this, &ViewTripDialog::onViewExpensesClicked);
    connect(expenseListWidget, &QListWidget::itemDoubleClicked, this, &ViewTripDialog::onViewExpensesClicked);
    connect(addExpenseButton, &QPushButton::clicked, this, &ViewTripDialog::onAddExpenseClicked);

    closeButton->setDefault(true);
}

void ViewTripDialog::setupTripInfoSection() {
    tripInfoGroupBox = new QGroupBox("🌍 Trip Information");
    tripInfoGroupBox->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 16px; "
        "    color: #2c3e50; "
        "    border: 2px solid #3498db; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 15px; "
        "}"
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 10px; "
        "    padding: 0 5px 0 5px; "
        "}");

    QFormLayout *tripLayout = new QFormLayout(tripInfoGroupBox);
    tripLayout->setVerticalSpacing(10);
    tripLayout->setHorizontalSpacing(15);
    tripLayout->setLabelAlignment(Qt::AlignRight);

    idLabel = new QLabel();
    destinationLabel = new QLabel();
    descriptionDisplay = new QTextEdit();
    startDateLabel = new QLabel();
    endDateLabel = new QLabel();
    statusLabel = new QLabel();

    descriptionDisplay->setReadOnly(true);
    descriptionDisplay->setMaximumHeight(120);
    descriptionDisplay->setStyleSheet(
        "QTextEdit { "
        "    border: 2px solid #ddd; "
        "    border-radius: 5px; "
        "    padding: 8px; "
        "    background-color: #f8f9fa; "
        "    font-size: 13px; "
        "}");

    QString labelStyle =
        "QLabel { "
        "    font-size: 14px; "
        "    padding: 8px; "
        "    background-color: #f8f9fa; "
        "    border: 2px solid #ddd; "
        "    border-radius: 5px; "
        "    min-height: 20px; "
        "}";

    idLabel->setStyleSheet(labelStyle);
    destinationLabel->setStyleSheet(labelStyle);
    startDateLabel->setStyleSheet(labelStyle);
    endDateLabel->setStyleSheet(labelStyle);
    statusLabel->setStyleSheet(labelStyle);

    QLabel *idLabelText = new QLabel("🆔 Trip ID:");
    QLabel *destLabelText = new QLabel("📍 Destination:");
    QLabel *descLabelText = new QLabel("📝 Description:");
    QLabel *startLabelText = new QLabel("📅 Start Date:");
    QLabel *endLabelText = new QLabel("📅 End Date:");
    QLabel *statusLabelText = new QLabel("📊 Status:");

    QString formLabelStyle = "QLabel { font-weight: bold; font-size: 13px; color: #2c3e50; }";
    idLabelText->setStyleSheet(formLabelStyle);
    destLabelText->setStyleSheet(formLabelStyle);
    descLabelText->setStyleSheet(formLabelStyle);
    startLabelText->setStyleSheet(formLabelStyle);
    endLabelText->setStyleSheet(formLabelStyle);
    statusLabelText->setStyleSheet(formLabelStyle);

    tripLayout->addRow(idLabelText, idLabel);
    tripLayout->addRow(destLabelText, destinationLabel);
    tripLayout->addRow(descLabelText, descriptionDisplay);
    tripLayout->addRow(startLabelText, startDateLabel);
    tripLayout->addRow(endLabelText, endDateLabel);
    tripLayout->addRow(statusLabelText, statusLabel);
}

void ViewTripDialog::setupExpenseSection() {
    expenseInfoGroupBox = new QGroupBox("💰 Expense Information");
    expenseInfoGroupBox->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 16px; "
        "    color: #2c3e50; "
        "    border: 2px solid #27ae60; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 15px; "
        "}"
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 10px; "
        "    padding: 0 5px 0 5px; "
        "}");

    QVBoxLayout *expenseLayout = new QVBoxLayout(expenseInfoGroupBox);
    expenseLayout->setSpacing(15);
    expenseLayout->setContentsMargins(15, 15, 15, 15);

    totalExpenseLabel = new QLabel();
    QString labelStyle =
        "QLabel { "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "    padding: 10px; "
        "    background-color: #e8f8f5; "
        "    border: 2px solid #27ae60; "
        "    border-radius: 6px; "
        "    color: #196f3d; "
        "    text-align: center; "
        "}";
    totalExpenseLabel->setStyleSheet(labelStyle);
    totalExpenseLabel->setAlignment(Qt::AlignCenter);
    expenseLayout->addWidget(totalExpenseLabel);

    QScrollArea *expenseScrollArea = new QScrollArea();
    expenseScrollArea->setWidgetResizable(true);
    expenseScrollArea->setFrameShape(QFrame::Box);
    expenseScrollArea->setStyleSheet(
        "QScrollArea { "
        "    background-color: #f8fbf8; "
        "    border: 2px solid #a9dfbf; "
        "    border-radius: 6px; "
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
        "    background-color: #2ecc71; "
        "}");

    expenseListWidget = new QListWidget();
    expenseListLayout = new QVBoxLayout(expenseListWidget);
    expenseListLayout->setSpacing(8);
    expenseListLayout->setContentsMargins(10, 10, 10, 10);

    updateExpenseDisplay();

    expenseScrollArea->setWidget(expenseListWidget);
    expenseLayout->addWidget(expenseScrollArea, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    addExpenseButton = new QPushButton("➕ Add Expense");
    viewExpensesButton = new QPushButton("👀 View All Expenses");

    addExpenseButton->setMinimumHeight(40);
    addExpenseButton->setMinimumWidth(120);
    addExpenseButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #3498db; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 8px; "
        "    padding: 8px 16px; "
        "    font-size: 12px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #2980b9; }"
        "QPushButton:pressed { background-color: #1f6391; }");

    viewExpensesButton->setMinimumHeight(40);
    viewExpensesButton->setMinimumWidth(120);
    viewExpensesButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #27ae60; "
        "    color: white; "
        "    border: none; "
        "    border-radius: 8px; "
        "    padding: 8px 16px; "
        "    font-size: 12px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #219150; }"
        "QPushButton:pressed { background-color: #1e7e34; }");

    buttonsLayout->addWidget(addExpenseButton);
    buttonsLayout->addWidget(viewExpensesButton);
    buttonsLayout->addStretch();

    expenseLayout->addLayout(buttonsLayout);
}

void ViewTripDialog::setupPeopleSection() {
    peopleInfoGroupBox = new QGroupBox("👥 People Information");
    peopleInfoGroupBox->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 16px; "
        "    color: #2c3e50; "
        "    border: 2px solid #e67e22; "
        "    border-radius: 8px; "
        "    margin-top: 10px; "
        "    padding-top: 15px; "
        "}"
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 10px; "
        "    padding: 0 5px 0 5px; "
        "}");

    QVBoxLayout *peopleLayout = new QVBoxLayout(peopleInfoGroupBox);
    peopleLayout->setSpacing(15);

    QGroupBox *hostGroupBox = new QGroupBox("🏠 Trip Host");
    hostGroupBox->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 14px; "
        "    color: #8e44ad; "
        "    border: 2px solid #9b59b6; "
        "    border-radius: 6px; "
        "    margin-top: 8px; "
        "    padding-top: 10px; "
        "}"
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 8px; "
        "    padding: 0 3px 0 3px; "
        "}");
    hostGroupBox->setMaximumHeight(200);

    QFormLayout *hostLayout = new QFormLayout(hostGroupBox);
    hostLayout->setVerticalSpacing(10);

    hostLabel = new QLabel();
    hostNameLabel = new QLabel();
    hostEmailLabel = new QLabel();
    hostPhoneLabel = new QLabel();
    hostInfoButton = new QPushButton("👤 View Full Host Details");

    QString hostLabelStyle =
        "QLabel { "
        "    font-size: 13px; "
        "    padding: 6px 8px; "
        "    background-color: #f4f1f8; "
        "    border: 1px solid #d7b3d7; "
        "    border-radius: 4px; "
        "    min-height: 18px; "
        "}";

    hostLabel->setStyleSheet(hostLabelStyle);
    hostNameLabel->setStyleSheet(hostLabelStyle);
    hostEmailLabel->setStyleSheet(hostLabelStyle);
    hostPhoneLabel->setStyleSheet(hostLabelStyle);

    hostInfoButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #9b59b6; "
        "    color: white; "
        "    border: none; "
        "    padding: 6px 16px; "
        "    border-radius: 5px; "
        "    font-size: 12px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #8e44ad; }"
        "QPushbutton:disabled {"
        "background-color: #bdc3c7;"
        "color: #7f8c8d;"
        "}");

    QLabel *hostIdLbl = new QLabel("ID:");
    QLabel *hostNameLbl = new QLabel("Name:");
    QLabel *hostEmailLbl = new QLabel("Email:");
    QLabel *hostPhoneLbl = new QLabel("Phone:");

    QString hostFormLabelStyle = "QLabel { font-weight: bold; font-size: 12px; color: #8e44ad; }";
    hostIdLbl->setStyleSheet(hostFormLabelStyle);
    hostNameLbl->setStyleSheet(hostFormLabelStyle);
    hostEmailLbl->setStyleSheet(hostFormLabelStyle);
    hostPhoneLbl->setStyleSheet(hostFormLabelStyle);

    hostLayout->addRow(hostIdLbl, hostLabel);
    hostLayout->addRow(hostNameLbl, hostNameLabel);
    hostLayout->addRow(hostEmailLbl, hostEmailLabel);
    hostLayout->addRow(hostPhoneLbl, hostPhoneLabel);
    hostLayout->addRow("", hostInfoButton);

    QGroupBox *membersGroupBox = new QGroupBox("👥 Trip Member(s)");
    membersGroupBox->setStyleSheet(
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 14px; "
        "    color: #27ae60; "
        "    border: 2px solid #2ecc71; "
        "    border-radius: 6px; "
        "    margin-top: 8px; "
        "    padding-top: 10px; "
        "}"
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 8px; "
        "    padding: 0 3px 0 3px; "
        "}");
    membersGroupBox->setMinimumHeight(300);

    QVBoxLayout *membersLayout = new QVBoxLayout(membersGroupBox);
    membersLayout->setSpacing(10);

    membersCountLabel = new QLabel();
    membersCountLabel->setStyleSheet(
        "QLabel { "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "    padding: 8px; "
        "    background-color: #e8f8f5; "
        "    border: 1px solid #a9dfbf; "
        "    border-radius: 4px; "
        "    color: #27ae60; "
        "}");

    membersListWidget = new QListWidget();
    membersListWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    membersListWidget->setStyleSheet(
        "QListWidget { "
        "    border: 2px solid #a9dfbf; "
        "    border-radius: 5px; "
        "    background-color: #f8fbf8; "
        "    font-size: 13px; "
        "}"
        "QListWidget::item { "
        "    padding: 8px; "
        "    border-bottom: 1px solid #d5f4e6; "
        "}"
        "QListWidget::item:hover { "
        "    background-color: #e8f8f5; "
        "}"
        "QListWidget::item:selected { "
        "    background-color: #2ecc71; "
        "    color: white; "
        "}");

    membersInfoButton = new QPushButton("👥 View All Member(s) Details");
    membersInfoButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #2ecc71; "
        "    color: white; "
        "    border: none; "
        "    padding: 8px 16px; "
        "    border-radius: 5px; "
        "    font-size: 12px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #27ae60; }"
        "QPushbutton:disabled {"
        "background-color: #bdc3c7;"
        "color: #7f8c8d;"
        "}");

    membersLayout->addWidget(membersCountLabel);
    membersLayout->addWidget(membersListWidget, 1);
    membersLayout->addWidget(membersInfoButton);

    peopleLayout->addWidget(hostGroupBox, 1);
    peopleLayout->addWidget(membersGroupBox, 3);
}

void ViewTripDialog::displayCurrentTrip() {
    idLabel->setText(QString::fromStdString(trip.getID()));
    destinationLabel->setText(QString::fromStdString(trip.getDestination()));
    descriptionDisplay->setPlainText(QString::fromStdString(trip.getDescription()));
    startDateLabel->setText(QString::fromStdString(trip.getStartDate().toString()));
    endDateLabel->setText(QString::fromStdString(trip.getEndDate().toString()));

    QString status = QString::fromStdString(statusToString(trip.getStatus()));
    QString statusColor;
    QString statusIcon;

    if (status == "Planned") {
        statusColor = "#3498db";
        statusIcon = "🗓️";
    } else if (status == "Ongoing") {
        statusColor = "#f39c12";
        statusIcon = "🚀";
    } else if (status == "Completed") {
        statusColor = "#27ae60";
        statusIcon = "✅";
    } else if (status == "Cancelled") {
        statusColor = "#e74c3c";
        statusIcon = "❌";
    }

    statusLabel->setText(QString("%1 %2").arg(statusIcon).arg(status));
    statusLabel->setStyleSheet(QString("QLabel { "
                                       "    font-size: 14px; "
                                       "    font-weight: bold; "
                                       "    padding: 8px; "
                                       "    background-color: %1; "
                                       "    color: white; "
                                       "    border-radius: 4px; "
                                       "}")
                                   .arg(statusColor));

    updateExpenseDisplay();
}

void ViewTripDialog::displayPeopleInfo() {
    qDebug() << "=== displayPeopleInfo() ===";
    qDebug() << "PersonManager available:" << (personManager != nullptr);

    HOST host = trip.getHost();
    qDebug() << "Trip host ID:" << QString::fromStdString(host.getID());
    qDebug() << "Host name:" << QString::fromStdString(host.getFullName());

    if (!host.getID().empty()) {
        if (personManager) {
            HOST freshHost = personManager->getHostByID(host.getID());
            if (!freshHost.getID().empty()) {
                host = freshHost;
                qDebug() << "Using fresh host data from PersonManager";
            } else {
                qDebug() << "Host not found in PersonManager, using trip data";
            }
        }

        hostLabel->setText(QString::fromStdString(host.getID()));
        hostNameLabel->setText(QString::fromStdString(host.getFullName()));
        hostEmailLabel->setText(QString::fromStdString(host.getEmail()));
        hostPhoneLabel->setText(QString::fromStdString(host.getPhoneNumber()));
        hostInfoButton->setEnabled(true);
    } else {
        hostLabel->setText("No host assigned");
        hostNameLabel->setText("N/A");
        hostEmailLabel->setText("N/A");
        hostPhoneLabel->setText("N/A");
        hostInfoButton->setEnabled(false);
    }

    vector<MEMBER> members = trip.getMembers();
    qDebug() << "Trip has" << members.size() << "members";

    if (personManager && !members.empty()) {
        vector<MEMBER> freshMembers;
        for (const MEMBER &member : members) {
            if (!member.getID().empty()) {
                MEMBER freshMember = personManager->getMemberByID(member.getID());
                if (!freshMember.getID().empty()) {
                    freshMembers.push_back(freshMember);
                    qDebug() << "Updated member:" << QString::fromStdString(freshMember.getFullName());
                } else {
                    freshMembers.push_back(member);
                    qDebug() << "Member not found in PersonManager, keeping original:"
                             << QString::fromStdString(member.getFullName());
                }
            }
        }
        members = freshMembers;
    }

    membersCountLabel->setText(QString("Total Member(s): %1").arg(members.size()));

    membersListWidget->clear();
    if (members.empty()) {
        QListWidgetItem *noMembersItem = new QListWidgetItem("No members assigned to this trip");
        noMembersItem->setFlags(Qt::NoItemFlags);
        noMembersItem->setData(Qt::ForegroundRole, QColor("#7f8c8d"));
        membersListWidget->addItem(noMembersItem);
        membersInfoButton->setEnabled(false);
    } else {
        for (size_t i = 0; i < members.size(); i++) {
            const MEMBER &member = members[i];
            if (!member.getID().empty()) {
                QString memberInfo = QString("👤 %1").arg(QString::fromStdString(member.getInfo()));
                QListWidgetItem *item = new QListWidgetItem(memberInfo);
                item->setData(Qt::UserRole, static_cast<int>(i));
                membersListWidget->addItem(item);
            }
        }
        membersInfoButton->setEnabled(true);
    }

    qDebug() << "=== displayPeopleInfo() completed ===";
}

void ViewTripDialog::onHostInfoClicked() {
    HOST host = trip.getHost();
    if (!host.getID().empty()) {
        QDialog *hostDialog = new QDialog(this);
        hostDialog->setWindowTitle("Host Details");
        hostDialog->setModal(true);
        hostDialog->setMinimumSize(600, 400);
        hostDialog->resize(700, 600);
        hostDialog->setMaximumSize(900, 700);

        QVBoxLayout *mainLayout = new QVBoxLayout(hostDialog);
        mainLayout->setSpacing(15);
        mainLayout->setContentsMargins(20, 20, 20, 20);

        QLabel *titleLabel = new QLabel("🏠 Host Information");
        titleLabel->setStyleSheet(
            "QLabel { "
            "    font-size: 18px; "
            "    font-weight: bold; "
            "    color: #8e44ad; "
            "    padding: 10px; "
            "    background-color: #f4f1f8; "
            "    border: 2px solid #9b59b6; "
            "    border-radius: 8px; "
            "    text-align: center; "
            "}");
        titleLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(titleLabel);

        QScrollArea *scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(true);
        scrollArea->setFrameShape(QFrame::NoFrame);

        QWidget *contentWidget = new QWidget();
        QFormLayout *formLayout = new QFormLayout(contentWidget);
        formLayout->setVerticalSpacing(12);
        formLayout->setHorizontalSpacing(15);

        QLabel *idLabel = new QLabel(QString::fromStdString(host.getID()));
        QLabel *nameLabel = new QLabel(QString::fromStdString(host.getFullName()));
        QLabel *emailLabel = new QLabel(QString::fromStdString(host.getEmail()));
        QLabel *phoneLabel = new QLabel(QString::fromStdString(host.getPhoneNumber()));
        QLabel *genderLabel = new QLabel(QString::fromStdString(genderToString(host.getGender())));
        QLabel *dobLabel = new QLabel(QString::fromStdString(host.getDateOfBirth().toString()));
        QLabel *addressLabel = new QLabel(QString::fromStdString(host.getAddress()));
        QLabel *emergencyLabel = new QLabel(QString::fromStdString(host.getEmergencyContact()));

        QString dataLabelStyle =
            "QLabel { "
            "    font-size: 13px; "
            "    padding: 8px 12px; "
            "    background-color: #fdfdfe; "
            "    border: 1px solid #d7b3d7; "
            "    border-radius: 6px; "
            "    color: #2c3e50; "
            "    font-weight: 500; "
            "}";

        idLabel->setStyleSheet(dataLabelStyle);
        nameLabel->setStyleSheet(dataLabelStyle);
        emailLabel->setStyleSheet(dataLabelStyle);
        phoneLabel->setStyleSheet(dataLabelStyle);
        genderLabel->setStyleSheet(dataLabelStyle);
        dobLabel->setStyleSheet(dataLabelStyle);
        addressLabel->setStyleSheet(dataLabelStyle);
        emergencyLabel->setStyleSheet(dataLabelStyle);

        QString fieldLabelStyle =
            "QLabel { "
            "    font-weight: bold; "
            "    font-size: 12px; "
            "    color: #8e44ad; "
            "    padding: 4px; "
            "}";

        QLabel *idFieldLabel = new QLabel("🆔 ID:");
        QLabel *nameFieldLabel = new QLabel("👤 Full Name:");
        QLabel *emailFieldLabel = new QLabel("📧 Email:");
        QLabel *phoneFieldLabel = new QLabel("📱 Phone:");
        QLabel *genderFieldLabel = new QLabel("⚧️ Gender:");
        QLabel *dobFieldLabel = new QLabel("🎂 Date of Birth:");
        QLabel *addressFieldLabel = new QLabel("🏠 Address:");
        QLabel *emergencyFieldLabel = new QLabel("🚨 Emergency Contact:");

        idFieldLabel->setStyleSheet(fieldLabelStyle);
        nameFieldLabel->setStyleSheet(fieldLabelStyle);
        emailFieldLabel->setStyleSheet(fieldLabelStyle);
        phoneFieldLabel->setStyleSheet(fieldLabelStyle);
        genderFieldLabel->setStyleSheet(fieldLabelStyle);
        dobFieldLabel->setStyleSheet(fieldLabelStyle);
        addressFieldLabel->setStyleSheet(fieldLabelStyle);
        emergencyFieldLabel->setStyleSheet(fieldLabelStyle);

        formLayout->addRow(idFieldLabel, idLabel);
        formLayout->addRow(nameFieldLabel, nameLabel);
        formLayout->addRow(emailFieldLabel, emailLabel);
        formLayout->addRow(phoneFieldLabel, phoneLabel);
        formLayout->addRow(genderFieldLabel, genderLabel);
        formLayout->addRow(dobFieldLabel, dobLabel);
        formLayout->addRow(addressFieldLabel, addressLabel);
        formLayout->addRow(emergencyFieldLabel, emergencyLabel);

        scrollArea->setWidget(contentWidget);
        mainLayout->addWidget(scrollArea);

        QPushButton *closeBtn = new QPushButton("✅ Close");
        closeBtn->setStyleSheet(
            "QPushButton { "
            "    background-color: #9b59b6; "
            "    color: white; "
            "    border: none; "
            "    padding: 10px 20px; "
            "    border-radius: 6px; "
            "    font-size: 13px; "
            "    font-weight: bold; "
            "}"
            "QPushButton:hover { background-color: #8e44ad; }"
            "QPushButton:pressed { background-color: #7d3c98; }");

        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->addStretch();
        buttonLayout->addWidget(closeBtn);
        buttonLayout->addStretch();
        mainLayout->addLayout(buttonLayout);

        connect(closeBtn, &QPushButton::clicked, hostDialog, &QDialog::accept);

        if (this->parentWidget()) {
            hostDialog->move(this->geometry().center() - hostDialog->rect().center());
        }

        hostDialog->exec();
        delete hostDialog;
    }
}

void ViewTripDialog::onMemberInfoClicked() {
    vector<MEMBER> members = trip.getMembers();
    if (members.empty()) {
        QMessageBox::information(this, "Members Information", "No members assigned to this trip.");
        return;
    }

    QDialog *membersDialog = new QDialog(this);
    membersDialog->setWindowTitle("Members Details");
    membersDialog->setModal(true);
    membersDialog->setMinimumSize(600, 500);
    membersDialog->resize(650, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(membersDialog);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QLabel *titleLabel = new QLabel(QString("👥 Members Details"));
    titleLabel->setStyleSheet(
        "QLabel { "
        "    font-size: 18px; "
        "    font-weight: bold; "
        "    color: #27ae60; "
        "    padding: 12px; "
        "    background-color: #e8f8f5; "
        "    border: 2px solid #2ecc71; "
        "    border-radius: 8px; "
        "    text-align: center; "
        "}");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(
        "QScrollArea { "
        "    background-color: #f8fbf8; "
        "    border: 1px solid #a9dfbf; "
        "    border-radius: 6px; "
        "}");

    QWidget *contentWidget = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setSpacing(15);
    contentLayout->setContentsMargins(15, 15, 15, 15);

    for (size_t i = 0; i < members.size(); ++i) {
        const MEMBER &member = members[i];
        if (!member.getID().empty()) {
            QGroupBox *memberBox = new QGroupBox(QString("%1").arg(QString::fromStdString(member.getFullName())));
            memberBox->setStyleSheet(
                "QGroupBox { "
                "    font-weight: bold; "
                "    font-size: 14px; "
                "    color: #27ae60; "
                "    border: 2px solid #58d68d; "
                "    border-radius: 8px; "
                "    margin-top: 10px; "
                "    padding-top: 15px; "
                "    background-color: #f9fffe; "
                "}"
                "QGroupBox::title { "
                "    subcontrol-origin: margin; "
                "    left: 10px; "
                "    padding: 0 8px 0 8px; "
                "    background-color: #e8f8f5; "
                "    border-radius: 4px; "
                "}");

            QFormLayout *memberLayout = new QFormLayout(memberBox);
            memberLayout->setVerticalSpacing(8);
            memberLayout->setHorizontalSpacing(12);

            QLabel *idLabel = new QLabel(QString::fromStdString(member.getID()));
            QLabel *nameLabel = new QLabel(QString::fromStdString(member.getFullName()));
            QLabel *emailLabel = new QLabel(QString::fromStdString(member.getEmail()));
            QLabel *dobLabel = new QLabel(QString::fromStdString(member.getDateOfBirth().toString()));
            QLabel *phoneLabel = new QLabel(QString::fromStdString(member.getPhoneNumber()));
            QLabel *genderLabel = new QLabel(QString::fromStdString(genderToString(member.getGender())));

            vector<pair<string, EXPENSE>> memberSpendings = member.getSpendingsForTrip(trip.getID());
            long long totalSpent = 0;
            for (size_t i = 0; i < memberSpendings.size(); i++) {
                if (memberSpendings[i].first == trip.getID()) {
                    totalSpent += memberSpendings[i].second.getAmount();
                }
            }

            QLabel *spentLabel = new QLabel(QString("%1").arg(formatCurrency(totalSpent)));

            QString memberDataStyle =
                "QLabel { "
                "    font-size: 12px; "
                "    padding: 6px 10px; "
                "    background-color: #fdfefd; "
                "    border: 1px solid #a9dfbf; "
                "    border-radius: 4px; "
                "    color: #1e4d28; "
                "}";

            idLabel->setStyleSheet(memberDataStyle);
            nameLabel->setStyleSheet(memberDataStyle);
            emailLabel->setStyleSheet(memberDataStyle);
            dobLabel->setStyleSheet(memberDataStyle);
            phoneLabel->setStyleSheet(memberDataStyle);
            genderLabel->setStyleSheet(memberDataStyle);
            spentLabel->setStyleSheet(memberDataStyle + "QLabel { font-weight: bold; color: #196f3d; }");

            QString memberFieldStyle =
                "QLabel { "
                "    font-weight: bold; "
                "    font-size: 11px; "
                "    color: #27ae60; "
                "    padding: 2px; "
                "}";

            QLabel *idField = new QLabel("🆔 ID:");
            QLabel *nameField = new QLabel("👤 Name:");
            QLabel *emailField = new QLabel("📧 Email:");
            QLabel *dobField = new QLabel("🗓️ Date of birth:");
            QLabel *phoneField = new QLabel("📱 Phone:");
            QLabel *genderField = new QLabel("⚧️ Gender:");
            QLabel *spentField = new QLabel("💰 Total Spent:");

            idField->setStyleSheet(memberFieldStyle);
            nameField->setStyleSheet(memberFieldStyle);
            emailField->setStyleSheet(memberFieldStyle);
            dobField->setStyleSheet(memberFieldStyle);
            phoneField->setStyleSheet(memberFieldStyle);
            genderField->setStyleSheet(memberFieldStyle);
            spentField->setStyleSheet(memberFieldStyle);

            memberLayout->addRow(idField, idLabel);
            memberLayout->addRow(nameField, nameLabel);
            memberLayout->addRow(emailField, emailLabel);
            memberLayout->addRow(dobField, dobLabel);
            memberLayout->addRow(phoneField, phoneLabel);
            memberLayout->addRow(genderField, genderLabel);
            memberLayout->addRow(spentField, spentLabel);

            contentLayout->addWidget(memberBox);
        }
    }

    contentLayout->addStretch();
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);

    QLabel *statsLabel = new QLabel(QString("📊 Total Members: %1").arg(members.size()));
    statsLabel->setStyleSheet(
        "QLabel { "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "    color: #27ae60; "
        "    padding: 8px 15px; "
        "    background-color: #e8f8f5; "
        "    border: 1px solid #a9dfbf; "
        "    border-radius: 6px; "
        "}");
    statsLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(statsLabel);

    QPushButton *closeBtn = new QPushButton("✅ Close");
    closeBtn->setStyleSheet(
        "QPushButton { "
        "    background-color: #2ecc71; "
        "    color: white; "
        "    border: none; "
        "    padding: 10px 20px; "
        "    border-radius: 6px; "
        "    font-size: 13px; "
        "    font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #27ae60; }"
        "QPushButton:pressed { background-color: #229954; }");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeBtn);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    connect(closeBtn, &QPushButton::clicked, membersDialog, &QDialog::accept);

    if (this->parentWidget()) {
        membersDialog->move(this->geometry().center() - membersDialog->rect().center());
    }

    membersDialog->exec();
    delete membersDialog;
}

void ViewTripDialog::editCurrentTrip() {
    EditTripDialog editDialog(trip, this);
    editDialog.setPersonManager(personManager);
    if (editDialog.exec() == QDialog::Accepted) {
        trip = editDialog.getUpdatedTrip();
        displayCurrentTrip();
        displayPeopleInfo();
    }
}

void ViewTripDialog::onViewExpensesClicked() {
    ViewExpenseDialog dialog(trip, personManager, this);
    if (dialog.exec() == QDialog::Accepted) {
        if (dialog.hasExpensesChanged()) {
            trip = dialog.getUpdatedTrip();
        }
        updateExpenseDisplay();

        displayCurrentTrip();
        displayPeopleInfo();
    }
}

void ViewTripDialog::onAddExpenseClicked() {
    AddExpenseDialog dialog(trip, this, personManager);
    if (dialog.exec() == QDialog::Accepted) {
        EXPENSE newExpense = dialog.getExpense();

        trip.addExpense(newExpense);

        MEMBER memberInCharge = newExpense.getPIC();
        if (personManager) {
            MEMBER freshMember = personManager->getMemberByID(memberInCharge.getID());
            if (!freshMember.getID().empty()) {
                freshMember.addSpending(newExpense, trip);
                personManager->updateMember(memberInCharge, freshMember);

                vector<MEMBER> tripMembers = trip.getMembers();

                for (size_t i = 0; i < tripMembers.size(); i++) {
                    if (tripMembers[i].getID() == freshMember.getID()) {
                        tripMembers[i] = freshMember;
                        break;
                    }
                }

                trip.setMembers(tripMembers);

                updatePeopleCacheFile(personManager->getAllMembers(), personManager->getAllHosts());
            }
        } else {
            vector<MEMBER> tripMembers = trip.getMembers();
            for (size_t i = 0; i < tripMembers.size(); i++) {
                if (tripMembers[i].getID() == memberInCharge.getID()) {
                    tripMembers[i].addSpending(newExpense, trip);
                    break;
                }
            }
            trip.setMembers(tripMembers);
        }

        updateExpenseDisplay();
        displayCurrentTrip();
        displayPeopleInfo();
    }
}

void ViewTripDialog::closeView() { accept(); }

TRIP ViewTripDialog::getUpdatedTrip() const { return trip; }

TRIP ViewTripDialog::getOriginalTrip() const { return originalTrip; }

void ViewTripDialog::updateExpenseDisplay() {
    expenseListWidget->clear();

    expenseListWidget->setStyleSheet(
        "QListWidget { "
        "    border: 2px solid #a9dfbf; "
        "    border-radius: 8px; "
        "    background-color: #f8fbf8; "
        "    font-size: 13px; "
        "    padding: 5px; "
        "}"
        "QListWidget::item { "
        "    background-color: #fdfdfe; "
        "    border: 2px solid #d5f4e6; "
        "    border-radius: 8px; "
        "    margin: 3px; "
        "    padding: 12px; "
        "    color: #2c3e50; "
        "}"
        "QListWidget::item:hover { "
        "    background-color: #e8f8f5; "
        "    border-color: #27ae60; "
        "    transform: scale(1.02); "
        "}"
        "QListWidget::item:selected { "
        "    background-color: #d5f4e6; "
        "    border-color: #27ae60; "
        "    color: #196f3d; "
        "    font-weight: bold; "
        "}"
        "QListWidget::item:selected:hover { "
        "    background-color: #c8e6c9; "
        "    border-color: #1e7e34; "
        "}");

    vector<EXPENSE> expenses = trip.getAllExpenses();

    if (expenses.empty()) {
        QListWidgetItem *noExpensesItem = new QListWidgetItem("📝 No expenses recorded for this trip");
        noExpensesItem->setFlags(Qt::NoItemFlags);

        noExpensesItem->setData(Qt::ForegroundRole, QColor("#7f8c8d"));
        noExpensesItem->setData(Qt::BackgroundRole, QColor("#f8f9fa"));
        noExpensesItem->setData(Qt::TextAlignmentRole, Qt::AlignCenter);
        noExpensesItem->setData(Qt::SizeHintRole, QSize(0, 80));

        QFont italicFont;
        italicFont.setItalic(true);
        italicFont.setPointSize(14);
        noExpensesItem->setData(Qt::FontRole, italicFont);

        expenseListWidget->addItem(noExpensesItem);
    } else {
        for (size_t i = 0; i < expenses.size(); ++i) {
            const EXPENSE &expense = expenses[i];

            QString expenseSummary = QString(
                                         "💰 %1\n"
                                         "📂 %2  •  👤 %3\n"
                                         "📅 %4")
                                         .arg(formatCurrency(expense.getAmount()))
                                         .arg(QString::fromStdString(categoryToString(expense.getCategory())))
                                         .arg(QString::fromStdString(expense.getPIC().getFullName()))
                                         .arg(QString::fromStdString(expense.getDate().toString()));

            QListWidgetItem *item = new QListWidgetItem(expenseSummary);
            item->setData(Qt::UserRole, static_cast<int>(i));

            item->setData(Qt::SizeHintRole, QSize(0, 85));

            item->setData(Qt::ForegroundRole, QColor("#2c3e50"));

            QString styledSummary = QString(
                                        "📅 %1\n"
                                        "💰 %2 • 📂 %3  •  👤 %4 (ID: %5)")
                                        .arg(QString::fromStdString(expense.getDate().toString()))
                                        .arg(formatCurrency(expense.getAmount()))
                                        .arg(QString::fromStdString(categoryToString(expense.getCategory())))
                                        .arg(QString::fromStdString(expense.getPIC().getFullName()))
                                        .arg(QString::fromStdString(expense.getPIC().getID()));

            item->setText(styledSummary);

            item->setData(Qt::BackgroundRole, QColor("#e8f5e8"));

            QFont itemFont;
            itemFont.setPointSize(12);
            itemFont.setWeight(QFont::Medium);
            item->setData(Qt::FontRole, itemFont);

            expenseListWidget->addItem(item);
        }
    }

    long long totalExpenses = 0;
    for (const EXPENSE &expense : expenses) {
        totalExpenses += expense.getAmount();
    }

    QString formattedTotal = formatCurrency(totalExpenses);
    QString totalIcon = "💰";
    QString countIcon = "📊";

    if (totalExpenses >= 5000000) {
        totalIcon = "💸";
    } else if (totalExpenses >= 2000000) {
        totalIcon = "💰";
    } else {
        totalIcon = "💵";
    }

    totalExpenseLabel->setText(QString("%1 Total: %2  |  %3 Count: %4")
                                   .arg(totalIcon)
                                   .arg(formattedTotal)
                                   .arg(countIcon)
                                   .arg(expenses.size()));
}

QString ViewTripDialog::formatCurrency(long long amount) const {
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