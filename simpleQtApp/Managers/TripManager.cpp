#include "TripManager.h"

using namespace std;

void TRIPMANAGER::addTrip(const TRIP &trip) {
    trips.push_back(trip);
    notifyTripAdded(trip.getID());
}

bool TRIPMANAGER::removeTrip(const string &tripID) {
    for (size_t i = 0; i < trips.size(); ++i) {
        if (this->trips[i].getID() == tripID) {
            this->trips.erase(this->trips.begin() + i);
            notifyTripRemoved(tripID);
            return true;
        }
    }
    return false;
}

bool TRIPMANAGER::updateTrip(const TRIP &originalTrip, const TRIP &updatedTrip) {
    for (size_t i = 0; i < this->trips.size(); ++i) {
        if (this->trips[i].getID() == originalTrip.getID()) {
            this->trips[i] = updatedTrip;
            notifyTripUpdated(updatedTrip.getID());
            return true;
        }
    }
    return false;
}

const vector<TRIP> &TRIPMANAGER::getAllTrips() const { return this->trips; }

TRIP *TRIPMANAGER::findTripById(const string &id) {
    for (size_t i = 0; i < this->trips.size(); ++i) {
        if (this->trips[i].getID() == id) {
            return &this->trips[i];
        }
    }
    return nullptr;
}

size_t TRIPMANAGER::getTripCount() const { return this->trips.size(); }