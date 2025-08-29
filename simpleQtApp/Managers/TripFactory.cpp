#include "TripFactory.h"

#include <iomanip>
#include <sstream>

string TRIPFACTORY::generateTripID(const string& destination, const DATE& startDate) {
    string id;
    bool isNewWord = true;

    for (char c : destination) {
        if (isspace(c)) {
            isNewWord = true;
        } else if (isNewWord) {
            id += toupper(c);
            isNewWord = false;
        }
    }

    ostringstream dateStream;
    dateStream << setfill('0') << setw(2) << startDate.getMonth() << setfill('0') << setw(2) << startDate.getDay();

    id += "_" + dateStream.str();

    return id;
}

TRIP TRIPFACTORY::createTrip(const string& destination, const string& description, const DATE& startDate,
                             const DATE& endDate, STATUS status, const vector<EXPENSE>& expenses,
                             long long totalExpense) {
    string tripID = generateTripID(destination, startDate);
    return TRIP(tripID, destination, description, startDate, endDate, status, expenses, totalExpense);
}
