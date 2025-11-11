#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

// ==================== EVENT CLASS ====================

class Event {
private:
    string eventName;
    string date;          // Format: DD-MM-YYYY
    string venue;
    int capacity;
    int registeredCount;

public:
    // Constructor
    Event(string name = "", string d = "", string v = "", int cap = 0, int reg = 0);

    // Getters
    string getEventName() const;
    string getDate() const;
    string getVenue() const;
    int getCapacity() const;
    int getRegisteredCount() const;
    int getAvailableSeats() const;

    // Setters
    void setEventName(const string& name);
    void setDate(const string& d);
    void setVenue(const string& v);
    void setCapacity(int cap);
    void setRegisteredCount(int count);

    // Check if event has available seats
    bool hasAvailableSeats() const;

    // Increment registered count
    void registerStudent();

    // Decrement registered count
    void unregisterStudent();

    // Format event data for file storage (pipe-delimited)
    string toFileFormat() const;

    // Display event information in table format
    void display(int index = 0) const;

    // Display detailed event information
    void displayDetailed(int index = 0) const;
};

#endif // EVENT_H
