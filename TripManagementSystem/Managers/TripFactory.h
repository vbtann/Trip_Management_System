#ifndef TRIPFACTORY_H
#define TRIPFACTORY_H

#include "../Models/header.h"

using namespace std;

class TRIPFACTORY {
   private:
    static int nextTripNumber;

   public:
    static TRIP createTrip(const string& destination, const string& description, const DATE& startDate,
                           const DATE& endDate, STATUS status, const vector<EXPENSE>& expenses, long long totalExpense);

    static string generateTripID(const string& destination, const DATE& startDate);
};

#endif  // TRIPFACTORY_H
