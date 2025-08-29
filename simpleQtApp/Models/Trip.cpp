#include "header.h"

int TRIP::tripCount = 0;

// FUNC: Constructors
TRIP::TRIP()
    : ID(""),
      Destination(""),
      Description(""),
      startDate(1, 1, 2024),
      endDate(1, 1, 2024),
      status(STATUS::Planned),
      expenses(vector<EXPENSE>()),
      totalExpense(0) {
    tripCount++;
}

TRIP::TRIP(const TRIP &other)
    : ID(other.ID),
      Destination(other.Destination),
      Description(other.Description),
      startDate(other.startDate),
      endDate(other.endDate),
      status(other.status),
      members(other.members),
      host(other.host),
      expenses(other.expenses),
      totalExpense(other.totalExpense) {}

TRIP::TRIP(const string &_tripID, const string &_dest, const string &_desc, int _startDay, int _startMonth,
           int _startYear, int _endDay, int _endMonth, int _endYear, const STATUS &_status,
           const vector<EXPENSE> &_expenses, long long _totalExpense)
    : ID(_tripID),  // Use the provided ID directly
      Destination(toUpper(_dest)),
      Description(_desc),
      startDate(_startDay, _startMonth, _startYear),
      endDate(_endDay, _endMonth, _endYear),
      status(_status),
      expenses(_expenses),
      totalExpense(_totalExpense) {
    tripCount++;
}

TRIP::TRIP(const string &_tripID, const string &_dest, const string &_desc, const DATE &_startDate,
           const DATE &_endDate, const STATUS &_status, const vector<EXPENSE> &_expenses, long long _totalExpense)
    : ID(_tripID),  // Use the provided ID directly
      Destination(toUpper(_dest)),
      Description(_desc),
      startDate(_startDate),
      endDate(_endDate),
      status(_status),
      expenses(_expenses),
      totalExpense(_totalExpense) {
    tripCount++;
}

// FUNC: Getters
string TRIP::getID() const { return this->ID; }

string TRIP::getDestination() const { return this->Destination; }

string TRIP::getDescription() const { return this->Description; }

DATE TRIP::getStartDate() const { return this->startDate; }

DATE TRIP::getEndDate() const { return this->endDate; }

STATUS TRIP::getStatus() const { return this->status; }

string TRIP::getStatusString() const {
    switch (this->status) {
        case STATUS::Planned:
            return "Planned";
        case STATUS::Ongoing:
            return "Ongoing";
        case STATUS::Completed:
            return "Completed";
        case STATUS::Cancelled:
            return "Cancelled";
        default:
            return "Unknown";
    }
}

vector<EXPENSE> TRIP::getAllExpenses() const { return this->expenses; }

long long TRIP::getTotalExpense() const { return this->totalExpense; }

HOST TRIP::getHost() const { return this->host; }

vector<MEMBER> TRIP::getMembers() const { return this->members; }

// FUNC: Setters
void TRIP::setID(const std::string &_ID) { this->ID = _ID; }

void TRIP::setDestination(const std::string &_destination) { this->Destination = _destination; }

void TRIP::setDescription(const string &_description) { this->Description = _description; }

void TRIP::setStartDate(const DATE &_startDate) { this->startDate = _startDate; }

void TRIP::setEndDate(const DATE &_endDate) { this->endDate = _endDate; }

void TRIP::setStatus(const STATUS &_status) { this->status = _status; }

void TRIP::setTotalExpense(long long _amount) { this->totalExpense = _amount; }

void TRIP::setExpenses(const vector<EXPENSE> &_expenses) {
    // this->totalExpense = 0;
    // this->expenses.clear();
    // for (const EXPENSE &expense : _expenses) {
    //     this->expenses.push_back(expense);
    //     this->totalExpense += expense.getAmount();
    // }
    this->expenses = _expenses;
}

// FUNC: Utility methods
void TRIP::addMember(const MEMBER &member) {
    auto it = find_if(members.begin(), members.end(),
                      [&](const MEMBER &_member) { return member.getID() == _member.getID(); });

    // Add member to trip if not exist
    if (it == members.end()) {
        members.push_back(member);
    }
}

void TRIP::setMembers(const vector<MEMBER> &members) {
    // this->members.clear();
    // for (const MEMBER &member : members) {
    //     this->members.push_back(member);
    // }
    this->members = members;
}

void TRIP::setHost(const HOST &_host) { this->host = _host; }

bool TRIP::hasHost() const { return !this->host.getID().empty(); }

void TRIP::addExpense(const EXPENSE &expense) {
    if (expense.getAmount() <= 0) {
        return;
    }

    this->expenses.push_back(expense);
    this->totalExpense += expense.getAmount();
}

// Operators overloading
TRIP &TRIP::operator=(const TRIP &other) {
    this->ID = other.ID;
    this->Description = other.Description;
    this->Destination = other.Destination;
    this->startDate = other.startDate;
    this->endDate = other.endDate;
    this->status = other.status;
    this->members = other.members;
    this->host = other.host;
    this->expenses = other.expenses;
    this->totalExpense = other.totalExpense;

    return *this;
}

bool TRIP::operator==(const TRIP &other) const {
    return (this->ID == other.ID && this->Description == other.Description && this->Destination == other.Destination &&
            this->startDate == other.startDate && this->endDate == other.endDate && this->status == other.status &&
            this->members == other.members && this->host == other.host);
}

// FUNC: Output
ostream &operator<<(ostream &os, const TRIP &trip) {
    os << trip.ID << endl
       << trip.Destination << endl
       << trip.Description << endl
       << trip.startDate << endl
       << trip.endDate << endl
       << trip.getStatusString() << endl;

    return os;
}
