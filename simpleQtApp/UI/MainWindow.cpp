#include "MainWindow.h"

#include <QDateTime>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <algorithm>

#include "../Managers/FileManager.h"
#include "../Managers/Observer.h"
#include "../Managers/PersonFactory.h"
#include "../Managers/PersonManager.h"
#include "../Managers/TripManager.h"
#include "../Models/header.h"
#include "AddPersonDialog.h"
#include "AddTripDialog.h"
#include "EditTripDialog.h"
#include "FilterTripDialog.h"
#include "ViewTripDialog.h"

// ========================================
// CONSTRUCTOR & DESTRUCTOR
// ========================================

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    personManager = new PERSONMANAGER();
    tripManager = new TRIPMANAGER();
    personManager->addObserver(this);
    tripManager->addObserver(this);

    setupUI();
    setupStatusBar();
    setMinimumSize(1000, 900);
    resize(1200, 900);

    QRect screenGeometry = QApplication::primaryScreen()->geometry();
    move(screenGeometry.center() - rect().center());

    vector<TRIP> cachedTrips;
    loadCacheFromFile(cachedTrips);
    if (!cachedTrips.empty()) {
        for (const TRIP &trip : cachedTrips) {
            try {
                tripManager->addTrip(trip);
            } catch (const exception &e) {
                addDebugMessage(QString("Failed to load trip '%1': %2")
                                    .arg(QString::fromStdString(trip.getDestination()))
                                    .arg(e.what()));
            }
        }
    }

    addDebugMessage("Application initialization completed");
}

MainWindow::~MainWindow() {
    addDebugMessage("Saving application state before exit...");

    saveCacheToFile();

    savePeopleDataToCache(personManager->getAllMembers(), personManager->getAllHosts(),
                          getPeopleCacheFilePath().toStdString());
    if (personManager) {
        personManager->removeObserver(this);
        delete personManager;
    }

    if (tripManager) {
        tripManager->removeObserver(this);
        delete tripManager;
    }

    addDebugMessage("Application shutdown completed");
}

// ========================================
// UI SETUP FUNCTIONS
// ========================================
void MainWindow::setupUI() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    centralWidget->setStyleSheet("QWidget { background-color: #f0f8f0; }");

    mainSplitter = new QSplitter(Qt::Horizontal);
    mainSplitter->setStyleSheet(
        "QSplitter::handle { "
        "    background-color: #a9dfbf; "
        "    width: 3px; "
        "}"
        "QSplitter::handle:hover { "
        "    background-color: #27ae60; "
        "}");

    setupSidebar();
    setupMainContent();

    mainSplitter->addWidget(sidebar);
    mainSplitter->addWidget(mainContent);
    mainSplitter->setStretchFactor(0, 1);
    mainSplitter->setStretchFactor(1, 2);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addWidget(mainSplitter);
    mainLayout->setContentsMargins(8, 8, 8, 8);
}

void MainWindow::setupSidebar() {
    sidebar = new QWidget();
    sidebar->setMinimumWidth(250);
    sidebar->setMaximumWidth(400);

    sidebar->setStyleSheet(
        "QWidget { "
        "    background-color: #f8fbf8; "
        "    color: #2c3e50; "
        "}"
        "QGroupBox { "
        "    font-weight: bold; "
        "    font-size: 14px; "
        "    color: #27ae60; "
        "    border: 2px solid #a9dfbf; "
        "    border-radius: 8px; "
        "    margin-top: 15px; "
        "    padding-top: 10px; "
        "    background-color: #fdfdfe; "
        "}"
        "QGroupBox::title { "
        "    subcontrol-origin: margin; "
        "    left: 15px; "
        "    padding: 0 8px 0 8px; "
        "    background-color: #fdfdfe; "
        "}"
        "QPushButton { "
        "    text-align: left; "
        "    padding: 10px 15px; "
        "    margin: 3px; "
        "    border-radius: 6px; "
        "    border: 2px solid #d5f4e6; "
        "    background-color: #fdfdfe; "
        "    color: #2c3e50; "
        "    font-size: 13px; "
        "    font-weight: 500; "
        "    max-height: 17px;"
        "    min-height: 10px;"
        "    height: 15px;"
        "}"
        "QPushButton:hover { "
        "    background-color: #e8f8f5; "
        "    border-color: #a9dfbf; "
        "    color: #1e7e34; "
        "}"
        "QPushButton:pressed { "
        "    background-color: #d5f4e6; "
        "    border-color: #27ae60; "
        "}");

    sidebarLayout = new QVBoxLayout(sidebar);

    quickActionsGroup = new QGroupBox("📁 File Operations");
    QVBoxLayout *quickLayout = new QVBoxLayout(quickActionsGroup);
    importButton = new QPushButton("📥 Import Trips");
    exportButton = new QPushButton("📤 Export Trips");

    quickLayout->addWidget(importButton);
    quickLayout->addWidget(exportButton);

    managementGroup = new QGroupBox("🎯 Trip Management");
    QVBoxLayout *mgmtLayout = new QVBoxLayout(managementGroup);
    addTripButton = new QPushButton("➕ Add New Trip");
    deleteButton = new QPushButton("🗑️ Delete Trip");
    editTripButton = new QPushButton("✏️ Edit Trip");
    detailViewButton = new QPushButton("🔍 View Detail");

    addTripButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #27ae60; "
        "    color: white; "
        "    font-weight: bold; "
        "    border: 2px solid #229954; "
        "}"
        "QPushButton:hover { "
        "    background-color: #229954; "
        "    border-color: #1e7e34; "
        "}"
        "QPushButton:pressed { "
        "    background-color: #1e7e34; "
        "}");

    deleteButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #e74c3c; "
        "    color: white; "
        "    font-weight: bold; "
        "    border: 2px solid #c0392b; "
        "}"
        "QPushButton:hover { "
        "    background-color: #c0392b; "
        "    border-color: #a93226; "
        "}"
        "QPushButton:pressed { "
        "    background-color: #a93226; "
        "}");

    mgmtLayout->addWidget(addTripButton);
    mgmtLayout->addWidget(deleteButton);
    mgmtLayout->addWidget(editTripButton);
    mgmtLayout->addWidget(detailViewButton);

    peopleGroup = new QGroupBox("👥 People Management");
    QVBoxLayout *peopleLayout = new QVBoxLayout(peopleGroup);

    QVBoxLayout *peopleImportExportLayout = new QVBoxLayout();

    importPeopleButton = new QPushButton("📥 Import People");
    importPeopleButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #3498db; "
        "    color: white; "
        "    font-weight: bold; "
        "    border: 2px solid #2980b9; "
        "}"
        "QPushButton:hover { "
        "    background-color: #2980b9; "
        "    border-color: #21618c; "
        "}"
        "QPushButton:pressed { "
        "    background-color: #21618c; "
        "}");

    exportPeopleButton = new QPushButton("📤 Export People");
    exportPeopleButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #2ecc71; "
        "    color: white; "
        "    font-weight: bold; "
        "    border: 2px solid #27ae60; "
        "}"
        "QPushButton:hover { "
        "    background-color: #27ae60; "
        "    border-color: #229954; "
        "}"
        "QPushButton:pressed { "
        "    background-color: #229954; "
        "}");

    peopleImportExportLayout->addWidget(importPeopleButton);
    peopleImportExportLayout->addWidget(exportPeopleButton);
    peopleLayout->addLayout(peopleImportExportLayout);

    QPushButton *managePeopleButton = new QPushButton("👥 Manage People");
    managePeopleButton->setStyleSheet(
        "QPushButton { "
        "    background-color: #9b59b6; "
        "    color: white; "
        "    font-weight: bold; "
        "    border: 2px solid #8e44ad; "
        "}"
        "QPushButton:hover { "
        "    background-color: #8e44ad; "
        "    border-color: #7d3c98; "
        "}"
        "QPushButton:pressed { "
        "    background-color: #7d3c98; "
        "}");

    peopleLayout->addWidget(managePeopleButton);

    // View and Filter Group
    viewGroup = new QGroupBox("🔍 View and Filter");
    QVBoxLayout *viewLayout = new QVBoxLayout(viewGroup);

    filterButton = new QPushButton("🔽 Filter Trips");
    QPushButton *upcomingButton = new QPushButton("⏰ Upcoming Trips");
    QPushButton *completedButton = new QPushButton("✅ Completed Trips");
    refreshButton = new QPushButton("🔄 Refresh View");

    viewLayout->addWidget(filterButton);
    viewLayout->addWidget(upcomingButton);
    viewLayout->addWidget(completedButton);
    viewLayout->addWidget(refreshButton);

    sidebarLayout->addWidget(quickActionsGroup);
    sidebarLayout->addWidget(managementGroup);
    sidebarLayout->addWidget(peopleGroup);
    sidebarLayout->addWidget(viewGroup);
    sidebarLayout->addStretch();
    connect(importButton, &QPushButton::clicked, this, &MainWindow::onImportTripsClicked);
    connect(exportButton, &QPushButton::clicked, this, &MainWindow::onExportTripsClicked);
    connect(addTripButton, &QPushButton::clicked, this, &MainWindow::onAddTripClicked);
    connect(editTripButton, &QPushButton::clicked, this, &MainWindow::onEditTripClicked);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteTripClicked);
    connect(detailViewButton, &QPushButton::clicked, this, &MainWindow::onViewTripDetailsClicked);
    connect(managePeopleButton, &QPushButton::clicked, this, &MainWindow::onManagePeopleClicked);
    connect(filterButton, &QPushButton::clicked, this, &MainWindow::onFilterTripsClicked);
    connect(upcomingButton, &QPushButton::clicked, this, &MainWindow::onShowUpcomingTripsClicked);
    connect(completedButton, &QPushButton::clicked, this, &MainWindow::onShowCompletedTripsClicked);
    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshViewClicked);
    connect(importPeopleButton, &QPushButton::clicked, this, &MainWindow::onImportPeopleClicked);
    connect(exportPeopleButton, &QPushButton::clicked, this, &MainWindow::onExportPeopleClicked);
}

void MainWindow::setupMainContent() {
    mainContent = new QWidget();
    mainContentLayout = new QVBoxLayout(mainContent);

    headerWidget = new QWidget();
    headerWidget->setStyleSheet(
        "QWidget { "
        "    background-color: #fdfdfe; "
        "    border-bottom: 3px solid #a9dfbf; "
        "    border-radius: 0px 0px 8px 8px; "
        "}");

    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(20, 15, 20, 15);

    titleLabel = new QLabel("🎯 Trip Management Dashboard");
    titleLabel->setStyleSheet(
        "QLabel { "
        "    font-size: 26px; "
        "    font-weight: bold; "
        "    color: #27ae60; "
        "    padding: 5px 0; "
        "}");

    statsLabel = new QLabel("0 trips loaded");
    statsLabel->setStyleSheet(
        "QLabel { "
        "    font-size: 14px; "
        "    color: #7f8c8d; "
        "    background-color: #e8f8f5; "
        "    padding: 8px 12px; "
        "    border-radius: 6px; "
        "    border: 1px solid #d5f4e6; "
        "}");

    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(statsLabel);

    tripDisplayArea = new QWidget();
    tripDisplayArea->setStyleSheet("QWidget { background-color: #f8fbf8; }");
    QVBoxLayout *displayLayout = new QVBoxLayout(tripDisplayArea);
    displayLayout->setContentsMargins(15, 15, 15, 15);

    tripsTable = new QTableWidget();
    tripsTable->setColumnCount(6);
    QStringList headers = {"ID", "Destination", "Description", "Start Date", "End Date", "Status"};
    tripsTable->setHorizontalHeaderLabels(headers);

    tripsTable->setColumnWidth(0, 80);
    tripsTable->setColumnWidth(1, 120);
    tripsTable->setColumnWidth(2, 200);
    tripsTable->setColumnWidth(3, 100);
    tripsTable->setColumnWidth(4, 100);
    tripsTable->setColumnWidth(5, 80);
    tripsTable->horizontalHeader()->setMinimumSectionSize(60);
    tripsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    tripsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    tripsTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tripsTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);
    tripsTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Interactive);
    tripsTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);

    int minTableWidth = 80 + 120 + 200 + 100 + 100 + 80;
    tripsTable->setMinimumWidth(minTableWidth);

    tripsTable->setStyleSheet(
        "QTableWidget { "
        "    gridline-color: #d5f4e6; "
        "    background-color: #fdfdfe; "
        "    border: 2px solid #a9dfbf; "
        "    border-radius: 8px; "
        "    selection-background-color: #d5f4e6; "
        "    font-size: 13px; "
        "    outline: none; "
        "}"
        "QTableWidget::item { "
        "    padding: 16px 10px; "
        "    border-bottom: 1px solid #e8f8f5; "
        "    color: #2c3e50; "
        "    outline: none; "
        "}"
        "QTableWidget::item:selected { "
        "    background-color: #d5f4e6; "
        "    color: #1e7e34; "
        "    font-weight: bold; "
        "    outline: none; "
        "}"
        "QTableWidget::item:hover { "
        "    background-color: #e8f8f5; "
        "}"
        "QTableWidget::item:focus { "
        "    outline: none; "
        "    border: none; "
        "}"
        "QHeaderView::section { "
        "    background-color: #27ae60; "
        "    color: white; "
        "    font-weight: bold; "
        "    font-size: 14px; "
        "    padding: 12px 10px; "
        "    border: 1px solid #229954; "
        "    border-radius: 0px; "
        "    min-height: 20px;"
        "}"
        "QHeaderView::section:hover { "
        "    background-color: #229954; "
        "}"
        "QScrollBar:vertical { "
        "    background-color: #f8fbf8; "
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

    tripsTable->setColumnWidth(0, 100);
    tripsTable->setColumnWidth(1, 140);
    tripsTable->setColumnWidth(2, 500);
    tripsTable->setColumnWidth(3, 100);
    tripsTable->setColumnWidth(4, 100);
    tripsTable->setColumnWidth(5, 90);

    tripsTable->horizontalHeader()->setStretchLastSection(true);
    tripsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    tripsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    tripsTable->setAlternatingRowColors(true);
    tripsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tripsTable->verticalHeader()->setDefaultSectionSize(45);

    displayLayout->addWidget(tripsTable);

    mainContentLayout->addWidget(headerWidget);
    mainContentLayout->addWidget(tripDisplayArea);

    connect(tripsTable, &QTableWidget::itemDoubleClicked, this, &MainWindow::onViewTripDetailsClicked);
}

void MainWindow::setupMenuBar() {
    QMenu *fileMenu = menuBar()->addMenu("&File");
    QAction *importAction = new QAction("&Import Trips", this);
    fileMenu->addAction(importAction);
    connect(importAction, &QAction::triggered, this, &MainWindow::onImportTripsClicked);

    QAction *exportAction = new QAction("&Export Trips", this);
    fileMenu->addAction(exportAction);
    connect(exportAction, &QAction::triggered, this, &MainWindow::onExportTripsClicked);

    fileMenu->addSeparator();
    QAction *exitAction = new QAction("E&xit", this);
    fileMenu->addAction(exitAction);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    QMenu *editMenu = menuBar()->addMenu("&Edit");
    QAction *addTripAction = new QAction("&Add Trip", this);
    editMenu->addAction(addTripAction);
    connect(addTripAction, &QAction::triggered, this, &MainWindow::onAddTripClicked);

    QAction *editTripAction = new QAction("&Edit Trip", this);
    editMenu->addAction(editTripAction);
    connect(editTripAction, &QAction::triggered, this, &MainWindow::onEditTripClicked);

    QAction *deleteTripAction = new QAction("&Delete Trip", this);
    editMenu->addAction(deleteTripAction);
    connect(deleteTripAction, &QAction::triggered, this, &MainWindow::onDeleteTripClicked);

    QMenu *viewMenu = menuBar()->addMenu("&View");
    QAction *refreshAction = new QAction("&Refresh", this);
    viewMenu->addAction(refreshAction);
    connect(refreshAction, &QAction::triggered, this, &MainWindow::onRefreshViewClicked);

    QAction *filterAction = new QAction("&Filter Trips", this);
    viewMenu->addAction(filterAction);
    connect(filterAction, &QAction::triggered, this, &MainWindow::onFilterTripsClicked);

    QMenu *helpMenu = menuBar()->addMenu("&Help");
    QAction *aboutAction = new QAction("&About", this);
    helpMenu->addAction(aboutAction);
}

void MainWindow::setupStatusBar() { statusBar()->showMessage("Ready"); }

// ========================================
// CACHE & DATA MANAGEMENT
// ========================================
void MainWindow::loadCacheFromFile(vector<TRIP> &outputTrips) {
    try {
        if (cacheFileExists()) {
            size_t previousCount = outputTrips.size();

            loadTripCacheFile(outputTrips, personManager);

            size_t loadedCount = outputTrips.size() - previousCount;

            if (loadedCount > 0) {
                statusBar()->showMessage(QString("Loaded %1 trips from previous session").arg(loadedCount), 3000);
            } else {
                statusBar()->showMessage("Cache file found but appears to be empty", 3000);
            }
        } else {
            statusBar()->showMessage("No previous data found - Ready for new trips", 3000);
            outputTrips.clear();  // Ensure we start with empty list
        }
    } catch (const std::exception &e) {
        QMessageBox::warning(
            this, "Cache Load Error",
            QString("Failed to load cached trips: %1\n\nStarting with empty trip list.").arg(e.what()));
        outputTrips.clear();  // Clear any partially loaded data
        statusBar()->showMessage("Cache load failed - Starting fresh", 3000);
    }
}

void MainWindow::saveCacheToFile() {
    try {
        addDebugMessage("Updating cache file...");
        std::vector<TRIP> currentTrips = tripManager->getAllTrips();

        addDebugMessage(QString("Saving %1 trips to cache").arg(currentTrips.size()));

        updateCacheFile(currentTrips);

        addDebugMessage("Cache file updated successfully");

    } catch (const std::exception &e) {
        addDebugMessage(QString("Error saving cache: %1").arg(e.what()));
        QMessageBox::warning(this, "Cache Save Error", QString("Failed to save trips to cache: %1").arg(e.what()));
    }
}

// ========================================
// DISPLAY UPDATE FUNCTIONS
// ========================================

void MainWindow::updateTripDisplay(vector<TRIP> trips) {
    if (!tripsTable) {
        return;
    }

    tripsTable->setRowCount(trips.size());

    for (size_t i = 0; i < trips.size(); ++i) {
        const TRIP &trip = trips[i];

        tripsTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(trip.getID())));
        tripsTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(trip.getDestination())));
        tripsTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(trip.getDescription())));
        tripsTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(trip.getStartDate().toString())));
        tripsTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(trip.getEndDate().toString())));
        tripsTable->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(trip.getStatusString())));

        QString status = QString::fromStdString(trip.getStatusString());
        for (int j = 0; j < 6; ++j) {
            tripsTable->item(i, j)->setData(Qt::UserRole, status);
        }
    }

    updateStatusBar(trips);
}

void MainWindow::updateStatusBar(const vector<TRIP> trips) {
    if (statsLabel) {
        statsLabel->setText(QString("Trips count: %1").arg(trips.size()));
    }
    statusBar()->showMessage(QString("Ready - %1 trips").arg(trips.size()));
}

void MainWindow::addDebugMessage(const QString &message) {
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    Q_UNUSED(message);
}

// ========================================
// FILE OPERATIONS (First sidebar group)
// ========================================

void MainWindow::onImportTripsClicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Import Trip Information", getProjectPath(),
                                                    "JSON Files (*.json);;All Files (*)");

    string extension = fileName.section('.', -1).toStdString();
    if (!fileName.isEmpty()) {
        addDebugMessage("Starting import from: " + fileName);

        std::vector<TRIP> importedTrips;
        if (extension == "json") {
            importTripInfoFromJson(importedTrips, fileName.toStdString(), personManager);
        }

        for (const TRIP &trip : importedTrips) {
            tripManager->addTrip(trip);
        }

        addDebugMessage(QString("Import completed. %1 trips loaded.").arg(importedTrips.size()));
        QMessageBox::information(this, "Import Complete",
                                 QString("Successfully imported %1 trips.").arg(importedTrips.size()));
    }
}

void MainWindow::onExportTripsClicked() {
    std::vector<TRIP> currentTrips = tripManager->getAllTrips();

    if (currentTrips.empty()) {
        QMessageBox::warning(this, "No Data", "No trips to export. Please import trips first.");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(
        this, "Export Trip Information", getProjectPath() + "/trips_export.json", "JSON Files (*.json);;All Files (*)");

    if (!fileName.isEmpty()) {
        addDebugMessage("Exporting to: " + fileName);
        exportTripsInfoToJson(currentTrips, fileName.toStdString());
        addDebugMessage("Export completed successfully.");
        QMessageBox::information(this, "Export Complete",
                                 QString("Successfully exported %1 trips.").arg(currentTrips.size()));
    }
}

// ========================================
// TRIP MANAGEMENT (Second sidebar group)
// ========================================

void MainWindow::onAddTripClicked() {
    addDebugMessage("Opening Add Trip dialog...");

    AddTripDialog dialog(personManager, this);
    if (dialog.exec() == QDialog::Accepted) {
        TRIP newTrip = dialog.getTripData();
        tripManager->addTrip(newTrip);
        QMessageBox::information(
            this, "Trip Added",
            QString("Trip to %1 has been added successfully.").arg(QString::fromStdString(newTrip.getDestination())));
    } else {
        addDebugMessage("Add trip operation was cancelled.");
    }
}

void MainWindow::onEditTripClicked() {
    int currentRow = tripsTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a trip to edit.");
        return;
    }

    QString tripIdToEdit = tripsTable->item(currentRow, 0)->text();
    vector<TRIP> allTrips = tripManager->getAllTrips();

    auto it = find_if(allTrips.begin(), allTrips.end(),
                      [&](const TRIP &trip) { return trip.getID() == tripIdToEdit.toStdString(); });

    if (it != allTrips.end()) {
        EditTripDialog editDialog(*it, this);
        editDialog.setPersonManager(personManager);

        if (editDialog.exec() == QDialog::Accepted) {
            tripManager->updateTrip(editDialog.getOriginalTrip(), editDialog.getUpdatedTrip());
            addDebugMessage("Trip updated: " + tripIdToEdit);
        }
    }
}

void MainWindow::onDeleteTripClicked() {
    int currentRow = tripsTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a trip to delete.");
        return;
    }

    QString tripIdToDelete = tripsTable->item(currentRow, 0)->text();
    QString destination = tripsTable->item(currentRow, 1)->text();

    int ret = QMessageBox::question(this, "Delete Trip",
                                    QString("Are you sure you want to delete the trip to %1?").arg(destination),
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        bool success = tripManager->removeTrip(tripIdToDelete.toStdString());

        if (success) {
            QMessageBox::information(this, "Trip Deleted",
                                     QString("Trip to %1 has been deleted successfully.").arg(destination));
            addDebugMessage("Deleted trip with ID: " + tripIdToDelete);
        } else {
            addDebugMessage("Error: Could not find trip with ID to delete: " + tripIdToDelete);
            QMessageBox::critical(this, "Error", "Could not find the trip to delete. Please refresh.");
        }
    }
}

void MainWindow::onViewTripDetailsClicked() {
    int currentRow = tripsTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a trip to view details");
        return;
    }

    QString tripIdToView = tripsTable->item(currentRow, 0)->text();
    vector<TRIP> allTrips = tripManager->getAllTrips();

    auto it = find_if(allTrips.begin(), allTrips.end(),
                      [&](const TRIP &trip) { return trip.getID() == tripIdToView.toStdString(); });

    if (it != allTrips.end()) {
        ViewTripDialog dialog(*it, personManager, this);  // Pass by reference

        if (dialog.exec() == QDialog::Accepted) {
            TRIP originalTrip = dialog.getOriginalTrip();
            TRIP updatedTrip = dialog.getUpdatedTrip();

            bool isUpdated =
                ((originalTrip.getAllExpenses().size() != updatedTrip.getAllExpenses().size()) ||
                 (originalTrip.getMembers().size() != updatedTrip.getMembers().size()) ||
                 (originalTrip.getDescription() != updatedTrip.getDescription()) || (!(originalTrip == updatedTrip)));

            if (isUpdated) {
                tripManager->updateTrip(originalTrip, updatedTrip);
                updateTripDisplay(tripManager->getAllTrips());
                statusBar()->showMessage("Trip updated successfully.", 2000);
            } else {
                statusBar()->showMessage("No changes detected.", 2000);
            }

            saveCacheToFile();
        }
    }
}

// ========================================
// PEOPLE MANAGEMENT (Third sidebar group)
// ========================================

void MainWindow::onManagePeopleClicked() {
    addDebugMessage("Opening People Management dialog...");

    ManagePeopleDialog dialog(personManager, this);
    dialog.exec();

    addDebugMessage("People Management dialog closed.");
}

TRIPMANAGER *MainWindow::getTripManager() const { return tripManager; }

// ========================================
// VIEW AND FILTER (Fourth sidebar group)
// ========================================

void MainWindow::onFilterTripsClicked() {
    std::vector<TRIP> allTrips = tripManager->getAllTrips();
    FilterTripDialog filterDialog(allTrips, this);

    if (filterDialog.exec() == QDialog::Accepted) {
        std::vector<TRIP> filteredTrips = filterDialog.getFilteredTrips();
        updateTripDisplay(filteredTrips);
        updateStatusBar(filteredTrips);

        addDebugMessage(
            QString("Applied filters - showing %1 of %2 trips").arg(filteredTrips.size()).arg(allTrips.size()));

        statusBar()->showMessage(
            QString("Filtered view: %1 of %2 trips").arg(filteredTrips.size()).arg(allTrips.size()), 5000);
    }
}

void MainWindow::onShowUpcomingTripsClicked() {
    std::vector<TRIP> allTrips = tripManager->getAllTrips();
    std::vector<TRIP> upcomingTrips;
    for (const TRIP &trip : allTrips) {
        if (trip.getStatusString() == "Planned") {
            upcomingTrips.push_back(trip);
        }
    }
    updateTripDisplay(upcomingTrips);
}

void MainWindow::onShowCompletedTripsClicked() {
    std::vector<TRIP> allTrips = tripManager->getAllTrips();
    std::vector<TRIP> completedTrips;
    for (const TRIP &trip : allTrips) {
        if (trip.getStatusString() == "Completed") {
            completedTrips.push_back(trip);
        }
    }
    updateTripDisplay(completedTrips);
}

void MainWindow::onRefreshViewClicked() {
    updateTripDisplay(tripManager->getAllTrips());
    tripsTable->clearSelection();
    tripsTable->setCurrentItem(nullptr);
    addDebugMessage("View refreshed by user.");
    statusBar()->showMessage("View refreshed.", 2000);
}

// ========================================
// HELP & SETTINGS
// ========================================

// Observer implementation
void MainWindow::onTripAdded(const std::string &tripId) {
    addDebugMessage("Observer: Trip added - " + QString::fromStdString(tripId));

    std::vector<TRIP> currentTrips = tripManager->getAllTrips();
    updateTripDisplay(currentTrips);
    saveCacheToFile();

    statusBar()->showMessage(QString("New trip added: %1").arg(QString::fromStdString(tripId)), 3000);
}

void MainWindow::onTripRemoved(const std::string &tripId) {
    addDebugMessage("Observer: Trip removed - " + QString::fromStdString(tripId));

    std::vector<TRIP> currentTrips = tripManager->getAllTrips();
    updateTripDisplay(currentTrips);
    saveCacheToFile();

    statusBar()->showMessage(QString("Trip removed: %1").arg(QString::fromStdString(tripId)), 3000);
}

void MainWindow::onTripUpdated(const std::string &tripId) {
    addDebugMessage("Observer: Trip updated - " + QString::fromStdString(tripId));

    vector<TRIP> currentTrips = tripManager->getAllTrips();
    updateTripDisplay(currentTrips);
    saveCacheToFile();

    statusBar()->showMessage(QString("Trip updated: %1").arg(QString::fromStdString(tripId)), 3000);
}

void MainWindow::onPersonAdded(const string &personID) {
    addDebugMessage("Person added: " + QString::fromStdString(personID));
}

void MainWindow::onPersonRemoved(const string &personID) {
    addDebugMessage("Person removed: " + QString::fromStdString(personID));
}

void MainWindow::onPersonUpdated(const string &personID) {
    addDebugMessage("Person updated: " + QString::fromStdString(personID));
}

void MainWindow::onImportPeopleClicked() {
    addDebugMessage("Opening People Import dialog...");

    QString filename = QFileDialog::getOpenFileName(this, "Import People from file", getProjectPath(),
                                                    "JSON Files (*.json);; All Files (*.*)");

    string extension = filename.section('.', -1).toStdString();
    if (filename.isEmpty()) {
        return;  // User canceled
    }

    try {
        vector<MEMBER> importedMembers;
        vector<HOST> importedHosts;

        if (extension == "json") {
            importPeopleInfoFromJson(importedMembers, importedHosts, filename.toStdString());
        } else {
            QMessageBox::warning(this, "Invalid File Type", "Please select a valid CSV file for importing people.");
            return;
        }

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

        QMessageBox::information(
            this, "Import Successful",
            QString("Successfully imported %1 people.").arg(importedMembers.size() + importedHosts.size()));

        addDebugMessage(QString("Imported %1 people from %2")
                            .arg(importedMembers.size() + importedHosts.size())
                            .arg(QFileInfo(filename).fileName()));

    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Import Error", QString("An error occurred during import: %1").arg(e.what()));
        addDebugMessage(QString("Error importing people: %1").arg(e.what()));
    }
}

void MainWindow::onExportPeopleClicked() {
    addDebugMessage("Opening People Export dialog...");

    const vector<MEMBER> &members = personManager->getAllMembers();
    const vector<HOST> &hosts = personManager->getAllHosts();

    if (members.empty() && hosts.empty()) {
        QMessageBox::warning(this, "Export Failed", "There are no people to export.");
        return;
    }

    QString filename =
        QFileDialog::getSaveFileName(this, "Export People to JSON", getProjectPath() + "/people_export.json",
                                     "JSON Files (*.json);;All Files (*.*)");

    if (filename.isEmpty()) {
        return;  // User canceled
    }

    try {
        exportPeopleInfoToJson(members, hosts, filename.toStdString());

        int totalPeople = members.size() + hosts.size();
        QMessageBox::information(
            this, "Export Successful",
            QString("Successfully exported %1 people to %2.").arg(totalPeople).arg(QFileInfo(filename).fileName()));

        addDebugMessage(QString("Exported %1 people to %2").arg(totalPeople).arg(QFileInfo(filename).fileName()));

    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Export Error", QString("An error occurred during export: %1").arg(e.what()));
        addDebugMessage(QString("Error exporting people: %1").arg(e.what()));
    }
}
