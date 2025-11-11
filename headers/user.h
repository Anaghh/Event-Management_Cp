#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>

using namespace std;

// ==================== BASE USER CLASS ====================

class User {
protected:
    string username;
    string password;
    string fullName;
    
public:
    // Constructor to initialize user data
    User(string uname, string pass, string name);
    
    // Virtual functions for polymorphism
    virtual void displayMenu() = 0;
    virtual string getUserType() = 0;
    
    // Getter methods
    string getUsername() const;
    string getFullName() const;
    
    // Authentication method
    bool authenticate(string pass);
    
    // Virtual destructor for proper cleanup
    virtual ~User();
};

#endif // USER_H
