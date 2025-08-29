#include "header.h"

using namespace std;

// FUNC: Constructor
HOST::HOST() : PERSON(), emergencyContact("") {}

HOST::HOST(const string &_id, const string &_fullName, const GENDER &_gender, const DATE &_dob)
    : PERSON(_id, _fullName, _gender, _dob) {}

// FUNC: Getters
vector<string> HOST::getHostedTripIDs() const { return this->hostedTripID; }

string HOST::getEmergencyContact() const { return this->emergencyContact; }

string HOST::getRole() const { return "Host"; }

string HOST::getInfo() const {
    return this->fullName + " - " + to_string(this->getAge()) + " - " + genderToString(this->gender) +
           " - Hosted: " + to_string(this->hostedTripID.size()) + " trips";
}

// FUNC: Setters
void HOST::setEmergencyContact(const string &_contact) { this->emergencyContact = _contact; }

// FUNC: Utility methods
bool HOST::hasHostedTrip(const string &_tripID) {
    return find(this->hostedTripID.begin(), this->hostedTripID.end(), _tripID) != this->hostedTripID.end();
}
void HOST::hostTrip(const string &_tripID) {
    if (!this->hasHostedTrip(_tripID)) {
        this->hostedTripID.push_back(_tripID);
    }
}

// Operators overloading
HOST &HOST::operator=(const HOST &other) {
    if (this != &other) {
        PERSON::operator=(other);
        this->hostedTripID = other.hostedTripID;
        this->emergencyContact = other.emergencyContact;
    }
    return *this;
}

bool HOST::operator==(const HOST &other) const { return this->ID == other.ID && this->fullName == other.fullName; }