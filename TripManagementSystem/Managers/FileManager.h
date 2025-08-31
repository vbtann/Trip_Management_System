#pragma once

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../Managers/PersonFactory.h"
#include "../Managers/TripFactory.h"
#include "../Models/header.h"

class PERSONMANAGER;

using namespace std;
using json = nlohmann::ordered_json;

// ==================== TRIP FUNCTIONS (JSON ONLY) ====================

void importTripInfoFromJson(vector<TRIP> &trips, const string &filePath, const PERSONMANAGER *personManager);
void exportTripsInfoToJson(const vector<TRIP> &trips, const string &outputFilePath);

void loadTripCacheFile(vector<TRIP> &trips, const PERSONMANAGER *personManager);
void updateCacheFile(const vector<TRIP> &trips);
bool cacheFileExists();
QString getCacheFilePath();

void saveTripDataToCache(const vector<TRIP> &trips, const string &filePath);
void loadTripDataFromCache(vector<TRIP> &trips, const string &filePath, const PERSONMANAGER *personManager);
void importTripsFromFile(vector<TRIP> &trips, const string &filePath, const PERSONMANAGER *personManager);
void exportTripsToFile(const vector<TRIP> &trips, const string &filePath);

void from_json(const json &j, TRIP &trip, const PERSONMANAGER *personManager);
void to_json(json &j, const TRIP &trip);

void importTripFromCache(vector<TRIP> &trips, const string &filePath, const PERSONMANAGER *personManager);
void saveTripAttendeesToCache(const vector<TRIP> &trips, const string &filePath);

// ==================== PEOPLE FUNCTIONS ====================
void importPeopleInfoFromJson(vector<MEMBER> &members, vector<HOST> &hosts, const string &filePath,
                              const PERSONMANAGER *personManager = nullptr);
void exportPeopleInfoToJson(const vector<MEMBER> &members, const vector<HOST> &hosts, const string &outputFilePath);

void loadPeopleCacheFile(vector<MEMBER> &members, vector<HOST> &hosts, const PERSONMANAGER *personManager = nullptr);
void updatePeopleCacheFile(const vector<MEMBER> &members, const vector<HOST> &hosts);
bool peopleCacheFileExists();
QString getPeopleCacheFilePath();

void savePeopleDataToCache(const vector<MEMBER> &members, const vector<HOST> &hosts, const string &filePath);
void loadPeopleDataFromCache(vector<MEMBER> &members, vector<HOST> &hosts, const string &filePath,
                             const PERSONMANAGER *personManager = nullptr);
void importPeopleFromFile(vector<MEMBER> &members, vector<HOST> &hosts, const string &filePath,
                          const PERSONMANAGER *personManager = nullptr);
void exportPeopleToFile(const vector<MEMBER> &members, const vector<HOST> &hosts, const string &filePath);

void from_json(const json &j, PERSON &person);
vector<pair<string, EXPENSE>> parseSpendingsFromJson(const json &spendingsJson, const PERSONMANAGER *personManager);
json spendingsToJson(const vector<pair<string, EXPENSE>> &spendings);

vector<pair<string, EXPENSE>> getSpendingsForMember(const string &memberID, const vector<MEMBER> &members);
void updateMemberSpending(const string &memberID, const vector<pair<string, EXPENSE>> &newSpendings,
                          vector<MEMBER> &members);

// ==================== HELPER FUNCTIONS ====================
DATE extractDate(string _date);

string toUpper(const string &str);