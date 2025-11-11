# Event Management System - C++

A comprehensive event management system demonstrating Object-Oriented Programming (OOP) principles in C++. This application allows administrators to manage events and students to register for events.

## Phase 2 - Complete Implementation ✅

### Project Structure

```
event-management-cpp/
├── event-management.cpp    # Complete implementation (single file)
├── events.txt             # Event data storage
├── users.txt              # User credentials and profile
├── registrations.txt      # Student event registrations
└── README.md              # Documentation
```

### File Format Specifications

#### events.txt (Pipe-delimited)
```
EventName|DD-MM-YYYY|Venue|Capacity|RegisteredCount
Tech Fest 2025|15-03-2025|Main Auditorium|200|1
```

#### registrations.txt (Pipe-delimited)
```
StudentUsername|EventName|DD-MM-YYYY HH:MM
john|Tech Fest 2025|11-11-2025 14:30
```

#### users.txt (Comma-separated)
```
username,password,fullname,usertype
admin,admin123,System Administrator,admin
john,pass123,John Smith,student
```

---

## Features Implemented

### Phase 1 - Core Features ✅
- User authentication with role-based access control
- Admin and Student user roles
- File-based data storage
- Basic event viewing
- OOP principles (Inheritance, Polymorphism, Encapsulation, Abstraction)

### Phase 2 - Advanced Features ✅

#### CRUD Operations for Events
- **Create**: Add new events with validation
- **Read**: View all events or search/filter
- **Update**: Edit event details (date, venue, capacity)
- **Delete**: Remove events and associated registrations

#### Event Management
- Add new events with automatic duplicate prevention
- Edit event properties (name, date, venue, capacity)
- Delete events with cascade delete for registrations
- Capacity validation to prevent double-booking
- Real-time registration count updates

#### Registration System
- Students can register for available events
- Automatic duplicate registration prevention
- Seat availability checking
- Student unregistration with automatic count decrement
- Registration timestamp tracking

#### Student Features
- **Browse Events**: View all available events with availability
- **My Registrations**: View personal registered events
- **Event Search**: 
  - Search by event name (case-insensitive)
  - Filter events by date
- **Register/Unregister**: Easy registration management
- **Event Details**: View detailed event information

#### Admin Features
- **Event Management Menu**: Create, edit, delete events
- **Event Statistics**: View system-wide statistics
  - Total events, capacity, registrations
  - Per-event occupancy percentages
- **Registration Reports**:
  - View all participants for specific events
  - Registration summary across all events
  - Participant lists with registration dates
- **User Management**:
  - Add new student accounts
  - View all system users
  - Automatic username duplication prevention

#### Data Validation
- Date format validation (DD-MM-YYYY)
- Positive capacity checking
- Non-empty string validation
- Username/Event name uniqueness checking
- Duplicate registration prevention
- Capacity constraints enforcement

#### Error Handling
- File operation error handling
- Input validation with user feedback
- Graceful error recovery
- Informative error messages

---

## OOP Concepts Demonstrated

### 1. **Inheritance**
- `User` base class with `Admin` and `Student` derived classes
- Shared functionality in base class
- Specialized behavior in derived classes

### 2. **Polymorphism**
- Virtual functions: `displayMenu()`, `getUserType()`
- Dynamic casting for user-type specific operations
- Function overriding in derived classes

### 3. **Encapsulation**
- Protected class members
- Public getter methods
- Private data in classes

### 4. **Abstraction**
- Pure virtual functions in base class
- Complex operations hidden from user
- Simple user interfaces for complex functionality

### 5. **Additional Patterns**
- `Event` and `Registration` classes for data management
- Utility functions for string operations
- Vector containers for dynamic data storage

---

## Compilation and Execution

### Compile
```bash
g++ -std=c++17 -o event-management event-management.cpp
```

### Run
```bash
./event-management
```

### Default Test Credentials
```
Admin:
  Username: admin
  Password: admin123

Students:
  Username: john     | Password: pass123
  Username: alice    | Password: alice456
  Username: bob      | Password: bob789
```

---

## Usage Guide

### Admin Menu
1. **Manage Events** - Create, edit, or delete events
2. **View All Events** - See complete event list with details
3. **View Event Statistics** - System-wide statistics and occupancy
4. **View Registration Reports** - Participant lists and registrations
5. **Manage Users** - Add new students or view all users
6. **Logout** - Exit the system

### Student Menu
1. **Browse Available Events** - View all events and optionally register
2. **My Registrations** - View, see details, or unregister from events
3. **Search Events** - Search by name or filter by date
4. **Logout** - Exit the system

---

## Code Architecture

### Main Components

#### Utility Functions
- `split()` - String parsing by delimiter
- `trim()` - Whitespace removal
- `isNumeric()` - Numeric validation
- `isValidDate()` - Date format validation
- `toLower()` - String case conversion

#### Event Class
- Stores event details and registration info
- Methods for availability checking
- File format conversion
- Display methods with formatting

#### Registration Class
- Tracks student-event registrations
- Stores registration timestamps
- File format conversion

#### User Hierarchy
- `User` - Abstract base class
- `Admin` - Event and user management
- `Student` - Event browsing and registration

#### Core Functions
- `authenticateUser()` - Login processing
- `main()` - Application flow control

---

## Key Implementation Details

### File I/O Strategy
- Load entire files into memory (vectors)
- Modify in-memory structures
- Write complete files back to disk
- Atomic operations (prevent partial writes)

### Date-Time Handling
- Date format: DD-MM-YYYY
- Registration timestamp: DD-MM-YYYY HH:MM
- Current system time for registration dates

### Validation Rules
1. **Event Names**: Non-empty, unique (case-insensitive)
2. **Dates**: DD-MM-YYYY format, year >= 2025
3. **Capacity**: Positive integer
4. **Username**: Unique, non-empty
5. **Registrations**: One per student per event, respects capacity

### Memory Management
- Dynamic allocation for User objects
- Proper cleanup with delete statements
- No memory leaks in normal operation

---

## Future Enhancements (Phase 3)

- Header file separation (.h and .cpp files)
- Database integration (SQLite/MySQL)
- Event categories/tags
- Rating and review system
- Email notifications
- Seat preference selection
- Event cancellation with notification
- Admin approval workflow
- Dashboard analytics
- CSV export functionality

---

## Technical Specifications

- **Language**: C++17
- **Compilation**: g++ with C++17 standard
- **Standard Library**: iostream, fstream, string, vector, algorithm, sstream, iomanip, ctime
- **File Format**: Plain text (pipe and comma-delimited)
- **Data Persistence**: Text-based file storage
- **User Interface**: Terminal/Console

---

## Testing Checklist

- ✅ Admin can create events
- ✅ Admin can edit event details
- ✅ Admin can delete events
- ✅ Student can register for events
- ✅ Student can unregister from events
- ✅ Capacity constraints are enforced
- ✅ Duplicate registrations are prevented
- ✅ Search and filter functionality works
- ✅ Registration reports generate correctly
- ✅ New user creation works
- ✅ Data persists across sessions
- ✅ Error handling is comprehensive

---

## Notes

- All data is stored in plain text files in the same directory as the executable
- Registration timestamps are automatically generated
- The system supports unlimited events and users
- Capacity can be changed if current registrations allow it
- Deleting an event removes all associated registrations

---

**Version**: 2.0 (Phase 2 Complete)  
**Last Updated**: November 11, 2025  
**Status**: Ready for Production
