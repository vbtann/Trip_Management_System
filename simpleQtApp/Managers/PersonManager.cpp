#include "PersonManager.h"

#include <QDebug>
#include <algorithm>

using namespace std;

PERSONMANAGER::PERSONMANAGER() : peopleNeedsUpdate(true) {
    if (peopleCacheFileExists()) {
        vector<HOST> cachedHosts;
        vector<MEMBER> cachedMembers;

        loadPeopleCacheFile(cachedMembers, cachedHosts);

        if (!cachedHosts.empty() || !cachedMembers.empty()) {
            addMultipleHosts(cachedHosts);
            addMultipleMembers(cachedMembers);
        } else {
            QMessageBox::warning(
                nullptr, "Cache Load Error",
                "Failed to load people cache. Starting with empty person list. Please check the cache file.");
        }
    }
    refreshPeopleVector();
}

PERSONMANAGER::~PERSONMANAGER() { updatePeopleCacheFile(this->members, this->hosts); }

void PERSONMANAGER::addPerson(const PERSON &person) {
    if (person.getRole() == "Member") {
        const MEMBER &member = static_cast<const MEMBER &>(person);
        addMember(member);
    } else if (person.getRole() == "Host") {
        const HOST &host = static_cast<const HOST &>(person);
        addHost(host);
    }
}

void PERSONMANAGER::addMember(const MEMBER &member) {
    members.push_back(member);
    peopleNeedsUpdate = true;

    notifyPersonAdded(member.getID());
    updatePeopleCacheFile(members, hosts);
}

void PERSONMANAGER::addHost(const HOST &host) {
    hosts.push_back(host);
    peopleNeedsUpdate = true;

    notifyPersonAdded(host.getID());
    updatePeopleCacheFile(members, hosts);
    qDebug() << "Added host:" << QString::fromStdString(host.getFullName());
}

bool PERSONMANAGER::removePerson(const string &personID) {
    if (removeMember(personID)) {
        return true;
    }
    return removeHost(personID);
}

bool PERSONMANAGER::removeMember(const string &memberID) {
    auto it = find_if(members.begin(), members.end(),
                      [&memberID](const MEMBER &member) { return member.getID() == memberID; });

    if (it != members.end()) {
        members.erase(it);
        peopleNeedsUpdate = true;

        notifyPersonRemoved(memberID);
        updatePeopleCacheFile(members, hosts);
        qDebug() << "Removed member:" << QString::fromStdString(memberID);
        return true;
    }
    return false;
}

bool PERSONMANAGER::removeHost(const string &hostID) {
    auto it = find_if(hosts.begin(), hosts.end(), [&hostID](const HOST &host) { return host.getID() == hostID; });

    if (it != hosts.end()) {
        hosts.erase(it);
        peopleNeedsUpdate = true;

        notifyPersonRemoved(hostID);
        updatePeopleCacheFile(members, hosts);
        return true;
    }
    return false;
}

bool PERSONMANAGER::updatePerson(const PERSON &originalPerson, const PERSON &updatedPerson) {
    if (originalPerson.getRole() == "Member" && updatedPerson.getRole() == "Member") {
        return updateMember(static_cast<const MEMBER &>(originalPerson), static_cast<const MEMBER &>(updatedPerson));
    } else if (originalPerson.getRole() == "Host" && updatedPerson.getRole() == "Host") {
        return updateHost(static_cast<const HOST &>(originalPerson), static_cast<const HOST &>(updatedPerson));
    } else {
        if (removePerson(originalPerson.getID())) {
            addPerson(updatedPerson);
            return true;
        }
    }
    return false;
}

bool PERSONMANAGER::updateMember(const MEMBER &originalMember, const MEMBER &updatedMember) {
    auto it = find_if(members.begin(), members.end(),
                      [&originalMember](const MEMBER &member) { return member.getID() == originalMember.getID(); });

    if (it != members.end()) {
        *it = updatedMember;
        peopleNeedsUpdate = true;

        notifyPersonUpdated(updatedMember.getID());
        updatePeopleCacheFile(members, hosts);
        qDebug() << "Updated member:" << QString::fromStdString(updatedMember.getID());
        return true;
    }
    return false;
}

bool PERSONMANAGER::updateHost(const HOST &originalHost, const HOST &updatedHost) {
    auto it = find_if(hosts.begin(), hosts.end(),
                      [&originalHost](const HOST &host) { return host.getID() == originalHost.getID(); });

    if (it != hosts.end()) {
        *it = updatedHost;
        peopleNeedsUpdate = true;

        notifyPersonUpdated(updatedHost.getID());
        updatePeopleCacheFile(members, hosts);
        qDebug() << "Updated host:" << QString::fromStdString(updatedHost.getID());
        return true;
    }
    return false;
}

bool PERSONMANAGER::updatePeople(const vector<PERSON> &updatedPeople) {
    members.clear();
    hosts.clear();

    for (const PERSON &person : updatedPeople) {
        if (person.getRole() == "Member") {
            members.push_back(static_cast<const MEMBER &>(person));
        } else if (person.getRole() == "Host") {
            hosts.push_back(static_cast<const HOST &>(person));
        }
    }

    peopleNeedsUpdate = true;
    updatePeopleCacheFile(members, hosts);
    return true;
}

PERSON *PERSONMANAGER::findPersonById(const string &id) {
    MEMBER *member = findMemberById(id);
    if (member) {
        return member;
    }

    HOST *host = findHostById(id);
    if (host) {
        return host;
    }

    return nullptr;
}

MEMBER *PERSONMANAGER::findMemberById(const string &id) {
    auto it = find_if(members.begin(), members.end(), [&id](const MEMBER &member) { return member.getID() == id; });
    return (it != members.end()) ? &(*it) : nullptr;
}

HOST *PERSONMANAGER::findHostById(const string &id) {
    auto it = find_if(hosts.begin(), hosts.end(), [&id](const HOST &host) { return host.getID() == id; });
    return (it != hosts.end()) ? &(*it) : nullptr;
}

const vector<PERSON> &PERSONMANAGER::getAllPeople() const {
    if (peopleNeedsUpdate) {
        refreshPeopleVector();
    }
    return people;
}

const vector<MEMBER> &PERSONMANAGER::getAllMembers() const { return members; }

const vector<HOST> &PERSONMANAGER::getAllHosts() const { return hosts; }

vector<MEMBER> PERSONMANAGER::getAllMembersLegacy() const { return members; }

vector<HOST> PERSONMANAGER::getAllHostsLegacy() const { return hosts; }

void PERSONMANAGER::refreshPeopleVector() const {
    people.clear();
    people.reserve(members.size() + hosts.size());

    for (const MEMBER &member : members) {
        people.push_back(member);
    }

    for (const HOST &host : hosts) {
        people.push_back(host);
    }

    peopleNeedsUpdate = false;
    qDebug() << "Refreshed people vector:" << people.size() << "total people";
}

size_t PERSONMANAGER::getPersonCount() const {
    if (peopleNeedsUpdate) {
        refreshPeopleVector();
    }
    return people.size();
}

size_t PERSONMANAGER::getMemberCount() const { return members.size(); }

size_t PERSONMANAGER::getHostCount() const { return hosts.size(); }

HOST PERSONMANAGER::getHostByID(const string &hostID) {
    HOST *host = findHostById(hostID);
    return host ? *host : HOST();
}

MEMBER PERSONMANAGER::getMemberByID(const string &memberID) {
    MEMBER *member = findMemberById(memberID);
    return member ? *member : MEMBER();
}

void PERSONMANAGER::debugPrintCounts() const {
    qDebug() << "PersonManager Debug Counts:";
    qDebug() << "  Members:" << members.size();
    qDebug() << "  Hosts:" << hosts.size();
    qDebug() << "  Total People:" << getPersonCount();
    qDebug() << "  People needs update:" << peopleNeedsUpdate;
}

bool PERSONMANAGER::validateDataIntegrity() const {
    bool valid = true;

    for (size_t i = 0; i < members.size(); ++i) {
        for (size_t j = i + 1; j < members.size(); ++j) {
            if (members[i].getID() == members[j].getID()) {
                qDebug() << "Duplicate member ID found:" << QString::fromStdString(members[i].getID());
                valid = false;
            }
        }
    }

    for (size_t i = 0; i < hosts.size(); ++i) {
        for (size_t j = i + 1; j < hosts.size(); ++j) {
            if (hosts[i].getID() == hosts[j].getID()) {
                qDebug() << "Duplicate host ID found:" << QString::fromStdString(hosts[i].getID());
                valid = false;
            }
        }
    }

    for (const MEMBER &member : members) {
        for (const HOST &host : hosts) {
            if (member.getID() == host.getID()) {
                qDebug() << "ID exists in both vectors:" << QString::fromStdString(member.getID());
                valid = false;
            }
        }
    }

    return valid;
}

void PERSONMANAGER::addMultipleMembers(const vector<MEMBER> &newMembers) {
    for (const MEMBER &mem : newMembers) {
        addMember(mem);
    }
}

void PERSONMANAGER::addMultipleHosts(const vector<HOST> &newHosts) {
    for (const HOST &host : newHosts) {
        addHost(host);
    }
}