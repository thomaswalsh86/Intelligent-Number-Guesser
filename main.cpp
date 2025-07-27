#include <algorithm>
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <string>
#include <cerrno>  // Add this at the top for `errno`
#include <cstring>
#include <cstdlib>

using namespace std;

// Constants
const int NUMBER_RANGE = 3000;
const int TRIALS = 3000;
const int MIN_VALUE = 1;
const int MAX_VALUE = NUMBER_RANGE;
const int ERROR_VALUE = NUMBER_RANGE + 1;

// Function declarations
int linear(int num);
int randSampling(int num);
int quadProb(int num);
void send(const string& filename, const string& strat, int num, int guesses, chrono::milliseconds duration, bool success);

int main() {
    cout << "Hello! This is a Guessing Game.\n";

    int choice1;
    int choice2;

    cout << "Press (1) to run one method or (2) to run all:\n";
    cin >> choice1;

    if (choice1 == 1) {
        cout << "Press (1) for Random Sampling, (2) for Linear, or (3) for Quadratic Probability:\n";
        cin >> choice2;

        cout << "Select a number between 1 and " << NUMBER_RANGE << " and I will guess it:\n";

        int num;
        cin >> num;

        while (num < MIN_VALUE || num > MAX_VALUE) {
            cout << "Please enter a number between " << MIN_VALUE << " and " << MAX_VALUE << ":\n";
            cin >> num;
        }
        switch (choice2) {
            case 1:
                randSampling(num);
                break;
            case 2:
                linear(num);
                break;
            case 3:
                quadProb(num);
                break;
            default:
                cout << "Invalid input. Exiting.\n";
                return 1;
        }
    }
    else {
        int successes = 0;
        int total_guesses_rand = 0;
        int total_guesses_linear = 0;
        int total_guesses_quad = 0;

        // Initialize random number generator
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(MIN_VALUE, MAX_VALUE);

        for (int i = 1; i <= TRIALS; ++i) {
            int target = dist(gen);

            int rand_result = randSampling(target);
            int linear_result = linear(target);
            int quad_result = quadProb(target);

            if (rand_result == ERROR_VALUE || linear_result == ERROR_VALUE || quad_result == ERROR_VALUE) {
                cout << "Guess not found in trial " << i << ".\n";
            }

            successes++;
            total_guesses_rand += rand_result;
            total_guesses_linear += linear_result;
            total_guesses_quad += quad_result;
        }

        cout << "\nResults after " << TRIALS << " trials:\n";
        cout << "Successes: " << successes << " (" << (successes * 100.0 / TRIALS) << "%)\n";
        cout << "Average guesses:\n";
        cout << "  Random Sampling: " << (total_guesses_rand * 1.0 / successes) << "\n";
        cout << "  Linear Search: " << (total_guesses_linear * 1.0 / successes) << "\n";
        cout << "  Quadratic Probing: " << (total_guesses_quad * 1.0 / successes) << "\n";
    }

    return 0;
}

int randSampling(int num) {
    vector<int> numbers;
    auto start = chrono::high_resolution_clock::now();

    for (int i = MIN_VALUE; i <= MAX_VALUE; ++i) {
        numbers.push_back(i);
    }

    auto rng = default_random_engine(random_device{}());
    shuffle(numbers.begin(), numbers.end(), rng);

    int guesses = 0;
    for (int k : numbers) {
        guesses++;
        if (k == num) {
            cout << "Using Random Sampling: Found " << k << " in " << guesses << " guesses.\n";
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
            send("C:\\Users\\Thetj\\CLionProjects\\GuessingGame\\stats\\gameResults.csv", "randSampling", num, guesses, duration, true);
            return guesses;
        }
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    send("C:\\Users\\Thetj\\CLionProjects\\GuessingGame\\stats\\gameResults.csv", "randSampling", num, ERROR_VALUE, duration, false);
    return ERROR_VALUE;
}

int linear(int num) {
    auto start = chrono::high_resolution_clock::now();
    int guesses = 0;

    for (int i = MIN_VALUE; i <= MAX_VALUE; ++i) {
        guesses++;
        if (i == num) {
            cout << "Using Linear Search: Found " << i << " in " << guesses << " guesses.\n";
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
            send("C:\\Users\\Thetj\\CLionProjects\\GuessingGame\\stats\\gameResults.csv", "linear", num, guesses, duration, true);
            return guesses;
        }
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    send("C:\\Users\\Thetj\\CLionProjects\\GuessingGame\\stats\\gameResults.csv", "linear", num, ERROR_VALUE, duration, false);
    return ERROR_VALUE;
}

int quadProb(int num) {
    auto st = chrono::high_resolution_clock::now();
    int guesses = 0;
    int i = 0;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(MIN_VALUE, MAX_VALUE);
    int start = dist(gen);

    while (guesses < NUMBER_RANGE) {
        int guess = (start + i * i) % NUMBER_RANGE;
        guess = (guess == 0) ? NUMBER_RANGE : guess;

        guesses++;
        if (guess == num) {
            cout << "Using Quadratic Probing: Found " << guess << " in " << guesses << " guesses.\n";
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(end - st);
            send("C:\\Users\\Thetj\\CLionProjects\\GuessingGame\\stats\\gameResults.csv", "quadProb", num, guesses, duration, true);
            return guesses;
        }
        i++;
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - st);
    send("C:\\Users\\Thetj\\CLionProjects\\GuessingGame\\stats\\gameResults.csv", "quadProb", num, ERROR_VALUE, duration, false);
    return ERROR_VALUE;
}

void send(const string& filename, const string& strat, int num, int guesses, chrono::milliseconds duration, bool success) {
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cerr << "ERROR: Could not open file '" << filename << "' for writing.\n";
        cerr << "Reason: " << strerror(errno) << "\n";
        return;
    }

    file << strat << ","
         << num << ","
         << guesses << ","
         << duration.count() << ","
         << (success ? "true" : "false") << "\n";  // Write success status

    if (file.fail()) {
        cerr << "ERROR: Failed to write to file.\n";
    }

    file.close();
}