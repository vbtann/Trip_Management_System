#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>
#include <vector>

using namespace std;

class OBSERVER {
   public:
    virtual ~OBSERVER() {}

    virtual void onTripAdded(const string &tripID) = 0;
    virtual void onTripRemoved(const string &tripID) = 0;
    virtual void onTripUpdated(const string &tripID) = 0;

    virtual void onPersonAdded(const string &personID) = 0;
    virtual void onPersonRemoved(const string &personID) = 0;
    virtual void onPersonUpdated(const string &personID) = 0;
};

class SUBJECT {
   private:
    vector<OBSERVER *> observers;

   public:
    void addObserver(OBSERVER *observer);
    void removeObserver(OBSERVER *observer);

    void notifyTripAdded(const string &tripID);
    void notifyTripRemoved(const string &tripID);
    void notifyTripUpdated(const string &tripID);

    void notifyPersonAdded(const string &personID);
    void notifyPersonRemoved(const string &personID);
    void notifyPersonUpdated(const string &personID);
};

#endif  // OBSERVER_H