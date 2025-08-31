#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QApplication>
#include <QComboBox>
#include <QDateEdit>
#include <QDateTime>
#include <QDialog>
#include <QDir>
#include <QFileDialog>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QProgressBar>
#include <QPushButton>
#include <QRect>
#include <QScrollArea>
#include <QSplitter>
#include <QStackedLayout>
#include <QStatusBar>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QTimer>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>
#include <algorithm>
#include <iostream>
#include <vector>

#include "../Managers/FileManager.h"
#include "../Managers/Observer.h"
#include "../Managers/PersonManager.h"
#include "../Managers/TripManager.h"
#include "../Models/header.h"
#include "ManagePeopleDialog.h"

void saveTripAttendeesToCache(const vector<TRIP> &trips, const string &filePath);

class TRIP;
class TRIPMANAGER;
class AddTripDialog;
class FilterTripDialog;

class MainWindow : public QMainWindow, public OBSERVER {
    Q_OBJECT

   public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    TRIPMANAGER *getTripManager() const;

    // Observer pattern methods
    void onTripAdded(const string &tripID) override;
    void onTripRemoved(const string &tripID) override;
    void onTripUpdated(const string &tripID) override;
    void onPersonAdded(const string &personID) override;
    void onPersonRemoved(const string &personID) override;
    void onPersonUpdated(const string &personID) override;

   private slots:
    // File Operations
    void onImportTripsClicked();
    void onExportTripsClicked();

    // Trip Management
    void onAddTripClicked();
    void onEditTripClicked();
    void onDeleteTripClicked();
    void onViewTripDetailsClicked();

    // People Management
    void onManagePeopleClicked();

    // View and Filter
    void onFilterTripsClicked();
    void onRefreshViewClicked();
    void onShowUpcomingTripsClicked();
    void onShowCompletedTripsClicked();

    // People import/export functions
    void onImportPeopleClicked();
    void onExportPeopleClicked();

   private:
    void setupUI();
    void setupMenuBar();
    void setupStatusBar();
    void setupSidebar();
    void setupMainContent();
    void updateTripDisplay(const vector<TRIP> trips);
    void updateStatusBar(const vector<TRIP> trips);
    void addDebugMessage(const QString &message);
    void loadCacheFromFile(vector<TRIP> &outputTrips);
    void saveCacheToFile();

    // UI Components
    QWidget *centralWidget;
    QSplitter *mainSplitter;

    // Sidebar Components
    QWidget *sidebar;
    QVBoxLayout *sidebarLayout;
    QGroupBox *quickActionsGroup;
    QGroupBox *managementGroup;
    QGroupBox *peopleGroup;
    QGroupBox *viewGroup;

    // Main Content Area
    QWidget *mainContent;
    QVBoxLayout *mainContentLayout;
    QWidget *headerWidget;
    QWidget *tripDisplayArea;
    QTableWidget *tripsTable;

    // Header Components
    QLabel *titleLabel;
    QLabel *statsLabel;
    QPushButton *refreshButton;

    // Action Buttons
    QPushButton *importButton;
    QPushButton *exportButton;
    QPushButton *addTripButton;
    QPushButton *editTripButton;
    QPushButton *deleteButton;
    QPushButton *detailViewButton;
    QPushButton *manageMembersButton;
    QPushButton *manageHostsButton;
    QPushButton *memberStatsButton;
    QPushButton *filterButton;
    QPushButton *addPersonButton;
    QPushButton *editPersonButton;
    QPushButton *deletePersonButton;
    QPushButton *viewPersonButton;

    // Add these buttons
    QPushButton *importPeopleButton;
    QPushButton *exportPeopleButton;

    // Status Components
    QLabel *statusLabel;
    QLabel *tripCountLabel;
    QProgressBar *progressBar;

    // Data
    PERSONMANAGER *personManager;
    TRIPMANAGER *tripManager;

    // Helper function to get project path (relative to executable)
    QString getProjectPath() const {
        QDir currentDir = QDir::current();
        currentDir.cdUp();
        return currentDir.absolutePath();
    }
};

#endif