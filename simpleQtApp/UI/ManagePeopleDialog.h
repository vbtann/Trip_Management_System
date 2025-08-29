#ifndef MANAGEPEOPLEDIALOG_H
#define MANAGEPEOPLEDIALOG_H

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QDialog>
#include <QFileDialog>
#include <QFormLayout>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QRect>
#include <QScreen>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QVariant>

#include "../Managers/Observer.h"
#include "../Managers/PersonManager.h"
#include "../Models/header.h"
#include "EditPersonDialog.h"
#include "MainWindow.h"

class MainWindow;
class TRIPMANAGER;

class ManagePeopleDialog : public QDialog {
    Q_OBJECT

   public:
    explicit ManagePeopleDialog(PERSONMANAGER* personManager, QWidget* parent = nullptr);

   private slots:
    void onAddPersonClicked();
    void onEditPersonClicked();
    void onDeletePersonClicked();
    void onViewPersonClicked();
    void onPersonSelectionChanged();
    void onRefreshClicked();
    void onCloseClicked();
    void onImportPeopleClicked();
    void onExportPeopleClicked();

   private:
    void updatePeopleData();
    void setupUI();
    void styleComponents();
    void refreshPersonList();
    void updatePersonInfo();
    void showPersonDetailDialog(PERSON* person);
    QString formatCurrency(long long amount);

    PERSONMANAGER* personManager;

    QListWidget* peopleListWidget;
    QLabel* personCountLabel;
    QLabel* memberCountLabel;
    QLabel* hostCountLabel;
    QLabel* selectedPersonInfoLabel;

    QPushButton* addPersonButton;
    QPushButton* editPersonButton;
    QPushButton* deletePersonButton;
    QPushButton* viewPersonButton;
    QPushButton* refreshButton;
    QPushButton* closeButton;
    QPushButton* importPeopleButton;
    QPushButton* exportPeopleButton;
};

#endif