#include "header.h"

using namespace std;

// FUNC: Constructors
MEMBER::MEMBER()
    : PERSON(),
      emergencyContact(""),
      hasDriverLicense(false),
      totalSpent(0),
      spendings(vector<pair<string, EXPENSE>>()) {}

MEMBER::MEMBER(const string &_id, const string &_fullName, const GENDER &_gender, const DATE &_dob)
    : PERSON(_id, _fullName, _gender, _dob),
      emergencyContact(""),
      hasDriverLicense(false),
      totalSpent(0),
      spendings(vector<pair<string, EXPENSE>>()) {}

MEMBER::MEMBER(const MEMBER &other)
    : PERSON(other),
      joinedTripID(other.joinedTripID),
      emergencyContact(other.emergencyContact),
      hasDriverLicense(other.hasDriverLicense),
      interests(other.interests),
      totalSpent(other.totalSpent),
      spendings(other.spendings) {}

// FUNC: Getters
vector<string> MEMBER::getJoinedTripIDs() const { return this->joinedTripID; }

string MEMBER::getLastJoinedTripID() const {
    if (this->joinedTripID.empty()) {
        return "";
    }
    return this->joinedTripID[joinedTripID.size() - 1];
}

int MEMBER::getJoinedTripCount() const { return this->joinedTripID.size(); }

string MEMBER::getEmergencyContact() const { return this->emergencyContact; }

bool MEMBER::getHasDriverLicense() const { return this->hasDriverLicense; }

vector<string> MEMBER::getInterests() const { return this->interests; }

long long MEMBER::getTotalSpent() const { return (this->totalSpent > 0) ? this->totalSpent : 0; }

string MEMBER::getRole() const { return "Member"; }

string MEMBER::getInfo() const { return this->ID + "  -  " + this->fullName + "  -  " + to_string(this->getAge()); }

vector<pair<string, EXPENSE>> MEMBER::getSpendings() const { return this->spendings; }

vector<pair<string, EXPENSE>> MEMBER::getSpendingsForTrip(const string &tripID) const {
    vector<pair<string, EXPENSE>> filteredSpendings;
    for (const pair<string, EXPENSE> &spending : this->spendings) {
        if (spending.first == tripID) {
            filteredSpendings.push_back(spending);
        }
    }
    return filteredSpendings;
}

// FUNC: Setters
void MEMBER::setEmergencyContact(const string &_contact) { this->emergencyContact = _contact; }

void MEMBER::setSpendings(const vector<pair<string, EXPENSE>> &_spendings) {
    this->spendings = _spendings;
    this->totalSpent = 0;
    for (const auto &spending : this->spendings) {
        this->totalSpent += spending.second.getAmount();
    }
}

void MEMBER::setTotalSpent(long long _totalSpent) { this->totalSpent = _totalSpent; }

// FUNC: Utility methods
void MEMBER::addToTotalSpent(const long long _amount) { this->totalSpent += _amount; }

void MEMBER::addSpending(const EXPENSE &_expense, const TRIP &trip) {
    this->spendings.push_back(make_pair(trip.getID(), _expense));
    this->addToTotalSpent(_expense.getAmount());
}

void MEMBER::addInterest(const string &_interest) {
    if (find(this->interests.begin(), this->interests.end(), _interest) == this->interests.end()) {
        this->interests.push_back(_interest);
    }
}

// NOTE: Check if member has joined this trip or not
bool MEMBER::hasJoinedTrip(const string &_tripID) {
    return find(this->joinedTripID.begin(), this->joinedTripID.end(), _tripID) != joinedTripID.end();
}

// NOTE: Add trip ID to member joined trips list
void MEMBER::joinTrip(const string &_tripID) {
    if (!this->hasJoinedTrip(_tripID)) {
        this->joinedTripID.push_back(_tripID);
    }
}

// NOTE: Remove trip from member joined trips list
void MEMBER::leaveTrip(const string &_tripID) {
    auto it = find(this->joinedTripID.begin(), this->joinedTripID.end(), _tripID);
    if (it != this->joinedTripID.end()) {
        joinedTripID.erase(it);
    }
}

void MEMBER::removeSpending(const string &tripID, const EXPENSE &expenseToRemove) {
    auto it = this->spendings.begin();
    while (it != this->spendings.end()) {
        if (it->first == tripID && it->second == expenseToRemove) {
            this->totalSpent -= it->second.getAmount();
            if (this->totalSpent < 0) {
                this->totalSpent = 0;
            }

            it = this->spendings.erase(it);
            break;
        } else {
            ++it;
        }
    }
}

void MEMBER::clearSpendings() {
    this->spendings.clear();
    this->totalSpent = 0;
}

// Operators overloading
MEMBER &MEMBER::operator=(const MEMBER &other) {
    if (this != &other) {
        PERSON::operator=(other);
        this->joinedTripID = other.joinedTripID;
        this->emergencyContact = other.emergencyContact;
        this->hasDriverLicense = other.hasDriverLicense;
        this->interests = other.interests;
        this->totalSpent = other.totalSpent;
        this->spendings.clear();
        this->spendings = other.spendings;
    }

    return *this;
}

bool MEMBER::operator==(const MEMBER &other) const { return this->ID == other.ID; }