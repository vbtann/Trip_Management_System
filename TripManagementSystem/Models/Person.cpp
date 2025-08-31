#include "header.h"

using namespace std;

// FUNC: Constructors
PERSON::PERSON()
    : fullName(""), ID(""), email(""), phoneNumber(""), address(""), gender(GENDER::Male), dateOfBirth(DATE()) {}

PERSON::PERSON(const string &_id, const string &_fullName, const GENDER &_gender, const DATE &_dob)
    : fullName(_fullName), ID(_id), gender(_gender), dateOfBirth(_dob) {}

// FUNC: Convert PERSON's info to string (Format: ID,Email,Full Name,Date of
// Birth,Phone Number,Address,Gender,Nationality)
string PERSON::toString() const {
    string info = "";
    info = info + this->ID + ',' + this->email + ',' + this->fullName + ',' + this->dateOfBirth.toString() + ',' +
           this->phoneNumber + ',' + this->address + ',' + genderToString(this->gender);
    return info;
}

string PERSON::getRole() const { return "Person"; }
string PERSON::getInfo() const { return this->toString(); }

// FUNC: Getters
string PERSON::getFullName() const { return this->fullName; }
string PERSON::getID() const { return this->ID; }
string PERSON::getEmail() const { return this->email; }
string PERSON::getPhoneNumber() const { return this->phoneNumber; }
string PERSON::getAddress() const { return this->address; }
GENDER PERSON::getGender() const { return this->gender; }
DATE PERSON::getDateOfBirth() const { return this->dateOfBirth; }

// FUNC: Setters
void PERSON::setFullName(const string &_fullName) { this->fullName = _fullName; }
void PERSON::setGender(const GENDER &_gender) { this->gender = _gender; }
void PERSON::setDOB(const DATE &_dob) { this->dateOfBirth = _dob; }
void PERSON::setID(const string &_ID) { this->ID = _ID; }
void PERSON::setEmail(const string &_email) { this->email = _email; }
void PERSON::setPhoneNumber(const string &_phoneNumber) { this->phoneNumber = _phoneNumber; }
void PERSON::setAddress(const string &_address) { this->address = _address; }
void PERSON::setDateOfBirth(const DATE &_dob) { this->dateOfBirth = _dob; }

// FUNC: Utility method
int PERSON::getAge() const { return 2025 - this->dateOfBirth.getYear(); }
string PERSON::getContactInfo() const {
    string info = "";
    info += this->email + "," + this->phoneNumber + "," + this->address;
    return info;
}
