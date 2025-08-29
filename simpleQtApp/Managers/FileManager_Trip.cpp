#include "../Models/header.h"
#include "FileManager.h"
#include "PersonManager.h"

using namespace std;

QString getCacheFilePath() {
    QDir currentDir = QDir::current();
    if (currentDir.exists("cache")) {
        return currentDir.absoluteFilePath("cache/cache.json");
    } else {
        throw std::runtime_error("Cache folder not found in current directory: " +
                                 currentDir.absolutePath().toStdString());
    }
}

bool cacheFileExists() {
    QFileInfo cacheFile(getCacheFilePath());
    return cacheFile.exists();
}

DATE extractDate(string _date) {
    int _day, _month, _year;

    stringstream ss1(_date);
    string token;
    getline(ss1, token, '/');
    _day = stoi(token);
    getline(ss1, token, '/');
    _month = stoi(token);
    getline(ss1, token);
    _year = stoi(token);

    DATE date(_day, _month, _year);
    return date;
}

void from_json(const json &j, TRIP &trip, const PERSONMANAGER *personManager) {
    try {
        string idStr = j.value("id", "");
        string destinationStr = j.value("destination", "");
        string descriptionStr = j.value("description", "");
        string startDateStr = j.value("start_date", "");
        string endDateStr = j.value("end_date", "");
        string statusStr = j.value("status", "Planned");

        if (idStr.empty() || destinationStr.empty() || startDateStr.empty() || endDateStr.empty()) {
            throw std::runtime_error("Missing required trip fields in JSON");
        }

        DATE startDate = extractDate(startDateStr);
        DATE endDate = extractDate(endDateStr);
        STATUS status = stringToStatus(statusStr);

        vector<EXPENSE> expenses;
        long long totalExpense = 0;
        trip = TRIP(idStr, toUpper(destinationStr), descriptionStr, startDate, endDate, status, expenses, totalExpense);

        if (personManager) {
            vector<HOST> hosts = personManager->getAllHosts();
            vector<MEMBER> members = personManager->getAllMembers();

            string hostID = j.value("host_id", "");
            if (!hostID.empty()) {
                auto it = find_if(hosts.begin(), hosts.end(), [&](const HOST &host) { return host.getID() == hostID; });
                if (it != hosts.end()) {
                    trip.setHost(*it);
                }
            }

            if (j.contains("member_ids") && j["member_ids"].is_array()) {
                for (const auto &memberIdJson : j["member_ids"]) {
                    string memberID = memberIdJson.get<string>();
                    if (!memberID.empty()) {
                        auto it = find_if(members.begin(), members.end(),
                                          [&](const MEMBER &member) { return member.getID() == memberID; });
                        if (it != members.end()) {
                            trip.addMember(*it);
                        }
                    }
                }
            }

            if (j.contains("expenses") && j["expenses"].is_array()) {
                for (const auto &expenseJson : j["expenses"]) {
                    try {
                        string dateStr = expenseJson.value("date", "");
                        string categoryStr = expenseJson.value("category", "");
                        long long amount = expenseJson.value("amount", 0);
                        string note = expenseJson.value("note", "");
                        string picID = expenseJson.value("personInCharge", "");

                        if (dateStr.empty() || amount <= 0 || picID.empty()) {
                            continue;
                        }

                        DATE expenseDate = extractDate(dateStr);
                        CATEGORY category = stringToCategory(categoryStr);

                        MEMBER pic;
                        auto memberIt = find_if(members.begin(), members.end(),
                                                [&](const MEMBER &member) { return member.getID() == picID; });
                        if (memberIt != members.end()) {
                            pic = *memberIt;
                        } else {
                            continue;
                        }

                        EXPENSE expense(expenseDate, category, amount, note, pic);
                        trip.addExpense(expense);

                    } catch (const std::exception &e) {
                        continue;
                    }
                }
            }
        }

    } catch (const std::exception &e) {
        throw std::runtime_error("Error parsing trip from JSON: " + string(e.what()));
    }
}

void to_json(json &j, const TRIP &trip) {
    j = json{{"id", trip.getID()},
             {"destination", trip.getDestination()},
             {"description", trip.getDescription()},
             {"start_date", trip.getStartDate().toString()},
             {"end_date", trip.getEndDate().toString()},
             {"status", statusToString(trip.getStatus())},
             {"host_id", trip.getHost().getID()},
             {"member_ids", json::array()},
             {"expenses", json::array()}};

    for (const MEMBER &member : trip.getMembers()) {
        j["member_ids"].push_back(member.getID());
    }

    for (const EXPENSE &expense : trip.getAllExpenses()) {
        json expenseJson = {{"date", expense.getDate().toString()},
                            {"category", categoryToString(expense.getCategory())},
                            {"amount", expense.getAmount()},
                            {"note", expense.getNote()},
                            {"personInCharge", expense.getPIC().getID()}};
        j["expenses"].push_back(expenseJson);
    }
}

void importTripInfoFromJson(vector<TRIP> &trips, const string &filePath, const PERSONMANAGER *personManager) {
    ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open JSON file for import: " + filePath);
    }

    try {
        json j;
        file >> j;
        file.close();

        if (!j.is_array()) {
            throw std::runtime_error("Invalid JSON structure: expected array of trips");
        }

        trips.clear();

        int successCount = 0;
        int errorCount = 0;

        for (const auto &tripJson : j) {
            try {
                TRIP trip;
                from_json(tripJson, trip, personManager);
                trips.push_back(trip);
                successCount++;

            } catch (const std::exception &e) {
                errorCount++;
            }
        }

        if (successCount == 0 && errorCount > 0) {
            throw std::runtime_error("Failed to import any trips from JSON file");
        }

    } catch (const json::parse_error &e) {
        file.close();
        throw std::runtime_error("JSON parse error: " + string(e.what()));

    } catch (const json::type_error &e) {
        file.close();
        throw std::runtime_error("JSON type error: " + string(e.what()));

    } catch (const std::exception &e) {
        file.close();
        throw;
    }
}

void exportTripsInfoToJson(const vector<TRIP> &trips, const string &outputFilePath) {
    ofstream output(outputFilePath);
    if (!output.is_open()) {
        throw runtime_error("Cannot open file for writing: " + outputFilePath);
    }

    json j = json::array();
    for (const TRIP &trip : trips) {
        json tripJson;
        to_json(tripJson, trip);
        j.push_back(tripJson);
    }

    output << j.dump(4);
    output.close();
}

void loadTripCacheFile(vector<TRIP> &trips, const PERSONMANAGER *personManager) {
    QString cacheFilePath = getCacheFilePath();

    QFileInfo cacheFile(cacheFilePath);
    if (!cacheFile.exists()) {
        return;
    }

    try {
        trips.clear();
        importTripInfoFromJson(trips, cacheFilePath.toStdString(), personManager);
    } catch (const exception &e) {
    }
}

void updateCacheFile(const vector<TRIP> &trips) {
    QString cacheFilePath = getCacheFilePath();

    try {
        exportTripsInfoToJson(trips, cacheFilePath.toStdString());
    } catch (const exception &e) {
    }
}

void saveTripDataToCache(const vector<TRIP> &trips, const string &filePath) {
    try {
        exportTripsInfoToJson(trips, filePath);
    } catch (const exception &e) {
        throw runtime_error("Failed to save trip data to cache: " + string(e.what()));
    }
}

void loadTripDataFromCache(vector<TRIP> &trips, const string &filePath, const PERSONMANAGER *personManager) {
    try {
        importTripInfoFromJson(trips, filePath, personManager);
    } catch (const exception &e) {
        throw runtime_error("Failed to load trip data from cache: " + string(e.what()));
    }
}

void importTripsFromFile(vector<TRIP> &trips, const string &filePath, const PERSONMANAGER *personManager) {
    try {
        importTripInfoFromJson(trips, filePath, personManager);
    } catch (const exception &e) {
        throw runtime_error("Failed to import trips from file: " + string(e.what()));
    }
}

void exportTripsToFile(const vector<TRIP> &trips, const string &filePath) {
    try {
        exportTripsInfoToJson(trips, filePath);
    } catch (const exception &e) {
        throw runtime_error("Failed to export trips to file: " + string(e.what()));
    }
}

void saveTripAttendeesToCache(const vector<TRIP> &trips, const string &filePath) {
    saveTripDataToCache(trips, filePath);
}

void importTripFromCache(vector<TRIP> &trips, const string &filePath, const PERSONMANAGER *personManager) {
    loadTripDataFromCache(trips, filePath, personManager);
}
