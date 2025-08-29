#include "header.h"

using namespace std;

const string genderToString(GENDER gender) {
     if (gender == GENDER::Female) {
          return "Female";
     } else {
          return "Male";
     }
}

GENDER stringToGender(const string &genderString) {
     if (genderString == "Female") {
          return GENDER::Female;
     } else {
          return GENDER::Male;
     }
}