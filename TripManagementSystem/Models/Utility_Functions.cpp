#include "header.h"

using namespace std;

string toUpper(const string &str) {
    string res = str;
    transform(res.begin(), res.end(), res.begin(), [](unsigned char c) { return toupper(c); });

    return res;
}