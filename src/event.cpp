#include "event.h"
#include <iomanip>

// Constructor
Event::Event(string name, string d, string v, int cap, int reg)
    : eventName(name), date(d), venue(v), capacity(cap), registeredCount(reg) {}

// Getters
string Event::getEventName() const { return eventName; }
string Event::getDate() const { return date; }
string Event::getVenue() const { return venue; }
int Event::getCapacity() const { return capacity; }
int Event::getRegisteredCount() const { return registeredCount; }
int Event::getAvailableSeats() const { return capacity - registeredCount; }

// Setters
void Event::setEventName(const string& name) { eventName = name; }
void Event::setDate(const string& d) { date = d; }
void Event::setVenue(const string& v) { venue = v; }
void Event::setCapacity(int cap) { capacity = cap; }
void Event::setRegisteredCount(int count) { registeredCount = count; }

// Check if event has available seats
bool Event::hasAvailableSeats() const {
    return registeredCount < capacity;
}

// Increment registered count
void Event::registerStudent() {
    if (hasAvailableSeats()) {
        registeredCount++;
    }
}

// Decrement registered count
void Event::unregisterStudent() {
    if (registeredCount > 0) {
        registeredCount--;
    }
}

// Format event data for file storage (pipe-delimited)
string Event::toFileFormat() const {
    return eventName + "|" + date + "|" + venue + "|" + to_string(capacity) + "|" + to_string(registeredCount);
}

// Display event information in table format
void Event::display(int index) const {
    cout << "  ";
    if (index > 0) cout << setw(2) << index << ". ";
    cout << left << setw(25) << eventName << " | " 
         << setw(12) << date << " | " 
         << setw(20) << venue << " | "
         << setw(4) << capacity << " | "
         << setw(4) << registeredCount << " | "
         << getAvailableSeats() << endl;
}

// Display detailed event information
void Event::displayDetailed(int index) const {
    cout << "\n--- Event Details ---" << endl;
    if (index > 0) cout << "Event #" << index << endl;
    cout << "Name: " << eventName << endl;
    cout << "Date: " << date << endl;
    cout << "Venue: " << venue << endl;
    cout << "Total Capacity: " << capacity << endl;
    cout << "Registered Students: " << registeredCount << endl;
    cout << "Available Seats: " << getAvailableSeats() << endl;
    cout << "Occupancy: " << fixed << setprecision(1) 
         << (capacity > 0 ? (registeredCount * 100.0 / capacity) : 0) << "%" << endl;
}
