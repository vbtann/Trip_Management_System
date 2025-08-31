#ifndef TRIPMANAGER_H
#define TRIPMANAGER_H

#include <string>
#include <vector>

#include "../Models/header.h"
#include "Observer.h"

using namespace std;

class TRIPMANAGER : public SUBJECT {
   private:
    vector<TRIP> trips;

   public:
    void addTrip(const TRIP &trip);
    bool removeTrip(const string &tripID);
    bool updateTrip(const TRIP &originalTrip, const TRIP &updatedTrip);
    const vector<TRIP> &getAllTrips() const;
    TRIP *findTripById(const string &id);
    size_t getTripCount() const;
};

#endif  // TRIPMANAGER_H