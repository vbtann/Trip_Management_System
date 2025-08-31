#include "header.h"

using namespace std;

const string statusToString(STATUS status) {
     switch (status) {
          case STATUS::Planned:
               return "Planned";
          case STATUS::Ongoing:
               return "Ongoing";
          case STATUS::Completed:
               return "Completed";
          case STATUS::Cancelled:
               return "Cancelled";
          default:
               return "Unknown";
     }
}

STATUS stringToStatus(const string &statusStr) {
     if (statusStr == "Planned") return STATUS::Planned;
     if (statusStr == "Ongoing") return STATUS::Ongoing;
     if (statusStr == "Completed") return STATUS::Completed;
     if (statusStr == "Cancelled") return STATUS::Cancelled;

     return STATUS::Planned;
}