#include "student.h"

// ========================================
// STUDENT.CPP - Student Class Implementation
// ========================================
// This file implements the Student class which inherits from User.
// Students can: browse events, register/unregister, view their registrations, search events.
// Demonstrates: Inheritance, File I/O, Vector manipulation, Searching/Filtering

// Constructor with base class initialization
// What it does: Creates a Student object by passing data to the User constructor
// The colon syntax calls the parent class (User) constructor
Student::Student(string uname, string pass, string name) : User(uname, pass, name) {}

// Polymorphism: Student-specific menu implementation
// What it does: Overrides the pure virtual function from User base class
// This is the menu shown to student users (different from admin menu)
void Student::displayMenu() {
    cout << "\n=== STUDENT DASHBOARD ===" << endl;
    cout << "Welcome, " << fullName << "!" << endl;  // fullName inherited from User
    cout << "1. Browse Available Events" << endl;    // View and register for events
    cout << "2. My Registrations" << endl;           // See what they're registered for
    cout << "3. Search Events" << endl;              // Search by name or filter by date
    cout << "4. Logout" << endl;                     // Exit student portal
    cout << "Choose an option: ";
}

// Virtual method implementation
// What it does: Returns the user type as a string
// Called polymorphically in main.cpp
string Student::getUserType() {
    return "Student";
}

// ==================== FILE I/O OPERATIONS ====================
// Students need to read/write events and registrations files
// These functions are similar to Admin's but Student doesn't have extra features

// Load events from file
// What it does: Reads events.txt and creates Event objects
// Why students need this: To browse available events
vector<Event> Student::loadEventsFromFile() {
    vector<Event> events;
    ifstream file("data/events.txt");
    string line;
    
    if (!file.is_open()) {
        return events;    // Return empty vector if file doesn't exist
    }
    
    // Read and parse each line
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        vector<string> parts = split(line, '|');
        if (parts.size() >= 4) {
            string name = trim(parts[0]);
            string date = trim(parts[1]);
            string venue = trim(parts[2]);
            int capacity = stoi(trim(parts[3]));
            int registered = (parts.size() > 4) ? stoi(trim(parts[4])) : 0;
            
            events.push_back(Event(name, date, venue, capacity, registered));
        }
    }
    
    file.close();
    return events;
}

// Save events to file
// What it does: Writes updated events back to file
// Why students need this: When they register/unregister, the registered count changes
bool Student::saveEventsToFile(const vector<Event>& events) {
    ofstream file("data/events.txt");
    
    if (!file.is_open()) {
        return false;
    }
    
    for (const auto& event : events) {
        file << event.toFileFormat() << endl;
    }
    
    file.close();
    return true;
}

// Load registrations from file
// What it does: Reads all registrations to check what events user is registered for
vector<Registration> Student::loadRegistrationsFromFile() {
    vector<Registration> registrations;
    ifstream file("data/registrations.txt");
    string line;
    
    if (!file.is_open()) {
        return registrations;
    }
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        vector<string> parts = split(line, '|');
        if (parts.size() == 3) {
            registrations.push_back(Registration(trim(parts[0]), trim(parts[1]), trim(parts[2])));
        }
    }
    
    file.close();
    return registrations;
}

// Save registrations to file
// What it does: Writes updated registrations back to file
// Called when: Student registers or unregisters from an event
bool Student::saveRegistrationsToFile(const vector<Registration>& registrations) {
    ofstream file("data/registrations.txt");
    
    if (!file.is_open()) {
        return false;
    }
    
    for (const auto& reg : registrations) {
        file << reg.toFileFormat() << endl;
    }
    
    file.close();
    return true;
}

// Get current date and time
// What it does: Returns current system date/time as a formatted string
// Used when: Recording when a student registered for an event
// Returns: String in format "DD-MM-YYYY HH:MM"
string Student::getCurrentDateTime() {
    time_t now = time(0);                  // Get current time as seconds since epoch
    tm* timeinfo = localtime(&now);         // Convert to local time structure
    char buffer[20];                        // Character array to hold formatted string
    // strftime formats the time according to the format string
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M", timeinfo);
    return string(buffer);                  // Convert char array to string
}

// View available events for registration
void Student::viewAvailableEvents() {
    vector<Event> events = loadEventsFromFile();
    
    cout << "\n=== AVAILABLE EVENTS FOR REGISTRATION ===" << endl;
    
    if (events.empty()) {
        cout << "No events available for registration!" << endl;
        return;
    }
    
    cout << "\n" << string(110, '=') << endl;
    cout << "  " << left << setw(2) << "#" << "  "
         << setw(25) << "EVENT NAME" << " | "
         << setw(12) << "DATE" << " | "
         << setw(20) << "VENUE" << " | "
         << setw(4) << "CAP" << " | "
         << setw(4) << "REG" << " | "
         << setw(4) << "AVL" << endl;
    cout << string(110, '=') << endl;
    
    for (size_t i = 0; i < events.size(); i++) {
        events[i].display(i + 1);
    }
    cout << string(110, '=') << endl;
}

// View my registrations
void Student::viewMyRegistrations() {
    vector<Event> events = loadEventsFromFile();
    vector<Registration> registrations = loadRegistrationsFromFile();
    
    cout << "\n=== MY REGISTRATIONS ===" << endl;
    
    vector<Registration> myRegs;
    for (const auto& reg : registrations) {
        if (reg.getStudentUsername() == username) {
            myRegs.push_back(reg);
        }
    }
    
    if (myRegs.empty()) {
        cout << "You are not registered for any events!" << endl;
        return;
    }
    
    cout << "==========================================" << endl;
    cout << "You are registered for " << myRegs.size() << " event(s):" << endl;
    cout << "==========================================" << endl;
    
    for (size_t i = 0; i < myRegs.size(); i++) {
        cout << i + 1 << ". " << myRegs[i].getEventName() 
             << " (Registered: " << myRegs[i].getRegistrationDate() << ")" << endl;
    }
    
    cout << "\nOptions:" << endl;
    cout << "1. View Event Details" << endl;
    cout << "2. Unregister from Event" << endl;
    cout << "3. Back" << endl;
    cout << "Choose option: ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    if (choice == 1) {
        cout << "Enter event number: ";
        int eventNum;
        cin >> eventNum;
        cin.ignore();
        
        if (eventNum >= 1 && eventNum <= (int)myRegs.size()) {
            for (const auto& event : events) {
                if (event.getEventName() == myRegs[eventNum - 1].getEventName()) {
                    event.displayDetailed(eventNum);
                    break;
                }
            }
        }
    } else if (choice == 2) {
        cout << "Enter event number to unregister: ";
        int eventNum;
        cin >> eventNum;
        cin.ignore();
        
        if (eventNum >= 1 && eventNum <= (int)myRegs.size()) {
            unregisterFromEvent(myRegs[eventNum - 1].getEventName());
        }
    }
}

// ==================== REGISTRATION OPERATIONS ====================

// Register for an event
// What it does: Allows student to sign up for an event
// Process: Load data → Show events → Get choice → Validate → Update files
void Student::registerForEvent() {
    // Load current data from files
    vector<Event> events = loadEventsFromFile();
    vector<Registration> registrations = loadRegistrationsFromFile();
    
    // Check if there are any events to register for
    if (events.empty()) {
        cout << "No events available!" << endl;
        return;
    }
    
    // Show available events to the student
    viewAvailableEvents();
    
    // Get student's choice
    cout << "\nEnter event number to register (0 to cancel): ";
    int eventNum;
    cin >> eventNum;
    cin.ignore();    // Clear input buffer
    
    // Validate the choice (must be valid event number)
    if (eventNum < 1 || eventNum > (int)events.size()) {
        cout << "Invalid selection!" << endl;
        return;
    }
    
    // Get reference to the selected event (arrays/vectors are 0-indexed, display is 1-indexed)
    Event& selectedEvent = events[eventNum - 1];
    string eventName = selectedEvent.getEventName();
    
    // VALIDATION 1: Check if already registered
    // Loop through all registrations to see if this student already registered for this event
    for (const auto& reg : registrations) {
        if (reg.getStudentUsername() == username && reg.getEventName() == eventName) {
            cout << "Error: You are already registered for this event!" << endl;
            return;    // Exit early if duplicate found
        }
    }
    
    // VALIDATION 2: Check if event has capacity
    if (!selectedEvent.hasAvailableSeats()) {
        cout << "Error: Event is full! No available seats." << endl;
        return;
    }
    
    // All validations passed - proceed with registration
    // Step 1: Increment the registered count for the event
    selectedEvent.registerStudent();
    
    // Step 2: Create a new Registration object and add it to the vector
    // 'username' is inherited from User base class
    registrations.push_back(Registration(username, eventName, getCurrentDateTime()));
    
    // Step 3: Save both updated vectors back to files
    // && means "and" - both saves must succeed
    if (saveEventsToFile(events) && saveRegistrationsToFile(registrations)) {
        cout << "\nSuccess! You have been registered for '" << eventName << "'!" << endl;
    } else {
        cout << "Error: Registration failed!" << endl;
    }
}

// Unregister from an event
// What it does: Removes a student's registration from an event
// Process: Find the registration → Remove it → Update event count → Save files
void Student::unregisterFromEvent(const string& eventName) {
    // Load current data
    vector<Event> events = loadEventsFromFile();
    vector<Registration> registrations = loadRegistrationsFromFile();
    
    // Find and remove registration using find_if algorithm
    // find_if searches for the first element that matches the condition
    // LAMBDA FUNCTION: [this, &eventName](const Registration& r) { ... }
    // - [this, &eventName] = capture clause (what variables from outside the lambda can be used)
    // - (const Registration& r) = parameter (each Registration as it's checked)
    // - { return ... } = function body that returns true if this is the registration to remove
    auto it = find_if(registrations.begin(), registrations.end(),
        [this, &eventName](const Registration& r) {
            return r.getStudentUsername() == username && r.getEventName() == eventName;
        });
    
    // Check if registration was found
    // If find_if doesn't find anything, it returns registrations.end()
    if (it == registrations.end()) {
        cout << "Error: Registration not found!" << endl;
        return;
    }
    
    // Remove the registration from the vector
    // 'it' is an iterator (like a pointer) to the element we want to remove
    registrations.erase(it);
    
    // Find the event and decrease its registered count
    for (auto& event : events) {
        if (event.getEventName() == eventName) {
            event.unregisterStudent();    // Decrements registeredCount
            break;    // Found the event, no need to continue looping
        }
    }
    
    // Save updated data back to files
    if (saveEventsToFile(events) && saveRegistrationsToFile(registrations)) {
        cout << "Success! You have been unregistered from '" << eventName << "'!" << endl;
    } else {
        cout << "Error: Unregistration failed!" << endl;
    }
}

// ==================== SEARCH AND FILTER OPERATIONS ====================

// Search events by name
// What it does: Finds events whose names contain the search term (case-insensitive)
// Example: Searching "tech" will find "Tech Fest 2025" and "Robotech Workshop"
void Student::searchEventByName() {
    vector<Event> events = loadEventsFromFile();
    
    cout << "\n=== SEARCH EVENTS ===" << endl;
    cout << "Enter event name to search: ";
    
    // Get search term from user
    string searchTerm;
    getline(cin, searchTerm);
    searchTerm = toLower(trim(searchTerm));    // Convert to lowercase for case-insensitive search
    
    // Validate input
    if (searchTerm.empty()) {
        cout << "Search term cannot be empty!" << endl;
        return;
    }
    
    // Search through events and collect matching ones
    vector<Event> results;    // Vector to store matching events
    for (const auto& event : events) {
        // find() returns string::npos if the substring is not found
        // So if find() doesn't return npos, it means the search term was found
        if (toLower(event.getEventName()).find(searchTerm) != string::npos) {
            results.push_back(event);    // Add matching event to results
        }
    }
    
    // Check if any events matched
    if (results.empty()) {
        cout << "No events found matching '" << searchTerm << "'!" << endl;
        return;
    }
    
    // Display search results in a formatted table
    cout << "\n=== SEARCH RESULTS ===" << endl;
    cout << "\n" << string(110, '=') << endl;    // string(110, '=') creates a string of 110 '=' characters
    
    // Table header
    cout << "  " << left << setw(2) << "#" << "  "
         << setw(25) << "EVENT NAME" << " | "
         << setw(12) << "DATE" << " | "
         << setw(20) << "VENUE" << " | "
         << setw(4) << "CAP" << " | "
         << setw(4) << "REG" << " | "
         << setw(4) << "AVL" << endl;
    cout << string(110, '=') << endl;
    
    // Display each matching event
    // size_t is an unsigned integer type for sizes/counts
    for (size_t i = 0; i < results.size(); i++) {
        results[i].display(i + 1);    // Display with 1-based numbering
    }
    cout << string(110, '=') << endl;
    cout << "Found " << results.size() << " event(s)" << endl << endl;
}

// Filter events by date
void Student::filterEventsByDate() {
    vector<Event> events = loadEventsFromFile();
    
    cout << "\n=== FILTER EVENTS BY DATE ===" << endl;
    cout << "Enter date (DD-MM-YYYY): ";
    
    string searchDate;
    getline(cin, searchDate);
    searchDate = trim(searchDate);
    
    if (!isValidDate(searchDate)) {
        cout << "Invalid date format!" << endl;
        return;
    }
    
    vector<Event> results;
    for (const auto& event : events) {
        if (event.getDate() == searchDate) {
            results.push_back(event);
        }
    }
    
    if (results.empty()) {
        cout << "No events found on " << searchDate << "!" << endl;
        return;
    }
    
    cout << "\n=== EVENTS ON " << searchDate << " ===" << endl;
    cout << "\n" << string(110, '=') << endl;
    cout << "  " << left << setw(2) << "#" << "  "
         << setw(25) << "EVENT NAME" << " | "
         << setw(12) << "DATE" << " | "
         << setw(20) << "VENUE" << " | "
         << setw(4) << "CAP" << " | "
         << setw(4) << "REG" << " | "
         << setw(4) << "AVL" << endl;
    cout << string(110, '=') << endl;
    
    for (size_t i = 0; i < results.size(); i++) {
        results[i].display(i + 1);
    }
    cout << string(110, '=') << endl;
    cout << "Found " << results.size() << " event(s)" << endl << endl;
}
