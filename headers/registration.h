#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <string>
#include <iostream>

using namespace std;

// ==================== REGISTRATION CLASS ====================

class Registration {
private:
    string studentUsername;
    string eventName;
    string registrationDate;  // Format: DD-MM-YYYY HH:MM

public:
    // Constructor
    Registration(string username = "", string event = "", string regDate = "");

    // Getters
    string getStudentUsername() const;
    string getEventName() const;
    string getRegistrationDate() const;

    // Format registration data for file storage
    string toFileFormat() const;

    // Display registration info
    void display() const;
};

#endif // REGISTRATION_H
