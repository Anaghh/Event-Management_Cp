#ifndef STUDENT_H
#define STUDENT_H

#include "user.h"
#include "event.h"
#include "registration.h"
#include "utils.h"
#include <vector>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <iostream>

using namespace std;

// ==================== STUDENT CLASS ====================

class Student : public User {
public:
    // Constructor with base class initialization
    Student(string uname, string pass, string name);
    
    // Polymorphism: Student-specific menu implementation
    void displayMenu() override;
    
    // Virtual method implementation
    string getUserType() override;
    
    // File I/O Operations
    vector<Event> loadEventsFromFile();
    bool saveEventsToFile(const vector<Event>& events);
    vector<Registration> loadRegistrationsFromFile();
    bool saveRegistrationsToFile(const vector<Registration>& registrations);
    
    // Utility for timestamps
    string getCurrentDateTime();
    
    // Event Browsing
    void viewAvailableEvents();
    void viewMyRegistrations();
    
    // Registration Management
    void registerForEvent();
    void unregisterFromEvent(const string& eventName);
    
    // Search and Filter
    void searchEventByName();
    void filterEventsByDate();
};

#endif // STUDENT_H
