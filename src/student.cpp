#include "student.h"

// Constructor with base class initialization
Student::Student(string uname, string pass, string name) : User(uname, pass, name) {}

// Polymorphism: Student-specific menu implementation
void Student::displayMenu() {
    cout << "\n=== STUDENT DASHBOARD ===" << endl;
    cout << "Welcome, " << fullName << "!" << endl;
    cout << "1. Browse Available Events" << endl;
    cout << "2. My Registrations" << endl;
    cout << "3. Search Events" << endl;
    cout << "4. Logout" << endl;
    cout << "Choose an option: ";
}

// Virtual method implementation
string Student::getUserType() {
    return "Student";
}

// Load events from file
vector<Event> Student::loadEventsFromFile() {
    vector<Event> events;
    ifstream file("data/events.txt");
    string line;
    
    if (!file.is_open()) {
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
string Student::getCurrentDateTime() {
    time_t now = time(0);
    tm* timeinfo = localtime(&now);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M", timeinfo);
    return string(buffer);
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
         << "AVL" << endl;
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

// Register for an event
void Student::registerForEvent() {
    vector<Event> events = loadEventsFromFile();
    vector<Registration> registrations = loadRegistrationsFromFile();
    
    if (events.empty()) {
        cout << "No events available!" << endl;
        return;
    }
    
    viewAvailableEvents();
    
    cout << "\nEnter event number to register (0 to cancel): ";
    int eventNum;
    cin >> eventNum;
    cin.ignore();
    
    if (eventNum < 1 || eventNum > (int)events.size()) {
        cout << "Invalid selection!" << endl;
        return;
    }
    
    Event& selectedEvent = events[eventNum - 1];
    string eventName = selectedEvent.getEventName();
    
    // Check if already registered
    for (const auto& reg : registrations) {
        if (reg.getStudentUsername() == username && reg.getEventName() == eventName) {
            cout << "Error: You are already registered for this event!" << endl;
            return;
        }
    }
    
    // Check if event has capacity
    if (!selectedEvent.hasAvailableSeats()) {
        cout << "Error: Event is full! No available seats." << endl;
        return;
    }
    
    // Register student
    selectedEvent.registerStudent();
    registrations.push_back(Registration(username, eventName, getCurrentDateTime()));
    
    if (saveEventsToFile(events) && saveRegistrationsToFile(registrations)) {
        cout << "\nSuccess! You have been registered for '" << eventName << "'!" << endl;
    } else {
        cout << "Error: Registration failed!" << endl;
    }
}

// Unregister from an event
void Student::unregisterFromEvent(const string& eventName) {
    vector<Event> events = loadEventsFromFile();
    vector<Registration> registrations = loadRegistrationsFromFile();
    
    // Find and remove registration
    auto it = find_if(registrations.begin(), registrations.end(),
        [this, &eventName](const Registration& r) {
            return r.getStudentUsername() == username && r.getEventName() == eventName;
        });
    
    if (it == registrations.end()) {
        cout << "Error: Registration not found!" << endl;
        return;
    }
    
    registrations.erase(it);
    
    // Find event and decrease registered count
    for (auto& event : events) {
        if (event.getEventName() == eventName) {
            event.unregisterStudent();
            break;
        }
    }
    
    if (saveEventsToFile(events) && saveRegistrationsToFile(registrations)) {
        cout << "Success! You have been unregistered from '" << eventName << "'!" << endl;
    } else {
        cout << "Error: Unregistration failed!" << endl;
    }
}

// Search events by name
void Student::searchEventByName() {
    vector<Event> events = loadEventsFromFile();
    
    cout << "\n=== SEARCH EVENTS ===" << endl;
    cout << "Enter event name to search: ";
    
    string searchTerm;
    getline(cin, searchTerm);
    searchTerm = toLower(trim(searchTerm));
    
    if (searchTerm.empty()) {
        cout << "Search term cannot be empty!" << endl;
        return;
    }
    
    vector<Event> results;
    for (const auto& event : events) {
        if (toLower(event.getEventName()).find(searchTerm) != string::npos) {
            results.push_back(event);
        }
    }
    
    if (results.empty()) {
        cout << "No events found matching '" << searchTerm << "'!" << endl;
        return;
    }
    
    cout << "\n=== SEARCH RESULTS ===" << endl;
    cout << "\n" << string(110, '=') << endl;
    cout << "  " << left << setw(2) << "#" << "  "
         << setw(25) << "EVENT NAME" << " | "
         << setw(12) << "DATE" << " | "
         << setw(20) << "VENUE" << " | "
         << setw(4) << "CAP" << " | "
         << setw(4) << "REG" << " | "
         << "AVL" << endl;
    cout << string(110, '=') << endl;
    
    for (size_t i = 0; i < results.size(); i++) {
        results[i].display(i + 1);
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
         << "AVL" << endl;
    cout << string(110, '=') << endl;
    
    for (size_t i = 0; i < results.size(); i++) {
        results[i].display(i + 1);
    }
    cout << string(110, '=') << endl;
    cout << "Found " << results.size() << " event(s)" << endl << endl;
}
