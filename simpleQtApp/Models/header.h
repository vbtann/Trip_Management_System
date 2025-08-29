#ifndef HEADER_H
#define HEADER_H

#define enl "\n"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

// Forward declarations
class PERSON;
class MEMBER;
class HOST;
class EXPENSE;
class TRIP;

// FUNC: toUpper
string toUpper(const string &str);

// CLASS: STATUS
enum class STATUS { Planned, Ongoing, Completed, Cancelled };
const string statusToString(STATUS status);
STATUS stringToStatus(const string &statusStr);

// CLASS: GENDER
enum class GENDER { Male, Female };
const string genderToString(GENDER gender);
GENDER stringToGender(const string &genderString);

// CLASS: CATEGORY
enum class CATEGORY { Food, Drink, Gas, Accommodation, Others };
const string categoryToString(CATEGORY _category);
CATEGORY stringToCategory(const string &_categoryStr);

// CLASS: DATE
class DATE {
   private:
    int day, month, year;

   public:
    DATE();
    DATE(int _day, int _month, int _year);
    DATE(const DATE &date);
    ~DATE() = default;

    // FUNC: Getters
    int getDay() const;
    int getMonth() const;
    int getYear() const;

    // FUNC: Setters
    void setDate(int _day, int _month, int _year);

    // FUNC: Utility methods
    string toString() const;

    // Operators overloading
    DATE &operator=(const DATE &other);
    bool operator==(const DATE &other) const;
    bool operator<(const DATE &rhs) const;
    friend ostream &operator<<(ostream &, const DATE &);
};

// CLASS: PERSON
class PERSON {
   protected:
    string fullName, ID;
    string email, phoneNumber, address;
    GENDER gender;
    DATE dateOfBirth;

   public:
    PERSON();
    PERSON(const string &_id, const string &_fullName, const GENDER &_gender, const DATE &_dob);

    virtual ~PERSON() = default;
    virtual string getRole() const;
    virtual string getInfo() const;

    // FUNC: Getters
    virtual string getFullName() const;
    virtual string getID() const;
    virtual string getEmail() const;
    virtual string getPhoneNumber() const;
    virtual string getAddress() const;
    virtual GENDER getGender() const;
    virtual DATE getDateOfBirth() const;
    virtual string toString() const;

    // FUNC: Setters
    void setFullName(const string &_fullName);
    void setEmail(const string &_email);
    void setPhoneNumber(const string &_phoneNumber);
    void setAddress(const string &_address);
    void setGender(const GENDER &_gender);
    void setDOB(const DATE &_dob);
    void setID(const string &_ID);
    void setDateOfBirth(const DATE &_dob);

    // FUNC: Utility methods
    int getAge() const;
    string getContactInfo() const;
};

// CLASS: MEMBER
class MEMBER : public PERSON {
   private:
    vector<string> joinedTripID;
    string emergencyContact;
    bool hasDriverLicense;
    vector<string> interests;
    long long totalSpent;
    vector<pair<string, EXPENSE>> spendings;

   public:
    MEMBER();
    MEMBER(const string &_id, const string &_fullName, const GENDER &_gender, const DATE &_dob);
    MEMBER(const MEMBER &other);

    // FUNC: Getters
    vector<string> getJoinedTripIDs() const;
    string getLastJoinedTripID() const;
    int getJoinedTripCount() const;
    string getEmergencyContact() const;
    bool getHasDriverLicense() const;
    vector<string> getInterests() const;
    long long getTotalSpent() const;
    string getRole() const override;
    string getInfo() const override;
    vector<pair<string, EXPENSE>> getSpendings() const;
    vector<pair<string, EXPENSE>> getSpendingsForTrip(const string &tripID) const;

    // FUNC: Setters
    void setEmergencyContact(const string &_contact);
    void setSpendings(const vector<pair<string, EXPENSE>> &_spendings);
    void setTotalSpent(long long _totalSpent);
    void clearSpendings();

    // FUNC: Utility methods
    void addToTotalSpent(const long long _amount);
    void addSpending(const EXPENSE &_expense, const TRIP &trip);
    void addInterest(const string &_interest);
    void joinTrip(const string &_tripID);
    void leaveTrip(const string &_tripID);
    bool hasJoinedTrip(const string &_tripID);
    void removeSpending(const string &_tripID, const EXPENSE &expenseToRemove);

    // Operators overloading
    MEMBER &operator=(const MEMBER &other);
    bool operator==(const MEMBER &other) const;
};

// CLASS: HOST
class HOST : public PERSON {
   private:
    vector<string> hostedTripID;
    string emergencyContact;

   public:
    HOST();
    HOST(const string &_id, const string &_fullName, const GENDER &_gender, const DATE &_dob);

    // FUNC: Getters
    vector<string> getHostedTripIDs() const;
    string getEmergencyContact() const;
    string getRole() const override;
    string getInfo() const override;

    // FUNC: Setters
    void setEmergencyContact(const string &_contact);

    // FUNC: Utility methods
    bool hasHostedTrip(const string &_tripID);
    void hostTrip(const string &_tripID);

    // Operators overloading
    HOST &operator=(const HOST &other);
    bool operator==(const HOST &other) const;
};

class EXPENSE {
   private:
    DATE date;
    CATEGORY category;
    long long amount;
    string note;
    MEMBER personInCharge;

   public:
    // Constructors
    EXPENSE();
    EXPENSE(const EXPENSE &other);
    EXPENSE(const DATE &_date, const CATEGORY _category, long long _amount, const string &_note, const MEMBER &_member);

    // Getters
    long long getAmount() const;
    CATEGORY getCategory() const;
    string getNote() const;
    MEMBER getPIC() const;
    DATE getDate() const;

    // Setters
    void setAmount(long long _amount);
    void setCategory(CATEGORY _category);
    void setNote(const string &_note);
    void setPIC(const MEMBER &_member);
    void setDate(const DATE &_date);

    // Utility functions
    void displaySummary() const;
    string formatCurrency() const;

    // Operator overloading
    EXPENSE &operator=(const EXPENSE &other);
    bool operator==(const EXPENSE &other) const;
};

// CLASS: TRIP
class TRIP {
   private:
    static int tripCount;
    string ID, Destination, Description;
    DATE startDate, endDate;
    STATUS status;
    vector<MEMBER> members;
    HOST host;
    vector<EXPENSE> expenses;
    long long totalExpense;

   public:
    // NOTE: Constructors
    TRIP();
    TRIP(const TRIP &other);
    TRIP(const string &_tripID, const string &_dest, const string &_desc, int _startDay, int _startMonth,
         int _startYear, int _endDay, int _endMonth, int _endYear, const STATUS &_status,
         const vector<EXPENSE> &_expenses, long long _totalExpense);
    TRIP(const string &_tripID, const string &_dest, const string &_desc, const DATE &_startDate, const DATE &_endDate,
         const STATUS &_status, const vector<EXPENSE> &_expenses, long long _totalExpense);

    // ~TRIP() { tripCount--; }

    // NOTE: Getters
    string getID() const;
    string getDestination() const;
    string getDescription() const;
    DATE getStartDate() const;
    DATE getEndDate() const;
    STATUS getStatus() const;
    string getStatusString() const;
    vector<EXPENSE> getAllExpenses() const;
    long long getTotalExpense() const;

    HOST getHost() const;
    vector<MEMBER> getMembers() const;

    // NOTE: Setters
    void setID(const string &_ID);
    void setDestination(const string &_destination);
    void setDescription(const string &_description);
    void setStartDate(const DATE &_startDate);
    void setEndDate(const DATE &_endDate);
    void setStatus(const STATUS &_status);
    void setTotalExpense(long long _amount);
    void setExpenses(const vector<EXPENSE> &_expenses);

    // FUNC: Utility methods
    // People
    void addMember(const MEMBER &member);
    void setMembers(const vector<MEMBER> &members);
    void setHost(const HOST &_host);
    bool hasHost() const;
    // Expense
    void addExpense(const EXPENSE &expense);

    // Operators overloading
    TRIP &operator=(const TRIP &other);
    bool operator==(const TRIP &other) const;
    friend ostream &operator<<(ostream &, const TRIP &);
};

#endif  // HEADER_H
