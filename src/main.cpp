#include "user.h"
#include "admin.h"
#include "student.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// ==================== AUTHENTICATION ====================

// Function to load users from file and authenticate
User* authenticateUser(string username, string password) {
    ifstream file("data/users.txt");
    string line, uname, pass, name, type;
    
    if (!file.is_open()) {
        cout << "Error: Could not open users.txt file!" << endl;
        cout << "Please ensure users.txt exists with proper user data." << endl;
        return nullptr;
    }
    
    cout << "Checking credentials..." << endl;
    
    // File format: username,password,fullname,usertype
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        vector<string> parts = split(line, ',');
        
        // Ensure we have all required fields
        if (parts.size() >= 4) {
            uname = trim(parts[0]);
            pass = trim(parts[1]);
            name = trim(parts[2]);
            type = trim(parts[3]);
            
            // Check if credentials match
            if (uname == username && pass == password) {
                file.close();
                
                // Polymorphism: Return appropriate user type
                if (type == "admin") {
                    cout << "Admin access granted!" << endl;
                    return new Admin(uname, pass, name);
                } else if (type == "student") {
                    cout << "Student access granted!" << endl;
                    return new Student(uname, pass, name);
                }
            }
        }
    }
    
    file.close();
    return nullptr;
}

// ==================== WELCOME MESSAGE ====================

void displayWelcome() {
    cout << "\n=================================================" << endl;
    cout << "   COLLEGE EVENT MANAGEMENT SYSTEM - PHASE 2    " << endl;
    cout << "=================================================" << endl;
    cout << "Features available in this phase:" << endl;
    cout << "• User Authentication (Admin/Student)" << endl;
    cout << "• Role-based Menu Systems" << endl;
    cout << "• CRUD Operations on Events" << endl;
    cout << "• Event Registration & Management" << endl;
    cout << "• Search & Filter Events" << endl;
    cout << "• Registration Reports" << endl;
    cout << "• User Management (Admin)" << endl;
    cout << "=================================================" << endl;
}

// ==================== MAIN APPLICATION ====================

int main() {
    displayWelcome();
    
    string username, password;
    User* currentUser = nullptr;
    int loginAttempts = 0;
    const int maxAttempts = 3;
    
    // Login loop with attempt limit
    while (currentUser == nullptr && loginAttempts < maxAttempts) {
        cout << "\n=== LOGIN SYSTEM ===" << endl;
        cout << "Default credentials for testing:" << endl;
        cout << "Admin: admin / admin123" << endl;
        cout << "Student: john / pass123" << endl;
        cout << "        alice / alice456" << endl;
        cout << "        bob / bob789" << endl;
        cout << "========================" << endl;
        
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;
        
        currentUser = authenticateUser(username, password);
        loginAttempts++;
        
        if (currentUser == nullptr) {
            cout << "Invalid credentials! " << endl;
            if (loginAttempts < maxAttempts) {
                cout << "Attempts remaining: " << (maxAttempts - loginAttempts) << endl;
                cout << "Please try again." << endl;
            }
        }
    }
    
    // Check if login attempts exceeded
    if (currentUser == nullptr) {
        cout << "Maximum login attempts exceeded. Access denied!" << endl;
        return 1;
    }
    
    // Main application loop after successful authentication
    cout << "\n*** LOGIN SUCCESSFUL ***" << endl;
    cout << "User Type: " << currentUser->getUserType() << endl;
    cout << "Welcome, " << currentUser->getFullName() << "!" << endl;
    
    int choice;
    bool running = true;
    
    // Main menu loop demonstrating polymorphism
    while (running) {
        // Polymorphism in action: Different menus based on user type
        currentUser->displayMenu();
        cin >> choice;
        cin.ignore();
        
        // Handle menu choices using dynamic casting
        if (Admin* admin = dynamic_cast<Admin*>(currentUser)) {
            // Admin-specific menu handling
            switch (choice) {
                case 1:
                    admin->manageEvents();
                    break;
                case 2:
                    admin->viewAllEvents();
                    break;
                case 3:
                    admin->displayEventStats();
                    break;
                case 4:
                    admin->viewRegistrationReports();
                    break;
                case 5:
                    admin->manageUsers();
                    break;
                case 6:
                    cout << "\nLogging out..." << endl;
                    cout << "Thank you for using Admin panel!" << endl;
                    running = false;
                    break;
                default:
                    cout << "Invalid choice! Please select 1-6." << endl;
            }
        } 
        else if (Student* student = dynamic_cast<Student*>(currentUser)) {
            // Student-specific menu handling
            switch (choice) {
                case 1: {
                    student->viewAvailableEvents();
                    cout << "\nWould you like to register for an event? (yes/no): ";
                    string regChoice;
                    getline(cin, regChoice);
                    if (toLower(trim(regChoice)) == "yes") {
                        student->registerForEvent();
                    }
                    break;
                }
                case 2:
                    student->viewMyRegistrations();
                    break;
                case 3: {
                    cout << "1. Search by Name" << endl;
                    cout << "2. Filter by Date" << endl;
                    cout << "Choose option: ";
                    int searchChoice;
                    cin >> searchChoice;
                    cin.ignore();
                    
                    if (searchChoice == 1) {
                        student->searchEventByName();
                    } else if (searchChoice == 2) {
                        student->filterEventsByDate();
                    } else {
                        cout << "Invalid choice!" << endl;
                    }
                    break;
                }
                case 4:
                    cout << "\nLogging out..." << endl;
                    cout << "Thank you for using Student portal!" << endl;
                    running = false;
                    break;
                default:
                    cout << "Invalid choice! Please select 1-4." << endl;
            }
        }
    }
    
    // Cleanup memory to prevent memory leaks
    delete currentUser;
    cout << "\n=== SESSION ENDED ===" << endl;
    cout << "Thank you for using College Event Management System!" << endl;
    
    return 0;
}
