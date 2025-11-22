#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>    
#include <numeric>  
#include <algorithm> 
#include <iomanip>  

using namespace std;

int main() {
    // SETTINGS ---
    string filename = "super_cold_test_data.csv";
    const float REFERENCE_VALUE = 3.8;
    // --------------------------

    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "[ERROR] Could not open file '" << filename << "'" << endl;
        return 1;
    }

    vector<float> temperatures;
    string line;

    cout << "Reading file: " << filename << "..." << endl;

    // Skip header
    getline(file, line);

    // Read line by line
    while (getline(file, line)) {
        stringstream ss(line);
        string segment;
        vector<string> row;

        while (getline(ss, segment, ',')) {
            row.push_back(segment);
        }

        // CSV: Date,Time,Temp,Type,Status -> Temp is index 2
        if (row.size() >= 3) {
            try {
                float temp = stof(row[2]);
                temperatures.push_back(temp);
            }
            catch (...) {
                continue;
            }
        }
    }
    file.close();

    if (temperatures.empty()) {
        cout << "[ERROR] No data found!" << endl;
        return 1;
    }

    // CALCULATIONS ---

    long dataCount = temperatures.size(); // KAÇ DATA OLDUĞU

    double sum = accumulate(temperatures.begin(), temperatures.end(), 0.0);
    double mean = sum / dataCount;

    float minVal = *min_element(temperatures.begin(), temperatures.end());
    float maxVal = *max_element(temperatures.begin(), temperatures.end());

    // Standart Sapma
    double sqSum = 0.0;
    for (float val : temperatures) {
        sqSum += pow(val - mean, 2);
    }
    double variance = sqSum / dataCount;
    double stdDev = sqrt(variance);

    // Doğruluk (Accuracy)
    double absoluteError = abs(mean - REFERENCE_VALUE);
    double accuracyPercent = 100.0 - ((absoluteError / REFERENCE_VALUE) * 100.0);

    // --- RAPOR / REPORT OUTPUT ---
    cout << fixed << setprecision(3);

    cout << "\n================================================" << endl;
    cout << "           TEMPERATURE ANALYSIS            " << endl;
    cout << "================================================" << endl;
    cout << "Target Reference        : " << REFERENCE_VALUE << " C" << endl;
    cout << "Total Data Points Used  : " << dataCount << " samples" << endl;
    cout << "------------------------------------------------" << endl;
    cout << "Measured Average (Mean) : " << mean << " C" << endl;
    cout << "Measured Min            : " << minVal << " C" << endl;
    cout << "Measured Max            : " << maxVal << " C" << endl;
    cout << "Max-Min Span (Fluctuation): " << (maxVal - minVal) << " C" << endl;
    cout << "------------------------------------------------" << endl;

    cout << ">>> FINAL RESULTS <<<" << endl;

    // 1. Repeatability Interpretation
    cout << "1. REPEATABILITY (Standard Deviation): " << stdDev << endl;
    cout << "   Assessment: ";

    if (stdDev < 0.2) {
        cout << "[PERFECT] Lab-grade stability." << endl;
    }
    else if (stdDev < 1.2) {
        cout << "[GOOD] Stable and Acceptable for High Temp." << endl;
    }
    else if (stdDev < 2.5) {
        cout << "[MODERATE] Acceptable but some noise present." << endl;
    }
    else {
        cout << "[UNSTABLE] High fluctuation detected." << endl;
    }

    // 2. Accuracy Interpretation
    cout << "2. ACCURACY (vs 23.8C):" << endl;
    cout << "   Error Rate    : " << absoluteError << " C" << endl;
    cout << "   Accuracy Score: " << accuracyPercent << " %" << endl;

    cout << "\nPress Enter to exit...";
    cin.get();

    return 0;
}