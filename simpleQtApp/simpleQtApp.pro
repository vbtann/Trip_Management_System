QT += core widgets gui

CONFIG += c++17 debug_and_release 

TARGET = simpleQtApp
TEMPLATE = app

INCLUDEPATH += UI Models Managers include

# Main application file
SOURCES += main.cpp

# UI files
SOURCES += UI/MainWindow.cpp \
    UI/AddTripDialog.cpp \
    UI/FilterTripDialog.cpp \
    UI/ViewTripDialog.cpp \
    UI/EditTripDialog.cpp \
    UI/AddPersonDialog.cpp \
    UI/ManagePeopleDialog.cpp \
    UI/EditPersonDialog.cpp \
    UI/AddExpenseDialog.cpp \
    UI/ViewExpenseDialog.cpp \

# Model files  
SOURCES += Models/Date.cpp \
    Models/Gender.cpp \
    Models/Host.cpp \
    Models/Person.cpp \
    Models/Member.cpp \
    Models/Status.cpp \
    Models/Trip.cpp \
    Models/Expense.cpp \
    Models/Category.cpp \
    Models/Utility_Functions.cpp

# Manager files
SOURCES += Managers/FileManager_Trip.cpp \
    Managers/FileManager_People.cpp \
    Managers/TripManager.cpp \
    Managers/Observer.cpp \
    Managers/PersonFactory.cpp \
    Managers/TripFactory.cpp \
    Managers/PersonManager.cpp

# Header files
HEADERS += UI/MainWindow.h \
    UI/AddTripDialog.h \
    UI/FilterTripDialog.h \
    UI/ViewTripDialog.h \
    UI/EditTripDialog.h \
    UI/AddPersonDialog.h \
    UI/ManagePeopleDialog.h \
    UI/EditPersonDialog.h \
    UI/AddExpenseDialog.h \
    UI/ViewExpenseDialog.h \
    Models/header.h \
    Managers/FileManager.h \
    Managers/TripManager.h \
    Managers/Observer.h \
    Managers/PersonFactory.h \
    Managers/TripFactory.h \
    Managers/PersonManager.h

# Compiler definitions
DEFINES += QT_DEPRECATED_WARNINGS

# Debug configuration
CONFIG(debug, debug|release) {
    DESTDIR = debug
    TARGET = simpleQtApp
}

# Release configuration  
CONFIG(release, debug|release) {
    DESTDIR = release
    TARGET = simpleQtApp
}

# Additional compiler flags for better debugging
QMAKE_CXXFLAGS += -Wall -Wextra