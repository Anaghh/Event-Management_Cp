#include "admin.h"

// ========================================
// ADMIN.CPP - Admin Class Implementation
// ========================================
// This file implements the Admin class which inherits from User.
// Admin has full control: manage events (create/edit/delete), view reports, manage users.
// Demonstrates: Inheritance, File I/O, CRUD operations, Vector manipulation

// Constructor calling base class constructor
// What it does: Creates an Admin object by passing data to the User constructor
// The colon syntax ": User(uname, pass, name)" calls the parent class constructor
// This is called "constructor chaining" or "constructor initialization list"
Admin::Admin(string uname, string pass, string name) : User(uname, pass, name) {}

// Polymorphism: Admin-specific menu implementation
// What it does: Overrides the pure virtual function from User base class
// This is the menu shown to admin users
// 'override' keyword (in header) tells compiler we're intentionally overriding a virtual function
void Admin::displayMenu() {
    cout << "\n=== ADMIN DASHBOARD ===" << endl;
    cout << "Welcome, " << fullName << "!" << endl;  // fullName inherited from User class
    cout << "1. Manage Events" << endl;              // Add/Edit/Delete events
    cout << "2. View All Events" << endl;            // List all events
    cout << "3. View Event Statistics" << endl;      // Show occupancy stats
    cout << "4. View Registration Reports" << endl;  // See who registered for what
    cout << "5. Manage Users" << endl;               // Add students, view users
    cout << "6. Logout" << endl;                     // Exit admin panel
    cout << "Choose an option: ";
}

// Virtual method implementation
// What it does: Returns the user type as a string
// This is called polymorphically in main.cpp to display user type
string Admin::getUserType() {
    return "Admin";
}

// ==================== FILE I/O OPERATIONS ====================
// These functions handle reading and writing data to text files
// This is how we persist data (save it permanently) without a database

// Load events from file into vector
// What it does: Reads events.txt and creates Event objects from the data
// Returns: Vector (dynamic array) containing all Event objects
vector<Event> Admin::loadEventsFromFile() {
    vector<Event> events;              // Create empty vector to store events
    ifstream file("data/events.txt");  // Open file for reading (ifstream = input file stream)
    string line;                       // Variable to store each line
    
    // Check if file opened successfully
    if (!file.is_open()) {
        cout << "Error: Could not open events.txt file!" << endl;
        return events;    // Return empty vector
    }
    
    // Read file line by line
    while (getline(file, line)) {
        if (line.empty()) continue;    // Skip empty lines
        
        // Split the line by pipe (|) delimiter
        // File format: eventname|date|venue|capacity|registered
        vector<string> parts = split(line, '|');
        
        // Make sure we have at least 4 parts (registered count is optional)
        if (parts.size() >= 4) {
            // Extract and clean each field
            string name = trim(parts[0]);
            string date = trim(parts[1]);
            string venue = trim(parts[2]);
            int capacity = stoi(trim(parts[3]));  // stoi = "string to integer"
            
            // Ternary operator: (condition ? if_true : if_false)
            // If there's a 5th part, use it; otherwise use 0
            int registered = (parts.size() > 4) ? stoi(trim(parts[4])) : 0;
            
            // Create Event object and add it to the vector
            // push_back() adds an element to the end of the vector
            events.push_back(Event(name, date, venue, capacity, registered));
        }
    }
    
    file.close();    // Always close files when done
    return events;   // Return vector containing all events
}

// Save events to file
// What it does: Writes all Event objects back to events.txt
// Parameters: 'const vector<Event>&' means we pass by reference (efficient) and won't modify it
// Returns: true if successful, false if error
bool Admin::saveEventsToFile(const vector<Event>& events) {
    ofstream file("data/events.txt");  // Open file for writing (ofstream = output file stream)
    
    // Check if file opened successfully
    if (!file.is_open()) {
        cout << "Error: Could not open events.txt for writing!" << endl;
        return false;
    }
    
    // Write each event to the file
    // 'const auto&' = compiler figures out the type (Event), const means read-only, & means reference
    for (const auto& event : events) {
        // toFileFormat() converts Event object to pipe-delimited string
        file << event.toFileFormat() << endl;
    }
    
    file.close();    // Always close files when done
    return true;     // Indicate success
}

// Load registrations from file
// What it does: Reads registrations.txt and creates Registration objects
// Similar to loadEventsFromFile but for registrations
// Returns: Vector of Registration objects
vector<Registration> Admin::loadRegistrationsFromFile() {
    vector<Registration> registrations;      // Empty vector to store registrations
    ifstream file("data/registrations.txt"); // Open registrations file
    string line;
    
    // If file doesn't exist or can't open, return empty vector (not an error)
    if (!file.is_open()) {
        return registrations;
    }
    
    // Read each line from the file
    while (getline(file, line)) {
        if (line.empty()) continue;    // Skip empty lines
        
        // Split by pipe delimiter
        // File format: username|eventname|registrationdate
        vector<string> parts = split(line, '|');
        
        // Must have exactly 3 parts
        if (parts.size() == 3) {
            // Create Registration object and add to vector
            registrations.push_back(Registration(trim(parts[0]), trim(parts[1]), trim(parts[2])));
        }
    }
    
    file.close();
    return registrations;
}

// Save registrations to file
// What it does: Writes all Registration objects back to registrations.txt
// Called after: A student registers or unregisters from an event
bool Admin::saveRegistrationsToFile(const vector<Registration>& registrations) {
    ofstream file("data/registrations.txt");  // Open for writing
    
    if (!file.is_open()) {
        cout << "Error: Could not open registrations.txt for writing!" << endl;
        return false;
    }
    
    // Write each registration to file
    for (const auto& reg : registrations) {
        file << reg.toFileFormat() << endl;
    }
    
    file.close();
    return true;
}

// ==================== EVENT MANAGEMENT MENU ====================

// Admin menu for event management (CRUD operations)
// What it does: Shows a submenu for managing events
// CRUD = Create, Read, Update, Delete
void Admin::manageEvents() {
    bool managing = true;    // Control variable for the loop
    
    // Loop until user chooses to go back
    while (managing) {
        cout << "\n=== EVENT MANAGEMENT ===" << endl;
        cout << "1. Add New Event" << endl;          // CREATE
        cout << "2. Edit Event" << endl;             // UPDATE
        cout << "3. Delete Event" << endl;           // DELETE
        cout << "4. Back to Dashboard" << endl;
        cout << "Choose an option: ";
        
        int choice;
        cin >> choice;      // Get user choice
        cin.ignore();       // Clear input buffer (important for mixing cin >> and getline)
        
        // Switch statement for menu navigation
        // More efficient than multiple if-else when checking same variable
        switch (choice) {
            case 1:
                addNewEvent();    // Call function to add a new event
                break;            // Exit switch (not the loop)
            case 2:
                editEvent();      // Call function to modify existing event
                break;
            case 3:
                deleteEvent();    // Call function to remove an event
                break;
            case 4:
                managing = false; // Set to false to exit the loop
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    }
}

// Add new event (CREATE operation)
// What it does: Gets event details from admin and adds it to the system
// Input validation: Checks for empty fields, duplicate names, valid date format
void Admin::addNewEvent() {
    cout << "\n=== ADD NEW EVENT ===" << endl;
    
    // Declare variables to store event details
    string eventName, date, venue;
    int capacity;
    
    // Get event name
    cout << "Event Name: ";
    getline(cin, eventName);    // getline reads entire line (allows spaces in name)
    eventName = trim(eventName); // Remove leading/trailing whitespace
    
    // Validate: Name cannot be empty
    if (eventName.empty()) {
        cout << "Error: Event name cannot be empty!" << endl;
        return;    // Exit function early if validation fails
    }
    
    // Check for duplicate event names
    vector<Event> events = loadEventsFromFile();  // Load existing events
    for (const auto& e : events) {
        // Case-insensitive comparison using toLower()
        if (toLower(e.getEventName()) == toLower(eventName)) {
            cout << "Error: Event with this name already exists!" << endl;
            return;    // Exit if duplicate found
        }
    }
    
    // Get and validate date
    cout << "Date (DD-MM-YYYY): ";
    getline(cin, date);
    date = trim(date);
    
    // isValidDate() checks format and value ranges
    if (!isValidDate(date)) {
        cout << "Error: Invalid date format! Use DD-MM-YYYY format." << endl;
        return;
    }
    
    // Get and validate venue
    cout << "Venue: ";
    getline(cin, venue);
    venue = trim(venue);
    
    if (venue.empty()) {
        cout << "Error: Venue cannot be empty!" << endl;
        return;
    }
    
    // Get and validate capacity
    cout << "Capacity: ";
    cin >> capacity;
    
    if (capacity <= 0) {
        cout << "Error: Capacity must be greater than 0!" << endl;
        return;
    }
    
    // All validations passed - add new event to vector
    // Last parameter (0) means 0 students registered initially
    events.push_back(Event(eventName, date, venue, capacity, 0));
    
    // Save updated vector back to file
    if (saveEventsToFile(events)) {
        cout << "\nSuccess! Event '" << eventName << "' added successfully!" << endl;
    } else {
        cout << "Error: Failed to save event!" << endl;
    }
}

// Edit event
void Admin::editEvent() {
    cout << "\n=== EDIT EVENT ===" << endl;
    
    vector<Event> events = loadEventsFromFile();
    if (events.empty()) {
        cout << "No events to edit!" << endl;
        return;
    }
    
    viewAllEvents();
    
    cout << "Enter event number to edit (0 to cancel): ";
    int eventNum;
    cin >> eventNum;
    cin.ignore();
    
    if (eventNum < 1 || eventNum > (int)events.size()) {
        cout << "Invalid selection!" << endl;
        return;
    }
    
    Event& event = events[eventNum - 1];
    
    cout << "\nEditing: " << event.getEventName() << endl;
    cout << "1. Edit Name" << endl;
    cout << "2. Edit Date" << endl;
    cout << "3. Edit Venue" << endl;
    cout << "4. Edit Capacity" << endl;
    cout << "5. Done" << endl;
    cout << "Choose option: ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    switch (choice) {
        case 1: {
            string newName;
            cout << "New event name: ";
            getline(cin, newName);
            newName = trim(newName);
            
            if (!newName.empty()) {
                bool duplicate = false;
                for (const auto& e : events) {
                    if (toLower(e.getEventName()) == toLower(newName) && 
                        toLower(e.getEventName()) != toLower(event.getEventName())) {
                        duplicate = true;
                        break;
                    }
                }
                
                if (duplicate) {
                    cout << "Error: Event with this name already exists!" << endl;
                } else {
                    event.setEventName(newName);
                    cout << "Name updated successfully!" << endl;
                }
            }
            break;
        }
        case 2: {
            string newDate;
            cout << "New date (DD-MM-YYYY): ";
            getline(cin, newDate);
            newDate = trim(newDate);
            
            if (isValidDate(newDate)) {
                event.setDate(newDate);
                cout << "Date updated successfully!" << endl;
            } else {
                cout << "Error: Invalid date format!" << endl;
            }
            break;
        }
        case 3: {
            string newVenue;
            cout << "New venue: ";
            getline(cin, newVenue);
            newVenue = trim(newVenue);
            
            if (!newVenue.empty()) {
                event.setVenue(newVenue);
                cout << "Venue updated successfully!" << endl;
            }
            break;
        }
        case 4: {
            int newCapacity;
            cout << "New capacity: ";
            cin >> newCapacity;
            
            if (newCapacity < event.getRegisteredCount()) {
                cout << "Error: New capacity cannot be less than registered count (" 
                     << event.getRegisteredCount() << ")!" << endl;
            } else if (newCapacity > 0) {
                event.setCapacity(newCapacity);
                cout << "Capacity updated successfully!" << endl;
            } else {
                cout << "Error: Capacity must be greater than 0!" << endl;
            }
            break;
        }
        case 5:
            break;
        default:
            cout << "Invalid choice!" << endl;
            return;
    }
    
    if (choice >= 1 && choice <= 4) {
        if (saveEventsToFile(events)) {
            cout << "Changes saved successfully!" << endl;
        } else {
            cout << "Error: Failed to save changes!" << endl;
        }
    }
}

// Delete event
void Admin::deleteEvent() {
    cout << "\n=== DELETE EVENT ===" << endl;
    
    vector<Event> events = loadEventsFromFile();
    if (events.empty()) {
        cout << "No events to delete!" << endl;
        return;
    }
    
    viewAllEvents();
    
    cout << "Enter event number to delete (0 to cancel): ";
    int eventNum;
    cin >> eventNum;
    cin.ignore();
    
    if (eventNum < 1 || eventNum > (int)events.size()) {
        cout << "Invalid selection!" << endl;
        return;
    }
    
    cout << "\nDeleting: " << events[eventNum - 1].getEventName() << endl;
    cout << "This will also remove all registrations for this event!" << endl;
    cout << "Are you sure? (yes/no): ";
    
    string confirmation;
    getline(cin, confirmation);
    
    if (toLower(trim(confirmation)) == "yes") {
        string deletedEventName = events[eventNum - 1].getEventName();
        events.erase(events.begin() + eventNum - 1);
        
        if (saveEventsToFile(events)) {
            vector<Registration> registrations = loadRegistrationsFromFile();
            vector<Registration> updatedRegistrations;
            
            // Loop through all registrations and keep only those not matching the deleted event
            for (const auto& reg : registrations) {
                if (reg.getEventName() != deletedEventName) {
                    updatedRegistrations.push_back(reg);
                }
            }
            
            // Replace the original vector with the filtered one
            registrations = updatedRegistrations;
            saveRegistrationsToFile(registrations);
            
            cout << "Event deleted successfully!" << endl;
        } else {
            cout << "Error: Failed to delete event!" << endl;
        }
    } else {
        cout << "Deletion cancelled!" << endl;
    }
}

// Admin functionality: Read and display events from file
void Admin::viewAllEvents() {
    vector<Event> events = loadEventsFromFile();
    
    cout << "\n=== ALL EVENTS ===" << endl;
    
    if (events.empty()) {
        cout << "No events found in the system!" << endl;
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
    cout << "Total events: " << events.size() << endl << endl;
}

// Display event statistics
void Admin::displayEventStats() {
    vector<Event> events = loadEventsFromFile();
    vector<Registration> registrations = loadRegistrationsFromFile();
    
    cout << "\n=== EVENT STATISTICS ===" << endl;
    
    int totalEvents = events.size();
    int totalCapacity = 0;
    int totalRegistered = 0;
    
    for (const auto& event : events) {
        totalCapacity += event.getCapacity();
        totalRegistered += event.getRegisteredCount();
    }
    
    cout << "Total Events: " << totalEvents << endl;
    cout << "Total Capacity: " << totalCapacity << endl;
    cout << "Total Registrations: " << totalRegistered << endl;
    cout << "Overall Occupancy: " << fixed << setprecision(1) 
         << (totalCapacity > 0 ? (totalRegistered * 100.0 / totalCapacity) : 0) << "%" << endl;
    
    cout << "\nEvent-wise Breakdown:" << endl;
    cout << "-------------------------------------------" << endl;
    for (const auto& event : events) {
        cout << event.getEventName() << ": " << event.getRegisteredCount() 
             << "/" << event.getCapacity() << " (" << fixed << setprecision(1)
             << (event.getCapacity() > 0 ? (event.getRegisteredCount() * 100.0 / event.getCapacity()) : 0) 
             << "%)" << endl;
    }
}

// View registration reports
void Admin::viewRegistrationReports() {
    cout << "\n=== REGISTRATION REPORTS ===" << endl;
    
    vector<Event> events = loadEventsFromFile();
    vector<Registration> registrations = loadRegistrationsFromFile();
    
    if (events.empty()) {
        cout << "No events in the system!" << endl;
        return;
    }
    
    cout << "Select event to view participants (0 to view summary):" << endl;
    viewAllEvents();
    
    cout << "Enter event number (0 to view all): ";
    int choice;
    cin >> choice;
    cin.ignore();
    
    if (choice == 0) {
        cout << "\n=== REGISTRATION SUMMARY ===" << endl;
        for (const auto& event : events) {
            int count = 0;
            for (const auto& reg : registrations) {
                if (reg.getEventName() == event.getEventName()) {
                    count++;
                }
            }
            cout << event.getEventName() << ": " << count << " registrations" << endl;
        }
    } else if (choice >= 1 && choice <= (int)events.size()) {
        string eventName = events[choice - 1].getEventName();
        cout << "\n=== PARTICIPANTS FOR: " << eventName << " ===" << endl;
        
        int count = 0;
        for (const auto& reg : registrations) {
            if (reg.getEventName() == eventName) {
                cout << "  " << ++count << ". " << reg.getStudentUsername() 
                     << " (Registered: " << reg.getRegistrationDate() << ")" << endl;
            }
        }
        
        if (count == 0) {
            cout << "No registrations for this event!" << endl;
        } else {
            cout << "\nTotal Participants: " << count << endl;
        }
    } else {
        cout << "Invalid selection!" << endl;
    }
}

// Manage users (add new student)
void Admin::manageUsers() {
    cout << "\n=== USER MANAGEMENT ===" << endl;
    cout << "1. Add New Student" << endl;
    cout << "2. View All Users" << endl;
    cout << "3. Back" << endl;
    cout << "Choose option: ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    switch (choice) {
        case 1:
            addNewStudent();
            break;
        case 2:
            viewAllUsers();
            break;
        case 3:
            break;
        default:
            cout << "Invalid choice!" << endl;
    }
}

// Add new student account
void Admin::addNewStudent() {
    cout << "\n=== ADD NEW STUDENT ===" << endl;
    
    string username, password, fullName;
    
    cout << "Username: ";
    getline(cin, username);
    username = trim(username);
    
    if (username.empty()) {
        cout << "Error: Username cannot be empty!" << endl;
        return;
    }
    
    // Check if username exists
    ifstream file("data/users.txt");
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        vector<string> parts = split(line, ',');
        if (parts.size() > 0 && trim(parts[0]) == username) {
            cout << "Error: Username already exists!" << endl;
            file.close();
            return;
        }
    }
    file.close();
    
    cout << "Password: ";
    getline(cin, password);
    
    if (password.empty()) {
        cout << "Error: Password cannot be empty!" << endl;
        return;
    }
    
    cout << "Full Name: ";
    getline(cin, fullName);
    fullName = trim(fullName);
    
    if (fullName.empty()) {
        cout << "Error: Full name cannot be empty!" << endl;
        return;
    }
    
    // Append to users.txt
    ofstream outfile("data/users.txt", ios::app);
    if (!outfile.is_open()) {
        cout << "Error: Could not open users.txt for writing!" << endl;
        return;
    }
    
    outfile << username << "," << password << "," << fullName << ",student" << endl;
    outfile.close();
    
    cout << "Success! Student account created successfully!" << endl;
}

// View all users
void Admin::viewAllUsers() {
    cout << "\n=== ALL USERS ===" << endl;
    
    ifstream file("data/users.txt");
    string line;
    int count = 0;
    
    if (!file.is_open()) {
        cout << "Error: Could not open users.txt!" << endl;
        return;
    }
    
    cout << "====================================" << endl;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        vector<string> parts = split(line, ',');
        if (parts.size() >= 4) {
            cout << ++count << ". Username: " << trim(parts[0]) 
                 << " | Name: " << trim(parts[2]) 
                 << " | Type: " << trim(parts[3]) << endl;
        }
    }
    cout << "====================================" << endl;
    cout << "Total Users: " << count << endl;
    
    file.close();
}
