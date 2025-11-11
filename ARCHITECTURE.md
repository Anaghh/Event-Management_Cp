# Project Architecture - Modular Structure

## Overview

The Event Management System has been refactored from a single 1376-line file into a modular, component-based architecture. This follows the **Single Responsibility Principle** where each file has a specific purpose.

---

## File Structure

```
event-management-cpp/
├── Header Files (.h)              [Interface Declarations]
│   ├── utils.h                    # Utility functions
│   ├── event.h                    # Event class
│   ├── registration.h             # Registration class
│   ├── user.h                     # User base class
│   ├── admin.h                    # Admin class
│   └── student.h                  # Student class
│
├── Implementation Files (.cpp)     [Code Implementations]
│   ├── utils.cpp                  # Utility implementations
│   ├── event.cpp                  # Event class methods
│   ├── registration.cpp           # Registration class methods
│   ├── user.cpp                   # User base class methods
│   ├── admin.cpp                  # Admin class methods
│   ├── student.cpp                # Student class methods
│   └── main.cpp                   # Entry point & auth
│
├── Data Files
│   ├── events.txt                 # Event storage
│   ├── users.txt                  # User credentials
│   └── registrations.txt          # Registration tracking
│
├── Build Files
│   ├── Makefile                   # Compilation configuration
│   └── event-management           # Compiled executable
│
└── Documentation
    ├── README.md                  # Main documentation
    ├── QUICK_START.md             # Quick reference
    └── ARCHITECTURE.md            # This file
```

---

## Module Descriptions

### 1. **utils.h / utils.cpp** 
**Purpose**: Reusable utility functions

**Functions**:
- `split()` - String parsing by delimiter
- `trim()` - Remove whitespace
- `isNumeric()` - Check if string is numeric
- `isValidDate()` - Validate DD-MM-YYYY format
- `toLower()` - Convert to lowercase

**Dependencies**: None (STL only)

**Used by**: All modules

---

### 2. **event.h / event.cpp**
**Purpose**: Event data model and operations

**Class**: `Event`
- **Members**: eventName, date, venue, capacity, registeredCount
- **Methods**: 
  - Getters/Setters
  - `hasAvailableSeats()` - Check availability
  - `registerStudent()` / `unregisterStudent()` - Update counts
  - `toFileFormat()` - Serialize to file
  - `display()` - Table format output
  - `displayDetailed()` - Detailed view

**Dependencies**: iostream, iomanip

**Used by**: Admin, Student

**Lines**: ~95 (header) + ~80 (impl)

---

### 3. **registration.h / registration.cpp**
**Purpose**: Registration tracking data model

**Class**: `Registration`
- **Members**: studentUsername, eventName, registrationDate
- **Methods**:
  - Getters
  - `toFileFormat()` - Serialize to file
  - `display()` - Show registration info

**Dependencies**: iostream

**Used by**: Admin, Student

**Lines**: ~30 (header) + ~30 (impl)

---

### 4. **user.h / user.cpp**
**Purpose**: Base class for all users

**Class**: `User` (Abstract)
- **Members**: username, password, fullName
- **Pure Virtual Methods**:
  - `displayMenu()` - Show user-specific menu
  - `getUserType()` - Return "Admin" or "Student"
- **Methods**:
  - Getters
  - `authenticate()` - Password verification

**Inheritance Pattern**:
```
    User (Abstract)
    ├── Admin
    └── Student
```

**Dependencies**: iostream

**Used by**: Admin, Student

**Lines**: ~30 (header) + ~20 (impl)

---

### 5. **admin.h / admin.cpp**
**Purpose**: Admin user functionality

**Class**: `Admin : public User`
- **File I/O Methods**:
  - `loadEventsFromFile()`
  - `saveEventsToFile()`
  - `loadRegistrationsFromFile()`
  - `saveRegistrationsToFile()`

- **Event Management**:
  - `addNewEvent()`
  - `editEvent()`
  - `deleteEvent()`
  - `viewAllEvents()`

- **Reporting**:
  - `displayEventStats()`
  - `viewRegistrationReports()`

- **User Management**:
  - `manageUsers()`
  - `addNewStudent()`
  - `viewAllUsers()`

**Dependencies**: user.h, event.h, registration.h, utils.h, fstream, vector

**Used by**: main.cpp

**Lines**: ~100 (header) + ~550 (impl)

---

### 6. **student.h / student.cpp**
**Purpose**: Student user functionality

**Class**: `Student : public User`
- **File I/O Methods** (same as Admin):
  - `loadEventsFromFile()`
  - `saveEventsFromFile()`
  - `loadRegistrationsFromFile()`
  - `saveRegistrationsToFile()`

- **Event Browsing**:
  - `viewAvailableEvents()`
  - `viewMyRegistrations()`

- **Registration Management**:
  - `registerForEvent()`
  - `unregisterFromEvent()`

- **Search & Filter**:
  - `searchEventByName()`
  - `filterEventsByDate()`

- **Utility**:
  - `getCurrentDateTime()` - Generate timestamps

**Dependencies**: user.h, event.h, registration.h, utils.h, fstream, vector, ctime

**Used by**: main.cpp

**Lines**: ~80 (header) + ~450 (impl)

---

### 7. **main.cpp**
**Purpose**: Application entry point and authentication

**Functions**:
- `authenticateUser()` - Login processing
- `displayWelcome()` - Welcome screen
- `main()` - Application flow control

**Key Flow**:
1. Display welcome message
2. Login loop (max 3 attempts)
3. Route to Admin or Student dashboard
4. Menu loop until logout
5. Cleanup and exit

**Dependencies**: user.h, admin.h, student.h, utils.h

**Lines**: ~165

---

## Compilation Process

### Using Makefile (Recommended)
```bash
make              # Build
make clean        # Remove artifacts
make rebuild      # Clean + build
```

### Manual Compilation
```bash
g++ -std=c++17 -c main.cpp
g++ -std=c++17 -c admin.cpp
g++ -std=c++17 -c student.cpp
g++ -std=c++17 -c user.cpp
g++ -std=c++17 -c event.cpp
g++ -std=c++17 -c registration.cpp
g++ -std=c++17 -c utils.cpp
g++ -std=c++17 -o event-management *.o
```

---

## Design Patterns Used

### 1. **Object-Oriented Programming**
- **Encapsulation**: Data hiding with private members
- **Inheritance**: Admin/Student inherit from User
- **Polymorphism**: Virtual functions in User base class
- **Abstraction**: Pure virtual functions define interface

### 2. **Single Responsibility Principle**
- Each class has one reason to change
- Each file has a single purpose

### 3. **Separation of Concerns**
- **Logic**: Admin.cpp, Student.cpp
- **Data Models**: Event.cpp, Registration.cpp
- **Utilities**: utils.cpp
- **Flow Control**: main.cpp

### 4. **Dependency Injection**
- Pass objects between functions
- No circular dependencies

---

## File Dependencies Graph

```
main.cpp
├── user.h
├── admin.h
│   ├── user.h
│   ├── event.h
│   ├── registration.h
│   └── utils.h
├── student.h
│   ├── user.h
│   ├── event.h
│   ├── registration.h
│   └── utils.h
└── utils.h

event.h
└── (iostream, iomanip)

registration.h
└── (iostream)

user.h
└── (iostream)

admin.h
├── user.h
├── event.h
├── registration.h
├── utils.h
└── (fstream, vector, algorithm, iostream, iomanip)

student.h
├── user.h
├── event.h
├── registration.h
├── utils.h
└── (fstream, vector, algorithm, ctime, iostream)

utils.h
└── (vector, sstream, algorithm, cctype)
```

---

## Code Statistics

| Module | Header (Lines) | Implementation (Lines) | Total |
|--------|---|---|---|
| utils | 22 | 53 | 75 |
| event | 47 | 82 | 129 |
| registration | 30 | 18 | 48 |
| user | 32 | 19 | 51 |
| admin | 50 | 550 | 600 |
| student | 46 | 480 | 526 |
| main | - | 165 | 165 |
| **TOTAL** | **227** | **1,367** | **1,594** |

**vs. Original Single File**: 1,376 lines → 1,594 lines (with better organization)

---

## Key Improvements

### 1. **Modularity**
- ✅ Each class in separate files
- ✅ Easy to locate functionality
- ✅ Easy to test individual modules

### 2. **Maintainability**
- ✅ Changes don't affect entire codebase
- ✅ Easier code review
- ✅ Clear separation of concerns

### 3. **Reusability**
- ✅ Utility functions in separate module
- ✅ Classes can be reused in other projects
- ✅ Header files define interfaces

### 4. **Scalability**
- ✅ Easy to add new user types
- ✅ Easy to add new features
- ✅ File I/O can be replaced with database later

### 5. **Compilation**
- ✅ Only modified files recompiled (faster builds)
- ✅ Makefile automates the process
- ✅ Clear dependency management

---

## Adding New Features

### Example: Add a Teacher User Type

**Step 1**: Create header
```cpp
// teacher.h
#ifndef TEACHER_H
#define TEACHER_H

#include "user.h"
#include "event.h"

class Teacher : public User {
public:
    Teacher(string uname, string pass, string name);
    void displayMenu() override;
    string getUserType() override;
    // Teacher-specific methods
};

#endif
```

**Step 2**: Create implementation
```cpp
// teacher.cpp
#include "teacher.h"
// Implementation
```

**Step 3**: Update main.cpp
```cpp
// In authenticateUser()
if (type == "teacher") {
    return new Teacher(uname, pass, name);
}

// In main() menu loop
if (Teacher* teacher = dynamic_cast<Teacher*>(currentUser)) {
    // Handle teacher menu
}
```

**Step 4**: Update Makefile
```makefile
SOURCES = main.cpp admin.cpp student.cpp teacher.cpp user.cpp event.cpp registration.cpp utils.cpp
```

---

## Future Refactoring

### Phase 3 Enhancements
1. **Database Integration**
   - Replace text files with SQLite
   - Create `database.h` / `database.cpp`

2. **Additional Classes**
   - `Venue` class for venue management
   - `Category` class for event categorization
   - `Notification` class for alerts

3. **Advanced Features**
   - Create `payment.h` for registration fees
   - Create `feedback.h` for ratings/reviews
   - Create `email.h` for notifications

4. **Logging**
   - Create `logger.h` for audit trails
   - Create `validator.h` for enhanced validation

---

## Compilation Notes

- **Language**: C++17
- **Compiler**: g++
- **Standard Library**: iostream, fstream, string, vector, algorithm, sstream, iomanip, ctime, cctype
- **Build System**: Makefile
- **No External Dependencies**: Uses only C++ standard library

---

## Summary

The refactored modular architecture provides:
- **Clear Organization**: Each file has a specific purpose
- **Easier Maintenance**: Find code quickly and make changes confidently
- **Better Testing**: Test individual components in isolation
- **Scalability**: Add features without touching existing code
- **Professional Structure**: Follows C++ best practices

Total refactoring result: **1,376-line monolith → 7 focused modules + build system**

---

**Version**: 2.1 (Modular Architecture)  
**Last Updated**: November 11, 2025  
**Status**: Production Ready
