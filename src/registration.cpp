#include "registration.h"

// Constructor
Registration::Registration(string username, string event, string regDate)
    : studentUsername(username), eventName(event), registrationDate(regDate) {}

// Getters
string Registration::getStudentUsername() const { return studentUsername; }
string Registration::getEventName() const { return eventName; }
string Registration::getRegistrationDate() const { return registrationDate; }

// Format registration data for file storage
string Registration::toFileFormat() const {
    return studentUsername + "|" + eventName + "|" + registrationDate;
}

// Display registration info
void Registration::display() const {
    cout << "  " << studentUsername << " -> " << eventName << " (Registered: " << registrationDate << ")" << endl;
}
