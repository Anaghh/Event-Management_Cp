#include "event.h"
#include <iomanip>    // For formatting output (setw, setprecision, etc.)

// ========================================
// EVENT.CPP - Event Class Implementation
// ========================================
// This file implements the Event class which represents a college event.
// Each Event has a name, date, venue, capacity, and tracks how many students registered.

// Constructor with member initializer list
// What it does: Creates an Event object with the provided details
// Parameters: name (event name), d (date), v (venue), cap (max capacity), reg (already registered count)
// Default values in header allow creating empty Event objects too
Event::Event(string name, string d, string v, int cap, int reg)
    : eventName(name),           // Initialize event name
      date(d),                   // Initialize date
      venue(v),                  // Initialize venue
      capacity(cap),             // Initialize maximum capacity
      registeredCount(reg) {}    // Initialize current registration count

// Getter methods - Provide read-only access to private member variables
// 'const' at the end means these functions don't modify the object (they're read-only)
string Event::getEventName() const { 
    return eventName;            // Return the name of the event
}

string Event::getDate() const { 
    return date;                 // Return the date (DD-MM-YYYY format)
}

string Event::getVenue() const { 
    return venue;                // Return where the event is held
}

int Event::getCapacity() const { 
    return capacity;             // Return maximum number of students allowed
}

int Event::getRegisteredCount() const { 
    return registeredCount;      // Return how many students have registered
}

int Event::getAvailableSeats() const { 
    return capacity - registeredCount;  // Calculate remaining seats
}

// Setter methods - Allow controlled modification of private member variables
// 'const string&' means we pass by reference (efficient) and won't modify the parameter
void Event::setEventName(const string& name) { 
    eventName = name;            // Update event name (used when editing)
}

void Event::setDate(const string& d) { 
    date = d;                    // Update event date
}

void Event::setVenue(const string& v) { 
    venue = v;                   // Update venue
}

void Event::setCapacity(int cap) { 
    capacity = cap;              // Update maximum capacity
}

void Event::setRegisteredCount(int count) { 
    registeredCount = count;     // Update registration count
}

// Check if event has available seats
// What it does: Returns true if there's room for more students, false if full
// Used before: Allowing a student to register
bool Event::hasAvailableSeats() const {
    return registeredCount < capacity;   // True if registrations haven't reached capacity
}

// Increment registered count when a student registers
// What it does: Increases the registration count by 1 (if space available)
// Safety check: Only increments if seats are available
void Event::registerStudent() {
    if (hasAvailableSeats()) {           // Check if there's space
        registeredCount++;               // Increase count by 1
    }
}

// Decrement registered count when a student unregisters
// What it does: Decreases the registration count by 1 (if any registrations exist)
// Safety check: Only decrements if count is greater than 0
void Event::unregisterStudent() {
    if (registeredCount > 0) {           // Check if there are registrations to remove
        registeredCount--;               // Decrease count by 1
    }
}

// Format event data for file storage (pipe-delimited)
// What it does: Converts the Event object into a string for saving to events.txt
// Format: eventname|date|venue|capacity|registeredcount
// Example: "Tech Fest 2025|15-03-2025|Main Auditorium|100|45"
// Why pipe (|): We use | as separator because event names/venues might contain commas
string Event::toFileFormat() const {
    return eventName + "|" + date + "|" + venue + "|" + 
           to_string(capacity) + "|" + to_string(registeredCount);
}

// Display event information in table format
// What it does: Prints event in a formatted table row
// Parameters: index (optional row number for display)
// Used in: Listing all events in a table view
void Event::display(int index) const {
    cout << "  ";
    
    // If index is provided (> 0), show the row number
    if (index > 0) cout << setw(2) << index << ". ";
    
    cout << left << setw(25) << eventName << " | "      // Event name (25 chars wide)
         << setw(12) << date << " | "                   // Date (12 chars wide)
         << setw(20) << venue << " | "                  // Venue (20 chars wide)
         << setw(4) << capacity << " | "                // Capacity (4 chars wide)
         << setw(4) << registeredCount << " | "         // Registered (4 chars wide)
         << setw(4) << getAvailableSeats() << endl;     // Available seats (4 chars wide)
}

// Display detailed event information
// What it does: Shows complete event details in a vertical format
// Parameters: index (optional event number for display)
// Used when: Viewing full details of a specific event
void Event::displayDetailed(int index) const {
    cout << "\n--- Event Details ---" << endl;
    if (index > 0) cout << "Event #" << index << endl;
    
    cout << "Name: " << eventName << endl;
    cout << "Date: " << date << endl;
    cout << "Venue: " << venue << endl;
    cout << "Total Capacity: " << capacity << endl;
    cout << "Registered Students: " << registeredCount << endl;
    cout << "Available Seats: " << getAvailableSeats() << endl;
    
    // Calculate and display occupancy percentage
    // fixed: use fixed-point notation
    // setprecision(1): show 1 decimal place
    // Ternary operator: (condition ? if_true : if_false)
    cout << "Occupancy: " << fixed << setprecision(1) 
         << (capacity > 0 ? (registeredCount * 100.0 / capacity) : 0) << "%" << endl;
}
