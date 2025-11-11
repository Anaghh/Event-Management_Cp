#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

// ==================== UTILITY FUNCTIONS ====================

// Split string by delimiter
vector<string> split(const string& str, char delimiter);

// Trim whitespace from string
string trim(const string& str);

// Check if string contains only digits
bool isNumeric(const string& str);

// Validate date format (DD-MM-YYYY)
bool isValidDate(const string& date);

// Convert string to lowercase
string toLower(const string& str);

#endif // UTILS_H
