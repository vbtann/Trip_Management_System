#include "Observer.h"

using namespace std;

void SUBJECT::addObserver(OBSERVER *observer) { observers.push_back(observer); }

void SUBJECT::removeObserver(OBSERVER *observer) {
    for (size_t i = 0; i < observers.size(); ++i) {
        if (observers[i] == observer) {
            observers.erase(observers.begin() + i);
            break;
        }
    }
}

void SUBJECT::notifyTripAdded(const string &tripID) {
    for (size_t i = 0; i < observers.size(); ++i) {
        observers[i]->onTripAdded(tripID);
    }
}

void SUBJECT::notifyTripRemoved(const string &tripID) {
    for (size_t i = 0; i < observers.size(); ++i) {
        observers[i]->onTripRemoved(tripID);
    }
}

void SUBJECT::notifyTripUpdated(const string &tripID) {
    for (size_t i = 0; i < observers.size(); ++i) {
        observers[i]->onTripUpdated(tripID);
    }
}

void SUBJECT::notifyPersonAdded(const string &personID) {
    for (size_t i = 0; i < observers.size(); ++i) {
        observers[i]->onPersonAdded(personID);
    }
}

void SUBJECT::notifyPersonRemoved(const string &personID) {
    for (size_t i = 0; i < observers.size(); ++i) {
        observers[i]->onPersonRemoved(personID);
    }
}

void SUBJECT::notifyPersonUpdated(const string &personID) {
    for (size_t i = 0; i < observers.size(); ++i) {
        observers[i]->onPersonUpdated(personID);
    }
}