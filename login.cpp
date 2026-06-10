/*#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

// Function to check if username is valid
bool isValidUsername(string username) {
    if (username.empty()) {
        return false;
    }

    for (char ch : username) {
        if (!isalnum(ch) && ch != '_') {
            return false;
        }
    }

    return true;
}

// Function to check password validity
bool isValidPassword(string password) {
    return password.length() >= 6;
}

// Registration Function
void registerUser() {
    string username, password;

    cout << "\n===== User Registration =====\n";

    cout << "Enter Username: ";
    cin >> username;

    // Username validation
    if (!isValidUsername(username)) {
        cout << "Error: Username can contain only letters, digits, and underscores.\n";
        return;
    }

    // Check if username already exists
    string filename = username + ".txt";

    ifstream checkFile(filename);

    if (checkFile.is_open()) {
        cout << "Error: Username already exists.\n";
        checkFile.close();
        return;
    }

    cout << "Enter Password (minimum 6 characters): ";
    cin >> password;

    // Password validation
    if (!isValidPassword(password)) {
        cout << "Error: Password must be at least 6 characters long.\n";
        return;
    }

    // Store credentials
    ofstream outFile(filename);

    if (!outFile) {
        cout << "Error: Unable to create user file.\n";
        return;
    }

    outFile << username << endl;
    outFile << password << endl;

    outFile.close();

    cout << "Registration Successful!\n";
}

// Login Function
void loginUser() {
    string username, password;
    string storedUsername, storedPassword;

    cout << "\n===== User Login =====\n";

    cout << "Enter Username: ";
    cin >> username;

    cout << "Enter Password: ";
    cin >> password;

    string filename = username + ".txt";

    ifstream inFile(filename);

    if (!inFile) {
        cout << "Error: User does not exist.\n";
        return;
    }

    getline(inFile, storedUsername);
    getline(inFile, storedPassword);

    inFile.close();

    if (username == storedUsername && password == storedPassword) {
        cout << "Login Successful! Welcome, " << username << ".\n";
    } else {
        cout << "Error: Invalid username or password.\n";
    }
}

// Main Function
int main() {
    int choice;

    do {
        cout << "\n===== Login and Registration System =====\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                registerUser();
                break;

            case 2:
                loginUser();
                break;

            case 3:
                cout << "Exiting program...\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 3);

    return 0;
}
    */
   #include <iostream>
#include <fstream>
#include <string>
 
using namespace std;
 
const string DB_FILE = "users.txt";
 
// Check if username already exists
bool userExists(const string& username) {
    ifstream file(DB_FILE);
    string line;
    while (getline(file, line)) {
        string storedUser = line.substr(0, line.find(','));
        if (storedUser == username)
            return true;
    }
    return false;
}
 
// Register a new user
void registerUser() {
    string username, password;
 
    cout << "\n-- Register --\n";
    cout << "Enter username: ";
    cin >> username;
 
    // Basic validation
    if (username.length() < 3) {
        cout << "Error: Username must be at least 3 characters.\n";
        return;
    }
 
    if (userExists(username)) {
        cout << "Error: Username already taken.\n";
        return;
    }
 
    cout << "Enter password: ";
    cin >> password;
 
    if (password.length() < 6) {
        cout << "Error: Password must be at least 6 characters.\n";
        return;
    }
 
    // Save to file as: username,password
    ofstream file(DB_FILE, ios::app);
    file << username << "," << password << "\n";
 
    cout << "Registration successful! You can now log in.\n";
}
 
// Login an existing user
void loginUser() {
    string username, password;
 
    cout << "\n-- Login --\n";
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
 
    ifstream file(DB_FILE);
    string line;
 
    while (getline(file, line)) {
        string storedUser = line.substr(0, line.find(','));
        string storedPass = line.substr(line.find(',') + 1);
 
        if (storedUser == username && storedPass == password) {
            cout << "Login successful! Welcome, " << username << "!\n";
            return;
        }
    }
 
    cout << "Error: Invalid username or password.\n";
}
 
int main() {
    int choice;
 
    cout << "=== Login & Registration System ===\n";
 
    while (true) {
        cout << "\n1. Register\n2. Login\n3. Exit\nChoice: ";
        cin >> choice;
 
        if      (choice == 1) registerUser();
        else if (choice == 2) loginUser();
        else if (choice == 3) { cout << "Goodbye!\n"; break; }
        else                   cout << "Invalid option.\n";
    }
 
    return 0;
}