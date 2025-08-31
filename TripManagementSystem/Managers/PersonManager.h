#ifndef PERSONMANAGER_H
#define PERSONMANAGER_H

#include <string>
#include <vector>

#include "../Models/header.h"
#include "FileManager.h"
#include "Observer.h"

using namespace std;

class OBSERVER;

class PERSONMANAGER : public SUBJECT {
   private:
    vector<MEMBER> members;
    vector<HOST> hosts;

    mutable vector<PERSON> people;
    mutable bool peopleNeedsUpdate;

   public:
    PERSONMANAGER();
    ~PERSONMANAGER();

    void addPerson(const PERSON &person);
    void addMember(const MEMBER &member);
    void addHost(const HOST &host);

    bool removePerson(const string &personID);
    bool removeMember(const string &memberID);
    bool removeHost(const string &hostID);

    bool updatePerson(const PERSON &originalPerson, const PERSON &updatedPerson);
    bool updateMember(const MEMBER &originalMember, const MEMBER &updatedMember);
    bool updateHost(const HOST &originalHost, const HOST &updatedHost);
    bool updatePeople(const vector<PERSON> &updatedPeople);

    PERSON *findPersonById(const string &id);
    MEMBER *findMemberById(const string &id);
    HOST *findHostById(const string &id);

    const vector<PERSON> &getAllPeople() const;
    const vector<MEMBER> &getAllMembers() const;
    const vector<HOST> &getAllHosts() const;

    vector<MEMBER> getAllMembersLegacy() const;
    vector<HOST> getAllHostsLegacy() const;

    size_t getPersonCount() const;
    size_t getMemberCount() const;
    size_t getHostCount() const;

    HOST getHostByID(const string &hostID);
    MEMBER getMemberByID(const string &memberID);

    void addMultipleMembers(const vector<MEMBER> &newMembers);
    void addMultipleHosts(const vector<HOST> &newHosts);

    void refreshPeopleVector() const;
    bool validateDataIntegrity() const;
    void debugPrintCounts() const;
};

#endif