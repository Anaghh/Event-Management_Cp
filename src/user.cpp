#include "user.h"

// Constructor to initialize user data
User::User(string uname, string pass, string name) {
    username = uname;
    password = pass;
    fullName = name;
}

// Getter methods
string User::getUsername() const { return username; }
string User::getFullName() const { return fullName; }

// Authentication method
bool User::authenticate(string pass) {
    return password == pass;
}

// Virtual destructor for proper cleanup
User::~User() {}
