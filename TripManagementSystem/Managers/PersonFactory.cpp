#include "PersonFactory.h"

#include <iomanip>
#include <sstream>

using namespace std;

int PERSONFACTORY::nextHostNumber = 1;
int PERSONFACTORY::nextMemberNumber = 1;

string PERSONFACTORY::generatePersonID(const string &fullName, const DATE &dob) {
    string id = "";
    bool isNewWord = true;

    for (char c : fullName) {
        if (isspace(c)) {
            isNewWord = true;
        } else if (isNewWord) {
            id += toupper(c);
            isNewWord = false;
        }
    }

    ostringstream dateStream;
    dateStream << setfill('0') << setw(2) << dob.getMonth() << setfill('0') << setw(2) << dob.getDay();

    id += "_" + dateStream.str();

    return id;
}

MEMBER PERSONFACTORY::createMember(const string &fullName, const GENDER &gender, const DATE &dob) {
    string memberID = generatePersonID(fullName, dob);
    return MEMBER(memberID, fullName, gender, dob);
}

HOST PERSONFACTORY::createHost(const string &fullName, const GENDER &gender, const DATE &dob) {
    string hostID = generatePersonID(fullName, dob);
    return HOST(hostID, fullName, gender, dob);
}