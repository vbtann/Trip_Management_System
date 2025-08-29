#include "header.h"

using namespace std;

const string categoryToString(CATEGORY _category) {
    switch (_category) {
        case CATEGORY::Accommodation:
            return "Accommodation";
        case CATEGORY::Food:
            return "Food";
        case CATEGORY::Drink:
            return "Drink";
        case CATEGORY::Gas:
            return "Gas";
        case CATEGORY::Others:
            return "Others";
        default:
            return "Others";
    }
}

CATEGORY stringToCategory(const string &_categoryStr) {
    if (_categoryStr == "Accommodation") {
        return CATEGORY::Accommodation;
    } else if (_categoryStr == "Food") {
        return CATEGORY::Food;
    } else if (_categoryStr == "Drink") {
        return CATEGORY::Drink;
    } else if (_categoryStr == "Gas") {
        return CATEGORY::Gas;
    } else if (_categoryStr == "Others") {
        return CATEGORY::Others;
    }
    return CATEGORY::Others;
}
