#ifndef ADMIN_H
#define ADMIN_H

#include "user.h"
#include "event.h"
#include "registration.h"
#include "utils.h"
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <iomanip>

using namespace std;

// ==================== ADMIN CLASS ====================

class Admin : public User {
public:
    // Constructor calling base class constructor
    Admin(string uname, string pass, string name);
    
    // Polymorphism: Admin-specific menu implementation
    void displayMenu() override;
    
    // Virtual method implementation
    string getUserType() override;
    
    // File I/O Operations
    vector<Event> loadEventsFromFile();
    bool saveEventsToFile(const vector<Event>& events);
    vector<Registration> loadRegistrationsFromFile();
    bool saveRegistrationsToFile(const vector<Registration>& registrations);
    
    // Event Management
    void manageEvents();
    void addNewEvent();
    void editEvent();
    void deleteEvent();
    
    // View and Report Features
    void viewAllEvents();
    void displayEventStats();
    void viewRegistrationReports();
    
    // User Management
    void manageUsers();
    void addNewStudent();
    void viewAllUsers();
};

#endif // ADMIN_H
