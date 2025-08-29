#ifndef PERSONFACTORY_H
#define PERSONFACTORY_H

#include "../Models/header.h"

using namespace std;

enum class PersonRole { MEMBER, HOST };

class PERSONFACTORY {
   private:
    static int nextMemberNumber;
    static int nextHostNumber;

   public:
    static MEMBER createMember(const string &fullName, const GENDER &gender, const DATE &dob);
    static HOST createHost(const string &fullName, const GENDER &gender, const DATE &dob);

    static string generatePersonID(const string &fullName, const DATE &dob);
};
#endif  // PERSONFACTORY_H