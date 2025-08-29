#include "header.h"

// Constructors
DATE::DATE() : day(1), month(1), year(1) {};

DATE::DATE(int _day, int _month, int _year) : day(_day), month(_month), year(_year) {}

DATE::DATE(const DATE &date) : day(date.day), month(date.month), year(date.year) {}

void DATE::setDate(int _day, int _month, int _year) {
    this->day = _day;
    this->month = _month;
    this->year = _year;
}

// Getters
int DATE::getDay() const { return this->day; }
int DATE::getMonth() const { return this->month; }
int DATE::getYear() const { return this->year; }

std::string DATE::toString() const {
    string day = (this->day < 10) ? "0" + to_string(this->day) : to_string(this->day);
    string month = (this->month < 10) ? "0" + to_string(this->month) : to_string(this->month);
    string year = (this->year < 10) ? "0" + to_string(this->year) : to_string(this->year);
    return day + '/' + month + '/' + year;
}

// Utility methods
bool DATE::operator<(const DATE &rhs) const {
    if (this->year != rhs.year) {
        return this->year < rhs.year;
    }
    if (this->month != rhs.month) {
        return this->month < rhs.month;
    }
    return this->day < rhs.day;
}

DATE &DATE::operator=(const DATE &other) {
    this->day = other.day;
    this->month = other.month;
    this->year = other.year;
    return *this;
}

bool DATE::operator==(const DATE &other) const {
    return (this->day == other.day && this->month == other.month && this->year == other.year);
}

ostream &operator<<(ostream &os, const DATE &date) {
    os << date.day << '/' << date.month << '/' << date.year;
    return os;
}
