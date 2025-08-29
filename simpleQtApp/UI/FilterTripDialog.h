#ifndef FILTERTRIPDIALOG_H
#define FILTERTRIPDIALOG_H

#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>
#include <set>
#include <vector>

#include "Models/header.h"

class FilterTripDialog : public QDialog {
    Q_OBJECT

   public:
    explicit FilterTripDialog(const std::vector<TRIP> &allTrips, QWidget *parent = nullptr);
    std::vector<TRIP> getFilteredTrips() const;

   private slots:
    void applyAndClose();
    void applyFilters();
    void clearFilters();
    void onPresetFilterChanged();

   private:
    void setupUI();
    void setupDestinationFilter();
    void setupDateFilter();
    void setupStatusFilter();
    void setupDescriptionFilter();
    void setupSortingOptions();
    void setupButtons();

    bool matchesFilters(const TRIP &trip) const;
    std::vector<TRIP> sortTrips(std::vector<TRIP> trips) const;

    std::vector<TRIP> _allTrips;
    std::vector<TRIP> _filteredTrips;

    QGroupBox *destinationGroup;
    QGroupBox *dateGroup;
    QGroupBox *statusGroup;
    QGroupBox *descriptionGroup;
    QGroupBox *sortGroup;
    QGroupBox *presetGroup;

    QLineEdit *destinationLineEdit;
    QCheckBox *destinationExactMatch;
    QComboBox *destinationComboBox;

    QCheckBox *enableDateFilter;
    QDateEdit *startDateFrom;
    QDateEdit *startDateTo;
    QDateEdit *endDateFrom;
    QDateEdit *endDateTo;
    QComboBox *dateRangePreset;

    QCheckBox *statusPlanned;
    QCheckBox *statusOngoing;
    QCheckBox *statusCompleted;
    QCheckBox *statusCancelled;

    QLineEdit *descriptionKeywords;
    QCheckBox *descriptionCaseSensitive;

    QComboBox *sortByComboBox;
    QRadioButton *sortAscending;
    QRadioButton *sortDescending;

    QComboBox *presetFiltersCombo;

    QPushButton *applyButton;
    QPushButton *clearButton;
    QPushButton *cancelButton;

    QLabel *resultsLabel;
};

#endif