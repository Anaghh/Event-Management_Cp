#include "admin.h"

// Constructor calling base class constructor
Admin::Admin(string uname, string pass, string name) : User(uname, pass, name) {}

// Polymorphism: Admin-specific menu implementation
void Admin::displayMenu() {
    cout << "\n=== ADMIN DASHBOARD ===" << endl;
    cout << "Welcome, " << fullName << "!" << endl;
    cout << "1. Manage Events" << endl;
    cout << "2. View All Events" << endl;
    cout << "3. View Event Statistics" << endl;
    cout << "4. View Registration Reports" << endl;
    cout << "5. Manage Users" << endl;
    cout << "6. Logout" << endl;
    cout << "Choose an option: ";
}

// Virtual method implementation
string Admin::getUserType() {
    return "Admin";
}

// Load events from file into vector
vector<Event> Admin::loadEventsFromFile() {
    vector<Event> events;
    ifstream file("data/events.txt");
    string line;
    
    if (!file.is_open()) {
        cout << "Error: Could not open events.txt file!" << endl;
        return events;
    }
    
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
bool Admin::saveEventsToFile(const vector<Event>& events) {
    ofstream file("data/events.txt");
    
    if (!file.is_open()) {
        cout << "Error: Could not open events.txt for writing!" << endl;
        return false;
    }
    
    for (const auto& event : events) {
        file << event.toFileFormat() << endl;
    }
    
    file.close();
    return true;
}

// Load registrations from file
vector<Registration> Admin::loadRegistrationsFromFile() {
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
bool Admin::saveRegistrationsToFile(const vector<Registration>& registrations) {
    ofstream file("data/registrations.txt");
    
    if (!file.is_open()) {
        cout << "Error: Could not open registrations.txt for writing!" << endl;
        return false;
    }
    
    for (const auto& reg : registrations) {
        file << reg.toFileFormat() << endl;
    }
    
    file.close();
    return true;
}

// Admin menu for event management
void Admin::manageEvents() {
    bool managing = true;
    
    while (managing) {
        cout << "\n=== EVENT MANAGEMENT ===" << endl;
        cout << "1. Add New Event" << endl;
        cout << "2. Edit Event" << endl;
        cout << "3. Delete Event" << endl;
        cout << "4. Back to Dashboard" << endl;
        cout << "Choose an option: ";
        
        int choice;
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                addNewEvent();
                break;
            case 2:
                editEvent();
                break;
            case 3:
                deleteEvent();
                break;
            case 4:
                managing = false;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    }
}

// Add new event
void Admin::addNewEvent() {
    cout << "\n=== ADD NEW EVENT ===" << endl;
    
    string eventName, date, venue;
    int capacity;
    
    cout << "Event Name: ";
    getline(cin, eventName);
    eventName = trim(eventName);
    
    if (eventName.empty()) {
        cout << "Error: Event name cannot be empty!" << endl;
        return;
    }
    
    // Check for duplicate
    vector<Event> events = loadEventsFromFile();
    for (const auto& e : events) {
        if (toLower(e.getEventName()) == toLower(eventName)) {
            cout << "Error: Event with this name already exists!" << endl;
            return;
        }
    }
    
    cout << "Date (DD-MM-YYYY): ";
    getline(cin, date);
    date = trim(date);
    
    if (!isValidDate(date)) {
        cout << "Error: Invalid date format! Use DD-MM-YYYY format." << endl;
        return;
    }
    
    cout << "Venue: ";
    getline(cin, venue);
    venue = trim(venue);
    
    if (venue.empty()) {
        cout << "Error: Venue cannot be empty!" << endl;
        return;
    }
    
    cout << "Capacity: ";
    cin >> capacity;
    
    if (capacity <= 0) {
        cout << "Error: Capacity must be greater than 0!" << endl;
        return;
    }
    
    // Add new event
    events.push_back(Event(eventName, date, venue, capacity, 0));
    
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
            registrations.erase(
                remove_if(registrations.begin(), registrations.end(),
                [&](const Registration& r) { return r.getEventName() == deletedEventName; }),
                registrations.end()
            );
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
         << "AVL" << endl;
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
