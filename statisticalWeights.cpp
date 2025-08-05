//
// Created by Thetj on 01/08/2025.
//

#include "statisticalWeights.h"
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include <iostream>
#include <fstream>
#include <cstring>
#include <omp.h>

using namespace std;
float findBias(int num);
bool isPrime(int num);
bool rep(int num);
void writeResults(const unordered_map<int, float>& weights);

int main() {

cout<<"begining..."<<endl;
    std::unordered_map<int, float> weights;
    #pragma omp parallel for
    for (int num = 1; num <= 3000; num++) {
        cout<<num<<": "<<findBias(num)<<endl;
        float bias = findBias(num);
        #pragma omp critical
        weights[num] = bias;
    }
cout<<"printing..."<<endl;
    writeResults(weights);

    return 0;

}
float findBias(int num) {
    float bias = 1.0f; // Neutral base weight

    float magnitude_bias = 5.0f / (1.0f + log(num));
    bias *= magnitude_bias;

    const std::unordered_set<int> major_cultural = {7, 42, 69, 666, 1337, 777, 911, 420, 314, 404};
    const std::unordered_set<int> minor_cultural = {13, 4, 8, 21, 23, 108, 123, 111, 222, 333, 1001, 1984, 2001, 2020, 2023, 1234};

    if (major_cultural.count(num)) {
        return 10.0f;
    } else if (minor_cultural.count(num)) {
        bias *= 5.0f;
    }

    // Rule 3: Round Numbers
    if (num % 10 == 0 || num % 5 == 0 || num % 25 == 0) {
        bias *= 1.5f;
    }

    if (isPrime(num)) bias *= 2.0f;

    if (rep(num)) bias *= 1.5f;
    return bias;
}

bool isPrime(int num) {
    if (num <= 1) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}

bool rep(int num) {
    if (num < 10) return false;

    bool digits[10] = {false};

    while (num > 0) {
        int digit = num % 10;
        if (digits[digit]) {
            return true;
        }

        digits[digit] = true;
        num /= 10;
    }
    return false;
}


void writeResults(const unordered_map<int, float>& weights) {
    const string filename = "C:\\Users\\Thetj\\CLionProjects\\GuessingGame\\stats\\lists.csv";

    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "ERROR: Could not open file '" << filename << "' for writing.\n";
        cerr << "Reason: " << strerror(errno) << "\n";
        return;
    }

    for (const auto& entry : weights) {
        file << entry.first << "-" << entry.second << "\n";
    }

    if (file.fail()) {
        cerr << "ERROR: Failed to write to file.\n";
    }

    file.close();
}