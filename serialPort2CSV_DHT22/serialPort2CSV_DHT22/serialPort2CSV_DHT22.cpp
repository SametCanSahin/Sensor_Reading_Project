#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>

using namespace std;

// --- TEST RANGES (Celsius) ---
const float COLD_MIN = -10.0, COLD_MAX = 15.0; // e.g., Fridge or Outdoor
const float ROOM_MIN = 18.0, ROOM_MAX = 28.0; // e.g., Office/Home
const float HOT_MIN = 30.0, HOT_MAX = 150.0; // e.g., Near Heater

// Function to read from Serial Port
string ReadSerialLine(HANDLE hSerial) {
    string result = "";
    char c;
    DWORD bytesRead;

    while (true) {
        if (ReadFile(hSerial, &c, 1, &bytesRead, NULL)) {
            if (bytesRead > 0) {
                if (c == '\n') break;
                if (c != '\r') result += c;
            }
        }
        else {
            return "";
        }
    }
    return result;
}

// Function to get Date and Time
string GetTimestamp() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[80];
    // Format: YYYY-MM-DD,HH:MM:SS (Comma separates date and time for CSV columns)
    strftime(buffer, 80, "%Y-%m-%d,%H:%M:%S", ltm);
    return string(buffer);
}

int main() {
    // --- SETTINGS ---
    const char* portName = "COM8";
    // ----------------

    // --- USER TEST SELECTION ---
    int choice;
    float minLimit, maxLimit;
    string fileName, testName;

    cout << "========================================" << endl;
    cout << "   DHT22 REPEATABILITY TEST SETUP" << endl;
    cout << "========================================" << endl;
    cout << "Please select the test scenario:" << endl;
    cout << "1. Cold Environment Test (" << COLD_MIN << " - " << COLD_MAX << " C)" << endl;
    cout << "2. Room Temperature Test (" << ROOM_MIN << " - " << ROOM_MAX << " C)" << endl;
    cout << "3. Hot Environment Test  (" << HOT_MIN << " - " << HOT_MAX << " C)" << endl;
    cout << "Your choice (1-3): ";
    cin >> choice;

    switch (choice) {
    case 1:
        minLimit = COLD_MIN; maxLimit = COLD_MAX;
        fileName = "super_cold_test_data.csv";
        testName = "COLD";
        break;
    case 2:
        minLimit = ROOM_MIN; maxLimit = ROOM_MAX;
        fileName = "room_test_data.csv";
        testName = "ROOM";
        break;
    case 3:
        minLimit = HOT_MIN; maxLimit = HOT_MAX;
        fileName = "hot_test_data.csv";
        testName = "HOT";
        break;
    default:
        cout << "Invalid choice! Exiting program." << endl;
        return 1;
    }

    cout << "\n[" << testName << "] Test is starting..." << endl;
    cout << "Target Range: " << minLimit << "C to " << maxLimit << "C." << endl;
    cout << "Data will be saved to '" << fileName << "'\n" << endl;

    // --- SERIAL PORT CONNECTION ---
    HANDLE hSerial = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (hSerial == INVALID_HANDLE_VALUE) {
        cerr << "Error: Could not open port! (" << portName << ")" << endl;
        cerr << "Check if Arduino is connected or Serial Monitor is open elsewhere." << endl;
        return 1;
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    GetCommState(hSerial, &dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    SetCommState(hSerial, &dcbSerialParams);

    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    SetCommTimeouts(hSerial, &timeouts);

    // --- FILE OPERATIONS ---
    ofstream myFile;
    myFile.open(fileName, ios::out | ios::app);

    if (!myFile.is_open()) {
        cerr << "Error: Could not create file." << endl;
        CloseHandle(hSerial);
        return 1;
    }

    myFile.seekp(0, ios::end);
    if (myFile.tellp() == 0) {

        myFile << "Date,Time,Temperature_C,Test_Type,Status" << endl;
    }

    cout << "Reading data... (Close window or Ctrl+C to stop)" << endl;
    cout << "------------------------------------------------" << endl;

    while (true) {
        string tempStr = ReadSerialLine(hSerial);

        if (!tempStr.empty()) {
            try {
                // Convert string to float for validation
                float tempVal = stof(tempStr);
                string timeStr = GetTimestamp();
                string status = "OK";

                // Range Check
                bool rangeError = false;
                if (tempVal < minLimit || tempVal > maxLimit) {
                    status = "OUT_OF_RANGE";
                    rangeError = true;
                }

                // Console Output
                cout << "[" << timeStr << "] -> " << tempVal << " C ";
                if (rangeError) {
                    cout << " <--- [WARNING] Out of target range!";
                }
                cout << endl;

                // Save to File
                myFile << timeStr << "," << tempVal << "," << testName << "," << status << endl;
                myFile.flush();

            }
            catch (...) {
                cerr << "Data conversion error (Received bad data)" << endl;
            }
        }
        Sleep(100); // Small delay
    }

    myFile.close();
    CloseHandle(hSerial);
    return 0;
}