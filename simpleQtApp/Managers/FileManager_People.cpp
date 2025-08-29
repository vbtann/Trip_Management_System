#include "FileManager.h"
#include "PersonFactory.h"
#include "PersonManager.h"

using namespace std;

QString getPeopleCacheFilePath() {
    QDir currentDir = QDir::current();

    if (currentDir.exists("cache")) {
        return currentDir.absoluteFilePath("cache/people_cache.json");
    } else {
        throw std::runtime_error("Cache folder not found in current directory: " +
                                 currentDir.absolutePath().toStdString());
    }
}

bool peopleCacheFileExists() {
    QFileInfo cacheFile(getPeopleCacheFilePath());
    return cacheFile.exists();
}

vector<pair<string, EXPENSE>> parseSpendingsFromJson(const json &spendingsJson, const PERSONMANAGER *personManager) {
    vector<pair<string, EXPENSE>> spendings;

    if (!spendingsJson.is_array()) {
        return spendings;
    }

    for (const auto &spendingJson : spendingsJson) {
        try {
            string tripId = spendingJson.value("trip_id", "");
            string dateStr = spendingJson.value("date", "");
            string categoryStr = spendingJson.value("category", "");
            long long amount = spendingJson.value("amount", 0);
            string note = spendingJson.value("note", "");
            string personId = spendingJson.value("personInCharge", "");

            if (tripId.empty() || dateStr.empty() || amount <= 0 || personId.empty()) {
                continue;
            }

            DATE expenseDate = extractDate(dateStr);
            CATEGORY category = stringToCategory(categoryStr);

            MEMBER pic(personId, "Temporary Name", GENDER::Male, DATE(1, 1, 2000));

            EXPENSE expense(expenseDate, category, amount, note, pic);
            spendings.push_back(make_pair(tripId, expense));

        } catch (const std::exception &e) {
            continue;
        }
    }

    return spendings;
}

json spendingsToJson(const vector<pair<string, EXPENSE>> &spendings) {
    json spendingsJson = json::array();

    for (const auto &spending : spendings) {
        try {
            json spendingJson = {{"trip_id", spending.first},
                                 {"date", spending.second.getDate().toString()},
                                 {"category", categoryToString(spending.second.getCategory())},
                                 {"amount", spending.second.getAmount()},
                                 {"note", spending.second.getNote()},
                                 {"personInCharge", spending.second.getPIC().getID()}};
            spendingsJson.push_back(spendingJson);
        } catch (const std::exception &e) {
            continue;
        }
    }

    return spendingsJson;
}

void from_json(const json &j, PERSON &person) {
    try {
        string id = j.value("id", "");
        string fullName = j.value("full_name", "");
        string dobStr = j.value("date_of_birth", "");
        string email = j.value("email", "");
        string phone = j.value("phone_number", "");
        string address = j.value("address", "");
        string genderStr = j.value("gender", "");

        if (fullName.empty() || dobStr.empty() || genderStr.empty()) {
            throw std::runtime_error("Missing required person fields in JSON");
        }

        DATE dob = extractDate(dobStr);
        GENDER gender = stringToGender(genderStr);

        person.setID(id);
        person.setFullName(fullName);
        person.setDateOfBirth(dob);
        person.setEmail(email);
        person.setPhoneNumber(phone);
        person.setAddress(address);
        person.setGender(gender);
    } catch (const std::exception &e) {
        throw std::runtime_error("Invalid PERSON JSON format: " + string(e.what()));
    }
}

void importPeopleInfoFromJson(vector<MEMBER> &members, vector<HOST> &hosts, const string &filePath,
                              const PERSONMANAGER *personManager) {
    ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open JSON file for import: " + filePath);
    }

    try {
        json j;
        file >> j;
        file.close();

        if (!j.is_array()) {
            throw std::runtime_error("Invalid JSON structure: expected array of people");
        }

        members.clear();
        hosts.clear();

        for (const auto &personJson : j) {
            try {
                if (!personJson.contains("role")) {
                    continue;
                }

                string role = personJson["role"].get<string>();
                string id = personJson.value("id", "");
                string fullName = personJson.value("full_name", "");
                string dobStr = personJson.value("date_of_birth", "");
                string email = personJson.value("email", "");
                string phone = personJson.value("phone_number", "");
                string genderStr = personJson.value("gender", "");
                string address = personJson.value("address", "");

                if (fullName.empty() || dobStr.empty() || genderStr.empty()) {
                    continue;
                }

                DATE dob = extractDate(dobStr);
                GENDER gender = stringToGender(genderStr);

                if (role == "Member") {
                    MEMBER member(id, fullName, gender, dob);
                    member.setEmail(email);
                    member.setPhoneNumber(phone);
                    member.setAddress(address);

                    string emergencyContact = personJson.value("emergency_contact", "");
                    member.setEmergencyContact(emergencyContact);

                    if (personJson.contains("interests") && personJson["interests"].is_array()) {
                        for (const auto &interest : personJson["interests"]) {
                            if (interest.is_string()) {
                                member.addInterest(interest.get<string>());
                            }
                        }
                    }

                    if (personJson.contains("spendings") && personJson["spendings"].is_array()) {
                        vector<pair<string, EXPENSE>> spendings =
                            parseSpendingsFromJson(personJson["spendings"], nullptr);
                        member.setSpendings(spendings);
                    }

                    if (personJson.contains("total_spent") && personJson["total_spent"].is_number()) {
                        long long totalSpent = personJson["total_spent"].get<long long>();
                        member.setTotalSpent(totalSpent);
                    }

                    members.push_back(member);

                } else if (role == "Host") {
                    HOST host(id, fullName, gender, dob);
                    host.setEmail(email);
                    host.setPhoneNumber(phone);
                    host.setAddress(address);

                    string emergencyContact = personJson.value("emergency_contact", "");
                    host.setEmergencyContact(emergencyContact);

                    hosts.push_back(host);
                }

            } catch (const std::exception &e) {
                continue;
            }
        }

    } catch (const json::parse_error &e) {
        file.close();
        throw std::runtime_error("JSON parse error: " + string(e.what()));
    } catch (const std::exception &e) {
        file.close();
        throw std::runtime_error("Error reading people from JSON: " + string(e.what()));
    }
}

void exportPeopleInfoToJson(const vector<MEMBER> &members, const vector<HOST> &hosts, const string &outputFilePath) {
    ofstream output(outputFilePath);
    if (!output.is_open()) {
        throw runtime_error("Cannot open file for writing: " + outputFilePath);
    }

    json j = json::array();

    // Export hosts first
    for (const HOST &host : hosts) {
        json hostJson = {{"id", host.getID()},
                         {"full_name", host.getFullName()},
                         {"date_of_birth", host.getDateOfBirth().toString()},
                         {"email", host.getEmail()},
                         {"phone_number", host.getPhoneNumber()},
                         {"address", host.getAddress()},
                         {"gender", genderToString(host.getGender())},
                         {"role", "Host"},
                         {"emergency_contact", host.getEmergencyContact()}};
        j.push_back(hostJson);
    }

    for (const MEMBER &member : members) {
        json memberJson = {{"id", member.getID()},
                           {"full_name", member.getFullName()},
                           {"date_of_birth", member.getDateOfBirth().toString()},
                           {"email", member.getEmail()},
                           {"phone_number", member.getPhoneNumber()},
                           {"address", member.getAddress()},
                           {"gender", genderToString(member.getGender())},
                           {"role", "Member"},
                           {"emergency_contact", member.getEmergencyContact()},
                           {"total_spent", member.getTotalSpent()}};

        try {
            vector<string> interests = member.getInterests();
            json interestsJson = json::array();
            for (const string &interest : interests) {
                interestsJson.push_back(interest);
            }
            memberJson["interests"] = interestsJson;
        } catch (...) {
            memberJson["interests"] = json::array();
        }

        try {
            vector<pair<string, EXPENSE>> spendings = member.getSpendings();
            memberJson["spendings"] = spendingsToJson(spendings);
        } catch (...) {
            memberJson["spendings"] = json::array();
        }

        j.push_back(memberJson);
    }

    output << j.dump(4);
    output.close();
}

void loadPeopleCacheFile(vector<MEMBER> &members, vector<HOST> &hosts, const PERSONMANAGER *personManager) {
    QString cacheFilePath = getPeopleCacheFilePath();

    QFileInfo cacheFile(cacheFilePath);
    if (!cacheFile.exists()) {
        return;
    }

    members.clear();
    hosts.clear();
    importPeopleInfoFromJson(members, hosts, cacheFilePath.toStdString(), personManager);
}

void updatePeopleCacheFile(const vector<MEMBER> &members, const vector<HOST> &hosts) {
    QString cacheFilePath = getPeopleCacheFilePath();

    exportPeopleInfoToJson(members, hosts, cacheFilePath.toStdString());
}

void importPeopleFromFile(vector<MEMBER> &members, vector<HOST> &hosts, const string &filePath,
                          const PERSONMANAGER *personManager) {
    try {
        importPeopleInfoFromJson(members, hosts, filePath, personManager);
    } catch (const exception &e) {
        throw runtime_error("Failed to import people from file: " + string(e.what()));
    }
}
void exportPeopleToFile(const vector<MEMBER> &members, const vector<HOST> &hosts, const string &filePath) {
    try {
        exportPeopleInfoToJson(members, hosts, filePath);
    } catch (const exception &e) {
        throw runtime_error("Failed to export people to file: " + string(e.what()));
    }
}

void savePeopleDataToCache(const vector<MEMBER> &members, const vector<HOST> &hosts, const string &filePath) {
    try {
        exportPeopleInfoToJson(members, hosts, filePath);
    } catch (const exception &e) {
        throw runtime_error("Failed to save people data to cache: " + string(e.what()));
    }
}

void loadPeopleDataFromCache(vector<MEMBER> &members, vector<HOST> &hosts, const string &filePath,
                             const PERSONMANAGER *personManager) {
    try {
        importPeopleInfoFromJson(members, hosts, filePath, personManager);
    } catch (const exception &e) {
        throw runtime_error("Failed to load people data from cache: " + string(e.what()));
    }
}

vector<pair<string, EXPENSE>> getSpendingsForMember(const string &memberID, const vector<MEMBER> &members) {
    for (const MEMBER &member : members) {
        if (member.getID() == memberID) {
            return member.getSpendings();
        }
    }
    return vector<pair<string, EXPENSE>>();
}

void updateMemberSpending(const string &memberID, const vector<pair<string, EXPENSE>> &newSpendings,
                          vector<MEMBER> &members) {
    for (MEMBER &member : members) {
        if (member.getID() == memberID) {
            member.setSpendings(newSpendings);

            long long totalSpent = 0;
            for (const auto &spending : newSpendings) {
                totalSpent += spending.second.getAmount();
            }
            member.setTotalSpent(totalSpent);
            break;
        }
    }
}
