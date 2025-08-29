#include "header.h"

using namespace std;

// Constructors
EXPENSE::EXPENSE() : date(DATE()), amount(0), category(), note(""), personInCharge() {}

EXPENSE::EXPENSE(const DATE &_date, const CATEGORY _category, long long _amount, const string &_note,
                 const MEMBER &_member)
    : date(_date), amount(_amount), category(_category), note(_note), personInCharge(_member) {}

EXPENSE::EXPENSE(const EXPENSE &other)
    : date(other.date),
      amount(other.amount),
      category(other.category),
      note(other.note),
      personInCharge(other.personInCharge) {}

// Getters
long long EXPENSE::getAmount() const { return this->amount; }

CATEGORY EXPENSE::getCategory() const { return this->category; }

string EXPENSE::getNote() const { return this->note; }

MEMBER EXPENSE::getPIC() const { return this->personInCharge; }

DATE EXPENSE::getDate() const { return this->date; }

// Setters
void EXPENSE::setAmount(long long _amount) { this->amount = _amount; }

void EXPENSE::setCategory(CATEGORY _category) { this->category = _category; }

void EXPENSE::setNote(const string &_note) { this->note = _note; }

void EXPENSE::setPIC(const MEMBER &_member) { this->personInCharge = _member; }

void EXPENSE::setDate(const DATE &_date) { this->date = _date; }

// Utility functions
void EXPENSE::displaySummary() const {
    cout << this->date.toString() << " - " << categoryToString(this->category) << " _ " << this->amount << " - "
         << this->personInCharge.getFullName() << "\n";
}

string EXPENSE::formatCurrency() const {
    string amountStr = to_string(this->amount);
    string formatted;
    int count = 0;

    for (int i = amountStr.length() - 1; i >= 0; --i) {
        if (count > 0 && count % 3 == 0) {
            formatted.insert(formatted.begin(), '.');
        }
        formatted.insert(formatted.begin(), amountStr[i]);
        count++;
    }

    return formatted + " VND";
}

// Operator overloading
EXPENSE &EXPENSE::operator=(const EXPENSE &other) {
    this->date = other.date;
    this->amount = other.amount;
    this->category = other.category;
    this->note = other.note;
    this->personInCharge = other.personInCharge;

    return *this;
}

bool EXPENSE::operator==(const EXPENSE &other) const {
    return (this->date == other.date && this->amount == other.amount && this->category == other.category &&
            this->note == other.note && this->personInCharge.getID() == other.personInCharge.getID());
}
